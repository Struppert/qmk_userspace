// leader/intents_rg_fzf.c
#include QMK_KEYBOARD_H
#include "intents.h"
#include "os_shell.h"

void intent_ripgrep_project(void) {
  send_cmd_with_empty_quotes("rg -n --hidden --glob '!.git' ");
}

void intent_rg_fzf_open_editor_line(void) {
  if (is_win()) {
    send_line(
        "rg --line-number --no-heading -S \"\" | fzf | ForEach-Object { $p = "
        "$_ -split \":\"; if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }");
    tap_code(KC_LEFT);
  } else {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    tap_code(KC_LEFT);
  }
}

void intent_fzf_files_preview_open(void) {
  if (is_win()) {
    send_line("(git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files }) | "
              "fzf --ansi --preview 'bat --style=numbers --color=always {} | "
              "head -500' | ForEach-Object { if($_){ nvim $_ } }");
  } else {
    SEND_STRING("(git ls-files 2>/dev/null || rg --files) | "
                "fzf --ansi --preview 'bat --style=numbers --color=always {} | "
                "head -500' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
  }
}

void intent_fzf_files_multi_open(void) {
  if (is_win()) {
    send_line("(git ls-files 2>$null; if($LASTEXITCODE -ne 0){ rg --files }) | "
              "fzf -m | ForEach-Object { if($_){ nvim $_ } }");
  } else {
    SEND_STRING("(git ls-files 2>/dev/null || rg --files) | fzf -m | ");
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
    SEND_STRING("xargs -r ${EDITOR:-nvim}\n");
#else
    SEND_STRING("xargs -r nvim\n");
#endif
  }
}

void intent_fzf_dirs_cd(void) {
  if (is_win()) {
    send_line(
        "Get-ChildItem -Recurse -Directory | ForEach-Object { $_.FullName } | "
        "fzf | ForEach-Object { if($_){ Set-Location $_ } }");
  } else {
    send_line("(fd -t d -H . 2>/dev/null || find . -type d -not -path "
              "'*/\\.git/*') | fzf | xargs -r -I{} sh -lc 'cd \"{}\"'");
  }
}

void intent_rg_fzf_preview_open_editor_line(void) {
  if (is_win()) {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi --delimiter=: "
                "--preview 'bat --style=numbers --color=always "
                "--highlight-line {2} {1} | head -500' | "
                "ForEach-Object { $p = $_ -split \":\"; if ($p.Length -ge 2) { "
                "nvim \"+$($p[1])\" $p[0] } }\n");
    tap_code(KC_LEFT);
  } else {
    SEND_STRING("rg --line-number --no-heading --color=always -S \"\" | "
                "fzf --ansi --delimiter=: "
                "--preview 'bat --style=numbers --color=always "
                "--highlight-line {2} {1} | head -500' | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    tap_code(KC_LEFT);
  }
}

void intent_rg_context_around3(void) {
  SEND_STRING("rg -n -C 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}
void intent_rg_context_before3(void) {
  SEND_STRING("rg -n -B 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}
void intent_rg_context_after3(void) {
  SEND_STRING("rg -n -A 3 --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}

void intent_rg_multiline(void) {
  SEND_STRING("rg -n -U --hidden --glob '!.git' \"\"");
  tap_code(KC_LEFT);
}

void intent_rg_types_cpp(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t c -t cpp -t h -t hpp \"\"");
  tap_code(KC_LEFT);
}
void intent_rg_types_rust(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t rust \"\"");
  tap_code(KC_LEFT);
}
void intent_rg_types_go(void) {
  SEND_STRING("rg -n --hidden --glob '!.git' -t go \"\"");
  tap_code(KC_LEFT);
}

void intent_gitgrep_fzf_open_editor_line(void) {
  if (is_win()) {
    send_line("git grep -n -I -e \"\" | fzf | ForEach-Object { $p = $_ -split "
              "\":\"; if ($p.Length -ge 2) { nvim \"+$($p[1])\" $p[0] } }");
    tap_code(KC_LEFT);
  } else {
    SEND_STRING("git grep -n -I -e \"\" | "
                "fzf --ansi | "
                "awk -F: '{print \"+\"$2\" \"$1}' | "
#if defined(UNICODEMAP_ENABLE) || defined(UNICODE_ENABLE)
                "xargs -r ${EDITOR:-nvim}\n"
#else
                "xargs -r nvim\n"
#endif
    );
    tap_code(KC_LEFT);
  }
}
