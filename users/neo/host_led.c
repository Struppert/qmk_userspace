// users/neo/host_led.c
#include QMK_KEYBOARD_H
#include "host_led.h"
#include "transport.h"
#include "wireless.h"

void host_led_apply(void) {
  transport_t t = get_transport();
  if (t == TRANSPORT_BLUETOOTH) {
#ifdef BT_INDCATION_LED_MATRIX_LIST
    uint8_t host = wireless_get_host_index();
    if (host >= 1 && host <= 3) {
      static const uint8_t bt_host_leds[] = BT_INDCATION_LED_MATRIX_LIST;
      rgb_matrix_set_color(bt_host_leds[host - 1], 0, 0, 255);
    }
#endif
  } else if (t == TRANSPORT_P2P4) {
#ifdef P24G_INDICATION_LED_INDEX
    rgb_matrix_set_color(P24G_INDICATION_LED_INDEX, 0, 0, 255);
#endif
  }
  // TRANSPORT_USB: kein Wireless-Ziel aktiv, kein Overlay.
}

void host_led_task(void) {
  static uint16_t last = 0;
  if (timer_elapsed(last) < 500) {
    return;
  }
  last = timer_read();
  host_led_apply();
  rgb_matrix_update_pwm_buffers();
}
