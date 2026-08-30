#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
via_macros.py - VIA/QMK Dynamic-Macros zwischen Boards synchronisieren.

Liest die komplette Makro-Bank eines QMK/VIA-Boards per Raw-HID aus und
schreibt sie in eine JSON-Datei (menschenlesbar UND exakt round-trip-fähig),
bzw. liest eine solche JSON-Datei wieder auf ein (ggf. anderes) Board zurück.

Ersetzt keychron_macro_sync.py, das mit veralteten VIA-Kommando-IDs
(0x05/0x06/0x07 statt der echten 0x0C-0x10 aus quantum/via.h) und über
HID Feature-Reports statt der echten Output/Input-Raw-HID-Reports
arbeitete - beides passt nicht zum aktuellen QMK-Protokoll.

Protokoll-Details (quantum/via.c, quantum/dynamic_keymap.c,
quantum/send_string/send_string_keycodes.h):
  - Raw-HID-Reports sind exakt 32 Byte, OHNE führendes Report-ID-Byte.
  - id_dynamic_keymap_macro_get_count       = 0x0C
  - id_dynamic_keymap_macro_get_buffer_size = 0x0D
  - id_dynamic_keymap_macro_get_buffer      = 0x0E  [offsetHi,offsetLo,size<=28]
  - id_dynamic_keymap_macro_set_buffer      = 0x0F  [offsetHi,offsetLo,size<=28,data...]
  - id_dynamic_keymap_macro_reset           = 0x10
  - Die gesamte Makro-Bank ist EIN Buffer; einzelne Makros sind darin durch
    ein einzelnes 0x00-Byte getrennt (bis zu `count` Stück, Rest ist
    Null-Padding).
  - Innerhalb eines Makros: druckbare ASCII-Bytes sind wörtliche Zeichen;
    Byte 0x01 (SS_QMK_PREFIX) leitet eine Spezial-Sequenz ein:
      \\x01\\x01<keycode>   TAP
      \\x01\\x02<keycode>   DOWN
      \\x01\\x03<keycode>   UP
      \\x01\\x04<digits>|   DELAY (ms), Ziffern als ASCII, terminiert mit '|'

Nutzung:
    python3 via_macros.py export -o macros.json --path /dev/hidraw8
    python3 via_macros.py import -i macros.json --path /dev/hidraw8
    python3 via_macros.py list             # findet Raw-HID-Interfaces aller QMK-Boards

