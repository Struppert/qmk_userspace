#pragma once
#include "quantum.h"

// Keychron V1 – F-Row Presets (je Layer verwendbar)
// Aufteilung für 75%: 13 Keys links (FROW0) + 2 Keys rechts (CROW0) – beide mit
// trailing comma!

// clang-format off
// Default (Stock)
#define V1_FROW0_DEFAULT \
  KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,

#define V1_CROW0_DEFAULT \
  KC_DEL, KC_MUTE,

// Alternative Belegung
#define V1_FROW0_ALT \
  _______, KC_BRID, KC_BRIU, KC_TASK, KC_FLXP, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,

#define V1_CROW0_ALT \
  _______, _______,

//clang-format on
