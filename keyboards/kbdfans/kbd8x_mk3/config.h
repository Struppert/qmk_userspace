// keyboards/kbdfans/kbd8x_mk3/config.h
#pragma once

// Matrix size
#define MATRIX_ROWS 12
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

// Shift-Register (74HC165) Pins – STM32 Port-Notation:
#define SR_LATCH_PIN B12
#define SR_CLOCK_PIN B13
#define SR_DATA_PIN  B14


// RGB-Underglow (WS2812)
#define WS2812_DI_PIN B15
// optional: #define RGBLIGHT_LED_COUNT 16
// optional: #define RGBLIGHT_SLEEP
// RGBLIGHT_* Steps kannst du bei Bedarf wieder setzen

// Debounce
#define DEBOUNCE 5
