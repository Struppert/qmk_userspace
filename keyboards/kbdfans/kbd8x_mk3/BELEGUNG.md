# KBD8X MK3 – Tastenbelegung (Keymap `neo`)

Ergänzung zu `README.md` (dort: Hardware/Bootloader/RGB/VIA). Diese Datei
beschreibt, was auf welcher Taste liegt - Stand des kompilierten Default-
Keymaps (`keymaps/neo/keymap.c` + `users/neo/layouts/*.h`), **nicht**
zwingend das, was gerade tatsächlich geflasht ist, falls über VIA live
umbelegt wurde (VIA-Änderungen landen im EEPROM, nicht in dieser Datei -
`id_dynamic_keymap_reset` im README setzt sie auf genau diesen Stand zurück).

## 🗂️ Layer-Übersicht

| # | Name | Typ | Erreichbar über |
|--:|:--|:--|:--|
| 0 | `_QWERTZ` | Default | Boot-Default, oder `_SYS`→`DF(_QWERTZ)` |
| 1 | `_NEOQWERTZ1` | Default | nur über `_SYS`→`DF(_NEOQWERTZ1)` |
| 2 | `_NEOQWERTZ2` | Momentary | Neo-Shift-Ebene, über `NEO_SHIFT` (auf Ebene 1) |
| 3 | `_NEOQWERTZ3` | Momentary | Capslock-Tap-Dance halten (Ebene 1-3), oder L3-Taste |
| 4 | `_NEOQWERTZ4` | Momentary | `L4_MO_NEO` (rechtes AltGr auf Ebene 1-3) |
| 5 | `_NOTED1` | Default | nur über `_SYS`→`DF(_NOTED1)` |
| 6 | `_NOTED2` | Momentary | Noted-Shift-Ebene, über `NOTED_SHIFT` |
| 7 | `_NOTED3` | Momentary | Capslock-Tap-Dance halten (Ebene 5-7), oder L3-Taste |
| 8 | `_NOTED4` | Momentary | `L4_MO_NOTED` |
| 9 | `_FN` | Momentary | `SP_FN` (Leertaste halten) |
| 10 | `_SYS` | Momentary | `SYS_ESC` (Ebene 5/8 Esc-Position) halten, Capslock-Tap-Dance halten (Ebene 0/1-4), oder `SYS_MO` (auf `_FN`) |

**Es gibt hier keine Ebene `_RGB` und keinen physischen DIP-Schalter** -
beides anders als auf den übrigen Boards in diesem Userspace:

- `_RGB` (in anderen Keymaps Layer 11) fehlt hier komplett: das Board hat
  kein `RGB_MATRIX`, nur `RGBLIGHT` (`users/neo/rules.mk`), die Ebene wäre
  also wirkungslos. Sie ist nicht Teil von `keymaps[]` in `keymap.c` -
  `DYNAMIC_KEYMAP_LAYER_COUNT` ist entsprechend auf 11 gesetzt (nicht 12,
  `keymaps/neo/config.h`). Die FN-Taste, die auf anderen Boards `RGB_MO`
  (→ `_RGB`) ist, ist hier lokal auf `KC_NO` umgebogen (Override von
  `FN60_ROW4` direkt in `keymap.c`, ohne das geteilte `layouts/fn60.h`
  anzufassen).
- **Kein DIP-Schalter auf der Unterseite dieses Boards.** Die
  Default-Layer-Wahl läuft ausschließlich über die drei `DF(...)`-Tasten
  auf der `_SYS`-Ebene (`DF(_QWERTZ)`, `DF(_NEOQWERTZ1)`, `DF(_NOTED1)`,
  siehe Ebene 10 unten) - dauerhaft im EEPROM, kein Hardware-Schalter nötig.

## 🔑 Sondertasten-Legende

