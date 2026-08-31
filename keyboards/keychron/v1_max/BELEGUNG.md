# Keychron V1 Max ISO Encoder – Tastenbelegung (Keymap `neo`)

Ergänzung zu `README.md` (dort: Hardware/Build/Flash/VIA). Diese Datei
beschreibt, was auf welcher Taste liegt - Stand des kompilierten
Default-Keymaps (`keymaps/neo/keymap.c` + `users/neo/layouts/*.h`),
nicht zwingend das, was gerade über VIA live umbelegt wurde.

Anders als kbd8x_mk3 ist dieses Board **nicht** speicherbeschränkt - es
läuft die volle `users/neo`-Ausstattung: kompletter Leader-Baum (nicht
nur rg+fzf), Dynamic Macros, RGB_MATRIX, alle Tap-Dance/Hold-Tuning-Hooks
aus `keymap_logic.c`.

## 🗂️ Layer-Übersicht

| # | Name | Typ | Erreichbar über |
|--:|:--|:--|:--|
| 0 | `_QWERTZ` | Default | Boot-Default, oder `_SYS`→`DF(_QWERTZ)` |
| 1 | `_NEOQWERTZ1` | Default | nur über `_SYS`→`DF(_NEOQWERTZ1)` |
| 2 | `_NEOQWERTZ2` | Momentary | Neo-Shift-Ebene, über `NEO_SHIFT` |
| 3 | `_NEOQWERTZ3` | Momentary | Capslock-Tap-Dance halten (Ebene 1-3), oder L3-Taste |
| 4 | `_NEOQWERTZ4` | Momentary | `L4_MO_NEO` (rechtes AltGr auf Ebene 1-3) |
| 5 | `_NOTED1` | Default | nur über `_SYS`→`DF(_NOTED1)` |
| 6 | `_NOTED2` | Momentary | Noted-Shift-Ebene, über `NOTED_SHIFT` |
| 7 | `_NOTED3` | Momentary | Capslock-Tap-Dance halten (Ebene 5-7), oder L3-Taste |
| 8 | `_NOTED4` | Momentary | `L4_MO_NOTED` |
| 9 | `_FN` | Momentary | `SP_FN` (Leertaste halten) |
| 10 | `_SYS` | Momentary | `SYS_ESC`/Capslock-Tap-Dance halten, oder `SYS_MO` (auf `_FN`) |
| 11 | `_RGB` | Momentary | `RGB_MO` (auf `_FN`) - **funktioniert hier tatsächlich**, dieses Board hat echtes `RGB_MATRIX` (anders als kbd8x_mk3) |
| 12 | `_WIN_FN` | Momentary | `MO(_WIN_FN)` an der LWin-Position, nur während `_SYS`/`_RGB` gehalten wird - siehe eigener Abschnitt unten |

**Kein physischer DIP-Schalter** (`DIP_SWITCH_ENABLE` ist nirgends
gesetzt) - `dip_switch_update_user()` in `keymap.c` ist totes,
nie aufgerufenes Boilerplate, wie beim kbd8x_mk3 vor der Bereinigung dort
(hier bewusst nicht angefasst, nur zur Info). Default-Layer-Wahl läuft
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
| `MO(_WIN_FN)` | – | `_WIN_FN` (nur zusammen mit `_SYS`/`_RGB`, siehe unten) |
| `TD(TD_CAPS_SYS_LEAD)` | Esc | `_SYS` (Ebene 0) |
| `TD(TD_CAPS_NEO_LEAD)` | Esc | `_NEOQWERTZ3` (Ebene 1-3) |
| `TD(TD_CAPS_NOTED_LEAD)` | Esc | `_NOTED3` (Ebene 5-7) |
| `TD(TD_NEO_L3MO_LEAD)` | – | `_NEOQWERTZ3` |
| `TD(TD_NOTED_L3MO_LEAD)` | – | `_NOTED3` |
| `QK_LEAD` | Leader-Sequenz starten | – |
| `DF(x)` | Default-Layer dauerhaft auf `x` setzen (EEPROM) | – |
| `BT_HST1/2/3` | Bluetooth-Host 1/2/3 wählen | – |
| `P2P4G` | Auf 2.4G-Dongle-Modus wechseln | – |
| `BAT_LVL` | Akkustand anzeigen (LED-Balken) | – |

Alle fünf `TD(...)`-Tasten haben zusätzlich einen **Doppeltap → Leader**
- funktioniert hier immer (`LEADER_WIRED` ist für nicht-speicherbeschränkte
Boards fest an).

## 👆 Tap Dance im Detail

Identisch zu kbd8x_mk3 (gleiche geteilte `tap_dance_impl.c`/Layout-Dateien):

