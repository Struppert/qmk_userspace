# Keychron V3 Max ISO Encoder – Tastenbelegung (Keymap `neo`)

Ergänzung zu `README.md` (dort: Hardware/Build/Flash/VIA) und
`BLUETOOTH.md` (dort: BT-Pairing/Host-Wechsel im Detail). Diese Datei
beschreibt, was auf welcher Taste liegt - Stand des kompilierten
Default-Keymaps (`keymaps/neo/keymap.c` + `users/neo/layouts/*.h` +
`users/neo/formfactors/ff_tkl_iso_v3.h`), nicht zwingend das, was gerade
über VIA live umbelegt wurde.

Formfaktor = **V3 (TKL)**, Wireless-Logik = **V1 Max** - siehe README.md
für die Herleitung. Wie V1 Max ist dieses Board **nicht**
speicherbeschränkt (`KEYBOARD` matcht keinen der kbd8x_mk3/bella-Filter
in `users/neo/rules.mk`) - volle Ausstattung: kompletter Leader-Baum,
Dynamic Macros, echtes RGB_MATRIX, alle Tap-Dance/Hold-Tuning-Hooks aus
`keymap_logic.c`.

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
| 12 | `_TETRIS` | Toggle | `TG(_TETRIS)` auf `_SYS` - **nur wenn beim Build `TETRIS_GAME_ENABLE=yes` gesetzt ist** (Default: aus) |

**Kein `_WIN_FN`** wie beim V1 Max: dessen zusätzliche, über LWin+Capslock
erreichbare Zweitweg-Ebene für die BT-Tasten wurde für dieses Board
bewusst weggelassen (siehe `BLUETOOTH.md`, Abschnitt "Kein `_WIN_FN`") -
`_BLUETOOTH_LAYER` ist in `keymap.c` entsprechend **nicht** definiert,
d.h. `_WIN_FN` taucht im `enum layers` (`keymap_iso_common.h`) für dieses
Board gar nicht erst auf, kostet also auch keinen EEPROM-Layer-Slot.

**Kein physischer DIP-Schalter** (`DIP_SWITCH_ENABLE` ist nirgends
gesetzt) - `dip_switch_update_user()` in `keymap.c` ist totes,
nie aufgerufenes Boilerplate (wie bei V1 Max). Default-Layer-Wahl läuft
über die `DF(...)`-Tasten auf `_SYS`.

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
| `TG(_TETRIS)` | Ebene `_TETRIS` an/aus (Toggle) | – |
| `TET_LEFT/RIGHT/DOWN/ROT/DROP` | Tetris-Steuerung (nur `_TETRIS`, nur `TETRIS_GAME_ENABLE=yes`) | – |
| `BT_HST1/2/3` | Bluetooth-Host 1/2/3 wählen | – |
| `P2P4G` | Auf 2.4G-Dongle-Modus wechseln | – |
| `BAT_LVL` | Akkustand anzeigen (LED-Balken) | – |
| `RM_NEXT` | RGB-Matrix-Modus weiter (F-Reihe, fix auf allen Ebenen) | – |

Alle fünf `TD(...)`-Tasten haben zusätzlich einen **Doppeltap → Leader**
- funktioniert hier immer (`LEADER_WIRED` ist für nicht-speicherbeschränkte
Boards fest an).

## 👆 Tap Dance im Detail

Identisch zu V1 Max/kbd8x_mk3 (gleiche geteilte `tap_dance_impl.c`/
Layout-Dateien):

| Tap-Dance | Einfacher Tap | Halten | Doppeltap |
|:--|:--|:--|:--|
| `TD_CAPS_SYS_LEAD` | `KC_ESC` | Layer `_SYS` an | Leader-Sequenz starten |
| `TD_CAPS_NEO_LEAD` | `KC_ESC` | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_CAPS_NOTED_LEAD` | `KC_ESC` | Layer `_NOTED3` an | Leader-Sequenz starten |
| `TD_NEO_L3MO_LEAD` | – | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_NOTED_L3MO_LEAD` | – | Layer `_NOTED3` an | Leader-Sequenz starten |

`TAPPING_TERM = 250 ms`, `RETRO_TAPPING` aktiv. Wie beim V1 Max sind hier
**alle** Tuning-Hooks (`get_tapping_term`, `get_ignore_mod_tap_interrupt`,
`get_hold_on_other_key_press`, `keymap_logic.c`) aktiv - `SP_FN`/`SYS_ESC`
u.a. haben also das volle "tap-first"-Verhalten.

## 🚦 Leader (Menü-Position)

`QK_LEAD` sitzt auf Reihe 5 Position 7 (8-Tasten-Bottom-Row: LCtrl, LWin,
LAlt, `[Leertaste]`, RAlt, RWin, **`QK_LEAD`**, RCtrl - TKL-Board, anders
als V1 Max/75% hat dieses Board eine eigenständige RWin-Taste). Sequenz
innerhalb von 500 ms tippen (`LEADER_TIMEOUT`, `LEADER_PER_KEY_TIMING`),
danach **kurz pausieren** (kein Bestätigungs-Tastendruck nötig - sonst
werden weitere Tasten in die Sequenz eingesammelt statt normal getippt).

