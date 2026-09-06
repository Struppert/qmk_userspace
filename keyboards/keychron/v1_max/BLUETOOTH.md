# Keychron V1 Max – Bluetooth verbinden

**Eigenständig lesbar** - keine andere Datei nötig, um das Board per
Bluetooth zu koppeln/wechseln. (Für alles andere rund ums Keymap: siehe
`BELEGUNG.md`/`README.md`.) Basiert auf der tatsächlichen Logik in
`keyboards/keychron/common/wireless/keychron_wireless_common.c`
(Fork `~/keychron-qmk/qmk_firmware`).

## 🔌 Voraussetzung: Transport-Schalter

**Ohne diesen Schritt tut keine der folgenden Tasten irgendetwas.** Das
V1 Max hat einen physischen 3-Positionen-Schalter (Unterseite/Rückkante
des Boards) für USB-Kabel / Bluetooth / 2.4G-Dongle. Firmwareseitig wird
das über zwei GPIO-Pins gelesen (`BT_MODE_SELECT_PIN`, `P24G_MODE_SELECT_PIN`
in `config.h`) und alle 100 ms entprellt in `keychron_wireless_common.c`
(`wireless_pre_task()`) - je nach Schalterstellung wird automatisch
`TRANSPORT_USB`, `TRANSPORT_BLUETOOTH` oder `TRANSPORT_P2P4` gesetzt.

Alle `BT_HST1-3`/`P2P4G`-Tasten sind im Quellcode explizit an den bereits
aktiven Transport gekoppelt (`keychron_wireless_common.c`,
`process_record_keychron_wireless`): der `BT_HST1-3`-Fall greift nur,
wenn `get_transport() == TRANSPORT_BLUETOOTH` bereits gilt, der
`P2P4G`-Fall nur bei `TRANSPORT_P2P4`. Steht der Schalter auf USB, laufen
Tastendrücke auf diesen Positionen komplett ins Leere - kein Fehler,
einfach keine Wirkung. Also: **erst Schalter auf Bluetooth stellen**,
dann erst die Tasten unten benutzen.

## 🗺️ Wie man zu diesen Tasten kommt

Die BT-Tasten sind keine eigene Ebene, sondern liegen auf `_SYS` - einer
Ebene, die man **hält**, keine, auf die man dauerhaft umschaltet:

1. **Capslock-Taste halten** (physisch: die Taste, wo normalerweise
   Feststelltaste sitzt - ist hier als Tap-Dance belegt: kurz getippt
   → `Esc`, gehalten → Ebene `_SYS` aktiv).
2. **Während Capslock gehalten wird**, eine der folgenden Tasten
   drücken - sie liegen auf der QWERTZ-Reihe (zweite Buchstabenreihe von
   oben), an den physischen Positionen von `U`, `I`, `O`, `P`, `Ü` - also
   unter der **rechten Hand**, mit `R`/`T`/`Z` als freiem Puffer dazwischen.
   **Nicht** Q/W/E - die liegen auf `_SYS` direkt neben dem Puffer, sind
   aber Default-Layer-Auswahl (`DF(_QWERTZ)`/`DF(_NEOQWERTZ1)`/
   `DF(_NOTED1)`, siehe `BELEGUNG.md`), keine Bluetooth-Funktion:

| Physische Position (auf `_QWERTZ`-Ebene) | Auf `_SYS` (Capslock halten) | Kurz drücken | 2+ Sekunden halten |
|:--|:--|:--|:--|
| `U` | `BT_HST1` | Zu Bluetooth-Host-Slot 1 verbinden/wechseln | **Neues Gerät auf Slot 1 koppeln (Pairing)** |
| `I` | `BT_HST2` | Zu Bluetooth-Host-Slot 2 verbinden/wechseln | **Neues Gerät auf Slot 2 koppeln (Pairing)** |
| `O` | `BT_HST3` | Zu Bluetooth-Host-Slot 3 verbinden/wechseln | **Neues Gerät auf Slot 3 koppeln (Pairing)** |
| `P` | `P2P4G` | Auf 2.4G-Dongle-Modus wechseln (Schalter muss auf 2.4G stehen) | – (siehe Baustelle unten) |
| `Ü` | `BAT_LVL` | Akkustand kurz per LED-Balken anzeigen (nur wirksam ohne USB-Strom, siehe unten) | – |

Also z.B.: Capslock halten + `U` kurz tippen = zu Host-Slot 1 wechseln;
Capslock halten + `U` **zwei Sekunden** halten = Host-Slot 1 neu koppeln.

Diese Reihenfolge ist ein lokaler Override in `keymap.c`
(`#define SYS60_ROW2 ...`): ursprünglich (seit 2026-08-31) direkt neben
den drei Layer-Auswahltasten Q/W/E auf R/T/Z, seit 2026-09-06 bewusst
weiter auf die rechte Hand verschoben (U/I/O/P/Ü), damit die
Bluetooth-Tasten nicht mehr direkt an der Layer-Auswahl kleben - R/T/Z
sind seitdem frei (`KC_NO`).

