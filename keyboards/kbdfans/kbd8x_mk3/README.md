# KBDFans KBD8X MK3 (custom STM32 build)

## 🧩 Überblick
Dieses Verzeichnis enthält eine vollständig eigene QMK-Firmware für das
**KBDFans KBD8X MK3 (STM32/UniCore-Variante)**, rekonstruiert gegen die
reale Vendor-Firmware (github.com/yangdigi/vial-qmk-v5, Branch `ava`,
`keyboards/ydkb/unicore_f1`), da die ursprünglichen Annahmen (74HC165,
STM32F103xE, HSE-Takt) alle falsch waren und das Board damit nie gebootet
bzw. nie getippt hat.

- MCU: **STM32F103CBT6** (Medium Density, 128 KB Flash / 20 KB SRAM) - NICHT
  die "xE"-Variante (512 KB/64 KB), die früher hier fälschlich angenommen
  wurde. RAM ist entsprechend knapp bemessen (~17.4 KB von 20 KB genutzt).
- Takt: **HSI intern** (kein externer Quarz vorhanden), 4 MHz PLL-Eingang ×
  PLLMUL 12 = 48 MHz SYSCLK, USB direkt bei 48 MHz (kein USBPRE-Teiler
  nötig). Eine HSE-Konfiguration hängt beim Booten für immer (kein Quarz
  bestückt) - das war der Grund, warum frühere Builds nie USB enumeriert
  haben.
- Matrix: **seriell, 2-Draht "walking bit"-Scan** (SCK=PB12, SDI=PB13
  bidirektional) - KEIN 74HC165-Paralleleingangs-Schieberegister. Siehe
  `matrix.c` für das genaue Protokoll. `MATRIX_ROWS=14` (Kettenlänge,
  reale Tasten nur in Zeilen 0-11).
- Layout: ISO TKL, 96 Tasten
- Bootloader: UniCore-F1 (ydkb.io), UF2-Drag&Drop, VID:PID `1209:db42`,
  reserviert die ersten 16 KB Flash (App startet bei `0x08004000`)

## ⚙️ Build
```bash
qmk compile -kb kbdfans/kbd8x_mk3 -km neo
```

## 🔌 Flashen
**Bootloader-Modus:** Board vom USB trennen, Esc (oben links) halten,
wieder anstecken, halten bis der Bootloader-Modus aktiv ist (`lsusb` zeigt
`1209:db42`, mountet als FAT-Laufwerk "UniCore-F1").

**Empfohlen** (schneller und zuverlässiger als `qmk flash`, welches
`uf2conv.py --wait` nutzt und öfter hängen bleibt):
```bash
cp .build/kbdfans_kbd8x_mk3_neo.uf2 /run/media/$USER/UniCore-F1/NEW.UF2
sync
```
Das Board bootet danach automatisch neu (kein manueller Power-Cycle nötig).

**Alternativ:** `qmk flash -kb kbdfans/kbd8x_mk3 -km neo`

## 🔧 Hardware-Zusammenfassung
| Signal | MCU-Pin | Funktion |
|:--|:--|:--|
| SCK | PB12 | Matrix-Scan Clock (Output) |
| SDI | PB13 | Matrix-Scan Data (bidirektional); doppelt genutzt für Pull-up/-down-Autodetect (`has_extra_pullup`, `matrix_init_custom()`) |
| Rev-Detect | PB9 | Pull-up, erkennt `ver5020`-Variante (`is_ver5020`) - welcher Zweig dieses konkrete Board nimmt, ist nicht geloggt/verifiziert, aber die Matrix funktioniert einwandfrei damit |
| caps/scroll GPIO (Fallback, unbestätigt) | PB14 / PA8 | Redundanter simple-GPIO-Pfad wie in der Vendor-Referenz (`single_color_indicator_set()`, on=HIGH) - zeigte empirisch **keine sichtbare Wirkung** auf diesem Board; die tatsächlich funktionierende Indicator-Ansteuerung läuft über die WS2812-Kette (siehe unten) |
| RGB_DI | PB15 = TIM1_CH3N | WS2812 Data Out über Hardware-PWM+DMA (2 Indikator-LEDs @ Position 0/1 + 4 Underglow-LEDs @ Position 2-5, Kette = 6) |
| RST Pads | NRST↔GND | Hard-Reset (kein Taster bestückt) |
| SWD | PA13/14 + GND | Debug |

