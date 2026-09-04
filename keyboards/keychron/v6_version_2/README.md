# Keychron V6 (Version 2, WB32) ISO Encoder (keymap `neo`)

## 🧩 Überblick
Offizielles Keychron-Board, 96%-ISO mit Nummernblock und Lautstärke-Knob.
**Nicht dasselbe wie `keyboards/keychron/v6` in mainline QMK** - diese
Definition ist für eine neuere Hardware-Revision, die Keychron nie in
ihren eigenen `master` zurückgemergt hat (analog zu `v1_max`, siehe
`../v1_max/README.md`).

- MCU: **WB32F3G71** (Westberry Tech), nicht STM32L432 wie die
  Mainline-`keychron/v6`-Definition. Erkennbar am Bootloader-Gerät in
  `lsusb`: `342d:dfa0 Westberry Tech. WB Device in DFU Mode` (statt
  `0483:df11 STM Device in DFU Mode`).
- USB PID im Normalbetrieb: `3434:0368` - **verschieden** von mainline
  `v6/iso_encoder`s PID `0x0363`. Das war der entscheidende Hinweis, dass
  es sich um zwei unterschiedliche Boards/Definitionen handelt.
- Layout: `LAYOUT_iso_110` - visuell identisch zu mainline `v6`
  (gleiche x/y-Reihenfolge in den Argumenten), aber andere elektrische
  Matrix (andere `matrix:`-Werte pro Taste). Für unsere positions-basierten
  `LAYOUT_ISO`-Makros (`users/neo/formfactors/ff_96_iso_q6_v6.h`) ist das
  irrelevant - der Keymap-Code ist 1:1 identisch zu dem, was für mainline
  `v6` geschrieben wurde.
- Matrix: 6 Rows × 20 Cols (`info.json` → `matrix_pins`).
- Encoder: 1× Rotary (`B14`/`B15`), Lautstärke auf allen 12 Ebenen.
- DIP-Switch: vorhanden, schaltet aber (siehe unten) **nicht** wie bei
  den anderen Boards zwischen `_QWERTZ`/`_NEOQWERTZ1`.

## ⚙️ Build - braucht den Keychron-Fork, nicht mainline
Wie bei `v1_max`: mainline `~/qmk_firmware` kennt `keychron/v6_version_2`
nicht. Es muss gegen `~/keychron-qmk/qmk_firmware` (Branch `2025q3`)
gebaut werden.

```bash
qmk config user.qmk_home=/home/dieter/keychron-qmk/qmk_firmware
cd /home/dieter/keychron-qmk/qmk_firmware
make keychron/v6_version_2/iso_encoder:neo QMK_USERSPACE=/home/dieter/qmk_userspace
qmk config user.qmk_home=/home/dieter/qmk_firmware   # danach zurücksetzen!
```

## 🔌 Flashen (WB32-DFU, kein Standard-`dfu-util`)
Der WB32-Chip nutzt **nicht** das Standard-USB-DFU-Protokoll
(`bInterfaceClass 255`, vendor-spezifisch), sondern ein eigenes. QMKs
`dfu-util`-Pfad greift hier nicht - es braucht `wb32-dfu-updater_cli`
(AUR: `wb32-dfu-updater_cli-git`, `yay -S wb32-dfu-updater_cli-git`).

Einmalig nötig, da es dafür (anders als für `0483:df11`) noch keine
udev-Regel gab:
```bash
echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="342d", ATTRS{idProduct}=="dfa0", TAG+="uaccess"' \
  | sudo tee /etc/udev/rules.d/60-wb32-dfu.rules
sudo udevadm control --reload-rules && sudo udevadm trigger
```
(Board danach kurz aus dem Bootloader raus und wieder rein, damit die
Regel greift - sonst `LIBUSB_ERROR_ACCESS`.)

Bootloader-Modus wie gewohnt am Board auslösen (zeigt sich danach als
`342d:dfa0 Westberry Tech. WB Device in DFU Mode` in `lsusb`), dann:
```bash
make keychron/v6_version_2/iso_encoder:neo:flash QMK_USERSPACE=/home/dieter/qmk_userspace
```
(`qmk_home` muss dafür wie oben auf den Fork zeigen.) Der `:flash`-Target
ruft automatisch `wb32-dfu-updater_cli` auf, schreibt und resettet das
Board.

## 🐛 Bekannte Baustellen im Vendor-Code (Fork `2025q3`, Stand 2026-09-04)
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Build bricht in `quantum/rgb_matrix/animations/pixel_rain_anim.h` mit `-Werror=unused-but-set-variable` | Vendor-/Upstream-Bug (`region_mask` wird gesetzt, aber nie gelesen) - gleiches Muster wie `lkbt51_dfu_tx` bei V1 Max | `CFLAGS += -Wno-error=unused-but-set-variable` in `keymaps/neo/rules.mk` (bereits gesetzt) |
| Linker: `multiple definition of 'dip_switch_update_user'` | `keyboards/keychron/common/factory_test.c` im Fork definiert `dip_switch_update_user` bereits **nicht-weak** (nur Factory-Test-Reporting, kein Layer-Wechsel) | Kein eigenes `dip_switch_update_user` im Keymap definieren (anders als V3/Q3/kbd8x_mk3) |
| DIP-Schalter schaltet den falschen Layer | `keyboards/keychron/v6_version_2/v6_version_2.c` hat ein eigenes, hartkodiertes `dip_switch_update_kb`: `default_layer_set(1UL << (active ? 0 : 2))` - Index 0/2 sind in unserem `enum layers` `_QWERTZ`/`_NEOQWERTZ2`, nicht `_QWERTZ`/`_NEOQWERTZ1` wie bei den anderen Boards | Noch offen - würde einen Patch im Fork brauchen (`v6_version_2.c` ist Board-Code, nicht Keymap-Code, liegt außerhalb von `qmk_userspace`). Bis dahin: DIP-Schalter auf diesem Board weicht von der sonstigen Konvention ab. |

## 💡 VIA
- `via.json` liegt hier im Verzeichnis - 1:1 Kopie von Keychrons eigener
  Definition (`~/keychron-qmk/qmk_firmware/keyboards/keychron/
  v6_version_2/via_json/v6_version_2_iso_encoder.json`), PID `0x0368`
  passt zum tatsächlichen Gerät.

## 🎛️ Makros
- Werden über `tools/via_macros.py` synchron zu den anderen Boards
  gehalten (gleiches Personen-Makro-Set, 12 Slots).

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V6 (Version 2 / WB32) ISO Encoder Board,
       custom Keymap gegen Keychrons eigenen QMK-Fork
       (~/keychron-qmk/qmk_firmware, Branch 2025q3)
```
