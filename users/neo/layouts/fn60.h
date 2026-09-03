#pragma once
#include "layout_core.h"
// clang-format off

// QMK's own Dynamic Macros (DM_REC*/DM_PLY*/DM_RSTP) - not available on
// boards with DYNAMIC_MACRO_ENABLE=no (currently kbd8x_mk3, see its
// README.md RAM-Budget section: the recording buffer costs 2560B RAM,
// dropped in favor of VIA's own dynamic keymap macros, QK_MACRO_0..11
// below, which stay unaffected on every board). Falls back to KC_NO so
// this shared row layout doesn't need a board-specific copy.
#ifdef DYNAMIC_MACRO_ENABLE
  #define FN60_DM_PLY1 DM_PLY1
  #define FN60_DM_REC1 DM_REC1
  #define FN60_DM_REC2 DM_REC2
  #define FN60_DM_RSTP DM_RSTP
  #define FN60_DM_PLY2 DM_PLY2
#else
  #define FN60_DM_PLY1 KC_NO
  #define FN60_DM_REC1 KC_NO
  #define FN60_DM_REC2 KC_NO
  #define FN60_DM_RSTP KC_NO
  #define FN60_DM_PLY2 KC_NO
#endif

#define FN60_ROW1  KC_ESC, F_ROW_BASE, KC_DEL,
#define FN60_ROW2  KC_TAB, QK_MACRO_6, QK_MACRO_4, QK_MACRO_2, QK_MACRO_0, FN60_DM_PLY1, KC_NO, FN60_DM_REC1, FN60_DM_REC2, FN60_DM_RSTP, KC_NO, QK_MACRO_10, QK_MACRO_8,
#define FN60_ROW3  KC_ESC, QK_MACRO_7, QK_MACRO_5, QK_MACRO_3, QK_MACRO_1, FN60_DM_PLY2, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_NO, QK_MACRO_11, QK_MACRO_9, KC_ENT,
#define FN60_ROW4  KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MO, SYS_MO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,
// clang-format on

#define FN60_R5_LCTL KC_LCTL
#define FN60_R5_LALT KC_LALT
#define FN60_R5_SPACE SP_FN
#define FN60_R5_RALT KC_NO
#define FN60_R5_RCTL KC_RCTL
