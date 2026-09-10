# Keychron Q10 Pro ISO Encoder – Tastenbelegung (Keymap `neo`)

Ergänzung zu `README.md` (dort: Hardware/Build/Flash/VIA) und
`BLUETOOTH.md` (dort: BT-Pairing/Host-Wechsel im Detail). Diese Datei
beschreibt, was auf welcher Taste liegt - Stand des kompilierten
Keymaps (`keymaps/neo/keymap.c` + `users/neo/layouts/*.h` +
`users/neo/formfactors/ff_91_iso_q10_pro.h`), nicht zwingend das, was
gerade über VIA live umbelegt wurde. **Noch nicht am echten Board
gegengeprüft** - siehe die Warnhinweise unten.

Reihe 1-4 (Zahlenreihe/QWERTZ-Reihe/Grundreihe-Kern/untere Buchstaben-
reihe) nutzen **dieselben** geteilten `MAIN##_ROWx`-Makros wie alle
anderen Boards (V1 Max, V3 Max, kbd8x_mk3, ...) - der Buchstabeninhalt
ist also identisch zu z.B. V3 Max. Was das Q10 Pro physisch anders macht
(Makro-Spalte links, keine Spalte O in Reihe 2, eine überzählige Spalte
in Reihe 4, geteilte Leertaste in Reihe 5) ist in
`users/neo/formfactors/ff_91_iso_q10_pro.h` gekapselt.

## 🗂️ Layer-Übersicht

| # | Name | Typ | Erreichbar über |
|--:|:--|:--|:--|
| 0 | `_QWERTZ` | Default | Boot-Default, oder `_SYS`→`DF(_QWERTZ)` |
| 1 | `_NEOQWERTZ1` | Default | nur über `_SYS`→`DF(_NEOQWERTZ1)` |
| 2 | `_NEOQWERTZ2` | Momentary | Neo-Shift-Ebene, über `NEO_SHIFT` |
| 3 | `_NEOQWERTZ3` | Momentary | Capslock-Tap-Dance halten (Ebene 1-3), oder L3-Taste |
| 4 | `_NEOQWERTZ4` | Momentary | `L4_MO_NEO` - Taste neben LShift (Reihe 4) auf Ebene 1-3 |
| 5 | `_NOTED1` | Default | nur über `_SYS`→`DF(_NOTED1)` |
| 6 | `_NOTED2` | Momentary | Noted-Shift-Ebene, über `NOTED_SHIFT` |
| 7 | `_NOTED3` | Momentary | Capslock-Tap-Dance halten (Ebene 5-7), oder L3-Taste |
| 8 | `_NOTED4` | Momentary | `L4_MO_NOTED` |
| 9 | `_FN` | Momentary | `SP_FN` (rechte Leertaste halten) |
| 10 | `_SYS` | Momentary | Capslock-Tap-Dance halten, oder `SYS_MO` (auf `_FN`) |
| 11 | `_RGB` | Momentary | `RGB_MO` (auf `_FN`) - echtes `RGB_MATRIX` |

**Kein `_WIN_FN`, kein `_TETRIS`** - beides bewusst nicht portiert (siehe
README.md). `_BLUETOOTH_LAYER`/`TETRIS_GAME_ENABLE` sind nicht definiert,
diese Layer tauchen im `enum layers` also gar nicht erst auf.

**Kein physischer DIP-Schalter** (`DIP_SWITCH_ENABLE` ist nirgends
gesetzt) - `dip_switch_update_user()` in `keymap.c` ist totes Boilerplate.
Default-Layer-Wahl läuft über die `DF(...)`-Tasten auf `_SYS`.

## 🔑 Sondertasten-Legende

| Code | Tap | Hold |
|:--|:--|:--|
| `SP_FN` | Leertaste (rechts) | `_FN` |
| `MO(_FN)` | – | `_FN` (physische Fn-Taste, Reihe 4 zwischen B und N) |
| `SYS_MO` | – | `_SYS` |
| `RGB_MO` | – | `_RGB` (funktioniert - echtes RGB_MATRIX) |
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
| `BT_HST1/2/3` | Bluetooth-Host 1/2/3 wählen | – |
| `BAT_LVL` | Akkustand anzeigen (LED-Balken) | – |
| `QK_MACRO_1/3/5/7/9` | Dynamic-Macro-Wiedergabe (Makro-Spalte links, Reihe 1-5, ungerade Slots) | – |

**Kein `P2P4G`** (kein 2.4G-Support in diesem Firmware-Port) und **keine
eigene Row5-Leader-Taste** wie bei V1 Max/V3 Max - die geteilte Leertaste
lässt dafür physisch keinen Platz. Leader bleibt über die
`TD(...)`-Doppeltap-Tasten (Capslock) sowie den Combo `F+J` (nur auf
`_QWERTZ`/`_NEOQWERTZ1`) erreichbar.

