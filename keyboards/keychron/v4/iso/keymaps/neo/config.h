// keyboards/keychron/q4/iso/keymaps/neo/config.h
#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// VIA/Dynamic Keymap: wir nutzen 7 Layer (0..6). Ohne diese Vorgabe zeigt VIA
// sonst nur die Standardanzahl an.
#ifdef DYNAMIC_KEYMAP_LAYER_COUNT
#    undef DYNAMIC_KEYMAP_LAYER_COUNT
#endif
#define DYNAMIC_KEYMAP_LAYER_COUNT 7

// ─────────────────────────────────────────────────────────────────────────────
// Unicode-Input-Modus für UC(...) Keycodes (z. B. UC(0x20AC) für "€").
// Du arbeitest primär unter Linux → UC_LNX ist passend.
// Falls du die gleiche Firmware unter Windows/macOS nutzt, ändere hier:
//   Windows + WinCompose:  UC_WINC
//   Windows (alt):         UC_WIN
//   macOS:                 UC_MAC
//   FreeBSD:               UC_BSD
#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_WINDOWS, UNICODE_MODE_WINCOMPOSE

// ─────────────────────────────────────────────────────────────────────────────
// Mod-Tap / Layer-Tap Tuning – verbessert das Verhalten von
//   - SP_FN  (Space: Tap=Space, Hold=FN)
//   - L3_ESC (Esc:  Tap=Esc,   Hold=Neo-3)
// Die Werte sind konservativ gewählt und vermeiden "versehentliche Holds".
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

// Anzahl VIA-Makro-Slots (für QK_MACRO_0..7)
#ifndef DYNAMIC_KEYMAP_MACRO_COUNT
#    define DYNAMIC_KEYMAP_MACRO_COUNT 15
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Nichts weiter nötig. RGB-/VIA-/Unicode-Features werden in rules.mk aktiviert:
//   VIA_ENABLE = yes
//   UNICODE_ENABLE = yes          // für UC(...)
//   RGB_MATRIX_ENABLE = yes       // für RGB_MATRIX_* Keycodes (Q-Serie typisch)
//   // ODER: RGBLIGHT_ENABLE = yes // falls dein Board RGBLIGHT statt MATRIX nutzt
