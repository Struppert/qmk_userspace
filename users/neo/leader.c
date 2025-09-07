// users/neo/leader_git.c
#include QMK_KEYBOARD_H

#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

// Für korrektes Senden auf DE-Tastaturlayout:
#define SENDSTRING_LANGUAGE de_DE
#include "sendstring_german.h"

static inline void send_and_enter(const char *s) {
  send_string(s);
  tap_code(KC_ENT);
}

#if defined(LEADER_ENABLE)
// ── Leader Sequenzen ───────────────────────────
void leader_start_user(void) {
  // optionales Feedback beim Start
}

void leader_end_user(void) {
  if (leader_sequence_two_keys(KC_A, KC_A)) {
    send_string("LEADER OK\n");
  }

  // git status / add / diff / staged-diff
  if (leader_sequence_two_keys(KC_G, KC_S)) {
    send_string("git status\n");
  } else if (leader_sequence_two_keys(KC_G, KC_A)) {
    send_string("git add -A\n");
  } else if (leader_sequence_two_keys(KC_G, KC_D)) {
    send_string("git diff\n");
  } else if (leader_sequence_three_keys(KC_G, KC_D, KC_S)) {
    send_string("git diff --staged\n");
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
    send_string("git push\n");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_1)) {
    send_string("git push --set-upstream origin HEAD\n");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_9)) {
    send_string("git push --tags\n");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_EXLM)) {
    send_string("git push --force-with-lease\n");
  }

  // logs
  else if (leader_sequence_two_keys(KC_G, KC_L)) {
    send_string("git log --oneline --graph --decorate -n 30\n");
  } else if (leader_sequence_three_keys(KC_G, KC_L, KC_A)) {
    send_string("git log --all --decorate --oneline --graph\n");
  }

  // branch helpers
  else if (leader_sequence_three_keys(KC_G, KC_B, KC_N)) {
    SEND_STRING("git switch -c ");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_S)) {
    send_string("git switch -\n");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_D)) {
    SEND_STRING("git branch -D ");
  }

  // rebase / tags
  else if (leader_sequence_two_keys(KC_G, KC_R)) {
    SEND_STRING("git rebase -i HEAD~");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_N)) {
    SEND_STRING("git tag -a v");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_P)) {
    send_string("git push --tags\n");
  }

  // ── fzf ───────────────────────────────────────────────────────────────
  // LEAD f f  → fzf starten (einfach)
  else if (leader_sequence_two_keys(KC_F, KC_F)) {
    send_and_enter("fzf");
  }
  // LEAD f e  → fzf-Auswahl im $EDITOR öffnen (eine Datei)
  else if (leader_sequence_two_keys(KC_F, KC_E)) {
    send_and_enter("fzf | xargs -r $EDITOR");
  }
  // LEAD f p  → fzf mit Vorschau (bat) – hübsche Vorschau rechts
  else if (leader_sequence_two_keys(KC_F, KC_P)) {
    send_and_enter(
        "fzf --preview 'bat --style=numbers --color=always {} | head -500'");
  }
  // LEAD g f f → git ls-files | fzf (nur getrackte Dateien)
  else if (leader_sequence_three_keys(KC_G, KC_F, KC_F)) {
    send_and_enter("git ls-files | fzf");
  }

  // ── ripgrep (rg) ──────────────────────────────────────────────────────
  // Basis: versteckte Dateien mitnehmen, .git ausschließen, Zeilennummern
  // LEAD r r  → rg <dein-suchbegriff> (Cursor in Anführungszeichen)
  else if (leader_sequence_two_keys(KC_R, KC_R)) {
    SEND_STRING("rg -n --hidden --glob '!.git' \"\"");
    tap_code(KC_LEFT); // Cursor zwischen die Anführungszeichen
  }
  // LEAD r s  → "smart case" (-S) aktivieren
  else if (leader_sequence_two_keys(KC_R, KC_S)) {
    SEND_STRING("rg -n -S --hidden --glob '!.git' \"\"");
    tap_code(KC_LEFT);
  }
  // LEAD r a  → in ALLEN Branch-Dateien suchen (git ls-files + rg)
  else if (leader_sequence_two_keys(KC_R, KC_A)) {
    SEND_STRING("git ls-files | rg -n --hidden --glob '!.git' \"\"");
    tap_code(KC_LEFT);
  }
  // LEAD r l  → rg + less -R (scrollbare Ausgabe)
  else if (leader_sequence_two_keys(KC_R, KC_L)) {
    SEND_STRING("rg -n --hidden --glob '!.git' \"\" | less -R");
    tap_code(KC_LEFT);
  }
  // LEAD r g r → nur in *.go suchen (Beispiel für Globs)
  else if (leader_sequence_three_keys(KC_R, KC_G, KC_R)) {
    SEND_STRING("rg -n --hidden --glob '!.git' -g '*.go' \"\"");
    tap_code(KC_LEFT);
  }
  // LEAD r b  → nur in *.rs (Rust) suchen (weiteres Beispiel)
  else if (leader_sequence_two_keys(KC_R, KC_B)) {
    SEND_STRING("rg -n --hidden --glob '!.git' -g '*.rs' \"\"");
    tap_code(KC_LEFT);
  }
}
#endif // LEADER_ENABLE
