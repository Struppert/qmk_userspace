# Keychron V4 ISO (keymap `neo`)

## 🧩 Überblick
Reines 60%-Wired-Board, kein Wireless/Bluetooth. Anders als V1 Max/V3 Max
baut dieses Board **gegen mainline QMK**, nicht gegen Keychrons eigenen
Fork - `keyboards/keychron/v4` existiert ganz normal in `~/qmk_firmware`,
kein Umschalten von `qmk_home` nötig.

- MCU: **STM32L432** (Cortex-M4)
- RGB: SNLED27351-LED-Treiber, **62 LEDs**, `RGB_MATRIX_ENABLE=yes`
- Matrix: 5×14 (`LAYOUT_60_iso`)
- Kein Encoder
- Bootloader: STM32-eigener ROM-DFU (`0483:df11` in `lsusb`), Flash über
  `dfu-util` (kein UF2 drag&drop)
- USB-ID: echte Keychron-Hersteller-ID (`3434:0342`)

Nicht speicherbeschränkt (`KEYBOARD` matcht keinen der kbd8x_mk3/bella-
Filter in `users/neo/rules.mk`) - volle Ausstattung: kompletter
Leader-Baum, Dynamic Macros, echtes RGB_MATRIX, alle Tap-Dance/
Hold-Tuning-Hooks aus `keymap_logic.c`. Details siehe `BELEGUNG.md`.

## ⚙️ Build
```bash
qmk compile -kb keychron/v4/iso -km neo
```
Ganz normaler mainline-Build - kein Fork-Umschalten wie bei V1 Max/V3 Max
nötig. Gebaut am 2026-09-17: sauberer Compile, nur die übliche harmlose
`lto-wrapper`-Warnung (serielle LTRANS-Kompilierung, kein Fehler).
Binärgröße `keychron_v4_iso_neo.bin` ≈ 62,1 KB.

## 🔌 Flashen (DFU)
Bootloader-Modus am Board auslösen (Board zeigt sich danach als
`0483:df11 STM Device in DFU Mode` in `lsusb`), dann:
```bash
qmk flash -kb keychron/v4/iso -km neo -bl dfu-util
```

**Geflasht und live verifiziert (2026-09-17):** `dfu-util`-Flash lief
sauber durch (Erase + Download je 100%), Board bootet danach wieder als
`3434:0342 Keychron V4` mit aktivem Raw-HID-Interface.

**Wichtig - EEPROM-Falle:** VIAs Dynamic Keymap liegt im EEPROM, nicht im
PROGMEM-Array aus `keymap.c`. Reines Reflashen übernimmt eine geänderte
Tastenbelegung **nicht automatisch**. Nach jedem Flash mit geänderter
Keymap einmal `EE_CLR` drücken (liegt auf der `_SYS`-Ebene neben
`QK_BOOT`, siehe `BELEGUNG.md`) - das löscht dabei aber auch die
VIA-Makros mit, die danach neu importiert werden müssen (siehe unten).

## 💡 VIA
Trotz echter Keychron-Hersteller-USB-ID (`3434:0342`, wie beim V3 Max)
funktioniert VIA hier **problemlos** - anders als beim V3 Max/kbd8x_mk3
gibt es hier keinen fehlschlagenden Online-Fetch, kein board-eigenes
`via.json` nötig. Live bestätigt (2026-09-17).

## 🎛️ Makros
- `DYNAMIC_KEYMAP_MACRO_COUNT = 15`, Puffer 1500 B (mehr Slots als
  kbd8x_mk3 (12), obwohl der Byte-Puffer etwas kleiner ist (1591 B) -
  kürzere Durchschnittsmakros pro Slot).
- Erstsync 2026-09-17: von der laufenden kbd8x_mk3 exportiert
  (`tools/via_macros.py export --path /dev/hidraw5`, 10/12 Slots belegt)
  und auf dieses Board importiert (`--path /dev/hidraw8` - Raw-HID-Pfad
  über `tools/via_macros.py list` ermitteln, ändert sich je nach
  USB-Reihenfolge), per Re-Export byte-identisch verifiziert. Nach dem
  `EE_CLR`-Reset (siehe Abschnitt Flashen) ein zweites Mal importiert und
  erneut byte-identisch verifiziert.

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V4 ISO Board (60%), custom Keymap gegen
       mainline QMK (kein Fork nötig).
```