**Zusätzlicher Schnellzugriff: Combo `F+J` → Leader** - beide Tasten
gleichzeitig drücken (`COMBO_TERM = 40 ms`), aber **nur auf `_QWERTZ`
und `_NEOQWERTZ1`** (`combo_should_trigger()` in `combos_bindings.inc`
schränkt das explizit auf diese beiden Default-Layer ein, `COMBO_ENABLE`
ist für dieses Board aktiv). Auf allen anderen Ebenen tut die Kombination
nichts Besonderes (F und J werden normal getippt).

**Voller Leader-Baum** (`users/neo/leader/table.c`, identisch zu V1 Max/
Q3/V3 - nicht die reduzierte kbd8x_mk3-Variante):

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

## 🎛️ Encoder

1 Encoder (`NUM_ENCODERS=1`), auf **allen** Ebenen einheitlich Vol-/Vol+
(anders als V1 Max, das pro Ebene unterschiedlich belegt - hier 1:1 von
V3s eigenem `encoder_map[]` übernommen):

| Ebene | CCW | CW |
|:--|:--|:--|
| alle (inkl. `_SYS`/`_RGB`/`_FN`) | `Vol-` | `Vol+` |
| `_TETRIS` (falls aktiv) | `(frei)` | `(frei)` |

## 🔒 Fixe Bereiche (auf allen Layern identisch, inkl. `_SYS`/`_RGB` und `_TETRIS` falls aktiv)

Anders als beim V1 Max (dort eine Ausnahme: `_WIN_FN`) gibt es hier
**keine** Ebene mit abweichender F-Reihe/Center-Block - der Formfaktor
(`ff_tkl_iso_v3.h`) reicht F-Reihe und Nav-Block auf **jeder** Ebene
unverändert durch, exakt wie bei V3 selbst.

**F-Reihe (physische Reihe 0, 17 Tasten inkl. Extra-Taste ganz rechts):**
`Esc · F1 · F2 · F3 · F4 · F5 · F6 · F7 · F8 · F9 · F10 · F11 · F12 · Vol+ · PrtScn · (frei) · RM_NEXT`

`RM_NEXT` (RGB-Matrix-Modus weiter) sitzt ganz rechts in der F-Reihe -
zusätzlich zur vollen Einstellung über `_RGB`-Ebene bzw. VIAs
Lighting-Tab.

**Navigationsblock (rechts, alle Layer):**
- Reihe 1: `Ins · Home · PgUp`
- Reihe 2: `Del · End · PgDn`
- Reihe 4: `Up`
- Reihe 5: `Left · Down · Right`

**Kein ISO-Enter-Split wie bei kbd8x_mk3** - Enter sitzt hier ganz normal
als letzte Taste jeder Reihe-3-Definition (`*_ROW3`), keine
`ROW3_DROP_ENT()`-Sonderbehandlung nötig.

## ⌨️ Layer-Tabellen

Reihenfolge: Reihe 1 (Zahlenreihe), Reihe 2 (QWERTZ-Reihe), Reihe 3
(Grundreihe **inkl. Enter**), Reihe 4 (untere Buchstabenreihe), Reihe 5
(Leertastenreihe, **8 Tasten** inkl. eigenständiger RWin-Taste und Leader
- anders als V1 Max/75%, das hier nur 7 Tasten ohne RWin hat).

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

`Macro0`-`Macro11` = `QK_MACRO_0`…`QK_MACRO_11` (VIA-Makro-Tab).
`DM_REC1/2`/`DM_PLY1/2`/`DM_RSTP` (Dynamic Macros) **funktionieren hier**
(`DYNAMIC_MACRO_ENABLE=yes` für nicht speicherbeschränkte Boards). `RGB_MO`
funktioniert ebenfalls (echtes RGB_MATRIX).

### Ebene 10 — `_SYS`
| Reihe 1 | `Esc` `QK_BOOT` `EE_CLR` `(frei)` ×10 `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `DF(_QWERTZ)` `DF(_NEOQWERTZ1)` `DF(_NOTED1)` `(frei)` ×3 `BT_HST1` `BT_HST2` `BT_HST3` `P2P4G` `BAT_LVL` `TETRIS_ENTRY` |
| **Reihe 3** | `KC_CAPS` `UC_MODE_CYCLE` `UC_SET_LNX` `UC_SET_WIN` `UC_SET_WINC` `(frei)` ×8 `Enter` |
| **Reihe 4** | `LShift` `KC_OS_CYCLE` `KC_OS_WIN` `KC_OS_LNX` `KC_OS_MAC` `(frei)` ×7 `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `RWin` `QK_LEAD` `RCtrl` |

