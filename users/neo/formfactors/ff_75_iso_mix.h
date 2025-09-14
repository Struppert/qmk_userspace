#pragma once
#include "quantum.h"

// Erwartet pro Layer/MAIN:
//   MAIN##_FROW0    (13 Keys, z. B. ESC, F1..F12, mit trailing comma!)
//   MAIN##_CROW0    ( 2 Keys, z. B. DEL, MUTE, mit trailing comma!)
//   MAIN##_ROW1..4  (Main)
//   MAIN##_CROW1..5 (Center für Rows 1..5, jeweils mit trailing comma!)
//   BRROW5          (expandiertes BR7_* Makro; Bottom-Row 7 Keys)

// clang-format off
#define FF75_ISO_MIX_LIST(MAIN, BRROW5) \
  /*Row0*/ MAIN##_FROW0, MAIN##_CROW0 \
  /*Row1*/ MAIN##_ROW1, MAIN##_CROW1 \
  /*Row2*/ MAIN##_ROW2, MAIN##_CROW2 \
  /*Row3*/ MAIN##_ROW3, MAIN##_CROW3 \
  /*Row4*/ MAIN##_ROW4, MAIN##_CROW4 \
  /*Row5*/ BRROW5,      MAIN##_CROW5

#define KEYMAP_75_ISO_MIX_USING(MAIN, BRROW5, PCB_LAYOUT_MACRO) \
  PCB_LAYOUT_MACRO(FF75_ISO_MIX_LIST(MAIN, BRROW5))

#ifdef LAYOUT_75_iso
#  define KEYMAP_75_ISO_MIX(MAIN, BRROW5) KEYMAP_75_ISO_MIX_USING(MAIN, BRROW5, LAYOUT_75_iso)
#else
#  define KEYMAP_75_ISO_MIX(MAIN, BRROW5) KEYMAP_75_ISO_MIX_USING(MAIN, BRROW5, LAYOUT_tkl_iso)
#endif

