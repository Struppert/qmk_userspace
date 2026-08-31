# Keychron V1 Max – Bluetooth verbinden

**Eigenständig lesbar** - keine andere Datei nötig, um das Board per
Bluetooth zu koppeln/wechseln. (Für alles andere rund ums Keymap: siehe
`BELEGUNG.md`/`README.md`.) Basiert auf der tatsächlichen Logik in
`keyboards/keychron/common/wireless/keychron_wireless_common.c`
(Fork `~/keychron-qmk/qmk_firmware`).

## 🗺️ Wie man zu diesen Tasten kommt

Die BT-Tasten sind keine eigene Ebene, sondern liegen auf `_SYS` - einer
Ebene, die man **hält**, keine, auf die man dauerhaft umschaltet:

1. **Capslock-Taste halten** (physisch: die Taste, wo normalerweise
   Feststelltaste sitzt - ist hier als Tap-Dance belegt: kurz getippt
   → `Esc`, gehalten → Ebene `_SYS` aktiv).
2. **Während Capslock gehalten wird**, eine der folgenden Tasten
   drücken - sie liegen auf der QWERTZ-Reihe (zweite Buchstabenreihe von
   oben), an den physischen Positionen von `Q`, `W`, `E`, `R`, `T`:

| Physische Position (auf `_QWERTZ`-Ebene) | Auf `_SYS` (Capslock halten) | Kurz drücken | 2+ Sekunden halten |
|:--|:--|:--|:--|
| `Q` | `BT_HST1` | Zu Bluetooth-Host-Slot 1 verbinden/wechseln | **Neues Gerät auf Slot 1 koppeln (Pairing)** |
| `W` | `BT_HST2` | Zu Bluetooth-Host-Slot 2 verbinden/wechseln | **Neues Gerät auf Slot 2 koppeln (Pairing)** |
| `E` | `BT_HST3` | Zu Bluetooth-Host-Slot 3 verbinden/wechseln | **Neues Gerät auf Slot 3 koppeln (Pairing)** |
| `R` | `P2P4G` | Auf 2.4G-Dongle-Modus wechseln (kein Bluetooth) | – (siehe Baustelle unten) |
| `T` | `BAT_LVL` | Akkustand kurz per LED-Balken anzeigen | – |

Also z.B.: Capslock halten + `Q` kurz tippen = zu Host-Slot 1 wechseln;
Capslock halten + `Q` **zwei Sekunden** halten = Host-Slot 1 neu koppeln.

**Alternativer Weg** (funktioniert zusätzlich, unabhängig vom obigen):
`_SYS` halten (Capslock) **und gleichzeitig** die Taste an der
LWin-Position (unten links, normalerweise Windows-Taste) mit halten -
das aktiviert zusätzlich eine Ebene `_WIN_FN`, auf der dieselben
`BT_HST1-3`/`P2P4G`/`BAT_LVL` nochmal liegen (an leicht anderen
physischen Positionen, Reihe darunter). Für den täglichen Gebrauch ist
der Weg oben (nur Capslock halten) einfacher, da nur eine Zusatztaste
statt zwei.

## 📶 Neues Gerät koppeln (Pairing)

1. Einen der drei Host-Slots wählen (z.B. Slot 1) und die zugehörige
   Taste **mindestens 2 Sekunden gedrückt halten**, bis losgelassen -
   nicht nur antippen (ein kurzer Tap verbindet stattdessen nur zu einem
   *bereits gekoppelten* Gerät auf diesem Slot, falls vorhanden, und tut
   sonst nichts).
2. Die LED an der entsprechenden `BT_HST`-Taste selbst (Q/W/E, siehe
   Tabelle oben - fest verdrahtete RGB-Matrix-Positionen für die drei
   Host-Slots) beginnt **im Sekundentakt zu blinken** - das Board ist
   jetzt im Pairing-Modus und für den Host sichtbar.
3. Auf dem Rechner die Kopplung wie gewohnt anstoßen (siehe Linux-Beispiel
   unten). Nach erfolgreicher Kopplung geht das Blinken in ein kurzes,
   periodisches Aufleuchten über (verbunden-Zustand).
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

## 💡 LED-Bedeutung (Host-Slot-Position)

| Zustand | LED-Verhalten |
|:--|:--|
| Pairing-Modus aktiv | Blinkt (~1×/Sekunde), dauerhaft bis Kopplung oder Timeout |
| Verbunden | Leuchtet kurz auf, dann für einige Sekunden aus, wiederholt sich |
| Verbindungsaufbau/Reconnect | Blinkt schnell |
| Getrennt | Aus (bis auf gelegentliches kurzes Aufblitzen) |

## 🔋 Akkustand
`BAT_LVL` (Capslock halten + `T` kurz tippen, siehe Tabelle oben) zeigt
den Akkustand für ein paar Sekunden als LED-Balken über mehrere RGB-
Matrix-Positionen (10 LEDs, je mehr davon leuchten desto voller der
Akku).

## ⚠️ Bekannte Baustelle
`P2P4G` hat laut Quellcode denselben 2-Sekunden-Halte-Pairing-Mechanismus
wie `BT_HST1-3` (gemeinsamer `pairing_key_timer`) - für den 2.4G-Dongle
aber nicht ausprobiert/dokumentiert in dieser Session. Falls der
mitgelieferte Dongle mal neu gekoppelt werden muss: vermutlich dieselbe
2-Sekunden-Halte-Geste auf `P2P4G`, dann den Dongle-seitigen Pairing-Knopf
(falls vorhanden) betätigen - nicht verifiziert, bei Bedarf zuerst am
Gerät testen statt blind zu vertrauen.
