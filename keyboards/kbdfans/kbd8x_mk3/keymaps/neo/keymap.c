// keyboards/kbdfans/kbd8x_mk3/keymaps/neo/keymap.c - KBD8x with CUSTOM matrix
#define LAYOUT_ISO LAYOUT_tkl_iso
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

// TKL-Formfaktor (KBD8X) + Bottom-Row-Picker
#include "formfactors/ff_tkl_iso_kbd8x_mk3.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"

// Global matrix buffers - MUST be defined here for keymap_introspection.c to link
matrix_row_t raw_matrix[MATRIX_ROWS] = {0};
matrix_row_t matrix[MATRIX_ROWS] = {0};

// Matrix wrapper functions - these are called by QMK's main loop
// The _custom versions are implemented in keyboards/kbdfans/kbd8x_mk3/matrix.c
extern void matrix_init_custom(void);
extern bool matrix_scan_custom(matrix_row_t out[MATRIX_ROWS]);

void matrix_init(void) {
    matrix_init_custom();
}

uint8_t matrix_scan(void) {
    return matrix_scan_custom(raw_matrix);
}

// matrix_get_row() is defined in matrix.c (handles MATRIX_MASKED there)

// Stub for matrix_print - called only when debug_config.matrix is true
// Define in keymap.c to avoid ARM/Thumb linking issues
void matrix_print(void) {
    // Debug print disabled for KBD8x to save memory
    // If needed: print("Matrix state\n"); for debugging
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_TKL_ISO_KBD8X(QWERTZ60,      (BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ1_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ2_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ3_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ4_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_TKL_ISO_KBD8X(NOTED1_60,     (BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_TKL_ISO_KBD8X(NOTED2_60,     (BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_TKL_ISO_KBD8X(NOTED3_60,     (BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_TKL_ISO_KBD8X(NOTED4_60,     (BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_FN]          = KEYMAP_TKL_ISO_KBD8X(FN60,          (BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_TKL_ISO_KBD8X(SYS60,         (BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD))),
    [_RGB]         = KEYMAP_TKL_ISO_KBD8X(RGB60,         (BR8_POS_1_3_4_5_8(RGB60,         KC_LGUI, KC_RGUI, QK_LEAD))),
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