| Code | Tap | Hold |
|:--|:--|:--|
| `SP_FN` | Leertaste | `_FN` |
| `SYS_ESC` | Esc | `_SYS` |
| `SYS_MO` | – | `_SYS` (reine Momentary-Taste, kein Tap) |
| `NEO_SHIFT` | – | `_NEOQWERTZ2` |
| `NOTED_SHIFT` | – | `_NOTED2` |
| `L4_MO_NEO` | – | `_NEOQWERTZ4` |
| `L4_MO_NOTED` | – | `_NOTED4` |
| `TD(TD_CAPS_SYS_LEAD)` | Esc | `_SYS` (Ebene 0) |
| `TD(TD_CAPS_NEO_LEAD)` | Esc | `_NEOQWERTZ3` (Ebene 1-3) |
| `TD(TD_CAPS_NOTED_LEAD)` | Esc | `_NOTED3` (Ebene 5-7) |
| `TD(TD_NEO_L3MO_LEAD)` | – | `_NEOQWERTZ3` |
| `TD(TD_NOTED_L3MO_LEAD)` | – | `_NOTED3` |
| `QK_LEAD` | Leader-Sequenz starten | – |
| `DF(x)` | Default-Layer dauerhaft auf `x` setzen (EEPROM) | – |

Alle fünf `TD(...)`-Tasten haben zusätzlich einen **Doppeltap → Leader**
(`leader_start()`), siehe Abschnitt Leader unten.

## 👆 Tap Dance im Detail

Sitzt physisch immer auf der **Capslock**-Position (außer `TD_NEO_L3MO_LEAD`
und `TD_NOTED_L3MO_LEAD`, die eine Zusatztaste in Reihe 3 belegen, siehe
Layer-Tabellen unten).

| Tap-Dance | Einfacher Tap | Halten | Doppeltap |
|:--|:--|:--|:--|
| `TD_CAPS_SYS_LEAD` | `KC_ESC` | Layer `_SYS` an | Leader-Sequenz starten |
| `TD_CAPS_NEO_LEAD` | `KC_ESC` | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_CAPS_NOTED_LEAD` | `KC_ESC` | Layer `_NOTED3` an | Leader-Sequenz starten |
| `TD_NEO_L3MO_LEAD` | – (kein Tap-Keycode) | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_NOTED_L3MO_LEAD` | – (kein Tap-Keycode) | Layer `_NOTED3` an | Leader-Sequenz starten |

`TAPPING_TERM = 250 ms`, `RETRO_TAPPING` aktiv (`keymaps/neo/config.h`).

**Wichtig:** Die pro-Key-Tuning-Hooks (`get_tapping_term`,
`get_ignore_mod_tap_interrupt`, `get_hold_on_other_key_press`), die auf den
Keychron-Boards `SP_FN`/`SYS_ESC` u.a. "tap-first" verhalten geben, sind auf
kbd8x_mk3 **nicht kompiliert** (`keymap_logic.c` steht nicht in `SRC` für
dieses Board, `users/neo/rules.mk`) - hier gelten die reinen QMK-Defaults
mit dem oben genannten `TAPPING_TERM`.

## 🚦 Leader (Menü-Taste)

`QK_LEAD` sitzt auf der **Menü-Taste** (Kontextmenü, zwischen rechtem Win
und rechtem Strg) - auf jeder unteren Reihe unten identisch platziert.
Sequenz innerhalb von 500 ms tippen (`LEADER_TIMEOUT`,
`LEADER_PER_KEY_TIMING`, `keymaps/neo/config.h`).

Reduzierter Satz (RAM-bedingt, siehe `users/neo/leader/table_min.c`) - nur
**rg** und **fzf**, kein Git/WezTerm/zoxide/yazi/zellij/sed wie auf den
Keychron-Boards:

| Sequenz | Aktion |
|:--|:--|
| `A A` | Selbsttest: tippt "LEADER OK" |
| `F H` | Kontexthilfe fzf-Gruppe |
| `F F` | `fzf` |
| `F E` | Auswahl → `$EDITOR` |
| `F P` | Preview mit `bat` |
| `F O` | Dateien mit Preview → Editor |
| `F M` | Multi-Select → Editor |
| `F D` | Verzeichnis → `cd` |
| `R H` | Kontexthilfe rg-Gruppe |
| `R R` | `rg` Basis |
| `R S` | `-S` smart case |
| `R A` | `git ls-files \| rg` |
| `R L` | `rg … \| less -R` |
| `R G R` | `-g '*.go'` |
| `R B` | `-g '*.rs'` |
| `R F` | `rg \| fzf \| nvim` + Zeilensprung |
| `R C A` / `R C U` / `R C D` | Kontext `-C 3` / `-B 3` / `-A 3` |
| `R M` | `-U` multiline |
| `R T C` / `R T R` / `R T G` | Dateitypen cpp/rust/go |