## 🗺️ Physische Tastenpositionen (Matrix-Koordinaten)
Nicht offensichtlich aus dem Code - Positionsargumente in den `*_ROW*`-Makros
werden 1:1 der Reihe nach den `"matrix":[row,col]`-Einträgen aus
`keyboard.json` zugeordnet, nicht nach Augenschein. Wichtige, in dieser
Session verifizierte Zuordnungen (per `dynamic_keymap_get_keycode`
gegengeprüft):

| Physische Taste | Matrix | Default-Keycode (Layer 0, `_QWERTZ`) |
|:--|:--|:--|
| Tab | `[2,0]` | `KC_TAB` |
| Capslock | `[2,7]` | `TD(TD_CAPS_SYS_LEAD)` - Tap→`KC_ESC`, Hold→Layer `_SYS` (kein `KC_CAPS`!) |
| Druck/PrtScn | `[10,0]` | `KC_PSCR` |
| Scroll Lock | `[11,4]` | `KC_NO` (unbelegt - keine der `*_ROW0`-Makros vergibt hier `KC_SCRL`) |
| Pause | `[11,5]` | `RM_NEXT` |

Capslock sendet also standardmäßig **kein** `KC_CAPS` - das ist Absicht
(User-Preference, Capslock gilt als "unnötigster Key", daher Tap-Dance-
Überladung). Für einen Live-Test wurden `KC_CAPS`/`KC_SCRL` testweise per
VIA gesetzt und per `id_dynamic_keymap_reset` (Raw-HID-Kommando `0x06`,
ohne führendes Report-ID-Byte, s.u.) wieder auf die kompilierten Defaults
zurückgesetzt - kein Neuflash nötig, siehe Abschnitt "Raw-HID-Scripting".

## 💡 VIA
- VIA aktiviert (`VIA_ENABLE = yes`, `users/neo/rules.mk`).
  `VIA_PROTOCOL_VERSION = 0x000C` (12, QMK-Default aus `quantum/via.h`).
- Vial ist **nicht** konfiguriert (kein `VIAL_ENABLE`, kein `vial.json`).
- Lokale VIA-Definition liegt in `via.json` (manuell in der VIA-App laden,
  Design-Tab → Load Draft Definition, dann im Configure-Tab auf
  "Authorize Device" klicken - das Board ist nicht im offiziellen
  VIA-Katalog registriert). Struktur wurde gegen `keyboard.json` verifiziert
  (Matrixpositionen 1:1 deckungsgleich).
- `via.json` nutzt das **v3-Schema** (`menus: []`, `firmwareVersion: 0`,
  kein `lighting`-Feld mehr) - ab `VIA_PROTOCOL_VERSION` 11+ verlangt die
  aktuelle usevia.app-Version zwingend eine v3-Definition; die alte
  `lighting: "none"`-basierte v2-Datei wurde deshalb von VIA abgelehnt
  ("Fetching v3 definition failed"), unabhängig davon, welche Version man
  im Datei-Upload-Dialog manuell auswählt.

**Status (Stand 2026-08-30): verbindet und funktioniert.** Die
Verbindung selbst lief nach einem Reboot sauber an - der zuvor vermutete
Raw-HID-"off-by-one"-Bug in der Firmware war offenbar kein reproduzierbarer
Firmware-Fehler (eher ein VIA-/App-seitiger Verbindungs-Cache-Zustand, der
sich durch den Reboot gelöst hat).

