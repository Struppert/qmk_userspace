// leader/intents_wezterm.c
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

void intent_open_wezterm(void) {
  if (is_win()) {
    SEND_STRING(SS_LGUI("r"));
    wait_ms(120);
    send_string("wezterm");
    tap_code(KC_ENTER);
  } else if (is_linux() || is_mac()) {
    send_line("wezterm");
  }
}

void intent_split_pane_h(void) { // horizontaler Split (nebeneinander)
  if (is_win())
    send_line("wezterm cli split-pane -- powershell");
  else
    send_line("wezterm cli split-pane -- fish");
}

void intent_split_pane_v(void) { // vertikaler Split (untereinander)
  if (is_win())
    send_line("wezterm cli split-pane --horizontal -- powershell");
  else
    send_line("wezterm cli split-pane --horizontal -- fish");
}

void intent_kill_pane(void) { send_line("wezterm cli kill-pane"); }
void intent_next_pane_right(void) {
  send_line("wezterm cli activate-pane-direction Right");
}
void intent_prev_pane_left(void) {
  send_line("wezterm cli activate-pane-direction Left");
}

void intent_new_tab_here(void) {
  if (is_win())
    send_line("wezterm cli spawn --cwd . -- powershell");
  else
    send_line("wezterm cli spawn --cwd . -- fish");
}

void intent_close_tab(void) { send_line("wezterm cli close-tab"); }

void intent_activate_tab(uint8_t n) {
  char buf[32];
  snprintf(buf, sizeof(buf), "wezterm cli activate-tab %u", (unsigned)n);
  send_line(buf);
}
