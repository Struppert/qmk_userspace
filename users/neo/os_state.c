// users/neo/os_state.c
#include "os_state.h"

// -------- Persistenz-Format ----------
#define OS_CFG_VERSION 0x01u
static inline uint32_t oscfg_pack(uint8_t os) {
  return (uint32_t)OS_CFG_VERSION | ((uint32_t)os << 8);
}
static inline uint8_t oscfg_unpack(uint32_t val) {
  return ((val & 0xFFu) == OS_CFG_VERSION) ? (uint8_t)((val >> 8) & 0xFFu)
                                           : OS_LNX;
}

// -------- interner Zustand -----------
static os_mode_t g_os_mode = OS_LNX;

// -------- API-Implementierung --------
void os_feedback(os_mode_t os) {
#ifdef RGBLIGHT_ENABLE
  // einfache Farbcodierung: Win=blau, Linux=grün, macOS=weiß/blau (hell)
  switch (os) {
  case OS_WIN:
    rgblight_sethsv_noeeprom(170, 255, 100);
    break;
  case OS_LNX:
    rgblight_sethsv_noeeprom(85, 255, 100);
    break;
  case OS_MAC:
    rgblight_sethsv_noeeprom(0, 0, 180);
    break;
  }
#else
  (void)os;
#endif
}

void os_set(os_mode_t os) {
  g_os_mode = os;
  eeconfig_update_user(oscfg_pack((uint8_t)os));
  os_feedback(os);
}

void os_cycle(void) { os_set((os_mode_t)(((int)g_os_mode + 1) % 3)); }

os_mode_t os_current(void) { return g_os_mode; }

void os_load(void) {
  g_os_mode = (os_mode_t)oscfg_unpack(eeconfig_read_user());
  os_feedback(g_os_mode);
}

// Komfortausgabe fürs aktive Terminal
void os_print(void) {
  switch (g_os_mode) {
  case OS_WIN:
    send_string_P(PSTR("[OS] Windows\n"));
    break;
  case OS_LNX:
    send_string_P(PSTR("[OS] Linux\n"));
    break;
  case OS_MAC:
    send_string_P(PSTR("[OS] macOS\n"));
    break;
  }
}

// Unicode-Helfer mit optionalem RGB-Matrix-Feedback
void uc_set_and_feedback(uint8_t mode) {
  set_unicode_input_mode(mode);
#ifdef RGB_MATRIX_ENABLE
  uint8_t r = 0, g = 0, b = 0;
  switch (mode) {
  case UNICODE_MODE_LINUX:
    g = 20;
    break; // grün
  case UNICODE_MODE_WINDOWS:
    r = 20;
    break; // rot
  case UNICODE_MODE_WINCOMPOSE:
    r = 20;
    b = 20;
    break; // magenta
  default:
    b = 20;
    break; // blau
  }
  rgb_matrix_set_color_all(r, g, b);
#endif
}