Board-Auswahl: --path /dev/hidrawN ist am zuverlässigsten (siehe `list`).
Alternativ --vid/--pid: durchsucht dann alle hidraw-Nodes mit dieser
Vendor/Product-ID nach dem Interface, dessen HID-Report-Descriptor die
Vendor-Usage-Page 0xFF60 deklariert (das ist QMKs Raw-HID/VIA-Interface).
"""

import argparse
import ctypes
import fcntl
import glob
import json
import os
import re
import select
import struct
import sys
import time
from dataclasses import dataclass, field
from typing import Dict, List, Optional

# ----------------------------------------------------------------------
# Keycode-Tabelle (Name -> Byte), aus send_string_keycodes.h übernommen.
# Deckt Buchstaben/Zahlen/Modifier/Funktionstasten/gängige Sondertasten ab -
# für "Shortcuts" (Strg+C-artige Kombos, Funktionstasten, Text) ausreichend.
# ----------------------------------------------------------------------
_KEYCODE_TABLE_SRC = """
A 04 B 05 C 06 D 07 E 08 F 09 G 0a H 0b I 0c J 0d K 0e L 0f M 10
N 11 O 12 P 13 Q 14 R 15 S 16 T 17 U 18 V 19 W 1a X 1b Y 1c Z 1d
1 1e 2 1f 3 20 4 21 5 22 6 23 7 24 8 25 9 26 0 27
ENTER 28 ENT 28 ESCAPE 29 ESC 29 BACKSPACE 2a BSPC 2a TAB 2b SPACE 2c SPC 2c
MINUS 2d MINS 2d EQUAL 2e EQL 2e LEFT_BRACKET 2f LBRC 2f RIGHT_BRACKET 30 RBRC 30
BACKSLASH 31 BSLS 31 NONUS_HASH 32 NUHS 32 SEMICOLON 33 SCLN 33 QUOTE 34 QUOT 34
GRAVE 35 GRV 35 COMMA 36 COMM 36 DOT 37 SLASH 38 SLSH 38 CAPS_LOCK 39 CAPS 39
F1 3a F2 3b F3 3c F4 3d F5 3e F6 3f F7 40 F8 41 F9 42 F10 43 F11 44 F12 45
PRINT_SCREEN 46 PSCR 46 SCROLL_LOCK 47 SCRL 47 PAUSE 48 PAUS 48 INSERT 49 INS 49
HOME 4a PAGE_UP 4b PGUP 4b DELETE 4c DEL 4c END 4d PAGE_DOWN 4e PGDN 4e
RIGHT 4f RGHT 4f LEFT 50 DOWN 51 UP 52 NUM_LOCK 53 NUM 53
KP_SLASH 54 PSLS 54 KP_ASTERISK 55 PAST 55 KP_MINUS 56 PMNS 56 KP_PLUS 57 PPLS 57
KP_ENTER 58 PENT 58 KP_1 59 P1 59 KP_2 5a P2 5a KP_3 5b P3 5b KP_4 5c P4 5c
KP_5 5d P5 5d KP_6 5e P6 5e KP_7 5f P7 5f KP_8 60 P8 60 KP_9 61 P9 61
KP_0 62 P0 62 KP_DOT 63 PDOT 63 NONUS_BACKSLASH 64 NUBS 64 APPLICATION 65 APP 65
F13 68 F14 69 F15 6a F16 6b F17 6c F18 6d F19 6e F20 6f F21 70 F22 71 F23 72 F24 73
EXECUTE 74 HELP 75 MENU 76 SELECT 77 STOP 78 AGAIN 79 UNDO 7a CUT 7b COPY 7c
PASTE 7d FIND 7e KB_MUTE 7f KB_VOLUME_UP 80 KB_VOLUME_DOWN 81
LOCKING_CAPS_LOCK 82 LCAP 82 LOCKING_NUM_LOCK 83 LNUM 83 LOCKING_SCROLL_LOCK 84 LSCR 84
KP_COMMA 85 PCMM 85
LEFT_CTRL e0 LCTL e0 LEFT_SHIFT e1 LSFT e1 LEFT_ALT e2 LALT e2 LOPT e2
LEFT_GUI e3 LGUI e3 LCMD e3 LWIN e3
RIGHT_CTRL e4 RCTL e4 RIGHT_SHIFT e5 RSFT e5 RIGHT_ALT e6 RALT e6 ALGR e6 ROPT e6
RIGHT_GUI e7 RGUI e7 RCMD e7 RWIN e7
"""


def _build_keycode_table() -> Dict[str, int]:
    toks = _KEYCODE_TABLE_SRC.split()
    table: Dict[str, int] = {}
    for i in range(0, len(toks) - 1, 2):
        name, hexval = toks[i], toks[i + 1]
        table[name.upper()] = int(hexval, 16)
    return table


KEYCODE_TO_BYTE = _build_keycode_table()
BYTE_TO_KEYCODE = {v: k for k, v in KEYCODE_TO_BYTE.items() if len(k) > 1}
# bevorzugt kurze Namen (LCTL statt LEFT_CTRL) fuer die Byte->Name-Reversetabelle
for _name, _val in KEYCODE_TO_BYTE.items():
    if len(_name) <= 5:
        BYTE_TO_KEYCODE[_val] = _name

SS_QMK_PREFIX = 0x01
SS_TAP_CODE = 0x01
SS_DOWN_CODE = 0x02
SS_UP_CODE = 0x03
SS_DELAY_CODE = 0x04


# ----------------------------------------------------------------------
# Menschenlesbare Kodierung eines einzelnen Makros
# ----------------------------------------------------------------------
def decode_macro(raw: bytes) -> str:
    out: List[str] = []
    i = 0
    n = len(raw)
    while i < n:
        b = raw[i]
        if b == SS_QMK_PREFIX and i + 1 < n:
            sub = raw[i + 1]
            if sub in (SS_TAP_CODE, SS_DOWN_CODE, SS_UP_CODE) and i + 2 < n:
                kc = raw[i + 2]
                name = BYTE_TO_KEYCODE.get(kc, f"0x{kc:02x}")
                word = {SS_TAP_CODE: "TAP", SS_DOWN_CODE: "DOWN", SS_UP_CODE: "UP"}[sub]
                out.append(f"{{{word} {name}}}")
                i += 3
                continue
            if sub == SS_DELAY_CODE:
                j = i + 2
                digits = []
                while j < n and raw[j:j + 1].isdigit():
                    digits.append(chr(raw[j]))
                    j += 1
                if j < n and raw[j] == ord("|"):
                    out.append(f"{{DELAY {''.join(digits)}}}")
                    i = j + 1
                    continue
            # unbekannte/kaputte Escape-Sequenz - roh als Hex durchreichen,
            # damit kein Datenverlust entsteht
            out.append(f"{{RAW:{b:02x}}}")
            i += 1
            continue
        ch = chr(b)
        if 0x20 <= b < 0x7F:
            if ch in "{}\\":
                out.append("\\" + ch)
            else:
                out.append(ch)
        elif ch in ("\n", "\t"):
            out.append(ch)
        else:
            out.append(f"{{RAW:{b:02x}}}")
        i += 1
    return "".join(out)


_TOKEN_RE = re.compile(r"\{(TAP|DOWN|UP)\s+([A-Za-z0-9_]+)\}|\{DELAY\s+(\d+)\}|\{RAW:([0-9a-fA-F]{2})\}")


def encode_macro(text: str) -> bytes:
    out = bytearray()
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]
        if ch == "\\" and i + 1 < n and text[i + 1] in "{}\\":
            out.append(ord(text[i + 1]))
            i += 2
            continue
        if ch == "{":
            m = _TOKEN_RE.match(text, i)
            if not m:
                raise ValueError(f"Ungültiges Token ab Position {i}: {text[i:i+20]!r}")
            if m.group(1):
                word, name = m.group(1), m.group(2).upper()
                kc = KEYCODE_TO_BYTE.get(name)
                if kc is None and name.startswith("0X"):
                    kc = int(name, 16)
                if kc is None:
                    raise ValueError(f"Unbekannter Keycode-Name: {name}")
                sub = {"TAP": SS_TAP_CODE, "DOWN": SS_DOWN_CODE, "UP": SS_UP_CODE}[word]
                out.extend([SS_QMK_PREFIX, sub, kc])
            elif m.group(3):
                out.append(SS_QMK_PREFIX)
                out.append(SS_DELAY_CODE)
                out.extend(m.group(3).encode("ascii"))
                out.append(ord("|"))
            elif m.group(4):
                out.append(int(m.group(4), 16))
            i = m.end()
            continue
        out.append(ord(ch) if ord(ch) < 128 else ord("?"))
        i += 1
    return bytes(out)


# ----------------------------------------------------------------------
# Raw-HID-Transport (kein hidapi noetig - direkt gegen /dev/hidrawN)
# ----------------------------------------------------------------------
_HIDIOCGRDESCSIZE = 0x80044801
_HIDIOCGRDESC = 0x90044802


def _read_report_descriptor(devpath: str) -> Optional[bytes]:
    try:
        fd = os.open(devpath, os.O_RDWR)
    except OSError:
        return None
    try:
        size_buf = ctypes.create_string_buffer(4)
        fcntl.ioctl(fd, _HIDIOCGRDESCSIZE, size_buf, True)
        size = struct.unpack("i", size_buf)[0]
        desc_buf = ctypes.create_string_buffer(4 + 4096)
        struct.pack_into("i", desc_buf, 0, size)
        fcntl.ioctl(fd, _HIDIOCGRDESC, desc_buf, True)
        return bytes(desc_buf[4:4 + size])
    except OSError:
        return None
    finally:
        os.close(fd)


def find_raw_hid_path(vid: int, pid: Optional[int] = None) -> Optional[str]:
    """Sucht unter allen /dev/hidrawN mit passender VID[/PID] dasjenige
    Interface, dessen Report-Descriptor die Vendor-Usage-Page 0xFF60
    deklariert (QMKs RAW_ENABLE/VIA-Kanal)."""
    for node in sorted(glob.glob("/dev/hidraw*")):
        name = os.path.basename(node)
        sysfs = f"/sys/class/hidraw/{name}/device/uevent"
        try:
            with open(sysfs) as f:
                content = f.read()
        except OSError:
            continue
        m = re.search(r"HID_ID=\S+:0000([0-9A-Fa-f]{4}):0000([0-9A-Fa-f]{4})", content)
        if not m:
            continue
        node_vid, node_pid = int(m.group(1), 16), int(m.group(2), 16)
        if node_vid != vid or (pid is not None and node_pid != pid):
            continue
        desc = _read_report_descriptor(node)
        if desc and b"\x06\x60\xff" in desc:
            return node
    return None


def list_raw_hid_devices() -> List[Dict[str, str]]:
    found = []
    for node in sorted(glob.glob("/dev/hidraw*")):
        name = os.path.basename(node)
        sysfs = f"/sys/class/hidraw/{name}/device/uevent"
        try:
            with open(sysfs) as f:
                content = f.read()
        except OSError:
            continue
        hid_id = re.search(r"HID_ID=(\S+)", content)
        hid_name = re.search(r"HID_NAME=(.+)", content)
        desc = _read_report_descriptor(node)
        is_raw = bool(desc and b"\x06\x60\xff" in desc)
        found.append(
            {
                "path": node,
                "hid_id": hid_id.group(1) if hid_id else "?",
                "name": hid_name.group(1) if hid_name else "?",
                "raw_hid": is_raw,
            }
        )
    return found


class RawHid:
    def __init__(self, path: str, debug: bool = False):
        self.path = path
        self.debug = debug
        self.fd: Optional[int] = None

    def open(self) -> None:
        self.fd = os.open(self.path, os.O_RDWR | os.O_NONBLOCK)

    def close(self) -> None:
        if self.fd is not None:
            os.close(self.fd)
            self.fd = None

    def cmd(self, payload: bytes, wait: float = 2.0) -> bytes:
        assert self.fd is not None
        if len(payload) > 32:
            raise ValueError("Report darf max. 32 Byte sein")
        report = payload + bytes(32 - len(payload))
        if self.debug:
            print(f"[hid] >> {report.hex(' ')}", file=sys.stderr)
        os.write(self.fd, report)
        r, _, _ = select.select([self.fd], [], [], wait)
        if not r:
            raise TimeoutError(f"Keine Antwort innerhalb {wait}s (cmd=0x{payload[0]:02x})")
        resp = os.read(self.fd, 32)
        if self.debug:
            print(f"[hid] << {resp.hex(' ')}", file=sys.stderr)
        return resp


# ----------------------------------------------------------------------
# VIA Dynamic-Macro-Client
# ----------------------------------------------------------------------
ID_MACRO_GET_COUNT = 0x0C
ID_MACRO_GET_BUFFER_SIZE = 0x0D
ID_MACRO_GET_BUFFER = 0x0E
ID_MACRO_SET_BUFFER = 0x0F
ID_MACRO_RESET = 0x10
CHUNK = 28  # max. Nutzdaten pro get/set_buffer-Report (32 - 1cmd - 2offset - 1size)


class ViaMacros:
    def __init__(self, hid: RawHid):
        self.hid = hid

    def get_count(self) -> int:
        resp = self.hid.cmd(bytes([ID_MACRO_GET_COUNT]))
        return resp[1]

    def get_buffer_size(self) -> int:
        resp = self.hid.cmd(bytes([ID_MACRO_GET_BUFFER_SIZE]))
        return (resp[1] << 8) | resp[2]

    def get_buffer(self, size: Optional[int] = None) -> bytes:
        if size is None:
            size = self.get_buffer_size()
        buf = bytearray()
        offset = 0
        while offset < size:
            n = min(CHUNK, size - offset)
            resp = self.hid.cmd(bytes([ID_MACRO_GET_BUFFER, (offset >> 8) & 0xFF, offset & 0xFF, n]))
            buf.extend(resp[4:4 + n])
            offset += n
        return bytes(buf)

    def set_buffer(self, data: bytes, size: Optional[int] = None) -> None:
        if size is None:
            size = self.get_buffer_size()
        if len(data) > size:
            raise ValueError(f"Makro-Daten ({len(data)} Byte) passen nicht in Buffer ({size} Byte)")
        padded = data + bytes(size - len(data))
        offset = 0
        while offset < size:
            n = min(CHUNK, size - offset)
            chunk = padded[offset:offset + n]
            self.hid.cmd(bytes([ID_MACRO_SET_BUFFER, (offset >> 8) & 0xFF, offset & 0xFF, n]) + chunk)
            time.sleep(0.005)
            offset += n

    def reset(self) -> None:
        self.hid.cmd(bytes([ID_MACRO_RESET]), wait=3.0)


# ----------------------------------------------------------------------
# Export / Import
# ----------------------------------------------------------------------
@dataclass
class MacroSlot:
    slot: int
    text: str = ""


def split_buffer(raw: bytes, count: int) -> List[bytes]:
    parts = raw.split(b"\x00")
    slots = parts[:count]
    while len(slots) < count:
        slots.append(b"")
    return slots


def join_buffer(slots: List[bytes]) -> bytes:
    return b"\x00".join(slots) + b"\x00"


def resolve_hid(path: Optional[str], vid: Optional[str], pid: Optional[str], debug: bool) -> RawHid:
    if path:
        h = RawHid(path, debug=debug)
        h.open()
        return h
    if not vid:
        raise SystemExit("Entweder --path oder --vid angeben (siehe `via_macros.py list`)")
    vid_i = int(vid, 16) if vid.lower().startswith("0x") else int(vid)
    pid_i = None
    if pid:
        pid_i = int(pid, 16) if pid.lower().startswith("0x") else int(pid)
    found = find_raw_hid_path(vid_i, pid_i)
    if not found:
        raise SystemExit(
            f"Kein Raw-HID-Interface für VID=0x{vid_i:04x} gefunden. "
            f"`sudo` nötig für /sys/class/hidraw-Zugriff? Oder --path direkt angeben "
            f"(siehe `via_macros.py list`)."
        )
    h = RawHid(found, debug=debug)
    h.open()
    return h


def cmd_export(args) -> None:
    hid = resolve_hid(args.path, args.vid, args.pid, args.debug)
    try:
        via = ViaMacros(hid)
        count = via.get_count()
        size = via.get_buffer_size()
        print(f"Board meldet {count} Makro-Slots, Buffer {size} Byte", file=sys.stderr)
        raw = via.get_buffer(size)
        slots = split_buffer(raw, count)
        data = {
            "format": "via-macros-v1",
            "source_device": {"path": hid.path},
            "buffer_size": size,
            "count": count,
            "slots": [
                {"slot": i, "text": decode_macro(s), "bytes": s.hex(" ")}
                for i, s in enumerate(slots)
            ],
        }
        with open(args.out, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        non_empty = sum(1 for s in slots if s)
        print(f"Export ok -> {args.out} ({non_empty}/{count} Slots belegt)")
    finally:
        hid.close()


def cmd_import(args) -> None:
    with open(args.infile, "r", encoding="utf-8") as f:
        data = json.load(f)
    hid = resolve_hid(args.path, args.vid, args.pid, args.debug)
    try:
        via = ViaMacros(hid)
        count = via.get_count()
        size = via.get_buffer_size()
        print(f"Zielboard: {count} Makro-Slots, Buffer {size} Byte", file=sys.stderr)

        slot_entries = sorted(data["slots"], key=lambda s: s["slot"])
        if len(slot_entries) > count:
            print(
                f"Warnung: JSON hat {len(slot_entries)} Slots, Zielboard nur {count} - "
                f"überzählige werden ignoriert",
                file=sys.stderr,
            )
        slots_bytes: List[bytes] = []
        for i in range(count):
            if i < len(slot_entries):
                entry = slot_entries[i]
                # bytes (Hex) gewinnt, wenn vorhanden -> exaktes Round-Trip;
                # sonst wird der von Hand editierbare Text neu kodiert.
                if entry.get("bytes"):
                    raw = bytes.fromhex(entry["bytes"].replace(" ", ""))
                else:
                    raw = encode_macro(entry.get("text", ""))
            else:
                raw = b""
            slots_bytes.append(raw)

        buf = join_buffer(slots_bytes)
        if len(buf) > size:
            raise SystemExit(
                f"Makro-Daten ({len(buf)} Byte) passen nicht in den Zielbuffer ({size} Byte) - "
                f"Makros kürzen oder weniger Slots befüllen."
            )

        if not args.no_reset:
            via.reset()
            time.sleep(0.2)
        via.set_buffer(buf, size)
        print(f"Import ok <- {args.infile} ({len(slot_entries)} Slots geschrieben)")
    finally:
        hid.close()


def cmd_list(_args) -> None:
    devs = list_raw_hid_devices()
    if not devs:
        print("Keine /dev/hidraw-Geräte lesbar (Berechtigung? sudo?).")
        return
    for d in devs:
        marker = "  <-- Raw-HID (VIA)" if d["raw_hid"] else ""
        print(f"{d['path']:16} {d['hid_id']:24} {d['name']}{marker}")


def main() -> None:
    ap = argparse.ArgumentParser(description="VIA/QMK Dynamic-Macros exportieren/importieren")
    sub = ap.add_subparsers(dest="cmd", required=True)

    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--path", default=None, help="/dev/hidrawN direkt (siehe `list`)")
    common.add_argument("--vid", default=None, help="Vendor-ID (Hex, z.B. 0x9D5B) - Fallback wenn --path fehlt")
    common.add_argument("--pid", default=None, help="Product-ID (Hex), optional zusammen mit --vid")
    common.add_argument("--debug", action="store_true")

    ap_exp = sub.add_parser("export", parents=[common], help="Makros vom Board in JSON exportieren")
    ap_exp.add_argument("-o", "--out", required=True)

    ap_imp = sub.add_parser("import", parents=[common], help="Makros aus JSON ins Board schreiben")
    ap_imp.add_argument("-i", "--in", dest="infile", required=True)
    ap_imp.add_argument(
        "--no-reset", action="store_true",
        help="Vor dem Schreiben KEINEN macro_reset senden (sonst werden alte Slots >Anzahl im JSON nicht geleert)",
    )

    sub.add_parser("list", help="Alle Raw-HID-fähigen HID-Geräte auflisten")

    args = ap.parse_args()
    if args.cmd == "export":
        cmd_export(args)
    elif args.cmd == "import":
        cmd_import(args)
    elif args.cmd == "list":
        cmd_list(args)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Abgebrochen.", file=sys.stderr)
        sys.exit(130)
    except (SystemExit,):
        raise
    except Exception as e:
        print(f"Fehler: {e}", file=sys.stderr)
        sys.exit(1)
