// leader/os_shell.c
#include QMK_KEYBOARD_H
#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

#include "os_shell.h"
#include <stdio.h>

// German send_string LUTs now live in users/neo/sendstring_de.c (compiled
// for every board, unlike this file) - see that file for why.

// #define SHELL_DISPATCH_USE_PWSH7

static void ps_escape_and_send(const char *s) {
  char buf[256];
  size_t j = 0;
  for (const char *p = s; *p && j + 2 < sizeof(buf); ++p) {
    if (*p == '\'') {
      buf[j++] = '\'';
      buf[j++] = '\'';
    } else {
      buf[j++] = *p;
    }
  }
  buf[j] = '\0';
  send_string(buf);
}

void shell_run_cmd(const char *cmd) {
  if (shell_current() == SH_PWSH) {
#ifdef SHELL_DISPATCH_USE_PWSH7
    SEND_STRING("pwsh -NoLogo -NoProfile -NonInteractive -ExecutionPolicy "
                "Bypass -Command ' & { ");
#else
    SEND_STRING("powershell -NoLogo -NoProfile -NonInteractive "
                "-ExecutionPolicy Bypass -Command ' & { ");
#endif
    ps_escape_and_send(cmd);
    SEND_STRING(" } '" SS_TAP(X_ENTER));
  } else {
    send_string(cmd);
    tap_code(KC_ENTER);
  }
}

void send_line(const char *s) { shell_run_cmd(s); }

void send_line_os(const char *posix_cmd, const char *pwsh_cmd,
                  const char *mac_cmd_opt) {
  const char *cmd = posix_cmd;
  if (is_win())
    cmd = pwsh_cmd ? pwsh_cmd : posix_cmd;
  else if (is_mac() && mac_cmd_opt && *mac_cmd_opt)
    cmd = mac_cmd_opt;
  shell_run_cmd(cmd);
}

void type_quotes_and_place_cursor(void) {
  send_string("\"\"");
  tap_code(KC_LEFT);
}
void send_cmd_with_empty_quotes(const char *prefix) {
  send_string(prefix);
  type_quotes_and_place_cursor();
}

// Shell names
static inline const char *shell_name_short(shell_t sh) {
  switch (sh) {
  case SH_FISH:
    return "fish";
  case SH_ZSH:
    return "zsh";
  case SH_PWSH:
    return "pwsh";
  default:
    return "bash";
  }
}
static inline const char *shell_name_long(shell_t sh) {
  switch (sh) {
  case SH_FISH:
    return "fish (friendly interactive shell)";
  case SH_ZSH:
    return "zsh (Z shell)";
  case SH_PWSH:
    return "PowerShell";
  default:
    return "bash (Bourne Again SH)";
  }
}

void shell_print_to_app(void) {
  send_string(shell_name_short(shell_current()));
  SEND_STRING(SS_TAP(X_ENTER));
}

void leader_help_shell_group(void) {
  SEND_STRING("Leader S (Shell)\n"
              "  S B  -> Shell=bash\n"
              "  S F  -> Shell=fish\n"
              "  S Z  -> Shell=zsh\n"
              "  S W  -> Shell=pwsh (PowerShell)\n"
              "  S C  -> Shell cycle (bash->fish->zsh->pwsh)\n"
              "  S P  -> print current shell\n"
              "  S H  -> this help\n");
  SEND_STRING("  Current: ");
  send_string(shell_name_long(shell_current()));
  SEND_STRING(SS_TAP(X_ENTER));
}
