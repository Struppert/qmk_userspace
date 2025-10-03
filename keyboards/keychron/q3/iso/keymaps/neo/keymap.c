// keyboards/keychron/q3/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_tkl_iso
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

// TKL-Formfaktor (Q3) + Bottom-Row-Picker
#include "formfactors/ff_tkl_iso_q3.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_TKL_ISO_Q3(QWERTZ60,      (BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_TKL_ISO_Q3(NEOQWERTZ1_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_TKL_ISO_Q3(NEOQWERTZ2_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_TKL_ISO_Q3(NEOQWERTZ3_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_TKL_ISO_Q3(NEOQWERTZ4_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_TKL_ISO_Q3(NOTED1_60,     (BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_TKL_ISO_Q3(NOTED2_60,     (BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_TKL_ISO_Q3(NOTED3_60,     (BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_TKL_ISO_Q3(NOTED4_60,     (BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_FN]          = KEYMAP_TKL_ISO_Q3(FN60,          (BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_TKL_ISO_Q3(SYS60,         (BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_TKL_ISO_Q3(RGB60,         (BR8_POS_1_3_4_5_8(RGB60,         KC_LGUI, KC_RGUI, QK_LEAD))),
};

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
