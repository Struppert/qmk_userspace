#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import json
import sys
import time
from dataclasses import dataclass
from typing import List, Optional, Dict, Any

# --------------------------------------------------------------------
# HID-Binding laden + API-Form ermitteln
# --------------------------------------------------------------------
try:
    import hid
except Exception:
    print(
        "Fehler: 'hid' (hidapi) nicht gefunden. Installiere z.B. mit: pip install hidapi",
        file=sys.stderr,
    )
    raise

_HID_HAS_Device = hasattr(hid, "Device")
_HID_HAS_device = hasattr(hid, "device")
if not (_HID_HAS_Device or _HID_HAS_device):
    print(
        "Fehler: Das geladene 'hid'-Modul bietet weder 'Device' noch 'device'.",
        file=sys.stderr,
    )
    sys.exit(1)


def _hid_open_path(path: bytes):
    if _HID_HAS_Device:
        return hid.Device(path=path)
    else:
        d = hid.device()
        d.open_path(path)
        return d


def _hid_open_vid_pid(vid: int, pid: int):
    if _HID_HAS_Device:
        return hid.Device(vid=vid, pid=pid)
    else:
        d = hid.device()
        d.open(vid, pid)
        return d


def _hid_close(dev) -> None:
    try:
        dev.close()
    except Exception:
        pass


def _hid_set_nonblocking(dev, flag: bool) -> None:
    try:
        dev.set_nonblocking(flag)
    except Exception:
        pass


def _hid_send_feature_report(dev, data: bytes) -> int:
    return dev.send_feature_report(data)


def _hid_get_feature_report(dev, report_id: int, size: int) -> bytes:
    return dev.get_feature_report(report_id, size)


def _hid_enumerate():
    return hid.enumerate()


# --------------------------------------------------------------------
# Defaults / VIA-Kommandos
# --------------------------------------------------------------------
DEFAULT_VENDOR_ID = 0x3434
CMD_MACRO_GET_COUNT = 0x05
CMD_MACRO_GET_BUFFER = 0x06
CMD_MACRO_SET_BUFFER = 0x07
PREFERRED_REPORT_SIZES = [64, 32]
PROFILE_A_HEADER_LEN = 4
PROFILE_B_HEADER_LEN = 2


# --------------------------------------------------------------------
# JSON-Modelle
# --------------------------------------------------------------------
@dataclass
class DeviceInfo:
    vendor_id: int
    product_id: Optional[int] = None

    @staticmethod
    def from_json(d: Dict[str, Any]) -> "DeviceInfo":
        def to_int(x):
            if x is None:
                return None
            if isinstance(x, int):
                return x
            s = str(x)
            return int(s, 16) if s.lower().startswith("0x") else int(s)

        return DeviceInfo(
            vendor_id=to_int(d.get("vendor_id", DEFAULT_VENDOR_ID)),
            product_id=to_int(d.get("product_id")),
        )


@dataclass
class MacroEntry:
    slot: int
    text: Optional[str] = None
    bytes_hex: Optional[str] = None

    def to_bytes(self, encoding: str) -> bytes:
        if self.bytes_hex:
            return bytes.fromhex(self.bytes_hex.replace(" ", ""))
        return (self.text or "").encode("utf-8")


@dataclass
class MacroJson:
    device: DeviceInfo
    encoding: str
    macros: List[MacroEntry]
    slots: Optional[int] = None

    @staticmethod
    def from_file(path: str) -> "MacroJson":
        with open(path, "r", encoding="utf-8") as f:
            data = json.load(f)
        dev = DeviceInfo.from_json(data.get("device", {"vendor_id": DEFAULT_VENDOR_ID}))
        enc = data.get("encoding", "text")
        slots = data.get("slots")
        macros: List[MacroEntry] = []
        for m in data.get("macros", []):
            macros.append(
                MacroEntry(
                    slot=int(m["slot"]), text=m.get("text"), bytes_hex=m.get("bytes")
                )
            )
        return MacroJson(device=dev, encoding=enc, macros=macros, slots=slots)

    def to_dict(self) -> Dict[str, Any]:
        d = {
            "device": {"vendor_id": f"0x{self.device.vendor_id:04x}"},
            "encoding": self.encoding,
            "macros": [],
        }
        if self.device.product_id is not None:
            d["device"]["product_id"] = f"0x{self.device.product_id:04x}"
        if self.slots is not None:
            d["slots"] = self.slots
        for m in self.macros:
            mm: Dict[str, Any] = {"slot": m.slot}
            if m.bytes_hex:
                mm["bytes"] = m.bytes_hex
            else:
                mm["text"] = m.text or ""
            d["macros"].append(mm)
        return d

    def save(self, path: str) -> None:
        with open(path, "w", encoding="utf-8") as f:
            json.dump(self.to_dict(), f, ensure_ascii=False, indent=2)


