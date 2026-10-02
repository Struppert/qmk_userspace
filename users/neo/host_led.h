#pragma once
// Dauerhafte blaue LED unter der Taste des aktuell aktiven Wireless-Ziels
// (BT_HST1-3/P2P4G) - unabhängig vom normalen RGB-Matrix-Rendering.
//
// host_led_apply() schreibt nur die Farbe ins Framebuffer (für die
// rgb_matrix_indicators_user()/rgb_matrix_none_indicators_user()-Hooks in
// keymap.c). host_led_task() ist zusätzlich nötig, weil Keychrons eigene
// Wireless-Firmware die Hintergrundbeleuchtung ein paar Sekunden nach der
// Verbindungs-Animation wieder abschaltet (um einen manuell per RGB_TOG
// gewählten "aus"-Zustand zu respektieren) - das passiert über eigene,
// von unserem Hook unabhängige Timer/Transitions. host_led_task() schreibt
// daher zusätzlich alle 500ms direkt in den LED-Treiber-Puffer
// (rgb_matrix_update_pwm_buffers(), bypasst rgb_matrix_task()s Enable-
// Gating komplett) - aus housekeeping_task_user() aufrufen.
void host_led_apply(void);
void host_led_task(void);
