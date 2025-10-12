// leader/intents.h
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include QMK_KEYBOARD_H
#include "os_shell.h"

#ifdef __cplusplus
extern "C" {
#endif

// —— WezTerm ——
void intent_open_wezterm(void);
void intent_split_pane_h(void);
void intent_split_pane_v(void);
void intent_kill_pane(void);
void intent_next_pane_right(void);
void intent_prev_pane_left(void);
void intent_new_tab_here(void);
void intent_close_tab(void);
void intent_activate_tab(uint8_t n);

// —— Dev / Build ——
void intent_dev_env(void);
void intent_build_and_run_debug(void);

// —— ripgrep / fzf combo ——
void intent_ripgrep_project(void);
void intent_rg_fzf_open_editor_line(void);
void intent_rg_fzf_preview_open_editor_line(void);
void intent_rg_context_around3(void);
void intent_rg_context_before3(void);
void intent_rg_context_after3(void);
void intent_rg_multiline(void);
void intent_rg_types_cpp(void);
void intent_rg_types_rust(void);
void intent_rg_types_go(void);

// —— fzf helpers ——
void intent_fzf_files_preview_open(void);
void intent_fzf_files_multi_open(void);
void intent_fzf_dirs_cd(void);

// —— Git grep ——
void intent_gitgrep_fzf_open_editor_line(void);

// —— zoxide / yazi ——
void intent_zoxide_fzf_cd(void);
void intent_yazi_git_root(void);

// —— zellij ——
void intent_zellij_attach_main(void);
void intent_zellij_list_sessions(void);
void intent_zellij_attach_prompt(void);
void intent_zellij_kill_session_prompt(void);
void intent_zellij_rename_session_prompt(void);
void intent_zellij_new_tab(void);
void intent_zellij_prev_tab(void);
void intent_zellij_close_tab(void);
void intent_zellij_go_to_tab(uint8_t n);
void intent_zellij_rename_tab_prompt(void);
void intent_zellij_new_tab_layout_prompt(void);
void intent_zellij_new_pane_right(void);
void intent_zellij_new_pane_down(void);
void intent_zellij_new_pane_floating(void);
void intent_zellij_close_pane(void);
void intent_zellij_move_focus(const char *dir);
void intent_zellij_resize(const char *dir, int amt);
void intent_zellij_toggle_floating_all(void);
void intent_zellij_toggle_embed_or_floating(void);
void intent_zellij_toggle_fullscreen(void);
void intent_zellij_toggle_frames(void);

// —— sed——
void intent_sed_help(void);
void intent_sed_replace_template(void);
void intent_sed_filter_template(void);
void intent_sed_echo_test(void);
void intent_sed_fzf_files(void);

#ifdef __cplusplus
}
#endif
