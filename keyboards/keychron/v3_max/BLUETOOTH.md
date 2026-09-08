# Keychron V3 Max – Bluetooth verbinden

**Eigenständig lesbar.** Identischer Mechanismus wie beim V1 Max (siehe
dort `BLUETOOTH.md` für die volle Herleitung/Historie) - hier nur die für
das V3 Max spezifischen Werte (andere LED-Indizes, keine `_WIN_FN`-Ebene).

## 🔌 Voraussetzung: Transport-Schalter
Physischer 3-Positionen-Schalter (USB-Kabel / Bluetooth / 2.4G-Dongle),
gleiche Pins/Logik wie V1 Max (`BT_MODE_SELECT_PIN`, `P24G_MODE_SELECT_PIN`,
`keychron_wireless_common.c`). **Ohne Bluetooth-Stellung tun die Tasten
unten nichts.**

## 🗺️ Wie man zu diesen Tasten kommt

1. **Capslock-Taste halten** (Tap-Dance: kurz → `Esc`, gehalten → `_SYS`).
2. **Während Capslock gehalten wird**, eine der folgenden Tasten auf der
   QWERTZ-Reihe drücken - physische Positionen `U`/`I`/`O`/`P`/`Ü`, mit
   `R`/`T`/`Z` als freiem Puffer. **Nicht** Q/W/E - die sind weiterhin
   Default-Layer-Auswahl (`DF(_QWERTZ)`/`DF(_NEOQWERTZ1)`/`DF(_NOTED1)`):

| Physische Position | Auf `_SYS` (Capslock halten) | Kurz drücken | 2+ Sekunden halten |
|:--|:--|:--|:--|
| `U` | `BT_HST1` | Zu Bluetooth-Host-Slot 1 verbinden/wechseln | **Neues Gerät auf Slot 1 koppeln** |
| `I` | `BT_HST2` | Zu Bluetooth-Host-Slot 2 verbinden/wechseln | **Neues Gerät auf Slot 2 koppeln** |
| `O` | `BT_HST3` | Zu Bluetooth-Host-Slot 3 verbinden/wechseln | **Neues Gerät auf Slot 3 koppeln** |
| `P` | `P2P4G` | Auf 2.4G-Dongle-Modus wechseln | – (nicht ausprobiert, siehe V1 Max BLUETOOTH.md) |
| `Ü` | `BAT_LVL` | Akkustand kurz per LED-Balken anzeigen | – |

**Kein `_WIN_FN`-Zweitweg wie beim V1 Max:** dort existiert zusätzlich
eine über `LWin`+Capslock erreichbare Ebene mit denselben Tasten auf der
Zahlenreihe - für das V3 Max bewusst weggelassen, da sie beim V1 Max
selbst als "für den täglichen Gebrauch unnötig, da der Capslock-Weg
einfacher ist" dokumentiert ist und ein händisch nachgebautes komplettes
TKL-Vollformat-Layout dafür ein unnötiges Fehlerrisiko wäre. Falls
gewünscht, kann sie nach demselben Muster wie beim V1 Max ergänzt werden.

## 📶 Neues Gerät koppeln (Pairing)
Identisch zum V1 Max: Host-Slot-Taste **≥ 2 Sekunden** halten (nicht nur
tippen) → LED blinkt im Sekundentakt (Pairing-Modus) → am Rechner koppeln
(z.B. `bluetoothctl`: `power on; agent on; default-agent; scan on; pair
<MAC>; trust <MAC>; connect <MAC>`) → LED hört auf zu blinken, leuchtet
~2s durchgehend, geht dann aus.

## 🔁 Zwischen bereits gekoppelten Hosts wechseln
Kurzer Tap auf `BT_HST1/2/3` verbindet zum zuletzt gekoppelten Gerät auf
diesem Slot.

## 💡 LED-Indizes (V3 Max-spezifisch, 88 statt 82 LEDs)
`BT_INDCATION_LED_MATRIX_LIST`/`P24G_INDICATION_LED_INDEX` in
`keymaps/neo/config.h` lokal auf `{23, 24, 25}`/`26` überschrieben
(LED-Indizes für die physischen Positionen U/I/O/P, ermittelt aus dem
"Key Matrix zu LED Index"-Array in `iso_encoder.c` des Forks). Setzt
voraus, dass `v3_max/config.h` im Fork diese beiden Defines mit
`#ifndef` absichert - wurde für dieses Board ergänzt (analog zum
V1-Max-Fix vom 2026-09-06, Fork-Commit `53d2f53474`).
`BAT_LEVEL_LED_LIST` bleibt unverändert auf dem Board-Default
(`{17..26}`, Q bis P).

## 🔋 Akkustand
Wie beim V1 Max: `BAT_LVL` zeigt den Akkustand als LED-Balken, wirkt nur
ohne USB-Stromversorgung bei aktivem Bluetooth/2.4G-Transport.
