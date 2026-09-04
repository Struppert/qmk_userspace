# Keychron Q6 ISO (keymap `neo`)

## 🧩 Überblick
Offizielles Keychron-Board, 96%-ISO mit Nummernblock, **kein**
Lautstärke-Knob (im Gegensatz zur V6 dieses Userspace, siehe
`../v6_version_2/README.md`). Baut **gegen mainline QMK** - anders als
V1 Max und V6 (Version 2) braucht dieses Board keinen Fork.

- MCU: STM32 (Standard-ROM-DFU, `0483:df11` in `lsusb` im
  Bootloader-Modus) - Custom-Matrix (`CUSTOM_MATRIX = lite`,
  `matrix.c`), aber keine sonstigen Vendor-Eigenheiten.
- USB PID im Normalbetrieb: `3434:0162`.
- Layout: `LAYOUT_iso_109` - 96%-ISO-Formfaktor mit Nummernblock, ein
  Slot weniger als die encoder-lose Matrix der V6 (`LAYOUT_iso_110`),
  da hier der Encoder-Klick-Kontakt physisch fehlt. Gemeinsamer
  Formfaktor-Header mit V6: `users/neo/formfactors/ff_96_iso_q6_v6.h`
  (`KEYMAP_96_ISO_Q6(...)`).
- Alle 12 Ebenen aktiv (QWERTZ, 4× NEOQWERTZ, 4× NOTED, FN, SYS, RGB) -
  Q6 hat RGB_MATRIX, daher (anders als kbd8x_mk3) mit `_RGB`-Ebene.

## ⚙️ Build & Flash
Normaler mainline-Workflow, kein `qmk_home`-Umschalten nötig:
```bash
qmk compile -kb keychron/q6/iso -km neo
```
Bootloader-Modus am Board auslösen (zeigt sich als `0483:df11
STMicroelectronics STM Device in DFU Mode` in `lsusb`), dann:
```bash
qmk flash -kb keychron/q6/iso -km neo
```
Läuft über Standard-`dfu-util`, kein Zusatz-Tool nötig.

## 🎛️ DIP-Schalter
`dip_switch_update_user` im Keymap selbst (wie bei V3/Q3/kbd8x_mk3) -
schaltet zwischen `_QWERTZ` (Win-Stellung) und `_NEOQWERTZ1`
(Mac-Stellung). Kein Konflikt mit Vendor-Code, da hier (anders als bei
V6/Version 2) kein Keychron-Fork mit eigenem
`dip_switch_update_user`/`dip_switch_update_kb` im Spiel ist.

## 🐛 Offen (Stand 2026-09-04)
`(` (Shift+8, `DE_SS`... nein, einfach `KC_LSFT`+`KC_8` auf `_QWERTZ`)
erzeugt auf diesem Board ein falsches/kaputtes Zeichen ("Unicode-Fehler")
- auf Linux-Host mit Standard-Deutsch-Layout, Board bestätigt auf
`_QWERTZ` (nicht `_NEOQWERTZ1`) stehend, also **nicht** das
DIP-Schalter/Ebenen-Mismatch, das zuerst vermutet wurde. Ursache noch
unklar - Shift+8 ist ein reiner `KC_8`+Shift, keine UC()/Unicode-Taste,
daher auch die Fehlermeldung überraschend. Mögliche nächste
Ansatzpunkte, wenn wieder Zeit ist:
- Vergleich mit V3 (baut gegen dasselbe mainline QMK, gleiches
  `QWERTZ60_ROW1`/`NUMBER_ROW` - funktioniert dort einwandfrei) -
  spricht gegen einen Bug in `users/neo/layouts/numbers.h` selbst.
- Q6 hat `CUSTOM_MATRIX = lite` (`matrix.c`, siehe `rules.mk`) - evtl.
  eine Scan-/Debounce-Eigenheit dieses Custom-Matrix-Codes, die
  spezifisch Shift+8 (oder generell schnelle Shift+Zahl-Kombos)
  falsch reportet (Ghosting? Prüfen mit `qmk console`/Debug-Matrix-Print
  während des Tippens).
- Prüfen, ob das Problem auch auf anderen physischen Tasten der
  Zahlenreihe auftritt, oder wirklich nur bei "8"/Shift+8.

## 💡 VIA
Kein eigenes `via.json` nötig - Q6 ist offizielles Keychron-Produkt und
bei VIA online registriert (anders als kbd8x_mk3).

## 🎛️ Makros
Über `tools/via_macros.py` synchron zu den anderen Boards gehalten
(gleiches Personen-Makro-Set, 12 Slots).

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron Q6 ISO Board, custom Keymap gegen mainline
       QMK (~/qmk_firmware)
```
