// users/neo/leader.c
// ──────────────────────────────────────────────────────────────────────────────
// Leader-Shortcuts mit OS-Auto-Routing
//
// Diese Datei stellt ein einheitliches Leader-Interface bereit: Du drückst
// dieselbe Leader-Sequenz, egal ob der Host Windows (PowerShell),
// Linux (fish) oder macOS (fish) ist. Die OS-spezifische Variante wird
// automatisch über os_state::os_current() gewählt.
//
// • OS-Wahl (persistent) liegt in os_state (externes Modul).
//   Leader:  LEAD O W / O L / O M   → OS setzen
//            LEAD O C               → OS zyklisch
//            LEAD O P               → aktuellen OS-Modus ausgeben
//
// • WezTerm-Intents: öffnen, splitten, Tabs/Panes steuern, neue Shell spawnen.
//   Unter Windows wird PowerShell gestartet, unter Linux/macOS fish.
//
// • Bestehende Git / rg / fzf / zoxide / yazi Kommandos: Varianten für
//   POSIX vs. PowerShell wurden vereinheitlicht. Es gibt jetzt eine Sequenz,
//   die je nach os_current() die korrekte Befehlsfolge sendet.
//
// • SendString ist für deutsches Layout vorbereitet (sendstring_german.h).
//   Falls du US-Layout sendest, kannst du das einkommentieren.
//
// Anpassungspunkte:
//   - is_win()/is_linux()/is_mac() ggf. an deine os_state-Enums anpassen
//   - WezTerm-Pfade/Startkommandos bei Bedarf ändern
//   - Weitere Intents/Kommandos einfach über die Router-Helper hinzufügen
// ──────────────────────────────────────────────────────────────────────────────

#include QMK_KEYBOARD_H

#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

#include "os_state.h" // stellt os_current(), os_set(), os_cycle(), os_print() bereit

// ──────────────────────────────────────────────────────────────────────────────
// SendString → korrekt für DE-Layout (nur hier einbinden, sonst Linker-Fehler!)
#define SENDSTRING_LANGUAGE de_DE
#include "sendstring_german.h"

// ──────────────────────────────────────────────────────────────────────────────
// OS Helpers (Router)
static inline bool is_win(void) { return os_current() == OS_WIN; }
static inline bool is_linux(void) { return os_current() == OS_LNX; }
static inline bool is_mac(void) { return os_current() == OS_MAC; }

// ──────────────────────────────────────────────────────────────────────────────
// Kleine Helfer
static inline void send_and_enter(const char *s) {
  send_string(s);
  tap_code(KC_ENT);
}
static inline void move_left(uint8_t n) {
  while (n--)
    tap_code(KC_LEFT);
}
static inline void type_quotes_and_place_cursor(void) {
  // tippt "" und setzt Cursor dazwischen
  send_string("\"\"");
  tap_code(KC_LEFT);
}
static inline void send_cmd_with_empty_quotes(const char *prefix) {
  // z.B. prefix="rg -n --hidden --glob '!.git' " → tippt prefix + "" und setzt
  // Cursor
  send_string(prefix);
  type_quotes_and_place_cursor();
}
static inline void send_line(const char *s) { send_and_enter(s); }

// Router: wähle je OS richtigen Befehl (mac fällt auf POSIX zurück, wenn nicht
// gesetzt)
static inline void send_line_os(const char *posix_cmd, const char *pwsh_cmd,
                                const char *mac_cmd_opt) {
  if (is_win()) {
    if (pwsh_cmd) {
      send_line(pwsh_cmd);
      return;
    }
    // Fallback: POSIX-Variante (nicht ideal, aber besser als NOP)
    send_line(posix_cmd);
  } else if (is_mac()) {
    if (mac_cmd_opt && *mac_cmd_opt) {
      send_line(mac_cmd_opt);
      return;
    }
    send_line(posix_cmd);
  } else {
    send_line(posix_cmd);
  }
}

