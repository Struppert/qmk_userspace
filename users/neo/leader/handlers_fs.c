// leader/handlers_fs.c (zoxide / yazi)
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

void h_z_help(void) { help_zoxide(); }
void h_z_z(void) { send_cmd_with_empty_quotes("z "); }
void h_z_i(void) { send_line("zi"); }
void h_z_add(void) { send_line("zoxide add"); }
void h_z_list(void) { send_cmd_with_empty_quotes("zoxide query -l "); }
void h_z_top(void) { send_line("zoxide query -t"); }
void h_z_remove(void) { send_cmd_with_empty_quotes("zoxide remove "); }
void h_z_fzf_cd(void) { intent_zoxide_fzf_cd(); }
void h_z_nvim_z(void) {
  send_line_os("nvim \"$(zoxide query -i)\"", "nvim (zoxide query -i)", NULL);
}

void h_y_help(void) { help_yazi(); }
void h_y_y(void) { send_line("yazi"); }
void h_y_z(void) {
  send_line_os("yazi \"$(zoxide query -i)\"", "yazi (zoxide query -i)", NULL);
}
void h_y_git_root(void) { intent_yazi_git_root(); }
void h_y_files_list(void) {
  send_line_os(
      "yazi \"$(git ls-files 2>/dev/null || rg --files)\"",
      "yazi (git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files })",
      NULL);
}
