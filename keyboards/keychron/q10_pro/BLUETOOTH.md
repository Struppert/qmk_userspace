# Keychron Q10 Pro – Bluetooth verbinden

**Eigenständig lesbar.** Ähnlicher Mechanismus wie bei V1 Max/V3 Max
(siehe dort für die volle Herleitung/Historie des Grundmusters), aber
mit einem echten Unterschied: **dieser Firmware-Port unterstützt nur
Bluetooth + USB, kein 2.4G-Dongle** (die alte `wireless_playground`-
Firmware hatte dafür nie Pins/Code - anders als V1 Max/V3 Max/Q10 Max mit
ihrem LKBT51-Chip und `P2P4G`-Support). Der CKBT51-Chip dieses Boards ist
reiner Bluetooth-Chip.

## 🔌 Voraussetzung: Transport-Schalter
Physischer Schalter/Modus-Pin (`BT_MODE_SELECT_PIN`, alte Firmware:
`USB_BT_MODE_SELECT_PIN`) - **ohne Bluetooth-Stellung tun die Tasten
unten nichts.**

## 🗺️ Wie man zu diesen Tasten kommt

1. **Capslock-Taste halten** (Tap-Dance: kurz → `Esc`, gehalten → `_SYS`).
2. **Während Capslock gehalten wird**, eine der folgenden Tasten auf der
   QWERTZ-Reihe drücken - physische Positionen `O`/`P`/`Ü`/`+`, mit
   `R`/`T`/`Z`/`U`/`I` als freiem Puffer davor. **Nicht** Q/W/E - die sind
   weiterhin Default-Layer-Auswahl (`DF(_QWERTZ)`/`DF(_NEOQWERTZ1)`/
   `DF(_NOTED1)`):

| Physische Position | Auf `_SYS` (Capslock halten) | Kurz drücken | 2+ Sekunden halten |
|:--|:--|:--|:--|
| `O` | `BT_HST1` | Zu Bluetooth-Host-Slot 1 verbinden/wechseln | **Neues Gerät auf Slot 1 koppeln** |
| `P` | `BT_HST2` | Zu Bluetooth-Host-Slot 2 verbinden/wechseln | **Neues Gerät auf Slot 2 koppeln** |
| `Ü` | `BT_HST3` | Zu Bluetooth-Host-Slot 3 verbinden/wechseln | **Neues Gerät auf Slot 3 koppeln** |
| `+` | `BAT_LVL` | Akkustand kurz per LED-Balken anzeigen | – |

**Kein `_WIN_FN`-Zweitweg** wie beim V1 Max - bewusst nicht portiert
(gleiche Begründung wie beim V3 Max: der Capslock-Weg reicht für den
täglichen Gebrauch, ein zusätzliches handgebautes Vollformat-Layout wäre
nur unnötiges Fehlerrisiko).

## 📶 Neues Gerät koppeln (Pairing)
Host-Slot-Taste **≥ 2 Sekunden** halten (nicht nur tippen) → LED blinkt
im Sekundentakt (Pairing-Modus) → am Rechner koppeln (z.B.
`bluetoothctl`: `power on; agent on; default-agent; scan on; pair <MAC>;
trust <MAC>; connect <MAC>`) → LED hört auf zu blinken, leuchtet ~2s
durchgehend, geht dann aus. **Noch nicht am echten Board ausprobiert.**

## 🔁 Zwischen bereits gekoppelten Hosts wechseln
Kurzer Tap auf `BT_HST1/2/3` verbindet zum zuletzt gekoppelten Gerät auf
diesem Slot.

## 💡 LED-Indizes - NICHT verifiziert
`BT_INDCATION_LED_MATRIX_LIST` steht in
`keyboards/keychron/q10_pro/iso_encoder/config.h` (im Fork) auf
`{17, 18, 19}` - **1:1 unverändert aus der alten Firmware übernommen**
(dort hieß das Macro `HOST_LED_MATRIX_LIST`, gleicher Wert). Anders als
beim V1-Max/V3-Max-Fix (siehe dort: dort wurde die alte Zeile-1-Annahme
per Raw-HID-Test als falsch entlarvt und auf Zeile 2 korrigiert) **wurde
das hier noch nicht gegen die echte 90-LED-Tabelle
(`iso_encoder/iso_encoder.c`) durchgerechnet oder am Board getestet.**
Möglich, dass diese drei Indizes nicht die tatsächlichen `BT_HST1-3`-
Tasten (physisch O/P/Ü) treffen, sondern noch die alten Positionen aus
der ursprünglichen Werksbelegung (Zahlenreihe o.ä.) - **vor dem ersten
Pairing-Test unbedingt per Raw-HID oder visuell prüfen**, genau wie beim
V1 Max/V3 Max ein LED-Index-Fehler erst durch einen echten Test gefunden
wurde.

## 🔋 Akkustand
`BAT_LVL` zeigt den Akkustand als LED-Balken, wirkt nur ohne USB-
Stromversorgung bei aktivem Bluetooth-Transport. `BAT_LEVEL_LED_LIST`
(`{17..26}`) ebenfalls unverändert aus der alten Firmware übernommen,
nicht verifiziert.

## ⚠️ Was noch nicht getestet ist
**Alles hier ist bisher nur Compile-verifiziert, nicht am echten Board.**
Insbesondere: ob die Keycodes tatsächlich an den beschriebenen
physischen Positionen landen (kein Raw-HID-Test wie bei V1 Max/V3 Max
durchgeführt), tatsächliches Bluetooth-Pairing, Verbindungswechsel
zwischen Slots, ob die Indikator-LEDs überhaupt an den richtigen Tasten
blinken, und `BAT_LVL` selbst. Vor dem ersten produktiven Einsatz: erst
per Raw-HID (`tools/via_macros.py`) gegenprüfen, dann am Gerät testen -
nicht blind vertrauen.
