// VIA: keine Layer im EEPROM
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 12 // <- 0 = keine VIA-Layer gespeichert

// VIA: nur wenige, kurze Makros
#undef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_MACRO_COUNT 4 // später hochdrehen (6/8), wenn es passt

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
