#pragma once
// Tetris-Spiellogik + Terminal-Rendering für den _TETRIS-Layer.
// Nur kompiliert wenn TETRIS_GAME_ENABLE gesetzt ist (siehe rules.mk).
#include <stdbool.h>

void tetris_start(void);
void tetris_stop(void);
bool tetris_is_active(void);
void tetris_task(void); // aus housekeeping_task_user() aufrufen

void tetris_input_left(void);
void tetris_input_right(void);
void tetris_input_rotate(void);
void tetris_input_soft_drop(void);
void tetris_input_hard_drop(void);