# --------------------------------------------------------------------
# HID Wrapper (mit Fallback)
# --------------------------------------------------------------------
class HidDevice:
    def __init__(
        self,
        vid: int,
        pid: Optional[int] = None,
        report_size: Optional[int] = None,
        debug: bool = False,
        path: Optional[bytes] = None,
    ):
        self.vid = vid
        self.pid = pid
        self.report_size = report_size
        self.debug = debug
        self.path = path
        self.handle = None

    def open(self) -> None:
        # 1) Expliziter Path
        if self.path:
            try:
                self.handle = _hid_open_path(self.path)
                _hid_set_nonblocking(self.handle, False)
                if self.debug:
                    print(f"[hid] open path={self.path!r}")
                return
            except Exception as e:
                if self.debug:
                    print(
                        f"[hid] open(path) failed: {repr(e)}  -> fallback to VID/PID/enum"
                    )

        # 2) Explizite VID/PID
        if self.pid is not None:
            try:
                self.handle = _hid_open_vid_pid(self.vid, self.pid)
                _hid_set_nonblocking(self.handle, False)
                if self.debug:
                    print(f"[hid] open VID=0x{self.vid:04x} PID=0x{self.pid:04x}")
                return
            except Exception as e:
                if self.debug:
                    print(f"[hid] open(vid,pid) failed: {repr(e)}  -> fallback to enum")

        # 3) Enumerieren: usage_page 0xFF60 bevorzugen
        candidate = None
        fallback = None
        for dev in _hid_enumerate():
            if dev.get("vendor_id") != self.vid:
                continue
            if dev.get("usage_page") == 0xFF60:
                candidate = dev
                break
            if fallback is None:
                fallback = dev
        target = candidate or fallback
        if target:
            try:
                self.handle = _hid_open_path(target["path"])
                _hid_set_nonblocking(self.handle, False)
                self.pid = target.get("product_id")
                if self.debug:
                    up = target.get("usage_page")
                    print(
                        f"[hid] open enum path={target['path']!r} VID=0x{self.vid:04x} PID=0x{self.pid:04x} usage_page={hex(up) if up is not None else 'n/a'}"
                    )
                return
            except Exception as e:
                if self.debug:
                    print(f"[hid] open(enum path) failed: {repr(e)}")

        raise RuntimeError(
            "Kein HID-Interface geöffnet (path/vidpid/enum). Prüfe, ob VIA/Launcher geschlossen ist."
        )

    def close(self) -> None:
        if self.handle:
            _hid_close(self.handle)
            self.handle = None

    def _send_feature(self, payload: bytes) -> int:
        assert self.handle is not None
        sizes = [self.report_size] if self.report_size else PREFERRED_REPORT_SIZES
        last_exc = None
        for size in sizes:
            report = bytearray([0x00]) + bytearray(payload)
            if len(report) < size:
                report.extend(b"\x00" * (size - len(report)))
            try:
                if self.debug:
                    print(f"[hid] SEND({size}): {report.hex(' ')}")
                n = _hid_send_feature_report(self.handle, bytes(report))
                if n > 0:
                    return n
            except Exception as e:
                last_exc = e
                continue
        if last_exc:
            raise last_exc
        raise RuntimeError("send_feature_report fehlgeschlagen")

    def _get_feature(self, expected_len: int) -> bytes:
        assert self.handle is not None
        sizes = [self.report_size] if self.report_size else PREFERRED_REPORT_SIZES
        last_exc = None
        for size in sizes:
            try:
                data = _hid_get_feature_report(self.handle, 0x00, size)
                if self.debug:
                    print(f"[hid] RECV({size}): {bytes(data).hex(' ')}")
                payload = bytes(data)[1:]  # Report-ID entfernen
                return payload[:expected_len] if expected_len else payload
            except Exception as e:
                last_exc = e
                continue
        if last_exc:
            raise last_exc
        raise RuntimeError("get_feature_report fehlgeschlagen")


