// users/neo/leader.c
// ──────────────────────────────────────────────────────────────────────────────
// Leader-Shortcuts mit OS-Auto-Routing + kontextsensitive Hilfe
//
// Neu:
//  • LEAD H H → Gruppenübersicht / Cheat-Sheet Kopf
//  • LEAD O H / T H / P H / G H / R H / Z H / Y H / F H → Hilfe pro Gruppe
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
// // gesetzt)
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

// ——— Hilfe-Ausgabe (nur Text; nichts wird ausgeführt) ————————————————
static inline void help_hr(void) {
  send_line("────────────────────────────────────────");
}
static inline void help_head(const char *title) {
  help_hr();
  send_line(title);
  help_hr();
}
static inline void help_line(const char *s) { send_line(s); }

// Gruppen-Hilfetexte
static inline void help_groups_overview(void) {
  help_head("#Leader Hilfe – Gruppenübersicht");
  help_line("#LEAD O H   → OS wählen/anzeigen (Windows/Linux/macOS)");
  help_line("#LEAD T H   → Tabs (WezTerm)");
  help_line("#LEAD P H   → Panes (WezTerm)");
  help_line("#LEAD G H   → Git "
            "(Status/Diff/Commit/Push/Log/Branch/Rebase/Tags/Grep)");
  help_line("#LEAD F H   → fzf");
  help_line("#LEAD R H   → ripgrep (rg) & Varianten");
  help_line("#LEAD Z H   → zoxide");
  help_line("#LEAD Y H   → yazi (Dateimanager)");
  help_line("#LEAD J H   → zellij (Terminal-Multiplexer)");
  help_hr();
  help_line("#Allgemein/Test: LEAD A A → \"LEADER OK\"");
  help_line(
      "#Hinweis: Befehle mit \"\" setzen den Cursor zwischen die Quotes.");
}

static inline void help_os(void) {
  help_head("#Hilfe: OS wählen / anzeigen (LEAD O …)");
  help_line("#O W → OS = Windows");
  help_line("#O L → OS = Linux");
  help_line("#O M → OS = macOS");
  help_line("#O C → OS zyklisch wechseln (Win→Linux→mac→…)");
  help_line("#O P → aktuellen OS-Modus in die aktive App tippen");
}

static inline void help_tabs(void) {
  help_head("#Hilfe: WezTerm Tabs (LEAD T …)");
  help_line("#T     → WezTerm öffnen");
  help_line("#T N   → Neuer Tab im aktuellen Pfad");
  help_line("#T C   → Tab schließen");
  help_line("#T 1..9→ Tab 1..9 aktivieren");
}

static inline void help_panes(void) {
  help_head("#Hilfe: WezTerm Panes (LEAD P …)");
  help_line("#P S   → Split horizontal (nebeneinander)");
  help_line("#P V   → Split vertikal (untereinander)");
  help_line("#P X   → Aktuelles Pane schließen");
  help_line("#P N   → Nächstes Pane (rechts) aktivieren");
  help_line("#P P   → Vorheriges Pane (links) aktivieren");
}

static inline void help_git(void) {
  help_head("#Hilfe: Git (LEAD G …)");
  help_line("#G S       → git status");
  help_line("#G A       → git add -A");
  help_line("#G D       → git diff");
  help_line("#G D S     → git diff --staged");
  help_line("#G C       → git commit -m \"…\"");
  help_line("#G C F/B/R/T/D → fix/feat/refactor/test/docs:");
  help_line("#G P       → git push");
  help_line("#G P 1     → push + set-upstream origin HEAD");
  help_line("#G P 9     → push --tags");
  help_line("#G P !     → push --force-with-lease");
  help_line("#G L       → log kompakt (30)");
  help_line("#G L A     → log all graph");
  help_line("#G B N/S/D → branch neu / zurück / löschen");
  help_line("#G R       → rebase -i HEAD~");
  help_line("#G T N/P   → tag -a v… / push --tags");
  help_line("#G G       → git grep -n -I -e \"…\"");
  help_line("#G G I/W/L → grep (ignore-case / wort / nur dateien)");
  help_line("#G G F     → grep | fzf | nvim +Zeile");
  help_line("#G P S/G   → pickaxe -S/-G … --patch --stat");
}

