// keyboards/keychron/v6/iso_encoder/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_iso_110
#include QMK_KEYBOARD_H

#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core)
#include "layouts/fn60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/qwertz60.h"
#include "layouts/rgb60.h"
#include "layouts/sys60.h"

// 96%-ISO-Formfaktor (V6, mit Encoder) + Bottom-Row-Picker
#include "formfactors/ff_96_iso_q6_v6.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_96_ISO_V6(QWERTZ60,      (BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_96_ISO_V6(NEOQWERTZ1_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_96_ISO_V6(NEOQWERTZ2_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_96_ISO_V6(NEOQWERTZ3_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_96_ISO_V6(NEOQWERTZ4_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_96_ISO_V6(NOTED1_60,     (BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_96_ISO_V6(NOTED2_60,     (BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_96_ISO_V6(NOTED3_60,     (BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_96_ISO_V6(NOTED4_60,     (BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_FN]          = KEYMAP_96_ISO_V6(FN60,          (BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_96_ISO_V6(SYS60,         (BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_96_ISO_V6(RGB60,         (BR8_POS_1_3_4_5_8(RGB60,         KC_LGUI, KC_RGUI, QK_LEAD))),
};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ3]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ4]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED1]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED2]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED3]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED4]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_FN]          = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_RGB]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

#include "tap_dance_bindings.inc"

bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
