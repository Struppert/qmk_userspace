// leader/table_min.c
// Reduced leader table for kbd8x_mk3 (20K RAM, ~3K headroom - see
// LEADER_TRIE_MAX_NODES/EDGES in rules.mk). Only rg + fzf, the two groups
// that mattered most; Git/WezTerm/zoxide/yazi/zellij/sed pulled in too
// many handler files for the remaining budget. Compiled INSTEAD of
// leader/table.c - never both (duplicate leader_table[]/leader_table_count()).
#include QMK_KEYBOARD_H
#include "core.h"
#include "handlers.h"

// clang-format off
const LeaderEntry leader_table[] = {
  // Global
  LENTRY2(KC_A, KC_A, h_leader_ok, LFLAG_NONE, "Test: LEADER OK"),

  // fzf (+ H)
  LENTRY2(KC_F, KC_H, h_ctx_F,                  LFLAG_NONE, "Hilfe fzf"),
  LENTRY2(KC_F, KC_F, h_fzf_run,                LFLAG_NONE, "fzf"),
  LENTRY2(KC_F, KC_E, h_fzf_edit_sel,           LFLAG_NONE, "Auswahl -> $EDITOR"),
  LENTRY2(KC_F, KC_P, h_fzf_preview,            LFLAG_NONE, "Preview mit bat"),
  LENTRY2(KC_F, KC_O, h_fzf_files_preview_open, LFLAG_NONE, "Dateien mit Preview -> Editor"),
  LENTRY2(KC_F, KC_M, h_fzf_files_multi_open,   LFLAG_NONE, "Multi-Select -> Editor"),
  LENTRY2(KC_F, KC_D, h_fzf_dirs_cd,            LFLAG_NONE, "Verzeichnis -> cd"),

  // ripgrep (+ H)
  LENTRY2(KC_R, KC_H, h_ctx_R,          LFLAG_NONE, "Hilfe rg"),
  LENTRY2(KC_R, KC_R, h_rg_base,        LFLAG_NONE, "rg Basis"),
  LENTRY2(KC_R, KC_S, h_rg_smart,       LFLAG_NONE, "-S smart"),
  LENTRY2(KC_R, KC_A, h_rg_gitfiles,    LFLAG_NONE, "git ls-files | rg"),
  LENTRY2(KC_R, KC_L, h_rg_less,        LFLAG_NONE, "rg … | less -R"),
  LENTRY3(KC_R, KC_G, KC_R, h_rg_go_glob, LFLAG_NONE, "-g '*.go'"),
  LENTRY2(KC_R, KC_B, h_rg_rs_glob,     LFLAG_NONE, "-g '*.rs'"),
  // NOTE: table.c (full board) has a second R,F entry (h_rg_fzf_preview)
  // that's unreachable there too - leader_table_dispatch matches the
  // first hit, so only one R,F binding can ever fire. Kept the same one.
  LENTRY2(KC_R, KC_F, h_rg_fzf_edit,    LFLAG_NONE, "rg | fzf | nvim+Zeile"),
  LENTRY3(KC_R, KC_C, KC_A, h_rg_ctx_C3, LFLAG_NONE, "-C 3"),
  LENTRY3(KC_R, KC_C, KC_U, h_rg_ctx_B3, LFLAG_NONE, "-B 3"),
  LENTRY3(KC_R, KC_C, KC_D, h_rg_ctx_A3, LFLAG_NONE, "-A 3"),
  LENTRY2(KC_R, KC_M, h_rg_multiline,   LFLAG_NONE, "-U multiline"),
  LENTRY3(KC_R, KC_T, KC_C, h_rg_types_cpp,  LFLAG_NONE, "-t c/cpp/h/hpp"),
  LENTRY3(KC_R, KC_T, KC_R, h_rg_types_rust, LFLAG_NONE, "-t rust"),
  LENTRY3(KC_R, KC_T, KC_G, h_rg_types_go,   LFLAG_NONE, "-t go"),
};
// clang-format on

size_t leader_table_count(void) {
  return sizeof(leader_table) / sizeof(leader_table[0]);
}
