// leader/handlers_ctx.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "help.h"

static void ctx(uint16_t a) {
  uint16_t p[1] = {a};
  leader_print_help_for_prefix(p, 1);
}

void h_ctx_T(void) { ctx(KC_T); }
void h_ctx_P(void) { ctx(KC_P); }
void h_ctx_G(void) { ctx(KC_G); }
void h_ctx_F(void) { ctx(KC_F); }
void h_ctx_R(void) { ctx(KC_R); }
void h_ctx_Z(void) { ctx(KC_Z); }
void h_ctx_Y(void) { ctx(KC_Y); }
void h_ctx_J(void) { ctx(KC_J); }
