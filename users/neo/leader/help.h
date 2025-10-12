// leader/help.h
#pragma once
#include <stdint.h>
#include QMK_KEYBOARD_H
#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

void help_block_begin(void);
void help_block_line(const char *s);
void help_block_end(void);
void help_block_hr(void);

// High-level help sections
void help_groups_overview(void);
void help_os(void);
void help_tabs(void);
void help_panes(void);
void help_git(void);
void help_fzf(void);
void help_rg(void);
void help_zoxide(void);
void help_yazi(void);
void help_zellij(void);
void help_sed(void);

// Contextual help (trie-like) for a prefix sequence
void leader_print_help_for_prefix(const uint16_t *prefix, uint8_t plen);

#ifdef __cplusplus
}
#endif
