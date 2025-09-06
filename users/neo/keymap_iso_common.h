#pragma once
#include QMK_KEYBOARD_H
#include "keymap_extras/keymap_german.h"
#include "process_unicode.h"
#include "quantum.h"

#ifndef LAYOUT_ISO
#error                                                                         \
    "Define LAYOUT_ISO before including keymap_iso_common.h (e.g., #define LAYOUT_ISO LAYOUT_iso_62)"
#endif

enum layers {
  _QWERTZ = 0,
  _NEOQWERTZ1,
  _NEOQWERTZ2,
  _NEO3,
  _NEO4,
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
};

// Mod-/Layer-Aliase
#define SYS_ESC LT(_SYS, KC_ESC)
#define SP_FN LT(_FN, KC_SPC)
#define SYS_MO MO(_SYS)

#define L3_ESC_NEO LT(_NEO3, KC_ESC)
#define L3_MO_NEO MO(_NEO3)
#define L4_MO_NEO MO(_NEO4)
#define NEO_SHIFT MO(_NEOQWERTZ2)

#define L3_ESC_NOTED LT(_NOTED3, KC_ESC)
#define L3_MO_NOTED MO(_NOTED3)
#define L4_MO_NOTED MO(_NOTED4)
#define NOTED_SHIFT MO(_NOTED2)

#ifdef COMBO_ENABLE
#include "process_combo.h"
const combo_t key_combos[] PROGMEM = {};
#endif

#ifdef TAP_DANCE_ENABLE
qk_tap_dance_action_t tap_dance_actions[] = {};
#endif

// ── Beim Start Linux-Unicode ─────────────────────────────────────────────────
void keyboard_post_init_user(void) {
  set_unicode_input_mode(UNICODE_MODE_LINUX);
}

