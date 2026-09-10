// keyboards/keychron/q10_pro/iso_encoder/keymaps/neo/keymap.c
// Q10 Pro = 91-Tasten ISO+Knob, BT+USB (kein 2.4G in diesem Port, siehe
// keyboards/keychron/q10_pro/BLUETOOTH.md), Makro-Spalte links (M1-M5).

// ----- Board-Layout-Auswahl (Q10 Pro ISO+Encoder) -----
#define LAYOUT_ISO LAYOUT_91_iso

#include QMK_KEYBOARD_H
// QMK_KEYBOARD_H bringt BT_HST1/etc. NICHT mit - siehe v1_max/BLUETOOTH.md,
// Abschnitt "Behobener Bug": #ifndef-Fallbacks auf echte Enum-Konstanten
// funktionieren nie. Referenz-Keymap (iso_encoder/keymaps/default) inkludiert
// deshalb zusätzlich explizit keychron_common.h.
#include "keychron_common.h"

#include "keymap_iso_common.h"
#include "tap_dance_ids.h"

// Layout-Bausteine (60%-Main + 5er-Row5-Core, hier nur ROW1-4 genutzt -
// Row5 ist beim Q10 Pro wegen der geteilten Leertaste fix, siehe
// formfactors/ff_91_iso_q10_pro.h)
#include "layouts/fn60.h"
#include "layouts/neoqwertz60.h"
#include "layouts/noted60.h"
#include "layouts/qwertz60.h"
#include "layouts/rgb60.h"
#include "layouts/sys60.h"

// 91-Tasten-ISO-Formfactor mit Makro-Spalte/geteilter Leertaste
#include "formfactors/ff_91_iso_q10_pro.h"

// BT-Host-Wahl (BT_HST1-3) und Akkustand (BAT_LVL) auf _SYS - kein P2P4G
// (dieser Firmware-Port unterstützt nur BT+USB, kein 2.4G-Dongle, siehe
// BLUETOOTH.md) und kein TETRIS_ENTRY (noch nicht portiert). SYS60_ROW2 ist
// geteilt (layouts/sys60.h), daher lokaler Override statt die Datei für
// andere Boards anzufassen - gleiches Muster wie V1 Max/V3 Max.
#undef SYS60_ROW2
#define SYS60_ROW2  KC_TAB, DF(_QWERTZ), DF(_NEOQWERTZ1), DF(_NOTED1), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, BT_HST1, BT_HST2, BT_HST3, BAT_LVL,

// Combos (oder Stub, oder aus wenn COMBO_ENABLE=no)
#include "combos_bindings.inc"
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTZ]      = KEYMAP_91_ISO_Q10PRO(QWERTZ60),
    [_NEOQWERTZ1]  = KEYMAP_91_ISO_Q10PRO(NEOQWERTZ1_60),
    [_NEOQWERTZ2]  = KEYMAP_91_ISO_Q10PRO(NEOQWERTZ2_60),
    [_NEOQWERTZ3]  = KEYMAP_91_ISO_Q10PRO(NEOQWERTZ3_60),
    [_NEOQWERTZ4]  = KEYMAP_91_ISO_Q10PRO(NEOQWERTZ4_60),
    [_NOTED1]      = KEYMAP_91_ISO_Q10PRO(NOTED1_60),
    [_NOTED2]      = KEYMAP_91_ISO_Q10PRO(NOTED2_60),
    [_NOTED3]      = KEYMAP_91_ISO_Q10PRO(NOTED3_60),
    [_NOTED4]      = KEYMAP_91_ISO_Q10PRO(NOTED4_60),
    [_FN]          = KEYMAP_91_ISO_Q10PRO(FN60),
    [_SYS]         = KEYMAP_91_ISO_Q10PRO(SYS60),
    [_RGB]         = KEYMAP_91_ISO_Q10PRO(RGB60),
};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [_QWERTZ]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ1]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ2]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ3]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NEOQWERTZ4]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED1]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED2]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED3]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_NOTED4]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_FN]          = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_SYS]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [_RGB]         = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

#include "tap_dance_bindings.inc"

// _SYS-Layer-Indikator: rot solide, bei aktivem Bluetooth-Transport blau
// statt rot, dabei blinkt zusätzlich die Taste des gerade gewählten
// BT-Host-Slots (BT_HST1-3). Ersetzt für die Dauer der _SYS-Ebene den
// normal aktiven RGB-Matrix-Effekt komplett (return false). 1:1 vom V1
// Max/V3 Max übernommen (Logik ist unabhängig vom Formfaktor), nutzt
// BT_INDCATION_LED_MATRIX_LIST aus keyboards/keychron/q10_pro/iso_encoder/
// config.h ({17, 18, 19} - beim Portieren aus dem alten HOST_LED_MATRIX_LIST
// übernommen, noch nicht per Raw-HID gegen die echten BT_HST-Tasten
// verifiziert, siehe BLUETOOTH.md).
#if defined(RGB_MATRIX_ENABLE) && defined(KC_BLUETOOTH_ENABLE)
#include "transport.h"
#include "wireless.h"

bool rgb_matrix_indicators_user(void) {
    if (!layer_state_is(_SYS)) {
        return true;
    }

    if (get_transport() == TRANSPORT_BLUETOOTH) {
        rgb_matrix_set_color_all(0, 0, 255);

#ifdef BT_INDCATION_LED_MATRIX_LIST
        uint8_t host = wireless_get_host_index();
        if (host >= 1 && host <= 3) {
            static const uint8_t bt_host_leds[] = BT_INDCATION_LED_MATRIX_LIST;
            if ((timer_read() / 300) % 2) {
                rgb_matrix_set_color(bt_host_leds[host - 1], 255, 255, 255);
            } else {
                rgb_matrix_set_color(bt_host_leds[host - 1], 0, 0, 0);
            }
        }
#endif
    } else {
        rgb_matrix_set_color_all(255, 0, 0);
    }

    return false;
}
#endif

__attribute__((weak)) bool dip_switch_update_user(uint8_t index, bool active) {
  if (index == 0) {
    // Konvention: active == Mac-Stellung
    default_layer_set(1UL << (active ? _NEOQWERTZ1 : _QWERTZ));
    return false; // Event verarbeitet
  }
  return true;
}