# --------------------------------------------------------------------
# VIA Macro Client (Profil A/B)
# --------------------------------------------------------------------
class ViaMacroClient:
    def __init__(self, dev: HidDevice, debug: bool = False):
        self.dev = dev
        self.debug = debug
        self.profile: Optional[str] = None
        self.chunk: Optional[int] = None

    def _calc_chunk(self, header_len: int) -> int:
        for size in (
            [self.dev.report_size] if self.dev.report_size else PREFERRED_REPORT_SIZES
        ):
            return size - 1 - header_len
        return 32 - 1 - header_len

    def _probe_profile_a(self) -> None:
        self.chunk = self._calc_chunk(PROFILE_A_HEADER_LEN)
        self.dev._send_feature(bytes([CMD_MACRO_GET_BUFFER, 0x00, 0x00]))
        _ = self.dev._get_feature(0)

    def _probe_profile_b(self) -> None:
        self.chunk = self._calc_chunk(PROFILE_B_HEADER_LEN)
        self.dev._send_feature(bytes([CMD_MACRO_GET_BUFFER, 0x00]))
        _ = self.dev._get_feature(0)

    def detect_profile_and_chunk(self) -> None:
        try:
            self._probe_profile_a()
            self.profile = "A"
            if self.debug:
                print("[via] Profil A erkannt")
                return
        except Exception as e:
            if self.debug:
                print(f"[via] Profil A fehlgeschlagen: {e}")
        try:
            self._probe_profile_b()
            self.profile = "B"
            if self.debug:
                print("[via] Profil B erkannt")
                return
        except Exception as e:
            if self.debug:
                print(f"[via] Profil B fehlgeschlagen: {e}")
        raise RuntimeError("Konnte weder Profil A noch B erkennen.")

    def get_macro_count(self) -> Optional[int]:
        try:
            self.dev._send_feature(bytes([CMD_MACRO_GET_COUNT]))
            resp = self.dev._get_feature(32)
            return resp[0] if resp else None
        except Exception as e:
            if self.debug:
                print(f"[via] get_macro_count fehlgeschlagen: {e}")
            return None

    def read_slot(self, slot: int, max_bytes: int = 1024) -> bytes:
        if self.profile is None:
            self.detect_profile_and_chunk()
        buf = bytearray()
        offset = 0
        tries = 0
        while len(buf) < max_bytes and tries < 64:
            payload = (
                bytes([CMD_MACRO_GET_BUFFER, slot & 0xFF, offset & 0xFF])
                if self.profile == "A"
                else bytes([CMD_MACRO_GET_BUFFER, slot & 0xFF])
            )
            self.dev._send_feature(payload)
            resp = self.dev._get_feature(
                self.dev.report_size or PREFERRED_REPORT_SIZES[0]
            )
            data = resp.rstrip(b"\x00")
            if not data:
                break
            buf.extend(data)
            if len(data) < (self.chunk or len(data)):
                break
            offset += len(data)
            tries += 1
        return bytes(buf)

    def write_slot(self, slot: int, data: bytes) -> None:
        if self.profile is None:
            self.detect_profile_and_chunk()
        offset = 0
        while offset < len(data):
            chunk = data[offset : offset + (self.chunk or len(data))]
            header = (
                bytes(
                    [
                        CMD_MACRO_SET_BUFFER,
                        slot & 0xFF,
                        (offset & 0xFF),
                        (len(chunk) & 0xFF),
                    ]
                )
                if self.profile == "A"
                else bytes([CMD_MACRO_SET_BUFFER, slot & 0xFF])
            )
            self.dev._send_feature(header + chunk)
            time.sleep(0.002)
            offset += len(chunk)


