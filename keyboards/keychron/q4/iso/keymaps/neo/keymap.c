// keyboards/keychron/q4/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_iso_62
#include QMK_KEYBOARD_H
// clang-format off
#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

#include "layouts/qwertz60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/fn60.h"
#include "layouts/sys60.h"
#include "layouts/rgb60.h"

#include "formfactors/row5_pick.h"
#include "formfactors/ff_60_iso.h"

// Combos: entweder aus, oder stub/echte Datei:
#include "combos_bindings.inc"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_60_ISO_BR(QWERTZ60,      BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NEOQWERTZ1]  = KEYMAP_60_ISO_BR(NEOQWERTZ1_60, BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NEOQWERTZ2]  = KEYMAP_60_ISO_BR(NEOQWERTZ2_60, BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NEOQWERTZ3]  = KEYMAP_60_ISO_BR(NEOQWERTZ3_60, BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NEOQWERTZ4]  = KEYMAP_60_ISO_BR(NEOQWERTZ4_60, BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NOTED1]      = KEYMAP_60_ISO_BR(NOTED1_60,     BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NOTED2]      = KEYMAP_60_ISO_BR(NOTED2_60,     BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NOTED3]      = KEYMAP_60_ISO_BR(NOTED3_60,     BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD)),
    [_NOTED4]      = KEYMAP_60_ISO_BR(NOTED4_60,     BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD)),
    [_FN]          = KEYMAP_60_ISO_BR(FN60,          BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD)),
    [_SYS]         = KEYMAP_60_ISO_BR(SYS60,         BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD)),
    [_RGB]         = KEYMAP_60_ISO_BR(RGB60,         BR8_POS_1_3_4_5_8(RGB60,         KC_LGUI, KC_RGUI, QK_LEAD)),
};

#include "tap_dance_bindings.inc"

// clang-format on
