// keyboards/keychron/q4/iso/keymaps/neo/config.h
#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// VIA/Dynamic Keymap: wir nutzen 7 Layer (0..6). Ohne diese Vorgabe zeigt VIA
// sonst nur die Standardanzahl an.
#ifdef DYNAMIC_KEYMAP_LAYER_COUNT
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#endif
#define DYNAMIC_KEYMAP_LAYER_COUNT 8

// ─────────────────────────────────────────────────────────────────────────────
// Unicode-Input-Modus für UC(...) Keycodes (z. B. UC(0x20AC) für "€").
// Du arbeitest primär unter Linux → UC_LNX ist passend.
// Falls du die gleiche Firmware unter Windows/macOS nutzt, ändere hier:
//   Windows + WinCompose:  UC_WINC
//   Windows (alt):         UC_WIN
//   macOS:                 UC_MAC
//   FreeBSD:               UC_BSD
#define UNICODE_SELECTED_MODES                                                 \
  UNICODE_MODE_LINUX, UNICODE_MODE_WINDOWS, UNICODE_MODE_WINCOMPOSE
