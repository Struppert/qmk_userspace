#pragma once
#include "quantum.h"
// clang-format off
// Helfer, um ( ... )-eingeklammerte Listen als Argument wieder zu "entpacken"
#define EXPAND(...) __VA_ARGS__

// Einheitlicher Aufrufer
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// Keychron V10 (LAYOUT_iso_90, 90 Tasten, ISO+Knob, kein BT/Fork - baut
// gegen mainline QMK wie Q6/V3) - physische Eigenheiten gegenüber einem
// generischen 60%-Kern, abgeglichen mit Keychrons eigenem Referenz-Keymap
// (iso_encoder/keymaps/default) und dem rgb_matrix-Layout in keyboard.json:
//  - Spalte A auf Zeile 1-5 existiert laut Info.json elektrisch, ist aber
//    in Keychrons eigenem Default (MAC_BASE/WIN_BASE) durchgehend KC_TRNS -
//    hier fix auf KC_NO, keine echten Tasten (anders als Q10 Pros
//    Makro-Spalte). Nur Zeile 0/Spalte A ist real belegt (Mute, siehe
//    V10_FROW0).
//  - Row3 hat wie beim Q10 Pro eine ISO-Enter-Taste, die im Referenz-Keymap
//    exakt an der Position landet, an der der generische 60%-Kern
//    (MAIN##_ROW3, endet auf ...HASH, ENT) sie ohnehin erwartet - anders als
//    Q10 Pro ist hier KEIN ROW3_DROP_ENT/Neueinfügen nötig, die Token-Zahl
//    passt 1:1 (14 Tokens: CAPS..HASH + ENT).
//  - Row4 hat zwischen den echten QWERTZ-Positionen 'B' und 'N' eine
//    zusätzliche, elektrisch reale Taste (siehe rgb_matrix-Layout: Lücke
//    zwischen matrix[4,7]/[4,8] auf ISO, matrix[4,1]/[4,3] auf ANSI).
//    Keychrons eigenes Referenz-Keymap legt dort nochmal KC_B rein (reiner
//    Lückenfüller). In Analogie zum Q10 Pro (dort vom Nutzer bestätigt: das
//    ist physisch die Fn-Taste) hier ebenfalls MO(_FN) - **noch nicht am
//    echten Board verifiziert**, bei Bedarf leicht zu korrigieren.
//  - Row5 (Grundreihe) hat eine physische Leertasten-Aufteilung
//    (Leer|Fn+Leer statt einer durchgehenden Leertaste), wie beim Q10 Pro -
//    dazu nur je eine Alt- und eine Gui-Taste pro Seite (kein Platz für
//    RGUI/Leader/RCTL, die hier direkt in den Pfeiltasten-Block übergehen).
//    Reihenfolge Ctrl-Gui-Alt (PC-Konvention, wie der Rest dieses
//    Userspace) statt Keychrons eigener Mac/Win-Sonderreihenfolge - fix auf
//    jeder Ebene identisch, nicht per-Ebene parametrisiert (row5_pick.h
//    passt wegen der Sonderform ohnehin nicht).
//  - Kein KC_END auf diesem Board (Referenz-Keymap hat nur Ins/Del/Home/
//    PgUp/PgDn, kein End).

// ── F-Row (fix, jede Ebene identisch) ──
#define V10_FROW0 \
KC_MUTE, KC_ESC, F_ROW_BASE, KC_INS, KC_DEL,

// ── Row4-Kern gesplittet um die zusätzliche Fn-Taste zwischen B und N (13 Token -> 7+6) ──
#define V10_ROW4_SPLIT7_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...) \
a1,a2,a3,a4,a5,a6,a7,
#define V10_ROW4_SPLIT7(...) V10_ROW4_SPLIT7_(__VA_ARGS__)
#define V10_ROW4_SPLIT6_(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...) \
a8,a9,a10,a11,a12,a13,
#define V10_ROW4_SPLIT6(...) V10_ROW4_SPLIT6_(__VA_ARGS__)

#define V10_ROW1(MAIN) KC_NO, MAIN##_ROW1 KC_PGUP,
#define V10_ROW2(MAIN) KC_NO, MAIN##_ROW2 KC_PGDN,
#define V10_ROW3(MAIN) KC_NO, MAIN##_ROW3 KC_HOME,
#define V10_ROW4(MAIN) KC_NO, V10_ROW4_SPLIT7(MAIN##_ROW4) MO(_FN), V10_ROW4_SPLIT6(MAIN##_ROW4) KC_UP,

// ── Row5 (Grundreihe, fix) - siehe Erklärung oben ──
#define V10_ROW5 \
KC_NO, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, MO(_FN), KC_SPC, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT

#define FF90_ISO_V10_LIST(MAIN) \
/* Row0 (F)   */ V10_FROW0 \
/* Row1       */ V10_ROW1(MAIN) \
/* Row2       */ V10_ROW2(MAIN) \
/* Row3       */ V10_ROW3(MAIN) \
/* Row4       */ V10_ROW4(MAIN) \
/* Row5       */ V10_ROW5

// Wrapper
#define KEYMAP_90_ISO_V10(MAIN) \
LAYOUT_APPLY(LAYOUT_ISO, FF90_ISO_V10_LIST(MAIN))
// clang-format on
