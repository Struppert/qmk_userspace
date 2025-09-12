// users/neo/os_state.c
#include QMK_KEYBOARD_H
#include "os_state.h"

// Unicode-API nur einbinden, wenn gebaut:
#if defined(UNICODE_ENABLE) || defined(UNICODEMAP_ENABLE)
#  include "unicode.h"   // stellt set_unicode_input_mode(..) bereit
#endif

#include "print.h"

static os_mode_t g_os    = OS_LNX;   // werden bei init gesetzt/überschrieben
static shell_t   g_shell = SH_FISH;

static inline void persist_now(void) {
  userspace_persist_settings((uint8_t)g_os, (uint8_t)g_shell);
}

static inline shell_t default_shell_for(os_mode_t os) {
  switch (os) {
    case OS_WIN: return SH_PWSH; // Windows ⇒ PowerShell
    case OS_MAC: return SH_ZSH;  // macOS  ⇒ zsh
    default:     return SH_FISH; // Linux  ⇒ fish
  }
}

/* ----- OS ----- */

os_mode_t os_current(void) { return g_os; }

void os_set(os_mode_t m) {
  if (m > OS_MAC) m = OS_LNX;
  g_os    = m;
  g_shell = default_shell_for(m);   // ← KOPPLUNG: Shell passend zum OS setzen
  persist_now();
}

void os_cycle(void) {
  g_os = (os_mode_t)((g_os + 1) % 3);
  g_shell = default_shell_for(g_os); // ← ebenfalls koppeln
  persist_now();
}

void os_print(void) {
  if (g_os == OS_WIN) {
    send_string("Windows");
  } else if (g_os == OS_MAC) {
    send_string("macOS");
  } else {
    send_string("Linux");
  }
  tap_code(KC_ENTER);
}

/* ----- SHELL (weiterhin separat möglich) ----- */

shell_t shell_current(void) { return g_shell; }

void shell_set(shell_t sh, bool persist) {
  if (sh > SH_PWSH) sh = SH_BASH;
  g_shell = sh;
  if (persist) persist_now();
}

void shell_cycle(bool persist) {
  g_shell = (shell_t)((g_shell + 1) % 4);
  if (persist) persist_now();
}

void shell_print(void) {
  switch (g_shell) {
    case SH_FISH: send_string("fish"); break;
    case SH_ZSH:  send_string("zsh");  break;
    case SH_PWSH: send_string("pwsh"); break;
    default:      send_string("bash"); break;
  }
  tap_code(KC_ENTER);
}

// -----------------------------------------------------------------------------
// Unicode helper: kompatibel zu alten/neuen QMKs, ohne enum-Types
// -----------------------------------------------------------------------------

// Fallback-Zuordnungen (Makros), falls nur Alt- oder Neu-Variante existiert:
#if defined(UC_LNX) || defined(UC_WIN) || defined(UC_MAC) || defined(UC_WINC)
// neue Bezeichner vorhanden → verwende sie direkt
#  define UC_FOR_LNX   UC_LNX
#  define UC_FOR_WIN   UC_WIN
#  define UC_FOR_MAC   UC_MAC
#  ifdef UC_WINC
#    define UC_FOR_WINC UC_WINC
#  endif
#elif defined(UNICODE_MODE_LINUX) || defined(UNICODE_MODE_WINDOWS) || defined(UNICODE_MODE_MAC) || defined(UNICODE_MODE_WINC)
// nur alte Bezeichner vorhanden
#  define UC_FOR_LNX   UNICODE_MODE_LINUX
#  define UC_FOR_WIN   UNICODE_MODE_WINDOWS
#  define UC_FOR_MAC   UNICODE_MODE_MAC
#  ifdef UNICODE_MODE_WINC
#    define UC_FOR_WINC UNICODE_MODE_WINC
#  endif
#else
// gar nichts definiert → sinnvolle Defaults, damit es kompiliert
#  define UC_FOR_LNX   0
#  define UC_FOR_WIN   1
#  define UC_FOR_MAC   2
#endif

// Mappt beliebige eingehende Werte (alt/neu) auf die "FOR_*"-Konstanten.
// Wir bleiben bewusst bei uint8_t (kein unicode_input_mode_t).
static inline uint8_t map_uc_mode(uint8_t m) {
  // Direkter Treffer (neu)
  #if defined(UC_LNX)
  if (m == UC_LNX) return UC_FOR_LNX;
  #endif
  #if defined(UC_WIN)
  if (m == UC_WIN) return UC_FOR_WIN;
  #endif
  #if defined(UC_MAC)
  if (m == UC_MAC) return UC_FOR_MAC;
  #endif
  #if defined(UC_WINC) && defined(UC_FOR_WINC)
  if (m == UC_WINC) return UC_FOR_WINC;
  #endif

  // Direkter Treffer (alt)
  #if defined(UNICODE_MODE_LINUX)
  if (m == UNICODE_MODE_LINUX) return UC_FOR_LNX;
  #endif
  #if defined(UNICODE_MODE_WINDOWS)
  if (m == UNICODE_MODE_WINDOWS) return UC_FOR_WIN;
  #endif
  #if defined(UNICODE_MODE_MAC)
  if (m == UNICODE_MODE_MAC) return UC_FOR_MAC;
  #endif
  #if defined(UNICODE_MODE_WINC) && defined(UC_FOR_WINC)
  if (m == UNICODE_MODE_WINC) return UC_FOR_WINC;
  #endif

  // Fallback
  return UC_FOR_LNX;
}

// Deine gewünschte, schlanke API:
void uc_set_and_feedback(uint8_t legacy_mode) {
  #if defined(UNICODE_ENABLE) || defined(UNICODEMAP_ENABLE)
  set_unicode_input_mode(map_uc_mode(legacy_mode));
  #else
  (void)legacy_mode; // Unicode nicht gebaut → no-op
  #endif
}

// Praktisch: Unicode passend zum aktuell gewählten OS setzen
void uc_set_for_current_os(void) {
  uint8_t mode = UC_FOR_LNX;
  switch (os_current()) {
    case OS_WIN: mode = UC_FOR_WIN; break;
    case OS_MAC: mode = UC_FOR_MAC; break;
    default:     mode = UC_FOR_LNX; break;
  }
  uc_set_and_feedback(mode);
}
