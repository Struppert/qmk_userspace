// leader/core.h
// Core types, flags, and dispatch helpers for leader system
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include QMK_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

// Attribute for maybe-unused static/global functions
#if defined(__GNUC__) || defined(__clang__)
#define LEADER_MAYBE_UNUSED __attribute__((unused))
#else
#define LEADER_MAYBE_UNUSED
#endif

// Handler type
typedef void (*leader_handler_t)(void);

// Flags for entries
enum { LFLAG_NONE = 0, LFLAG_DANGEROUS = 1 << 0 };

// Table entry
typedef struct {
  uint16_t seq[3]; // up to 3 keycodes; KC_NO for unused
  uint8_t len;     // 1..3
  leader_handler_t fn;
  uint16_t flags;
  const char *help; // short help string for contextual help
} LeaderEntry;

// Macros to define entries
#define SEQ1(a) {(a), KC_NO, KC_NO}
#define SEQ2(a, b) {(a), (b), KC_NO}
#define SEQ3(a, b, c) {(a), (b), (c)}
#define LENTRY1(a, fn, fl, h) {SEQ1(a), 1, (fn), (fl), (h)}
#define LENTRY2(a, b, fn, fl, h) {SEQ2(a, b), 2, (fn), (fl), (h)}
#define LENTRY3(a, b, c, fn, fl, h) {SEQ3(a, b, c), 3, (fn), (fl), (h)}

// Match helper bridging to QMK leader API
static inline bool leader_match_len(const uint16_t *seq, uint8_t len) {
  switch (len) {
  case 1:
    return leader_sequence_one_key(seq[0]);
  case 2:
    return leader_sequence_two_keys(seq[0], seq[1]);
  case 3:
    return leader_sequence_three_keys(seq[0], seq[1], seq[2]);
  default:
    return false;
  }
}

// Table provided by table.c
extern const LeaderEntry leader_table[];
size_t leader_table_count(void);

// Dispatch (implemented in core.c)
bool leader_table_dispatch(void);

#ifdef __cplusplus
}
#endif
