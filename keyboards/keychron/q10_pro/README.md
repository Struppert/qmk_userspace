# Keychron Q10 Pro ISO Encoder (keymap `neo`)

## 🧩 Überblick
Echtes, physisch vorhandenes Board (bestätigt via `lsusb`: `3434:06A1
Keychron Keychron Q10 Pro`, Keychron Launcher zeigt "Q10 iso knob") -
**nicht** zu verwechseln mit dem ähnlich benannten, aber hardwaretechnisch
anderen "Q10 Max" (`3434:08A1`). `keychron/q10_pro` existiert auf keinem
aktuell gepflegten Branch von Keychrons eigenem QMK-Fork mehr (nur noch auf
den alten `wireless_playground`/`wls_2025q1`-Branches) - dieses Board wurde
daher **auf `2025q3` portiert**, nicht gegen den veralteten Branch gebaut.

- MCU: **STM32L432** (Cortex-M4).
- RGB: SNLED27351-LED-Treiber über **I2C** (nicht SPI wie beim moderneren
  Q10 Max), **90 LEDs**.
- Matrix: **6×16, HC595-Schieberegister** (nicht direktes GPIO-Matrix-
  Scanning wie Q10 Max) - `keyboards/keychron/common/matrix.c`, Pins
  `HC595_STCP=B0, HC595_SHCP=A1, HC595_DS=A7` (Bibliotheks-Defaults, die
  auf `2025q3` explizit gesetzt werden müssen, da der automatische Fallback
  entfernt wurde).
- Wireless: **CKBT51-BT-Chip** (`keyboards/keychron/common/wireless/`,
  `bluetooth.mk`) - **kein 2.4G-Dongle-Support in diesem Port** (die alte
  Firmware auf `wireless_playground` hatte dafür nie Pins/Code, anders als
  Q10 Max/V1 Max mit ihrem LKBT51-Chip).
- Encoder: 1 Stück (`ENCODER_ENABLE`).
- Makro-Spalte: eine physische Extra-Spalte ganz links (M1-M5, matrix-
  Spalte A auf Reihe 1-5) - Dynamic-Macro-Wiedergabe, siehe `BELEGUNG.md`.
- Bootloader: STM32-eigener ROM-DFU (`0483:df11` in `lsusb` im
  Bootloader-Modus), Flash über `dfu-util` (kein UF2 drag&drop).

## ⚙️ Portierung (Firmware-Seite, `~/keychron-qmk/qmk_firmware`)
Board-Dateien (`keyboards/keychron/q10_pro/`) existierten auf `2025q3`
nicht und wurden neu angelegt, basierend auf den alten Q10-Pro-Werten
(Matrix/Encoder/LED-Tabelle/PID unverändert übernommen) plus dem
strukturellen Muster von `keychron/k8_pro` (teilt exakt dieselbe
HC595-Matrix/I2C-RGB/CKBT51-Architektur, viel näher als das äußerlich
ähnlichere aber hardwaretechnisch andere `q10_max`). Dabei gefundene und
behobene Probleme:
- Physisches Layout: negative y-Koordinaten (vom alten Schema erlaubt, vom
  neuen abgelehnt) - alle 91 Tasten um +0.55 verschoben (rein relativ).
- Diverse Makro-Umbenennungen (`RGB_DISABLE_WHEN_USB_SUSPENDED` →
  `RGB_MATRIX_SLEEP`, `USB_BT_MODE_SELECT_PIN` → `BT_MODE_SELECT_PIN`,
  `HOST_LED_MATRIX_LIST` → `BT_INDCATION_LED_MATRIX_LIST`, `FN_KEY1/2` →
  `FN_KEY_1/2`, `LOW_BAT_IND_INDEX` muss jetzt eine Array-Literal-Liste
  sein statt Skalar).
- Eigene Tastencodes (`KC_LOPTN`, `BT_HST1-3` etc.) sind inzwischen
  **Standard-Framework-Keycodes** (`keychron_common.h`) - die alte
  `q10_pro.h`-Enum kollidierte damit und wurde entfernt, `q10_pro.c` auf
  die minimale `k8_pro`-Struktur reduziert.
