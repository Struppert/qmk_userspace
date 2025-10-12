# QMK Leader Framework Architektur

Diese Datei beschreibt, wie die Komponenten `handler`, `intent`, `table`, `trie` und `help` im modularen QMK-Leader-System zusammenarbeiten.

---

## Überblick

Das Framework erweitert QMKs Leader-Key-System zu einem modularen, kontextsensitiven Steuer- und Hilfesystem.  
Es basiert auf folgenden Schichten:

1. **QMK-Hooks (`users/neo/leader.c`)**
2. **Core & Module (`leader/leader.c`, `leader/core.c`)**
3. **Table (`leader/table.c`)**
4. **Trie (`leader/trie.c`)**
5. **Dispatch (Handlers + Intents)**
6. **Help-System (`leader/help.c`)**

---

## Datenfluss und Ablauf

### 1. QMK Hooks

```c
// users/neo/leader.c
void leader_start_user(void) { leader_module_start(); }
void leader_end_user(void)   { leader_module_end(); }
```

- QMK ruft diese Funktionen bei Beginn/Ende einer Leader-Sequenz auf.
- Der Glue ruft intern `leader_module_start()` und `leader_module_end()` auf.

---

### 2. Modul-Einstieg (`leader/leader.c`)

```c
void leader_module_start(void) { leader_trie_build(); }
bool leader_module_end(void)   { return leader_table_dispatch(); }
```

- Baut beim Start das Trie aus der Tabelle.
- Führt am Ende das Dispatching aus.

---

### 3. Tabelle (`leader/table.c`)

Die Tabelle definiert alle Leader-Sequenzen deklarativ:

```c
LENTRY2(KC_G, KC_S, h_git_status, LFLAG_NONE, "git status")
```

- **seq**: Keycode-Folge (z. B. `KC_G`, `KC_S`)
- **handler**: Funktionszeiger auf `h_*`
- **help**: Text, der in der Hilfe erscheint

---

### 4. Trie (`leader/trie.c`)

- Baut eine Baumstruktur über alle Key-Sequenzen.
- Ermöglicht:
  - schnelle Suche nach exakter Sequenz
  - Ermittlung der Kinderknoten für Hilfe (nächste gültige Tasten)

API:
```c
void leader_trie_build(void);
const LeaderEntry* leader_trie_find_exact(const uint16_t *seq, uint8_t len);
size_t leader_trie_children(const uint16_t *prefix, uint8_t plen, uint16_t *out_keys, size_t max_out, bool *has_exact);
```

---

### 5. Dispatch-System (`leader/core.c`)

- Vergleicht die eingegebene Sequenz mit der Tabelle.
- Führt bei Treffer den zugehörigen Handler aus:
  ```c
  if (match(seq, entry)) entry->handler();
  ```
- Linearer Scan genügt, da Leader-Sequenzen sehr kurz sind.

---

### 6. Handler (`leader/handlers_*.c`)

- Semantische „Aktionen“ auf höherer Ebene.
- Beispiel (`handlers_git.c`):

```c
void h_git_status(void) { send_line("git status"); }
```

- Handler können direkt Strings senden oder Intents aufrufen.

---

### 7. Intents (`leader/intents_*.c`)

- Abstraktionsebene für OS-/Shell-spezifische Aufrufe.

Beispiel (`intents_dev.c`):
```c
void intent_build_and_run_debug(void) {
    if (is_win())
        send_line("msbuild zis50.sln /p:Configuration=Debug && .\\bin\\Debug\\app.exe");
    else
        send_line("cmake --build build --config Debug && ./build/app");
}
```

- `send_line_os()` abstrahiert PowerShell ↔ POSIX.

---

### 8. Hilfe-System (`leader/help.c`)

- Nutzt das Trie, um mögliche Fortsetzungen einer laufenden Sequenz anzuzeigen.
- Beispiel:
  ```c
  leader_print_help_for_prefix([KC_G], 1);
  ```
- Gibt alle `KC_*`-Nachfolger + ihre Help-Strings aus.

---

## Warum diese Trennung?

| Ebene | Zweck | Beispiel |
|-------|--------|-----------|
| Table | Deklarativ: Welche Tasten? | `KC_G, KC_S` |
| Trie | Struktur für Hilfe | Nachfolger anzeigen |
| Core | Dispatcher | Aufruf des Handlers |
| Handler | Bedeutungsebene | „Git Status“ |
| Intent | Technische Ausführung | `send_line_os()` |
| Help | Nutzerhilfe | Kontextsensitiv |

---

## Erweiterung – neuen Befehl hinzufügen

1. **Intent hinzufügen**
   ```c
   void intent_my_action(void){ send_line("echo myaction"); }
   ```
2. **Handler hinzufügen**
   ```c
   void h_my_action(void){ intent_my_action(); }
   ```
3. **Tabelle erweitern**
   ```c
   LENTRY2(KC_M, KC_A, h_my_action, LFLAG_NONE, "My Action")
   ```
4. Neu kompilieren – fertig.

---

## Vorteile

- Modular, kontextsensitiv, OS-portabel  
- Hilfe aus Datenstruktur statt Hardcode  
- Intents wiederverwendbar  
- Keine doppelten Strings/Keys  
- Einfach erweiterbar ohne Eingriff in QMK intern

---

## Dateienstruktur (relevant)

```
leader/
  core.c/h
  table.c
  trie.c/h
  help.c/h
  os_shell.c/h
  intents_*.c/h
  handlers_*.c/h
users/neo/leader.c
```

---

© 2025 – Leader Framework Architecture for QMK
