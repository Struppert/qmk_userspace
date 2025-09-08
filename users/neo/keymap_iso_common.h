#pragma once
#include QMK_KEYBOARD_H
#include "eeconfig.h" // EEPROM-API
#include "keymap_extras/keymap_german.h"
#include "process_unicode.h"
#include "quantum.h"

#include "os_state.h"

#ifdef TAP_DANCE_ENABLE
#include "process_tap_dance.h"
#endif

/* ---------- Leader sauber einbinden ---------- */
#if defined(LEADER_ENABLE) && defined(KEYMAP_IMPLEMENTATION)
#include "process_leader.h"
LEADER_EXTERNS();
#endif

#ifndef LAYOUT_ISO
#error                                                                         \
    "Define LAYOUT_ISO before including keymap_iso_common.h (e.g., #define LAYOUT_ISO LAYOUT_iso_62)"
#endif

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

// ── Eigene Keycodes (Unicode-Mode-Steuerung) ─────────────────────────────────
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

// Mod-/Layer-Aliase
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

#ifdef COMBO_ENABLE
#include "process_combo.h"
combo_t key_combos[] PROGMEM = {};
#endif

#ifdef TAP_DANCE_ENABLE
/* ── IDs ─────────────────────────────────────────────────────────────────── */
enum {
  TD_CAPS_SYS_LEAD =
      0,              // Base Caps: Hold = _SYS, Tap = ESC, Double-Tap = Leader
  TD_CAPS_NEO_LEAD,   // Neo Caps : Hold = _NEO3, Tap = ESC, Double-Tap = Leader
  TD_CAPS_NOTED_LEAD, // Noted Caps: Hold = _NOTED3, Tap = ESC, Double-Tap =
                      // Leader
  TD_NEO_L3MO_LEAD,   // Neo: Hold = _NEO3, Double-Tap = Leader
  TD_NOTED_L3MO_LEAD, // Noted: Hold = _NOTED3, Double-Tap = Leader
  TD_TEST_A_LEAD,     // Test: Tap = "a", Double-Tap = Leader
};

/* ── Tap-Dance: getrennte Callbacks pro Aktion (ohne user_data) ──────────── */
// static void td_lead_if_double(tap_dance_state_t *s) {
//   if (s->count >= 2 && !s->pressed) {
//     leader_start();
//     reset_tap_dance(s);
//   }
// }

/* Test A / Leader */
static void td_test_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  tap_code16(KC_A);
}
static void td_test_reset(tap_dance_state_t *s, void *ud) {}

/* CAPS: SYS */
static bool td_caps_sys_on = false;
static void td_caps_sys_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  if (s->pressed) {
    layer_on(_SYS);
    td_caps_sys_on = true;
  } else {
    tap_code16(KC_ESC);
  }
}
static void td_caps_sys_reset(tap_dance_state_t *s, void *ud) {
  if (td_caps_sys_on) {
    layer_off(_SYS);
    td_caps_sys_on = false;
  }
}

/* CAPS: NEO */
static bool td_caps_neo_on = false;
static void td_caps_neo_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  if (s->pressed) {
    layer_on(_NEOQWERTZ3);
    td_caps_neo_on = true;
  } else {
    tap_code16(KC_ESC);
  }
}
static void td_caps_neo_reset(tap_dance_state_t *s, void *ud) {
  if (td_caps_neo_on) {
    layer_off(_NEOQWERTZ3);
    td_caps_neo_on = false;
  }
}

/* CAPS: NOTED */
static bool td_caps_noted_on = false;
static void td_caps_noted_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  if (s->pressed) {
    layer_on(_NOTED3);
    td_caps_noted_on = true;
  } else {
    tap_code16(KC_ESC);
  }
}
static void td_caps_noted_reset(tap_dance_state_t *s, void *ud) {
  if (td_caps_noted_on) {
    layer_off(_NOTED3);
    td_caps_noted_on = false;
  }
}

