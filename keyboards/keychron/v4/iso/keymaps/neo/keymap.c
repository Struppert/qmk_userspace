#include "keymap_iso_common.h"
// keyboards/keychron/v4/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_60_iso
#include QMK_KEYBOARD_H
#include "combos_bindings.inc"
#include "keymap_iso_common.h" // Enums/Aliasse/Prototypen
#include "keymap_iso_common.inc"
#include "tap_dance_bindings.inc" // ← enum + tap_dance_actions (compile-time sichtbar)
