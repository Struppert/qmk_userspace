// leader/handlers_git.c
#include QMK_KEYBOARD_H
#include "handlers.h"
#include "intents.h"

// simple wrappers mapping leader actions to intents / send_line

void h_git_status(void) { send_line("git status"); }
void h_git_add_all(void) { send_line("git add -A"); }
void h_git_diff(void) { send_line("git diff"); }
void h_git_diff_staged(void) { send_line("git diff --staged"); }

void h_git_commit_empty(void) {
  SEND_STRING("git commit -m \"\"");
  tap_code(KC_LEFT);
}
void h_git_commit_fix(void) { SEND_STRING("git commit -m \"fix: "); }
void h_git_commit_feat(void) { SEND_STRING("git commit -m \"feat: "); }
void h_git_commit_refactor(void) { SEND_STRING("git commit -m \"refactor: "); }
void h_git_commit_test(void) { SEND_STRING("git commit -m \"test: "); }
void h_git_commit_docs(void) { SEND_STRING("git commit -m \"docs: "); }

void h_git_push(void) { send_line("git push"); }
void h_git_push_upstream(void) {
  send_line("git push --set-upstream origin HEAD");
}
void h_git_push_tags(void) { send_line("git push --tags"); }
void h_git_push_force_lease(void) { send_line("git push --force-with-lease"); }

void h_git_log_short(void) {
  send_line("git log --oneline --graph --decorate -n 30");
}
void h_git_log_all(void) {
  send_line("git log --all --decorate --oneline --graph");
}

void h_git_branch_new(void) { SEND_STRING("git switch -c "); }
void h_git_branch_back(void) { send_line("git switch -"); }
void h_git_branch_del(void) { SEND_STRING("git branch -D "); }

void h_git_rebase_i(void) { SEND_STRING("git rebase -i HEAD~"); }

void h_git_tag_annot(void) { SEND_STRING("git tag -a v"); }
void h_git_tags_push(void) { send_line("git push --tags"); }

void h_git_grep_base(void) { send_cmd_with_empty_quotes("git grep -n -I -e "); }
void h_git_grep_i(void) { send_cmd_with_empty_quotes("git grep -n -I -i -e "); }
void h_git_grep_w(void) { send_cmd_with_empty_quotes("git grep -n -I -w -e "); }
void h_git_grep_l(void) { send_cmd_with_empty_quotes("git grep -I -l -e "); }
void h_gitgrep_fzf(void) { intent_gitgrep_fzf_open_editor_line(); }
void h_git_grep_paths(void) {
  SEND_STRING("git grep -n -I -e \"\" -- ");
  tap_code(KC_LEFT);
}

void h_git_pickaxe_S(void) {
  send_cmd_with_empty_quotes("git log -S ");
  send_string(" --patch --stat");
}
void h_git_pickaxe_G(void) {
  send_cmd_with_empty_quotes("git log -G ");
  send_string(" --patch --stat");
}
