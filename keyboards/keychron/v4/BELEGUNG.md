# Keychron V4 ISO – Tastenbelegung (Keymap `neo`)

Ergänzung zu `README.md` (dort: Hardware/Build/Flash/VIA/Makros). Diese
Datei beschreibt, was auf welcher Taste liegt - Stand des kompilierten
Default-Keymaps (`keymaps/neo/keymap.c` + `users/neo/layouts/*.h` +
`users/neo/formfactors/ff_60_iso.h`), nicht zwingend das, was gerade über
VIA live umbelegt wurde.

Kompaktes 60% ohne eigene F-Reihe/Nav-Block und ohne Encoder - anders als
V3 Max/kbd8x_mk3 (TKL) gibt es hier **keinen** Abschnitt "Fixe Bereiche":
F-Tasten und Navigation sind ausschließlich über die `_FN`-Ebene
erreichbar, nicht auf jeder Ebene zusätzlich physisch vorhanden.

## 🗂️ Layer-Übersicht

| # | Name | Typ | Erreichbar über |
|--:|:--|:--|:--|
| 0 | `_QWERTZ` | Default | Boot-Default, oder `_SYS`→`DF(_QWERTZ)` |
| 1 | `_NEOQWERTZ1` | Default | nur über `_SYS`→`DF(_NEOQWERTZ1)` |
| 2 | `_NEOQWERTZ2` | Momentary | Neo-Shift-Ebene, über `NEO_SHIFT` |
| 3 | `_NEOQWERTZ3` | Momentary | Capslock-Tap-Dance halten (Ebene 1-3), oder L3-Taste |
| 4 | `_NEOQWERTZ4` | Momentary | `L4_MO_NEO` - Taste neben LShift (Reihe 4) auf Ebene 1-3, zusätzlich rechtes AltGr nur auf Ebene 1-2 (auf Ebene 3 ist AltGr normales `KC_RALT`) |
| 5 | `_NOTED1` | Default | nur über `_SYS`→`DF(_NOTED1)` |
| 6 | `_NOTED2` | Momentary | Noted-Shift-Ebene, über `NOTED_SHIFT` |
| 7 | `_NOTED3` | Momentary | Capslock-Tap-Dance halten (Ebene 5-7), oder L3-Taste |
| 8 | `_NOTED4` | Momentary | `L4_MO_NOTED` |
| 9 | `_FN` | Momentary | `SP_FN` (Leertaste halten) |
| 10 | `_SYS` | Momentary | `SYS_ESC`/Capslock-Tap-Dance halten, oder `SYS_MO` (auf `_FN`) |
| 11 | `_RGB` | Momentary | `RGB_MO` (auf `_FN`) - echtes `RGB_MATRIX` |

Kein `_WIN_FN` (kein Bluetooth, `_BLUETOOTH_LAYER` nicht definiert) und
kein `_TETRIS` (Board bislang nicht mit `TETRIS_GAME_ENABLE=yes` gebaut) -
`enum layers` in `keymap_iso_common.h` endet entsprechend bei `_RGB`,
genau die 12 Ebenen, die `DYNAMIC_KEYMAP_LAYER_COUNT` in `config.h`
vorsieht.

**Kein physischer DIP-Schalter** - Default-Layer-Wahl läuft über die
`DF(...)`-Tasten auf `_SYS`.

## 🔑 Sondertasten-Legende

| Code | Tap | Hold |
|:--|:--|:--|
| `SP_FN` | Leertaste | `_FN` |
| `SYS_ESC` | Esc | `_SYS` |
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
| `QK_BOOT` | Neustart in den DFU-Bootloader | – |
| `EE_CLR` | Komplettes EEPROM zurücksetzen (siehe README.md, Abschnitt Flashen) | – |
| `RM_NEXT` u.ä. | RGB-Matrix-Steuerung (siehe `_RGB`-Ebene) | – |

Alle fünf `TD(...)`-Tasten haben zusätzlich einen **Doppeltap → Leader**
- funktioniert hier immer (`LEADER_WIRED` ist für nicht-speicherbeschränkte
Boards fest an).

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
`get_hold_on_other_key_press`, `keymap_logic.c`) sind aktiv - `SP_FN`/
`SYS_ESC` u.a. haben also das volle "tap-first"-Verhalten.

## 🚦 Leader (Menü-Position)

`QK_LEAD` sitzt auf Reihe 5 Position 7 (8-Tasten-Bottom-Row: LCtrl, LWin,
LAlt, `[Leertaste]`, RAlt, RWin, **`QK_LEAD`**, RCtrl). Sequenz innerhalb
von 500 ms tippen (`LEADER_TIMEOUT`, `LEADER_PER_KEY_TIMING`), danach
**kurz pausieren** (kein Bestätigungs-Tastendruck nötig - sonst werden
weitere Tasten in die Sequenz eingesammelt statt normal getippt).