## 🔒 Fixe Bereiche (auf allen 11 Layern identisch)

Diese Positionen ändern sich **nicht** zwischen den Layern - der
Formfaktor (`formfactors/ff_tkl_iso_kbd8x_mk3.h`) reicht sie unverändert
durch, egal welches `MAIN`-Layout aktiv ist.

**F-Reihe (physische Reihe 0):**
`KC_ESC · F1 · F2 · F3 · F4 · F5 · F6 · F7 · F8 · F9 · F10 · F11 · F12 · KC_PSCR · (frei) · UG_TOGG`

`UG_TOGG` (RGBLIGHT an/aus) sitzt auf der Pause-Position, ganz rechts in der
F-Reihe. War vorher `RM_NEXT` (`QK_RGB_MATRIX_MODE_NEXT`) - totes
RGB_MATRIX-Keycode auf einem Board ohne `RGB_MATRIX_ENABLE`, siehe
`README.md`, RGB-Abschnitt. Zusätzlich auch auf der `_SYS`-Ebene gebunden
(Ebene 10 unten, rechte Hand vor Backspace).

**Navigationsblock (rechts, alle Layer):**
- Reihe 1: `Ins · Home · PgUp`
- Reihe 2: `Del · End · PgDn` (physisches ISO-Enter sitzt elektrisch hier, siehe unten)
- Reihe 4: `Up`
- Reihe 5: `Left · Down · Right`

**ISO-Enter:** Sitzt physisch neben der Reihe-3-Tastatur, elektrisch aber
in Reihe 2 (`KBD8X_CENTER_ROW2`) - deshalb endet in den Layer-Tabellen
unten jede Reihe-3-Zeile *nicht* mit Enter, obwohl die zugrundeliegenden
`*_ROW3`-Makros eins definieren (wird von `ROW3_DROP_ENT()` verworfen).

## ⌨️ Layer-Tabellen

Reihenfolge je Layer: Reihe 1 (Zahlenreihe), Reihe 2 (QWERTZ-Reihe),
Reihe 3 (Grundreihe, ohne Enter s.o.), Reihe 4 (Untere Buchstabenreihe),
Reihe 5 (Leertastenreihe, 8 Tasten inkl. Leader).

### Ebene 0 — `_QWERTZ`
| Reihe 1 | `QK_GESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_SYS_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `#` |
| **Reihe 4** | `LShift` `<` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` **`QK_LEAD`** `RCtrl` |

### Ebene 1 — `_NEOQWERTZ1` (Basis)
| Reihe 1 | `Esc` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `=` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` |
| **Reihe 4** | `NEO_SHIFT` `L4_MO_NEO` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `NEO_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` **`L4_MO_NEO`** `RWin` `QK_LEAD` `RCtrl` |

### Ebene 2 — `_NEOQWERTZ2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | `–` `°` `§` `ℓ` `»` `«` `$` `AltGr+E` `„` `"` `"` `ẞ` `` ` (tot)`` `Bksp` |
|:--|:--|
| **Reihe 2** | `–` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `*` *(alle Buchstaben groß/Shift)* |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` *(groß)* |
| **Reihe 4** | `–` `L4_MO_NEO` `Y` `X` `C` `V` `B` `N` `M` `;` `:` `_` `–` *(groß)* |
| **Reihe 5** | wie Ebene 1 |

### Ebene 3 — `_NEOQWERTZ3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `$` `~` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `@` `TD_NEO_L3MO_LEAD` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `#` `$` `\|` `~` `` ` `` `+` `%` `"` `'` `;` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 4 — `_NEOQWERTZ4` (Navigation/Numblock)
| Reihe 1 | `(frei)` ×11 `/` `*` `-` |
|:--|:--|
| **Reihe 2** | `Tab` `PgUp` `Bksp` `Up` `Del` `PgDn` `Down` `P7` `P8` `P9` `P+` `P-` `Enter` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `Home` `Left` `Down` `Right` `End` `Ins` `P4` `P5` `P6` `,` `.` `L4_MO_NEO` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `Esc` `Tab` `Ins` `Enter` `Ctrl+Z` `:` `P1` `P2` `P3` `(frei)` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `P0` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 5 — `_NOTED1` (Basis)
| Reihe 1 | `SYS_ESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `-` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` `W` `G` `,` `.` `K` `NOTED_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `L4_MO_NOTED` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 6 — `_NOTED2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | wie Ebene 2 |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` *(Buchstaben groß)* |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` *(groß)* |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` `W` `G` `-` `.` `K` `NOTED_SHIFT` *(groß)* |
| **Reihe 5** | wie Ebene 5 |

### Ebene 7 — `_NOTED3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `@` `~` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `TD_NOTED_L3MO_LEAD` `@` |
| **Reihe 4** | `LShift` `L4_MO_NOTED` `#` `$` `\|` `~` `` ` `` `+` `%` `"` `'` `;` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 8 — `_NOTED4` (Navigation/Numblock)
| Reihe 1 | `SYS_ESC` `F1` … `F12` `Ins` |
|:--|:--|
| **Reihe 2** | `Tab` `Esc` `Home` `Up` `End` `PgUp` `PrtScn` `Ins` `Del` `Left` `Down` `Right` `Pause` |
| **Reihe 3** | `SYS_ESC` `Calc` `Prev` `Play` `Next` `Mute` `Vol-` `Vol+` `PrtScn` `ScrLk` `Pause` `Bksp` `Tab` |
| **Reihe 4** | `LShift` `NumLk` `P/` `P7` `P8` `P9` `P+` `L3_MO_NOTED` `P4` `P5` `P6` `RShift` `Up` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

