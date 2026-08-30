// keyboards/kbdfans/kbd8x_mk3/matrix.c
// Serial "walking bit" matrix scanner for KBD8X MK3 STM32 daughterboard.
// QMK/ChibiOS only (no STM32 HAL). Implements the modern CUSTOM_MATRIX API.
//
// This is NOT a classic 74HC165 parallel-load shift register (that was a
// wrong assumption in an earlier version of this file - matched the SDI
// pin at 0x08004000 but no keys ever registered). Ported from the real
// YDKB/KBDFans firmware source (github.com/yangdigi/vial-qmk-v5,
// keyboards/ydkb/unicore_f1/{matrix.c,switch_board.h}):
//
//   SCK = PB12 (clock, always output)
//   SDI = PB13 (bidirectional: output while shifting a bit through the
//               chain, input while sampling the currently-selected key)
//
// A single '1' bit is shifted through a chain of MATRIX_ROWS*MATRIX_COLS
// positions, one clock pulse at a time. After each pulse SDI is switched
// to input and sampled: LOW means the currently-selected key is pressed.
// The chain length MUST equal MATRIX_ROWS*MATRIX_COLS exactly, or every
// key from the point of misalignment onward reads the wrong position
// (this is what caused the Enter/# swap seen even in official firmware
// builds using the wrong row count).

#include "quantum.h"
#include <string.h>

// Caps/Scroll-Indicator als RGBLIGHT-Layer auf WS2812-Kettenposition 0/1
// (siehe README.md "RGB / Status-LEDs" - Positionen 2-5 bleiben normales
// Underglow). Ersetzt den alten direkten ws2812_set_color_all()-Ansatz,
// der mit dem 48MHz-Bitbang-Timing nicht sauber lief.
const rgblight_segment_t PROGMEM caps_indicator[]   = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_RED});
const rgblight_segment_t PROGMEM scroll_indicator[] = RGBLIGHT_LAYER_SEGMENTS({1, 1, HSV_BLUE});
const rgblight_segment_t *const PROGMEM rgb_indicator_layers[] = RGBLIGHT_LAYERS_LIST(caps_indicator, scroll_indicator);

// Global matrix buffers - defined in keymap.c to ensure keymap_introspection.o can link
extern matrix_row_t raw_matrix[MATRIX_ROWS];
extern matrix_row_t matrix[MATRIX_ROWS];

#define CLOCK_PULSE()                 \
    do {                              \
        palSetPad(GPIOB, 12);         \
        palClearPad(GPIOB, 12);       \
    } while (0)

// Hardware revision / pull config, auto-detected in matrix_init_custom()
// exactly like the reference firmware (some PCB revisions invert SDI
// polarity and/or need pull-down instead of pull-up on the sense pin).
static bool is_ver5020       = false;
static bool has_extra_pullup = false;

static inline void key_sdi_off(void) {
    if (is_ver5020) {
        palClearPad(GPIOB, 13);
    } else {
        palSetPad(GPIOB, 13);
    }
}

static inline void key_sdi_on(void) {
    if (is_ver5020) {
        palSetPad(GPIOB, 13);
    } else {
        palClearPad(GPIOB, 13);
    }
}

static inline void select_key_ready(void) {
    palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
}

static inline void get_key_ready(void) {
    palSetPadMode(GPIOB, 13, has_extra_pullup ? PAL_MODE_INPUT_PULLDOWN : PAL_MODE_INPUT_PULLUP);
    wait_us(2);
}

// LOW on SDI while selected = key pressed.
static inline bool get_key(void) {
    return !palReadPad(GPIOB, 13);
}

// mode 0: reset the chain and seed a single '1' bit at position 0.
// mode 1: shift the walking bit to the next position (feeds a '0' in
//         behind it, same as the reference implementation).
static void select_key(uint8_t mode) {
    select_key_ready();
    if (mode == 0) {
        key_sdi_off();
        for (uint16_t i = 0; i < (uint16_t)MATRIX_ROWS * MATRIX_COLS; i++) {
            CLOCK_PULSE();
        }
        key_sdi_on();
        CLOCK_PULSE();
    } else {
        key_sdi_off();
        CLOCK_PULSE();
    }
    get_key_ready();
}

static void init_cols(void) {
    palSetGroupMode(GPIOB, (1 << 13) | (1 << 12), 0, PAL_MODE_OUTPUT_PUSHPULL);
    select_key_ready();
    key_sdi_off();
    for (uint16_t i = 0; i < (uint16_t)MATRIX_ROWS * MATRIX_COLS; i++) {
        CLOCK_PULSE();
    }
}

void matrix_init_custom(void) {
    // Hardware revision detection (PB9), matches the reference firmware.
    palSetPadMode(GPIOB, 9, PAL_MODE_INPUT_PULLUP);
    palSetPad(GPIOB, 9);
    wait_ms(10);
    is_ver5020 = (palReadPad(GPIOB, 9) == 0);

    // caps_led (PB14) / scroll_led (PA8). The reference firmware's polarity
    // (HIGH=on/LOW=off via single_color_indicator_set()) does NOT match
    // this board empirically - clearing both pins left the caps indicator
    // lit anyway, so this hardware is active-LOW (LOW=on/HIGH=off).
    // Actual on/off state is driven by led_update_kb() below.
    palSetPadMode(GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOB, 14);
    palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOA, 8);

    init_cols();

    // Pull-up/pull-down detection on the sense line (PB13), matches the
    // reference firmware.
    palSetPad(GPIOB, 13);
    palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLDOWN);
    wait_ms(5);
    has_extra_pullup = palReadPad(GPIOB, 13);
}

bool matrix_scan_custom(matrix_row_t out[MATRIX_ROWS]) {
    bool changed = false;

    select_key(0);
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t row_bits = 0;
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            if (get_key()) {
                row_bits |= ((matrix_row_t)1 << col);
            }
            select_key(1);
        }

        if (out[row] != row_bits) {
            out[row] = row_bits;
            changed  = true;
        }
    }

    return changed;
}

// caps_led (PB14) / scroll_led (PA8): kept as a redundant simple-GPIO path
// matching the vendor reference's single_color_indicator_set() (on=HIGH,
// off=LOW) in case this board revision has them wired as real GPIO LEDs
// alongside the WS2812 chain (some do, see is_sc_leds_mcu in the vendor
// source) - but the actual, confirmed-working indicator is the RGBLIGHT
// layer below (chain positions 0/1). Empirically, toggling these GPIOs
// alone previously showed no clean on/off effect.
bool led_update_kb(led_t led_state) {
    if (led_state.caps_lock) {
        palSetPad(GPIOB, 14);
    } else {
        palClearPad(GPIOB, 14);
    }
    if (led_state.scroll_lock) {
        palSetPad(GPIOA, 8);
    } else {
        palClearPad(GPIOA, 8);
    }
    rgblight_set_layer_state(0, led_state.caps_lock);
    rgblight_set_layer_state(1, led_state.scroll_lock);
    return led_update_user(led_state);
}

void keyboard_post_init_kb(void) {
    rgblight_layers = rgb_indicator_layers;
    keyboard_post_init_user();
}

// Matrix helper function - required by suspend.c and keyboard.c
// MUST be a real function, not inline or macro, to avoid ARM/Thumb linker conflicts
matrix_row_t matrix_get_row(uint8_t row) {
#ifdef MATRIX_MASKED
    return matrix[row] & matrix_mask[row];
#else
    return matrix[row];
#endif
}
