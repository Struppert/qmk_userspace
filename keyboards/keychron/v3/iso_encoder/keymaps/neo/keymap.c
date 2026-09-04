// keyboards/keychron/v3/iso_encoder/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_tkl_f13_iso
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

// TKL-Formfaktor (V3) + Bottom-Row-Picker
#include "formfactors/ff_tkl_iso_v3.h"
#include "formfactors/row5_pick.h"

#ifdef TETRIS_GAME_ENABLE
#include "layouts/tetris60.h"
#include "tetris.h"
#endif

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"

#ifdef TETRIS_GAME_ENABLE
#define TETRIS_ENTRY TG(_TETRIS)
#else
#define TETRIS_ENTRY KC_NO
#endif

#undef SYS60_ROW2
#define SYS60_ROW2  KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), TETRIS_ENTRY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_TKL_ISO_V3(QWERTZ60,      (BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_TKL_ISO_V3(NEOQWERTZ1_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_TKL_ISO_V3(NEOQWERTZ2_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_TKL_ISO_V3(NEOQWERTZ3_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_TKL_ISO_V3(NEOQWERTZ4_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_TKL_ISO_V3(NOTED1_60,     (BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_TKL_ISO_V3(NOTED2_60,     (BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_TKL_ISO_V3(NOTED3_60,     (BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_TKL_ISO_V3(NOTED4_60,     (BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_FN]          = KEYMAP_TKL_ISO_V3(FN60,          (BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_TKL_ISO_V3(SYS60,         (BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_TKL_ISO_V3(RGB60,         (BR8_POS_1_3_4_5_8(RGB60,         KC_LGUI, KC_RGUI, QK_LEAD))),
#ifdef TETRIS_GAME_ENABLE
    [_TETRIS]      = KEYMAP_TKL_ISO_V3(TETRIS60,      (BR8_POS_1_3_4_5_8(TETRIS60,      KC_NO, KC_NO, KC_NO))),
#endif
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
#ifdef TETRIS_GAME_ENABLE
  [_TETRIS]      = { ENCODER_CCW_CW(KC_NO, KC_NO)},
#endif
};
#endif // ENCODER_MAP_ENABLE

#include "tap_dance_bindings.inc"

__attribute__((weak)) bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