**Bug gefunden & gefixt: Layer 1-11 zeigten in VIA Müll, nur Layer 0 war
korrekt.** Ursache war ein Mismatch zwischen `via.json`s `"matrix"`-Feld
(`rows: 12`) und der echten Firmware-`MATRIX_ROWS` (14, siehe `config.h` -
die Kettenlänge des seriellen Scans, nicht die Anzahl belegter Zeilen).
VIAs `DYNAMIC_KEYMAP_GET_BUFFER`-Aufruf berechnet den Byte-Offset pro
Layer aus *seiner eigenen* `matrix`-Angabe, während die Firmware
(`quantum/dynamic_keymap.c`) intern strikt mit dem kompilierten
`MATRIX_ROWS`×`MATRIX_COLS` als Stride im EEPROM/Wear-Leveling-Speicher
arbeitet. Bei 12 vs. 14 Zeilen driftet der Offset pro Layer um
(14-12)×8×2 = 32 Byte auseinander - Layer 0 (Offset 0) blieb dadurch
zufällig korrekt, jeder folgende Layer war zunehmend verschoben. Fix:
`via.json`s `"matrix"` auf `{"rows": 14, "cols": 8}` gesetzt (das
`"layout"`-Array mit den physischen Tastenpositionen 0-11 bleibt
unverändert). Reiner VIA-Definitions-Fix, kein Firmware-Rebuild/Flash
nötig - einfach die Draft Definition in VIA neu laden.

## 🌈 RGB / Status-LEDs
| Parameter | Wert |
|:--|:--|
| WS2812_DI_PIN | PB15 = TIM1_CH3N (Default-AF, kein Remap) |
| WS2812_DRIVER | `pwm` (Hardware-PWM+DMA, siehe unten) |
| Kette | 6 LEDs: 2 Capslock/Scroll-Indikatoren (Position 0/1) + 4 Underglow (Position 2-5), per realer Vendor-Firmware: `PHY_INDICATOR_NUM=2`, `RGBLED_NUM=4` |
| RGBLIGHT_LAYERS | 2 Layer (Caps=rot @0, Scroll=blau @1), `RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF` gesetzt, `RGBLIGHT_MAX_LAYERS 2` (RAM-Sparen ggü. Default 8) |
| RGBLIGHT_ENABLE | `yes` |
| RGBLIGHT_LIMIT_VAL | `50` (Sicherheits-Obergrenze für Helligkeit/Stromaufnahme, 1:1 vom Vendor-Wert übernommen, nicht selbst gegen den USB-Port gemessen) |

**Status (Stand 2026-08-30): Capslock-Indicator funktioniert, Kette geht
sauber auf Schwarz.** Root Cause für die alten "LEDs bleiben an/weiß"-Symptome
war der QMK-Standard-WS2812-**Bitbang**-Treiber, dessen NOP-Timing-Schleife
auf diesem Board (48 MHz HSI-only SYSCLK statt der für STM32F103-Boards
üblichen 72 MHz) offenbar nicht sauber lief - Details/Verifikation dazu
nicht mehr rekonstruierbar, der Wechsel auf den **Hardware-PWM+DMA-Treiber**
(TIM1 CH3N, taktunabhängig da timer-getaktet statt NOP-Loop-getaktet) hat
das Problem behoben: die Kette geht jetzt deterministisch sauber auf Schwarz.

Indicator-Implementierung: Caps/Scroll werden als `RGBLIGHT_LAYERS`-Overlay
auf Kettenposition 0/1 umgesetzt (`matrix.c`, `led_update_kb()` +
`keyboard_post_init_kb()`), nicht wie in der Vendor-Referenz über direkte
GPIO-Pins (PB14/PA8) - letztere sind hier nur redundant als Fallback belassen
(vendor-Polarität on=HIGH übernommen), zeigten aber empirisch keine
Wirkung. Wichtiger QMK-Kern-Stolperstein dabei: `RGBLIGHT_LAYERS`-Overlays
rendern standardmäßig nur, wenn der globale `rgblight_config.enable`
(RGB-Hauptschalter) an ist - `RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF` in config.h
hebt das auf, damit der Indicator unabhängig vom RGB-Modus sichtbar ist.

