// leader/trie.c
#include "trie.h"
#include <string.h>

// A compact trie stored in two arrays: nodes and edges.
// Each node stores a range of edges [first, first+count).

typedef struct {
  uint32_t first;
  uint16_t count;
  uint16_t entry_idx;
} TNode;
typedef struct {
  uint16_t key;
  uint16_t next;
} TEdge;

static TNode g_nodes[512];  // enough for ~200 entries (adjust if needed)
static TEdge g_edges[1024]; // edges
static uint16_t g_node_count = 0;
static uint16_t g_edge_count = 0;

extern const LeaderEntry leader_table[];
size_t leader_table_count(void);

static uint16_t new_node(void) {
  uint16_t id = g_node_count++;
  g_nodes[id].first = g_edge_count;
  g_nodes[id].count = 0;
  g_nodes[id].entry_idx = 0xFFFF;
  return id;
}

static uint16_t ensure_child(uint16_t node, uint16_t key) {
  // linear search in this node's edges
  uint32_t f = g_nodes[node].first;
  uint16_t c = g_nodes[node].count;
  for (uint16_t i = 0; i < c; i++) {
    if (g_edges[f + i].key == key)
      return g_edges[f + i].next;
  }
  // append new edge
  uint16_t child = new_node();
  g_edges[g_nodes[node].first + g_nodes[node].count] =
      (TEdge){.key = key, .next = child};
  g_nodes[node].count++;
  g_edge_count++;
  return child;
}

void leader_trie_build(void) {
  g_node_count = 0;
  g_edge_count = 0;
  (void)new_node(); // root = 0
  size_t n = leader_table_count();
  for (size_t i = 0; i < n; i++) {
    const LeaderEntry *e = &leader_table[i];
    uint16_t node = 0;
    for (uint8_t j = 0; j < e->len; j++)
      node = ensure_child(node, e->seq[j]);
    g_nodes[node].entry_idx = (uint16_t)i; // exact match at this node
  }
}

static uint16_t follow(const uint16_t *seq, uint8_t len, bool *ok) {
  uint16_t node = 0;
  *ok = true;
  for (uint8_t i = 0; i < len; i++) {
    uint32_t f = g_nodes[node].first;
    uint16_t c = g_nodes[node].count;
    bool found = false;
    for (uint16_t k = 0; k < c; k++)
      if (g_edges[f + k].key == seq[i]) {
        node = g_edges[f + k].next;
        found = true;
        break;
      }
    if (!found) {
      *ok = false;
      return 0;
    }
  }
  return node;
}

const LeaderEntry *leader_trie_find_exact(const uint16_t *seq, uint8_t len) {
  bool ok = false;
  uint16_t node = follow(seq, len, &ok);
  if (!ok)
    return NULL;
  uint16_t idx = g_nodes[node].entry_idx;
  return (idx == 0xFFFF) ? NULL : &leader_table[idx];
}

size_t leader_trie_children(const uint16_t *prefix, uint8_t plen,
                            uint16_t *out_keys, size_t max_out,
                            bool *has_exact) {
  bool ok = false;
  uint16_t node = follow(prefix, plen, &ok);
  if (!ok) {
    if (has_exact)
      *has_exact = false;
    return 0;
  }
  if (has_exact)
    *has_exact = (g_nodes[node].entry_idx != 0xFFFF);
  uint32_t f = g_nodes[node].first;
  uint16_t c = g_nodes[node].count;
  size_t w = (c > max_out) ? max_out : c;
  for (size_t i = 0; i < w; i++)
    out_keys[i] = g_edges[f + i].key;
  return w;
}
