// leader/trie.h
#pragma once
#include "core.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Build a compact trie from leader_table[]. Call once at startup.
void leader_trie_build(void);

// Find an exact entry matching seq/len; returns NULL if none.
const LeaderEntry *leader_trie_find_exact(const uint16_t *seq, uint8_t len);

// List distinct next keys directly reachable from prefix. Returns number of
// children written to `out_keys` (up to max_out). If prefix itself is a full
// command, `has_exact` will be set true.
size_t leader_trie_children(const uint16_t *prefix, uint8_t plen,
                            uint16_t *out_keys, size_t max_out,
                            bool *has_exact);

#ifdef __cplusplus
}
#endif
