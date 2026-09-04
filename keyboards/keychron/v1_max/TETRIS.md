# Tetris-Layer (Prototyp)

Hinter `TETRIS_GAME_ENABLE` (aus per Default). Baut mit:

```
qmk compile -kb keychron/v1_max/iso_encoder -km neo -e TETRIS_GAME_ENABLE=yes
```

Zugang: `TG(_TETRIS)` liegt auf `_SYS` (bislang freier Slot in Reihe 2).
Bewegung vim-artig auf der Home Row: `H`=links, `J`=runter (soft drop),
`K`=rotieren, `L`=rechts. `Leertaste`=hard drop. `Esc` verlässt den Layer.

PC-Seite braucht keine Software, nur ein Terminal, das getippte Bytes roh
durchreicht statt sie als Kommando zu interpretieren. Das hängt an der
**Shell dahinter**, nicht am Terminal-Emulator - Windows Terminal, WezTerm
und Kitty sind alle vollwertige VT100/xterm-Emulatoren und stellen die
ANSI-Escapes (Cursor-Position, Clear) identisch dar.

**WezTerm/Kitty, oder Windows Terminal mit WSL (POSIX-Shell):**
```
stty raw -echo && cat
```
Schaltet lokales Echo/Zeilenverarbeitung der Shell aus. Mit `Strg+C`
beendet sich `cat` in raw mode nicht mehr - stattdessen `stty sane` in
einem zweiten Terminal, oder das Terminal-Fenster schließen.

**Windows Terminal mit nativem PowerShell (kein WSL, kein `stty`/`cat`):**
```powershell
while (($c = [Console]::Read()) -ge 0) { [Console]::Out.Write([char]$c) }
```
Liest Zeichen direkt über die Console-API statt über PSReadLine, also
ohne Zeilen-Editing/Kommando-Interpretation. `Strg+C` bricht das Skript
normal ab (kein raw-mode-Nachteil wie bei `stty`). Ungetestet auf echtem
Windows - bei Problemen zuerst hier ansetzen.

Alle drei Varianten laufen über dieselbe deutsche `send_string()`-LUT
(`sendstring_de.c`) wie der Rest des Keymaps - erwartet also ein
deutsches Tastaturlayout auf dem jeweiligen Host, unabhängig vom
OS/Terminal.

**Caveat:** `DYNAMIC_KEYMAP_LAYER_COUNT` wechselt zwischen 13 und 14 je
nach Flag - VIAs EEPROM-Keymap-Layout hängt daran. Nach dem Umschalten
von `TETRIS_GAME_ENABLE` einmal EEPROM zurücksetzen (Bootmagic-Kombo oder
`EEPROM_RESET`-Keycode), sonst kann VIA verschobene Daten anzeigen.

Stand: Skelett mit einem festen Teil (T-Tetromino), Bewegung/Rotation/
Zeilen-Clear funktionieren. Kein Score, kein Next-Piece, keine
Zufalls-Teile-Bag - bewusst minimal zum Testen ob das Grundprinzip
(Terminal-Rendering per HID-Keystrokes) überhaupt gut genug performt.
