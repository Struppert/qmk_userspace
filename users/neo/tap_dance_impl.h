// tap_dance_impl.h
#pragma once
#ifdef TAP_DANCE_ENABLE
#include "process_tap_dance.h" // Signaturen
#include "quantum.h"           // tap_dance_state_t

// Test A / Leader
void td_test_finished(tap_dance_state_t *s, void *ud);
void td_test_reset(tap_dance_state_t *s, void *ud);

// CAPS: SYS
void td_caps_sys_finished(tap_dance_state_t *s, void *ud);
void td_caps_sys_reset(tap_dance_state_t *s, void *ud);

// CAPS: NEO
void td_caps_neo_finished(tap_dance_state_t *s, void *ud);
void td_caps_neo_reset(tap_dance_state_t *s, void *ud);

// CAPS: NOTED
void td_caps_noted_finished(tap_dance_state_t *s, void *ud);
void td_caps_noted_reset(tap_dance_state_t *s, void *ud);

// L3 MO: NEO
void td_neo_l3mo_finished(tap_dance_state_t *s, void *ud);
void td_neo_l3mo_reset(tap_dance_state_t *s, void *ud);

// L3 MO: NOTED
void td_noted_l3mo_finished(tap_dance_state_t *s, void *ud);
void td_noted_l3mo_reset(tap_dance_state_t *s, void *ud);
#endif
