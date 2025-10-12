# QMK Leader Framework – Beispielmodul **sed** (vollständig)

Dieses Dokument führt dich _end-to-end_ durch das Hinzufügen eines neuen **sed**-Moduls
(im Stil deiner bestehenden Architektur: **table → trie → handlers → intents → help**).  
Alle Snippets sind passend zu deiner aktuellen Help-API (`help_block_begin(void)`, `help_block_line(const char*)`).

---

## 🧭 Überblick

**Ziel:** Ein neues Leader-Untermodul **sed** mit ein paar nützlichen Kommandos.

**Tastensequenzen (Vorschlag):**
```
LEAD S H  → sed --help
LEAD S R  → sed -i 's/foo/bar/g' file.txt
LEAD S F  → sed -n '/pattern/p' file.txt
LEAD S E  → echo 'text' | sed 's/foo/bar/'
LEAD S Z  → (fzf → Datei) | sed -n '/pattern/p' {file}
```

**Dateien (neu/ergänzt):**
```
leader/
 ├─ intents_sed.c          (NEU)
 ├─ handlers_sed.c         (NEU)
 ├─ intents.h              (+ Prototypen)
 ├─ handlers.h             (+ Prototypen)
 ├─ table.c                (+ sed-Einträge)
 ├─ help.c                 (+ help_sed + Gruppenübersicht)
 └─ rules.mk               (+ 2 neue SRC-Zeilen)
```

---

## 1) Intents (Low-Level, OS/Shell-Dispatch)

**`leader/intents_sed.c`**
```c
#include QMK_KEYBOARD_H
#include "os_shell.h"
#include "intents.h"

void intent_sed_help(void) {
    // schlicht, da sed überall gleich aufgerufen wird
    send_line_os("sed --help", "sed --help", NULL);
}

void intent_sed_replace_template(void) {
    // Template mit Cursor vor dem Dateinamen
    SEND_STRING("sed -i 's/foo/bar/g' file.txt");
    tap_code(KC_LEFT);
}

void intent_sed_filter_template(void) {
    SEND_STRING("sed -n '/pattern/p' file.txt");
    tap_code(KC_LEFT);
}

void intent_sed_echo_test(void) {
    SEND_STRING("echo 'text' | sed 's/foo/bar/'");
}

void intent_sed_fzf_files(void) {
    // POSIX: rg --files (Fallback find) → fzf → sed
    // PWSH:  rekursiv Dateien sammeln → fzf → sed
    send_line_os(
        "(rg --files || find . -type f) | fzf | xargs -I{} sed -n '/pattern/p' {}",
        "(Get-ChildItem -Recurse -File | ForEach-Object { $_.FullName }) | "
        "fzf | ForEach-Object { if($_){ sed -n '/pattern/p' $_ } }",
        NULL
    );
}
```

### `leader/intents.h` ergänzen
```c
// sed
void intent_sed_help(void);
void intent_sed_replace_template(void);
void intent_sed_filter_template(void);
void intent_sed_echo_test(void);
void intent_sed_fzf_files(void);
```

---

## 2) Handler (Semantik-Ebene, ruft Intents oder send_line direkt)

**`leader/handlers_sed.c`**
```c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"
#include "help.h"

// Kontext-/Gruppenhilfe (LEAD S H oder via ctx)
void h_s_help(void){ help_sed(); }

// Einzelaktionen
void h_sed_help(void){    intent_sed_help(); }
void h_sed_replace(void){ intent_sed_replace_template(); }
void h_sed_filter(void){  intent_sed_filter_template(); }
void h_sed_echo(void){    intent_sed_echo_test(); }
void h_sed_fzf(void){     intent_sed_fzf_files(); }
```

### `leader/handlers.h` ergänzen
```c
// sed
void h_s_help(void);
void h_sed_help(void);
void h_sed_replace(void);
void h_sed_filter(void);
void h_sed_echo(void);
void h_sed_fzf(void);
```

---

## 3) Tabelle (Deklarativ: Sequenzen + Hilfe)

In **`leader/table.c`** an geeigneter Stelle hinzufügen (z. B. nach zellij):

```c
// ───────────── sed group (S) ─────────────
LENTRY1(KC_S,              h_s_help,         LFLAG_NONE, "sed: Hilfe/Gruppe"),
LENTRY2(KC_S, KC_H,        h_sed_help,       LFLAG_NONE, "sed --help"),
LENTRY2(KC_S, KC_R,        h_sed_replace,    LFLAG_NONE, "replace template"),
LENTRY2(KC_S, KC_F,        h_sed_filter,     LFLAG_NONE, "filter lines (pattern)"),
LENTRY2(KC_S, KC_E,        h_sed_echo,       LFLAG_NONE, "echo test pipeline"),
LENTRY2(KC_S, KC_Z,        h_sed_fzf,        LFLAG_NONE, "fzf → sed pattern"),
```

> **Hinweis:** Die Hilfe-Texte in der Tabelle werden (über das Trie) von deiner kontextsensitiven Hilfe genutzt.