> Auf dieser Ebene ersetzt `SYS_ESC` (statt der Tap-Dance) sowohl die
> Esc- als auch die Capslock-Position - Doppeltap→Leader gibt es hier
> also nicht, nur Tap=Esc/Hold=`_SYS`.

### Ebene 9 — `_FN`
| Reihe 1 | `Esc` `F1` … `F12` `Del` |
|:--|:--|
| **Reihe 2** | `Tab` `Macro6` `Macro4` `Macro2` `Macro0` ⚠`DM_PLY1` `(frei)` ⚠`DM_REC1` ⚠`DM_REC2` ⚠`DM_RSTP` `(frei)` `Macro10` `Macro8` |
| **Reihe 3** | `Esc` `Macro7` `Macro5` `Macro3` `Macro1` ⚠`DM_PLY2` `Left` `Down` `Up` `Right` `(frei)` `Macro11` `Macro9` |
| **Reihe 4** | `LShift` `(frei)` ×5 `(frei)` `SYS_MO` `(frei)` `Home` `PgDn` `PgUp` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `RWin` `QK_LEAD` `RCtrl` |

`Macro0`-`Macro11` = `QK_MACRO_0`…`QK_MACRO_11` (VIA-Makro-Tab, funktioniert
- `VIA_ENABLE=yes`). ⚠ = **Dynamic-Macro-Tasten (`DM_*`) sind auf kbd8x_mk3
nicht funktionsfähig** (`DYNAMIC_MACRO_ENABLE=no`, RAM-Sparen laut
`users/neo/rules.mk`) - Tastendruck bleibt wirkungslos.

Reihe-4-Position 7 ist auf anderen Boards `RGB_MO` (→ `_RGB`) - auf
kbd8x_mk3 lokal auf `(frei)` (`KC_NO`) umgebogen, siehe Layer-Übersicht
oben.

