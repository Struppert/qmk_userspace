// qmk_userspace/keyboards/kbdfans/bella/rgb_iso/keymaps/neo/keymap.c
#ifndef LAYOUT_ISO
#define LAYOUT_ISO LAYOUT
#endif

#include QMK_KEYBOARD_H

#define BELLA_COMPACT_ENUM // EEPROM (1KB) reicht nur fuer 4 kompakte Layer
#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core)
#include "layouts/fn60.h"
#include "layouts/qwertz60.h"
#include "layouts/rgb60.h"
#include "layouts/sys60.h"

// 75%-Formfaktor (BELLA) + Bottom-Row-Picker
#include "formfactors/ff_75_iso_bella.h"
#include "formfactors/row5_pick.h"

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#if COMBO_ENABLE
#if defined(__AVR__) // nur nötig/aktiv auf AVR
#if defined(__has_builtin)
#if __has_builtin(__builtin_popcount) /* dummy to keep compilers calm */
#endif
#endif
#pragma push_macro("combo_t")
#undef combo_t
#define combo_t const combo_t
#include "combos_bindings.inc"
#pragma pop_macro("combo_t")
#else
#include "combos_bindings.inc"
#endif
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTZ]      = KEYMAP_75_ISO_BELLA(     QWERTZ60,      (BR6_POS_1_3_4_5_6(QWERTZ60,      KC_LGUI))),
	[_FN]          = KEYMAP_75_ISO_BELLA(     FN60,          (BR6_POS_1_3_4_5_6(FN60,          KC_LGUI))),
	[_SYS]         = KEYMAP_75_ISO_BELLA(     SYS60,         (BR6_POS_1_3_4_5_6(SYS60,         KC_LGUI))),
	[_RGB]         = KEYMAP_75_ISO_BELLA(     RGB60,         (BR6_POS_1_3_4_5_6(RGB60,         KC_LGUI))),
};
// clang-format on

#if defined(__AVR__)
#pragma push_macro("tap_dance_action_t")
#undef tap_dance_action_t
#define tap_dance_action_t const tap_dance_action_t
#include "tap_dance_bindings.inc"
#pragma pop_macro("tap_dance_action_t")
#else
#include "tap_dance_bindings.inc"
#endif

bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
