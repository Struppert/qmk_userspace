#pragma once
#include QMK_KEYBOARD_H
#include "keymap_extras/keymap_german.h"
#include "process_unicode.h"
#include "quantum.h"

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

/* ──────────────────────────────────────────────────────────────────────────
 * Tap-Dance Bausteine:
 *  - td_lt_lead:  Tap = Key, Hold = layer_on, Double-Tap = Leader
 *  - td_mo_lead:  Tap = nichts (wie MO), Hold = layer_on, Double-Tap = Leader
 * ────────────────────────────────────────────────────────────────────────── */

typedef struct {
  uint8_t layer;
  uint16_t tap_kc;
  bool on;
} td_lt_lead_t;
static void td_lt_finished(tap_dance_state_t *s, void *ud) {
  td_lt_lead_t *c = (td_lt_lead_t *)ud;
  if (!c)
    return;
  if (s->count == 2 && !s->pressed) {
    tap_code16(QK_LEAD);
  } else if (s->pressed) {
    layer_on(c->layer);
    c->on = true;
  } else {
    tap_code16(c->tap_kc);
  }
}
static void td_lt_reset(tap_dance_state_t *s, void *ud) {
  td_lt_lead_t *c = (td_lt_lead_t *)ud;
  if (c && c->on) {
    layer_off(c->layer);
    c->on = false;
  }
}

typedef struct {
  uint8_t layer;
  bool on;
} td_mo_lead_t;
static void td_mo_finished(tap_dance_state_t *s, void *ud) {
  td_mo_lead_t *c = (td_mo_lead_t *)ud;
  if (!c)
    return;
  if (s->count == 2 && !s->pressed) {
    tap_code16(QK_LEAD);
  } else if (s->pressed) {
    layer_on(c->layer);
    c->on = true;
  } else { /* Tap: keine Ausgabe, wie MO() */
  }
}
static void td_mo_reset(tap_dance_state_t *s, void *ud) {
  td_mo_lead_t *c = (td_mo_lead_t *)ud;
  if (c && c->on) {
    layer_off(c->layer);
    c->on = false;
  }
}

// kleine Helfer:
static inline void send_and_enter(const char *s) {
  send_string(s);
  tap_code(KC_ENT);
}
static inline void send_cc(const char *type) {
  SEND_STRING("git commit -m \"");
  SEND_STRING(type);
  SEND_STRING(": ");
  // Cursor ans Zeilenende bleibt im Commit-Textfeld
}

#if defined(LEADER_ENABLE)
void leader_start_user(void) {
  // optional: Feedback beim Start, z.B. RGB blitzen lassen
  // (du nutzt unten ohnehin layer_state_set_user für Farben)
}

void leader_end_user(void) {
  // ---- Deine Sequenzen (ehemals LEADER_DICTIONARY) ----

  if (leader_sequence_two_keys(KC_A, KC_A)) {
    send_and_enter("LEADER OK");
  }

  // git status / add / diff / staged-diff
  if (leader_sequence_two_keys(KC_G, KC_S)) {
    send_and_enter("git status");
  } else if (leader_sequence_two_keys(KC_G, KC_A)) {
    send_and_enter("git add -A");
  } else if (leader_sequence_two_keys(KC_G, KC_D)) {
    send_and_enter("git diff");
  } else if (leader_sequence_three_keys(KC_G, KC_D, KC_S)) {
    send_and_enter("git diff --staged");
  }

  // commit helpers
  else if (leader_sequence_two_keys(KC_G, KC_C)) {
    SEND_STRING("git commit -m \"\"");
    tap_code(KC_LEFT);
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_F)) {
    send_cc("fix");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_B)) {
    send_cc("feat");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_R)) {
    send_cc("refactor");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_T)) {
    send_cc("test");
  } else if (leader_sequence_three_keys(KC_G, KC_C, KC_D)) {
    send_cc("docs");
  }

  // push variants
  else if (leader_sequence_two_keys(KC_G, KC_P)) {
    send_and_enter("git push");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_1)) {
    send_and_enter("git push --set-upstream origin HEAD");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_9)) {
    send_and_enter("git push --tags");
  } else if (leader_sequence_three_keys(KC_G, KC_P, KC_EXLM)) {
    send_and_enter("git push --force-with-lease");
  }

  // logs
  else if (leader_sequence_two_keys(KC_G, KC_L)) {
    send_and_enter("git log --oneline --graph --decorate -n 30");
  } else if (leader_sequence_three_keys(KC_G, KC_L, KC_A)) {
    send_and_enter("git log --all --decorate --oneline --graph");
  }

  // branch helpers
  else if (leader_sequence_three_keys(KC_G, KC_B, KC_N)) {
    SEND_STRING("git switch -c ");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_S)) {
    send_and_enter("git switch -");
  } else if (leader_sequence_three_keys(KC_G, KC_B, KC_D)) {
    send_and_enter("git branch -D ");
  }

  // rebase / tags
  else if (leader_sequence_two_keys(KC_G, KC_R)) {
    SEND_STRING("git rebase -i HEAD~");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_N)) {
    SEND_STRING("git tag -a v");
  } else if (leader_sequence_three_keys(KC_G, KC_T, KC_P)) {
    send_and_enter("git push --tags");
  }

  // Kein Match → nichts tun (du kannst hier optional Feedback einbauen)
}
#endif /* LEADER_ENABLE */

