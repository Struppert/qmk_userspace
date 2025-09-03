
#include QMK_KEYBOARD_H
#include "keymap_extras/keymap_german.h"
#include "quantum.h"
#include "process_unicode.h"

/* Keychron Q4 ISO — NeoQwertz Ebenen 1–4 + FN + SYS
 * Layer:
 *  0 BASE  (DE-QWERTZ, OS=Deutsch)
 *  1 NEO1  (klein, Default)
 *  2 NEO2  (groß, Typografie: § $ € …)
 *  3 NEO3  (Programmier-Symbole, DE-Aliasse)
 *  4 NEO4  (Navigation & Ziffernblock)
 *  5 FN    (Makros, F-Tasten, EE_CLR)
 *  6 SYS   (Service/Bootloader)
 */

enum layers {
    _BASE = 0, // eine QWERTZ als fallback, kein Sprung in NEO. capslock nach _SYS, space nach _FN
    _NEO1,
    _NEO2,
    _NEO3,
    _NEO4,
    _FN,
    _SYS,
};

// ── NEU: Eigene Keycodes für Unicode-Mode-Steuerung ──────────────────────────
enum custom_keycodes {
    UC_SET_LNX = SAFE_RANGE, // explizit: Linux
    UC_SET_WIN,              // explizit: Windows (Alt+Numpad)
    UC_SET_WINC,             // explizit: Windows (WinCompose)
    UC_MODE_CYCLE,           // zyklisch: LNX → WIN → WINC → LNX
};

// Mod-/Layer-Aliase
#define L3_ESC LT(_NEO3, KC_ESC) // Caps: Tap=Esc, Hold=Neo-3
#define SYS_ESC LT(_SYS, KC_ESC) // Caps: Tap=Esc, Hold=SYS (nur in _BASE genutzt)
#define L3_MO MO(_NEO3)          // ISO-NUHS ('#'): Momentary Neo-3
#define L4_MO MO(_NEO4)          // ISO-NUBS ('<'): Momentary Neo-4
#define SP_FN LT(_FN, KC_SPC)    // Space: Tap=Space, Hold=FN
#define SYS_MO MO(_SYS)
#define LAYOUT_ISO LAYOUT_iso_62
#define NEO_SHIFT MO(_NEO2) // Shift-Ersatz: halten = _NEO2 aktiv

#ifdef COMBO_ENABLE
#    include "process_combo.h"
// Keine Combos definiert → leeres Array genügt
const combo_t key_combos[] PROGMEM = {};
#endif

#ifdef TAP_DANCE_ENABLE
// Keine Tap-Dances definiert → leeres Array genügt
qk_tap_dance_action_t tap_dance_actions[] = {};
#endif

// ── NEU: Beim Start standardmäßig auf Linux-Unicode stellen ──────────────────
void keyboard_post_init_user(void) {
    set_unicode_input_mode(UNICODE_MODE_LINUX);
}

