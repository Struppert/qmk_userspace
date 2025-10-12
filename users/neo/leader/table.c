// leader/table.c
#include QMK_KEYBOARD_H
#include "core.h"
#include "handlers.h"

// clang-format off
const LeaderEntry leader_table[] = {
  // Global
  LENTRY2(KC_H, KC_H, h_help_groups, LFLAG_NONE, "Gruppenübersicht"),
  LENTRY2(KC_A, KC_A, h_leader_ok,   LFLAG_NONE, "Test: LEADER OK"),

  // OS group (+ kontextsensitive Hilfe auf H)
  LENTRY2(KC_O, KC_H, h_ctx_O,     LFLAG_NONE, "Hilfe OS"),
  LENTRY2(KC_O, KC_W, h_os_win,    LFLAG_NONE, "OS = Windows"),
  LENTRY2(KC_O, KC_L, h_os_lin,    LFLAG_NONE, "OS = Linux"),
  LENTRY2(KC_O, KC_M, h_os_mac,    LFLAG_NONE, "OS = macOS"),
  LENTRY2(KC_O, KC_C, h_os_cycle,  LFLAG_NONE, "OS zyklisch"),
  LENTRY2(KC_O, KC_P, h_os_print,  LFLAG_NONE, "OS in App tippen"),

  // WezTerm Tabs (+ H)
  LENTRY2(KC_T, KC_H, h_ctx_T,        LFLAG_NONE, "Hilfe Tabs"),
  LENTRY1(KC_T,      h_wt_open,       LFLAG_NONE, "WezTerm öffnen"),
  LENTRY2(KC_T, KC_N, h_wt_newtab,    LFLAG_NONE, "Neuer Tab"),
  LENTRY2(KC_T, KC_C, h_wt_close_tab, LFLAG_NONE, "Tab schließen"),
  LENTRY2(KC_T, KC_1, h_wt_tab1,      LFLAG_NONE, "Tab 1"),
  LENTRY2(KC_T, KC_2, h_wt_tab2,      LFLAG_NONE, "Tab 2"),
  LENTRY2(KC_T, KC_3, h_wt_tab3,      LFLAG_NONE, "Tab 3"),
  LENTRY2(KC_T, KC_4, h_wt_tab4,      LFLAG_NONE, "Tab 4"),
  LENTRY2(KC_T, KC_5, h_wt_tab5,      LFLAG_NONE, "Tab 5"),
  LENTRY2(KC_T, KC_6, h_wt_tab6,      LFLAG_NONE, "Tab 6"),
  LENTRY2(KC_T, KC_7, h_wt_tab7,      LFLAG_NONE, "Tab 7"),
  LENTRY2(KC_T, KC_8, h_wt_tab8,      LFLAG_NONE, "Tab 8"),
  LENTRY2(KC_T, KC_9, h_wt_tab9,      LFLAG_NONE, "Tab 9"),

  // WezTerm Panes (+ H)
  LENTRY2(KC_P, KC_H, h_ctx_P,        LFLAG_NONE, "Hilfe Panes"),
  LENTRY2(KC_P, KC_S, h_wt_split_h,   LFLAG_NONE, "Split horizontal"),
  LENTRY2(KC_P, KC_V, h_wt_split_v,   LFLAG_NONE, "Split vertikal"),
  LENTRY2(KC_P, KC_X, h_wt_pane_kill, LFLAG_NONE, "Pane schließen"),
  LENTRY2(KC_P, KC_N, h_wt_pane_next, LFLAG_NONE, "Nächstes Pane"),
  LENTRY2(KC_P, KC_P, h_wt_pane_prev, LFLAG_NONE, "Vorheriges Pane"),

  // Git (+ H)
  LENTRY2(KC_G, KC_H, h_ctx_G,             LFLAG_NONE, "Hilfe Git"),
  LENTRY2(KC_G, KC_S, h_git_status,        LFLAG_NONE, "git status"),
  LENTRY2(KC_G, KC_A, h_git_add_all,       LFLAG_NONE, "git add -A"),
  LENTRY2(KC_G, KC_D, h_git_diff,          LFLAG_NONE, "git diff"),
  LENTRY3(KC_G, KC_D, KC_S, h_git_diff_staged, LFLAG_NONE, "git diff --staged"),
  LENTRY2(KC_G, KC_C, h_git_commit_empty,  LFLAG_NONE, "commit -m \"\""),
  LENTRY3(KC_G, KC_C, KC_F, h_git_commit_fix,      LFLAG_NONE, "commit fix:"),
  LENTRY3(KC_G, KC_C, KC_B, h_git_commit_feat,     LFLAG_NONE, "commit feat:"),
  LENTRY3(KC_G, KC_C, KC_R, h_git_commit_refactor, LFLAG_NONE, "commit refactor:"),
  LENTRY3(KC_G, KC_C, KC_T, h_git_commit_test,     LFLAG_NONE, "commit test:"),
  LENTRY3(KC_G, KC_C, KC_D, h_git_commit_docs,     LFLAG_NONE, "commit docs:"),
  LENTRY2(KC_G, KC_P, h_git_push,          LFLAG_NONE, "git push"),
  LENTRY3(KC_G, KC_P, KC_1, h_git_push_upstream,    LFLAG_NONE, "push --set-upstream"),
  LENTRY3(KC_G, KC_P, KC_9, h_git_push_tags,        LFLAG_NONE, "push --tags"),
  LENTRY3(KC_G, KC_P, KC_EXLM, h_git_push_force_lease, LFLAG_DANGEROUS, "push --force-with-lease"),
  LENTRY2(KC_G, KC_L, h_git_log_short,     LFLAG_NONE, "log kompakt"),
  LENTRY3(KC_G, KC_L, KC_A, h_git_log_all, LFLAG_NONE, "log all graph"),
  LENTRY3(KC_G, KC_B, KC_N, h_git_branch_new,   LFLAG_NONE, "switch -c <name>"),
  LENTRY3(KC_G, KC_B, KC_S, h_git_branch_back,  LFLAG_NONE, "switch -"),
  LENTRY3(KC_G, KC_B, KC_D, h_git_branch_del,   LFLAG_NONE, "branch -D <name>"),
  LENTRY2(KC_G, KC_R, h_git_rebase_i,       LFLAG_NONE, "rebase -i HEAD~"),
  LENTRY3(KC_G, KC_T, KC_N, h_git_tag_annot,    LFLAG_NONE, "tag -a v…"),
  LENTRY3(KC_G, KC_T, KC_P, h_git_tags_push,    LFLAG_NONE, "push --tags"),
  LENTRY2(KC_G, KC_G, h_git_grep_base,      LFLAG_NONE, "git grep -n -I -e …"),
  LENTRY3(KC_G, KC_G, KC_I, h_git_grep_i,   LFLAG_NONE, "git grep -i"),
  LENTRY3(KC_G, KC_G, KC_W, h_git_grep_w,   LFLAG_NONE, "git grep -w"),
  LENTRY3(KC_G, KC_G, KC_L, h_git_grep_l,   LFLAG_NONE, "git grep -l"),
  LENTRY3(KC_G, KC_G, KC_F, h_gitgrep_fzf,  LFLAG_NONE, "git grep | fzf | nvim"),
  LENTRY3(KC_G, KC_G, KC_P, h_git_grep_paths, LFLAG_NONE, "git grep … -- <paths>"),
  LENTRY3(KC_G, KC_P, KC_S, h_git_pickaxe_S, LFLAG_NONE, "log -S … --patch --stat"),
  LENTRY3(KC_G, KC_P, KC_G, h_git_pickaxe_G, LFLAG_NONE, "log -G … --patch --stat"),

  // fzf (+ H)
  LENTRY2(KC_F, KC_H, h_ctx_F,         LFLAG_NONE, "Hilfe fzf"),
  LENTRY2(KC_F, KC_F, h_fzf_run,       LFLAG_NONE, "fzf"),
  LENTRY2(KC_F, KC_E, h_fzf_edit_sel,  LFLAG_NONE, "Auswahl -> $EDITOR"),
  LENTRY2(KC_F, KC_P, h_fzf_preview,   LFLAG_NONE, "Preview mit bat"),
  LENTRY3(KC_G, KC_F, KC_F, h_git_ls_files_fzf, LFLAG_NONE, "git ls-files | fzf"),
  LENTRY2(KC_F, KC_O, h_fzf_files_preview_open, LFLAG_NONE, "Dateien mit Preview -> Editor"),
  LENTRY2(KC_F, KC_M, h_fzf_files_multi_open,   LFLAG_NONE, "Multi-Select -> Editor"),
  LENTRY2(KC_F, KC_D, h_fzf_dirs_cd,            LFLAG_NONE, "Verzeichnis -> cd"),

  // ripgrep (+ H)
  LENTRY2(KC_R, KC_H, h_ctx_R,         LFLAG_NONE, "Hilfe rg"),
  LENTRY2(KC_R, KC_R, h_rg_base,       LFLAG_NONE, "rg Basis"),
  LENTRY2(KC_R, KC_S, h_rg_smart,      LFLAG_NONE, "-S smart"),
  LENTRY2(KC_R, KC_A, h_rg_gitfiles,   LFLAG_NONE, "git ls-files | rg"),
  LENTRY2(KC_R, KC_L, h_rg_less,       LFLAG_NONE, "rg … | less -R"),
  LENTRY3(KC_R, KC_G, KC_R, h_rg_go_glob, LFLAG_NONE, "-g '*.go'"),
  LENTRY2(KC_R, KC_B, h_rg_rs_glob,    LFLAG_NONE, "-g '*.rs'"),
  LENTRY2(KC_R, KC_F, h_rg_fzf_edit,   LFLAG_NONE, "rg | fzf | nvim+Zeile"),
  LENTRY3(KC_R, KC_C, KC_A, h_rg_ctx_C3, LFLAG_NONE, "-C 3"),
  LENTRY3(KC_R, KC_C, KC_U, h_rg_ctx_B3, LFLAG_NONE, "-B 3"),
  LENTRY3(KC_R, KC_C, KC_D, h_rg_ctx_A3, LFLAG_NONE, "-A 3"),
  LENTRY2(KC_R, KC_M, h_rg_multiline,  LFLAG_NONE, "-U multiline"),
  LENTRY3(KC_R, KC_T, KC_C, h_rg_types_cpp,  LFLAG_NONE, "-t c/cpp/h/hpp"),
  LENTRY3(KC_R, KC_T, KC_R, h_rg_types_rust, LFLAG_NONE, "-t rust"),
  LENTRY3(KC_R, KC_T, KC_G, h_rg_types_go,   LFLAG_NONE, "-t go"),
  LENTRY2(KC_R, KC_F, h_rg_fzf_preview, LFLAG_NONE, "fzf preview + open"),

  // zoxide (+ H)
  LENTRY2(KC_Z, KC_H, h_ctx_Z,         LFLAG_NONE, "Hilfe zoxide"),
  LENTRY2(KC_Z, KC_Z, h_z_z,           LFLAG_NONE, "z \"…\""),
  LENTRY2(KC_Z, KC_I, h_z_i,           LFLAG_NONE, "zi"),
  LENTRY2(KC_Z, KC_A, h_z_add,         LFLAG_NONE, "zoxide add"),
  LENTRY2(KC_Z, KC_L, h_z_list,        LFLAG_NONE, "zoxide query -l"),
  LENTRY2(KC_Z, KC_T, h_z_top,         LFLAG_NONE, "zoxide query -t"),
  LENTRY2(KC_Z, KC_R, h_z_remove,      LFLAG_NONE, "zoxide remove"),
  LENTRY2(KC_Z, KC_F, h_z_fzf_cd,      LFLAG_NONE, "zoxide | fzf -> cd"),
  LENTRY2(KC_Z, KC_N, h_z_nvim_z,      LFLAG_NONE, "nvim $(zoxide query -i)"),

  // yazi (+ H)
  LENTRY2(KC_Y, KC_H, h_ctx_Y,         LFLAG_NONE, "Hilfe yazi"),
  LENTRY2(KC_Y, KC_Y, h_y_y,           LFLAG_NONE, "yazi"),
  LENTRY2(KC_Y, KC_Z, h_y_z,           LFLAG_NONE, "yazi $(zoxide)"),
  LENTRY2(KC_Y, KC_G, h_y_git_root,    LFLAG_NONE, "yazi Git-Root"),
  LENTRY2(KC_Y, KC_F, h_y_files_list,  LFLAG_NONE, "yazi mit Datei-/Pfadliste"),

  // zellij (+ H)
  LENTRY2(KC_J, KC_H, h_ctx_J,         LFLAG_NONE, "Hilfe zellij"),
  LENTRY2(KC_J, KC_J, h_j_attach_main, LFLAG_NONE, "attach -c main"),
  LENTRY2(KC_J, KC_L, h_j_list,        LFLAG_NONE, "list-sessions"),
  LENTRY2(KC_J, KC_A, h_j_attach_prompt, LFLAG_NONE, "attach <name>"),
  LENTRY2(KC_J, KC_K, h_j_kill_prompt,   LFLAG_NONE, "kill-session <name>"),
  LENTRY3(KC_J, KC_S, KC_R, h_j_rename_prompt, LFLAG_NONE, "rename-session <name>"),
  LENTRY3(KC_J, KC_T, KC_N, h_j_tab_new,   LFLAG_NONE, "new-tab"),
  LENTRY3(KC_J, KC_T, KC_P, h_j_tab_prev,  LFLAG_NONE, "prev tab"),
  LENTRY3(KC_J, KC_T, KC_X, h_j_tab_close, LFLAG_NONE, "close-tab"),
  LENTRY3(KC_J, KC_T, KC_R, h_j_tab_rename_prompt, LFLAG_NONE, "rename-tab <name>"),
  LENTRY3(KC_J, KC_T, KC_L, h_j_tab_layout_prompt, LFLAG_NONE, "new-tab --layout <file>"),
  LENTRY3(KC_J, KC_T, KC_1, h_j_tab1, LFLAG_NONE, "go-to-tab 1"),
  LENTRY3(KC_J, KC_T, KC_2, h_j_tab2, LFLAG_NONE, "go-to-tab 2"),
  LENTRY3(KC_J, KC_T, KC_3, h_j_tab3, LFLAG_NONE, "go-to-tab 3"),
  LENTRY3(KC_J, KC_T, KC_4, h_j_tab4, LFLAG_NONE, "go-to-tab 4"),
  LENTRY3(KC_J, KC_T, KC_5, h_j_tab5, LFLAG_NONE, "go-to-tab 5"),
  LENTRY3(KC_J, KC_T, KC_6, h_j_tab6, LFLAG_NONE, "go-to-tab 6"),
  LENTRY3(KC_J, KC_T, KC_7, h_j_tab7, LFLAG_NONE, "go-to-tab 7"),
  LENTRY3(KC_J, KC_T, KC_8, h_j_tab8, LFLAG_NONE, "go-to-tab 8"),
  LENTRY3(KC_J, KC_T, KC_9, h_j_tab9, LFLAG_NONE, "go-to-tab 9"),
  LENTRY3(KC_J, KC_P, KC_H, h_j_pane_right,  LFLAG_NONE, "split right"),
  LENTRY3(KC_J, KC_P, KC_V, h_j_pane_down,   LFLAG_NONE, "split down"),
  LENTRY3(KC_J, KC_P, KC_F, h_j_pane_float,  LFLAG_NONE, "floating pane"),
  LENTRY3(KC_J, KC_P, KC_X, h_j_pane_close,  LFLAG_NONE, "close pane"),
  LENTRY3(KC_J, KC_F, KC_H, h_j_focus_left,  LFLAG_NONE, "focus left"),
  LENTRY3(KC_J, KC_F, KC_L, h_j_focus_right, LFLAG_NONE, "focus right"),
  LENTRY3(KC_J, KC_F, KC_K, h_j_focus_up,    LFLAG_NONE, "focus up"),
  LENTRY3(KC_J, KC_F, KC_J, h_j_focus_down,  LFLAG_NONE, "focus down"),
  LENTRY3(KC_J, KC_R, KC_H, h_j_resize_left,  LFLAG_NONE, "resize left"),
  LENTRY3(KC_J, KC_R, KC_L, h_j_resize_right, LFLAG_NONE, "resize right"),
  LENTRY3(KC_J, KC_R, KC_K, h_j_resize_up,    LFLAG_NONE, "resize up"),
  LENTRY3(KC_J, KC_R, KC_J, h_j_resize_down,  LFLAG_NONE, "resize down"),
  LENTRY3(KC_J, KC_O, KC_F, h_j_toggle_floating_all, LFLAG_NONE, "toggle floating panes"),
  LENTRY3(KC_J, KC_O, KC_E, h_j_toggle_embed,        LFLAG_NONE, "toggle embed/floating"),
  LENTRY3(KC_J, KC_O, KC_U, h_j_toggle_full,         LFLAG_NONE, "toggle fullscreen"),
  LENTRY3(KC_J, KC_O, KC_B, h_j_toggle_frames,       LFLAG_NONE, "toggle frames"),

  // ───────────── sed group (S + E + D) ─────────────
  LENTRY1(KC_S,              h_s_help,         LFLAG_NONE, "sed: help group"),
  LENTRY2(KC_S, KC_H,        h_sed_help,       LFLAG_NONE, "sed --help"),
  LENTRY2(KC_S, KC_R,        h_sed_replace,    LFLAG_NONE, "replace template"),
  LENTRY2(KC_S, KC_F,        h_sed_filter,     LFLAG_NONE, "filter lines by pattern"),
  LENTRY2(KC_S, KC_E,        h_sed_echo,       LFLAG_NONE, "echo text | sed ..."),
  LENTRY2(KC_S, KC_Z,        h_sed_fzf,        LFLAG_NONE, "fzf → sed pattern"),
};
// clang-format on

size_t leader_table_count(void) {
  return sizeof(leader_table) / sizeof(leader_table[0]);
}
