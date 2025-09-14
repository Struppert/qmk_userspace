// keyboards/keychron/v1/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_iso_83
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
    // Default-F-Row
    [_QWERTZ]      = KEYMAP_75_ISO_V1(     QWERTZ60,      (BR7_POS_1_3_4_5_7(QWERTZ60,      KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_75_ISO_V1(     NEOQWERTZ1_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ1_60, KC_LGUI, QK_LEAD))),

    // Alternative F-Row (dein Wunsch)
    [_NEOQWERTZ2]  = KEYMAP_75_ISO_V1_FN(  NEOQWERTZ2_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ2_60, KC_LGUI, QK_LEAD))),

    // Default-F-Row
    [_NEOQWERTZ3]  = KEYMAP_75_ISO_V1(     NEOQWERTZ3_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ3_60, KC_LGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_75_ISO_V1(     NEOQWERTZ4_60, (BR7_POS_1_3_4_5_7(NEOQWERTZ4_60, KC_LGUI, QK_LEAD))),

    // Default-F-Row
    [_NOTED1]      = KEYMAP_75_ISO_V1(     NOTED1_60,     (BR7_POS_1_3_4_5_7(NOTED1_60,     KC_LGUI, QK_LEAD))),

    // Alternative F-Row (dein Wunsch)
    [_NOTED2]      = KEYMAP_75_ISO_V1_FN(  NOTED2_60,     (BR7_POS_1_3_4_5_7(NOTED2_60,     KC_LGUI, QK_LEAD))),

    // Default-F-Row
    [_NOTED3]      = KEYMAP_75_ISO_V1(     NOTED3_60,     (BR7_POS_1_3_4_5_7(NOTED3_60,     KC_LGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_75_ISO_V1(     NOTED4_60,     (BR7_POS_1_3_4_5_7(NOTED4_60,     KC_LGUI, QK_LEAD))),

    // Alternative F-Row (dein Wunsch)
    [_FN]          = KEYMAP_75_ISO_V1_FN(  FN60,          (BR7_POS_1_3_4_5_7(FN60,          KC_LGUI, QK_LEAD))),

    // Default-F-Row
    [_SYS]         = KEYMAP_75_ISO_V1(     SYS60,         (BR7_POS_1_3_4_5_7(SYS60,         KC_LGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_75_ISO_V1(     RGB60,         (BR7_POS_1_3_4_5_7(RGB60,         KC_LGUI, QK_LEAD))),

};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    [_NEOQWERTZ3]  = { ENCODER_CCW_CW(RGB_SAD, RGB_SAI) },
    [_NEOQWERTZ4]  = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_FN]          = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_RGB]         = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif

// clang-format on

#include "tap_dance_bindings.inc"