// ──────────────────────────────────────────────────────────────────────────────
// Keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // ───────── Layer 0: BASE (QWERTZ, ISO) ─────────
    [_BASE] = LAYOUT_ISO(
        /* Row1 */ QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_ACUT, KC_BSPC,
        /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
        /* Row3 */ SYS_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,
        /* Row4 */ KC_LSFT, DE_LABK, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, KC_RSFT,
        /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_0, KC_RCTL),

    // ───────── Layer 1: NeoQwertz Ebene 1 (klein) ─────────
    [_NEO1] = LAYOUT_ISO(
        /* Row1 */ KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_EQL, KC_BSPC,
        /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
        /* Row3 */ L3_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, L3_MO, KC_ENT,
        /* Row4 */ NEO_SHIFT, L4_MO, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, NEO_SHIFT,
        /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO, KC_RGUI, KC_1, KC_RCTL),

    // ───────── Layer 2: NeoQwertz Ebene 2 (groß + Typografie) ─────────
    // Buchstaben explizit "shifted", Sonderzeichen wie in deiner Vorlage (z. B. € auf E)
    [_NEO2] = LAYOUT_ISO(
        /* Row1 */ KC_TRNS, KC_EXLM, KC_DQUO, DE_SECT, DE_DLR, KC_PERC, KC_AMPR, RALT(KC_E), KC_LPRN, KC_RPRN, KC_EQL, DE_QUES, DE_ASTR, KC_TRNS,
        /* Row2 */ KC_TRNS, S(KC_Q), S(KC_W), S(KC_E) /*€*/, S(KC_R), S(KC_T), S(DE_Z), S(KC_U), S(KC_I), S(KC_O), S(KC_P), S(DE_UDIA), DE_ASTR,
        /* Row3 */ L3_ESC, S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), S(DE_UDIA), S(DE_ADIA), L3_MO, KC_TRNS,
        /* Row4 */ KC_TRNS, L4_MO, S(KC_Y), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), DE_SCLN, DE_COLN, DE_UNDS, KC_TRNS,
        /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO, KC_RGUI, KC_2, KC_RCTL),

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
        KC_NO, DE_AT, DE_UNDS, DE_LBRC, DE_RBRC, DE_CIRC,
        /* Z */ DE_EXLM, DE_LABK, DE_RABK, DE_EQL, DE_AMPR, DE_DLR, DE_TILD,

        /* Row3 (Caps A S D F G  H  J  K  L  Ö   Ä   #) */
        L3_ESC, DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, DE_AT, L3_MO, KC_ENT,

        /* Row4 (<>  Y  X  C  V  B   N   M   ,    .     -) */
        KC_LSFT, L4_MO, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,

        /* Row5 (Mods/Space-FN bleiben nutzbar, nichts transparent) */
        KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_3, KC_RCTL),

    // ───────── Layer 4: NeoQwertz Ebene 4 (Navi/Numblock & Extras) ─────────
    [_NEO4] = LAYOUT_ISO(
        /* Row1 (oper./Funktionen rechts, Rest neutral) — 14 Keys */
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_SLSH, KC_ASTR, KC_MINS,
        /* Row2 (Q W E R T  Z   U   I   O   P   Ü   +) — 13 Keys */
        KC_TAB, KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN, KC_DOWN, KC_P7, KC_P8, KC_P9, KC_PPLS, KC_PMNS, KC_ENT,
        /* Row3 (A S D F G  H   J   K   L   Ö    Ä    #  +Enter) — 14 Keys */
        KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_INS, KC_P4, KC_P5, KC_P6, KC_COMM, KC_DOT, KC_NO, L4_MO, KC_ENT,
        /* Row4 (Shift  <>  Y  X  C  V  B  N  M  ,   .   -  Shift) — 13 Keys */
        KC_LSFT, L4_MO, KC_ESC, KC_TAB, KC_INS, KC_ENT, LCTL(KC_Z), KC_P1, KC_P2, KC_P3, KC_NO, KC_NO, KC_RSFT,
        /* Row5 — 8 Keys */
        KC_LCTL, KC_LGUI, KC_LALT, KC_P0, KC_RALT, KC_RGUI, KC_4, KC_RCTL),

    /* ───────── Layer 5: FN (Makros/F-Tasten + EE_CLR rechts unten) ───────── */
    [_FN] = LAYOUT_ISO(
        /* Row1 */ KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        /* Row2 */ KC_TAB, QK_MACRO_6, QK_MACRO_4, QK_MACRO_2, QK_MACRO_0, UC_MODE_CYCLE, UC_SET_LNX, UC_SET_WIN, UC_SET_WINC, _______, _______, _______, _______,
        /* Row3 */ L3_ESC, QK_MACRO_7, QK_MACRO_5, QK_MACRO_3, QK_MACRO_1, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______, L3_MO, KC_ENT,
        /* Row4 */ KC_LSFT, L4_MO, SYS_MO, EE_CLR, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,
        /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO, KC_RGUI, KC_5, KC_RCTL),

    // ───────── Layer 6: SYS (Service/Bootloader) ─────────
    [_SYS] = LAYOUT_ISO(
        /* Row1 */ KC_ESC, DF(_BASE), DF(_NEO1), DF(_NEO2), DF(_NEO3), DF(_NEO4), QK_BOOT, EE_CLR, QK_RGB_MATRIX_TOGGLE, QK_RGB_MATRIX_MODE_NEXT, QK_RGB_MATRIX_MODE_PREVIOUS, QK_RGB_MATRIX_VALUE_UP, QK_RGB_MATRIX_VALUE_DOWN, KC_BSPC,
        /* Row2 */ KC_TAB, QK_RGB_MATRIX_HUE_UP, QK_RGB_MATRIX_HUE_DOWN, QK_RGB_MATRIX_SATURATION_UP, QK_RGB_MATRIX_SATURATION_DOWN, QK_RGB_MATRIX_SPEED_UP, QK_RGB_MATRIX_SPEED_DOWN, _______, _______, _______, _______, _______, _______,
        /* Row3 */ L3_ESC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, L3_MO, KC_ENT,
        /* Row4 */ KC_LSFT, L4_MO, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RSFT,
        /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO, KC_RGUI, KC_6, KC_RCTL),
}; // end keymaps

/* ── Tap/Hold Tuning — „Shift“-Gefühl für Caps, robuster Space-Tap ── */
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case L3_ESC:
        case SYS_ESC:
            return true; // Caps: Hold bevorzugen
        default:
            return false;
    }
}

bool get_ignore_mod_tap_interrupt(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case L3_ESC:
        case SYS_ESC:
        case SP_FN: // ← Space: Tap bleibt Tap, wenn du ihn nur kurz antippst
            return true;
        default:
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case L3_ESC:
        case SYS_ESC:
        case SP_FN: // ← WICHTIG: Space wird zum Hold, sobald du eine zweite Taste drückst
            return true;
        default:
            return false;
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case L3_ESC:
        case SYS_ESC:
            return 230; // Caps etwas großzügiger
        case SP_FN:
            return 200;
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
    if (!record->event.pressed) return true;

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
        case _BASE:
            rgb_matrix_set_color_all(0, 0, 0);
            break; // aus
        case _NEO1:
            rgb_matrix_set_color_all(0, 20, 0);
            break; // grün
        case _NEO2:
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
    }
    return state;
}
