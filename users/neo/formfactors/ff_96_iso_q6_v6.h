#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// Q6 (LAYOUT_iso_109, kein Encoder) und V6 (LAYOUT_iso_110, mit Encoder)
// teilen sich dieselbe Matrix bis auf einen einzigen Slot: den
// Encoder-Druckknopf (nur bei V6 vorhanden, sitzt in Row0 direkt nach der
// F-Reihe, vor PSCR). Alles andere - 60%-Kernblock, Nav-Cluster, Numpad,
// F13-F16 - ist auf beiden Boards identisch (siehe QMK-Stock-Keymaps
// keychron/q6/iso/keymaps/default und keychron/v6/iso_encoder/keymaps/default,
// dort unterscheiden sich WIN_BASE Row0 nur um dieses eine KC_MUTE).

// ── F-Row (Q6, kein Encoder): 20 Slots inkl. F13-F16
#define Q6_FROW0_DEFAULT \
KC_ESC, F_ROW_BASE, KC_PSCR, KC_NO, RM_NEXT, KC_F13, KC_F14, KC_F15, KC_F16,

#define Q6_FROW0_FN \
_______, KC_BRID, KC_BRIU, LGUI(KC_TAB), LGUI(KC_E), RM_VALD, RM_VALU, \
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, RM_TOGG, \
_______, _______, _______, _______,

// ── F-Row (V6, mit Encoder): 21 Slots - wie Q6, plus Encoder-Klick (KC_MUTE)
// direkt nach der F-Reihe.
#define V6_FROW0_DEFAULT \
KC_ESC, F_ROW_BASE, KC_MUTE, KC_PSCR, KC_NO, RM_NEXT, KC_F13, KC_F14, KC_F15, KC_F16,

#define V6_FROW0_FN \
_______, KC_BRID, KC_BRIU, LGUI(KC_TAB), LGUI(KC_E), RM_VALD, RM_VALU, \
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, RM_TOGG, \
_______, _______, _______, _______,

// ── Nav-Cluster (Ins/Home/PgUp .. Links/Runter/Rechts) - identisch Q6/V6,
// gleiche Spalte wie bei den TKL-Boards (V3/Q3/KBD8X).
#define Q6V6_CENTER_ROW1   KC_INS,   KC_HOME,  KC_PGUP
#define Q6V6_CENTER_ROW2   KC_DEL,   KC_END,   KC_PGDN
#define Q6V6_CENTER_ROW3
#define Q6V6_CENTER_ROW4   KC_UP
#define Q6V6_CENTER_ROW5   KC_LEFT, KC_DOWN, KC_RGHT

// ── Numpad-Block (rechte Spalte) - identisch Q6/V6, literal (kein NumLock-
// Layer, physischer Numpad-Block bleibt auf jeder Ebene erreichbar, analog
// zum Nav-Cluster).
#define Q6V6_NUMPAD_ROW1   KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS
#define Q6V6_NUMPAD_ROW2   KC_P7,    KC_P8,    KC_P9
#define Q6V6_NUMPAD_ROW3   KC_P4,    KC_P5,    KC_P6,    KC_PPLS
#define Q6V6_NUMPAD_ROW4   KC_P1,    KC_P2,    KC_P3
#define Q6V6_NUMPAD_ROW5   KC_P0,    KC_PDOT,  KC_PENT

#define FF96_ISO_Q6V6_LIST(MAIN, BRROW5, FROW0) \
/* Row0 (F)   */ FROW0 \
/* Row1 main */ MAIN##_ROW1 \
/* Row1 nav  */ Q6V6_CENTER_ROW1, \
/* Row1 np   */ Q6V6_NUMPAD_ROW1, \
/* Row2 main */ MAIN##_ROW2 \
/* Row2 nav  */ Q6V6_CENTER_ROW2, \
/* Row2 np   */ Q6V6_NUMPAD_ROW2, \
/* Row3 main */ MAIN##_ROW3 \
/* Row3 np   */ Q6V6_NUMPAD_ROW3, \
/* Row4 main */ MAIN##_ROW4 \
/* Row4 nav  */ Q6V6_CENTER_ROW4, \
/* Row4 np   */ Q6V6_NUMPAD_ROW4, \
/* Row5 br   */ EXPAND BRROW5, \
/* Row5 nav  */ Q6V6_CENTER_ROW5, \
/* Row5 np   */ Q6V6_NUMPAD_ROW5

// Wrapper Q6 (kein Encoder)
#define KEYMAP_96_ISO_Q6(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FF96_ISO_Q6V6_LIST(MAIN, BRROW5, Q6_FROW0_DEFAULT))

#define KEYMAP_96_ISO_Q6_FN(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FF96_ISO_Q6V6_LIST(MAIN, BRROW5, Q6_FROW0_FN))

// Wrapper V6 (mit Encoder)
#define KEYMAP_96_ISO_V6(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FF96_ISO_Q6V6_LIST(MAIN, BRROW5, V6_FROW0_DEFAULT))

#define KEYMAP_96_ISO_V6_FN(MAIN, BRROW5) \
LAYOUT_APPLY(LAYOUT_ISO, FF96_ISO_Q6V6_LIST(MAIN, BRROW5, V6_FROW0_FN))
// clang-format on
