// leader/handlers_wezterm.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

void h_tabs_help(void) { help_tabs(); }
void h_panes_help(void) { help_panes(); }

void h_wt_open(void) { intent_open_wezterm(); }
void h_wt_newtab(void) { intent_new_tab_here(); }
void h_wt_close_tab(void) { intent_close_tab(); }

void h_wt_tab1(void) { intent_activate_tab(0); }
void h_wt_tab2(void) { intent_activate_tab(1); }
void h_wt_tab3(void) { intent_activate_tab(2); }
void h_wt_tab4(void) { intent_activate_tab(3); }
void h_wt_tab5(void) { intent_activate_tab(4); }
void h_wt_tab6(void) { intent_activate_tab(5); }
void h_wt_tab7(void) { intent_activate_tab(6); }
void h_wt_tab8(void) { intent_activate_tab(7); }
void h_wt_tab9(void) { intent_activate_tab(8); }

void h_wt_split_h(void) { intent_split_pane_h(); }
void h_wt_split_v(void) { intent_split_pane_v(); }
void h_wt_pane_kill(void) { intent_kill_pane(); }
void h_wt_pane_next(void) { intent_next_pane_right(); }
void h_wt_pane_prev(void) { intent_prev_pane_left(); }
