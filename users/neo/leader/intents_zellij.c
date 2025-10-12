// leader/intents_zellij.c
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

void intent_zellij_attach_main(void) { send_line("zellij attach -c main"); }
void intent_zellij_list_sessions(void) { send_line("zellij list-sessions"); }
void intent_zellij_attach_prompt(void) { SEND_STRING("zellij attach "); }
void intent_zellij_kill_session_prompt(void) {
  SEND_STRING("zellij kill-session ");
}
void intent_zellij_rename_session_prompt(void) {
  SEND_STRING("zellij action rename-session ");
}

void intent_zellij_new_tab(void) { send_line("zellij action new-tab"); }
void intent_zellij_prev_tab(void) {
  send_line("zellij action go-to-previous-tab");
}
void intent_zellij_close_tab(void) { send_line("zellij action close-tab"); }

void intent_zellij_go_to_tab(uint8_t n) {
  char buf[32];
  snprintf(buf, sizeof(buf), "zellij action go-to-tab %u", (unsigned)n);
  send_line(buf);
}

void intent_zellij_rename_tab_prompt(void) {
  SEND_STRING("zellij action rename-tab ");
}
void intent_zellij_new_tab_layout_prompt(void) {
  SEND_STRING("zellij action new-tab --layout ");
}

void intent_zellij_new_pane_right(void) {
  send_line("zellij action new-pane -d right");
}
void intent_zellij_new_pane_down(void) {
  send_line("zellij action new-pane -d down");
}
void intent_zellij_new_pane_floating(void) {
  send_line("zellij action new-pane -f");
}
void intent_zellij_close_pane(void) { send_line("zellij action close-pane"); }

void intent_zellij_move_focus(const char *dir) {
  char buf[48];
  snprintf(buf, sizeof(buf), "zellij action move-focus %s", dir);
  send_line(buf);
}
void intent_zellij_resize(const char *dir, int amt) {
  char buf[64];
  snprintf(buf, sizeof(buf), "zellij action resize %s %d", dir, amt);
  send_line(buf);
}

void intent_zellij_toggle_floating_all(void) {
  send_line("zellij action toggle-floating-panes");
}
void intent_zellij_toggle_embed_or_floating(void) {
  send_line("zellij action toggle-pane-embed-or-floating");
}
void intent_zellij_toggle_fullscreen(void) {
  send_line("zellij action toggle-fullscreen");
}
void intent_zellij_toggle_frames(void) {
  send_line("zellij action toggle-pane-frames");
}