static inline void help_fzf(void) {
  help_head("#Hilfe: fzf (LEAD F …)");
  help_line("#F F     → fzf");
  help_line("#F E     → Auswahl → $EDITOR / nvim");
  help_line("#F P     → Preview mit bat");
  help_line("#G F F   → git ls-files | fzf");
  help_line("#— Dateien —");
  help_line("#F O     → Dateien mit Preview (Enter öffnet in $EDITOR)");
  help_line("#F M     → Multi-Select → $EDITOR <file1…>");
  help_line("#— Verzeichnisse —");
  help_line("#F D     → Verzeichnis wählen → cd (fd/find Fallback)");
}

static inline void help_rg(void) {
  help_head("#Hilfe: ripgrep (LEAD R …)");
  help_line("#R R     → Basis: rg -n --hidden --glob '!.git' \"…\"");
  help_line("#R S     → smart: -S");
  help_line("#R W     → wortgrenze: -w");
  help_line("#R I     → ignore-case: -i");
  help_line("#R L     → Liste mit less -R");
  help_line("#R F     → rg … | fzf | nvim +Zeile (Preview)");
  help_line("#— Kontext —");
  help_line("#R C A   → -C 3   (3 Zeilen Kontext um Treffer)");
  help_line("#R C U   → -B 3   (3 Zeilen davor)");
  help_line("#R C D   → -A 3   (3 Zeilen danach)");
  help_line("#— Multiline/Regex —");
  help_line("#R M     → -U     (multiline / ungeschützte Dateien)");
  help_line("#— Sprachen/Dateitypen —");
  help_line("#R T C   → C/C++: -t c -t cpp -t h -t hpp");
  help_line("#R T R   → Rust:  -t rust");
  help_line("#R T G   → Go:    -t go");
}

static inline void help_zoxide(void) {
  help_head("#Hilfe: zoxide (LEAD Z …)");
  help_line("#Z Z   → z \"…\"");
  help_line("#Z I   → zi");
  help_line("#Z A   → zoxide add");
  help_line("#Z L   → zoxide query -l \"…\"");
  help_line("#Z T   → zoxide query -t");
  help_line("#Z R   → zoxide remove \"…\"");
  help_line("#Z F   → zoxide | fzf → cd (OS-Route)");
  help_line("#Z N   → nvim $(zoxide query -i)");
}

static inline void help_yazi(void) {
  help_head("#Hilfe: yazi (LEAD Y …)");
  help_line("#Y Y   → yazi");
  help_line("#Y Z   → yazi \"$(zoxide query -i)\" (OS-Route möglich)");
  help_line("#Y G   → yazi Git-Root");
  help_line("#Y F   → yazi mit Datei-/Pfadliste");
}

// ───── 2: Zellij-Hilfe ─────
static inline void help_zellij(void) {
  help_head("#Hilfe: zellij (LEAD J …)");
  help_line("#J J       → attach -c main (Hauptsession starten/beitreten)");
  help_line("#J L       → list-sessions");
  help_line("#J A       → attach <name>");
  help_line("#J K       → kill-session <name> (Name tippen)");
  help_line("#J S R     → rename-session <name> (Name tippen)");
  help_line("#— Tabs —");
  help_line("#J T N     → new-tab");
  help_line("#J T P     → go-to-previous-tab");
  help_line("#J T X     → close-tab");
  help_line("#J T G 1..9→ go-to-tab 1..9");
  help_line("#J T R     → rename-tab <name>");
  help_line("#J T L     → new-tab --layout <file> --name <name>");
  help_line("#— Panes —");
  help_line("#J P H     → new-pane -d right   (Vertikal teilen)");
  help_line("#J P V     → new-pane -d down    (Horizontal teilen)");
  help_line("#J P F     → new-pane -f         (Floating Pane)");
  help_line("#J P X     → close-pane");
  help_line("#J F H/L/K/J → move-focus left/right/up/down");
  help_line("#J R H/L/K/J → resize left/right/up/down (kleine Schritte)");
  help_line("#— Floating / Frames / Fullscreen —");
  help_line("#J O F     → toggle-floating-panes   (alle zeigen/verstecken)");
  help_line("#J O E     → toggle-pane-embed-or-floating (einzelnes Pane)");
  help_line("#J O U     → toggle-fullscreen");
  help_line("#J O B     → toggle-pane-frames");
}

