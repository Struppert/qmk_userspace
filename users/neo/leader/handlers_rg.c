// leader/handlers_rg.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

void h_rg_help(void) { help_rg(); }
void h_rg_base(void) { intent_ripgrep_project(); }
void h_rg_smart(void) {
  send_cmd_with_empty_quotes("rg -n -S --hidden --glob '!.git' ");
}
void h_rg_gitfiles(void) {
  send_cmd_with_empty_quotes("git ls-files | rg -n --hidden --glob '!.git' ");
}
void h_rg_less(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' \"\" | less -R");
  tap_code(KC_LEFT);
}
void h_rg_go_glob(void) {
  send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.go' ");
}
void h_rg_rs_glob(void) {
  send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' -g '*.rs' ");
}
void h_rg_fzf_edit(void) { intent_rg_fzf_open_editor_line(); }
void h_rg_ctx_C3(void) { intent_rg_context_around3(); }
void h_rg_ctx_B3(void) { intent_rg_context_before3(); }
void h_rg_ctx_A3(void) { intent_rg_context_after3(); }
void h_rg_multiline(void) { intent_rg_multiline(); }
void h_rg_types_cpp(void) { intent_rg_types_cpp(); }
void h_rg_types_rust(void) { intent_rg_types_rust(); }
void h_rg_types_go(void) { intent_rg_types_go(); }
void h_rg_fzf_preview(void) { intent_rg_fzf_preview_open_editor_line(); }
