#include QMK_KEYBOARD_H
#include "eeconfig.h" // EEPROM-API
#include "keymap_extras/keymap_german.h"
#include "keymap_iso_common.h" // Enums/Aliasse/Prototypen
#include "os_state.h"
#include "process_unicode.h"
#include "quantum.h"

#ifdef LEADER_ENABLE
#include "process_leader.h"
#endif

/* ───────────────────────────────────────────────────────────────────────────
 * QMK Tap/Hold-Tuning
 * ───────────────────────────────────────────────────────────────────────────
 */
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
  (void)keycode;
  (void)record;
  return false;
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
  (void)record;
  switch (keycode) {
  case L3_ESC_NEO:
  case L3_ESC_NOTED:
  case SYS_ESC:
  case SP_FN:
    return true; // Tap-first
  default:
    return false;
  }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  (void)keycode;
  (void)record;
  return false;
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
  (void)record;
  return keycode == SP_FN;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  (void)record;
  switch (keycode) {
  case L3_ESC_NEO:
  case L3_ESC_NOTED:
  case SYS_ESC:
    return TAPPING_TERM + 30;
  case SP_FN:
    return TAPPING_TERM - 100;
  default:
    return TAPPING_TERM;
  }
}

/* ───────────────────────────────────────────────────────────────────────────
 * process_record_user: OS-/Unicode-Handling
 * ───────────────────────────────────────────────────────────────────────────
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed)
    return true;

  switch (keycode) {
  case KC_OS_WIN:
    os_set(OS_WIN);
    return false;
  case KC_OS_LNX:
    os_set(OS_LNX);
    return false;
  case KC_OS_MAC:
    os_set(OS_MAC);
    return false;
  case KC_OS_CYCLE:
    os_cycle();
    return false;

  case UC_SET_LNX:
    uc_set_and_feedback(UNICODE_MODE_LINUX);
    return false;
  case UC_SET_WIN:
    uc_set_and_feedback(UNICODE_MODE_WINDOWS);
    return false;
  case UC_SET_WINC:
    uc_set_and_feedback(UNICODE_MODE_WINCOMPOSE);
    return false;

  case UC_MODE_CYCLE: {
    uint8_t m = get_unicode_input_mode();
    if (m == UNICODE_MODE_LINUX)
      uc_set_and_feedback(UNICODE_MODE_WINDOWS);
    else if (m == UNICODE_MODE_WINDOWS)
      uc_set_and_feedback(UNICODE_MODE_WINCOMPOSE);
    else
      uc_set_and_feedback(UNICODE_MODE_LINUX);
    return false;
  }
  }
  return true;
}

/* ───────────────────────────────────────────────────────────────────────────
 * layer_state_set_user: RGB-Layerfarben
 * ───────────────────────────────────────────────────────────────────────────
 */
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGB_MATRIX_ENABLE
  switch (get_highest_layer(state)) {
  case _QWERTZ:
    rgb_matrix_set_color_all(20, 20, 20);
    break; // weiß
  case _NEOQWERTZ1:
    rgb_matrix_set_color_all(20, 20, 0);
    break; // gelb
  case _NOTED1:
    rgb_matrix_set_color_all(20, 0, 20);
    break; // magenta
  case _FN:
    rgb_matrix_set_color_all(0, 20, 20);
    break; // cyan
  case _SYS:
    rgb_matrix_set_color_all(20, 0, 0);
    break; // rot
  case _RGB:
    rgb_matrix_set_color_all(0, 20, 0);
    break; // grün
  default:
    rgb_matrix_set_color_all(0, 0, 0);
    break; // aus
  }
#endif
  return state;
}
