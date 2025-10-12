// leader/handlers_sed.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "help.h"
#include "intents.h"

void h_s_help(void) { help_sed(); }

void h_sed_help(void) { intent_sed_help(); }
void h_sed_replace(void) { intent_sed_replace_template(); }
void h_sed_filter(void) { intent_sed_filter_template(); }
void h_sed_echo(void) { intent_sed_echo_test(); }
void h_sed_fzf(void) { intent_sed_fzf_files(); }
