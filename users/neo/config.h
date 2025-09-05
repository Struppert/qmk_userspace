// users/neo/config.h
#pragma once

// VIA: wir nutzen 8 Layer (0..7)
#ifdef DYNAMIC_KEYMAP_LAYER_COUNT
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#endif
#define DYNAMIC_KEYMAP_LAYER_COUNT 12

#define DYNAMIC_MACRO_SIZE 512

// Unicode-Input-Modi für UC(...)
#define UNICODE_SELECTED_MODES                                                 \
  UNICODE_MODE_LINUX, UNICODE_MODE_WINDOWS, UNICODE_MODE_WINCOMPOSE

// Mod-/Layer-Tap Tuning
#define TAPPING_TERM 250
#define QUICK_TAP_TERM 90
#define TAPPING_TERM_PER_KEY
#define RETRO_TAPPING

// VIA-Makro-Slots (QK_MACRO_0..)
#ifndef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_MACRO_COUNT 15
#endif