**Scrolllock-LED reagiert nicht - aber das ist kein Firmware-Bug.**
`evtest` auf dem Keyboard-Event-Device zeigt einen sauberen
`KEY_SCROLLLOCK`-Press/Release von der Firmware, aber der zugehörige
`EV_LED`-Report vom Kernel ist `LED_NUML`, nicht `LED_SCROLLL` - der
Host toggelt beim Scrolllock-Tastendruck also aus irgendeinem
Grund die Numlock-LED-Bit statt der Scrolllock-LED-Bit (Linux-VT-
Tastaturschicht, nicht QMK). Da derselbe Indicator-Code-Pfad für Capslock
nachweislich funktioniert, ist die Firmware-Seite hier nicht die Ursache.

## 🐍 Raw-HID-Scripting (VIA-Protokoll direkt ansprechen)
Für gezielte Diagnose/Reset-Aktionen ohne die VIA-App: Python `hidapi` gegen
das Raw-HID-Interface (`/dev/hidrawN`, Interface `.1` bei diesem Board -
Interface `.0` ist die normale Tastatur, `.2` System/Consumer-Control).
Passenden Node finden:
```bash
udevadm info -q property -p /sys/class/hidraw/hidrawN | grep HID_ID
# gesucht: HID_ID=0003:00009D5B:00002303, Interface .1
```
**Wichtig:** Reports sind exakt 32 Byte, **ohne** führendes Report-ID-Byte
(anders als die übliche hidapi-Konvention!) - ein vorangestelltes `0x00`
verschiebt alle Felder um 1 und die Firmware ignoriert den Befehl
stillschweigend (sieht wie eine tote Verbindung aus, ist aber nur ein
falsches Report-Format). Außerdem kann nur eine Gegenstelle sinnvoll
gleichzeitig kommunizieren - `fuser -v /dev/hidrawN` prüfen, ob z.B. noch
ein VIA-Browser-Tab offen ist, bevor man "kaputte Verbindung" vermutet.

Nützliche Kommandos (`quantum/via.h`):
| Kommando | Byte 0 | Zweck |
|:--|:--|:--|
| `id_get_protocol_version` | `0x01` | Verbindungstest, Antwort enthält Protokollversion |
| `id_dynamic_keymap_get_keycode` | `0x04` | + Layer, Row, Col → liest einen Keycode |
| `id_dynamic_keymap_reset` | `0x06` | Setzt **alle** Layer/Keys auf die kompilierten Defaults zurück (macht dasselbe wie VIAs "Reset Keyboard to Default", aber gezielt ohne die GUI - praktisch nach Live-Tests einzelner Keys über VIA) |

## 🧰 Fehlerbehebung
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Keine Reaktion, kein USB | HSE-Takt konfiguriert, aber kein Quarz vorhanden | `mcuconf.h`: HSI statt HSE, siehe Kommentare dort |
| Tasten falsch/verschoben | Argument-Count-Mismatch in `ff_tkl_iso_kbd8x_mk3.h` | Physische Tastenzahl pro Zeile (`keyboard.json`) gegen die vom Formfactor gelieferte Argumentzahl prüfen |
| Bootloader nicht erkannt | `1209:db42` fehlt in `lsusb` | Board neu in Bootloader versetzen, USB-Berechtigung prüfen (`/etc/udev/rules.d/`) |
| Board bleibt nach Flash im Bootloader | Selten, aber möglich | USB-Kabel einmal aus-/wieder einstecken |
| Status-LEDs falsch/permanent an | WS2812-Bitbang-Treiber lief bei 48 MHz nicht sauber | Gelöst - PWM+DMA-Treiber, siehe RGB-Abschnitt |
| Scrolllock-LED reagiert nicht | Host toggelt LED_NUML statt LED_SCROLLL (kein Firmware-Bug) | Siehe RGB-Abschnitt |
| VIA zeigt Layer 1+ als Müll | `via.json`-Matrix (12 Zeilen) wich von echter `MATRIX_ROWS` (14) ab | Gelöst - siehe VIA-Abschnitt |
| VIA verbindet nicht ("All retries failed") | Vermutlich App-seitiger Verbindungscache | Behob sich nach Reboot |

