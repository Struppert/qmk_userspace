// leader/leader.h
#pragma once
#include <stdbool.h>
#include QMK_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

void leader_module_start(void);
bool leader_module_end(void);

#ifdef __cplusplus
}
#endif
