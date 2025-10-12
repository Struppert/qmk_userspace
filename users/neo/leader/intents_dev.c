// leader/intents_dev.c
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

void intent_dev_env(void) {
  if (is_win()) {
    send_line("call \"C:\\Program Files\\Microsoft Visual "
              "Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\"");
  } else {
    send_line("source ~/.local/share/nvim/env.fish");
  }
}

void intent_build_and_run_debug(void) {
  if (is_win()) {
    send_line(
        "msbuild zis50.sln /p:Configuration=Debug && .\\bin\\Debug\\app.exe");
  } else {
    send_line("cmake --build build --config Debug && ./build/app");
  }
}
