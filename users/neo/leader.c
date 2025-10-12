// users/neo/leader.c (glue)
#include QMK_KEYBOARD_H
#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

#include "leader/leader.h"

#if defined(LEADER_ENABLE)
void leader_start_user(void) { leader_module_start(); }
void leader_end_user(void) {
  if (!leader_module_end()) { /* no-op fallback */
  }
}
#endif
