#pragma once
// V1 encoder: identische Limits

// --- Wear Leveling EFL sauber konfigurieren ---
#undef EEPROM_DRIVER
#define EEPROM_DRIVER wear_leveling

#ifndef WEAR_LEVELING_DRIVER
#define WEAR_LEVELING_DRIVER efl
#endif

#undef WEAR_LEVELING_LOGICAL_SIZE
#define WEAR_LEVELING_LOGICAL_SIZE (6 * 1024) // 6 KiB logisch

// WICHTIG: ohne #ifndef, hart überschreiben!
#undef WEAR_LEVELING_BACKING_SIZE
#define WEAR_LEVELING_BACKING_SIZE                                             \
  (12 * 1024) // 12 KiB Backing (>= 2x, Vielfaches)

#ifndef BACKING_STORE_WRITE_SIZE
#define BACKING_STORE_WRITE_SIZE 8
#endif

// (danach erst deine anderen Defines wie DYNAMIC_KEYMAP_LAYER_COUNT usw.)

#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 13

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
