// leader/intents_fs.c (zoxide / yazi)
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

void intent_zoxide_fzf_cd(void) {
  send_line_os("cd \"$(zoxide query -l | fzf)\"",      // POSIX
               "Set-Location (zoxide query -l | fzf)", // PowerShell
               NULL);
}

void intent_yazi_git_root(void) {
  send_line_os("yazi \"$(git rev-parse --show-toplevel)\"",
               "yazi (git rev-parse --show-toplevel)", NULL);
}
