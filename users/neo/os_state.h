// users/neo/os_state.h
#pragma once
#include "eeconfig.h"
#include "process_unicode.h"
#include "quantum.h"
#include <stdint.h>

// Betriebssystem-Modus (persistenter Schalter)
typedef enum { OS_WIN = 0, OS_LNX = 1, OS_MAC = 2 } os_mode_t;

// Lebenszyklus
void os_load(void);         // aus EEPROM laden (beim Start)
void os_set(os_mode_t os);  // setzen + speichern + Feedback
void os_cycle(void);        // Windows -> Linux -> macOS -> Windows ...
os_mode_t os_current(void); // aktuellen Modus abfragen

// Optionales Feedback (RGB/Audio/OLED); darf NULL-Operation sein
void os_feedback(os_mode_t os);

// Komfort-Helfer (Terminalausgabe)
void os_print(void);

// Unicode-Helper (inkl. optionalem RGB-Matrix-Feedback)
void uc_set_and_feedback(uint8_t unicode_mode); // UNICODE_MODE_*
