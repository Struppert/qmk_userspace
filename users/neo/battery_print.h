#pragma once
// Gibt den aktuellen Akkustand (Prozent + Spannung) als Text ins gerade
// fokussierte Terminal/Fenster aus - per send_string(), wie tetris.c es
// fürs Terminal-Rendering tut. Im Gegensatz zum LED-Balken (BAT_LVL) ist
// das transport-unabhängig (USB/BT/2.4G), da es einfach Tastendrücke sind.
// Nur kompiliert wenn LK_WIRELESS_ENABLE gesetzt ist (siehe rules.mk).
void battery_print(void);
