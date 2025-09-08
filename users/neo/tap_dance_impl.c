// tap_dance_impl.c
#include QMK_KEYBOARD_H
#include "keymap_iso_common.h"   // Layer-Enums (_SYS, _NEOQWERTZ3, _NOTED3, …)
#include "tap_dance_ids.h"
#include "tap_dance_impl.h"

#ifdef TAP_DANCE_ENABLE
  #include "process_tap_dance.h"
  #ifdef LEADER_ENABLE
    #include "process_leader.h"
  #endif

  // Helper: Doppeltap → Leader starten
  static inline bool td_wants_leader(const tap_dance_state_t *s) {
    return (s->count >= 2) && !s->pressed;
  }

  /* Test A / Leader */
  void td_test_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    tap_code16(KC_A);
  }
  void td_test_reset(tap_dance_state_t *s, void *ud) { (void)s; (void)ud; }

  /* CAPS: SYS */
  static bool td_caps_sys_on = false;
  void td_caps_sys_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    if (s->pressed) { layer_on(_SYS); td_caps_sys_on = true; }
    else { tap_code16(KC_ESC); }
  }
  void td_caps_sys_reset(tap_dance_state_t *s, void *ud) {
    (void)s; (void)ud;
    if (td_caps_sys_on) { layer_off(_SYS); td_caps_sys_on = false; }
  }

  /* CAPS: NEO */
  static bool td_caps_neo_on = false;
  void td_caps_neo_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    if (s->pressed) { layer_on(_NEOQWERTZ3); td_caps_neo_on = true; }
    else { tap_code16(KC_ESC); }
  }
  void td_caps_neo_reset(tap_dance_state_t *s, void *ud) {
    (void)s; (void)ud;
    if (td_caps_neo_on) { layer_off(_NEOQWERTZ3); td_caps_neo_on = false; }
  }

  /* CAPS: NOTED */
  static bool td_caps_noted_on = false;
  void td_caps_noted_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    if (s->pressed) { layer_on(_NOTED3); td_caps_noted_on = true; }
    else { tap_code16(KC_ESC); }
  }
  void td_caps_noted_reset(tap_dance_state_t *s, void *ud) {
    (void)s; (void)ud;
    if (td_caps_noted_on) { layer_off(_NOTED3); td_caps_noted_on = false; }
  }

  /* L3 MO: NEO */
  static bool td_neo_l3mo_on = false;
  void td_neo_l3mo_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    if (s->pressed) { layer_on(_NEOQWERTZ3); td_neo_l3mo_on = true; }
  }
  void td_neo_l3mo_reset(tap_dance_state_t *s, void *ud) {
    (void)s; (void)ud;
    if (td_neo_l3mo_on) { layer_off(_NEOQWERTZ3); td_neo_l3mo_on = false; }
  }

  /* L3 MO: NOTED */
  static bool td_noted_l3mo_on = false;
  void td_noted_l3mo_finished(tap_dance_state_t *s, void *ud) {
    (void)ud;
    #ifdef LEADER_ENABLE
      if (td_wants_leader(s)) { leader_start(); reset_tap_dance(s); return; }
    #endif
    if (s->pressed) { layer_on(_NOTED3); td_noted_l3mo_on = true; }
  }
  void td_noted_l3mo_reset(tap_dance_state_t *s, void *ud) {
    (void)s; (void)ud;
    if (td_noted_l3mo_on) { layer_off(_NOTED3); td_noted_l3mo_on = false; }
  }
#endif // TAP_DANCE_ENABLE