### Ebene 10 — `_SYS`
| Reihe 1 | `Esc` `QK_BOOT` `EE_CLR` `(frei)` ×9 `UG_TOGG` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `DF(_QWERTZ)` `DF(_NEOQWERTZ1)` `DF(_NOTED1)` `(frei)` ×9 |
| **Reihe 3** | ⚠`KC_CAPS` ⚠`UC_MODE_CYCLE` ⚠`UC_SET_LNX` ⚠`UC_SET_WIN` ⚠`UC_SET_WINC` `(frei)` ×8 |
| **Reihe 4** | `LShift` ⚠`KC_OS_CYCLE` ⚠`KC_OS_WIN` ⚠`KC_OS_LNX` ⚠`KC_OS_MAC` `(frei)` ×7 `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `RWin` `QK_LEAD` `RCtrl` |

`UG_TOGG` (RGBLIGHT an/aus, Reihe 1 Position 13, rechte Hand direkt vor
Backspace - bewusst nicht neben `EE_CLR`) ist ein lokaler Override von
`SYS60_ROW1` nur für kbd8x_mk3 (`keymap.c`) - zusätzlich zur immer
erreichbaren Position auf der F-Reihe (siehe oben), hier explizit neben
`QK_BOOT`/`EE_CLR` gruppiert. Feinere Einstellungen (Modus/Farbe/
Helligkeit) laufen über VIAs Lighting-Tab (`via.json`, `qmk_rgblight`-Menü),
nicht über eigene Keycodes.

⚠ **Diese acht Tasten sind auf kbd8x_mk3 aktuell tot.** Sie werden in
`process_record_user()` (`users/neo/keymap_logic.c`) behandelt - diese Datei
ist für kbd8x_mk3 nicht in `SRC` (`users/neo/rules.mk`), also greift QMKs
leerer Weak-Default-Stub und die Tasten senden nichts. Funktioniert dagegen
normal: `QK_BOOT` (Bootloader), `EE_CLR` (EEPROM löschen), `UG_TOGG`
(RGBLIGHT-Toggle - reine QMK-Kernfunktion), alle drei
`DF(...)`-Tasten (Default-Layer wechseln - reine QMK-Kernfunktion, kein
Hook nötig).

### (Keine Ebene 11 — `_RGB`)

Andere Boards in diesem Userspace haben hier eine `_RGB`-Ebene mit
`QK_RGB_MATRIX_*`-Keycodes. kbd8x_mk3 hat kein `RGB_MATRIX`, nur
`RGBLIGHT` (`users/neo/rules.mk`) - diese Ebene wäre komplett wirkungslos
und ist deshalb hier gar nicht erst Teil von `keymaps[]` (siehe Layer-
Übersicht oben). Die einzigen tatsächlich funktionierenden RGB-
Bedienelemente auf kbd8x_mk3 sind `UG_TOGG` (F-Reihe und `_SYS`-Ebene,
beide bestätigt funktionierend - RGBLIGHT an/aus) und die automatischen
Capslock/Scroll-Indikator-Layer (siehe `README.md`, Abschnitt RGB).

## 📎 Legende: deutsche Sonderzeichen / Unicode

| Code | Zeichen | Code | Zeichen | Code | Zeichen |
|:--|:--|:--|:--|:--|:--|
| `DE_SS` | ß | `DE_ACUT` | ´ (tot) | `DE_GRV` | \` (tot) |
| `DE_UDIA` | Ü | `DE_ODIA` | Ö | `DE_ADIA` | Ä |
| `DE_HASH` | # | `DE_PLUS` | + | `DE_MINS` | - |
| `DE_LABK`/`DE_RABK` | < / > | `DE_COMM`/`DE_DOT` | , / . | `DE_CIRC` | ^ (tot) |
| `DE_DEG` | ° | `DE_SECT` | § | `DE_EXLM` | ! |
| `DE_DQUO`/`DE_QUOT` | " / ' | `DE_DLR` | $ | `DE_PERC` | % |
| `DE_AMPR` | & | `DE_SLSH` | / | `DE_LPRN`/`DE_RPRN` | ( / ) |
| `DE_EQL` | = | `DE_QUES` | ? | `DE_ASTR` | * |
| `DE_SCLN`/`DE_COLN` | ; / : | `DE_UNDS` | _ | `DE_LBRC`/`DE_RBRC` | [ / ] |
| `DE_LCBR`/`DE_RCBR` | { / } | `DE_BSLS` | \ | `DE_AT` | @ |
| `DE_EURO` | € | `DE_TILD` | ~ | `DE_PIPE` | \| |
| `UC(0x2113)` | ℓ | `UC(0x00BB)`/`UC(0x00AB)` | » / « | `UC(0x2039)`/`UC(0x203A)` | ‹ / › |
| `UC(0x201E)`/`UC(0x201C)`/`UC(0x201D)` | „ / " / " | `UC(0x201A)`/`UC(0x2018)`/`UC(0x2019)` | ‚ / ' / ' | `UC(0x1E9E)` | ẞ |
| `UC(0x21BB)` | ↻ | `UC(0x00B9)`/`UC(0x00B2)`/`UC(0x00B3)` | ¹ / ² / ³ | `UC(0x00A2)`/`UC(0x00A5)` | ¢ / ¥ |
| `UC(0x00B7)` | · | | | | |