**BT-Host-Wahl/Akkustand auf `_SYS`** (Reihe 2, Position 8-12 = physisch
`U`/`I`/`O`/`P`/`Ü`) - lokaler Override von `SYS60_ROW2` in `keymap.c`,
1:1 dieselbe Position wie beim V1 Max: Q/W/E bleiben Layer-Auswahl, R/T/Z
sind frei, die Bluetooth-Tasten liegen abgesetzt darunter auf U/I/O/P.
Details/Pairing-Ablauf siehe `BLUETOOTH.md`. **Anders als beim V1 Max
gibt es keinen zweiten Zugang über `_WIN_FN`** - dies ist der einzige Weg.

Alle Tasten dieser Ebene funktionieren hier (`process_record_user()`/
`keymap_logic.c` sind für dieses Board kompiliert).

**RGB-Layer-Indikator:** Solange `_SYS` gehalten wird, färbt
`rgb_matrix_indicators_user()` (in `keymap.c`) die komplette Matrix
**rot** ein - außer der aktive Transport ist gerade Bluetooth
(`get_transport() == TRANSPORT_BLUETOOTH`), dann **blau**, und zusätzlich
blinkt die Taste des aktuell verbundenen BT-Host-Slots (`BT_HST1/2/3`,
LED-Indizes aus `BT_INDCATION_LED_MATRIX_LIST`) im 300-ms-Takt weiß/aus.
Das ersetzt für die Haltedauer den sonst aktiven RGB-Matrix-Effekt
komplett - Logik 1:1 vom V1 Max übernommen, unabhängig vom Formfaktor.

`BT_INDCATION_LED_MATRIX_LIST`/`P24G_INDICATION_LED_INDEX` sind in
`keymaps/neo/config.h` lokal auf `{23, 24, 25}`/`26` (U/I/O/P)
überschrieben - andere Zahlen als beim V1 Max (`{21, 22, 23}`/`24`), weil
dieses Board 88 statt 82 LEDs hat und die Key-Matrix-zu-LED-Zuordnung
entsprechend anders liegt (siehe `BLUETOOTH.md` für die Herleitung).

### Ebene 11 — `_RGB`
Nutzt `layouts/rgb60.h` unverändert - `QK_RGB_MATRIX_*`-Keycodes
funktionieren hier tatsächlich (echtes RGB_MATRIX-Feature). Reihe 1:
Toggle/Modus±/Helligkeit±/Speed±, Reihe 2: Farbton±/Sättigung±. Für die
volle Einstellung eignet sich VIAs Lighting-Tab (`via.json`) besser als
diese Tastenkombinationen.

### Ebene 12 — `_TETRIS` (nur `TETRIS_GAME_ENABLE=yes`)

Eigenständiger Minispiel-Layer, rendert per HID-Keystrokes in ein
PC-Terminal - Details, PC-seitiges Setup und Caveats siehe
`keyboards/keychron/v1_max/TETRIS.md` (Portierungs-Referenz für alle
Boards, inkl. diesem hier). Build (Default: aus):
```
make keychron/v3_max/iso_encoder:neo QMK_USERSPACE=~/qmk_userspace TETRIS_GAME_ENABLE=yes
```
F-Reihe und Nav-Block sind wie bei allen anderen Ebenen fix (siehe oben),
Reihe 1/2/4 komplett `KC_NO`:

| Reihe | Belegung |
|:--|:--|
| **Reihe 3** | `KC_NO` ×6 `TET_LEFT`(H) `TET_DOWN`(J) `TET_ROT`(K) `TET_RIGHT`(L) `KC_NO` ×4 |
| **Reihe 5** | `KC_NO` `KC_NO` `KC_NO` `TET_DROP`(Leertaste) `KC_NO` `KC_NO` `KC_NO` `KC_NO` |

Vim-Bindung: `H`=links, `J`=runter (soft drop), `K`=rotieren, `L`=rechts,
Leertaste=hard drop, `Esc`=Layer verlassen.

**Caveat EEPROM:** `DYNAMIC_KEYMAP_LAYER_COUNT` wechselt zwischen 12 und
13 je nach `TETRIS_GAME_ENABLE` (`keymaps/neo/config.h`) - VIAs
EEPROM-Keymap-Layout hängt daran. Nach dem Umschalten des Flags einmal
EEPROM zurücksetzen (Bootmagic-Kombo oder `EEPROM_RESET`-Keycode), sonst
kann VIA verschobene Daten anzeigen.

**Gebaut, nicht geflasht:** wie der Default-Build auch mit
`TETRIS_GAME_ENABLE=yes` gegen den Fork getestet (siehe README.md) -
kompiliert sauber, am echten Board noch nicht ausprobiert.

## 📎 Legende: deutsche Sonderzeichen / Unicode

(Gleiche geteilte `keymap_extras/keymap_german.h`-Basis wie V1 Max/
kbd8x_mk3, keine Board-Unterschiede.)

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