Alle fünf `TD(...)`-Tasten haben zusätzlich einen **Doppeltap → Leader**
- funktioniert hier immer (`LEADER_WIRED` ist für nicht-speicher-
beschränkte Boards fest an).

## 👆 Tap Dance im Detail

Identisch zu V1 Max/V3 Max/kbd8x_mk3 (gleiche geteilte
`tap_dance_impl.c`/Layout-Dateien):

| Tap-Dance | Einfacher Tap | Halten | Doppeltap |
|:--|:--|:--|:--|
| `TD_CAPS_SYS_LEAD` | `KC_ESC` | Layer `_SYS` an | Leader-Sequenz starten |
| `TD_CAPS_NEO_LEAD` | `KC_ESC` | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_CAPS_NOTED_LEAD` | `KC_ESC` | Layer `_NOTED3` an | Leader-Sequenz starten |
| `TD_NEO_L3MO_LEAD` | – | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_NOTED_L3MO_LEAD` | – | Layer `_NOTED3` an | Leader-Sequenz starten |

`TAPPING_TERM = 250 ms`, `RETRO_TAPPING` aktiv. Alle Tuning-Hooks
(`get_tapping_term`, `get_ignore_mod_tap_interrupt`,
`get_hold_on_other_key_press`, `keymap_logic.c`) sind aktiv.

## 🚦 Leader

Kein eigener Row5-Slot (siehe oben) - Zugang ausschließlich über die
`TD(...)`-Doppeltap-Tasten (Capslock-Position auf jedem Basis-Layer) oder
den Combo `F+J` (nur `_QWERTZ`/`_NEOQWERTZ1`, `COMBO_TERM = 40 ms`).
Sequenz innerhalb von 500 ms tippen (`LEADER_TIMEOUT`,
`LEADER_PER_KEY_TIMING`), danach kurz pausieren.

**Voller Leader-Baum** (`users/neo/leader/table.c`, identisch zu allen
anderen nicht-speicherbeschränkten Boards): Global, OS-Wahl, WezTerm
Tabs/Panes, Git, fzf, ripgrep, zoxide, yazi, zellij, sed - Details siehe
`users/neo/leader/table.c` oder `LEAD H H` am Gerät selbst.

## 🎛️ Encoder

1 Encoder (`NUM_ENCODERS=1`), auf allen Ebenen einheitlich Vol-/Vol+.

## 🔒 Fixe Bereiche (auf allen Layern identisch, inkl. `_SYS`/`_RGB`)

Der Formfaktor (`ff_91_iso_q10_pro.h`) reicht F-Reihe, Makro-Spalte und
Nav-Block auf **jeder** Ebene unverändert durch - keine `_FN`-Sonder-
belegung für die F-Reihe (Q10 Pro zeigt auf Reihe 0 immer echte F1-F12,
nicht Medientasten-per-Fn wie bei manch anderem Board).

**Reihe 0 (F-Reihe, 16 Tasten):**
`[Mute] · Esc · F1 · F2 · F3 · F4 · F5 · F6 · F7 · F8 · F9 · F10 · F11 · F12 · F13 · Del`

`[Mute]` ist eine dedizierte Hardware-Taste (kein Neo-Ebeneninhalt, so
aus Keychrons eigenem Referenz-Keymap übernommen). Endet **F11-F12-F13-
Del** statt Ins-Del (Keychrons Referenz-Keymap hatte dort `Ins` - auf
Nutzerwunsch durch `F13` ersetzt).

**Makro-Spalte (Reihe 1-5, ganz links):** `QK_MACRO_1` `QK_MACRO_3`
`QK_MACRO_5` `QK_MACRO_7` `QK_MACRO_9` (Reihe 1 bis Reihe 5, ungerade
Slots aufsteigend) - physische Extra-Spalte, auf jeder Ebene identisch.
Zum Aufnehmen: VIA → Macros-Tab, Slots 2/4/6/8/10 (VIA zählt ab 1,
`QK_MACRO_1` = VIAs Makro 2, `QK_MACRO_9` = VIAs Makro 10 usw.).

**Navigationsblock (rechts, alle Layer):**
- Reihe 1 Ende: `PgUp`
- Reihe 2 Ende: `PgDn`
- Reihe 3 Ende: `Enter` `Home`
- Reihe 4 Ende: `Up`
- Reihe 5 Ende: `Left · Down · Right`

**ISO-Enter sitzt elektrisch in Reihe 2** (physisch hochkant zwischen
Reihe 2 und 3), wird hier aber der Übersicht halber am Ende von Reihe 3
aufgeführt (dort landet es auch im generierten `LAYOUT_91_iso`-Makro-
Aufruf) - siehe Kommentar in `ff_91_iso_q10_pro.h` für die Herleitung.