/* ── IDs
   ─────────────────────────────────────────────────────────────────── */
enum {
  TD_CAPS_SYS_LEAD = 0, // Base Caps: LT(_SYS, ESC) + DT=Leader
  TD_CAPS_NEO_LEAD,     // Neo Caps : LT(_NEO3, ESC) + DT=Leader
  TD_CAPS_NOTED_LEAD,   // Noted Caps: LT(_NOTED3, ESC) + DT=Leader
  TD_NEO_L3MO_LEAD,     // Neo „#“-Position: L3_MO_NEO + DT=Leader
  TD_NOTED_L3MO_LEAD,   // Noted „Ä“-Position: L3_MO_NOTED + DT=Leader
};

/* ── Konfigurationen ─────────────────────────────────────────────────────── */
static td_lt_lead_t td_caps_sys_cfg = {
    .layer = _SYS, .tap_kc = KC_ESC, .on = false};
static td_lt_lead_t td_caps_neo_cfg = {
    .layer = _NEOQWERTZ3, .tap_kc = KC_ESC, .on = false};
static td_lt_lead_t td_caps_noted_cfg = {
    .layer = _NOTED3, .tap_kc = KC_ESC, .on = false};

static td_mo_lead_t td_neo_l3mo_cfg = {.layer = _NEOQWERTZ3, .on = false};
static td_mo_lead_t td_noted_l3mo_cfg = {.layer = _NOTED3, .on = false};

/* ── Registrierung ───────────────────────────────────────────────────────── */