**Alternativer Weg** (funktioniert zusätzlich, unabhängig vom obigen):
`_SYS` halten (Capslock) **und gleichzeitig** die Taste an der
LWin-Position (unten links, normalerweise Windows-Taste) mit halten -
das aktiviert zusätzlich eine Ebene `_WIN_FN`, auf der dieselben
`BT_HST1-3`/`P2P4G`/`BAT_LVL` nochmal liegen, dort aber auf der
**Zahlenreihe** an den physischen Positionen `1`/`2`/`3`/`4` (eine ganze
Reihe über der QWERTZ-Reihe). Für den täglichen Gebrauch ist der Weg
oben (nur Capslock halten) einfacher, da nur eine Zusatztaste statt
zwei.

## 📶 Neues Gerät koppeln (Pairing)

1. Einen der drei Host-Slots wählen (z.B. Slot 1) und die zugehörige
   Taste **mindestens 2 Sekunden gedrückt halten**, bis losgelassen -
   nicht nur antippen (ein kurzer Tap verbindet stattdessen nur zu einem
   *bereits gekoppelten* Gerät auf diesem Slot, falls vorhanden, und tut
   sonst nichts).
2. Die LED an der gedrückten `BT_HST`-Taste selbst (`U`/`I`/`O` für Slot
   1/2/3) beginnt **im Sekundentakt zu blinken** (1000 ms an/aus,
   `BT_INDCATION_LED_MATRIX_LIST` in `keymaps/neo/config.h`, lokal auf
   `{21, 22, 23}` überschrieben, damit sie zur `SYS60_ROW2`-Verschiebung
   passt) - das Board ist jetzt im Pairing-Modus und für den Host
   sichtbar.
3. Auf dem Rechner die Kopplung wie gewohnt anstoßen (siehe Linux-Beispiel
   unten). Nach erfolgreicher Kopplung hört das Blinken auf und die LED
   leuchtet noch einmal für ~2 Sekunden durchgehend auf (verbunden-
   Zustand), dann geht sie aus - kein dauerhaftes Wiederholen.
4. Läuft das Pairing-Fenster ab, ohne dass sich ein Host meldet, geht die
   LED wieder aus (getrennt) - Schritt 1 einfach wiederholen.

### Linux (bluetoothctl, dieses System)
```bash
bluetoothctl
# innerhalb von bluetoothctl:
power on
agent on
default-agent
scan on
# warten bis "Keychron V1 Max" (oder ähnlich) auftaucht, dann MAC-Adresse
# aus der Liste übernehmen:
pair   <MAC-ADRESSE>
trust  <MAC-ADRESSE>
connect <MAC-ADRESSE>
scan off
exit
```
Alternativ grafisch über die übliche Bluetooth-Einstellungsseite der
Desktop-Umgebung - das Board muss dafür wie oben zuerst per
2-Sekunden-Halt in den Pairing-Modus versetzt werden, sonst taucht es in
der Geräteliste nicht auf.

## 🔁 Zwischen bereits gekoppelten Hosts wechseln

Kurzer Tap auf `BT_HST1`/`BT_HST2`/`BT_HST3` (nicht halten) verbindet zum
jeweils zuletzt auf diesem Slot gekoppelten Gerät - kein erneutes Pairing
nötig, solange die Kopplung auf beiden Seiten noch besteht (Host hat das
Gerät nicht "vergessen"/entfernt).

## 💡 LED-Bedeutung (an der jeweils gedrückten `BT_HST`-Taste selbst)

Werte aus `indicator.h` (`INDICATOR_CONFIG_*`-Makros im Fork):

| Zustand | LED-Verhalten |
|:--|:--|
| Pairing-Modus aktiv | Blinkt 1000 ms an / 1000 ms aus, dauerhaft bis Kopplung oder Timeout |
| Verbunden | **Einmaliges** Aufleuchten für ~2 Sekunden, danach aus - kein wiederkehrendes Blinken |
| Verbindungsaufbau/Reconnect | Schnelles Blinken (100 ms an/aus, ca. 600 ms lang, wiederholt sich bei weiteren Versuchen) |
| Getrennt | Keine eigene Status-Anzeige an dieser Position - LED zeigt nur den normal aktiven RGB-Matrix-Effekt (kann durch normale Tastendruck-Reaktivität trotzdem kurz aufblitzen) |

## 🔋 Akkustand
`BAT_LVL` (Capslock halten + `Ü` kurz tippen, siehe Tabelle oben) zeigt
den Akkustand für ein paar Sekunden als LED-Balken über 10 RGB-Matrix-
Positionen (`BAT_LEVEL_LED_LIST` in `config.h` - deckt die physischen
Positionen `Q` bis `P` ab, je mehr davon leuchten desto voller der Akku).
**Wirkt nur**, wenn der Transport-Schalter auf Bluetooth oder 2.4G steht
**und** kein USB-Kabel gerade Strom liefert (`process_record_keychron_wireless`:
`get_transport() & TRANSPORT_WIRELESS` sowie `!usb_power_connected()`) -
bei angestecktem Ladekabel bleibt der Tastendruck wirkungslos.