**Reihe 5 (Grundreihe, geteilte Leertaste, fix auf jeder Ebene):**
`LCtrl · LWin · LAlt · [Leertaste] · RWin · [Leertaste=Fn halten] · AltGr · RCtrl`

Anders als bei allen anderen Boards **nicht pro Ebene parametrisiert**
(z.B. `L4_MO_NEO` auf AltGr) - der Nutzer hat diese Belegung explizit so
festgelegt, unabhängig vom aktiven Neo-Layer.

**Reihe 4, überzählige Spalte = Fn:** zwischen den echten QWERTY-
Positionen (dort wo bei einem generischen Board direkt "B" auf "N"
folgen würde) hat das Q10 Pro physisch eine achte Taste - das ist die
physische **Fn-Taste** des Boards (`MO(_FN)`, fix auf jeder Ebene
identisch). Keychrons eigenes Referenz-Keymap füllt diese Position mit
einem zweiten `KC_B` (vermutlich nur ein Lückenfüller in deren Firmware,
nicht die tatsächliche Fn-Funktion).

## ⌨️ Layer-Tabellen

Reihenfolge: Reihe 1 (Zahlenreihe), Reihe 2 (QWERTZ-Reihe), Reihe 3
(Grundreihe, Enter am Ende separat da elektrisch aus Reihe 2 stammend),
Reihe 4 (untere Buchstabenreihe **mit** der überzähligen Spalte), Reihe 5
(Leertastenreihe, fix, siehe oben - hier nicht je Ebene wiederholt).

### Ebene 0 — `_QWERTZ`
| Reihe 1 | `QK_GESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_SYS_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `#` — `Enter` |
| **Reihe 4** | `LShift` `<` `Y` `X` `C` `V` `B` **Fn** `N` `M` `,` `.` `-` `RShift` |

### Ebene 1 — `_NEOQWERTZ1` (Basis)
| Reihe 1 | `Esc` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `=` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` — `Enter` |
| **Reihe 4** | `NEO_SHIFT` `L4_MO_NEO` `Y` `X` `C` `V` `B` **Fn** `N` `M` `,` `.` `-` `NEO_SHIFT` |

### Ebene 2 — `_NEOQWERTZ2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | `–` `°` `§` `ℓ` `»` `«` `$` `AltGr+E` `„` `"` `"` `ẞ` `` ` (tot)`` `Bksp` |
|:--|:--|
| **Reihe 2** | `–` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `*` *(groß)* |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` — `Enter` *(groß)* |
| **Reihe 4** | `–` `L4_MO_NEO` `Y` `X` `C` `V` `B` **Fn** `N` `M` `;` `:` `_` `–` *(groß)* |

### Ebene 3 — `_NEOQWERTZ3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `$` `~` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `@` `TD_NEO_L3MO_LEAD` — `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `#` `$` `\|` `~` `` ` `` **Fn** `+` `%` `"` `'` `;` `RShift` |

### Ebene 4 — `_NEOQWERTZ4` (Navigation/Numblock)
| Reihe 1 | `(frei)` ×11 `/` `*` `-` |
|:--|:--|
| **Reihe 2** | `Tab` `PgUp` `Bksp` `Up` `Del` `PgDn` `Down` `P7` `P8` `P9` `P+` `P-` `Enter` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `Home` `Left` `Down` `Right` `End` `Ins` `P4` `P5` `P6` `,` `.` `L4_MO_NEO` — `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `Esc` `Tab` `Ins` `Enter` `Ctrl+Z` **Fn** `:` `P1` `P2` `P3` `(frei)` `RShift` |

### Ebene 5 — `_NOTED1` (Basis)
| Reihe 1 | `SYS_ESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `-` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` — `Enter` |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` **Fn** `W` `G` `,` `.` `K` `NOTED_SHIFT` |

### Ebene 6 — `_NOTED2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | wie Ebene 2 |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` *(groß)* |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` — `Enter` *(groß)* |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` **Fn** `W` `G` `-` `.` `K` `NOTED_SHIFT` *(groß)* |

### Ebene 7 — `_NOTED3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `@` `~` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `TD_NOTED_L3MO_LEAD` `@` — `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NOTED` `#` `$` `\|` `~` `` ` `` **Fn** `+` `%` `"` `'` `;` `RShift` |

