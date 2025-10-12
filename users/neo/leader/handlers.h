// leader/handlers.h
#pragma once
#include QMK_KEYBOARD_H
#include "help.h"
#include "intents.h"
#include "os_shell.h"

#ifdef __cplusplus
extern "C" {
#endif

// Global
void h_leader_ok(void);
void h_help_groups(void);

// OS
void h_os_help(void);
void h_os_win(void);
void h_os_lin(void);
void h_os_mac(void);
void h_os_cycle(void);
void h_os_print(void);

// Shell
void h_shell_help(void);
void h_shell_bash(void);
void h_shell_fish(void);
void h_shell_zsh(void);
void h_shell_pwsh(void);
void h_shell_cycle(void);
void h_shell_print(void);

// WezTerm tabs
void h_tabs_help(void);
void h_wt_open(void);
void h_wt_newtab(void);
void h_wt_close_tab(void);
void h_wt_tab1(void);
void h_wt_tab2(void);
void h_wt_tab3(void);
void h_wt_tab4(void);
void h_wt_tab5(void);
void h_wt_tab6(void);
void h_wt_tab7(void);
void h_wt_tab8(void);
void h_wt_tab9(void);

// WezTerm panes
void h_panes_help(void);
void h_wt_split_h(void);
void h_wt_split_v(void);
void h_wt_pane_kill(void);
void h_wt_pane_next(void);
void h_wt_pane_prev(void);

// Git
void h_git_help(void);
void h_git_status(void);
void h_git_add_all(void);
void h_git_diff(void);
void h_git_diff_staged(void);
void h_git_commit_empty(void);
void h_git_commit_fix(void);
void h_git_commit_feat(void);
void h_git_commit_refactor(void);
void h_git_commit_test(void);
void h_git_commit_docs(void);
void h_git_push(void);
void h_git_push_upstream(void);
void h_git_push_tags(void);
void h_git_push_force_lease(void);
void h_git_log_short(void);
void h_git_log_all(void);
void h_git_branch_new(void);
void h_git_branch_back(void);
void h_git_branch_del(void);
void h_git_rebase_i(void);
void h_git_tag_annot(void);
void h_git_tags_push(void);
void h_git_grep_base(void);
void h_git_grep_i(void);
void h_git_grep_w(void);
void h_git_grep_l(void);
void h_gitgrep_fzf(void);
void h_git_grep_paths(void);
void h_git_pickaxe_S(void);
void h_git_pickaxe_G(void);

// fzf
void h_fzf_help(void);
void h_fzf_run(void);
void h_fzf_edit_sel(void);
void h_fzf_preview(void);
void h_git_ls_files_fzf(void);
void h_fzf_files_preview_open(void);
void h_fzf_files_multi_open(void);
void h_fzf_dirs_cd(void);

// rg
void h_rg_help(void);
void h_rg_base(void);
void h_rg_smart(void);
void h_rg_gitfiles(void);
void h_rg_less(void);
void h_rg_go_glob(void);
void h_rg_rs_glob(void);
void h_rg_fzf_edit(void);
void h_rg_ctx_C3(void);
void h_rg_ctx_B3(void);
void h_rg_ctx_A3(void);
void h_rg_multiline(void);
void h_rg_types_cpp(void);
void h_rg_types_rust(void);
void h_rg_types_go(void);
void h_rg_fzf_preview(void);

// zoxide
void h_z_help(void);
void h_z_z(void);
void h_z_i(void);
void h_z_add(void);
void h_z_list(void);
void h_z_top(void);
void h_z_remove(void);
void h_z_fzf_cd(void);
void h_z_nvim_z(void);

// yazi
void h_y_help(void);
void h_y_y(void);
void h_y_z(void);
void h_y_git_root(void);
void h_y_files_list(void);

// zellij
void h_j_help(void);
void h_j_attach_main(void);
void h_j_list(void);
void h_j_attach_prompt(void);
void h_j_kill_prompt(void);
void h_j_rename_prompt(void);
void h_j_tab_new(void);
void h_j_tab_prev(void);
void h_j_tab_close(void);
void h_j_tab_rename_prompt(void);
void h_j_tab_layout_prompt(void);
void h_j_tab1(void);
void h_j_tab2(void);
void h_j_tab3(void);
void h_j_tab4(void);
void h_j_tab5(void);
void h_j_tab6(void);
void h_j_tab7(void);
void h_j_tab8(void);
void h_j_tab9(void);
void h_j_pane_right(void);
void h_j_pane_down(void);
void h_j_pane_float(void);
void h_j_pane_close(void);
void h_j_focus_left(void);
void h_j_focus_right(void);
void h_j_focus_up(void);
void h_j_focus_down(void);
void h_j_resize_left(void);
void h_j_resize_right(void);
void h_j_resize_up(void);
void h_j_resize_down(void);
void h_j_toggle_floating_all(void);
void h_j_toggle_embed(void);
void h_j_toggle_full(void);
void h_j_toggle_frames(void);

// Context help wrappers
void h_ctx_O(void);
void h_ctx_T(void);
void h_ctx_P(void);
void h_ctx_G(void);
void h_ctx_F(void);
void h_ctx_R(void);
void h_ctx_Z(void);
void h_ctx_Y(void);
void h_ctx_J(void);

// Optional dev helpers (unused by default)
void h_dev_env(void) LEADER_MAYBE_UNUSED;
void h_build_run_dbg(void) LEADER_MAYBE_UNUSED;

// sed
void h_s_help(void);
void h_sed_help(void);
void h_sed_replace(void);
void h_sed_filter(void);
void h_sed_echo(void);
void h_sed_fzf(void);

#ifdef __cplusplus
}
#endif
