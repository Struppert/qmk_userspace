#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// ── F-Row (KBD8X): 16 Tasten inkl. DEL + MUTE am Ende
#define KBD8X_FROW0_DEFAULT \
KC_ESC, F_ROW_BASE, KC_PSCR,  KC_NO,    RM_NEXT,

// ── F-Row (KBD8X) – FN/Alt-Belegung (bereinigt)
#define KBD8X_FROW0_FN \
_______, KC_BRID, KC_BRIU, LGUI(KC_TAB), LGUI(KC_E), RM_VALD, RM_VALU, \
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,  _______,  _______,  RM_TOGG,

// ── Center-Block (KBD8X)
#define KBD8X_CENTER_ROW1   KC_INS,   KC_HOME,  KC_PGUP
#define KBD8X_CENTER_ROW2   KC_DEL,   KC_END,   KC_PGDN
#define KBD8X_CENTER_ROW3
#define KBD8X_CENTER_ROW4   KC_UP
#define KBD8X_CENTER_ROW5   KC_LEFT, KC_DOWN, KC_RGHT

#define FFtkl_ISO_KBD8X_LIST(MAIN, BRROW5, FROW0) \
/* Row0 (F)   */ FROW0 \
/* Row1 main */ MAIN##_ROW1 \
/* Row1 mid  */ KBD8X_CENTER_ROW1, \
/* Row2 main */ MAIN##_ROW2 \
/* Row2 mid  */ KBD8X_CENTER_ROW2, \
/* Row3 main */ MAIN##_ROW3 \
/* Row4 main */ MAIN##_ROW4 \
/* Row4 mid  */ KBD8X_CENTER_ROW4, \
/* Row5 br   */ EXPAND BRROW5, \
/* Row5 mid  */ KBD8X_CENTER_ROW5

// Wrapper
#define KEYMAP_TKL_ISO_KBD8X(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FFtkl_ISO_KBD8X_LIST(MAIN, BRROW5, KBD8X_FROW0_DEFAULT))

#define KEYMAP_TKL_ISO_KBD8X_FN(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FFtkl_ISO_KBD8X_LIST(MAIN, BRROW5, KBD8X_FROW0_FN))
// clang-format on
