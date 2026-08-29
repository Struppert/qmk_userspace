// keyboards/kbdfans/kbd8x_mk3/config.h
#pragma once

// Matrix size - MUST match the real scan chain length (rows*cols), not
// just how many rows have real keys mapped. Confirmed against the real
// YDKB firmware source (config.h: "MATRIX_ROWS 14 // max supported") -
// the serial walking-bit scan (see matrix.c) shifts through exactly
// MATRIX_ROWS*MATRIX_COLS positions each cycle, so a wrong row count here
// misaligns every key after the point where our count diverges from the
// real chain length. Only rows 0-11 have real keys; 12-13 stay unused.
#define MATRIX_ROWS 14
#define MATRIX_COLS 8
#define DIODE_DIRECTION COL2ROW

#ifndef KEYMAP_C
#    define KEYMAP_C "keyboards/kbdfans/kbd8x_mk3/keymaps/neo/keymap.c"
#endif

// CPU_CLOCK wird von ChibiOS aus STM32_SYSCLK (siehe mcuconf.h PLL-Settings)
// abgeleitet - platforms/chibios/chibios_config.h definiert es ungeschuetzt,
// ein eigener #define hier wuerde kollidieren.

// 12 Layer x 96 Keys x 2 Byte = 2304 Byte allein fuer Dynamic Keymaps;
// Default-Logical-Size (1024) reicht nicht. STM32F103 hat reichlich Flash.
#define WEAR_LEVELING_BACKING_SIZE (8 * 1024)
#define WEAR_LEVELING_LOGICAL_SIZE (4 * 1024)

// Matrix scan pins - NOT a 74HC165 SIPO shift register (that assumption
// was wrong). Confirmed against the real firmware source: a serial
// "walking bit" scan over 2 wires, SCK=PB12 (clock, output) and
// SDI=PB13 (bidirectional: output while shifting, input while reading
// the currently-selected key). See matrix.c for the exact protocol.
// No config.h defines needed - matrix.c hardcodes GPIOB 12/13 directly,
// matching the reference implementation's style.


// RGB-Underglow (WS2812) - disabled (RGBLIGHT_ENABLE=no), see rules.mk and
// README.md for the open WS2812-timing-at-48MHz LED issue.
#define WS2812_DI_PIN B15
// optional: #define RGBLIGHT_LED_COUNT 16
// optional: #define RGBLIGHT_SLEEP
// RGBLIGHT_* Steps kannst du bei Bedarf wieder setzen

// Debounce
#define DEBOUNCE 5
