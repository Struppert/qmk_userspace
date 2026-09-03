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
  wurde.
- **RAM-Budget (verifiziert 2026-09-03, neo-Keymap, `.build/kbdfans_kbd8x_mk3_neo.elf`):**
  `arm-none-eabi-size` meldete für den ursprünglichen (reduzierten) Build
  `.data`+`.bss` = 17404 / 20480 Byte (~85%, ≈2,6 KB "frei"). Das zählt
  aber vier separate NOBITS-Sektionen zusammen - per `readelf -S`:
  ```
  [ 1] .mstack   NOBITS  20000c00  000400   (1024)
  [ 2] .pstack   NOBITS  20001000  000800   (2048)
  [ 8] .bss      NOBITS  20001ca8  001b98   (7064)
  [25] .heap     NOBITS  20003840  0017c0   (6080)
  ```
  1024+2048+7064+6080 = 16216, exakt `size`s "bss"-Zahl. Die 6080-Byte
  `.heap`-Sektion ist aber keine feste Reservierung - laut Kommentar im
  ChibiOS-Sourcecode selbst
  (`lib/chibios/os/common/startup/ARMCMx/compilers/GCC/ld/rules_memory.ld:309`):
  *"The default heap uses the (statically) unused part of a RAM
  section."* `__heap_end__` ist per Linkerscript-Definition exakt das
  Ende der `ram0`-Region - der Bereich ist wörtlich "was übrig bleibt",
  keine Belegung. Und: `arm-none-eabi-nm` auf dem .elf zeigt kein
  `malloc`, `calloc`, `realloc` oder `_sbrk` verlinkt - im ganzen
  Firmware-Image ruft nichts den Heap auf, also wird davon kein Byte je
  beschrieben.

  **Wichtige zweite Korrektur:** Der Nenner ist nicht 20480 (physische
  Chipgröße), sondern **17408 Byte**. `MCU_LDSCRIPT=STM32F103CB` definiert
  `ram0` explizit als `org = 0x20000000 + 0xC00, len = 20k - 0xC00` (siehe
  `keyboards/kbdfans/kbd8x_mk3/ld/STM32F103CB.ld`) - die untersten 3072
  Byte SRAM sind für den UniCore-F1-Bootloader-Handoff reserviert und der
  Anwendung strukturell nicht zugänglich (siehe Commit `63da307`: ohne
  diesen Puffer crasht der Bootloader-Jump). `.mstack`+`.pstack`+`.data`+
  `.bss`+`.heap` summieren sich in jedem gemessenen Build exakt auf
  17404-17408 - das bestätigt den echten Pool rechnerisch.

  Der Leader-Trie (`leader/trie.c`, `g_nodes`/`g_edges`) wurde mit
  exaktem Nachbau des Build-Algorithmus (Python-Simulation über die
  echten `LENTRY`-Sequenzen) durchgerechnet, nicht geschätzt: der
  damals reduzierte Satz (`table_min.c`, 23 Einträge, nur rg+fzf)
  brauchte 30 Knoten/29 Kanten = 356 Byte; die volle Tabelle
  (`table.c`, 138 Einträge, inkl. Git/WezTerm/zoxide/yazi/zellij/sed)
  braucht 160 Knoten/159 Kanten = 1916 Byte - nicht die ursprünglich
  angenommenen 8 KB (generische Default-Größe
  `LEADER_TRIE_MAX_NODES=512/EDGES=1024`, nie die tatsächlich benötigte).

  **Vier gemessene Ausbaustufen** (`users/neo/rules.mk`), jeweils
  `.mstack`(1024) + `.pstack`(2048) fix, Rest gemessen:

  | Stufe | `.data` | `.bss` | `.heap` (=frei) | frei / 17408 |
  |---|---|---|---|---|
  | 1. Ursprung (`table_min.c`, nur rg+fzf) | 1188 | 7064 | 6080 | 34,9% |
  | 2. Volle Leader-Tabelle (`table.c`, 138 Einträge, `TRIE=224/224`) | 1188 | 8984 | 4160 | 23,9% |
  | 3. + Unicode/NKRO/Combo/Introspection/`keymap_logic.c`/`DYNAMIC_MACRO_ENABLE` | 1288 | 11760 | 1288 | 7,4% |
  | 4. **Final** - wie 3, aber `DYNAMIC_MACRO_ENABLE` wieder aus | 1280 | 9200 | **3856** | **22,2%** |

  Stufe 3→4: `DYNAMIC_MACRO_ENABLE`s `macro_buffer[]`
  (`DYNAMIC_MACRO_SIZE=256`) kostete allein 2560 Byte - für QMKs eigenes,
  flüchtiges (nicht EEPROM-persistentes) `DM_REC1`/`DM_PLY1`-Makrosystem,
  das sich mit VIAs Dynamic-Keymap-Macros (12 Slots, EEPROM-persistent
  via `wear_leveling`, weiterhin aktiv) funktional überschneidet - auf
  Nutzerentscheidung deaktiviert gelassen, um das Polster zu behalten.

  Als Konsequenz sind die fünf `DM_*`-Tasten (`DM_REC1/2`, `DM_PLY1/2`,
  `DM_RSTP`) auf der `_FN`-Ebene entfernt (`users/neo/layouts/fn60.h`,
  `FN60_DM_*`-Makros: `#ifdef DYNAMIC_MACRO_ENABLE` → echter Keycode,
  sonst `KC_NO`) statt als toter, wirkungsloser Keycode auf der Tastatur
  liegen zu bleiben - Q3/V3 behalten dort ihre funktionierenden Bindings,
  da die gemeinsame Layout-Datei board-abhängig verzweigt. Details siehe
  `BELEGUNG.md`, Ebene 9 (`_FN`). RAM/Flash unverändert (Keycode-Wahl an
  toten Positionen kostet nichts) - erneut verifiziert:
  `.data`=1280, `.bss`=16128 (`size`-Summe, identisch zu Stufe 4 oben).

  Flash (Stufe 4): 53908+1280 = 55188 / ~112640 Byte nutzbar (128K minus
  16K Bootloader) ≈ **49%**.

  **Fazit: kein Chip-/Daughterboard-Tausch nötig**, aber das Polster ist
  nach voller Q3/V3-Feature-Parität knapper als in Stufe 1/2 (22% statt
  35-45% frei) - vor jedem weiteren Feature-Zuwachs (mehr Leader-Einträge,
  mehr Layer, größere RGB-Puffer) erneut `arm-none-eabi-size`/`readelf -S`
  prüfen, nicht annehmen.
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
| Pause | `[11,5]` | `UG_TOGG` (war `RM_NEXT` - totes RGB_MATRIX-Keycode). Switch war defekt, seit 2026-08-31 getauscht und funktioniert, siehe RGB-Abschnitt |

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
| RGBLIGHT_DEFAULT_ON | bewusst nicht gesetzt (QMK-Default `true`) - die wandernde Underglow-Animation ist gewünscht |

