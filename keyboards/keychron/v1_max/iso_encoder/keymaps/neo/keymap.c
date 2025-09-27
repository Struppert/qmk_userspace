// keyboards/keychron/v1/iso/keymaps/neo/keymap.c

// ----- Board-Layout-Auswahl (V1 Max ISO+Encoder) -----
#define LAYOUT_ISO LAYOUT_iso_83
#define _BLUETOOTH_LAYER

// ---- Fallbacks für ältere QMKs (Keychron-Fork) ----
#ifndef QK_RGB_MATRIX_TOGGLE
#define QK_RGB_MATRIX_TOGGLE RGB_TOG
#define QK_RGB_MATRIX_MODE_NEXT RGB_MOD
#define QK_RGB_MATRIX_MODE_PREVIOUS RGB_RMOD
#define QK_RGB_MATRIX_VALUE_UP RGB_VAI
#define QK_RGB_MATRIX_VALUE_DOWN RGB_VAD
#define QK_RGB_MATRIX_SPEED_UP RGB_SPI
#define QK_RGB_MATRIX_SPEED_DOWN RGB_SPD
#define QK_RGB_MATRIX_HUE_UP RGB_HUI
#define QK_RGB_MATRIX_HUE_DOWN RGB_HUD
#define QK_RGB_MATRIX_SATURATION_UP RGB_SAI
#define QK_RGB_MATRIX_SATURATION_DOWN RGB_SAD
#endif

// ---- Stubs für Keychron-/OS-Extras, die der Fork evtl. nicht kennt ----
#ifndef KC_TASK
#define KC_TASK KC_NO
#endif
#ifndef KC_FILE
#define KC_FILE KC_NO
#endif
#ifndef BT_HST1
#define BT_HST1 KC_NO
#define BT_HST2 KC_NO
#define BT_HST3 KC_NO
#define P2P4G KC_NO
#endif
#ifndef BAT_LVL
#define BAT_LVL KC_NO
#endif

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
#include "formfactors/ff_75_iso_v1.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_75_ISO_V1(     QWERTZ60,      (BR7_POS_1_3_4_5_7(QWERTZ60,      KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_75_ISO_V1(     NEOQWERTZ1_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ1_60, KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_75_ISO_V1(     NEOQWERTZ2_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ2_60, KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_75_ISO_V1(     NEOQWERTZ3_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ3_60, KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_75_ISO_V1(     NEOQWERTZ4_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ4_60, KC_LGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_75_ISO_V1(     NOTED1_60,     (BR7_POS_1_3_4_5_7(NOTED1_60,     KC_LGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_75_ISO_V1(     NOTED2_60,     (BR7_POS_1_3_4_5_7(NOTED2_60,     KC_LGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_75_ISO_V1(     NOTED3_60,     (BR7_POS_1_3_4_5_7(NOTED3_60,     KC_LGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_75_ISO_V1(     NOTED4_60,     (BR7_POS_1_3_4_5_7(NOTED4_60,     KC_LGUI, QK_LEAD))),

    [_WIN_FN] = LAYOUT_iso_83(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,            RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_END,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

    [_FN]          = KEYMAP_75_ISO_V1(     FN60,          (BR7_POS_1_3_4_5_7(FN60,          KC_LGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_75_ISO_V1(     SYS60,         (BR7_POS_1_3_4_5_7(SYS60,         MO(_WIN_FN), QK_LEAD))),
    [_RGB]         = KEYMAP_75_ISO_V1(     RGB60,         (BR7_POS_1_3_4_5_7(RGB60,         MO(_WIN_FN), QK_LEAD))),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    [_NEOQWERTZ3]  = { ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [_NEOQWERTZ4]  = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NOTED1]      = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NOTED2]      = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NOTED3]      = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_NOTED4]      = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_FN]          = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_RGB]         = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [_WIN_FN]      = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
};
#endif

// clang-format on

#include "tap_dance_bindings.inc"
__attribute__((weak)) bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
