#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// ── F-Row (V1): 15 Tasten inkl. DEL + MUTE am Ende
#define V1_FROW0_DEFAULT \
  KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_MUTE,

// ── F-Row (V1) – FN/Alt-Belegung (bereinigt)
#define V1_FROW0_FN \
  _______, KC_BRID, KC_BRIU, LGUI(KC_TAB), LGUI(KC_E), RM_VALD, RM_VALU, \
  KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,

// ── Center-Block (V1)
#define V1_CENTER_ROW1   KC_PGUP
#define V1_CENTER_ROW2   KC_PGDN
#define V1_CENTER_ROW3   KC_HOME
#define V1_CENTER_ROW4   KC_UP
#define V1_CENTER_ROW5   KC_LEFT, KC_DOWN, KC_RGHT

// ── 75%-Argumentliste für LAYOUT_iso_83 (genau 83 Keys)
#define FF75_ISO_V1_LIST(MAIN, BRROW5, FROW0) \
    /* Row0 (F)   */ FROW0 \
    /* Row1 main */ MAIN##_ROW1 \
    /* Row1 mid  */ V1_CENTER_ROW1, \
    /* Row2 main */ MAIN##_ROW2 \
    /* Row2 mid  */ V1_CENTER_ROW2, \
    /* Row3 main */ MAIN##_ROW3 \
    /* Row3 mid  */ V1_CENTER_ROW3, \
    /* Row4 main */ MAIN##_ROW4 \
    /* Row4 mid  */ V1_CENTER_ROW4, \
    /* Row5 br   */ EXPAND BRROW5, \
    /* Row5 mid  */ V1_CENTER_ROW5

// Wrapper
#define KEYMAP_75_ISO_V1(MAIN, BRROW5) \
    LAYOUT_APPLY(LAYOUT_ISO, FF75_ISO_V1_LIST(MAIN, BRROW5, V1_FROW0_DEFAULT))

#define KEYMAP_75_ISO_V1_FN(MAIN, BRROW5) \
    LAYOUT_APPLY(LAYOUT_ISO, FF75_ISO_V1_LIST(MAIN, BRROW5, V1_FROW0_FN))
// clang-format on
