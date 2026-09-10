#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// Q10 Pro (LAYOUT_91_iso, 91 Tasten, ISO+Knob) - physische Eigenheiten
// gegenüber einem generischen 60%-Kern:
//  - Eine zusätzliche Makro-Spalte ganz links (M1-M5, matrix-Spalte A) auf
//    jeder Zeile 1-5 - siehe Q10PRO_MC_* unten, fix auf jeder Ebene, nicht
//    Teil des per-Layer MAIN##_ROWx-Inhalts.
//  - Row0 hat vor dem Esc noch eine dedizierte Mute-Taste (Spalte A) - laut
//    Keychrons eigenem Referenz-Keymap fix KC_MUTE, kein Neo-Inhalt.
//  - Row2 hat KEINE Spalte O (matrix[2,14] existiert elektrisch zwar, ist
//    aber die hochkant stehende ISO-Enter-Taste, die im generierten
//    LAYOUT_91_iso-Makro nicht in Row2s eigenem Argumentblock steht,
//    sondern zwischen Row3s O und P eingeschoben ist - siehe
//    default_keyboard.h: "...k3N, k3O, k2O, k3P". Row3 hat dafür eine Lücke
//    bei Spalte H (zwischen G und I, kein k3H-Parameter) UND diesen
//    zusätzlichen k2O-Slot direkt vor k3P - beides zusammen ergibt exakt
//    die physische ISO-Enter-Taste (matrix[2,14], visuell zwischen Row2 und
///   Row3 stehend). ROW3_DROP_ENT (von ff_tkl_iso_kbd8x_mk3.h übernommenes
//    Muster) verwirft das eigene trailing KC_ENT jeder MAIN##_ROW3, weil
//    Enter hier stattdessen explizit als k2O-Slot gesetzt wird.
//  - Row4 hat eine zusätzliche, elektrisch reale Spalte I zwischen den
//    echten QWERTY-Positionen (H='B', J='N') - Keychrons eigenes
//    Referenz-Keymap legt dort nochmal KC_B rein (vermutlich nur ein
//    Lückenfüller). Laut Nutzer ist das physisch die Fn-Taste -
//    `MO(_FN)`, fix auf jeder Ebene identisch (wie Makro-Spalte/Nav-Block),
//    nicht Teil des MAIN##_ROW4-Inhalts.
//  - Row5 (Grundreihe) hat eine physische Leertasten-Aufteilung
//    (Leer|Fn+Leer statt einer durchgehenden Leertaste) und wurde mit dem
//    Nutzer explizit geklärt (siehe Session-Notiz): Ctrl-Meta-Alt-Leer-
//    Meta-[Leer mit Fn-Hold]-AltGr-Ctrl. Anders als bei den anderen Boards
//    hier NICHT per-Ebene parametrisiert (row5_pick.h passt wegen der
//    Sonderform ohnehin nicht) - fix auf jeder Ebene identisch.

// ── Makro-Spalte (fix, jede Ebene identisch) ──
// Reihen 1-5 = QK_MACRO_1/3/5/7/9 (ungerade Makro-Slots, aufsteigend von
// oben nach unten) - explizite Nutzervorgabe, ersetzt die vorherige
// FN60_ROW3-Spiegelung (F/D/S/A).
#define Q10PRO_MC_1 QK_MACRO_1
#define Q10PRO_MC_2 QK_MACRO_3
#define Q10PRO_MC_3 QK_MACRO_5
#define Q10PRO_MC_4 QK_MACRO_7
#define Q10PRO_MC_5 QK_MACRO_9

// ── F-Row (fix, jede Ebene identisch - keine media-per-FN-Variante, siehe
// oben: V3 Max nutzt seine eigene FROW0_FN in der Praxis auch nicht).
// Endet F11-F12-F13-Del (nicht Ins) - F13 auf der Position, die bei
// Keychrons Referenz-Keymap KC_INS war, laut Nutzerwunsch. ──
#define Q10PRO_FROW0 \
KC_MUTE, KC_ESC, F_ROW_BASE, KC_F13, KC_DEL,

// ── Row3-Kern ohne trailing Enter (14 Token -> 13) ──
#define ROW3_DROP_ENT_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,...) \
a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,
#define ROW3_DROP_ENT(...) ROW3_DROP_ENT_(__VA_ARGS__)

// ── Row4-Kern gesplittet um die überzählige Spalte I herum (13 Token -> 7+6) ──
#define Q10PRO_ROW4_SPLIT7_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...) \
a1,a2,a3,a4,a5,a6,a7,
#define Q10PRO_ROW4_SPLIT7(...) Q10PRO_ROW4_SPLIT7_(__VA_ARGS__)
#define Q10PRO_ROW4_SPLIT6_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...) \
a8,a9,a10,a11,a12,a13,
#define Q10PRO_ROW4_SPLIT6(...) Q10PRO_ROW4_SPLIT6_(__VA_ARGS__)

#define Q10PRO_ROW1(MAIN) Q10PRO_MC_1, MAIN##_ROW1 KC_PGUP,
#define Q10PRO_ROW2(MAIN) Q10PRO_MC_2, MAIN##_ROW2 KC_PGDN,
#define Q10PRO_ROW3(MAIN) Q10PRO_MC_3, ROW3_DROP_ENT(MAIN##_ROW3) KC_ENT, KC_HOME,
#define Q10PRO_ROW4(MAIN) Q10PRO_MC_4, Q10PRO_ROW4_SPLIT7(MAIN##_ROW4) MO(_FN), Q10PRO_ROW4_SPLIT6(MAIN##_ROW4) KC_UP,

// ── Row5 (Grundreihe, fix) - siehe Erklärung oben ──
#define Q10PRO_ROW5 \
Q10PRO_MC_5, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RGUI, SP_FN, KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT

#define FF91_ISO_Q10PRO_LIST(MAIN) \
/* Row0 (F)   */ Q10PRO_FROW0 \
/* Row1       */ Q10PRO_ROW1(MAIN) \
/* Row2       */ Q10PRO_ROW2(MAIN) \
/* Row3       */ Q10PRO_ROW3(MAIN) \
/* Row4       */ Q10PRO_ROW4(MAIN) \
/* Row5       */ Q10PRO_ROW5

// Wrapper
#define KEYMAP_91_ISO_Q10PRO(MAIN) \
LAYOUT_APPLY(LAYOUT_ISO, FF91_ISO_Q10PRO_LIST(MAIN))
// clang-format on