| Tap-Dance | Einfacher Tap | Halten | Doppeltap |
|:--|:--|:--|:--|
| `TD_CAPS_SYS_LEAD` | `KC_ESC` | Layer `_SYS` an | Leader-Sequenz starten |
| `TD_CAPS_NEO_LEAD` | `KC_ESC` | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_CAPS_NOTED_LEAD` | `KC_ESC` | Layer `_NOTED3` an | Leader-Sequenz starten |
| `TD_NEO_L3MO_LEAD` | – | Layer `_NEOQWERTZ3` an | Leader-Sequenz starten |
| `TD_NOTED_L3MO_LEAD` | – | Layer `_NOTED3` an | Leader-Sequenz starten |

`TAPPING_TERM = 250 ms`, `RETRO_TAPPING` aktiv. Anders als kbd8x_mk3 sind
hier **alle** Tuning-Hooks (`get_tapping_term`, `get_ignore_mod_tap_interrupt`,
`get_hold_on_other_key_press`, `keymap_logic.c`) aktiv - `SP_FN`/`SYS_ESC`
u.a. haben also das volle "tap-first"-Verhalten.

## 🚦 Leader (Menü-Position)

`QK_LEAD` sitzt auf Reihe 5 Position 6 (7-Tasten-Bottom-Row: LCtrl, LWin,
LAlt, `[Leertaste]`, RAlt, **`QK_LEAD`**, RCtrl - es gibt auf diesem
75%-Board keine separate RWin-Taste). Sequenz innerhalb von 500 ms tippen
(`LEADER_TIMEOUT`, `LEADER_PER_KEY_TIMING`), danach **kurz pausieren**
(kein Bestätigungs-Tastendruck nötig - sonst werden weitere Tasten in die
Sequenz eingesammelt statt normal getippt).

**Voller Leader-Baum** (`users/neo/leader/table.c`, nicht die reduzierte
kbd8x_mk3-Variante):

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

1 Encoder (`NUM_ENCODERS=1`), CCW/CW pro Ebene unterschiedlich belegt:

| Ebene | CCW | CW |
|:--|:--|:--|
| `_QWERTZ`, `_NEOQWERTZ1`, `_SYS` | `Vol-` | `Vol+` |
| `_NEOQWERTZ2` | `Mausrad runter` | `Mausrad hoch` |
| `_NEOQWERTZ3`, `_NEOQWERTZ4`, `_NOTED1-4` | `Sättigung-`/`Farbton-`* | `Sättigung+`/`Farbton+`* |
| `_FN`, `_WIN_FN` | `Helligkeit runter (Display)` | `Helligkeit hoch (Display)` |
| `_RGB` | `Helligkeit-` | `Helligkeit+` |

\* `_NEOQWERTZ3` nutzt Sättigung, alle anderen (`_NEOQWERTZ4`,
`_NOTED1-4`) nutzen Farbton - siehe `encoder_map[]` in `keymap.c` für die
genaue Zuordnung.

## 🔒 Fixe Bereiche (auf allen 13 Layern identisch)

**F-Reihe (physische Reihe 0, 16 Tasten):**
`KC_ESC · F1 · F2 · F3 · F4 · F5 · F6 · F7 · F8 · F9 · F10 · F11 · F12 · KC_DEL · KC_MUTE`

**Center-Spalte (rechts, alle Layer):**
- Reihe 1: `PgUp`
- Reihe 2: `PgDn`
- Reihe 3: `Home`
- Reihe 4: `Up`
- Reihe 5: `Left · Down · Right`

**Kein ISO-Enter-Split wie bei kbd8x_mk3** - Enter sitzt hier ganz normal
als letzte Taste jeder Reihe-3-Definition (`*_ROW3`), keine
`ROW3_DROP_ENT()`-Sonderbehandlung nötig (75%-Layout hat physisch Platz
für ein eigenständiges Enter).

## ⌨️ Layer-Tabellen

Reihenfolge: Reihe 1 (Zahlenreihe), Reihe 2 (QWERTZ-Reihe), Reihe 3
(Grundreihe **inkl. Enter**), Reihe 4 (untere Buchstabenreihe), Reihe 5
(Leertastenreihe, 7 Tasten inkl. Leader - **keine RWin-Taste** auf diesem
Board).

### Ebene 0 — `_QWERTZ`
| Reihe 1 | `QK_GESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_SYS_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `#` `Enter` |
| **Reihe 4** | `LShift` `<` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` **`QK_LEAD`** `RCtrl` |

### Ebene 1 — `_NEOQWERTZ1` (Basis)
| Reihe 1 | `Esc` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `ß` `=` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Q` `W` `E` `R` `T` `Z` `U` `I` `O` `P` `Ü` `+` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `A` `S` `D` `F` `G` `H` `J` `K` `L` `Ö` `Ä` `TD_NEO_L3MO_LEAD` `Enter` |
| **Reihe 4** | `NEO_SHIFT` `L4_MO_NEO` `Y` `X` `C` `V` `B` `N` `M` `,` `.` `-` `NEO_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` **`L4_MO_NEO`** `QK_LEAD` `RCtrl` |

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
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `QK_LEAD` `RCtrl` |

