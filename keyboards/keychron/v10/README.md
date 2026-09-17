# Keychron V10 ISO (keymap `neo`)

## 🧩 Überblick
Offizielles Keychron-Board, TKL+ ISO mit Knob, **kein Bluetooth/2.4G**
(anders als V1 Max/V3 Max/Q10 Pro) - baut **gegen mainline QMK**, kein
Fork nötig (wie Q6/V3).

- MCU: STM32L432 (`stm32-dfu`-Bootloader). Custom-Matrix
  (`CUSTOM_MATRIX = lite`, `matrix.c`), RGB-Matrix (SNLED27351-Treiber).
- Layout: `LAYOUT_iso_90` - 90 Tasten, TKL-Formfaktor mit Knob, Ins/Del/
  Home/PgUp/PgDn/Pfeilen, **aber ohne End-Taste** (Keychrons eigenes
  Referenz-Keymap hat dort ebenfalls keine). Eigener Formfaktor-Header:
  `users/neo/formfactors/ff_90_iso_v10.h` (`KEYMAP_90_ISO_V10(...)`).
- Alle 12 Ebenen aktiv (QWERTZ, 4× NEOQWERTZ, 4× NOTED, FN, SYS, RGB).

## ⚙️ Build & Flash
Normaler mainline-Workflow, kein `qmk_home`-Umschalten nötig:
```bash
qmk compile -kb keychron/v10/iso_encoder -km neo
```
Bootloader-Modus am Board auslösen, dann:
```bash
qmk flash -kb keychron/v10/iso_encoder -km neo
```
Läuft über Standard-`dfu-util`, kein Zusatz-Tool nötig.

## 🎛️ DIP-Schalter
`dip_switch_update_user` im Keymap selbst (wie bei V3/Q3/Q6) - schaltet
zwischen `_QWERTZ` (Win-Stellung) und `_NEOQWERTZ1` (Mac-Stellung).

## ⚠️ Noch nicht am echten Board verifiziert
Diese Annahmen stammen aus Keychrons eigenem Referenz-Keymap
(`iso_encoder/keymaps/default`) und dem `rgb_matrix`-Layout in
`keyboard.json`, nicht aus einem Test am physischen Board - bitte nach
dem ersten Flash gegenprüfen (z. B. mit `qmk console`/Raw-HID, nicht nur
durch Tippen):

- **Fn-Taste zwischen B und N**: Es gibt laut Matrix-Definition eine
  zusätzliche, reale Taste zwischen den physischen Positionen von `B`
  und `N` (Keychrons Referenz-Keymap legt dort nochmal `KC_B` rein - ein
  reiner Lückenfüller). In Analogie zum Q10 Pro (dort vom Nutzer
  bestätigt: das ist dort die Fn-Taste) hier ebenfalls testweise
  `MO(_FN)` belegt (`ff_90_iso_v10.h`). Falls diese Taste bei dir gar
  nicht existiert oder etwas anderes ist, dort anpassen.
- **Geteilte Leertaste**: Row5 hat zwei Leertasten-Switches mit einer
  Fn-Taste dazwischen (`Leer - Fn - Leer`), wie beim Q10 Pro. Reihenfolge
  der übrigen Modifier ist Ctrl-Gui-Alt (PC-Konvention wie der Rest
  dieses Userspace), nicht Keychrons eigene Mac/Win-Sonderreihenfolge.
- **Mute-Taste** links von Esc (Zeile 0, Spalte A) - einzige echte Taste
  in dieser Spalte, alle anderen Zeilen haben dort laut Referenz-Keymap
  keine Taste (`KC_NO` in `ff_90_iso_v10.h`).

## 💡 VIA
Kein eigenes `via.json` nötig - V10 ist offizielles Keychron-Produkt und
bei VIA online registriert (wie Q6).

## 🎛️ Makros
Über `tools/via_macros.py` synchron zu den anderen Boards zu halten
(gleiches Personen-Makro-Set, 12 Slots) - nach dem ersten Flash mit
`import` einspielen.

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Basis: offizielles Keychron V10 ISO Board, custom Keymap gegen mainline
       QMK (~/qmk_firmware)
```
