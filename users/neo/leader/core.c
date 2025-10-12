// leader/core.c
#include QMK_KEYBOARD_H
#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

#include "core.h"
#include <stddef.h>
#include <string.h>

extern const LeaderEntry leader_table[]; // from table.c
extern size_t leader_table_count(void);  // implemented in table.c

bool leader_table_dispatch(void) {
  size_t n = leader_table_count();
  for (size_t i = 0; i < n; ++i) {
    LeaderEntry e = leader_table[i];
    if (leader_match_len(e.seq, e.len)) {
      e.fn();
      return true;
    }
  }
  return false;
}
