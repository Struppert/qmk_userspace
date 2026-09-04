// keyboards/kbdfans/kbd8x_mk3/keymaps/neo/keymap.c - KBD8x with CUSTOM matrix
#define LAYOUT_ISO LAYOUT_tkl_iso
#include QMK_KEYBOARD_H

#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core)
#include "layouts/fn60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/qwertz60.h"
#include "layouts/sys60.h"

// kbd8x_mk3 hat kein RGB_MATRIX (nur RGBLIGHT, siehe users/neo/rules.mk) -
// die geteilte _RGB-Ebene (layouts/rgb60.h) wäre hier komplett wirkungslos
// und wird deshalb unten gar nicht erst ins keymaps[]-Array aufgenommen.
// Die FN-Taste, die dort normalerweise hinführt (RGB_MO), zeigt sonst auf
// einen Layer-Index, den dieses Board nicht mehr definiert - lokal (nur in
// dieser Datei) auf KC_NO umgebogen, ohne layouts/fn60.h für andere Boards
// anzufassen.
#undef FN60_ROW4
#define FN60_ROW4  KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, SYS_MO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_RSFT,

// UG_TOGG (RGBLIGHT an/aus, siehe formfactors/ff_tkl_iso_kbd8x_mk3.h) auch
// direkt auf der _SYS-Ebene erreichbar - rechte Hand, neben Backspace
// (nicht neben EE_CLR, um ein versehentliches EEPROM-Löschen beim Tippen
// nicht durch eine Nachbartaste noch wahrscheinlicher zu machen).
// Zusätzlich zur F-Reihe-Position, die layerübergreifend sowieso überall
// gilt. SYS60_ROW1 ist geteilt (layouts/sys60.h) - lokaler Override statt
// die Datei für andere Boards anzufassen.
#undef SYS60_ROW1
#define SYS60_ROW1  KC_ESC, QK_BOOT, EE_CLR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, UG_TOGG, KC_BSPC,

// TKL-Formfaktor (KBD8X) + Bottom-Row-Picker
#include "formfactors/ff_tkl_iso_kbd8x_mk3.h"
#include "formfactors/row5_pick.h"

#ifdef TETRIS_GAME_ENABLE
#include "layouts/tetris60.h"
#include "tetris.h"
#endif

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"

#ifdef TETRIS_GAME_ENABLE
#define TETRIS_ENTRY TG(_TETRIS)
#else
#define TETRIS_ENTRY KC_NO
#endif

#undef SYS60_ROW2
#define SYS60_ROW2  KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), TETRIS_ENTRY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

// Global matrix buffers - MUST be defined here for keymap_introspection.c to link
matrix_row_t raw_matrix[MATRIX_ROWS] = {0};
matrix_row_t matrix[MATRIX_ROWS] = {0};

// Matrix wrapper functions - these are called by QMK's main loop
// The _custom versions are implemented in keyboards/kbdfans/kbd8x_mk3/matrix.c
#include "debounce.h"
extern void matrix_init_custom(void);
extern bool matrix_scan_custom(matrix_row_t out[MATRIX_ROWS]);

void matrix_init(void) {
    matrix_init_custom();
    debounce_init();
}

uint8_t matrix_scan(void) {
    // CUSTOM_MATRIX bypasses quantum/matrix_common.c entirely, so nothing
    // else calls debounce() for us - without this, raw_matrix updates but
    // matrix[] (what keyboard_task() actually reads via matrix_get_row())
    // never does, meaning no keypress can ever register regardless of
    // whether the scan itself is correct.
    bool changed = matrix_scan_custom(raw_matrix);
    changed       = debounce(raw_matrix, matrix, changed);
    return changed;
}

// matrix_get_row() is defined in matrix.c (handles MATRIX_MASKED there)

// Stub for matrix_print - called only when debug_config.matrix is true
// Define in keymap.c to avoid ARM/Thumb linking issues
void matrix_print(void) {
    // Debug print disabled for KBD8x to save memory
    // If needed: print("Matrix state\n"); for debugging
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_TKL_ISO_KBD8X(QWERTZ60,      (BR8_POS_1_3_4_5_8(QWERTZ60,      KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ1]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ1_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ1_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ2]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ2_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ2_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ3]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ3_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ3_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NEOQWERTZ4]  = KEYMAP_TKL_ISO_KBD8X(NEOQWERTZ4_60, (BR8_POS_1_3_4_5_8(NEOQWERTZ4_60, KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED1]      = KEYMAP_TKL_ISO_KBD8X(NOTED1_60,     (BR8_POS_1_3_4_5_8(NOTED1_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED2]      = KEYMAP_TKL_ISO_KBD8X(NOTED2_60,     (BR8_POS_1_3_4_5_8(NOTED2_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED3]      = KEYMAP_TKL_ISO_KBD8X(NOTED3_60,     (BR8_POS_1_3_4_5_8(NOTED3_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_NOTED4]      = KEYMAP_TKL_ISO_KBD8X(NOTED4_60,     (BR8_POS_1_3_4_5_8(NOTED4_60,     KC_LGUI, KC_RGUI, QK_LEAD))),
    [_FN]          = KEYMAP_TKL_ISO_KBD8X(FN60,          (BR8_POS_1_3_4_5_8(FN60,          KC_LGUI, KC_RGUI, QK_LEAD))),
    [_SYS]         = KEYMAP_TKL_ISO_KBD8X(SYS60,         (BR8_POS_1_3_4_5_8(SYS60,         KC_LGUI, KC_RGUI, QK_LEAD))),
    // kein [_RGB] hier - siehe Kommentar beim FN60_ROW4-Override oben.
#ifdef TETRIS_GAME_ENABLE
    [_TETRIS]      = KEYMAP_TKL_ISO_KBD8X(TETRIS60,      (BR8_POS_1_3_4_5_8(TETRIS60,      KC_NO, KC_NO, KC_NO))),
#endif
};
// clang-format on

#include "tap_dance_bindings.inc"

// Kein physischer DIP-Schalter auf diesem Board (und DIP_SWITCH_ENABLE ist
// für kbd8x_mk3 nirgends gesetzt) - Default-Layer-Wahl läuft ausschließlich
// über die DF(...)-Tasten auf der _SYS-Ebene, siehe BELEGUNG.md.