**Zusätzlicher Schnellzugriff: Combo `F+J` → Leader** - beide Tasten
gleichzeitig drücken (`COMBO_TERM = 40 ms`), aber **nur auf `_QWERTZ` und
`_NEOQWERTZ1`** (`combo_should_trigger()` in `combos_bindings.inc`
schränkt das explizit auf diese beiden Default-Layer ein). Auf allen
anderen Ebenen tut die Kombination nichts Besonderes (F und J werden
normal getippt).

**Voller Leader-Baum** (`users/neo/leader/table.c`, identisch zu V1 Max/
V3 Max/Q3/V3 - nicht die reduzierte kbd8x_mk3-Variante):

| Gruppe | Sequenz | Beispiele |
|:--|:--|:--|
| Global | `H H` / `A A` | Gruppenübersicht / Selbsttest "LEADER OK" |
| OS | `O H/W/L/M/C/P` | Hilfe / Windows/Linux/macOS wählen / zyklisch / anzeigen |
| WezTerm Tabs | `T` / `T H/N/C/1-9` | öffnen / Hilfe / neu / schließen / Tab 1-9 |
| WezTerm Panes | `P H/S/V/X/N/P` | Hilfe / Split h/v / schließen / nächstes/voriges |
| Git | `G H/S/A/D/C/P/L/B/R/T/G` (+ Unterebenen) | status/add/diff/commit-Varianten/push/log/branch/rebase/tag/grep |
| fzf | `F H/F/E/P/O/M/D` | Hilfe/fzf/Auswahl→Editor/Preview/Dateien/Multi/Verzeichnis→cd |
| ripgrep | `R H/R/S/A/L/B/F/M` (+ `R C ./R T .`) | Hilfe/Basis/smart/git-files/less/Glob/fzf/multiline/Kontext/Typen |
| zoxide | `Z H/Z/I/A/L/T/R/F/N` | Hilfe/`z`/`zi`/add/list/top/remove/fzf→cd/nvim |
| yazi | `Y H/Y/Z/G/F` | Hilfe/yazi/mit zoxide/Git-Root/Dateiliste |
| zellij | `J H/J/L/A/K` (+ viele Unterebenen `T .`/`P .`/`F .`/`R .`/`O .`) | attach/list/attach/kill, Tabs, Panes, Fokus, Resize, Toggles |
| sed | `S`/`S H/R/F/E/Z` | Gruppen-Hilfe/`--help`/replace/filter/echo-Test/fzf→Muster |

Details/genaue Tastenfolgen: `users/neo/leader/table.c` (Kommentar je
Eintrag) oder `LEAD H H` am Gerät selbst tippen für die Live-Übersicht.

## ⌨️ Layer-Tabellen

Reihenfolge: Reihe 1 (Zahlenreihe), Reihe 2 (QWERTZ-Reihe), Reihe 3
(Grundreihe **inkl. ISO-Enter**), Reihe 4 (untere Buchstabenreihe,
**mit zusätzlicher ISO-Taste links neben Y/Z**), Reihe 5 (Leertastenreihe,
**8 Tasten**).

### Ebene 0 — `_QWERTZ`
| Reihe 1 | `QK_GESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_SYS_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `#` `Enter` |
| **Reihe 4** | `LShift` `<` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` **`QK_LEAD`** `RCtrl` |

### Ebene 1 — `_NEOQWERTZ1` (Basis)
| Reihe 1 | `Esc` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `=` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` `Enter` |
| **Reihe 4** | `NEO_SHIFT` `L4_MO_NEO` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `NEO_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` **`L4_MO_NEO`** `RWin` `QK_LEAD` `RCtrl` |

### Ebene 2 — `_NEOQWERTZ2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | `–` `°` `§` `ℓ` `»` `«` `$` `AltGr+E` `„` `"` `"` `ẞ` `` ` (tot)`` `Bksp` |
|:--|:--|
| **Reihe 2** | `–` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `*` *(groß)* |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` `Enter` *(groß)* |
| **Reihe 4** | `–` `L4_MO_NEO` `Y` `X` `C` `V` `B` `N` `M` `;` `:` `_` `–` *(groß)* |
| **Reihe 5** | wie Ebene 1 |

### Ebene 3 — `_NEOQWERTZ3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `$` `~` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `@` `TD_NEO_L3MO_LEAD` `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `#` `$` `\|` `~` `` ` `` `+` `%` `"` `'` `;` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 4 — `_NEOQWERTZ4` (Navigation/Numblock)
| Reihe 1 | `(frei)` ×11 `/` `*` `-` |
|:--|:--|
| **Reihe 2** | `Tab` `PgUp` `Bksp` `Up` `Del` `PgDn` `Down` `P7` `P8` `P9` `P+` `P-` `Enter` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `Home` `Left` `Down` `Right` `End` `Ins` `P4` `P5` `P6` `,` `.` `L4_MO_NEO` `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `Esc` `Tab` `Ins` `Enter` `Ctrl+Z` `:` `P1` `P2` `P3` `(frei)` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `P0` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 5 — `_NOTED1` (Basis)
| Reihe 1 | `SYS_ESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `-` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` `Enter` |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` `W` `G` `,` `.` `K` `NOTED_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `L4_MO_NOTED` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 6 — `_NOTED2` (Shift-Ebene, Symbole/Unicode)
| Reihe 1 | wie Ebene 2 |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` *(groß)* |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` `Enter` *(groß)* |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` `W` `G` `-` `.` `K` `NOTED_SHIFT` *(groß)* |
| **Reihe 5** | wie Ebene 5 |