## 🩹 Behobener Bug: BT-Tasten waren seit 2026-08-31 komplett wirkungslos (gefixt 2026-09-06)

Bis 2026-09-06 haben `BT_HST1`/`BT_HST2`/`BT_HST3`/`P2P4G`/`BAT_LVL` **gar
nichts** getan, egal welche Taste/Position - unabhängig von Schalter-
stellung, Ebene oder der ganzen R/T/Z→U/I/O/P-Verschiebung weiter oben.
Nach außen sah das aus wie "leere"/`KC_NO`-Tasten, in VIA und Keychron
Launcher entsprechend nicht angezeigt.

**Ursache:** `keymap.c` hatte einen "Fallback für ältere QMKs"-Block:
```c
#ifndef BT_HST1
#define BT_HST1 KC_NO
...
#endif
```
Das sollte nur bei einem Fork greifen, der diese Keycodes nicht kennt.
Problem: `BT_HST1` ist in `keychron_common.h` eine echte **C-Enum-
Konstante**, kein `#define`-Makro - und der Preprocessor kann `#ifndef`
grundsätzlich nur auf Makros anwenden, niemals auf Enum-Werte. Der
Fallback hat deshalb **immer** gefeuert, komplett unabhängig davon ob
der Fork die echten Werte bereitstellte, und `BT_HST1` etc. dauerhaft zu
einem Makro `= KC_NO` gemacht - noch bevor die `keymaps[]`-Tabelle
gebaut wurde. Zusätzlich fehlte in `keymap.c` das
`#include "keychron_common.h"`, das Keychrons eigenes Referenz-Keymap
(`v1_max/iso_encoder/keymaps/keychron/keymap.c`) immer explizit setzt -
`QMK_KEYBOARD_H` allein bringt nur generisches `quantum.h` mit.

**Fix:** den Fallback-Block ersatzlos entfernt, `#include "keychron_common.h"`
direkt nach `#include QMK_KEYBOARD_H` ergänzt.

**Verifiziert** per `_Static_assert` auf den echten Enum-Wert, per
`-E`-Preprocessor-Dump von `keymaps[]` (zeigte danach `BT_HST1` als
echtes Token statt `KC_NO`), und live per Raw-HID-Abfrage
(`dynamic_keymap_get_keycode`) direkt am geflashten Board - `_SYS`
Reihe 2 liefert seitdem `0x7e0b`..`0x7e0f` auf U/I/O/P/Ü, nicht mehr 0.
Auch in VIA/Launcher jetzt sichtbar.

**Lehre:** `#ifndef X`-Fallbacks für Keycodes sind in QMK/Keychron-Forks
riskant, wenn `X` potenziell eine Enum-Konstante statt eines Makros ist
- ein still greifender Fallback ist schwerer zu bemerken als ein
Compile-Fehler. Betroffen war nur dieser Keymap - andere Boards in
diesem Userspace haben dieses Stub-Muster nicht.

## ⚠️ Bekannte Baustelle
`P2P4G` hat laut Quellcode denselben 2-Sekunden-Halte-Pairing-Mechanismus
wie `BT_HST1-3` (gemeinsamer `pairing_key_timer`) - für den 2.4G-Dongle
aber nicht ausprobiert/dokumentiert in dieser Session. Falls der
mitgelieferte Dongle mal neu gekoppelt werden muss: vermutlich dieselbe
2-Sekunden-Halte-Geste auf `P` (`P2P4G`), dann den Dongle-seitigen
Pairing-Knopf (falls vorhanden) betätigen - nicht verifiziert, bei Bedarf
zuerst am Gerät testen statt blind zu vertrauen.

**Erledigt (früher hier dokumentiert, seit 2026-09-06 behoben):** die
fest verdrahteten Indikator-LEDs gingen früher von Keychrons
Standard-Tastenbelegung aus (`BT_HST1-3` auf Q/W/E) und passten weder
zum R/T/Z- noch zum jetzigen U/I/O-Override. Fix: `BT_INDCATION_LED_MATRIX_LIST`
und `P24G_INDICATION_LED_INDEX` sind in `keymaps/neo/config.h` lokal auf
`{21, 22, 23}`/`24` überschrieben (dafür musste `v1_max/config.h` im
Fork die beiden Defines mit `#ifndef` absichern, sonst Compile-Error
durch doppelte Definition). Betrifft nur den `iso_encoder`+`neo`-Build -
die Stock-Keymaps von `ansi_encoder`/`jis_encoder` behalten Q/W/E und
damit weiterhin die Keychron-Standard-LED-Position.
