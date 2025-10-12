// leader/leader.c (trie build)
#include QMK_KEYBOARD_H
#if defined(LEADER_ENABLE)
#include "process_leader.h"
#endif

#include "core.h"
#include "leader.h"
#include "trie.h"

void leader_module_start(void) { leader_trie_build(); }

bool leader_module_end(void) { return leader_table_dispatch(); }