/* L3 MO: NEO */
static bool td_neo_l3mo_on = false;
static void td_neo_l3mo_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  if (s->pressed) {
    layer_on(_NEOQWERTZ3);
    td_neo_l3mo_on = true;
  }
}
static void td_neo_l3mo_reset(tap_dance_state_t *s, void *ud) {
  if (td_neo_l3mo_on) {
    layer_off(_NEOQWERTZ3);
    td_neo_l3mo_on = false;
  }
}

/* L3 MO: NOTED */
static bool td_noted_l3mo_on = false;
static void td_noted_l3mo_finished(tap_dance_state_t *s, void *ud) {
  if (s->count >= 2 && !s->pressed) {
    leader_start();
    reset_tap_dance(s);
    return;
  }
  if (s->pressed) {
    layer_on(_NOTED3);
    td_noted_l3mo_on = true;
  }
}
static void td_noted_l3mo_reset(tap_dance_state_t *s, void *ud) {
  if (td_noted_l3mo_on) {
    layer_off(_NOTED3);
    td_noted_l3mo_on = false;
  }
}

/* ── Registrierung (ohne .fn-Feld, kompatibel) ───────────────────────────── */
tap_dance_action_t PROGMEM tap_dance_actions[] = {
    [TD_TEST_A_LEAD] =
        ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_test_finished, td_test_reset),
    [TD_CAPS_SYS_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, td_caps_sys_finished, td_caps_sys_reset),
    [TD_CAPS_NEO_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, td_caps_neo_finished, td_caps_neo_reset),
    [TD_CAPS_NOTED_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, td_caps_noted_finished, td_caps_noted_reset),
    [TD_NEO_L3MO_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, td_neo_l3mo_finished, td_neo_l3mo_reset),
    [TD_NOTED_L3MO_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(
        NULL, td_noted_l3mo_finished, td_noted_l3mo_reset),
};
#endif /* TAP_DANCE_ENABLE */

