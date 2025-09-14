#pragma once
#include "quantum.h"
#include "row5_core.h"

// ───────────────────────────────────────────────
// layout_core.h – gemeinsamer Kern für Layout-Dateien
//
// Ein Layout definiert:
//   MAIN##_ROW1 .. MAIN##_ROW4  (jeweils mit trailing comma)
//   DECLARE_ROW5_CORE(MAIN, LCTL, LALT, SPACE, RALT_or_L4MO, RCTL)
//
// Keine F-Row, kein Center/Numpad hier – das gehört in formfactors/
// ───────────────────────────────────────────────