**Status (Stand 2026-08-31): Kette lief dauerhaft an/animiert - kein Bug,
sondern gewünscht, aber es gab keine Möglichkeit sie abzuschalten.**
`RM_NEXT` (letzte Taste der F-Reihe, fix auf allen Ebenen) war
`QK_RGB_MATRIX_MODE_NEXT` - ein **RGB_MATRIX**-Keycode auf einem Board, das
nur `RGBLIGHT_ENABLE=yes` hat (`RGB_MATRIX_ENABLE=no`). Die Taste war also
von Anfang an wirkungslos - es gab dadurch **keine** erreichbare
Möglichkeit, die Kette per Tastendruck an-/auszuschalten. Auf `UG_TOGG`
(den tatsächlichen RGBLIGHT-Toggle) umgestellt
(`users/neo/formfactors/ff_tkl_iso_kbd8x_mk3.h`), zusätzlich auch direkt auf
der `_SYS`-Ebene gebunden (rechte Hand, direkt vor Backspace - bewusst
nicht neben `EE_CLR`) (lokaler Override von
`SYS60_ROW1` in `keymap.c`, siehe BELEGUNG.md). Für Modus/Farbe/Helligkeit
jetzt auch VIAs Lighting-Tab nutzbar (`via.json`: `"menus": ["qmk_rgblight"]`
- vorher leer, siehe "Was noch offen ist" unten, jetzt erledigt).

**Nachtrag (behoben):** Die F-Reihen-Position von `UG_TOGG` (Matrix
`[11,5]`, Pause-Taste) hatte zunächst keinen Tastendruck registriert -
Ursache war ein defekter Switch (zusammen mit F12/`[10,5]`, per
Vendor-Firmware-Vergleich bestätigt), seit 2026-08-31 getauscht und
funktioniert. Details siehe "Was noch offen ist" unten.

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
| Neu geflashter Keycode wirkt nicht, obwohl `keymap.c` ihn korrekt zeigt | VIAs EEPROM-Gültigkeitsprüfung basiert nur auf Kalenderdatum (`QMK_BUILDDATE`), nicht Uhrzeit - zweiter Reflash am selben Tag löst keinen `dynamic_keymap_reset()` mehr aus, EEPROM bleibt auf dem Stand des ersten Flashes des Tages. `EE_CLR` behebt das NICHT (anderer EEPROM-Bereich) | `id_dynamic_keymap_reset` (Raw-HID `0x06`) explizit senden, siehe Abschnitt Raw-HID-Scripting |
| Einzelne Taste tut nichts, obwohl Keycode korrekt im EEPROM steht | War bei F12/`[10,5]` und Pause/`[11,5]` der Fall - defekte Switches (Hardware, per Vendor-Firmware-Test gegengeprüft), inzwischen getauscht und funktionieren wieder | Zur Diagnose bei anderen Tasten: per Raw-HID auf Test-Keycode umbiegen, mit `evtest` auf dem `-event-kbd`-Node gegenprüfen, ob ein Press/Release-Event ankommt; im Zweifel Original-Vendor-Firmware flashen (`~/Downloads/ydkb_kbdfans_kbd8xmk3_vial.uf2`) um Firmware- von Hardware-Ursachen zu trennen |
| `EE_CLR` gedrückt, danach fehlen VIA-Makros/Keymap-Anpassungen | `EE_CLR` formatiert auf diesem Board (wear-leveling-EEPROM) das **komplette** EEPROM (`eeprom_driver_format()`), nicht nur den allgemeinen `eeconfig`-Bereich - VIAs Dynamic-Keymap UND alle Makros sind danach weg | Makros vor jedem `EE_CLR`/Reflash-Test sichern (`tools/via_macros.py export`), siehe Raw-HID-Scripting-Abschnitt |

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
  über VIAs Lighting-Tab (`via.json` hat inzwischen die `qmk_rgblight`-Menu-
  Definition, siehe RGB-Abschnitt).
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
