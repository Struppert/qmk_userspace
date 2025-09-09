// userspace-init.c
// Initialisiert OS- und Unicode-Zustand nach dem Boot und setzt sinnvolle
// Defaults bei EEPROM-Reset, ohne dein restliches Userspace-Design zu ändern.

#include "os_state.h" // dein Userspace-Modul: OS speichern/laden + Helpers
#include "quantum.h"

// ─────────────────────────────────────────────────────────────────────────────
// 1) Defaults setzen, wenn das EEPROM initialisiert/gelöscht wurde
//    (wird von QMK genau EINMAL aufgerufen, wenn eeconfig (wieder)
//    initialisiert)
// ─────────────────────────────────────────────────────────────────────────────
void eeconfig_init_user(void) {
  // Setze einen sinnvollen Default für deinen Userspace:
  // → Linux als Default-OS (Unicode: Linux), inkl. visuellem Feedback wenn
  // vorhanden.
  //   os_set(...) sollte in deinem Userspace:
  //   - den OS-Zustand persistieren (EEPROM)
  //   - Unicode-Input-Mode passend setzen (oder via Helper, je nach
  //   Implementierung)
  os_set(OS_LNX);
}

// ─────────────────────────────────────────────────────────────────────────────
// 2) Nach dem Keyboard-Init (wenn alle Subsysteme bereit sind):
//    - Userspace-OS-Zustand aus EEPROM laden
//    - Unicode-Modus konsistent zum geladenen OS setzen
//    - (optional) visuelles Feedback anstoßen
// ─────────────────────────────────────────────────────────────────────────────
void keyboard_post_init_user(void) {
  // Lade den zuletzt gesetzten OS-Zustand aus dem EEPROM
  os_load();

  // Unicode-Eingabemodus passend zum OS setzen.
  // Falls du in os_state bereits einen Helper wie uc_set_and_feedback() hast,
  // kannst du alternativ *eine* Zeile aufrufen (siehe Kommentar unten).
#ifdef UNICODE_ENABLE
  switch (os_current()) {
  case OS_WIN:
    set_unicode_input_mode(UNICODE_MODE_WINDOWS);
    break;
  case OS_LNX:
    set_unicode_input_mode(UNICODE_MODE_LINUX);
    break;
  case OS_MAC:
    set_unicode_input_mode(UNICODE_MODE_MACOS);
    break;
  default:
    // Fallback: Linux
    set_unicode_input_mode(UNICODE_MODE_LINUX);
    break;
  }
#endif

  // OPTIONAL (wenn bei dir vorhanden und gewünscht):
  // uc_set_and_feedback(os_current());
  // → setzt Unicode-Mode + RGB/Statusanzeige konsistent in *einem* Schritt
}
