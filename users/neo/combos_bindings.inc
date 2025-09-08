// combos_bindings.inc  (vom board-spezifischen keymap.c inkludieren)
#ifdef COMBO_ENABLE
  #include "process_combo.h"
  combo_t key_combos[] PROGMEM = {};   // dein echter Content oder leer
#else
  // Leeres Array kompatibel zu ARRAY_SIZE(...) – Typ egal
  uint8_t key_combos[] PROGMEM = {};
#endif