---

## 4) Help: neue Gruppe + Gruppenübersicht aktualisieren

Deine Help-API erwartet **ein Argument pro Zeile** und kein Argument bei `help_block_begin()`.  
Hierfür zwei Änderungen in **`leader/help.c`**:

### 4.1 `help_sed()` hinzufügen
```c
void help_sed(void) {
  help_block_hr();
  help_block_line("#Hilfe: sed (LEAD S …)");
  help_block_hr();

  help_block_line("#S H → sed --help");
  help_block_line("#S R → replace template  (sed -i 's/foo/bar/g' file.txt)");
  help_block_line("#S F → filter lines      (sed -n '/pattern/p' file.txt)");
  help_block_line("#S E → echo test         (echo 'text' | sed 's/foo/bar/')");
  help_block_line("#S Z → fzf → sed pattern");
}
```

### 4.2 `help_groups_overview()` ergänzen
Füge die **sed-Zeile** ein (am Ende des Blocks vor dem Schluss-HR ist am übersichtlichsten):

```c
void help_groups_overview(void) {
  help_block_hr();
  help_block_line("#Leader Hilfe – Gruppenübersicht");
  help_block_line("#LEAD O H   → OS wählen/anzeigen (Windows/Linux/macOS)");
  help_block_line("#LEAD T H   → Tabs (WezTerm)");
  help_block_line("#LEAD P H   → Panes (WezTerm)");
  help_block_line("#LEAD G H   → Git "
                  "(Status/Diff/Commit/Push/Log/Branch/Rebase/Tags/Grep)");
  help_block_line("#LEAD F H   → fzf");
  help_block_line("#LEAD R H   → ripgrep (rg)");
  help_block_line("#LEAD Z H   → zoxide");
  help_block_line("#LEAD Y H   → yazi");
  help_block_line("#LEAD J H   → zellij");
  help_block_line("#LEAD S H   → sed (Stream Editor)");
  help_block_hr();
  help_block_line("#Allgemein/Test: LEAD A A → "LEADER OK"");
}
```

> **Optional:** Wenn du „S“ bereits als Gruppentaste für etwas anderes verwendest, wähle eine alternative Gruppe (z. B. `KC_E` für „edit“).

---

## 5) rules.mk anpassen

In deiner `users/neo/rules.mk` (oder wo du die SRC pflegst) ergänzen:

```make
SRC += leader/intents_sed.c        leader/handlers_sed.c
```

> Achte darauf, dass **keine** alten monolithischen Dateien (`leader/intents.c`, `leader/handlers.c`) mehr in `SRC` stehen.

---

## 6) Testplan

- Build: `qmk compile -kb keychron/v1/iso_encoder -km neo`  
- Laufzeitchecks:
  - `LEAD S H` → Hilfe erscheint im Terminal
  - `LEAD S R` → Template wird getippt, Cursor vor `file.txt`
  - `LEAD S F` → Filtertemplate
  - `LEAD S E` → Echo-Pipe
  - `LEAD S Z` → fzf-Pipe (setzt `fzf` und ggf. `rg` voraus)
- Kontext-Hilfe: `LEAD S` + deine Hilfe-Taste (z. B. `H`) → Trie listet `H,R,F,E,Z` und zieht Texte aus der Tabelle.

---

## 7) Troubleshooting

- **„too many arguments to function ‘help_block_begin/line’“**  
  → Nutze genau die Signaturen wie oben (kein Titel-Parameter für `help_block_begin()`, jeweils **ein** `const char*` für `help_block_line()`).

- **„redefinition of is_win/is_linux/is_mac“**  
  → Die Helfer kommen aus `os_shell.h`. In `intents_*.c` **nicht** erneut definieren.

- **Unused handler / -Werror**  
  → Jeder `h_*` muss in `leader/table.c` referenziert sein – sonst entfernen oder eintragen.

- **Konflikt mit bestehender S-Gruppe**  
  → Gruppe wechseln (andere erste Taste) und Tabelle + Help-Texte anpassen.

---

## 8) Warum diese Aufteilung? (Kurz)

| Ebene   | Zweck                                | Beispiel                         |
|---------|--------------------------------------|----------------------------------|
| Table   | Deklarativ: Sequenzen + Hilfe        | `KC_S, KC_R → h_sed_replace`     |
| Trie    | Kontext-/Prefix-Hilfe                | `leader_trie_children([KC_S],1)` |
| Handler | Semantik (ruft Intents / send_line)  | `h_sed_filter()`                 |
| Intent  | Technische Ausführung, OS/Shell-Rout.| `intent_sed_fzf_files()`         |
| Help    | Darstellung (HereDoc / PS-String)    | `help_sed()` + Gruppenübersicht  |

---

Viel Spaß beim Erweitern!  
Wenn du möchtest, kann ich daraus auch direkt ein **Patch-Set** (diff) gegen deinen aktuellen Stand generieren.
