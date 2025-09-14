// users/neo/userspace-init.c
#include QMK_KEYBOARD_H
#include "os_state.h"

// ──────────────────────────────────────────────────────────────────────────────
// Persistentes User-Wort (32 Bit) Layout
// [31:16] MAGIC, [15:8] VERSION, [7:6] reserved, [5:4] reserved, [3:2] SHELL, [1:0] OS
// ──────────────────────────────────────────────────────────────────────────────
#define USR_MAGIC     0xBEEF
#define USR_VERSION   1

#define OS_SHIFT      0
#define OS_MASK       0x3u

#define SH_SHIFT      2
#define SH_MASK       0x3u

#define VER_SHIFT     8
#define VER_MASK      0xFFu

#define MAG_SHIFT     16
#define MAG_MASK      0xFFFFu

static inline uint32_t pack_user(uint8_t os, uint8_t sh) {
  uint32_t w = 0;
  w |= ((uint32_t)(os & OS_MASK)) << OS_SHIFT;
  w |= ((uint32_t)(sh & SH_MASK)) << SH_SHIFT;
  w |= ((uint32_t)(USR_VERSION & VER_MASK)) << VER_SHIFT;
  w |= ((uint32_t)(USR_MAGIC & MAG_MASK)) << MAG_SHIFT;
  return w;
}

static inline void unpack_user(uint32_t w, uint8_t *os, uint8_t *sh, uint16_t *magic, uint8_t *ver) {
  if (os)    *os    = (uint8_t)((w >> OS_SHIFT)  & OS_MASK);
  if (sh)    *sh    = (uint8_t)((w >> SH_SHIFT)  & SH_MASK);
  if (ver)   *ver   = (uint8_t)((w >> VER_SHIFT) & VER_MASK);
  if (magic) *magic = (uint16_t)((w >> MAG_SHIFT) & MAG_MASK);
}

// ──────────────────────────────────────────────────────────────────────────────
// QMK EECONFIG Hooks
// ──────────────────────────────────────────────────────────────────────────────
void eeconfig_init_user(void) {
  // Defaults: OS = Linux (1), Shell = fish (1)
  uint8_t os = (uint8_t)OS_LNX;
  uint8_t sh = (uint8_t)SH_FISH;
  uint32_t w = pack_user(os, sh);
  eeconfig_update_user(w);
}

void userspace_persist_settings(uint8_t os, uint8_t sh) {
  // Bewahrt MAGIC/VER, aktualisiert OS/SHELL
  uint32_t w_old = eeconfig_read_user();
  uint16_t magic = 0; uint8_t ver = 0, os_old = 0, sh_old = 0;
  unpack_user(w_old, &os_old, &sh_old, &magic, &ver);

  if (magic != USR_MAGIC || ver != USR_VERSION) {
    // Wenn veraltet/leer: mit Defaults neu initialisieren
    eeconfig_init_user();
  }
  uint32_t w_new = pack_user(os, sh);
  eeconfig_update_user(w_new);
}

// ──────────────────────────────────────────────────────────────────────────────
// Init: beim Booten OS/Shell aus EEPROM laden und Anwenden
// ──────────────────────────────────────────────────────────────────────────────
void matrix_init_user(void) {
  uint32_t w = eeconfig_read_user();

  uint8_t os = (uint8_t)OS_LNX;   // Defaults
  uint8_t sh = (uint8_t)SH_FISH;
  uint16_t magic = 0; uint8_t ver = 0;

  unpack_user(w, &os, &sh, &magic, &ver);

  if (magic != USR_MAGIC || ver != USR_VERSION) {
    // Eeprom nicht initialisiert oder andere Version → Defaults setzen
    eeconfig_init_user();
    os = (uint8_t)OS_LNX;
    sh = (uint8_t)SH_FISH;
  }

  // Erst OS setzen (koppelt im Router ggf. Default-Shell), dann Shell explizit
  os_set((os_mode_t)os);
  shell_set((shell_t)sh, false);  // false: hier nicht erneut persistieren
}

// ──────────────────────────────────────────────────────────────────────────────
// WICHTIG: KEIN doppeltes keyboard_post_init_user!
// Falls du hier noch Board-spezifisches LED-/RGB-Init brauchst,
// dann aktiviere eine EINZIGE Definition und schütze sie, falls
// KEYMAP_INTROSPECTION_ENABLE verwendet wird.
// ──────────────────────────────────────────────────────────────────────────────
#if defined(KEYMAP_INTROSPECTION_ENABLE)
// Wenn Introspection aktiv ist, lassen wir keyboard_post_init_user leer/weg,
// um LTO-Multiple-Definitionen zu vermeiden.
#else
void keyboard_post_init_user(void) {
  // optional: RGB-Init, Hinweise, etc. Aktuell bewusst leer.
}
#endif
