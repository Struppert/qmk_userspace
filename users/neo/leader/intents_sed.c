// leader/intents_sed.c
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

// zeigt sed help
void intent_sed_help(void) { send_line_os("sed --help", "sed --help", NULL); }

// einfache inline-Ersetzung (Template)
void intent_sed_replace_template(void) {
  SEND_STRING("sed -i 's/foo/bar/g' file.txt");
  tap_code(KC_LEFT); // Cursor vor 'file.txt'
}

// pattern print (filter)
void intent_sed_filter_template(void) {
  SEND_STRING("sed -n '/pattern/p' file.txt");
  tap_code(KC_LEFT);
}

// interactive echo
void intent_sed_echo_test(void) {
  SEND_STRING("echo 'text' | sed 's/foo/bar/'");
}

// fzf + sed kombinieren
void intent_sed_fzf_files(void) {
  send_line_os(
      "(rg --files || find . -type f) | fzf | xargs -I{} sed -n '/pattern/p' "
      "{}",
      "(Get-ChildItem -Recurse -File | ForEach-Object { $_.FullName }) | "
      "fzf | ForEach-Object { if($_){ sed -n '/pattern/p' $_ } }",
      NULL);
}