- LED-Kanalbenennung im I2C-Treiber komplett von `A_1`…`I_16` auf
  `CBx_CAy` umbenannt (Werte identisch, nur Namen neu, mechanisch für alle
  270 Werte im 90-LED-Array übersetzt).
- **Geteilter Framework-Bug gefunden und behoben** in
  `common/wireless/ckbt51.c` (betrifft auch `k8_pro` und jedes zukünftige
  CKBT51-Board): Der `wireless_transport`-Struct-Initializer war nicht an
  ein neues Pflichtfeld (`update_bat_level`) angepasst - verifiziert durch
  Kompilieren des unveränderten `k8_pro` (gleicher Fehler). Auf designated
  initializers umgestellt.

## ⚙️ Build - braucht den Keychron-Fork, nicht mainline
```bash
qmk config user.qmk_home=/home/dieter/keychron-qmk/qmk_firmware
cd /home/dieter/keychron-qmk/qmk_firmware
make keychron/q10_pro/iso_encoder:neo QMK_USERSPACE=/home/dieter/qmk_userspace
qmk config user.qmk_home=/home/dieter/qmk_firmware   # danach zurücksetzen!
```
Gebaut und verifiziert: sauberer Compile, nur die übliche harmlose
`lto-wrapper`-Warnung (serielle LTRANS-Kompilierung, kein Fehler).
Binärgröße `keychron_q10_pro_iso_encoder_neo.bin` ≈ 78,5 KB.

## 🔌 Flashen (DFU, nicht UF2)
Bootloader-Modus wie gewohnt am Board auslösen (Board zeigt sich danach
als `0483:df11 STM Device in DFU Mode` in `lsusb`), dann:
```bash
make keychron/q10_pro/iso_encoder:neo:dfu-util QMK_USERSPACE=/home/dieter/qmk_userspace
```
(`qmk_home` muss dafür wie oben auf den Fork zeigen.)

**Noch nicht geflasht/live verifiziert** - der Compile-Schritt bestätigt
nur, dass die Firmware baut, nicht das tatsächliche Verhalten am echten
Gerät (Tastenbelegung, LEDs, Bluetooth). Siehe `BELEGUNG.md`/`BLUETOOTH.md`
für die offenen Verifizierungspunkte.

## 🎛️ Bluetooth
Siehe `BLUETOOTH.md` in diesem Verzeichnis - **kein 2.4G-Modus** (anders
als bei V1 Max/V3 Max/Q10 Max), nur BT+USB.

## 💡 VIA
`via.json` im V2-Schema, aus der tatsächlichen physischen Layout-Tabelle
des Firmware-Ports (`keyboards/keychron/q10_pro/iso_encoder/keyboard.json`
im Fork) generiert - Matrixpositionen (`row,col`) exakt, x/y-Offsets pro
Zeile berechnet, aber **nicht pixelgenau** zur echten Silhouette (die
Kerbe rund um Encoder/geteilte Leertaste wurde zugunsten eines einfachen,
sicher ladbaren Layouts eingeebnet - rein kosmetisch, ändert nichts an der
Tastenzuordnung). Import: VIA → Settings → "Show Design tab" aktivieren →
Design-Tab → V2-Schalter aktivieren → "Load Draft Definition" → diese
Datei auswählen.

## 🎛️ Makros
`DYNAMIC_KEYMAP_MACRO_COUNT = 12` wie bei den anderen Boards - noch nicht
mit `tools/via_macros.py` synchronisiert (setzt ein geflashtes Board
voraus).

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: Keychron Q10 Pro ISO Encoder Board, auf keychron-qmk-Fork Branch
       2025q3 portiert (Board existierte dort nicht, nur auf alten
       wireless_playground/wls_2025q1-Branches) - Wireless-/Matrix-Muster
       von k8_pro übernommen, alle Q10-Pro-eigenen Werte (Pins, LED-
       Tabelle, PID, Custom-Keycodes) aus der alten Firmware erhalten.
```
