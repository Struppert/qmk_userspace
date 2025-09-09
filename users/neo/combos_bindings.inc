// combos_bindings.inc  (vom board-spezifischen keymap.c inkludieren)
#ifdef COMBO_ENABLE
  #include "process_combo.h"
  #include "keymap_iso_common.h"
  // ---- Combo-Index ----
  enum combos {
    FJ_LEAD,
    COMBO_LEN_ENUM
  };
  uint16_t COMBO_LEN = COMBO_LEN_ENUM;

  // ---- Tastensatz F+J ----
  const uint16_t PROGMEM fj_combo[] = { KC_F, KC_J, COMBO_END };

  // ---- Mapping ----
  combo_t key_combos[] PROGMEM = {
    [FJ_LEAD] = COMBO(fj_combo, QK_LEAD),   // F+J → Leader
  };

  // ---- Nur auf _QWERTZ und _NEOQWERTZ1 erlauben ----
  // Erfordert: #define COMBO_SHOULD_TRIGGER in config.h
  #ifdef COMBO_SHOULD_TRIGGER
    bool combo_should_trigger(uint16_t combo_index, combo_t *combo,
      uint16_t keycode, keyrecord_t *record) {
      if (combo_index == FJ_LEAD) {
        uint8_t l = get_highest_layer(layer_state);
        return (l == _QWERTZ) || (l == _NEOQWERTZ1);
      }
      return true; // alle anderen Combos (falls vorhanden) normal erlauben
    }
    #endif  // COMBO_SHOULD_TRIGGER

  #else
    // Leeres Array kompatibel zu ARRAY_SIZE(...) – Typ egal
    uint8_t key_combos[] PROGMEM = {};
    #endif
