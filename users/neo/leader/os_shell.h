// leader/os_shell.h
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include QMK_KEYBOARD_H
#include "os_state.h"

#ifdef __cplusplus
extern "C" {
#endif

// OS helpers
static inline bool is_win(void) { return os_current() == OS_WIN; }
static inline bool is_linux(void) { return os_current() == OS_LNX; }
static inline bool is_mac(void) { return os_current() == OS_MAC; }

// Execution helpers
void shell_run_cmd(const char *cmd);
void send_line(const char *s);
void send_line_os(const char *posix_cmd, const char *pwsh_cmd,
                  const char *mac_cmd_opt);
void type_quotes_and_place_cursor(void);
void send_cmd_with_empty_quotes(const char *prefix);

// Shell helpers
void shell_print_to_app(void);
void leader_help_shell_group(void);

#ifdef __cplusplus
}
#endif
