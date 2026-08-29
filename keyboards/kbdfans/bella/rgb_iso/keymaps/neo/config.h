// VIA: keine Layer im EEPROM
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 4 // Enum hat 12 Layer, aber nur 4 definiert in keymap

// Space = LT(_FN, KC_SPC) (SP_FN, siehe qwertz60.h) - gleiches Tap/Hold-
// Tuning wie bei den Keychron-Boards (z.B. q4), sonst QMK-Default (200ms,
// kein Retro-Tapping).
#define TAPPING_TERM 250
#define TAPPING_TERM_PER_KEY
#define RETRO_TAPPING

// VIA: Layer 1 (_FN) nutzt QK_MACRO_0..QK_MACRO_11 (fn60.h) -> 12 Slots noetig.
// DYNAMIC_KEYMAP_MACRO_COUNT selbst kostet kein zusaetzliches EEPROM (der
// Puffer ist einfach der EEPROM-Rest nach den Layern), nur MACRO_COUNT
// begrenzt wie viele QK_MACRO_n IDs gueltig sind.
#undef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_MACRO_COUNT 12

#ifndef DYNAMIC_KEYMAP_MACRO_MAX_LENGTH
#define DYNAMIC_KEYMAP_MACRO_MAX_LENGTH 16 // später 24/32/40 ausprobieren
#endif

#ifndef VIA_EEPROM_LAYOUT_OPTIONS_SIZE
#define VIA_EEPROM_LAYOUT_OPTIONS_SIZE 0
#endif

// QMK Dynamic Macros (Live-Record) AUS – hat nichts mit VIA-Makros zu tun
#ifdef DYNAMIC_MACRO_SIZE
#undef DYNAMIC_MACRO_SIZE
#endif
#define DYNAMIC_MACRO_SIZE 0
