#pragma once
// Keep the header lightweight: only what's needed by users of this module
#include "keymap_extras/keymap_german.h"
#include "os_state.h" // your OS state helpers
#include "process_unicode.h"
#include "quantum.h" // MATRIX_*, keycodes, layer_state_t

#ifdef __cplusplus
extern "C" {
#endif

// ── Layers (public, used by other translation units) ───────────────────────
#ifndef V1_MINIMAL_ENUM
enum layers {
  _QWERTZ = 0,
  _NEOQWERTZ1,
  _NEOQWERTZ2,
  _NEOQWERTZ3,
  _NEOQWERTZ4,
  _NOTED1,
  _NOTED2,
  _NOTED3,
  _NOTED4,
  _FN,
  _SYS,
  _RGB,
};
#else
enum layers {
  _QWERTZ = 0,
  _NEOQWERTZ1,
  _NEOQWERTZ2,
  _NEOQWERTZ3,
  _NEOQWERTZ4,
  _FN,
  _SYS,
  _RGB,
};
#endif

#ifdef V1_MINIMAL_ENUM
// Wenn NOTED-Layer fehlen, mappe sie auf vorhandene:
#ifndef _NOTED1
#define _NOTED1 _NEOQWERTZ2
#define _NOTED2 _NEOQWERTZ2
#define _NOTED3 _NEOQWERTZ3
#define _NOTED4 _NEOQWERTZ4
#endif
#endif

// ── Custom keycodes (public) ───────────────────────────────────────────────
enum custom_keycodes {
  UC_SET_LNX = SAFE_RANGE,
  UC_SET_WIN,
  UC_SET_WINC,
  UC_MODE_CYCLE,
  KC_OS_WIN,
  KC_OS_LNX,
  KC_OS_MAC,
  KC_OS_CYCLE,
};

// ── Aliases used across keymaps (public) ───────────────────────────────────
#define SYS_ESC LT(_SYS, KC_ESC)
#define SP_FN LT(_FN, KC_SPC)
#define SYS_MO MO(_SYS)
#define RGB_MO MO(_RGB)

#define L3_ESC_NEO LT(_NEOQWERTZ3, KC_ESC)
#define L3_MO_NEO MO(_NEOQWERTZ3)
#define L4_MO_NEO MO(_NEOQWERTZ4)
#define NEO_SHIFT MO(_NEOQWERTZ2)

#define L3_ESC_NOTED LT(_NOTED3, KC_ESC)
#define L3_MO_NOTED MO(_NOTED3)
#define L4_MO_NOTED MO(_NOTED4)
#define NOTED_SHIFT MO(_NOTED2)

// ── Externs (definition lives in .c) ───────────────────────────────────────
extern const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS];

// ── QMK hooks implemented in .c (declared here for clarity) ────────────────
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record);
bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record);
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record);
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record);
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record);
bool process_record_user(uint16_t keycode, keyrecord_t *record);
layer_state_t layer_state_set_user(layer_state_t state);

#ifdef __cplusplus
}
#endif
