// keyboards/keychron/q4/iso/keymaps/neo/config.h
#pragma once

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
