// users/neo/battery_print.c
#include QMK_KEYBOARD_H
#include "battery.h"
#include "battery_print.h"
#include "lpm.h"
#include "transport.h"
#include <stdio.h>

// Verbindungs-/Pairing-Status (wireless_get_state()) wird hier bewusst NICHT
// mitgedruckt: ist das Board nicht verbunden, kommt der Tastendruck sowieso
// nirgends an - ein Terminal, das diese Zeile empfangen könnte, würde
// genau dann fehlen. Der Host-Slot (BT_HST1-3/P2P4G) ist stattdessen als
// permanentes blaues LED direkt unter der jeweiligen Taste sichtbar (siehe
// overlay_active_host_led() in keymap.c), nicht als Text hier.
static const char *transport_label(void) {
  switch (get_transport()) {
    case TRANSPORT_USB:
      return "USB";
    case TRANSPORT_BLUETOOTH:
      return "BT";
    case TRANSPORT_P2P4:
      return "2.4G";
    default:
      return "?";
  }
}

void battery_print(void) {
  char buf[64];
  snprintf(buf, sizeof(buf), "Akku: %u%% (%umV%s) | %s\n",
           (unsigned)battery_get_percentage(),
           (unsigned)battery_get_voltage(),
           usb_power_connected() ? ", laedt" : "", transport_label());
  send_string(buf);
}
