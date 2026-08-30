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


// RGB-Kette (WS2812): 4 Underglow-LEDs + 2 Indicator-Positionen (Caps/Scroll,
// siehe Vendor-Referenz kbd8xmk3_vial/config.h: RGBLED_NUM=4,
// PHY_INDICATOR_NUM=2). Hardware-PWM+DMA-Treiber (WS2812_DRIVER=pwm in
// rules.mk) statt Bitbang, siehe README.md RGB-Abschnitt.
#define WS2812_DI_PIN B15
#define RGBLIGHT_LED_COUNT 6
#define RGBLIGHT_LIMIT_VAL 50

// Chain-Positionen 0/1 sind per RGBLIGHT_LAYERS als feste Caps/Scroll-
// Indicator-Overlays reserviert (siehe led_update_kb() in matrix.c);
// Position 2-5 bleiben normales Underglow. Nur 2 Layer nötig -> spart RAM
// gegenüber dem RGBLIGHT_MAX_LAYERS-Default von 8.
#define RGBLIGHT_LAYERS
#define RGBLIGHT_MAX_LAYERS 2
// Ohne dieses Flag rendert QMK Layer-Overlays nur, wenn der globale
// RGB-Schalter (rgblight_config.enable) an ist - Caps/Scroll-Indicator
// soll aber unabhängig vom RGB-Modus (aktuell aus) sichtbar sein.
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF

// PB15 = TIM1_CH3N (Default-AF, kein Remap nötig). TIM1_UP-DMA-Request ist
// auf STM32F103 fest auf DMA1 Channel 5 verdrahtet, unabhängig vom
// gewählten Kanal.
#define WS2812_PWM_DRIVER PWMD1
#define WS2812_PWM_CHANNEL 3
#define WS2812_PWM_COMPLEMENTARY_OUTPUT
#define WS2812_PWM_DMA_STREAM STM32_DMA1_STREAM5
#define WS2812_PWM_DMA_CHANNEL 5

// Debounce
#define DEBOUNCE 5
