#pragma once
#include "quantum.h"

// 1) Variadischer Forwarder: wendet ein Makro M auf eine bereits
//    fertig expandierte, komma-separierte Liste an.
#define LAYOUT_APPLY(M, ...) M(__VA_ARGS__)

// 2) 60%-Argumentliste zusammenbauen – variadisch, damit der
//    Bottom-Row-Picker direkt als „Restliste“ durchgereicht wird.
#define FF60_ISO_LIST(MAIN, ...)                                               \
  MAIN##_ROW1 MAIN##_ROW2 MAIN##_ROW3 MAIN##_ROW4 __VA_ARGS__

// 3) Öffentliche Abkürzung für Keymap-Zeilen.
//    Wichtig: Wir rufen NICHT direkt LAYOUT_ISO( FF60_ISO_LIST(...) )
//    sondern nutzen den Forwarder, damit __VA_ARGS__ auf der Ebene von
//    LAYOUT_ISO ankommt.
#define KEYMAP_60_ISO_BR(MAIN, BRROW5)                                         \
  LAYOUT_APPLY(LAYOUT_ISO, FF60_ISO_LIST(MAIN, BRROW5))