// clang-format off
// ──────────────────────────────────────────────────────────────────────────────
// Keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // ───────── Layer 0: BASE (QWERTZ, ISO) ─────────
  [_QWERTZ] = LAYOUT_ISO(
    /* Row1 */ QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_ACUT, KC_BSPC,
    /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
    /* Row3 */ SYS_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,
    /* Row4 */ KC_LSFT, DE_LABK, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_0, KC_RCTL),

  // ───────── Layer 1: NeoQwertz Ebene 1 (klein) ─────────
  [_NEOQWERTZ1] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_EQL, KC_BSPC,
    /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
    /* Row3 */ L3_ESC_NEO, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, L3_MO_NEO, KC_ENT,
    /* Row4 */ NEO_SHIFT, L4_MO_NEO, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, NEO_SHIFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_1, KC_RCTL),

  // ───────── Layer 2: NeoQwertz Ebene 2 (groß + Typografie) ─────────
  // Buchstaben explizit "shifted"
  [_NEOQWERTZ2] = LAYOUT_ISO(
    /* Row1 */ KC_TRNS,
    DE_DEG,           // °  (Shift+1)        — Neo: Degree auf 1
    DE_SECT,          // §  (Shift+2)
    UC(0x2113),       // ℓ  (Shift+3)
    UC(0x00BB),       // »  (Shift+4)
    UC(0x00AB),       // «  (Shift+5)
    DE_DLR,           // $  (Shift+6)
    RALT(KC_E),       // €  (Shift+7)        — ohne Unicode, wie gewünscht
    UC(0x201E),       // „  (Shift+8)
    UC(0x201C),       // “  (Shift+9)
    UC(0x201D),       // ”  (Shift+0)
    UC(0x1E9E),       // ẞ  (Shift+ß)
    DE_GRV,           // `  (Shift+´)        — wie Standard-DE
    KC_BSPC,
    /* Row2 */ KC_TRNS, S(KC_Q), S(KC_W), S(KC_E) /*€*/, S(KC_R), S(KC_T), S(DE_Z), S(KC_U), S(KC_I), S(KC_O), S(KC_P), S(DE_UDIA), DE_ASTR,
    /* Row3 */ L3_ESC_NEO, S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), S(DE_UDIA), S(DE_ADIA), L3_MO_NEO, S(KC_ENT),
    /* Row4 */ KC_TRNS, L4_MO_NEO, S(DE_Y), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), DE_SCLN, DE_COLN, DE_UNDS, KC_TRNS,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_2, KC_RCTL),

  // ───────── Layer 3: NeoQwertz Ebene 3 (Programmieren) ─────────
  [_NEO3] = LAYOUT_ISO(
    /* Row1 (Ziffernreihe ohne Ziffern) */
    UC(0x21BB), /* ^  → ↻  („turning“/Drehtaste) */
    UC(0x00B9), /* 1  → ¹ */
    UC(0x00B2), /* 2  → ² */
    UC(0x00B3), /* 3  → ³ */
    UC(0x203A), /* 4  → › */
    UC(0x2039), /* 5  → ‹ */
    UC(0x00A2), /* 6  → ¢ */
    UC(0x00A5), /* 7  → ¥ */
    UC(0x201A), /* 8  → ‚ */
    UC(0x2018), /* 9  → ‘ */
    UC(0x2019), /* 0  → ’ (Apostroph) */
    DE_MINS,    /* ß → -  (Hyphen als Fallback, leicht erreichbar) */
    UC(0x00B7), /* ´ → ·  (Mittelpunkt) */
    KC_DEL,

    /* Row2 (Q W E R T  Z  U  I  O  P  Ü   +) – Programmierzeichen */
    KC_NO, DE_AT, DE_UNDS, DE_LBRC, DE_RBRC, DE_CIRC, DE_EXLM, DE_LABK, DE_RABK, DE_EQL, DE_AMPR, DE_DLR, DE_TILD,
    /* Row3 (Caps A S D F G  H  J  K  L  Ö   Ä   #) */
    L3_ESC_NEO, DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, DE_AT, L3_MO_NEO, KC_ENT,
    /* Row4 (<>  Y  X  C  V  B   N   M   ,    .     -) */
    KC_LSFT, L4_MO_NEO, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,
    /* Row5 (Mods/Space-FN bleiben nutzbar, nichts transparent) */
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_3, KC_RCTL),

  // ───────── Layer 4: NeoQwertz Ebene 4 (Navi/Numblock & Extras) ─────────
  [_NEO4] = LAYOUT_ISO(
    /* Row1 (oper./Funktionen rechts, Rest neutral) — 14 Keys */
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_SLSH, KC_ASTR, KC_MINS,
    /* Row2 (Q W E R T  Z   U   I   O   P   Ü   +) — 13 Keys */
    KC_TAB, KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN, KC_DOWN, KC_P7, KC_P8, KC_P9, KC_PPLS, KC_PMNS, KC_ENT,
    /* Row3 (A S D F G  H   J   K   L   Ö    Ä    #  +Enter) — 14 Keys */
    KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_INS, KC_P4, KC_P5, KC_P6, KC_COMM, KC_DOT, KC_NO, L4_MO_NEO, KC_ENT,
    /* Row4 (Shift  <>  Y  X  C  V  B  N  M  ,   .   -  Shift) — 13 Keys */
    KC_LSFT, L4_MO_NEO, KC_ESC, KC_TAB, KC_INS, KC_ENT, LCTL(KC_Z), KC_P1, KC_P2, KC_P3, KC_NO, KC_NO, KC_RSFT,
    /* Row5 — 8 Keys */
    KC_LCTL, KC_LGUI, KC_LALT, KC_P0, KC_RALT, KC_RGUI, KC_4, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 1 (klein) — laut noted(noted_base)
  [_NOTED1] = LAYOUT_ISO(
    /* R1(14) */ SYS_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_MINS,  DE_ACUT, KC_BSPC,
    /* R2(13) */ KC_TAB,  DE_Z, DE_Y, KC_U, KC_A, KC_Q, KC_P, KC_B, KC_M, KC_L, KC_F, KC_J, DE_SS,
    /* R3(14) */ L3_ESC_NOTED , KC_C, KC_S, KC_I, KC_E, KC_O, KC_D, KC_T, KC_N, KC_R, KC_H, L3_MO_NOTED, DE_ACUT, KC_ENT,
    /* R4(13) */ NOTED_SHIFT, L4_MO_NOTED, KC_V, KC_X, DE_UDIA, DE_ADIA, DE_ODIA, KC_W, KC_G, DE_COMM, DE_DOT, KC_K, NOTED_SHIFT,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NOTED, KC_RGUI, KC_5, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 2 (Shift) — korrespondierend zu Ebene 1
  [_NOTED2] = LAYOUT_ISO(
    /* R1(14) */ SYS_ESC, KC_EXLM, KC_AT,  KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, DE_UNDS, DE_ACUT, KC_BSPC,
    /* R2(13) */ KC_TAB,  S(DE_Z),    S(DE_Y),   S(KC_U),    S(KC_A),    S(KC_Q),    S(KC_P), S(KC_B), S(KC_M), S(KC_L), S(KC_F), S(KC_J), DE_SS,
    /* R3(14) */ L3_ESC_NOTED, S(KC_C),    S(KC_S),   S(KC_I),    S(KC_E),    S(KC_O),    S(KC_D), S(KC_T), S(KC_N), S(KC_R), S(KC_H), L3_MO_NOTED, DE_ACUT, KC_ENT,
    /* R4(13) */ NOTED_SHIFT, L4_MO_NOTED, S(KC_V),   S(KC_X),   S(DE_UDIA), S(DE_ADIA), S(DE_ODIA), S(KC_W), S(KC_G), DE_MINS, KC_DOT, S(KC_K), NOTED_SHIFT,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN,   L4_MO_NOTED,  KC_RGUI, KC_6, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 3: 1:1 aus _NEO3 übernehmen, NUR die zwei Felder in R4 tauschen
  [_NOTED3] = LAYOUT_ISO(
    /* R1(14) */ SYS_ESC, KC_PLUS, KC_LBRC, KC_RBRC, KC_SLSH, KC_EQL,  KC_ASTR, KC_LCBR, KC_RCBR, KC_LABK, KC_RABK, KC_BSLS, KC_GRV,  KC_DEL,
    /* R2(13) */ KC_TAB,  KC_AT,   KC_DQUO, KC_LPRN, KC_RPRN, KC_EXLM, KC_QUOT, KC_HASH, KC_PERC, KC_MINS, KC_COLN, KC_SCLN, KC_PIPE,
    /* R3(14) */ SYS_ESC, KC_CIRC, KC_QUES, KC_LABK, KC_RABK, KC_TILD, KC_AMPR, KC_UNDS, DE_EURO, KC_PPLS, KC_EQL,  DE_ACUT,  KC_BSPC, KC_ENT,
    /* R4(13) */ KC_LSFT, KC_DLR,  KC_DOT, KC_PGUP, KC_PGDN, KC_HOME, KC_END,  L3_MO_NOTED,     KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN,   KC_RALT, KC_RGUI, KC_7, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 4: 1:1 aus _NEO4 übernehmen, NUR die zwei Felder in R4 tauschen
  [_NOTED4] = LAYOUT_ISO(
    /* R1(14) */ SYS_ESC, KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, KC_INS,
    /* R2(13) */ KC_TAB,  KC_ESC, KC_HOME,KC_UP,  KC_END, KC_PGUP, KC_PSCR,KC_INS, KC_DEL, KC_LEFT,KC_DOWN,KC_RGHT, KC_PAUS,
    /* R3(14) */ SYS_ESC, KC_CALC,KC_MPRV,KC_MPLY,KC_MNXT,KC_MUTE, KC_VOLD,KC_VOLU,KC_PSCR,KC_SCRL,KC_PAUS, KC_BSPC, KC_TAB,  KC_ENT,
    /* R4(13) */ KC_LSFT, KC_NUM, KC_PSLS,KC_P7,  KC_P8,  KC_P9,  KC_PPLS, L3_MO_NOTED,    KC_P4,  KC_P5,  KC_P6,  KC_RSFT, KC_UP,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_8, KC_RCTL),

  /* ───────── Layer 5: FN (Makros/F-Tasten + EE_CLR rechts unten) ───────── */
  [_FN] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
    /* Row2 */ KC_TAB, QK_MACRO_6, QK_MACRO_4, QK_MACRO_2, QK_MACRO_0, DM_PLY1, KC_NO, DM_REC1, DM_REC2, DM_RSTP, KC_NO, KC_NO, KC_NO,
    /* Row3 */ KC_ESC, QK_MACRO_7, QK_MACRO_5, QK_MACRO_3, QK_MACRO_1, DM_PLY2, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_ENT,
    /* Row4 */ KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, MO(_RGB), MO(_SYS), KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_5, KC_RCTL),

  // ───────── Layer 6: SYS (Service/Bootloader) ─────────
  [_SYS] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, QK_BOOT, EE_CLR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    /* Row2 */ KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    /* Row3 */ KC_CAPS, UC_MODE_CYCLE, UC_SET_LNX, UC_SET_WIN, UC_SET_WINC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, L3_MO, KC_ENT,
    /* Row4 */ KC_LSFT, L4_MO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_6, KC_RCTL
  ),

  // ───────── Layer 7: RGB (alle RGB-Matrix Controls) ─────────
  [_RGB] = LAYOUT_ISO(
    /* Row1 */ KC_ESC,
    QK_RGB_MATRIX_TOGGLE,
    QK_RGB_MATRIX_MODE_NEXT,
    QK_RGB_MATRIX_MODE_PREVIOUS,
    QK_RGB_MATRIX_VALUE_UP,
    QK_RGB_MATRIX_VALUE_DOWN,
    QK_RGB_MATRIX_SPEED_UP,
    QK_RGB_MATRIX_SPEED_DOWN,
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,

    /* Row2 */ KC_TAB,
    QK_RGB_MATRIX_HUE_UP,
    QK_RGB_MATRIX_HUE_DOWN,
    QK_RGB_MATRIX_SATURATION_UP,
    QK_RGB_MATRIX_SATURATION_DOWN,
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    /* Row3 */ KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT,
    /* Row4 */ KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_7, KC_RCTL
  ),
}; // end keymaps
// clang-format on

/* Tap/Hold Tuning */
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  // Keine force-hold-Keys, wenn Tap-first gewünscht ist:
  default:
    return false;
  }
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
  switch (keycode) {
  // Keine hold-on-interrupt-Keys bei Tap-first:
  default:
    return false;
  }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
  return keycode == SP_FN; // nur Space bekommt Retro-Tapping
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

