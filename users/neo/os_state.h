// users/neo/os_state.h
#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    OS_WIN = 0,
    OS_LNX = 1,
    OS_MAC = 2,
} os_mode_t;

typedef enum {
    SH_BASH = 0,
    SH_FISH = 1,
    SH_ZSH  = 2,
    SH_PWSH = 3,   // PowerShell
} shell_t;

// Aktueller Status
os_mode_t os_current(void);
shell_t   shell_current(void);

// Setzen (persistiert & koppelt Shell mit OS)
void os_set(os_mode_t m);     // <-- einfache API wie in deinem Code
void os_cycle(void);

// Shell weiterhin separat steuerbar (führt Persist durch)
void shell_set(shell_t sh, bool persist);
void shell_cycle(bool persist);

// Print/Help
void os_print(void);
void shell_print(void);

// Persist-Hook (bereitgestellt von userspace-init.c)
void userspace_persist_settings(uint8_t os, uint8_t sh);

// Unicode: einfache zentrale API
void uc_set_and_feedback(uint8_t legacy_mode);
void uc_set_for_current_os(void);
