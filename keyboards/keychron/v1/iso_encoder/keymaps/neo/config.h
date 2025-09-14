#pragma once
// V1 encoder: identische Limits

// --- EEPROM / Wear-leveling: keymap-level override ---
#ifdef EEPROM_DRIVER
#undef EEPROM_DRIVER
#endif
#define EEPROM_DRIVER wear_leveling

#ifdef WEAR_LEVELING_BACKING_SIZE
#undef WEAR_LEVELING_BACKING_SIZE
#endif
#define WEAR_LEVELING_BACKING_SIZE (8 * 1024) // 8 KiB Flash reserviert

#ifdef WEAR_LEVELING_LOGICAL_SIZE
#undef WEAR_LEVELING_LOGICAL_SIZE
#endif
#define WEAR_LEVELING_LOGICAL_SIZE (4 * 1024) // 4 KiB für VIA sichtbar

#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 12

#undef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_MACRO_COUNT 8

#undef DYNAMIC_MACRO_SIZE
#define DYNAMIC_MACRO_SIZE 256

// Unicode-Input-Modi für UC(...)
#define UNICODE_SELECTED_MODES                                                 \
  UNICODE_MODE_LINUX, UNICODE_MODE_WINDOWS, UNICODE_MODE_WINCOMPOSE

// Mod-/Layer-Tap Tuning
#define TAPPING_TERM 250
#define QUICK_TAP_TERM 90
#define TAPPING_TERM_PER_KEY
#define RETRO_TAPPING

#define LEADER_TIMEOUT 500
#define LEADER_PER_KEY_TIMING

#define COMBO_TERM 40
#define COMBO_VARIABLE_LEN
#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_SHOULD_TRIGGER