# --------------------------------------------------------------------
# High-Level Export / Import
# --------------------------------------------------------------------
def export_macros_to_json(
    device: DeviceInfo,
    out_json: str,
    slots: int,
    debug: bool = False,
    path: Optional[str] = None,
    report_size: Optional[int] = None,
) -> None:
    dev = HidDevice(
        device.vendor_id,
        device.product_id,
        report_size=report_size,
        debug=debug,
        path=(path.encode() if path else None),
    )
    dev.open()
    try:
        cli = ViaMacroClient(dev, debug=debug)
        count = cli.get_macro_count()
        slot_count = min(slots, count) if count else slots
        entries: List[MacroEntry] = []
        for s in range(slot_count):
            raw = cli.read_slot(s)
            text = None
            bytes_hex = None
            try:
                text = raw.decode("utf-8")
            except UnicodeDecodeError:
                bytes_hex = raw.hex(" ")
            entries.append(MacroEntry(slot=s, text=text, bytes_hex=bytes_hex))
            if debug:
                print(f"[export] Slot {s}: {len(raw)} bytes")
        MacroJson(
            device=device, encoding="text", macros=entries, slots=slot_count
        ).save(out_json)
        print(f"Export ok → {out_json} ({slot_count} Slots)")
    finally:
        dev.close()


def import_macros_from_json(
    in_json: str,
    override_slots: Optional[int],
    debug: bool = False,
    path: Optional[str] = None,
    report_size: Optional[int] = None,
) -> None:
    mj = MacroJson.from_file(in_json)
    if override_slots is not None:
        mj.slots = override_slots
    dev = HidDevice(
        mj.device.vendor_id,
        mj.device.product_id,
        report_size=report_size,
        debug=debug,
        path=(path.encode() if path else None),
    )
    dev.open()
    try:
        cli = ViaMacroClient(dev, debug=debug)
        for m in mj.macros:
            data = m.to_bytes(mj.encoding)
            if debug:
                dd = data.hex(" ")
                print(
                    f"[import] Slot {m.slot}: {len(data)} bytes → {dd[:80]}{'...' if len(dd) > 80 else ''}"
                )
            cli.write_slot(m.slot, data)
        print(f"Import ok ← {in_json} ({len(mj.macros)} Slots)")
    finally:
        dev.close()


# --------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(
        description="Keychron Macro Sync (VIA HID) – Export/Import Makros als JSON"
    )
    sub = ap.add_subparsers(dest="cmd", required=True)

    ap_exp = sub.add_parser("export", help="Makros vom Board in JSON exportieren")
    ap_exp.add_argument("-o", "--out", required=True, help="Zieldatei (macro.json)")
    ap_exp.add_argument("--vid", default=f"0x{DEFAULT_VENDOR_ID:04x}", help="Vendor-ID")
    ap_exp.add_argument("--pid", default=None, help="Product-ID")
    ap_exp.add_argument(
        "--path", default=None, help="HID path (/dev/hidrawX). Überschreibt VID/PID."
    )
    ap_exp.add_argument(
        "--report-size", type=int, default=None, help="Report-Größe (32/64)"
    )
    ap_exp.add_argument("--slots", type=int, default=12, help="Anzahl Slots")
    ap_exp.add_argument("--debug", action="store_true", help="Debug-Ausgaben")

    ap_imp = sub.add_parser("import", help="Makros aus JSON ins Board schreiben")
    ap_imp.add_argument(
        "-i", "--in", dest="infile", required=True, help="Quelldatei (macro.json)"
    )
    ap_imp.add_argument(
        "--path", default=None, help="HID path (/dev/hidrawX). Überschreibt VID/PID."
    )
    ap_imp.add_argument(
        "--report-size", type=int, default=None, help="Report-Größe (32/64)"
    )
    ap_imp.add_argument(
        "--slots", type=int, default=None, help="Slots im JSON überschreiben"
    )
    ap_imp.add_argument("--debug", action="store_true", help="Debug-Ausgaben")

    args = ap.parse_args()

    if args.cmd == "export":
        vid = (
            int(args.vid, 16)
            if str(args.vid).lower().startswith("0x")
            else int(args.vid)
        )
        pid = None
        if args.pid:
            pid = (
                int(args.pid, 16)
                if str(args.pid).lower().startswith("0x")
                else int(args.pid)
            )
        device = DeviceInfo(vendor_id=vid, product_id=pid)
        export_macros_to_json(
            device=device,
            out_json=args.out,
            slots=args.slots,
            debug=args.debug,
            path=args.path,
            report_size=args.report_size,
        )
    elif args.cmd == "import":
        import_macros_from_json(
            in_json=args.infile,
            override_slots=args.slots,
            debug=args.debug,
            path=args.path,
            report_size=args.report_size,
        )


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Abgebrochen.", file=sys.stderr)
        sys.exit(130)
    except Exception as e:
        print(f"Fehler: {e}", file=sys.stderr)
        sys.exit(1)
