# Keychron V3 Max ISO Encoder (keymap `neo`)

## 🧩 Überblick
Kombination aus V3 (TKL-Formfaktor) und V1 Max (Wireless: 2.4G-Dongle +
Bluetooth, LKBT51-BT-Chip) - offizielles Keychron-Board, TKL-ISO mit
Encoder. Wie das V1 Max **baut es nicht gegen mainline QMK**, sondern
gegen Keychrons eigenen Fork (siehe Abschnitt Build) - `keyboards/keychron/v3_max`
existiert genau wie `v1_max` nur dort, nicht in `~/qmk_firmware`.

- MCU: **STM32F401** (Cortex-M4) - identisches `board.h`/`mcuconf.h`/
  `halconf.h` wie V1 Max (nur der Copyright-Jahresstempel unterscheidet
  sich), also gleiche RAM/Flash-Ausstattung.
- RGB: SNLED27351-LED-Treiber, **88 LEDs** (V1 Max: 82 - mehr Tasten durch
  TKL statt 75%), `RGB_MATRIX_ENABLE=yes` (`RGBLIGHT_ENABLE=no`).
- Wireless: LKBT51-BT-Chip (`keyboards/keychron/common/wireless/`),
  2.4G-Dongle-Modus (`P2P4G`), 3 BT-Host-Slots (`BT_HST1-3`) - identische
  Hardware-Anbindung wie V1 Max (`LK_WIRELESS_ENABLE`, gleiche Pins).
- Matrix: 6×17 (V1 Max: 6×16 - eine Spalte mehr durch TKL-F-Reihe).
- Bootloader: STM32-eigener ROM-DFU (`0483:df11` in `lsusb`), Flash über
  `dfu-util` (kein UF2 drag&drop).

## ⚙️ Build - braucht den Keychron-Fork, nicht mainline
Wie beim V1 Max: `keyboards/keychron/v3_max` und
`keyboards/keychron/common/wireless/` existieren nur im Fork
`~/keychron-qmk/qmk_firmware` (aktuell Branch `2025q3`), nicht in
Keychrons eigenem `master` und nicht in mainline QMK.

```bash
qmk config user.qmk_home=/home/dieter/keychron-qmk/qmk_firmware
cd /home/dieter/keychron-qmk/qmk_firmware
make keychron/v3_max/iso_encoder:neo QMK_USERSPACE=/home/dieter/qmk_userspace
qmk config user.qmk_home=/home/dieter/qmk_firmware   # danach zurücksetzen!
```
Gebaut und verifiziert (2026-09-08): sauberer Compile, nur die übliche
harmlose `lto-wrapper`-Warnung (serielle LTRANS-Kompilierung, kein
Fehler). Binärgröße `keychron_v3_max_iso_encoder_neo.bin` ≈ 120,6 KB -
praktisch identisch zum V1 Max (≈ 120,7 KB), wie bei gleicher MCU/
Feature-Ausstattung zu erwarten. Auch mit `TETRIS_GAME_ENABLE=yes`
gegengetestet - baut ebenfalls sauber.

## 🔌 Flashen (DFU, nicht UF2)
Bootloader-Modus wie gewohnt am Board auslösen (Board zeigt sich danach
als `0483:df11 STM Device in DFU Mode` in `lsusb`), dann:
```bash
make keychron/v3_max/iso_encoder:neo:dfu-util QMK_USERSPACE=/home/dieter/qmk_userspace
```
(`qmk_home` muss dafür wie oben auf den Fork zeigen.) **Noch nicht am
echten Board geflasht/verifiziert** - nur der Compile wurde getestet.

## 🎛️ Bluetooth
Gleicher Mechanismus wie beim V1 Max, siehe `BLUETOOTH.md` in diesem
Verzeichnis - einziger Unterschied sind die LED-Indizes (88 statt 82
LEDs) und dass hier **keine** redundante `_WIN_FN`-Ebene existiert (siehe
dort, bewusste Vereinfachung: nur der Capslock+U/I/O/P/Ü-Weg).

## 💡 VIA
- `via.json` liegt hier im Verzeichnis - 1:1 Kopie von Keychrons eigener
  offizieller Definition (`~/keychron-qmk/qmk_firmware/keyboards/keychron/
  v3_max/via_json/v3_max_iso_encoder.json`), Matrix-Maße (6×17) und
  Protokollversion unverändert.

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V3 Max ISO Encoder Board, custom Keymap
       gegen Keychrons eigenen QMK-Fork (~/keychron-qmk/qmk_firmware,
       Branch 2025q3) - Formfaktor von V3 (TKL) + Wireless-Logik von
       V1 Max übernommen.
```
