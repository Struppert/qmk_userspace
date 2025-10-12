// leader/handlers_os_shell.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "os_shell.h"

void h_help_groups(void) { help_groups_overview(); }
void h_leader_ok(void) { send_line("LEADER OK"); }

void h_ctx_O(void) { help_os(); }
void h_os_win(void) { os_set(OS_WIN); }
void h_os_lin(void) { os_set(OS_LNX); }
void h_os_mac(void) { os_set(OS_MAC); }
void h_os_cycle(void) { os_cycle(); }
void h_os_print(void) { os_print(); }

void h_ctx_S(void) { leader_help_shell_group(); }
void h_shell_bash(void) { shell_set(SH_BASH, true); }
void h_shell_fish(void) { shell_set(SH_FISH, true); }
void h_shell_zsh(void) { shell_set(SH_ZSH, true); }
void h_shell_pwsh(void) { shell_set(SH_PWSH, true); }
void h_shell_cycle(void) { shell_cycle(true); }
void h_shell_print(void) { shell_print_to_app(); }
