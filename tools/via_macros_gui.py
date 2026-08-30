#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
via_macros_gui.py - Tkinter-GUI für via_macros.py.

Erkennt angeschlossene QMK/VIA-Boards (per Raw-HID-Report-Descriptor,
siehe via_macros.find_raw_hid_path), und bietet Export (Board -> JSON),
Import (JSON -> Board) und Copy (Board -> Board direkt, ohne Zwischendatei).

Start: python3 via_macros_gui.py
"""

import queue
import sys
import threading
import time
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk
from typing import List, Optional

import via_macros as vm


# ----------------------------------------------------------------------
# Device-Auflistung / Anzeige
# ----------------------------------------------------------------------
class BoardEntry:
    def __init__(self, path: str, hid_id: str, name: str):
        self.path = path
        self.hid_id = hid_id
        self.name = name

    def label(self) -> str:
        return f"{self.name}  ({self.path})"


def scan_boards() -> List[BoardEntry]:
    boards = []
    for d in vm.list_raw_hid_devices():
        if d["raw_hid"]:
            boards.append(BoardEntry(d["path"], d["hid_id"], d["name"]))
    return boards


# ----------------------------------------------------------------------
# Hintergrund-Worker: HID-Operationen laufen NIE im Tk-Mainthread
# (Reports brauchen mehrere Sekunden für volle Makro-Buffer)
# ----------------------------------------------------------------------
class Worker:
    def __init__(self, log_queue: "queue.Queue[str]"):
        self.log_queue = log_queue

    def log(self, msg: str) -> None:
        self.log_queue.put(msg)

    def _open(self, path: str) -> vm.RawHid:
        h = vm.RawHid(path)
        h.open()
        return h

    def export_to_file(self, path: str, out_file: str) -> None:
        try:
            hid = self._open(path)
            try:
                via = vm.ViaMacros(hid)
                count = via.get_count()
                size = via.get_buffer_size()
                self.log(f"Board meldet {count} Slots, Buffer {size} Byte")
                raw = via.get_buffer(size)
                slots = vm.split_buffer(raw, count)
                import json

                data = {
                    "format": "via-macros-v1",
                    "source_device": {"path": path},
                    "buffer_size": size,
                    "count": count,
                    "slots": [
                        {"slot": i, "text": vm.decode_macro(s), "bytes": s.hex(" ")}
                        for i, s in enumerate(slots)
                    ],
                }
                with open(out_file, "w", encoding="utf-8") as f:
                    json.dump(data, f, ensure_ascii=False, indent=2)
                non_empty = sum(1 for s in slots if s)
                self.log(f"OK: Export -> {out_file} ({non_empty}/{count} Slots belegt)")
            finally:
                hid.close()
        except Exception as e:
            self.log(f"FEHLER Export: {e}")

    def import_from_file(self, path: str, in_file: str, do_reset: bool) -> None:
        try:
            import json

            with open(in_file, "r", encoding="utf-8") as f:
                data = json.load(f)
            hid = self._open(path)
            try:
                via = vm.ViaMacros(hid)
                self._write_slots(via, data["slots"], do_reset)
                self.log(f"OK: Import <- {in_file}")
            finally:
                hid.close()
        except Exception as e:
            self.log(f"FEHLER Import: {e}")

    def copy_direct(self, src_path: str, dst_path: str, do_reset: bool) -> None:
        try:
            src_hid = self._open(src_path)
            try:
                src_via = vm.ViaMacros(src_hid)
                count = src_via.get_count()
                size = src_via.get_buffer_size()
                self.log(f"Lese Quelle: {count} Slots, {size} Byte ...")
                raw = src_via.get_buffer(size)
                slots = vm.split_buffer(raw, count)
            finally:
                src_hid.close()

            entries = [{"slot": i, "text": "", "bytes": s.hex(" ")} for i, s in enumerate(slots)]
            dst_hid = self._open(dst_path)
            try:
                dst_via = vm.ViaMacros(dst_hid)
                self.log("Schreibe Ziel ...")
                self._write_slots(dst_via, entries, do_reset)
                self.log("OK: Kopiert Quelle -> Ziel")
            finally:
                dst_hid.close()
        except Exception as e:
            self.log(f"FEHLER Copy: {e}")

    def _write_slots(self, via: "vm.ViaMacros", slot_entries: list, do_reset: bool) -> None:
        count = via.get_count()
        size = via.get_buffer_size()
        self.log(f"Zielboard: {count} Slots, {size} Byte")
        slot_entries = sorted(slot_entries, key=lambda s: s["slot"])
        if len(slot_entries) > count:
            self.log(f"Warnung: {len(slot_entries)} Slots angeboten, Ziel nur {count} - Rest ignoriert")
        slots_bytes = []
        for i in range(count):
            if i < len(slot_entries):
                entry = slot_entries[i]
                if entry.get("bytes"):
                    raw = bytes.fromhex(entry["bytes"].replace(" ", ""))
                else:
                    raw = vm.encode_macro(entry.get("text", ""))
            else:
                raw = b""
            slots_bytes.append(raw)
        buf = vm.join_buffer(slots_bytes)
        if len(buf) > size:
            raise ValueError(f"Makro-Daten ({len(buf)} Byte) passen nicht in Zielbuffer ({size} Byte)")
        if do_reset:
            self.log("Reset Zielboard-Makros ...")
            via.reset()
            time.sleep(0.2)
        via.set_buffer(buf, size)


# ----------------------------------------------------------------------
# GUI
# ----------------------------------------------------------------------
class ViaMacrosApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("VIA Makro-Sync")
        self.geometry("640x480")
        self.resizable(True, True)

        self.log_queue: "queue.Queue[str]" = queue.Queue()
        self.worker = Worker(self.log_queue)

        self.boards: List[BoardEntry] = []

        self._build_ui()
        self._refresh_boards()
        self.after(150, self._drain_log_queue)

    # -- UI aufbauen ----------------------------------------------------
    def _build_ui(self) -> None:
        top = ttk.Frame(self, padding=8)
        top.pack(fill="x")

        ttk.Button(top, text="Boards neu suchen", command=self._refresh_boards).pack(side="left")
        self.board_count_var = tk.StringVar(value="0 Boards gefunden")
        ttk.Label(top, textvariable=self.board_count_var).pack(side="left", padx=10)

        nb = ttk.Notebook(self)
        nb.pack(fill="both", expand=True, padx=8, pady=8)

        self._build_export_tab(nb)
        self._build_import_tab(nb)
        self._build_copy_tab(nb)

        log_frame = ttk.LabelFrame(self, text="Log", padding=4)
        log_frame.pack(fill="both", expand=False, padx=8, pady=(0, 8))
        self.log_text = tk.Text(log_frame, height=8, state="disabled", wrap="word")
        self.log_text.pack(fill="both", expand=True)

    def _board_combo(self, parent) -> ttk.Combobox:
        cb = ttk.Combobox(parent, state="readonly", width=50)
        return cb

    def _build_export_tab(self, nb: ttk.Notebook) -> None:
        f = ttk.Frame(nb, padding=12)
        nb.add(f, text="Export (Board -> JSON)")

        ttk.Label(f, text="Quell-Board:").grid(row=0, column=0, sticky="w", pady=4)
        self.exp_board = self._board_combo(f)
        self.exp_board.grid(row=0, column=1, sticky="we", pady=4)

        ttk.Label(f, text="Ziel-Datei:").grid(row=1, column=0, sticky="w", pady=4)
        self.exp_file_var = tk.StringVar()
        ttk.Entry(f, textvariable=self.exp_file_var, width=45).grid(row=1, column=1, sticky="we", pady=4)
        ttk.Button(f, text="...", width=3, command=self._pick_export_file).grid(row=1, column=2, padx=4)

        ttk.Button(f, text="Export starten", command=self._do_export).grid(row=2, column=1, sticky="w", pady=12)
        f.columnconfigure(1, weight=1)

    def _build_import_tab(self, nb: ttk.Notebook) -> None:
        f = ttk.Frame(nb, padding=12)
        nb.add(f, text="Import (JSON -> Board)")

        ttk.Label(f, text="Ziel-Board:").grid(row=0, column=0, sticky="w", pady=4)
        self.imp_board = self._board_combo(f)
        self.imp_board.grid(row=0, column=1, sticky="we", pady=4)

        ttk.Label(f, text="Quell-Datei:").grid(row=1, column=0, sticky="w", pady=4)
        self.imp_file_var = tk.StringVar()
        ttk.Entry(f, textvariable=self.imp_file_var, width=45).grid(row=1, column=1, sticky="we", pady=4)
        ttk.Button(f, text="...", width=3, command=self._pick_import_file).grid(row=1, column=2, padx=4)

        self.imp_reset_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(
            f, text="Vorher alle Makros auf dem Ziel löschen (empfohlen)", variable=self.imp_reset_var
        ).grid(row=2, column=0, columnspan=2, sticky="w", pady=4)

        ttk.Button(f, text="Import starten", command=self._do_import).grid(row=3, column=1, sticky="w", pady=12)
        f.columnconfigure(1, weight=1)

    def _build_copy_tab(self, nb: ttk.Notebook) -> None:
        f = ttk.Frame(nb, padding=12)
        nb.add(f, text="Copy (Board -> Board)")

        ttk.Label(f, text="Quell-Board:").grid(row=0, column=0, sticky="w", pady=4)
        self.copy_src_board = self._board_combo(f)
        self.copy_src_board.grid(row=0, column=1, sticky="we", pady=4)

        ttk.Label(f, text="Ziel-Board:").grid(row=1, column=0, sticky="w", pady=4)
        self.copy_dst_board = self._board_combo(f)
        self.copy_dst_board.grid(row=1, column=1, sticky="we", pady=4)

        self.copy_reset_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(
            f, text="Vorher alle Makros auf dem Ziel löschen (empfohlen)", variable=self.copy_reset_var
        ).grid(row=2, column=0, columnspan=2, sticky="w", pady=4)

        ttk.Button(f, text="Kopieren", command=self._do_copy).grid(row=3, column=1, sticky="w", pady=12)
        f.columnconfigure(1, weight=1)

    # -- Board-Erkennung --------------------------------------------------
    def _refresh_boards(self) -> None:
        self.boards = scan_boards()
        labels = [b.label() for b in self.boards]
        for combo in (self.exp_board, self.imp_board, self.copy_src_board, self.copy_dst_board):
            combo["values"] = labels
            if labels and not combo.get():
                combo.current(0)
        self.board_count_var.set(f"{len(self.boards)} Board(s) gefunden")
        if not self.boards:
            self._log_line("Keine Raw-HID/VIA-Boards gefunden - angeschlossen? Berechtigung für /dev/hidraw*?")

    def _selected_path(self, combo: ttk.Combobox) -> Optional[str]:
        idx = combo.current()
        if idx < 0 or idx >= len(self.boards):
            return None
        return self.boards[idx].path

    # -- Dateiauswahl -----------------------------------------------------
    def _pick_export_file(self) -> None:
        path = filedialog.asksaveasfilename(
            title="Makros exportieren nach ...",
            defaultextension=".json",
            filetypes=[("JSON", "*.json"), ("Alle Dateien", "*.*")],
        )
        if path:
            self.exp_file_var.set(path)

    def _pick_import_file(self) -> None:
        path = filedialog.askopenfilename(
            title="Makros importieren von ...",
            filetypes=[("JSON", "*.json"), ("Alle Dateien", "*.*")],
        )
        if path:
            self.imp_file_var.set(path)

    # -- Aktionen (im Hintergrund-Thread) ----------------------------------
    def _run_bg(self, fn, *args) -> None:
        t = threading.Thread(target=fn, args=args, daemon=True)
        t.start()

    def _do_export(self) -> None:
        path = self._selected_path(self.exp_board)
        out_file = self.exp_file_var.get().strip()
        if not path:
            messagebox.showwarning("Export", "Bitte ein Quell-Board auswählen.")
            return
        if not out_file:
            messagebox.showwarning("Export", "Bitte eine Ziel-Datei auswählen.")
            return
        self._log_line(f"Export: {path} -> {out_file}")
        self._run_bg(self.worker.export_to_file, path, out_file)

    def _do_import(self) -> None:
        path = self._selected_path(self.imp_board)
        in_file = self.imp_file_var.get().strip()
        if not path:
            messagebox.showwarning("Import", "Bitte ein Ziel-Board auswählen.")
            return
        if not in_file or not Path(in_file).exists():
            messagebox.showwarning("Import", "Bitte eine gültige JSON-Datei auswählen.")
            return
        if not messagebox.askyesno(
            "Import bestätigen",
            f"Makros aus\n{in_file}\nauf das Board schreiben?\nBestehende Makros auf dem Board werden überschrieben.",
        ):
            return
        self._log_line(f"Import: {in_file} -> {path}")
        self._run_bg(self.worker.import_from_file, path, in_file, self.imp_reset_var.get())

    def _do_copy(self) -> None:
        src = self._selected_path(self.copy_src_board)
        dst = self._selected_path(self.copy_dst_board)
        if not src or not dst:
            messagebox.showwarning("Copy", "Bitte Quell- und Ziel-Board auswählen.")
            return
        if src == dst:
            messagebox.showwarning("Copy", "Quelle und Ziel sind identisch.")
            return
        src_label = self.copy_src_board.get()
        dst_label = self.copy_dst_board.get()
        if not messagebox.askyesno(
            "Copy bestätigen",
            f"Makros kopieren:\n{src_label}\n  ->\n{dst_label}\n\nBestehende Makros auf dem Ziel werden überschrieben.",
        ):
            return
        self._log_line(f"Copy: {src_label} -> {dst_label}")
        self._run_bg(self.worker.copy_direct, src, dst, self.copy_reset_var.get())

    # -- Log ----------------------------------------------------------------
    def _log_line(self, msg: str) -> None:
        self.log_queue.put(msg)

    def _drain_log_queue(self) -> None:
        try:
            while True:
                msg = self.log_queue.get_nowait()
                self.log_text.configure(state="normal")
                self.log_text.insert("end", msg + "\n")
                self.log_text.see("end")
                self.log_text.configure(state="disabled")
        except queue.Empty:
            pass
        self.after(150, self._drain_log_queue)


def main() -> None:
    try:
        import tkinter  # noqa: F401
    except ImportError:
        print("Fehler: tkinter nicht installiert (z.B. 'pacman -S tk' / 'apt install python3-tk').", file=sys.stderr)
        sys.exit(1)
    app = ViaMacrosApp()
    app.mainloop()


if __name__ == "__main__":
    main()
