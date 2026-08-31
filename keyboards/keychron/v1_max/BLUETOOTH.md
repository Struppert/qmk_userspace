# Keychron V1 Max – Bluetooth verbinden

Ergänzung zu `README.md`/`BELEGUNG.md`. Beschreibt, wie man das Board
per Bluetooth (nicht 2.4G-Dongle, nicht USB) mit einem Host koppelt und
zwischen den drei BT-Host-Slots wechselt - basierend auf der tatsächlichen
Logik in `keyboards/keychron/common/wireless/keychron_wireless_common.c`
(Fork `~/keychron-qmk/qmk_firmware`).

## 🔑 Tasten

Seit 2026-08-31 direkt auf der `_SYS`-Ebene (Capslock-Tap-Dance halten),
Reihe 2, Position 5-9 - siehe `BELEGUNG.md`:

| Taste | Kurz drücken | 2+ Sekunden halten |
|:--|:--|:--|
| `BT_HST1` | Zu Bluetooth-Host-Slot 1 verbinden/wechseln | **Neues Gerät auf Slot 1 koppeln (Pairing)** |
| `BT_HST2` | Zu Bluetooth-Host-Slot 2 verbinden/wechseln | **Neues Gerät auf Slot 2 koppeln (Pairing)** |
| `BT_HST3` | Zu Bluetooth-Host-Slot 3 verbinden/wechseln | **Neues Gerät auf Slot 3 koppeln (Pairing)** |
| `P2P4G` | Auf 2.4G-Dongle-Modus wechseln (kein Bluetooth) | – |
| `BAT_LVL` | Akkustand kurz per LED-Balken anzeigen | – |

Zusätzlich weiterhin über `_WIN_FN` erreichbar (halten während `_SYS`
oder `_RGB` gehalten wird), siehe `BELEGUNG.md` - redundant zur direkten
`_SYS`-Bindung.

## 📶 Neues Gerät koppeln (Pairing)

1. Einen der drei Host-Slots wählen (z.B. Slot 1) und die zugehörige
   Taste **mindestens 2 Sekunden gedrückt halten**, bis losgelassen -
   nicht nur antippen (ein kurzer Tap verbindet stattdessen nur zu einem
   *bereits gekoppelten* Gerät auf diesem Slot, falls vorhanden, und tut
   sonst nichts).
2. Die LED an der entsprechenden Host-Position (Slot 1/2/3 = feste
   Positionen auf der RGB-Matrix, siehe `README.md`/Vendor-`config.h`
   `BT_HOST_LED_MATRIX_LIST`) beginnt **im Sekundentakt zu blinken** -
   das Board ist jetzt im Pairing-Modus und für den Host sichtbar.
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
`BAT_LVL` (kurz drücken) zeigt den Akkustand für ein paar Sekunden als
LED-Balken (Positionen aus `BAT_LEVEL_LED_LIST` in der Board-`config.h`).

## ⚠️ Bekannte Baustelle
`P2P4G` hat laut Quellcode denselben 2-Sekunden-Halte-Pairing-Mechanismus
wie `BT_HST1-3` (gemeinsamer `pairing_key_timer`) - für den 2.4G-Dongle
aber nicht ausprobiert/dokumentiert in dieser Session. Falls der
mitgelieferte Dongle mal neu gekoppelt werden muss: vermutlich dieselbe
2-Sekunden-Halte-Geste auf `P2P4G`, dann den Dongle-seitigen Pairing-Knopf
(falls vorhanden) betätigen - nicht verifiziert, bei Bedarf zuerst am
Gerät testen statt blind zu vertrauen.
