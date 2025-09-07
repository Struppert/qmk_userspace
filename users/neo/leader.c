// users/neo/leader.c
#include QMK_KEYBOARD_H

#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

// ──────────────────────────────────────────────────────────────────────────────
// SendString → korrekt für DE-Layout (nur hier einbinden, sonst Linker-Fehler!)
#define SENDSTRING_LANGUAGE de_DE
#include "sendstring_german.h"

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
  // den Cursor
  send_string(prefix);
  type_quotes_and_place_cursor();
}
static inline void send_line(const char *s) { send_and_enter(s); }

// ──────────────────────────────────────────────────────────────────────────────
// Leader Cheat Sheet (kompakt & gruppiert)
// Hinweis:
//  • [POSIX] = bash/zsh/fish etc.   • [PowerShell] = Windows PowerShell
//  • Befehle mit "" setzen den Cursor automatisch zwischen die
//  Anführungszeichen • Viele Befehle enden auf "\n" (ENTER) und laufen sofort
//  los
// ──────────────────────────────────────────────────────────────────────────────
//
// Allgemein/Test
//  LEAD A A         → "LEADER OK"
//
// ── Git: Status/Diff/Commit/Push/Log/Branch/Rebase/Tags ──────────────────────
//  LEAD G S         → git status
//  LEAD G A         → git add -A
//  LEAD G D         → git diff
//  LEAD G D S       → git diff --staged
//
//  LEAD G C         → git commit -m ""                      (Cursor in "")
//  LEAD G C F       → git commit -m "fix: "                 (Cursor am
//  Zeilenende) LEAD G C B       → git commit -m "feat: " LEAD G C R       → git
//  commit -m "refactor: " LEAD G C T       → git commit -m "test: " LEAD G C D
//  → git commit -m "docs: "
//
//  LEAD G P         → git push
//  LEAD G P 1       → git push --set-upstream origin HEAD
//  LEAD G P 9       → git push --tags
//  LEAD G P !       → git push --force-with-lease
//
//  LEAD G L         → git log --oneline --graph --decorate -n 30
//  LEAD G L A       → git log --all --decorate --oneline --graph
//
//  LEAD G B N       → git switch -c                          (neuen Branch
//  anlegen) LEAD G B S       → git switch -                           (zurück
//  zum vorherigen) LEAD G B D       → git branch -D (Branch löschen; Namen
//  tippen)
//
//  LEAD G R         → git rebase -i HEAD~                    (Interaktiv; Zahl
//  eintippen) LEAD G T N       → git tag -a v (Version eintippen) LEAD G T P →
//  git push --tags
//
// ── Git Grep (Arbeitsbaum durchsuchen)
// ────────────────────────────────────────
//  LEAD G G         → git grep -n -I -e ""                   (Basis;
//  nummeriert, no-binary) LEAD G G I       → git grep -n -I -i -e ""
//  (ignore-case) LEAD G G W       → git grep -n -I -w -e "" (Wortgrenze) LEAD G
//  G L       → git grep -I -l -e ""                   (nur Dateinamen) LEAD G G
//  P       → git grep -n -I -e "" --                (Pathspec-Helfer; z. B.
//  src/)
//
//  LEAD G G F       → git grep … | fzf → ${EDITOR} +Zeile    [POSIX]
//                      (öffnet Auswahlzeile direkt im Editor)
//  LEAD G G W       → git grep … | fzf → nvim +Zeile         [PowerShell]
//
// ── „Pickaxe“ (Historie nach Content-Änderungen durchsuchen) ─────────────────
//  LEAD G P S       → git log -S "" --patch --stat           (String in
//  Patches) LEAD G P G       → git log -G "" --patch --stat           (Regex in
//  Patches)
//
// ── fzf
// ───────────────────────────────────────────────────────────────────────
//  LEAD F F         → fzf
//  LEAD F E         → fzf | xargs -r $EDITOR                 [POSIX]
//  LEAD F P         → fzf --preview 'bat …'                  (schöne Vorschau)
//  LEAD G F F       → git ls-files | fzf                     (nur getrackte
//  Dateien)
//
// ── ripgrep (rg)
// ──────────────────────────────────────────────────────────────
//  LEAD R R         → rg -n --hidden --glob '!.git' ""       (Basis)
//  LEAD R S         → rg -n -S --hidden --glob '!.git' ""    (smart-case)
//  LEAD R A         → git ls-files | rg -n --hidden … ""     (nur getrackte)
//  LEAD R L         → rg -n --hidden … "" | less -R          (scrollbar)
//
//  LEAD R G R       → rg -n --hidden -g '*.go' ""            (nur *.go)
//  LEAD R B         → rg -n --hidden -g '*.rs' ""            (nur *.rs)
//
//  LEAD R F         → rg … "" | fzf → ${EDITOR} +Zeile       [POSIX]
//  LEAD R F W       → rg … "" | fzf → nvim +Zeile            [PowerShell]
//
// ── zoxide
// ────────────────────────────────────────────────────────────────────
//  LEAD Z Z         → z ""                                   (Cursor in "")
//  LEAD Z I         → zi                                     (interaktiv/fzf)
//  LEAD Z A         → zoxide add
//  LEAD Z L         → zoxide query -l ""                     (Liste; Cursor in
//  "") LEAD Z T         → zoxide query -t                        (Top-Treffer)
//  LEAD Z R         → zoxide remove ""                       (Eintrag löschen)
//
//  LEAD Z F         → cd "$(zoxide query -l | fzf)"          [POSIX]
//  LEAD Z F W       → Set-Location (zoxide query -l | fzf)   [PowerShell]
//  LEAD Z N         → nvim "$(zoxide query -i)"              [POSIX]
//  LEAD Z N W       → nvim (zoxide query -i)                 [PowerShell]
//
// ── yazi (Dateimanager)
// ───────────────────────────────────────────────────────
//  LEAD Y Y         → yazi
//  LEAD Y Z         → yazi "$(zoxide query -i)"              [POSIX]
//  LEAD Y Z W       → yazi (zoxide query -i)                 [PowerShell]
//  LEAD Y G         → yazi $(git rev-parse --show-toplevel)  [POSIX]
//  LEAD Y G W       → yazi (git rev-parse --show-toplevel)   [PowerShell]
// ──────────────────────────────────────────────────────────────────────────────