### Ebene 7 — `_NOTED3` (Symbol-Ebene)
| Reihe 1 | `↻` `¹` `²` `³` `›` `‹` `¢` `¥` `‚` `'` `'` `-` `·` `Del` |
|:--|:--|
| **Reihe 2** | `(frei)` `@` `_` `[` `]` `^(tot)` `!` `<` `>` `=` `&` `@` `~` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `\` `/` `{` `}` `*` `?` `(` `)` `-` `:` `TD_NOTED_L3MO_LEAD` `@` `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NOTED` `#` `$` `\|` `~` `` ` `` `+` `%` `"` `'` `;` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

### Ebene 8 — `_NOTED4` (Navigation/Numblock)
| Reihe 1 | `SYS_ESC` `F1` … `F12` `Ins` |
|:--|:--|
| **Reihe 2** | `Tab` `Esc` `Home` `Up` `End` `PgUp` `PrtScn` `Ins` `Del` `Left` `Down` `Right` `Pause` |
| **Reihe 3** | `SYS_ESC` `Calc` `Prev` `Play` `Next` `Mute` `Vol-` `Vol+` `PrtScn` `ScrLk` `Pause` `Bksp` `Tab` `Enter` |
| **Reihe 4** | `LShift` `NumLk` `P/` `P7` `P8` `P9` `P+` `L3_MO_NOTED` `P4` `P5` `P6` `RShift` `Up` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `RWin` `QK_LEAD` `RCtrl` |

> `SYS_ESC` ersetzt hier (statt Tap-Dance) sowohl Esc- als auch
> Capslock-Position - kein Doppeltap→Leader auf dieser Ebene.

### Ebene 9 — `_FN`
| Reihe 1 | `Esc` `F1` … `F12` `Del` |
|:--|:--|
| **Reihe 2** | `Tab` `Macro6` `Macro4` `Macro2` `Macro0` `DM_PLY1` `(frei)` `DM_REC1` `DM_REC2` `DM_RSTP` `(frei)` `Macro10` `Macro8` |
| **Reihe 3** | `Esc` `Macro7` `Macro5` `Macro3` `Macro1` `DM_PLY2` `Left` `Down` `Up` `Right` `(frei)` `Macro11` `Macro9` `Enter` |
| **Reihe 4** | `LShift` `(frei)` ×5 `RGB_MO` `SYS_MO` `(frei)` `Home` `PgDn` `PgUp` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `RWin` `QK_LEAD` `RCtrl` |

`Macro0`-`Macro11` = `QK_MACRO_0`…`QK_MACRO_11` (VIA-Makro-Tab, 12 der 15
verfügbaren Slots liegen hier auf Tasten - die restlichen 3 nur über den
VIA-Makro-Tab erreichbar). `DM_REC1/2`/`DM_PLY1/2`/`DM_RSTP` (QMKs eigene
Dynamic Macros) funktionieren hier (`DYNAMIC_MACRO_ENABLE=yes`). `RGB_MO`
funktioniert ebenfalls (echtes RGB_MATRIX).

### Ebene 10 — `_SYS`
| Reihe 1 | `Esc` `QK_BOOT` `EE_CLR` `(frei)` ×10 `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `DF(_QWERTZ)` `DF(_NEOQWERTZ1)` `DF(_NOTED1)` `(frei)` ×9 |
| **Reihe 3** | `KC_CAPS` `UC_MODE_CYCLE` `UC_SET_LNX` `UC_SET_WIN` `UC_SET_WINC` `(frei)` ×8 `Enter` |
| **Reihe 4** | `LShift` `KC_OS_CYCLE` `KC_OS_WIN` `KC_OS_LNX` `KC_OS_MAC` `(frei)` ×7 `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `RWin` `QK_LEAD` `RCtrl` |

Anders als V1 Max/V3 Max liegen hier **keine** Bluetooth-Tasten auf
Reihe 2 (kein Wireless) - `SYS60_ROW2` bleibt die geteilte
Default-Version aus `users/neo/layouts/sys60.h`, nicht lokal
überschrieben.

### Ebene 11 — `_RGB`
Nutzt `layouts/rgb60.h` unverändert - `QK_RGB_MATRIX_*`-Keycodes
funktionieren hier tatsächlich (echtes RGB_MATRIX-Feature). Reihe 1:
Toggle/Modus±/Helligkeit±/Speed±, Reihe 2: Farbton±/Sättigung±. Für die
volle Einstellung eignet sich VIAs Lighting-Tab besser als diese
Tastenkombinationen (sofern die VIA-Definition lädt, siehe README.md).

## 📎 Legende: deutsche Sonderzeichen / Unicode

(Gleiche geteilte `keymap_extras/keymap_german.h`-Basis wie V1 Max/
V3 Max/kbd8x_mk3, keine Board-Unterschiede.)

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
