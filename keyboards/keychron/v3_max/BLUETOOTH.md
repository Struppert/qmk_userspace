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
`keymaps/neo/config.h` lokal auf `{40, 41, 42}`/`43` überschrieben
(LED-Indizes für die physischen Positionen U/I/O/P). Setzt voraus, dass
`v3_max/config.h` im Fork diese beiden Defines mit `#ifndef` absichert -
wurde für dieses Board ergänzt (analog zum V1-Max-Fix vom 2026-09-06,
Fork-Commit `53d2f53474`).

**Live per Raw-HID verifiziert** (2026-09-08, `id_dynamic_keymap_get_keycode`,
`tools/via_macros.py`s `RawHid`-Klasse gegen das frisch geflashte Board):
`_SYS`-Layer, physische QWERTZ-Buchstabenreihe = Matrix-**Zeile 2**
(nicht Zeile 1 - das ist die Zahlenreihe, per Test bestätigt:
`layer=0,row=1,col=1` lieferte `0x1e` = "1", nicht `KC_Q`). Auf
`row=2, col=7..11` liefert das Board `0x7e0b/0x7e0c/0x7e0d/0x7e0e/0x7e0f`
= `BT_HST1/BT_HST2/BT_HST3/P2P4G/BAT_LVL` - dieselben Werte wie beim
V1 Max auf U/I/O/P/Ü. Die LED-Indizes wurden entsprechend aus
`g_led_config`s **Zeile 2** (nicht Zeile 1) übernommen: `{33..49}` für
Zeile 2, Spalten 7-11 = `40/41/42/43/44`.

**Korrigiert:** eine erste Version dieser Datei/`config.h` hatte
`{23, 24, 25}`/`26` (aus `g_led_config`-Zeile 1 statt 2 berechnet, vor
dem ersten echten Flash nie gegengeprüft) - das hätte die Pairing-
Blink-LED auf der Zahlenreihe statt auf U/I/O/P aufleuchten lassen.
Nach dem Live-Test auf die oben genannten korrekten Werte gefixt.
`BAT_LEVEL_LED_LIST` bleibt unverändert auf dem Board-Default (`{17..26}`)
- das sind LEDs aus Zeile 1, also physisch die **Zahlenreihe** (1-0), nicht
die Buchstabenreihe (siehe Zeilen-Verwechslung oben - Zeile 1 ist auf
diesem Board tatsächlich die Zahlenreihe). Rein kosmetisch für den
LED-Balken ohne Belang, welche Reihe genau leuchtet - nicht eigens
verschoben, da `BAT_LVL` selbst weiterhin nur eine einzelne Taste ist
(auf Ü, Zeile 2) und nicht an diese Liste gekoppelt sein muss.

## 🔋 Akkustand
Wie beim V1 Max: `BAT_LVL` zeigt den Akkustand als LED-Balken, wirkt nur
ohne USB-Stromversorgung bei aktivem Bluetooth/2.4G-Transport.

## ⚠️ Was noch nicht getestet ist
Der Raw-HID-Check oben bestätigt nur, dass die richtigen Keycodes im
EEPROM stehen (`BT_HST1-3`/`P2P4G`/`BAT_LVL` auf U/I/O/P/Ü) - **nicht**
das tatsächliche Bluetooth-Verhalten. Noch nicht ausprobiert: Pairing
mit einem echten Host, Verbindungswechsel zwischen Slots, ob die
Indikator-LED optisch tatsächlich an U/I/O/P blinkt (LED-Indizes
`{40,41,42}`/`43` sind nur rechnerisch aus `g_led_config` hergeleitet
und gegen den Keycode-Test verifiziert, nicht visuell am Board), sowie
`BAT_LVL`/`P2P4G` selbst.