#if defined(LEADER_ENABLE)
void leader_start_user(void) {
  // optional: Feedback beim Start (RGB blitzen etc.)
}

void leader_end_user(void) {

  // ── Sanity / Test
  // ───────────────────────────────────────────────────────────
  if (leader_sequence_two_keys(KC_A, KC_A)) {
    send_line("LEADER OK");
  }

  // ── Git ────────────────────────────────────────────────────────────────────
  // status / add / diff / staged-diff
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

  // ── git grep (Arbeitsbaum) ────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_G, KC_G)) {
    // Basis: nummeriert, keine Binärdateien, Pattern editierbar
    send_cmd_with_empty_quotes("git grep -n -I -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_I)) {
    // ignore-case
    send_cmd_with_empty_quotes("git grep -n -I -i -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_W)) {
    // ganzes Wort
    send_cmd_with_empty_quotes("git grep -n -I -w -e ");
  } else if (leader_sequence_three_keys(KC_G, KC_G, KC_L)) {
    // nur Dateinamen (-l)
    send_cmd_with_empty_quotes("git grep -I -l -e ");
  }

  // git grep → fzf → Datei im Editor an Zeile öffnen (POSIX)
  else if (leader_sequence_three_keys(KC_G, KC_G, KC_F)) {
    SEND_STRING("git grep -n -I -e \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    // Cursor zurück in die Such-Quotes
    tap_code(KC_LEFT);
  }

  // git grep → fzf → Editor (PowerShell-Variante) — Suffix W
  else if (leader_sequence_three_keys(KC_G, KC_G, KC_W)) {
    SEND_STRING("git grep -n -I -e \"\" | fzf | "
                "ForEach-Object { $p = $_ -split \":\"; "
                "if ($p.Length -ge 2) { "
                "  nvim \"+$($p[1])\" $p[0] "
                "} }\n");
    tap_code(KC_LEFT);
  }

  // Optional: Pathspec schnell anhängen (du tippst das Pattern, dann ' -- '
  // folgt)
  else if (leader_sequence_three_keys(KC_G, KC_G, KC_P)) {
    // Beispiel: git grep -n -I -e "" -- src/
    SEND_STRING("git grep -n -I -e \"\" -- ");
    tap_code(KC_LEFT);
  }

  // ── „Pickaxe“ (History-Suche) ─────────────────────────────────────────────
  // Sucht Änderungen, die ein bestimmtes String-/Regex-Vorkommen
  // ein-/ausführen.
  else if (leader_sequence_three_keys(KC_G, KC_P, KC_S)) {
    // -S: exakt nach String in Patches suchen
    send_cmd_with_empty_quotes("git log -S ");
    send_string(" --patch --stat");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_G)) {
    // -G: Regex in Patches
    send_cmd_with_empty_quotes("git log -G ");
    send_string(" --patch --stat");
  }

  // ── fzf ────────────────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_F, KC_F)) {
    send_line("fzf");
  } else if (leader_sequence_two_keys(KC_F, KC_E)) {
    send_line("fzf | xargs -r $EDITOR");
  } else if (leader_sequence_two_keys(KC_F, KC_P)) {
    send_line(
        "fzf --preview 'bat --style=numbers --color=always {} | head -500'");
  } else if (leader_sequence_three_keys(KC_G, KC_F, KC_F)) {
    send_line("git ls-files | fzf");
  }

  // ── ripgrep / suchen ───────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_R, KC_R)) {
    // Standard-Suche mit Cursor in Anführungszeichen
    send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' ");
  } else if (leader_sequence_two_keys(KC_R, KC_S)) {
    // smart case
    send_cmd_with_empty_quotes("rg -n -S --hidden --glob '!.git' ");
  } else if (leader_sequence_two_keys(KC_R, KC_A)) {
    // nur getrackte Dateien
    send_cmd_with_empty_quotes("git ls-files | rg -n --hidden --glob '!.git' ");
  } else if (leader_sequence_two_keys(KC_R, KC_L)) {
    // scrollbare Ausgabe
    SEND_STRING("rg -n --hidden --glob '!.git' \"\" | less -R");
    tap_code(KC_LEFT);
  } else if (leader_sequence_three_keys(KC_R, KC_G, KC_R)) {
    // Beispiel: nur *.go
    send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.go' ");
  } else if (leader_sequence_two_keys(KC_R, KC_B)) {
    // Beispiel: nur *.rs
    send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.rs' ");
  }

  // rg → fzf → nvim (+ Zeile), POSIX
  else if (leader_sequence_two_keys(KC_R, KC_F)) {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    // Cursor zurück in die rg-Query („“)
    tap_code(KC_LEFT);
  }

  // rg → fzf → nvim (+ Zeile), PowerShell
  else if (leader_sequence_three_keys(KC_R, KC_F, KC_W)) {
    SEND_STRING("rg --line-number --no-heading -S \"\" | fzf | "
                "ForEach-Object { $p = $_ -split \":\"; "
                "if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }\n");
    tap_code(KC_LEFT);
  }

  // ── zoxide ────────────────────────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Z, KC_Z)) {
    // z "<query>"
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
  }

  // zoxide + fzf: POSIX
  else if (leader_sequence_two_keys(KC_Z, KC_F)) {
    send_line("cd \"$(zoxide query -l | fzf)\"");
  }
  // zoxide + fzf: PowerShell (Suffix W)
  else if (leader_sequence_three_keys(KC_Z, KC_F, KC_W)) {
    send_line("Set-Location (zoxide query -l | fzf)");
  }
  // zoxide → nvim: POSIX / PowerShell
  else if (leader_sequence_two_keys(KC_Z, KC_N)) {
    send_line("nvim \"$(zoxide query -i)\"");
  } else if (leader_sequence_three_keys(KC_Z, KC_N, KC_W)) {
    send_line("nvim (zoxide query -i)");
  }

  // ── yazi (TUI Dateimanager) ───────────────────────────────────────────────
  else if (leader_sequence_two_keys(KC_Y, KC_Y)) {
    // Direkt starten im aktuellen Verzeichnis
    send_line("yazi");
  } else if (leader_sequence_two_keys(KC_Y, KC_Z)) {
    // Ziel interaktiv via zoxide (POSIX)
    send_line("yazi \"$(zoxide query -i)\"");
  } else if (leader_sequence_three_keys(KC_Y, KC_Z, KC_W)) {
    // Ziel via zoxide (PowerShell)
    send_line("yazi (zoxide query -i)");
  } else if (leader_sequence_two_keys(KC_Y, KC_G)) {
    // Im Git-Root starten (POSIX)
    send_line("yazi \"$(git rev-parse --show-toplevel)\"");
  } else if (leader_sequence_three_keys(KC_Y, KC_G, KC_W)) {
    // Im Git-Root starten (PowerShell)
    send_line("yazi (git rev-parse --show-toplevel)");
  } else if (leader_sequence_two_keys(KC_Y, KC_F)) {
    // Datei via fzf wählen und yazi dort öffnen (POSIX)
    send_line("yazi \"$(git ls-files 2>/dev/null || rg --files)\""); // fallback
  }
  // Hinweis: yazi akzeptiert Datei- oder Ordnerpfad; bei Datei springt es
  // typischerweise ins Elternverzeichnis und selektiert sie (je nach Version).
}
#endif /* LEADER_ENABLE */
