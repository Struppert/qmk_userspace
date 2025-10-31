// keyboards/kbdfans/kbd8x_mk3/matrix.c
// Generic 74HC165 shift-in scanner for 12x8 matrix (KBD8X MK3 STM32 daughterboard)
// QMK/ChibiOS only (no STM32 HAL). Implements the modern CUSTOM_MATRIX API.

#include "quantum.h"
#include "kbd8x_mk3_stm32.h"   // <- ggf. an deinen Headernamen anpassen
#include <string.h>

// ========================= Tunables & Options ===============================

// 74HC165 /PL is active LOW
#ifndef SR_LATCH_ACTIVE_LOW
#    define SR_LATCH_ACTIVE_LOW 1
#endif

// Optional small sample delay after (or before) clock edge (µs)
#ifndef SR_DATA_SAMPLE_DELAY_US
#    define SR_DATA_SAMPLE_DELAY_US 0
#endif

// Shift edge: 165 typically shifts on rising edge
#ifndef SR_SHIFT_ON_RISING
#    define SR_SHIFT_ON_RISING 1
#endif

// Bit order as it comes out of the chain (experiment if mirrored per row)
#ifndef SR_SHIFT_LSB_FIRST
#    define SR_SHIFT_LSB_FIRST 1
#endif

// If pressed = logic high on DATA, keep =1 (common for 165 pull-ups, active low on keys)
#ifndef MATRIX_PRESSED_IS_HIGH
#    define MATRIX_PRESSED_IS_HIGH 1
#endif

// Column / Row remap (identity by default)
static const uint8_t COL_REMAP[MATRIX_COLS] = { 0,1,2,3,4,5,6,7 };
static const uint8_t ROW_REMAP[MATRIX_ROWS] = { 0,1,2,3,4,5,6,7,8,9,10,11 };

// ========================= Local State ======================================

static matrix_row_t last_reported[MATRIX_ROWS];  // what we last returned to QMK

// Provide weak user hook with correct (void) signature for new QMK
__attribute__((weak)) void matrix_init_user(void) {}
__attribute__((weak)) void matrix_scan_user(void) {}

// ========================= HW helpers (165) =================================

static inline void sr_parallel_load(void) {
    #if SR_LATCH_ACTIVE_LOW
    writePinLow(SR_LATCH_PIN);
    wait_us(1);
    writePinHigh(SR_LATCH_PIN);
    #else
    writePinHigh(SR_LATCH_PIN);
    wait_us(1);
    writePinLow(SR_LATCH_PIN);
    #endif
}

static inline void sr_clock_pulse(void) {
    #if SR_SHIFT_ON_RISING
    writePinHigh(SR_CLOCK_PIN);
    wait_us(1);
    writePinLow(SR_CLOCK_PIN);
    #else
    writePinLow(SR_CLOCK_PIN);
    wait_us(1);
    writePinHigh(SR_CLOCK_PIN);
    #endif
}

static inline uint8_t sr_read_8(void) {
    uint8_t v = 0;
    #if SR_SHIFT_LSB_FIRST
    for (uint8_t i = 0; i < 8; i++) {
        if (SR_DATA_SAMPLE_DELAY_US) wait_us(SR_DATA_SAMPLE_DELAY_US);
        uint8_t bit = readPin(SR_DATA_PIN) ? 1 : 0;
        if (!MATRIX_PRESSED_IS_HIGH) bit ^= 1;
        v |= (bit << i);              // LSB first
        sr_clock_pulse();
    }
    #else
    for (uint8_t i = 0; i < 8; i++) {
        if (SR_DATA_SAMPLE_DELAY_US) wait_us(SR_DATA_SAMPLE_DELAY_US);
        uint8_t bit = readPin(SR_DATA_PIN) ? 1 : 0;
        if (!MATRIX_PRESSED_IS_HIGH) bit ^= 1;
        v = (v << 1) | bit;           // MSB first
        sr_clock_pulse();
    }
    #endif
    return v;
}

static inline matrix_row_t apply_col_remap(uint8_t packed8) {
    matrix_row_t out = 0;
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint8_t src = COL_REMAP[c];
        uint8_t bit = (packed8 >> src) & 1u;
        out |= ((matrix_row_t)bit) << c;
    }
    return out;
}

// ========================= CUSTOM_MATRIX API ================================
//
// QMK calls:
//   void matrix_init_custom(void);
//   bool matrix_scan_custom(matrix_row_t out[MATRIX_ROWS]);
//
// Return value of scan = whether any change occurred (helps QMK debounce/processing)
// No direct access to a global `matrix` is required/allowed.

void matrix_init_custom(void) {
    // Configure pins
    setPinOutput(SR_LATCH_PIN);
    setPinOutput(SR_CLOCK_PIN);
    setPinInput(SR_DATA_PIN);

    // Idle levels
    #if SR_LATCH_ACTIVE_LOW
    writePinHigh(SR_LATCH_PIN);
    #else
    writePinLow(SR_LATCH_PIN);
    #endif
    #if SR_SHIFT_ON_RISING
    writePinLow(SR_CLOCK_PIN);
    #else
    writePinHigh(SR_CLOCK_PIN);
    #endif

    // Clear state
    memset(last_reported, 0, sizeof(last_reported));

    matrix_init_user();
}

bool matrix_scan_custom(matrix_row_t out[MATRIX_ROWS]) {
    bool changed = false;

    // Capture current inputs into shift regs
    sr_parallel_load();

    // Read all rows
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint8_t packed = sr_read_8();
        matrix_row_t row_bits = apply_col_remap(packed);

        // Apply row remap and write into 'out' buffer for QMK
        uint8_t rr = ROW_REMAP[r];
        out[rr] = row_bits;

        if (last_reported[rr] != row_bits) {
            last_reported[rr] = row_bits;
            changed = true;
        }
    }

    // Optional: user hook (no return value in new API)
    matrix_scan_user();

    return changed;
}