// Wie oben, aber mit editierbarer Query ("" + Cursor)
static inline void send_query_os(const char *posix_prefix,
                                 const char *pwsh_prefix,
                                 const char *mac_prefix_opt) {
  if (is_win()) {
    if (pwsh_prefix) {
      send_cmd_with_empty_quotes(pwsh_prefix);
      return;
    }
    send_cmd_with_empty_quotes(posix_prefix);
  } else if (is_mac()) {
    if (mac_prefix_opt && *mac_prefix_opt) {
      send_cmd_with_empty_quotes(mac_prefix_opt);
      return;
    }
    send_cmd_with_empty_quotes(posix_prefix);
  } else {
    send_cmd_with_empty_quotes(posix_prefix);
  }
}

// ──────────────────────────────────────────────────────────────────────────────
// WezTerm Intents (OS-spezifisch)
static inline void intent_open_wezterm(void) {
  if (is_win()) {
    SEND_STRING(SS_LGUI("r"));
    wait_ms(120);
    send_string("wezterm");
    tap_code(KC_ENTER);
  } else if (is_linux() || is_mac()) {
    // Öffne Standard-Terminal-Kürzel; alternativ direkt "wezterm" tippen:
    send_line("wezterm");
  }
}

static inline void
intent_split_pane_h(void) { // horizontaler Split (nebeneinander)
  if (is_win())
    send_line("wezterm cli split-pane -- powershell");
  else
    send_line("wezterm cli split-pane -- fish");
}
static inline void
intent_split_pane_v(void) { // vertikaler Split (untereinander)
  if (is_win())
    send_line("wezterm cli split-pane --horizontal -- powershell");
  else
    send_line("wezterm cli split-pane --horizontal -- fish");
}

static inline void intent_kill_pane(void) {
  send_line("wezterm cli kill-pane");
}
static inline void intent_next_pane_right(void) {
  send_line("wezterm cli activate-pane-direction Right");
}
static inline void intent_prev_pane_left(void) {
  send_line("wezterm cli activate-pane-direction Left");
}
static inline void intent_new_tab_here(void) {
  if (is_win())
    send_line("wezterm cli spawn --cwd . -- powershell");
  else
    send_line("wezterm cli spawn --cwd . -- fish");
}
static inline void intent_close_tab(void) {
  send_line("wezterm cli close-tab");
}
static inline void intent_activate_tab(uint8_t n) {
  // n: 0..8; WezTerm nutzt Indexe – passe ggf. an
  char buf[32];
  snprintf(buf, sizeof(buf), "wezterm cli activate-tab %u", (unsigned)n);
  send_line(buf);
}

// Dev-Environment
static inline void intent_dev_env(void) {
  if (is_win()) {
    // Pfad ggf. an deine VS-Edition anpassen:
    send_line("call \"C:\\Program Files\\Microsoft Visual "
              "Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\"");
  } else {
    send_line("source ~/.local/share/nvim/env.fish");
  }
}

// Build & Run (Beispiel)
static inline void intent_build_and_run_debug(void) {
  if (is_win()) {
    send_line(
        "msbuild zis50.sln /p:Configuration=Debug && .\\bin\\Debug\\app.exe");
  } else {
    send_line("cmake --build build --config Debug && ./build/app");
  }
}

// ripgrep im Projekt
static inline void intent_ripgrep_project(void) {
  // Beide können dieselbe rg-Zeile senden – Unterschiede entstehen meist erst
  // in Pipes
  send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' ");
}

// rg → fzf → nvim (+Zeile)
static inline void intent_rg_fzf_open_editor_line(void) {
  if (is_win()) {
    send_line(
        "rg --line-number --no-heading -S \"\" | fzf | ForEach-Object { $p = "
        "$_ -split \":\"; if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }");
    tap_code(KC_LEFT); // Cursor zurück in die Query
  } else {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    tap_code(KC_LEFT);
  }
}

