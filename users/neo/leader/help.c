// leader/help.c (trie-powered)
#include QMK_KEYBOARD_H
#include "help.h"
#include "os_shell.h"
#include "trie.h"
#include <stdio.h>
#include <string.h>

void help_block_begin(void) {
  if (is_win())
    SEND_STRING("$LEAD_HELP = @'\n");
  else
    SEND_STRING("cat <<'LEADHELP'\n");
}
void help_block_line(const char *s) {
  send_string(s);
  send_string("\n");
}
void help_block_end(void) {
  if (is_win())
    SEND_STRING("'@\nWrite-Host $LEAD_HELP\n");
  else
    SEND_STRING("LEADHELP\n");
}
void help_block_hr(void) {
  help_block_line("────────────────────────────────────────");
}

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
  help_block_line("#Allgemein/Test: LEAD A A → \"LEADER OK\"");
}

// minimal group heads (Details kommen aus kontextsensitiver Hilfe)
void help_os(void) {
  help_block_hr();
  help_block_line("#Hilfe: OS (LEAD O …)");
  help_block_hr();
}
void help_tabs(void) {
  help_block_hr();
  help_block_line("#Hilfe: WezTerm Tabs (LEAD T …)");
  help_block_hr();
}
void help_panes(void) {
  help_block_hr();
  help_block_line("#Hilfe: WezTerm Panes (LEAD P …)");
  help_block_hr();
}
void help_git(void) {
  help_block_hr();
  help_block_line("#Hilfe: Git (LEAD G …)");
  help_block_hr();
}
void help_fzf(void) {
  help_block_hr();
  help_block_line("#Hilfe: fzf (LEAD F …)");
  help_block_hr();
}
void help_rg(void) {
  help_block_hr();
  help_block_line("#Hilfe: ripgrep (LEAD R …)");
  help_block_hr();
}
void help_zoxide(void) {
  help_block_hr();
  help_block_line("#Hilfe: zoxide (LEAD Z …)");
  help_block_hr();
}
void help_yazi(void) {
  help_block_hr();
  help_block_line("#Hilfe: yazi (LEAD Y …)");
  help_block_hr();
}
void help_zellij(void) {
  help_block_hr();
  help_block_line("#Hilfe: zellij (LEAD J …)");
  help_block_hr();
}

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

// --- contextual help powered by the real trie ---
static const char *keycode_label(uint16_t kc) {
  static char buf[8];
  if (kc >= KC_A && kc <= KC_Z) {
    buf[0] = 'A' + (kc - KC_A);
    buf[1] = '\0';
    return buf;
  }
  if (kc >= KC_1 && kc <= KC_9) {
    buf[0] = '1' + (kc - KC_1);
    buf[1] = '\0';
    return buf;
  }
  if (kc == KC_0) {
    buf[0] = '0';
    buf[1] = '\0';
    return buf;
  }
  if (kc == KC_EXLM)
    return "!";
  snprintf(buf, sizeof(buf), "0x%X", kc);
  return buf;
}

static void print_context_header(const uint16_t *prefix, uint8_t plen) {
  help_block_hr();
  send_string("#Kontext-Hilfe: ");
  for (uint8_t i = 0; i < plen; i++) {
    send_string(keycode_label(prefix[i]));
    if (i + 1 < plen)
      send_string(" ");
  }
  send_string("\n");
  help_block_hr();
}

void leader_print_help_for_prefix(const uint16_t *prefix, uint8_t plen) {
  help_block_begin();
  print_context_header(prefix, plen);

  bool has_exact = false;
  uint16_t nexts[16];
  size_t n = leader_trie_children(prefix, plen, nexts, 16, &has_exact);

  if (has_exact) {
    const LeaderEntry *e = leader_trie_find_exact(prefix, plen);
    if (e && e->help) {
      help_block_line("<ENTER> -> ");
      help_block_line(e->help);
    }
  }

  for (size_t i = 0; i < n; i++) {
    uint16_t seq[8];
    for (uint8_t j = 0; j < plen; j++)
      seq[j] = prefix[j];
    seq[plen] = nexts[i];
    const LeaderEntry *exact = leader_trie_find_exact(seq, plen + 1);
    const char *label = keycode_label(nexts[i]);
    char line[160];
    snprintf(line, sizeof(line), "%s -> %s", label,
             exact && exact->help ? exact->help : "(weiter…)");
    help_block_line(line);
  }

  if (n == 0 && !has_exact) {
    help_block_line("(keine Fortsetzungen)");
  }

  help_block_end();
}