tap_dance_action_t PROGMEM tap_dance_actions[] = {
    [TD_CAPS_SYS_LEAD] =
        {
            .fn = {NULL, td_lt_finished, td_lt_reset, NULL},
            .user_data = &td_caps_sys_cfg,
        },
    [TD_CAPS_NEO_LEAD] =
        {
            .fn = {NULL, td_lt_finished, td_lt_reset, NULL},
            .user_data = &td_caps_neo_cfg,
        },
    [TD_CAPS_NOTED_LEAD] =
        {
            .fn = {NULL, td_lt_finished, td_lt_reset, NULL},
            .user_data = &td_caps_noted_cfg,
        },

    [TD_NEO_L3MO_LEAD] =
        {
            .fn = {NULL, td_mo_finished, td_mo_reset, NULL},
            .user_data = &td_neo_l3mo_cfg,
        },
    [TD_NOTED_L3MO_LEAD] =
        {
            .fn = {NULL, td_mo_finished, td_mo_reset, NULL},
            .user_data = &td_noted_l3mo_cfg,
        },
};
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
    /* Row3 */ TD(TD_CAPS_SYS_LEAD), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,
    /* Row4 */ KC_LSFT, DE_LABK, DE_Y, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, DE_COMM, DE_DOT, DE_MINS, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, QK_LEAD, KC_RCTL),

  // ───────── Layer 1: NeoQwertz Ebene 1 (klein) ─────────
  [_NEOQWERTZ1] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, DE_SS, DE_EQL, KC_BSPC,
    /* Row2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, DE_Z, KC_U, KC_I, KC_O, KC_P, DE_UDIA, DE_PLUS,
    /* Row3 */ TD(TD_CAPS_NEO_LEAD), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, DE_ODIA, DE_ADIA, TD(TD_NEO_L3MO_LEAD), KC_ENT,
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
    /* Row3 */ TD(TD_CAPS_NEO_LEAD), S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), S(DE_ODIA), S(DE_ADIA), TD(TD_NEO_L3MO_LEAD), S(KC_ENT),
    /* Row4 */ KC_TRNS, L4_MO_NEO, S(DE_Y), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), DE_SCLN, DE_COLN, DE_UNDS, KC_TRNS,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_2, KC_RCTL),

  // ───────── Layer 3: NeoQwertz Ebene 3 (Programmieren) ─────────
  [_NEOQWERTZ3] = LAYOUT_ISO(
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
    TD(TD_CAPS_NEO_LEAD), DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, DE_AT, TD(TD_NEO_L3MO_LEAD), KC_ENT,
    /* Row4 (<>  Y  X  C  V  B   N   M   ,    .     -) */
    KC_LSFT, L4_MO_NEO, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,
    /* Row5 (Mods/Space-FN bleiben nutzbar, nichts transparent) */
    KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NEO, KC_RGUI, KC_3, KC_RCTL),

  // ───────── Layer 4: NeoQwertz Ebene 4 (Navi/Numblock & Extras) ─────────
  [_NEOQWERTZ4] = LAYOUT_ISO(
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
    /* R3(14) */ TD(TD_CAPS_NOTED_LEAD), KC_C, KC_S, KC_I, KC_E, KC_O, KC_D, KC_T, KC_N, KC_R, KC_H, TD(TD_NOTED_L3MO_LEAD), DE_ACUT, KC_ENT,
    /* R4(13) */ NOTED_SHIFT, L4_MO_NOTED, KC_V, KC_X, DE_UDIA, DE_ADIA, DE_ODIA, KC_W, KC_G, DE_COMM, DE_DOT, KC_K, NOTED_SHIFT,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, L4_MO_NOTED, KC_RGUI, KC_5, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 2 (Shift) — korrespondierend zu Ebene 1
  [_NOTED2] = LAYOUT_ISO(
    /* R1(14) */ 
    KC_TRNS,
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
    /* R2(13) */ KC_TAB,  S(DE_Z),    S(DE_Y),   S(KC_U),    S(KC_A),    S(KC_Q),    S(KC_P), S(KC_B), S(KC_M), S(KC_L), S(KC_F), S(KC_J), DE_SS,
    /* R3(14) */ TD(TD_CAPS_NOTED_LEAD), S(KC_C),    S(KC_S),   S(KC_I),    S(KC_E),    S(KC_O),    S(KC_D), S(KC_T), S(KC_N), S(KC_R), S(KC_H), TD(TD_NOTED_L3MO_LEAD), DE_ACUT, KC_ENT,
    /* R4(13) */ NOTED_SHIFT, L4_MO_NOTED, S(KC_V),   S(KC_X),   S(DE_UDIA), S(DE_ADIA), S(DE_ODIA), S(KC_W), S(KC_G), DE_MINS, KC_DOT, S(KC_K), NOTED_SHIFT,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN,   L4_MO_NOTED,  KC_RGUI, KC_6, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 3:
  [_NOTED3] = LAYOUT_ISO(
    /* R1(14) */
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

    /* R2(13) */ KC_NO, DE_AT, DE_UNDS, DE_LBRC, DE_RBRC, DE_CIRC, DE_EXLM, DE_LABK, DE_RABK, DE_EQL, DE_AMPR, DE_AT, DE_TILD,
    /* R3(14) */ TD(TD_CAPS_NOTED_LEAD), DE_BSLS, DE_SLSH, DE_LCBR, DE_RCBR, DE_ASTR, DE_QUES, DE_LPRN, DE_RPRN, DE_MINS, DE_COLN, TD(TD_NOTED_L3MO_LEAD), DE_AT, KC_ENT,
    /* R4(13) */ KC_LSFT, L4_MO_NOTED, DE_HASH, DE_DLR, DE_PIPE, DE_TILD, DE_GRV, DE_PLUS, DE_PERC, DE_DQUO, DE_QUOT, DE_SCLN, KC_RSFT,
    /* R5(8)  */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_RALT, KC_RGUI, KC_7, KC_RCTL),

  // ─────────────────────────────────────────────────────────────────────────────
  // NOTED Ebene 4:
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
    /* Row4 */ KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MO, SYS_MO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_9, KC_RCTL),

  // ───────── Layer 6: SYS (Service/Bootloader) ─────────
  [_SYS] = LAYOUT_ISO(
    /* Row1 */ KC_ESC, QK_BOOT, EE_CLR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
    /* Row2 */ KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    /* Row3 */ KC_CAPS, UC_MODE_CYCLE, UC_SET_LNX, UC_SET_WIN, UC_SET_WINC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT,
    /* Row4 */ KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT,
    /* Row5 */ KC_LCTL, KC_LGUI, KC_LALT, SP_FN, KC_NO, KC_RGUI, KC_A, KC_RCTL
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
#ifdef RGB_MATRIX_ENABLE
  switch (get_highest_layer(state)) {
  case _QWERTZ:
    rgb_matrix_set_color_all(20, 20, 20); // weiß
    break;
  case _NEOQWERTZ1:
    rgb_matrix_set_color_all(20, 20, 0); // gelb
    break;
  case _NOTED1:
    rgb_matrix_set_color_all(20, 0, 20); // magenta
    break;
  case _FN:
    rgb_matrix_set_color_all(0, 20, 20); // cyan
    break;
  case _SYS:
    rgb_matrix_set_color_all(20, 0, 0); // rot
    break;
  case _RGB:
    rgb_matrix_set_color_all(0, 20, 0); // grün
    break;
  default:
    rgb_matrix_set_color_all(0, 0, 0); // aus
    break;
  }
#endif
  return state;
}