## 🔓 Was noch offen ist / weitere Möglichkeiten
Bewusst nicht (weiter) verfolgt oder unbestätigt - kein akuter Handlungsbedarf,
aber hier für die nächste Session festgehalten:

- **Physische Zuordnung der WS2812-Kettenpositionen 1-5 unbekannt.** Nur
  Position 0 (Capslock, rot) wurde visuell bestätigt. Es ist unklar, welche
  physische LED auf der Platine welcher Kettenposition entspricht - z. B.
  gibt es unterhalb von PgDn eine LED, die in der originalen Vial-Firmware
  eine Lauflicht-/Animationsfunktion zeigt (vermutlich Teil des
  `RGBLIGHT`-Default-Animationsmodus auf den Underglow-Positionen 2-5).
  Wer die komplette Kette durchmappen will: `ws2812_set_color(i, 255,0,0)`
  einzeln für `i=0..5` in `keyboard_post_init_kb()` testen, oder einfacher
  über VIA/Raw-HID, sobald `via.json` eine Lighting-Menu-Definition hat.
- **RGB-Underglow (Position 2-5) läuft hardware-seitig sauber (PWM-Fix),
  aber Farben/Animation wurden nie bewusst angeschaut/eingestellt** -
  bisher nur "aus" (Boot-Default) und Capslock-Rot getestet. Modus/Farbe/
  Helligkeit lassen sich über QMKs Standard-`RGBLIGHT`-Tastenkombinationen
  einstellen (falls im Keymap gebunden) oder per Raw-HID.
- **VIA-Lighting-Tab ist leer** (`via.json`s `"menus": []`) - RGB-Einstellungen
  sind aktuell nur über Firmware-seitige Keycodes/Raw-HID erreichbar, nicht
  über VIAs GUI-Slider. Bei Bedarf: `via.json` um eine
  `qmk_rgblight`-Menu-Definition ergänzen (Standard-QMK-Feature, siehe
  VIA-Doku "Lighting Menus").
- **PB14/PA8-GPIO-Fallback zeigt keine bestätigte Wirkung** - Code ist als
  harmloser Vendor-kompatibler Fallback belassen (siehe `matrix.c`), könnte
  bei Gelegenheit sauber isoliert getestet werden (WS2812 kurz deaktivieren,
  nur GPIO toggeln) oder als toter Code entfernt werden.
- **Scrolllock-LED (Host-Kernel-Quirk, LED_NUML statt LED_SCROLLL) bewusst
  nicht weiterverfolgt** - auf Nutzerwunsch, da Scroll Lock nicht verwendet
  wird.
- **`is_ver5020`/`has_extra_pullup`-Autodetection nie explizit geloggt** -
  Matrix funktioniert einwandfrei, aber welchen Zweig dieses konkrete Board
  tatsächlich nimmt, wurde nie über z. B. `CONSOLE_ENABLE`+`print()` verifiziert.
- **RGBLIGHT_LIMIT_VAL=50** 1:1 vom Vendor übernommen, nie gegen den
  tatsächlichen USB-Port dieses Rechners gemessen/validiert.

## 🧠 Erweiterung
- SPI1 (PA5/6/7) → FRAM, Flash, OLED
- I2C (PB6/PB7) → Sensoren
- RST Pads → optionaler Taster (aktuell nicht bestückt - Software-Bootloader-
  Eintritt über `QK_BOOT`-Taste auf der SYS-Ebene ist deshalb der einzige
  Weg ohne physischen Reset)

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Based on: KBD8X MK3 (QMK) + custom STM32 port, gegen reale Vendor-Firmware
          (github.com/yangdigi/vial-qmk-v5) rekonstruiert
```
