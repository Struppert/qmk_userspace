// users/neo/sendstring_de.c
//
// Provides the German ascii_to_keycode/shift/altgr/dead LUTs used by
// send_string()/SEND_STRING() and by VIA's dynamic-macro playback
// (quantum/dynamic_keymap.c calls the same global tables). Needs to be
// compiled into EVERY board's firmware exactly once - previously this
// only happened incidentally via leader/os_shell.c's own include, which
// is excluded from the memory-constrained (kbd8x_mk3/bella) build. That
// left those boards on QMK's default US-layout LUT, so any VIA macro
// containing a literal '\' (needs AltGr+ß under German layout) played
// back as the raw US backslash keycode instead - shown as '#' on a
// German host. Moved here so it's always linked, in exactly one
// translation unit, regardless of board.
#define SENDSTRING_LANGUAGE de_DE
#include "sendstring_german.h"
