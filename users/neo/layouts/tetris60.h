#pragma once
#include "layout_core.h"
// clang-format off

// Tetris-Layer: nur H/J/K/L (vim-Bewegung) sind belegt, der Rest ist tot.
// H=links, J=runter (soft drop), K=rotieren (analog "hoch" bei klassischem
// Tetris), L=rechts - exakt an den physischen H/J/K/L-Positionen von
// QWERTZ60_ROW3, damit die Belegung layoutunabhängig an der Home Row sitzt.

#define TETRIS60_ROW1 \
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

#define TETRIS60_ROW2 \
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

#define TETRIS60_ROW3 \
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TET_LEFT, TET_DOWN, TET_ROT, TET_RIGHT, KC_NO, KC_NO, KC_NO, KC_NO,

#define TETRIS60_ROW4 \
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

#define TETRIS60_R5_LCTL  KC_NO
#define TETRIS60_R5_LALT  KC_NO
#define TETRIS60_R5_SPACE TET_DROP
#define TETRIS60_R5_RALT  KC_NO
#define TETRIS60_R5_RCTL  KC_NO
// clang-format on
