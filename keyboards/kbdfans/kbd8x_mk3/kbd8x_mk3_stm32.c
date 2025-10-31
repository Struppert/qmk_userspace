#include "kbd8x_mk3_stm32.h"
void matrix_init_kb(void){matrix_init_custom();matrix_init_user();}
bool matrix_scan_kb(void){bool changed=matrix_scan_custom(matrix);if(changed)matrix_scan_user();return changed;}
