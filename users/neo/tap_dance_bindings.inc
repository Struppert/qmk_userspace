// tap_dance_bindings.inc  (vom board-spezifischen keymap.c inkludieren)
#ifdef TAP_DANCE_ENABLE
  #include "process_tap_dance.h"
  #include "tap_dance_ids.h" 

  // Vorwärtsdeklarationen der Callback-Funktionen (implementiert in keymap_logic.c)
  void td_test_finished(tap_dance_state_t *s, void *ud);
  void td_test_reset(tap_dance_state_t *s, void *ud);

  void td_caps_sys_finished(tap_dance_state_t *s, void *ud);
  void td_caps_sys_reset(tap_dance_state_t *s, void *ud);

  void td_caps_neo_finished(tap_dance_state_t *s, void *ud);
  void td_caps_neo_reset(tap_dance_state_t *s, void *ud);

  void td_caps_noted_finished(tap_dance_state_t *s, void *ud);
  void td_caps_noted_reset(tap_dance_state_t *s, void *ud);

  void td_neo_l3mo_finished(tap_dance_state_t *s, void *ud);
  void td_neo_l3mo_reset(tap_dance_state_t *s, void *ud);

  void td_noted_l3mo_finished(tap_dance_state_t *s, void *ud);
  void td_noted_l3mo_reset(tap_dance_state_t *s, void *ud);

  // Das Array MUSS in diesem TU definiert sein, damit Introspection sizeof(...) kann
  tap_dance_action_t PROGMEM tap_dance_actions[] = {
    [TD_TEST_A_LEAD]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_test_finished,      td_test_reset),
    [TD_CAPS_SYS_LEAD]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_caps_sys_finished,   td_caps_sys_reset),
    [TD_CAPS_NEO_LEAD]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_caps_neo_finished,   td_caps_neo_reset),
    [TD_CAPS_NOTED_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_caps_noted_finished, td_caps_noted_reset),
    [TD_NEO_L3MO_LEAD]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_neo_l3mo_finished,   td_neo_l3mo_reset),
    [TD_NOTED_L3MO_LEAD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_noted_l3mo_finished, td_noted_l3mo_reset),
  };
#endif
