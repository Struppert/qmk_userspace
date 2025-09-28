/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
#include QMK_KEYBOARD_H

// clang-format off

enum layers{
    MAC_BASE,
    WIN_BASE,
    _FN1,
    _FN2,
    _FN3
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_iso_68(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,
KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,                    KC_DEL,
        KC_CAPS, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,
KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,           KC_HOME,
        KC_LSFT, KC_NUBS,  KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,
KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            KC_RSFT, KC_UP, KC_LCTL,
KC_LOPT,  KC_LCMD,                             KC_SPC, KC_RCMD,  MO(_FN1),
MO(_FN3), KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_BASE] = LAYOUT_iso_68(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,
KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,                    KC_DEL,
        KC_CAPS, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,
KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,           KC_HOME,
        KC_LSFT, KC_NUBS,  KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,
KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            KC_RSFT, KC_UP, KC_LCTL,
KC_LWIN,  KC_LALT,                             KC_SPC, KC_RALT,  MO(_FN2),
MO(_FN3), KC_LEFT, KC_DOWN, KC_RGHT),

    [_FN1] = LAYOUT_iso_68(
        KC_GRV,  KC_BRID,  KC_BRIU,  KC_NO,   KC_NO,   RM_VALD, RM_VALU,
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______, _______,
        RM_TOGG, RM_NEXT,  RM_VALU,  RM_HUEU, RM_SATU, RM_SPDU, _______,
_______, _______, _______, _______,  _______,  _______, _______,
        _______, RM_PREV,  RM_VALD,  RM_HUED, RM_SATD, RM_SPDD, _______,
_______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,  _______, _______, _______, _______,
NK_TOGG, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,
_______,  _______,  _______,  _______, _______, _______),

    [_FN2] = LAYOUT_iso_68(
        KC_GRV,  KC_BRID,  KC_BRIU,  KC_TASK, KC_FLXP, RM_VALD, RM_VALU,
KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  _______, _______,
        RM_TOGG, RM_NEXT,  RM_VALU,  RM_HUEU, RM_SATU, RM_SPDU, _______,
_______, _______, _______, _______,  _______,  _______, _______,
        _______, RM_PREV,  RM_VALD,  RM_HUED, RM_SATD, RM_SPDD, _______,
_______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,  _______, _______, _______, _______,
NK_TOGG, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,
_______,  _______,  _______,  _______, _______, _______),

    [_FN3] = LAYOUT_iso_68(
        KC_TILD, KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,
KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,          _______,
        RM_TOGG, RM_NEXT,  RM_VALU,  RM_HUEU, RM_SATU, RM_SPDU, _______,
_______, _______, _______, _______,  _______,  _______, _______,
        _______, RM_PREV,  RM_VALD,  RM_HUED, RM_SATD, RM_SPDD, _______,
_______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,  _______, _______, _______, _______,
_______, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,
_______,  _______,  _______,  _______, _______, _______)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_FN1]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_FN2]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_FN3]   = { ENCODER_CCW_CW(_______, _______)}
};
*/

// keyboards/keychron/v1/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_iso_68
#include QMK_KEYBOARD_H

// #define V1_MINIMAL_ENUM // z.Z. nicht noetig da eeprom geaendert wurde
#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core)
#include "layouts/fn60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/qwertz60.h"
#include "layouts/rgb60.h"
#include "layouts/sys60.h"

// 75%-Formfaktor (V1) + Bottom-Row-Picker
#include "formfactors/ff_68_iso_v2.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_68_ISO_V2(     QWERTZ60,      (BR7_POS_1_3_4_5_7(QWERTZ60,      KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_68_ISO_V2(     NEOQWERTZ1_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ1_60, KC_LGUI, QK_LEAD))),

    [_NEOQWERTZ2]  = KEYMAP_68_ISO_V2(     NEOQWERTZ2_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ2_60, KC_LGUI, QK_LEAD))),

    [_NEOQWERTZ3]  = KEYMAP_68_ISO_V2(     NEOQWERTZ3_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ3_60, KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_68_ISO_V2(     NEOQWERTZ4_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ4_60, KC_LGUI, QK_LEAD))),

    [_NOTED1]      = KEYMAP_68_ISO_V2(     NOTED1_60,     (BR7_POS_1_3_4_5_7(NOTED1_60,     KC_LGUI, QK_LEAD))),

    [_NOTED2]      = KEYMAP_68_ISO_V2(     NOTED2_60,     (BR7_POS_1_3_4_5_7(NOTED2_60,     KC_LGUI, QK_LEAD))),

    [_NOTED3]      = KEYMAP_68_ISO_V2(     NOTED3_60,     (BR7_POS_1_3_4_5_7(NOTED3_60,     KC_LGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_68_ISO_V2(     NOTED4_60,     (BR7_POS_1_3_4_5_7(NOTED4_60,     KC_LGUI, QK_LEAD))),

    [_FN]          = KEYMAP_68_ISO_V2(     FN60,          (BR7_POS_1_3_4_5_7(FN60,          KC_LGUI, QK_LEAD))),

    [_SYS]         = KEYMAP_68_ISO_V2(     SYS60,         (BR7_POS_1_3_4_5_7(SYS60,         KC_LGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_68_ISO_V2(     RGB60,         (BR7_POS_1_3_4_5_7(RGB60,         KC_LGUI, QK_LEAD))),

};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    [_NEOQWERTZ3]  = { ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [_NEOQWERTZ4]  = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NOTED1]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NOTED2]      = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    [_NOTED3]      = { ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [_NOTED4]      = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_FN]          = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_RGB]         = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

#include "tap_dance_bindings.inc"

bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