// clang-format off
// ──────────────────────────────────────────────────────────────────────────────
// Keymaps (DEINE vollständigen Layer aus deiner letzten Version)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ───────── Layer 0: BASE (QWERTZ, ISO) ─────────
  [_QWERTZ] = LAYOUT_ISO(
    /* Row1 */ QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_ACUT, KC_BSPC,
    /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
    /* Row3 */ TD(TD_CAPS_SYS_LEAD), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,
    /* Row4 */ KC_LSFT, DE_LABK, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, TD(TD_TEST_A_LEAD), QK_LEAD, KC_RCTL),

  // ───────── Layer 1: NeoQwertz Ebene 1 (klein) ─────────
  [_NEOQWERTZ1] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_EQL, KC_BSPC,
    /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
    /* Row3 */ TD(TD_CAPS_NEO_LEAD), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, TD(TD_NEO_L3MO_LEAD), KC_ENT,
    /* Row4 */ NEO_SHIFT, L4_MO_NEO, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, NEO_SHIFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_1, KC_RCTL),

  // ───────── Layer 2: NeoQwertz Ebene 2 (groß + Typografie) ─────────
  [_NEOQWERTZ2] = LAYOUT_ISO(
    /* Row1 */ KC_TRNS,
    DE_DEG, DE_SECT, UC(0x2113), UC(0x00BB), UC(0x00AB), DE_DLR, RALT(KC_E), UC(0x201E), UC(0x201C), UC(0x201D), UC(0x1E9E), DE_GRV, KC_BSPC,
    /* Row2 */ KC_TRNS, S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T), S(DE_Z), S(KC_U), S(KC_I), S(KC_O), S(KC_P), S(DE_UDIA), DE_ASTR,
    /* Row3 */ TD(TD_CAPS_NEO_LEAD), S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), S(DE_ODIA), S(DE_ADIA), TD(TD_NEO_L3MO_LEAD), S(KC_ENT),
    /* Row4 */ KC_TRNS, L4_MO_NEO, S(DE_Y), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), DE_SCLN, DE_COLN, DE_UNDS, KC_TRNS,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_2, KC_RCTL),

  // ───────── Layer 3: NeoQwertz Ebene 3 (Programmieren) ─────────
  [_NEOQWERTZ3] = LAYOUT_ISO(
    UC(0x21BB), UC(0x00B9), UC(0x00B2), UC(0x00B3), UC(0x203A), UC(0x2039), UC(0x00A2), UC(0x00A5), UC(0x201A), UC(0x2018), UC(0x2019), DE_MINS, UC(0x00B7), KC_DEL,
    KC_NO, DE_AT, DE_UNDS, DE_LBRC, DE_RBRC, DE_CIRC, DE_EXLM, DE_LABK, DE_RABK, DE_EQL, DE_AMPR, DE_DLR, DE_TILD,
    TD(TD_CAPS_NEO_LEAD), DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, DE_AT, TD(TD_NEO_L3MO_LEAD), KC_ENT,
    KC_LSFT, L4_MO_NEO, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_3, KC_RCTL),

  // ───────── Layer 4: NeoQwertz Ebene 4 (Navi/Numblock & Extras) ─────────
  [_NEOQWERTZ4] = LAYOUT_ISO(
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_SLSH, KC_ASTR, KC_MINS,
    KC_TAB, KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN, KC_DOWN, KC_P7, KC_P8, KC_P9, KC_PPLS, KC_PMNS, KC_ENT,
    KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_INS, KC_P4, KC_P5, KC_P6, KC_COMM, KC_DOT, KC_NO, L4_MO_NEO, KC_ENT,
    KC_LSFT, L4_MO_NEO, KC_ESC, KC_TAB, KC_INS, KC_ENT, LCTL(KC_Z), KC_P1, KC_P2, KC_P3, KC_NO, KC_NO, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, KC_P0, KC_RALT, KC_RGUI, KC_4, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 1 (klein)
  [_NOTED1] = LAYOUT_ISO(
    SYS_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_MINS, DE_ACUT, KC_BSPC,
    KC_TAB,  DE_Z, DE_Y, KC_U, KC_A, KC_Q, KC_P, KC_B, KC_M, KC_L, KC_F, KC_J, DE_SS,
    TD(TD_CAPS_NOTED_LEAD), KC_C, KC_S, KC_I, KC_E, KC_O, KC_D, KC_T, KC_N, KC_R, KC_H, TD(TD_NOTED_L3MO_LEAD), DE_ACUT, KC_ENT,
    NOTED_SHIFT, L4_MO_NOTED, KC_V, KC_X, DE_UDIA, DE_ADIA, DE_ODIA, KC_W, KC_G, DE_COMM, DE_DOT, KC_K, NOTED_SHIFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NOTED, KC_RGUI, KC_5, KC_RCTL),

  // NOTED Ebene 2 (Shift)
  [_NOTED2] = LAYOUT_ISO(
    KC_TRNS, DE_DEG, DE_SECT, UC(0x2113), UC(0x00BB), UC(0x00AB), DE_DLR, RALT(KC_E), UC(0x201E), UC(0x201C), UC(0x201D), UC(0x1E9E), DE_GRV, KC_BSPC,
    KC_TAB,  S(DE_Z), S(DE_Y), S(KC_U), S(KC_A), S(KC_Q), S(KC_P), S(KC_B), S(KC_M), S(KC_L), S(KC_F), S(KC_J), DE_SS,
    TD(TD_CAPS_NOTED_LEAD), S(KC_C), S(KC_S), S(KC_I), S(KC_E), S(KC_O), S(KC_D), S(KC_T), S(KC_N), S(KC_R), S(KC_H), TD(TD_NOTED_L3MO_LEAD), DE_ACUT, KC_ENT,
    NOTED_SHIFT, L4_MO_NOTED, S(KC_V), S(KC_X), S(DE_UDIA), S(DE_ADIA), S(DE_ODIA), S(KC_W), S(KC_G), DE_MINS, KC_DOT, S(KC_K), NOTED_SHIFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NOTED, KC_RGUI, KC_6, KC_RCTL),

  // NOTED Ebene 3
  [_NOTED3] = LAYOUT_ISO(
    UC(0x21BB), UC(0x00B9), UC(0x00B2), UC(0x00B3), UC(0x203A), UC(0x2039), UC(0x00A2), UC(0x00A5), UC(0x201A), UC(0x2018), UC(0x2019), DE_MINS, UC(0x00B7), KC_DEL,
    KC_NO, DE_AT, DE_UNDS, DE_LBRC, DE_RBRC, DE_CIRC, DE_EXLM, DE_LABK, DE_RABK, DE_EQL, DE_AMPR, DE_AT, DE_TILD,
    TD(TD_CAPS_NOTED_LEAD), DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, TD(TD_NOTED_L3MO_LEAD), DE_AT, KC_ENT,
    KC_LSFT, L4_MO_NOTED, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_7, KC_RCTL),

  // NOTED Ebene 4
  [_NOTED4] = LAYOUT_ISO(
    SYS_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS,
    KC_TAB,  KC_ESC, KC_HOME, KC_UP, KC_END, KC_PGUP, KC_PSCR, KC_INS, KC_DEL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PAUS,
    SYS_ESC, KC_CALC, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_SCRL, KC_PAUS, KC_BSPC, KC_TAB, KC_ENT,
    KC_LSFT, KC_NUM, KC_PSLS, KC_P7, KC_P8, KC_P9, KC_PPLS, L3_MO_NOTED, KC_P4, KC_P5, KC_P6, KC_RSFT, KC_UP,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_8, KC_RCTL),

  /* ───────── Layer 5: FN ───────── */
  [_FN] = LAYOUT_ISO(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
    KC_TAB, QK_MACRO_6, QK_MACRO_4, QK_MACRO_2, QK_MACRO_0, DM_PLY1, KC_NO, DM_REC1, DM_REC2, DM_RSTP, KC_NO, KC_NO, KC_NO,
    KC_ESC, QK_MACRO_7, QK_MACRO_5, QK_MACRO_3, QK_MACRO_1, DM_PLY2, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_ENT,
    KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MO, SYS_MO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_9, KC_RCTL),

  // ───────── Layer 6: SYS ─────────
  [_SYS] = LAYOUT_ISO(
    KC_ESC, QK_BOOT, EE_CLR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_CAPS, UC_MODE_CYCLE, UC_SET_LNX, UC_SET_WIN, UC_SET_WINC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT,
    KC_LSFT, KC_OS_CYCLE, KC_OS_WIN, KC_OS_LNX, KC_OS_MAC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_A, KC_RCTL),

  // ───────── Layer 7: RGB ─────────
  [_RGB] = LAYOUT_ISO(
    KC_ESC, QK_RGB_MATRIX_TOGGLE, QK_RGB_MATRIX_MODE_NEXT, QK_RGB_MATRIX_MODE_PREVIOUS, QK_RGB_MATRIX_VALUE_UP, QK_RGB_MATRIX_VALUE_DOWN, QK_RGB_MATRIX_SPEED_UP, QK_RGB_MATRIX_SPEED_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    KC_TAB, QK_RGB_MATRIX_HUE_UP, QK_RGB_MATRIX_HUE_DOWN, QK_RGB_MATRIX_SATURATION_UP, QK_RGB_MATRIX_SATURATION_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT,
    KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_7, KC_RCTL)
};
// clang-format on

/* Tap/Hold Tuning */
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
  return false;
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
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
  return false;
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
  return keycode == SP_FN;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
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
