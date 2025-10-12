// leader/handlers_zellij.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

void h_j_help(void) { help_zellij(); }

void h_j_attach_main(void) { intent_zellij_attach_main(); }
void h_j_list(void) { intent_zellij_list_sessions(); }
void h_j_attach_prompt(void) { intent_zellij_attach_prompt(); }
void h_j_kill_prompt(void) { intent_zellij_kill_session_prompt(); }
void h_j_rename_prompt(void) { intent_zellij_rename_session_prompt(); }

void h_j_tab_new(void) { intent_zellij_new_tab(); }
void h_j_tab_prev(void) { intent_zellij_prev_tab(); }
void h_j_tab_close(void) { intent_zellij_close_tab(); }
void h_j_tab_rename_prompt(void) { intent_zellij_rename_tab_prompt(); }
void h_j_tab_layout_prompt(void) { intent_zellij_new_tab_layout_prompt(); }
void h_j_tab1(void) { intent_zellij_go_to_tab(1); }
void h_j_tab2(void) { intent_zellij_go_to_tab(2); }
void h_j_tab3(void) { intent_zellij_go_to_tab(3); }
void h_j_tab4(void) { intent_zellij_go_to_tab(4); }
void h_j_tab5(void) { intent_zellij_go_to_tab(5); }
void h_j_tab6(void) { intent_zellij_go_to_tab(6); }
void h_j_tab7(void) { intent_zellij_go_to_tab(7); }
void h_j_tab8(void) { intent_zellij_go_to_tab(8); }
void h_j_tab9(void) { intent_zellij_go_to_tab(9); }

void h_j_pane_right(void) { intent_zellij_new_pane_right(); }
void h_j_pane_down(void) { intent_zellij_new_pane_down(); }
void h_j_pane_float(void) { intent_zellij_new_pane_floating(); }
void h_j_pane_close(void) { intent_zellij_close_pane(); }

void h_j_focus_left(void) { intent_zellij_move_focus("left"); }
void h_j_focus_right(void) { intent_zellij_move_focus("right"); }
void h_j_focus_up(void) { intent_zellij_move_focus("up"); }
void h_j_focus_down(void) { intent_zellij_move_focus("down"); }

void h_j_resize_left(void) { intent_zellij_resize("left", 2); }
void h_j_resize_right(void) { intent_zellij_resize("right", 2); }
void h_j_resize_up(void) { intent_zellij_resize("up", 1); }
void h_j_resize_down(void) { intent_zellij_resize("down", 1); }

void h_j_toggle_floating_all(void) { intent_zellij_toggle_floating_all(); }
void h_j_toggle_embed(void) { intent_zellij_toggle_embed_or_floating(); }
void h_j_toggle_full(void) { intent_zellij_toggle_fullscreen(); }
void h_j_toggle_frames(void) { intent_zellij_toggle_frames(); }