### Ebene 4 — `_NEOQWERTZ4` (Navigation/Numblock)
| Reihe 1 | `(frei)` ×11 `/` `*` `-` |
|:--|:--|
| **Reihe 2** | `Tab` `PgUp` `Bksp` `Up` `Del` `PgDn` `Down` `P7` `P8` `P9` `P+` `P-` `Enter` |
| **Reihe 3** | `TD_CAPS_NEO_LEAD` `Home` `Left` `Down` `Right` `End` `Ins` `P4` `P5` `P6` `,` `.` `L4_MO_NEO` `Enter` |
| **Reihe 4** | `LShift` `L4_MO_NEO` `Esc` `Tab` `Ins` `Enter` `Ctrl+Z` `:` `P1` `P2` `P3` `(frei)` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `P0` `RAlt` `QK_LEAD` `RCtrl` |

### Ebene 5 — `_NOTED1` (Basis)
| Reihe 1 | `SYS_ESC` `1` `2` `3` `4` `5` `6` `7` `8` `9` `0` `-` `´(tot)` `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `Z` `Y` `U` `A` `Q` `P` `B` `M` `L` `F` `J` `ß` |
| **Reihe 3** | `TD_CAPS_NOTED_LEAD` `C` `S` `I` `E` `O` `D` `T` `N` `R` `H` `TD_NOTED_L3MO_LEAD` `´(tot)` `Enter` |
| **Reihe 4** | `NOTED_SHIFT` `L4_MO_NOTED` `V` `X` `Ü` `Ä` `Ö` `W` `G` `,` `.` `K` `NOTED_SHIFT` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `L4_MO_NOTED` `QK_LEAD` `RCtrl` |

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
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `QK_LEAD` `RCtrl` |

### Ebene 8 — `_NOTED4` (Navigation/Numblock)
| Reihe 1 | `SYS_ESC` `F1` … `F12` `Ins` |
|:--|:--|
| **Reihe 2** | `Tab` `Esc` `Home` `Up` `End` `PgUp` `PrtScn` `Ins` `Del` `Left` `Down` `Right` `Pause` |
| **Reihe 3** | `SYS_ESC` `Calc` `Prev` `Play` `Next` `Mute` `Vol-` `Vol+` `PrtScn` `ScrLk` `Pause` `Bksp` `Tab` `Enter` |
| **Reihe 4** | `LShift` `NumLk` `P/` `P7` `P8` `P9` `P+` `L3_MO_NOTED` `P4` `P5` `P6` `RShift` `Up` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `RAlt` `QK_LEAD` `RCtrl` |

> `SYS_ESC` ersetzt hier (statt Tap-Dance) sowohl Esc- als auch
> Capslock-Position - kein Doppeltap→Leader auf dieser Ebene.

### Ebene 9 — `_FN`
| Reihe 1 | `Esc` `F1` … `F12` `Del` |
|:--|:--|
| **Reihe 2** | `Tab` `Macro6` `Macro4` `Macro2` `Macro0` `DM_PLY1` `(frei)` `DM_REC1` `DM_REC2` `DM_RSTP` `(frei)` `Macro10` `Macro8` |
| **Reihe 3** | `Esc` `Macro7` `Macro5` `Macro3` `Macro1` `DM_PLY2` `Left` `Down` `Up` `Right` `(frei)` `Macro11` `Macro9` `Enter` |
| **Reihe 4** | `LShift` `(frei)` ×5 `RGB_MO` `SYS_MO` `(frei)` `Home` `PgDn` `PgUp` `RShift` |
| **Reihe 5** | `LCtrl` `LWin` `LAlt` `[Leertaste]` `(frei)` `QK_LEAD` `RCtrl` |

`Macro0`-`Macro11` = `QK_MACRO_0`…`QK_MACRO_11` (VIA-Makro-Tab).
`DM_REC1/2`/`DM_PLY1/2`/`DM_RSTP` (Dynamic Macros) **funktionieren hier**
(anders als kbd8x_mk3 - `DYNAMIC_MACRO_ENABLE=yes` für nicht
speicherbeschränkte Boards). `RGB_MO` funktioniert ebenfalls (echtes
RGB_MATRIX).

### Ebene 10 — `_SYS`
| Reihe 1 | `Esc` `QK_BOOT` `EE_CLR` `(frei)` ×9 `UG_TOGG`* `Bksp` |
|:--|:--|
| **Reihe 2** | `Tab` `DF(_QWERTZ)` `DF(_NEOQWERTZ1)` `DF(_NOTED1)` `BT_HST1` `BT_HST2` `BT_HST3` `P2P4G` `BAT_LVL` `(frei)` ×4 |
| **Reihe 3** | `KC_CAPS` `UC_MODE_CYCLE` `UC_SET_LNX` `UC_SET_WIN` `UC_SET_WINC` `(frei)` ×8 `Enter` |
| **Reihe 4** | `LShift` `KC_OS_CYCLE` `KC_OS_WIN` `KC_OS_LNX` `KC_OS_MAC` `(frei)` ×7 `RShift` |
| **Reihe 5** | `LCtrl` `MO(_WIN_FN)` `LAlt` `[Leertaste]` `RAlt` `QK_LEAD` `RCtrl` |

\* `UG_TOGG` in Reihe 1 ist die kbd8x_mk3-Konvention (RGBLIGHT-Toggle) -
**auf diesem Board nicht vorhanden**, da hier `RGBLIGHT_ENABLE=no` ist
(echtes RGB_MATRIX statt RGBLIGHT). Diese Position ist hier schlicht
`(frei)`.

**BT-Host-Wahl/Akkustand jetzt direkt auf `_SYS`** (Reihe 2, Position
5-9) - lokaler Override von `SYS60_ROW2` in `keymap.c`, seit 2026-08-31.
Vorher nur über die verschachtelte `_WIN_FN`-Ebene erreichbar (siehe
unten) - die bleibt zusätzlich bestehen.

Alle Tasten dieser Ebene funktionieren hier (anders als kbd8x_mk3) -
`process_record_user()`/`keymap_logic.c` sind für dieses Board kompiliert.

### Ebene 11 — `_RGB`
Nutzt `layouts/rgb60.h` unverändert - `QK_RGB_MATRIX_*`-Keycodes
funktionieren hier tatsächlich (echtes RGB_MATRIX-Feature). Reihe 1:
Toggle/Modus±/Helligkeit±/Speed±, Reihe 2: Farbton±/Sättigung±. Für die
volle Einstellung eignet sich VIAs Lighting-Tab (`via.json`) besser als
diese Tastenkombinationen.

### Ebene 12 — `_WIN_FN` (nur mit `_SYS`/`_RGB` gleichzeitig gehalten)

Kein eigener Eintrag in `keymaps[]` über die üblichen Formfactor-Makros -
direkt als `LAYOUT_iso_83(...)` in `keymap.c` definiert. Erreichbar, indem
man `_SYS` **oder** `_RGB` hält (Capslock-Tap-Dance bzw. `RGB_MO`) **und
gleichzeitig** die LWin-Position hält (dort liegt auf diesen beiden
Ebenen `MO(_WIN_FN)` statt `KC_LGUI`).

Reihenfolge wie in den übrigen Layer-Tabellen (F-Reihe, dann Reihe 1
Zahlenreihe … Reihe 5 Leertastenreihe) - hier per Tokenzahl gegen die
Quelle verifiziert (15,15,14,15,14,10 = 83, passt zu `LAYOUT_iso_83`):

| Reihe | Belegung |
|:--|:--|
| F-Reihe | `–` `Display-Helligkeit-` `Display-Helligkeit+` `KC_TASK` `KC_FILE` `RGB-Helligkeit-` `RGB-Helligkeit+` `Prev` `Play` `Next` `Mute` `Vol-` `Vol+` `–` `RGB_TOG` |
| Reihe 1 | `–` `BT_HST1` `BT_HST2` `BT_HST3` `P2P4G` `–` ×10 |
| Reihe 2 | `RGB_TOG` `RGB_MOD` `Helligkeit+` `Farbton+` `Sättigung+` `Speed+` `–` ×8 |
| Reihe 3 | `–` `RGB_RMOD` `Helligkeit-` `Farbton-` `Sättigung-` `Speed-` `–` ×8 `End` |
| Reihe 4 | `–` ×6 `BAT_LVL` `NumLk` `–` ×6 |
| Reihe 5 | `–` ×10 (komplett transparent - fällt durch auf `_SYS`/`_RGB` darunter) |

`BT_HST1-3`/`P2P4G`/`BAT_LVL` sind hier redundant zur neuen direkten
`_SYS`-Bindung oben - bewusst nicht entfernt, falls die
Zwei-Tasten-Kombination aus Gewohnheit weiterverwendet wird.

## 📎 Legende: deutsche Sonderzeichen / Unicode

Identisch zu kbd8x_mk3 - siehe dortige `BELEGUNG.md`, Abschnitt "Legende:
deutsche Sonderzeichen / Unicode" (gleiche geteilte `keymap_extras/
keymap_german.h`-Basis, keine Board-Unterschiede).
