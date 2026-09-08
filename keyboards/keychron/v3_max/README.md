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
(`qmk_home` muss dafür wie oben auf den Fork zeigen.)

**Geflasht und live verifiziert (2026-09-08):** `dfu-util`-Flash lief
sauber durch, Board bootet danach als `3434:0934 Keychron V3 Max` in
`lsusb`. Per Raw-HID (`tools/via_macros.py`s `RawHid`-Klasse,
`id_dynamic_keymap_get_keycode`) bestätigt, dass die `_SYS`-Ebene auf
U/I/O/P/Ü tatsächlich `BT_HST1-3`/`P2P4G`/`BAT_LVL` liefert (`0x7e0b`-
`0x7e0f`), nicht `KC_NO` - siehe `BLUETOOTH.md` für den vollen Ablauf,
inkl. eines dabei gefundenen und korrigierten LED-Index-Fehlers (falsche
Matrixzeile für die Pairing-Blink-LED, erste Version hätte auf der
Zahlenreihe statt auf U/I/O/P geblinkt). **Noch nicht getestet:**
tatsächliches Bluetooth-Pairing und ob die Indikator-LED optisch wirklich
an der richtigen Taste blinkt - der Raw-HID-Check bestätigt nur die
Keycode-Zuordnung im EEPROM, kein sichtbares LED-Verhalten.

## 🎛️ Bluetooth
Gleicher Mechanismus wie beim V1 Max, siehe `BLUETOOTH.md` in diesem
Verzeichnis - einziger Unterschied sind die LED-Indizes (88 statt 82
LEDs) und dass hier **keine** redundante `_WIN_FN`-Ebene existiert (siehe
dort, bewusste Vereinfachung: nur der Capslock+U/I/O/P/Ü-Weg).

## 💡 VIA
- **Automatische Erkennung schlägt fehl:** Da dieses Board die echte
  Keychron-Hersteller-USB-ID (`3434:0934`) meldet, versucht VIA beim
  Verbinden automatisch, seine eigene Online-Katalog-Definition zu laden
  ("Fetching v3 definition failed" - "v3" ist hier VIAs
  Definitions-Schema-Version, nicht der Board-Name). Dieser Fetch scheint
  in dieser Umgebung generell zu scheitern (auch beim manuellen Laden
  über den Design-Tab mit deaktiviertem V2-Schalter) - die ID wurde
  **bewusst nicht geändert**, um die Geräteidentität dieses echten Boards
  nicht zu verfälschen.
- **Workaround: `via.json` im V2-Schema.** Ursprünglich war die Datei
  hier eine 1:1-Kopie von Keychrons eigener V3-Schema-Definition
  (`~/keychron-qmk/qmk_firmware/keyboards/keychron/v3_max/via_json/
  v3_max_iso_encoder.json`, inkl. RGB-Matrix-Lighting-Menü) - die scheitert
  am V2-Schema-Validator (fehlendes Pflichtfeld `lighting`, unerlaubte
  Felder `keycodes`/`menus`). Auf reines V2-Schema reduziert (`lighting:
  "none"`, RGB-Lighting-Menü entfernt) - das lädt rein lokal ohne
  Netzwerk-Abhängigkeit. **Tradeoff:** kein RGB-Tab in VIA - RGB-Steuerung
  läuft weiterhin über die `_RGB`-Ebene direkt am Board (siehe
  `BELEGUNG.md`).
- Import: VIA → Settings → "Show Design tab" aktivieren → Design-Tab →
  **V2-Schalter aktivieren** → "Load Draft Definition" → diese Datei
  auswählen. Matrix-Maße (6×17) unverändert.

## 🎛️ Makros
- `DYNAMIC_KEYMAP_MACRO_COUNT = 12` wie bei V1 Max/kbd8x_mk3.
- Makros werden über `tools/via_macros.py` synchron zu den anderen
  Boards gehalten (gleiches Personen-Makro-Set). Erstsync 2026-09-08:
  von der laufenden kbd8x_mk3 exportiert (`--path /dev/hidraw5`, 10/12
  Slots belegt) und auf dieses Board importiert (`--path /dev/hidraw8` -
  Raw-HID-Interface über `tools/via_macros.py list` finden, Pfad ändert
  sich je nach USB-Reihenfolge), per Re-Export byte-identisch verifiziert.
  Board hat mit 3102 statt 1591 Byte Makro-Puffer deutlich mehr Platz als
  kbd8x_mk3 - alle 12 Slots passen ohne Kürzung.

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V3 Max ISO Encoder Board, custom Keymap
       gegen Keychrons eigenen QMK-Fork (~/keychron-qmk/qmk_firmware,
       Branch 2025q3) - Formfaktor von V3 (TKL) + Wireless-Logik von
       V1 Max übernommen.
```