// zellij Intents (OS-spezifisch)
// Sessions / Attach
static inline void intent_zellij_attach_main(void) {
  send_line("zellij attach -c main");
}
static inline void intent_zellij_list_sessions(void) {
  send_line("zellij list-sessions");
}
static inline void intent_zellij_attach_prompt(void) {
  SEND_STRING("zellij attach ");
} // Name tippen + ENTER
static inline void intent_zellij_kill_session_prompt(void) {
  SEND_STRING("zellij kill-session ");
}
static inline void intent_zellij_rename_session_prompt(void) {
  SEND_STRING("zellij action rename-session ");
}

// Tabs
static inline void intent_zellij_new_tab(void) {
  send_line("zellij action new-tab");
}
static inline void intent_zellij_prev_tab(void) {
  send_line("zellij action go-to-previous-tab");
}
static inline void intent_zellij_close_tab(void) {
  send_line("zellij action close-tab");
}
static inline void intent_zellij_go_to_tab(uint8_t n) {
  char buf[32];
  snprintf(buf, sizeof(buf), "zellij action go-to-tab %u", (unsigned)n);
  send_line(buf);
}
static inline void intent_zellij_rename_tab_prompt(void) {
  SEND_STRING("zellij action rename-tab ");
}
static inline void intent_zellij_new_tab_layout_prompt(void) {
  // Beispiel: new-tab --layout ~/.config/zellij/layouts/dev.kdl --name dev
  SEND_STRING(
      "zellij action new-tab --layout "); // dann Pfad + optional: " --name "
}

// Panes (splits / focus / resize / close / floating)
static inline void intent_zellij_new_pane_right(void) {
  send_line("zellij action new-pane -d right");
} // vertikaler Split
static inline void intent_zellij_new_pane_down(void) {
  send_line("zellij action new-pane -d down");
} // horizontaler Split
static inline void intent_zellij_new_pane_floating(void) {
  send_line("zellij action new-pane -f");
}
static inline void intent_zellij_close_pane(void) {
  send_line("zellij action close-pane");
}

static inline void intent_zellij_move_focus(const char *dir) {
  char buf[48];
  snprintf(buf, sizeof(buf), "zellij action move-focus %s", dir);
  send_line(buf);
}
static inline void intent_zellij_resize(const char *dir, int amt) {
  char buf[64];
  snprintf(buf, sizeof(buf), "zellij action resize %s %d", dir, amt);
  send_line(buf);
}

// Toggles: floating, embed, fullscreen, frames
static inline void intent_zellij_toggle_floating_all(void) {
  send_line("zellij action toggle-floating-panes");
}
static inline void intent_zellij_toggle_embed_or_floating(void) {
  send_line("zellij action toggle-pane-embed-or-floating");
}
static inline void intent_zellij_toggle_fullscreen(void) {
  send_line("zellij action toggle-fullscreen");
}
static inline void intent_zellij_toggle_frames(void) {
  send_line("zellij action toggle-pane-frames");
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
  char buf[32];
  snprintf(buf, sizeof(buf), "wezterm cli activate-tab %u", (unsigned)n);
  send_line(buf);
}

