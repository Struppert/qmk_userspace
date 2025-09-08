// keyboards/keychron/q4/iso/keymaps/neo/keymap.c
#define LAYOUT_ISO LAYOUT_iso_62
#include QMK_KEYBOARD_H
#include "keymap_iso_common.h" // Enums/Aliasse (API)

#include "combos_bindings.inc"   // ← key_combos-Array (immer vorhanden)
#include "keymap_iso_common.inc" // ← deine komplette Matrix (nutzt TD(...))
#include "tap_dance_bindings.inc" // ← enum + tap_dance_actions (compile-time sichtbar)
