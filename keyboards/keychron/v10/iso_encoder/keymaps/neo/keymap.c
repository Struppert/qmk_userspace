// keyboards/keychron/v10/iso_encoder/keymaps/neo/keymap.c
// V10 = 90-Tasten ISO+Knob, USB-only (kein BT/2.4G, kein Fork nötig -
// baut gegen mainline QMK wie Q6/V3). Kein Makro-Spalte wie Q10 Pro, aber
// dieselbe Fn-Taste zwischen B und N und dieselbe geteilte Leertaste - siehe
// formfactors/ff_90_iso_v10.h für die Details/Annahmen.

#define LAYOUT_ISO LAYOUT_iso_90
#include QMK_KEYBOARD_H

#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core, hier nur ROW1-4 genutzt -
// Row5 ist beim V10 wegen der geteilten Leertaste fix, siehe
// formfactors/ff_90_iso_v10.h)
#include "layouts/fn60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/qwertz60.h"
#include "layouts/rgb60.h"
#include "layouts/sys60.h"

// 90-Tasten-ISO-Formfactor mit Fn-Taste/geteilter Leertaste
#include "formfactors/ff_90_iso_v10.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_90_ISO_V10(QWERTZ60),
    [_NEOQWERTZ1]  = KEYMAP_90_ISO_V10(NEOQWERTZ1_60),
    [_NEOQWERTZ2]  = KEYMAP_90_ISO_V10(NEOQWERTZ2_60),
    [_NEOQWERTZ3]  = KEYMAP_90_ISO_V10(NEOQWERTZ3_60),
    [_NEOQWERTZ4]  = KEYMAP_90_ISO_V10(NEOQWERTZ4_60),
    [_NOTED1]      = KEYMAP_90_ISO_V10(NOTED1_60),
    [_NOTED2]      = KEYMAP_90_ISO_V10(NOTED2_60),
    [_NOTED3]      = KEYMAP_90_ISO_V10(NOTED3_60),
    [_NOTED4]      = KEYMAP_90_ISO_V10(NOTED4_60),
    [_FN]          = KEYMAP_90_ISO_V10(FN60),
    [_SYS]         = KEYMAP_90_ISO_V10(SYS60),
    [_RGB]         = KEYMAP_90_ISO_V10(RGB60),
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ3]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEOQWERTZ4]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NOTED1]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NOTED2]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NOTED3]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NOTED4]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN]          = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_RGB]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
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