### Ebene 8 — `_NOTED4` (Navigation/Numblock)
| Reihe 1 | `SYS_ESC` `F1` … `F12` `Ins` |
|:--|:--|
| **Reihe 2** | `Tab` `Esc` `Home` `Up` `End` `PgUp` `PrtScn` `Ins` `Del` `Left` `Down` `Right` `Pause` |
| **Reihe 3** | `SYS_ESC` `Calc` `Prev` `Play` `Next` `Mute` `Vol-` `Vol+` `PrtScn` `ScrLk` `Pause` `Bksp` `Tab` — `Enter` |
| **Reihe 4** | `LShift` `NumLk` `P/` `P7` `P8` `P9` `P+` **Fn** `L3_MO_NOTED` `P4` `P5` `P6` `RShift` |

> `SYS_ESC` ersetzt hier (statt Tap-Dance) sowohl Esc- als auch
> Capslock-Position - kein Doppeltap→Leader auf dieser Ebene.

### Ebene 9 — `_FN`
| Reihe 1 | `Esc` `F1` … `F12` `Del` |
|:--|:--|
| **Reihe 2** | `Tab` `Macro6` `Macro4` `Macro2` `Macro0` `DM_PLY1` `(frei)` `DM_REC1` `DM_REC2` `DM_RSTP` `(frei)` `Macro10` `Macro8` |
| **Reihe 3** | `Esc` `Macro7` `Macro5` `Macro3` `Macro1` `DM_PLY2` `Left` `Down` `Up` `Right` `(frei)` `Macro11` `Macro9` — `Enter` |
| **Reihe 4** | `LShift` `(frei)` ×5 `RGB_MO` **Fn** `SYS_MO` `(frei)` `Home` `PgDn` `PgUp` `RShift` |

`Macro0`-`Macro11` = `QK_MACRO_0`…`QK_MACRO_11` (VIA-Makro-Tab).
`DM_REC1/2`/`DM_PLY1/2`/`DM_RSTP` (Dynamic Macros) funktionieren hier.
`RGB_MO` funktioniert (echtes RGB_MATRIX).

### Ebene 10 — `_SYS`
| Reihe 1 | `Esc` `QK_BOOT` `EE_CLR` `(frei)` ×10 `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `DF(_QWERTZ)` `DF(_NEOQWERTZ1)` `DF(_NOTED1)` `(frei)` ×4 `BT_HST1` `BT_HST2` `BT_HST3` `BAT_LVL` |
| **Reihe 3** | `KC_CAPS` `UC_MODE_CYCLE` `UC_SET_LNX` `UC_SET_WIN` `UC_SET_WINC` `(frei)` ×8 — `Enter` |
| **Reihe 4** | `LShift` `KC_OS_CYCLE` `KC_OS_WIN` `KC_OS_LNX` `KC_OS_MAC` `(frei)` ×2 **Fn** `(frei)` ×4 `RShift` |

**BT-Host-Wahl/Akkustand auf `_SYS`** (Reihe 2, letzte 4 Positionen) -
lokaler Override von `SYS60_ROW2` in `keymap.c`. **Kein `P2P4G`** (siehe
README.md). Details/Pairing-Ablauf siehe `BLUETOOTH.md`.

**RGB-Layer-Indikator:** Solange `_SYS` gehalten wird, färbt
`rgb_matrix_indicators_user()` (in `keymap.c`) die komplette Matrix
**rot** ein - außer der aktive Transport ist gerade Bluetooth
(`get_transport() == TRANSPORT_BLUETOOTH`), dann **blau**, und zusätzlich
blinkt die Taste des aktuell verbundenen BT-Host-Slots (`BT_HST1/2/3`,
LED-Indizes aus `BT_INDCATION_LED_MATRIX_LIST`) im 300-ms-Takt weiß/aus -
1:1 von V1 Max/V3 Max übernommene Logik. **`BT_INDCATION_LED_MATRIX_LIST`
(`{17, 18, 19}`) wurde unverändert aus der alten Firmware übernommen und
noch nicht per Raw-HID/visuell gegen die echten BT_HST-Tasten verifiziert**
- siehe README.md und BLUETOOTH.md.

### Ebene 11 — `_RGB`
Nutzt `layouts/rgb60.h` unverändert - `QK_RGB_MATRIX_*`-Keycodes
funktionieren hier tatsächlich (echtes RGB_MATRIX-Feature). Reihe 1:
Toggle/Modus±/Helligkeit±/Speed±, Reihe 2: Farbton±/Sättigung±. Für die
volle Einstellung eignet sich VIAs Lighting-Tab besser (`via.json` hat
allerdings `lighting: "none"`, siehe README.md - RGB-Steuerung läuft
daher über diese Ebene direkt am Board).

## 📎 Legende: deutsche Sonderzeichen / Unicode

(Gleiche geteilte `keymap_extras/keymap_german.h`-Basis wie alle anderen
Boards, keine Unterschiede.)

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
