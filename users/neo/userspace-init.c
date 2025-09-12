// users/neo/userspace-init.c
#include QMK_KEYBOARD_H
#include "os_state.h"   // wir erweitern’s gleich um Shell-API

// --- Versionierung / Layout ---
#define USR_MAGIC     0xBEEF
#define USR_VERSION   1

// Bitlayout
#define OS_SHIFT      0
#define OS_MASK       0x3u

#define SH_SHIFT      2
#define SH_MASK       0x3u

#define VER_SHIFT     8
#define VER_MASK      0xFFu

#define MAG_SHIFT     16
#define MAG_MASK      0xFFFFu

static inline uint32_t pack_user(uint8_t os, uint8_t sh) {
  return ((uint32_t)(USR_MAGIC & MAG_MASK) << MAG_SHIFT) |
  ((uint32_t)(USR_VERSION & VER_MASK) << VER_SHIFT) |
  ((uint32_t)(os & OS_MASK) << OS_SHIFT) |
  ((uint32_t)(sh & SH_MASK) << SH_SHIFT);
}

static inline void unpack_user(uint32_t w, uint8_t *os, uint8_t *sh, uint16_t *magic, uint8_t *ver) {
  *magic = (uint16_t)((w >> MAG_SHIFT) & MAG_MASK);
  *ver   = (uint8_t)((w >> VER_SHIFT) & VER_MASK);
  *os    = (uint8_t)((w >> OS_SHIFT) & OS_MASK);
  *sh    = (uint8_t)((w >> SH_SHIFT) & SH_MASK);
}

// Zentraler Commit
static void userspace_commit(uint8_t os, uint8_t sh) {
  eeconfig_update_user(pack_user(os, sh));
}

// Wird aus os_state.c aufgerufen, wenn OS/Shell geändert wird:
void userspace_persist_settings(uint8_t os, uint8_t sh) {
  userspace_commit(os, sh);
}

// Initialisierung der Defaults + Laden aus EEPROM
void eeconfig_init_user(void) {
  // Defaults: OS=LNX, SHELL=FISH (anpassen, falls anders gewünscht)
  userspace_commit(/*os*/1, /*sh*/1);
}

void keyboard_post_init_user(void) {
  uint32_t w = eeconfig_read_user();
  uint8_t os = 1, sh = 1, ver = 0; uint16_t magic = 0;

  unpack_user(w, &os, &sh, &magic, &ver);

  if (magic != USR_MAGIC || ver != USR_VERSION) {
    eeconfig_init_user();
    os = 1; sh = 1; // Defaults (Linux/fish)
  }

  // Erst OS setzen (kann die Shell auto-koppeln) …
  os_set((os_mode_t)os);

  shell_set((shell_t)sh, false);
}
