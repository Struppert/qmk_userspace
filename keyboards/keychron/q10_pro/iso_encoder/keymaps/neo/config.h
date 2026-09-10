#pragma once
// Q10 Pro Neo-Keymap - 12 dynamische Layer (_QWERTZ, _NEOQWERTZ1-4,
// _NOTED1-4, _FN, _SYS, _RGB), kein Tetris (noch nicht portiert, siehe
// v1_max/TETRIS.md für das Muster falls später gewünscht).

// --- Wear Leveling EFL sauber konfigurieren ---
// Altes q10_pro (Firmware-Port) blieb bewusst bei den alten 2048/4096 Byte
// (siehe keyboards/keychron/q10_pro/config.h im Fork) - reichte für die
// alte 2-Layer-Werksbelegung (default+via), aber NICHT für 12 Neo-Layer:
// 12 Layer * 6 Rows * 16 Cols * 2 Byte allein für die Keymap-Tabelle waeren
// schon > 2048B. Bump auf V1-Max/V3-Max-Werte (6/12 KiB) - ANNAHME: STM32L432
// (Q10 Pro) hat genug freien Flash dafuer, wie STM32F401 (V1/V3 Max) - noch
// nicht durch einen echten Flash-Layout-Vergleich verifiziert, nur durch
// den Compile-Schritt (Linker meldet es, falls es nicht passt).
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
#define DYNAMIC_KEYMAP_LAYER_COUNT 12

#undef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_MACRO_COUNT 12

#undef DYNAMIC_MACRO_SIZE
#define DYNAMIC_MACRO_SIZE 256

// Unicode-Input-Modi für UC(...)
#define UNICODE_SELECTED_MODES                                                \
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
