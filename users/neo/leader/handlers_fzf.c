// leader/handlers_fzf.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

void h_fzf_help(void) { help_fzf(); }
void h_fzf_run(void) { send_line("fzf"); }
void h_fzf_edit_sel(void) {
  send_line_os("fzf | xargs -r ${EDITOR:-nvim}",
               "fzf | ForEach-Object { nvim $_ }", NULL);
}
void h_fzf_preview(void) {
  send_line(
      "fzf --preview 'bat --style=numbers --color=always {} | head -500'");
}
void h_git_ls_files_fzf(void) { send_line("git ls-files | fzf"); }
void h_fzf_files_preview_open(void) { intent_fzf_files_preview_open(); }
void h_fzf_files_multi_open(void) { intent_fzf_files_multi_open(); }
void h_fzf_dirs_cd(void) { intent_fzf_dirs_cd(); }
