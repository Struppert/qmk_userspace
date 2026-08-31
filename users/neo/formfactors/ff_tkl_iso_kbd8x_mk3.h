#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// Alle *_ROW3-Makros (qwertz60.h, neoqwertz60.h, noted60.h, fn60.h, sys60.h,
// rgb60.h) liefern einheitlich 14 Elemente und enden immer mit ", KC_ENT,".
// Physisch sitzt Enter auf KBD8X_mk3 aber elektrisch in Row2's Matrixzeile
// (siehe KBD8X_CENTER_ROW2), nicht in Row3 - das per-Layer-Enter am Ende von
// *_ROW3 wird hier verworfen, damit Row3 auf die real vorhandenen 13
// physischen Tasten passt (sonst verschiebt sich jede folgende Taste um 1).
#define ROW3_DROP_ENT_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,...) \
a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,
#define ROW3_DROP_ENT(...) ROW3_DROP_ENT_(__VA_ARGS__)

// ── F-Row (KBD8X): 16 Tasten inkl. DEL + MUTE am Ende
// Letzte Taste war RM_NEXT (QK_RGB_MATRIX_MODE_NEXT) - totes RGB_MATRIX-
// Keycode auf einem Board mit RGB_MATRIX_ENABLE=no (nur RGBLIGHT). Auf
// UG_TOGG (RGBLIGHT-Toggle, tatsächlich funktionsfähig) umgestellt - vorher
// gab es keine einzige erreichbare Taste, um die WS2812-Kette an/aus zu
// schalten.
#define KBD8X_FROW0_DEFAULT \
KC_ESC, F_ROW_BASE, KC_PSCR,  KC_NO,    UG_TOGG,

// ── F-Row (KBD8X) – FN/Alt-Belegung (bereinigt)
#define KBD8X_FROW0_FN \
_______, KC_BRID, KC_BRIU, LGUI(KC_TAB), LGUI(KC_E), RM_VALD, RM_VALU, \
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,  _______,  _______,  RM_TOGG,

// ── Center-Block (KBD8X)
#define KBD8X_CENTER_ROW1   KC_INS,   KC_HOME,  KC_PGUP
// Enter sitzt elektrisch am Ende von Row2 (matrix[8,0], direkt nach dem "+"
// auf [8,6]) - nicht am Ende von Row3 wie bei einem generischen 60%-Board.
#define KBD8X_CENTER_ROW2   KC_ENT,   KC_DEL,   KC_END,   KC_PGDN
#define KBD8X_CENTER_ROW3
#define KBD8X_CENTER_ROW4   KC_UP
#define KBD8X_CENTER_ROW5   KC_LEFT, KC_DOWN, KC_RGHT

#define FFtkl_ISO_KBD8X_LIST(MAIN, BRROW5, FROW0) \
/* Row0 (F)   */ FROW0 \
/* Row1 main */ MAIN##_ROW1 \
/* Row1 mid  */ KBD8X_CENTER_ROW1, \
/* Row2 main */ MAIN##_ROW2 \
/* Row2 mid  */ KBD8X_CENTER_ROW2, \
/* Row3 main */ ROW3_DROP_ENT(MAIN##_ROW3) \
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