static void set_uc_and_optionally_feedback(uint8_t mode) {
  set_unicode_input_mode(mode);
#ifdef RGB_MATRIX_ENABLE
  uint8_t r = 0, g = 0, b = 0;
  switch (mode) {
  case UNICODE_MODE_LINUX:
    g = 20;
    break; /* grün */
  case UNICODE_MODE_WINDOWS:
    r = 20;
    break; /* rot */
  case UNICODE_MODE_WINCOMPOSE:
    r = 20;
    b = 20;
    break; /* magenta */
  default:
    b = 20;
    break;
  }
  rgb_matrix_set_color_all(r, g, b);
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed)
    return true;
  switch (keycode) {
  case UC_SET_LNX:
    set_uc_and_optionally_feedback(UNICODE_MODE_LINUX);
    return false;
  case UC_SET_WIN:
    set_uc_and_optionally_feedback(UNICODE_MODE_WINDOWS);
    return false;
  case UC_SET_WINC:
    set_uc_and_optionally_feedback(UNICODE_MODE_WINCOMPOSE);
    return false;
  case UC_MODE_CYCLE: {
    uint8_t m = get_unicode_input_mode();
    if (m == UNICODE_MODE_LINUX)
      set_uc_and_optionally_feedback(UNICODE_MODE_WINDOWS);
    else if (m == UNICODE_MODE_WINDOWS)
      set_uc_and_optionally_feedback(UNICODE_MODE_WINCOMPOSE);
    else
      set_uc_and_optionally_feedback(UNICODE_MODE_LINUX);
    return false;
  }
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
  case _QWERTZ:
    rgb_matrix_set_color_all(0, 0, 0);
    break; // aus
  case _NEOQWERTZ1:
    rgb_matrix_set_color_all(0, 20, 0);
    break; // grün
  case _NEOQWERTZ2:
    rgb_matrix_set_color_all(20, 20, 0);
    break; // gelb
  case _NEO3:
    rgb_matrix_set_color_all(20, 0, 20);
    break; // magenta
  case _NEO4:
    rgb_matrix_set_color_all(0, 0, 20);
    break; // blau
  case _FN:
    rgb_matrix_set_color_all(0, 20, 20);
    break; // cyan
  case _SYS:
    rgb_matrix_set_color_all(20, 0, 0);
    break; // rot
  case _RGB:
    rgb_matrix_set_color_all(20, 20, 20);
    break; // weiß
  }
  return state;
}