// git grep → fzf → nvim (+Zeile)
static inline void intent_gitgrep_fzf_open_editor_line(void) {
  if (is_win()) {
    send_line("git grep -n -I -e \"\" | fzf | ForEach-Object { $p = $_ -split "
              "\":\"; if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }");
    tap_code(KC_LEFT);
  } else {
    SEND_STRING("git grep -n -I -e \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    tap_code(KC_LEFT);
  }
}

// zoxide + fzf → cd
static inline void intent_zoxide_fzf_cd(void) {
  send_line_os("cd \"$(zoxide query -l | fzf)\"",      // POSIX
               "Set-Location (zoxide query -l | fzf)", // PowerShell
               NULL                                    // mac = POSIX
  );
}

// yazi im Git-Root
static inline void intent_yazi_git_root(void) {
  send_line_os("yazi \"$(git rev-parse --show-toplevel)\"", // POSIX
               "yazi (git rev-parse --show-toplevel)",      // PowerShell
               NULL);
}

// ──────────────────────────────────────────────────────────────────────────────
// Leader Cheat Sheet (kompakt & gruppiert)
// Hinweis:
//  • OS wird automatisch per os_current() erkannt (permanent wählbar via LEAD O
//  …) • Befehle mit "" setzen den Cursor automatisch zwischen die Quotes •
//  Viele Befehle enden auf ENTER und laufen sofort los
// ──────────────────────────────────────────────────────────────────────────────
//
// Allgemein/Test
//  LEAD A A         → "LEADER OK"
//
// OS wählen / anzeigen
//  LEAD O W         → OS=Windows
//  LEAD O L         → OS=Linux
//  LEAD O M         → OS=macOS
//  LEAD O C         → OS zyklisch (Win→Linux→mac→…)
//  LEAD O P         → aktuellen OS-Modus in die aktive App tippen
//
// WezTerm
//  LEAD T           → WezTerm öffnen (Win: Win+R→wezterm, andere: "wezterm")
//  LEAD P S         → Pane split horizontal (nebeneinander)
//  LEAD P V         → Pane split vertikal (untereinander)
//  LEAD P X         → Pane kill
//  LEAD P N         → Pane rechts aktivieren
//  LEAD P P         → Pane links aktivieren
//  LEAD T N         → Neuer Tab im aktuellen Pfad
//  LEAD T C         → Tab schließen
//  LEAD T 1..9      → Tab 1..9 aktivieren
//
// Git: Status/Diff/Commit/Push/Log/Branch/Rebase/Tags (OS-neutral)
//  LEAD G S         → git status
//  LEAD G A         → git add -A
//  LEAD G D         → git diff
//  LEAD G D S       → git diff --staged
//
//  LEAD G C         → git commit -m ""                      (Cursor in "")
//  LEAD G C F       → git commit -m "fix: "
//  LEAD G C B       → git commit -m "feat: "
//  LEAD G C R       → git commit -m "refactor: "
//  LEAD G C T       → git commit -m "test: "
//  LEAD G C D       → git commit -m "docs: "
//
//  LEAD G P         → git push
//  LEAD G P 1       → git push --set-upstream origin HEAD
//  LEAD G P 9       → git push --tags
//  LEAD G P !       → git push --force-with-lease
//
//  LEAD G L         → git log --oneline --graph --decorate -n 30
//  LEAD G L A       → git log --all --decorate --oneline --graph
//
//  LEAD G B N       → git switch -c                        (neuen Branch
//  anlegen) LEAD G B S       → git switch -                        (zurück zum
//  vorherigen) LEAD G B D       → git branch -D (Branch löschen; Namen tippen)
//
//  LEAD G R         → git rebase -i HEAD~                  (Interaktiv; Zahl
//  tippen) LEAD G T N       → git tag -a v (Version tippen) LEAD G T P       →
//  git push --tags
//
// Git Grep (Arbeitsbaum durchsuchen; OS-Route)
//  LEAD G G         → git grep -n -I -e ""                 (Basis; nummeriert)
//  LEAD G G I       → git grep -n -I -i -e ""              (ignore-case)
//  LEAD G G W       → git grep -n -I -w -e ""              (Wortgrenze)
//  LEAD G G L       → git grep -I -l -e ""                 (nur Dateinamen)
//  LEAD G G F       → git grep … | fzf → ${EDITOR}/nvim +Zeile (OS-Router)
//
// „Pickaxe“ (Historie nach Content-Änderungen durchsuchen)
//  LEAD G P S       → git log -S "" --patch --stat         (String in Patches)
//  LEAD G P G       → git log -G "" --patch --stat         (Regex in Patches)
//
// fzf
//  LEAD F F         → fzf
//  LEAD F E         → fzf | xargs -r $EDITOR bzw. PowerShell-Äquivalent
//  LEAD F P         → fzf --preview 'bat …'
//  LEAD G F F       → git ls-files | fzf
//
// ripgrep (rg) & Varianten (OS-neutral / Router wenn nötig)
//  LEAD R R         → rg -n --hidden --glob '!.git' ""     (Basis)
//  LEAD R S         → rg -n -S --hidden --glob '!.git' ""  (smart-case)
//  LEAD R A         → git ls-files | rg -n --hidden … ""   (nur getrackte)
//  LEAD R L         → rg -n --hidden … "" | less -R        (scrollbar)
//  LEAD R G R       → rg -n --hidden -g '*.go' ""
//  LEAD R B         → rg -n --hidden -g '*.rs' ""
//  LEAD R F         → rg … "" | fzf → ${EDITOR}/nvim +Zeile (OS-Router)
//
// zoxide
//  LEAD Z Z         → z ""
//  LEAD Z I         → zi
//  LEAD Z A         → zoxide add
//  LEAD Z L         → zoxide query -l ""
//  LEAD Z T         → zoxide query -t
//  LEAD Z R         → zoxide remove ""
//  LEAD Z F         → zoxide | fzf → cd (OS-Router)
//  LEAD Z N         → nvim "$(zoxide query -i)" bzw. PowerShell-Äquivalent
//
// yazi (TUI Dateimanager)
//  LEAD Y Y         → yazi
//  LEAD Y Z         → yazi "$(zoxide query -i)" (OS-Router möglich)
//  LEAD Y G         → yazi Git-Root (OS-Router)
//
// ──────────────────────────────────────────────────────────────────────────────

#if defined(LEADER_ENABLE)

void leader_start_user(void) {
  // optional: Feedback beim Start (RGB blitzen etc.)
}

void leader_end_user(void) {

  // ── Sanity / Test
  if (leader_sequence_two_keys(KC_A, KC_A)) {
    send_line("LEADER OK");
  }

  // ── OS wählen / anzeigen ───────────────────────────────────────────────────
  if (leader_sequence_two_keys(KC_O, KC_W)) {
    os_set(OS_WIN);
  } else if (leader_sequence_two_keys(KC_O, KC_L)) {
    os_set(OS_LNX);
  } else if (leader_sequence_two_keys(KC_O, KC_M)) {
    os_set(OS_MAC);
  } else if (leader_sequence_two_keys(KC_O, KC_C)) {
    os_cycle();
  } else if (leader_sequence_two_keys(KC_O, KC_P)) {
    os_print();
  }

  // ── WezTerm ────────────────────────────────────────────────────────────────
  else if (leader_sequence_one_key(KC_T)) {
    intent_open_wezterm();
  } else if (leader_sequence_two_keys(KC_P, KC_S)) {
    intent_split_pane_h();
  } else if (leader_sequence_two_keys(KC_P, KC_V)) {
    intent_split_pane_v();
  } else if (leader_sequence_two_keys(KC_P, KC_X)) {
    intent_kill_pane();
  } else if (leader_sequence_two_keys(KC_P, KC_N)) {
    intent_next_pane_right();
  } else if (leader_sequence_two_keys(KC_P, KC_P)) {
    intent_prev_pane_left();
  } else if (leader_sequence_two_keys(KC_T, KC_N)) {
    intent_new_tab_here();
  } else if (leader_sequence_two_keys(KC_T, KC_C)) {
    intent_close_tab();
  } else if (leader_sequence_two_keys(KC_T, KC_1)) {
    intent_activate_tab(0);
  } else if (leader_sequence_two_keys(KC_T, KC_2)) {
    intent_activate_tab(1);
  } else if (leader_sequence_two_keys(KC_T, KC_3)) {
    intent_activate_tab(2);
  } else if (leader_sequence_two_keys(KC_T, KC_4)) {
    intent_activate_tab(3);
  } else if (leader_sequence_two_keys(KC_T, KC_5)) {
    intent_activate_tab(4);
  } else if (leader_sequence_two_keys(KC_T, KC_6)) {
    intent_activate_tab(5);
  } else if (leader_sequence_two_keys(KC_T, KC_7)) {
    intent_activate_tab(6);
  } else if (leader_sequence_two_keys(KC_T, KC_8)) {
    intent_activate_tab(7);
  } else if (leader_sequence_two_keys(KC_T, KC_9)) {
    intent_activate_tab(8);
  }

  // ── Git (OS-neutral) ───────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_G, KC_S)) {
    send_line("git status");
  } else if (leader_sequence_two_keys(KC_G, KC_A)) {
    send_line("git add -A");
  } else if (leader_sequence_two_keys(KC_G, KC_D)) {
    send_line("git diff");
  } else if (leader_sequence_three_keys(KC_G, KC_D, KC_S)) {
    send_line("git diff --staged");
  }

  // commit helpers
  else if (leader_sequence_two_keys(KC_G, KC_C)) {
    SEND_STRING("git commit -m \"\"");
    tap_code(KC_LEFT);
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_F)) {
    SEND_STRING("git commit -m \"fix: ");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_B)) {
    SEND_STRING("git commit -m \"feat: ");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_R)) {
    SEND_STRING("git commit -m \"refactor: ");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_T)) {
    SEND_STRING("git commit -m \"test: ");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_D)) {
    SEND_STRING("git commit -m \"docs: ");
  }

  // push variants
  else if (leader_sequence_two_keys(KC_G, KC_P)) {
    send_line("git push");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_1)) {
    send_line("git push --set-upstream origin HEAD");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_9)) {
    send_line("git push --tags");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_EXLM)) {
    send_line("git push --force-with-lease");
  }

  // logs
  else if (leader_sequence_two_keys(KC_G, KC_L)) {
    send_line("git log --oneline --graph --decorate -n 30");
  } else if (leader_sequence_three_keys(KC_G, KC_L, KC_A)) {
    send_line("git log --all --decorate --oneline --graph");
  }

  // branch helpers
  else if (leader_sequence_three_keys(KC_G, KC_B, KC_N)) {
    SEND_STRING("git switch -c ");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_S)) {
    send_line("git switch -");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_D)) {
    SEND_STRING("git branch -D ");
  }

  // rebase / tags
  else if (leader_sequence_two_keys(KC_G, KC_R)) {
    SEND_STRING("git rebase -i HEAD~");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_N)) {
    SEND_STRING("git tag -a v");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_P)) {
    send_line("git push --tags");
  }

  // ── git grep (OS-Route) ───────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_G, KC_G)) {
    send_cmd_with_empty_quotes("git grep -n -I -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_I)) {
    send_cmd_with_empty_quotes("git grep -n -I -i -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_W)) {
    send_cmd_with_empty_quotes("git grep -n -I -w -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_L)) {
    send_cmd_with_empty_quotes("git grep -I -l -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_F)) {
    intent_gitgrep_fzf_open_editor_line();
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_P)) {
    SEND_STRING("git grep -n -I -e \"\" -- ");
    tap_code(KC_LEFT);
  }

  // Pickaxe
  else if (leader_sequence_three_keys(KC_G, KC_P, KC_S)) {
    send_cmd_with_empty_quotes("git log -S ");
    send_string(" --patch --stat");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_G)) {
    send_cmd_with_empty_quotes("git log -G ");
    send_string(" --patch --stat");
  }

  // ── fzf ────────────────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_F, KC_F)) {
    send_line("fzf");
  } else if (leader_sequence_two_keys(KC_F, KC_E)) {
    // POSIX: fzf | xargs -r $EDITOR
    // PowerShell: fzf | ForEach-Object { nvim $_ }
    send_line_os("fzf | xargs -r ${EDITOR:-nvim}",
                 "fzf | ForEach-Object { nvim $_ }", NULL);
  } else if (leader_sequence_two_keys(KC_F, KC_P)) {
    send_line(
        "fzf --preview 'bat --style=numbers --color=always {} | head -500'");
  } else if (leader_sequence_three_keys(KC_G, KC_F, KC_F)) {
    send_line("git ls-files | fzf");
  }

  // ── ripgrep / suchen (OS-Route wo nötig) ───────────────────────────────────
  else if (leader_sequence_two_keys(KC_R, KC_R)) {
    intent_ripgrep_project();
  } else if (leader_sequence_two_keys(KC_R, KC_S)) {
    send_cmd_with_empty_quotes("rg -n -S --hidden --glob '!.git' ");
  } else if (leader_sequence_two_keys(KC_R, KC_A)) {
    send_cmd_with_empty_quotes("git ls-files | rg -n --hidden --glob '!.git' ");
  } else if (leader_sequence_two_keys(KC_R, KC_L)) {
    SEND_STRING("rg -n --hidden --glob '!.git' \"\" | less -R");
    tap_code(KC_LEFT);
  } else if (leader_sequence_three_keys(KC_R, KC_G, KC_R)) {
    send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.go' ");
  } else if (leader_sequence_two_keys(KC_R, KC_B)) {
    send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.rs' ");
  } else if (leader_sequence_two_keys(KC_R, KC_F)) {
    intent_rg_fzf_open_editor_line();
  }

  // ── zoxide (OS-Route) ─────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Z, KC_Z)) {
    send_cmd_with_empty_quotes("z ");
  } else if (leader_sequence_two_keys(KC_Z, KC_I)) {
    send_line("zi");
  } else if (leader_sequence_two_keys(KC_Z, KC_A)) {
    send_line("zoxide add");
  } else if (leader_sequence_two_keys(KC_Z, KC_L)) {
    send_cmd_with_empty_quotes("zoxide query -l ");
  } else if (leader_sequence_two_keys(KC_Z, KC_T)) {
    send_line("zoxide query -t");
  } else if (leader_sequence_two_keys(KC_Z, KC_R)) {
    send_cmd_with_empty_quotes("zoxide remove ");
  } else if (leader_sequence_two_keys(KC_Z, KC_F)) {
    intent_zoxide_fzf_cd();
  } else if (leader_sequence_two_keys(KC_Z, KC_N)) {
    send_line_os("nvim \"$(zoxide query -i)\"", "nvim (zoxide query -i)", NULL);
  }

  // ── yazi (OS-Route) ───────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Y, KC_Y)) {
    send_line("yazi");
  } else if (leader_sequence_two_keys(KC_Y, KC_Z)) {
    send_line_os("yazi \"$(zoxide query -i)\"", "yazi (zoxide query -i)", NULL);
  } else if (leader_sequence_two_keys(KC_Y, KC_G)) {
    intent_yazi_git_root();
  } else if (leader_sequence_two_keys(KC_Y, KC_F)) {
    // Datei-/Pfadliste; je nach yazi-Version springt es ins Verzeichnis
    send_line_os(
        "yazi \"$(git ls-files 2>/dev/null || rg --files)\"",
        "yazi (git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files })",
        NULL);
  }
}
#endif /* LEADER_ENABLE */
