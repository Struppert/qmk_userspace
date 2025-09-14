
#pragma once
#include "layout_core.h"
// clang-format off
#define QWERTZ60_ROW1  QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS,   DE_ACUT, KC_BSPC,
#define QWERTZ60_ROW2  KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z,  KC_U, KC_I, KC_O,  KC_P,  DE_UDIA, DE_PLUS,
#define QWERTZ60_ROW3  TD(TD_CAPS_SYS_LEAD), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,
#define QWERTZ60_ROW4  KC_LSFT, DE_LABK, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, KC_RSFT,
// clang-format on

// Kern-ROW5 (5 Tasten)
#define QWERTZ60_R5_LCTL KC_LCTL
#define QWERTZ60_R5_LALT KC_LALT
#define QWERTZ60_R5_SPACE SP_FN
#define QWERTZ60_R5_RALT KC_RALT
#define QWERTZ60_R5_RCTL KC_RCTL