// Dev-Environment
static inline void intent_dev_env(void) {
  if (is_win()) {
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
  send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' ");
}

// rg → fzf → nvim (+Zeile)
static inline void intent_rg_fzf_open_editor_line(void) {
  if (is_win()) {
    send_line(
        "rg --line-number --no-heading -S \"\" | fzf | ForEach-Object { $p = "
        "$_ -split \":\"; if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }");
    tap_code(KC_LEFT);
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

static inline void intent_fzf_files_preview_open(void) {
  if (is_win()) {
    // bevorzugt git-Index; Fallback rg --files
    send_line("(git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files }) | "
              "fzf --ansi --preview 'bat --style=numbers --color=always {} | "
              "head -500' | "
              "ForEach-Object { if($_){ nvim $_ } }");
  } else {
    SEND_STRING("(git ls-files 2>/dev/null || rg --files) | "
                "fzf --ansi --preview 'bat --style=numbers --color=always {} | "
                "head -500' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
  }
}

static inline void intent_fzf_files_multi_open(void) {
  if (is_win()) {
    send_line("(git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files }) | "
              "fzf -m | ForEach-Object { if($_){ nvim $_ } }");
  } else {
    SEND_STRING("(git ls-files 2>/dev/null || rg --files) | fzf -m | ");
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
    SEND_STRING("xargs -r ${EDITOR:-nvim}\n");
#else
    SEND_STRING("xargs -r nvim\n");
#endif
  }
}

static inline void intent_fzf_dirs_cd(void) {
  if (is_win()) {
    // Powershell: rekursiv Verzeichnisse, Auswahl → cd
    send_line(
        "Get-ChildItem -Recurse -Directory | ForEach-Object { $_.FullName } | "
        "fzf | ForEach-Object { if($_){ Set-Location $_ } }");
  } else {
    // bevorzugt fd; Fallback find
    send_line("(fd -t d -H . 2>/dev/null || find . -type d -not -path "
              "'*/\\.git/*') | fzf | xargs -r -I{} sh -lc 'cd \"{}\"'");
  }
}
static inline void intent_rg_fzf_preview_open_editor_line(void) {
  if (is_win()) {
    // file:line:… → öffne in nvim +line; mit Preview
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi --delimiter=: "
                "--preview 'bat --style=numbers --color=always "
                "--highlight-line {2} {1} | head -500' | "
                "ForEach-Object { $p = $_ -split \":\"; if ($p.Length -ge 2) { "
                "nvim \"+$($p[1])\" $p[0] } }\n");
    tap_code(KC_LEFT);
  } else {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi --delimiter=: "
                "--preview 'bat --style=numbers --color=always "
                "--highlight-line {2} {1} | head -500' | "
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
static inline void intent_rg_context_around3(void) {
  SEND_STRING("rg -n -C 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}
static inline void intent_rg_context_before3(void) {
  SEND_STRING("rg -n -B 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}
static inline void intent_rg_context_after3(void) {
  SEND_STRING("rg -n -A 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}
static inline void intent_rg_multiline(void) {
  SEND_STRING("rg -n -U --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}

static inline void intent_rg_types_cpp(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t c -t cpp -t h -t hpp \"\"");
  tap_code(KC_LEFT);
}

static inline void intent_rg_types_rust(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t rust \"\"");
  tap_code(KC_LEFT);
}

static inline void intent_rg_types_go(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t go \"\"");
  tap_code(KC_LEFT);
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
               NULL);
}

// yazi im Git-Root
static inline void intent_yazi_git_root(void) {
  send_line_os("yazi \"$(git rev-parse --show-toplevel)\"", // POSIX
               "yazi (git rev-parse --show-toplevel)",      // PowerShell
               NULL);
}

// ──────────────────────────────────────────────────────────────────────────────
// Leader Cheat Sheet (kompakt & gruppiert) – unverändert in Kommentaren
// ──────────────────────────────────────────────────────────────────────────────

#if defined(LEADER_ENABLE)

void leader_start_user(void) {
  // optional: Feedback beim Start (RGB blitzen etc.)
}

void leader_end_user(void) {

  // ——— Globale Hilfe ————————————————————————————————————————————————
  if (leader_sequence_two_keys(KC_H, KC_H)) {
    help_groups_overview();
  }

  // ── Sanity / Test
  else if (leader_sequence_two_keys(KC_A, KC_A)) {
    send_line("LEADER OK");
  }

  // ── OS wählen / anzeigen + Hilfe ───────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_O, KC_H)) {
    help_os();
  } else if (leader_sequence_two_keys(KC_O, KC_W)) {
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

  // ── WezTerm Tabs + Hilfe ───────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_T, KC_H)) {
    help_tabs();
  } else if (leader_sequence_one_key(KC_T)) {
    intent_open_wezterm();
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

  // ── WezTerm Panes + Hilfe ──────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_P, KC_H)) {
    help_panes();
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
  }

  // ── Git + Hilfe ────────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_G, KC_H)) {
    help_git();
  } else if (leader_sequence_two_keys(KC_G, KC_S)) {
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

  // git grep (OS-Route)
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

  // ── fzf + Hilfe ────────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_F, KC_H)) {
    help_fzf();
  } else if (leader_sequence_two_keys(KC_F, KC_F)) {
    send_line("fzf");
  } else if (leader_sequence_two_keys(KC_F, KC_E)) {
    send_line_os("fzf | xargs -r ${EDITOR:-nvim}",
                 "fzf | ForEach-Object { nvim $_ }", NULL);
  } else if (leader_sequence_two_keys(KC_F, KC_P)) {
    send_line(
        "fzf --preview 'bat --style=numbers --color=always {} | head -500'");
  } else if (leader_sequence_three_keys(KC_G, KC_F, KC_F)) {
    send_line("git ls-files | fzf");
  }

  // ── ripgrep + Hilfe ────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_R, KC_H)) {
    help_rg();
  } else if (leader_sequence_two_keys(KC_R, KC_R)) {
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
  } else if (leader_sequence_three_keys(KC_R, KC_C, KC_A)) { // -C 3
    intent_rg_context_around3();
  } else if (leader_sequence_three_keys(KC_R, KC_C, KC_U)) { // -B 3
    intent_rg_context_before3();
  } else if (leader_sequence_three_keys(KC_R, KC_C, KC_D)) { // -A 3
    intent_rg_context_after3();
  } else if (leader_sequence_two_keys(KC_R, KC_M)) { // -U
    intent_rg_multiline();
  }
  // Typfilter
  else if (leader_sequence_three_keys(KC_R, KC_T, KC_C)) { // C/C++
    intent_rg_types_cpp();
  } else if (leader_sequence_three_keys(KC_R, KC_T, KC_R)) { // Rust
    intent_rg_types_rust();
  } else if (leader_sequence_three_keys(KC_R, KC_T, KC_G)) { // Go
    intent_rg_types_go();
  }
  // fzf + Preview + Open (+Zeile)
  else if (leader_sequence_three_keys(KC_R, KC_F, KC_P)) {
    intent_rg_fzf_preview_open_editor_line();
  }

  // —— fzf erweitert ————————————————————————————————————————————————
  else if (leader_sequence_two_keys(
               KC_F, KC_O)) { // Dateien mit Preview, Enter→Editor
    intent_fzf_files_preview_open();
  } else if (leader_sequence_two_keys(KC_F, KC_M)) { // Multi-Select → Editor
    intent_fzf_files_multi_open();
  } else if (leader_sequence_two_keys(KC_F, KC_D)) { // Verzeichnis wählen → cd
    intent_fzf_dirs_cd();
  }

  // ── zoxide + Hilfe ─────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Z, KC_H)) {
    help_zoxide();
  } else if (leader_sequence_two_keys(KC_Z, KC_Z)) {
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

  // ── yazi + Hilfe ───────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Y, KC_H)) {
    help_yazi();
  } else if (leader_sequence_two_keys(KC_Y, KC_Y)) {
    send_line("yazi");
  } else if (leader_sequence_two_keys(KC_Y, KC_Z)) {
    send_line_os("yazi \"$(zoxide query -i)\"", "yazi (zoxide query -i)", NULL);
  } else if (leader_sequence_two_keys(KC_Y, KC_G)) {
    intent_yazi_git_root();
  } else if (leader_sequence_two_keys(KC_Y, KC_F)) {
    send_line_os(
        "yazi \"$(git ls-files 2>/dev/null || rg --files)\"",
        "yazi (git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files })",
        NULL);
  }

  // ── zellij + Hilfe ─────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_J, KC_H)) { // Hilfe
    help_zellij();
  }

  // Sessions
  else if (leader_sequence_two_keys(KC_J, KC_J)) { // attach -c main
    intent_zellij_attach_main();
  } else if (leader_sequence_two_keys(KC_J, KC_L)) { // list-sessions
    intent_zellij_list_sessions();
  } else if (leader_sequence_two_keys(KC_J, KC_A)) { // attach <name>
    intent_zellij_attach_prompt();
  } else if (leader_sequence_two_keys(KC_J, KC_K)) { // kill-session <name>
    intent_zellij_kill_session_prompt();
  } else if (leader_sequence_three_keys(KC_J, KC_S,
                                        KC_R)) { // rename-session <name>
    intent_zellij_rename_session_prompt();
  }

  // Tabs
  else if (leader_sequence_three_keys(KC_J, KC_T, KC_N)) { // new-tab
    intent_zellij_new_tab();
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_P)) { // prev tab
    intent_zellij_prev_tab();
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_X)) { // close-tab
    intent_zellij_close_tab();
  } else if (leader_sequence_three_keys(KC_J, KC_T,
                                        KC_R)) { // rename-tab <name>
    intent_zellij_rename_tab_prompt();
  } else if (leader_sequence_three_keys(
                 KC_J, KC_T, KC_L)) { // new-tab --layout <file> --name <name>
    intent_zellij_new_tab_layout_prompt();
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_1)) {
    intent_zellij_go_to_tab(1);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_2)) {
    intent_zellij_go_to_tab(2);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_3)) {
    intent_zellij_go_to_tab(3);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_4)) {
    intent_zellij_go_to_tab(4);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_5)) {
    intent_zellij_go_to_tab(5);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_6)) {
    intent_zellij_go_to_tab(6);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_7)) {
    intent_zellij_go_to_tab(7);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_8)) {
    intent_zellij_go_to_tab(8);
  } else if (leader_sequence_three_keys(KC_J, KC_T, KC_9)) {
    intent_zellij_go_to_tab(9);
  }

  // Panes (Splits / Floating / Close)
  else if (leader_sequence_three_keys(KC_J, KC_P,
                                      KC_H)) { // split vertikal (rechts)
    intent_zellij_new_pane_right();
  } else if (leader_sequence_three_keys(KC_J, KC_P,
                                        KC_V)) { // split horizontal (unten)
    intent_zellij_new_pane_down();
  } else if (leader_sequence_three_keys(KC_J, KC_P, KC_F)) { // floating pane
    intent_zellij_new_pane_floating();
  } else if (leader_sequence_three_keys(KC_J, KC_P, KC_X)) { // close-pane
    intent_zellij_close_pane();
  }

  // Fokus bewegen (Vim H L K J)
  else if (leader_sequence_three_keys(KC_J, KC_F, KC_H)) {
    intent_zellij_move_focus("left");
  } else if (leader_sequence_three_keys(KC_J, KC_F, KC_L)) {
    intent_zellij_move_focus("right");
  } else if (leader_sequence_three_keys(KC_J, KC_F, KC_K)) {
    intent_zellij_move_focus("up");
  } else if (leader_sequence_three_keys(KC_J, KC_F, KC_J)) {
    intent_zellij_move_focus("down");
  }

  // Resize (kleine Schritte)
  else if (leader_sequence_three_keys(KC_J, KC_R, KC_H)) {
    intent_zellij_resize("left", 2);
  } else if (leader_sequence_three_keys(KC_J, KC_R, KC_L)) {
    intent_zellij_resize("right", 2);
  } else if (leader_sequence_three_keys(KC_J, KC_R, KC_K)) {
    intent_zellij_resize("up", 1);
  } else if (leader_sequence_three_keys(KC_J, KC_R, KC_J)) {
    intent_zellij_resize("down", 1);
  }

  // Toggles
  else if (leader_sequence_three_keys(KC_J, KC_O, KC_F)) {
    intent_zellij_toggle_floating_all();
  } else if (leader_sequence_three_keys(KC_J, KC_O, KC_E)) {
    intent_zellij_toggle_embed_or_floating();
  } else if (leader_sequence_three_keys(KC_J, KC_O, KC_U)) {
    intent_zellij_toggle_fullscreen();
  } else if (leader_sequence_three_keys(KC_J, KC_O, KC_B)) {
    intent_zellij_toggle_frames();
  }
}
#endif /* LEADER_ENABLE */
