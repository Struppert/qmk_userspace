# Keychron V1 Max ISO Encoder (keymap `neo`)

## 🧩 Überblick
Offizielles Keychron-Board (Wireless: 2.4G-Dongle + Bluetooth, LKBT51-
BT-Chip), 75%-ISO mit Encoder. Im Gegensatz zu allen anderen Boards in
diesem Userspace ist das hier **das einzige Wireless-Board** und
**baut nicht gegen mainline QMK**, sondern gegen Keychrons eigenen Fork
(siehe Abschnitt Build).

- MCU: **STM32F401** (Cortex-M4) - potenteste MCU aller hier verwendeten
  Boards, deutlich mehr RAM/Flash als kbd8x_mk3 (STM32F103) oder die
  günstigeren Keychron-Modelle.
- RGB: SNLED27351-LED-Treiber, 82 LEDs, `RGB_MATRIX_ENABLE=yes`
  (`RGBLIGHT_ENABLE=no` - anders als kbd8x_mk3, dieses Board hat die
  "richtige" RGB-Matrix-Hardware).
- Wireless: LKBT51-BT-Chip (`keyboards/keychron/common/wireless/`),
  2.4G-Dongle-Modus (`P2P4G`), 3 BT-Host-Slots (`BT_HST1-3`).
- Bootloader: STM32-eigener ROM-DFU (`0483:df11` in `lsusb`), **UF2
  drag&drop wie bei kbd8x_mk3 gibt es hier nicht** - Flash läuft über
  `dfu-util`.

## ⚙️ Build - braucht den Keychron-Fork, nicht mainline
Keychron hat seine Wireless-Änderungen **nie in ihren eigenen `master`
zurückgemergt** (Stand 2026-08-31) - `keyboards/keychron/v1_max` und
`keyboards/keychron/common/wireless/` existieren nur auf separaten
"Playground"-Branches. Für alle anderen Boards in diesem Userspace
(Q3/Q4/Q8/V1/V2/V3/V4) reicht mainline `~/qmk_firmware` - nur für dieses
Board wird `~/keychron-qmk/qmk_firmware` gebraucht.

**Aktuell ausgecheckter Branch: `2025q3`** (Stand 2026-08-07, 592 Commits
hinter Keychrons `master`). Vorher stand der Fork auf `wireless_playground`
(Stand 2026-03-16, 3122 Commits hinter `master`, deutlich verwaister
Branch) - der Wechsel auf `2025q3` hat spürbar das Tippgefühl verbessert
(vermutlich Timing-/Scheduling-Fixes irgendwo in den 2707 Commits
Unterschied, nicht auf einen einzelnen Commit zurückgeführt). Falls die
V1 Max jemals wieder "komisch" wirkt: erst prüfen, ob der Fork wieder
veraltet ist (`git log -1` in `~/keychron-qmk/qmk_firmware` gegen
`origin/2025q3` vergleichen), bevor an `users/neo` gesucht wird.

```bash
qmk config user.qmk_home=/home/dieter/keychron-qmk/qmk_firmware
cd /home/dieter/keychron-qmk/qmk_firmware
make keychron/v1_max/iso_encoder:neo QMK_USERSPACE=/home/dieter/qmk_userspace
qmk config user.qmk_home=/home/dieter/qmk_firmware   # danach zurücksetzen!
```
Die normale `qmk compile -kb ... -km neo`-CLI dieses (älteren) Forks
akzeptiert das externe Userspace-Overlay nicht zuverlässig ("Invalid
keymap argument") - `make ... QMK_USERSPACE=...` direkt verwenden.

## 🔌 Flashen (DFU, nicht UF2)
Bootloader-Modus wie gewohnt am Board auslösen (Board zeigt sich danach
als `0483:df11 STM Device in DFU Mode` in `lsusb`), dann:
```bash
make keychron/v1_max/iso_encoder:neo:dfu-util QMK_USERSPACE=/home/dieter/qmk_userspace
```
(`qmk_home` muss dafür wie oben auf den Fork zeigen.) Baut, flasht per
`dfu-util` und das Board bootet automatisch neu.

## 🐛 Bekannte Baustellen im Vendor-Code (Stand 2026-08-31)
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Build bricht in `lkbt51.c` (`lkbt51_dfu_tx`) mit `-Werror=unused-but-set-variable` | Vendor-Bug (ungenutzte `checksum`-Variable) - existiert in `wireless_playground` UND `2025q3`, nicht upstream gefixt | `CFLAGS += -Wno-error=unused-but-set-variable` in `keymaps/neo/rules.mk` (bereits gesetzt) |
| `KC_MS_WH_DOWN`/`KC_MS_WH_UP` unbekannt beim Bauen gegen `2025q3` | QMK-Core hat die `KC_`-Präfix-Aliase entfernt (gleiches Muster wie bei kbd8x_mk3s `RGB_MATRIX_*`-Umbenennungen) | Auf `MS_WHLD`/`MS_WHLU` umgestellt (bereits im Keymap) |
| `STATIC_ASSERT("Number of encoder_map layers doesn't match...")` | War nur ein Folgefehler des `KC_MS_WH_*`-Fehlers oben (Compile-Fehler in `encoder_map[]` verwirrt die Introspection), kein eigener Bug | Verschwand nach obigem Fix von selbst |
| `qmk compile` meldet "Invalid keymap argument" | Fork-eigene `qmk`-CLI-Tooling kennt das externe Userspace-Overlay nicht zuverlässig | `make ... QMK_USERSPACE=...` statt `qmk compile` verwenden (siehe Build-Abschnitt) |

## 💡 VIA
- `via.json` liegt hier im Verzeichnis - **1:1 Kopie von Keychrons
  eigener offizieller Definition** (`~/keychron-qmk/qmk_firmware/
  keyboards/keychron/v1_max/via_json/v1_max_iso_encoder.json`), da
  Matrix-Maße (6×16) und Protokollversion unverändert sind - unser
  Custom-Keymap ändert nichts an der physischen Matrix. Enthält ein
  fertiges RGB-Matrix-Lighting-Menü.
- Da V1 Max ein offizielles Keychron-Produkt ist, kennt VIA es evtl.
  schon aus dem eigenen Katalog automatisch (anders als kbd8x_mk3, das
  nie offiziell registriert wurde) - falls nicht, die `via.json` hier
  manuell als Draft Definition laden (Design-Tab → Load Draft
  Definition), wie bei kbd8x_mk3.

## 🎛️ Makros
- `DYNAMIC_KEYMAP_MACRO_COUNT = 12` (an kbd8x_mk3 angeglichen, war
  ursprünglich 8) - Puffer hat mit `WEAR_LEVELING_LOGICAL_SIZE = 6 KiB`
  reichlich Platz übrig.
- Makros werden über `tools/via_macros.py` synchron zu den anderen
  Boards gehalten (gleiches Personen-Makro-Set).

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V1 Max ISO Encoder Board, custom Keymap
       gegen Keychrons eigenen QMK-Fork (~/keychron-qmk/qmk_firmware,
       Branch 2025q3)
```
