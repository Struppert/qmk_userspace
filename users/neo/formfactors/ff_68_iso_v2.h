#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// ── Center-Block (V1)
#define V1_CENTER_ROW1   KC_MUTE
#define V1_CENTER_ROW2   KC_PGUP
#define V1_CENTER_ROW3   KC_PGDN
#define V1_CENTER_ROW4   KC_UP
#define V1_CENTER_ROW5   KC_LEFT, KC_DOWN, KC_RGHT

// ── 75%-Argumentliste für LAYOUT_iso_83 (genau 83 Keys)
#define FF68_ISO_V2_LIST(MAIN, BRROW5) \
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
#define KEYMAP_68_ISO_V2(MAIN, BRROW5) \
    LAYOUT_APPLY(LAYOUT_ISO, FF68_ISO_V2_LIST(MAIN, BRROW5))

// clang-format on
