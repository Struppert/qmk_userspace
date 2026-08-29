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
| SDI | PB13 | Matrix-Scan Data (bidirektional) |
| Rev-Detect | PB9 | Pull-up, erkennt ver5020-Variante |
| caps/scroll indicator | PB14 / PA8 | Teil der WS2812-Kette, nicht simple GPIO-LEDs (siehe RGB-Abschnitt) |
| RGB_DI | PB15 | WS2812 Data Out (2 Indikator-LEDs + 4 Underglow-LEDs, Kette = 6) |
| RST Pads | NRST↔GND | Hard-Reset |
| SWD | PA13/14 + GND | Debug |

## 💡 VIA
- VIA aktiviert (`VIA_ENABLE = yes`, `users/neo/rules.mk`).
- Vial ist **nicht** konfiguriert (kein `VIAL_ENABLE`, kein `vial.json`).
- Lokale VIA-Definition liegt in `via.json` (manuell in der VIA-App laden,
  Design-Tab → Load Draft Definition - das Board ist nicht im offiziellen
  VIA-Katalog registriert). Struktur wurde gegen `keyboard.json` verifiziert
  (Matrixpositionen 1:1 deckungsgleich).

## 🌈 RGB / Status-LEDs — bekanntes offenes Problem
| Parameter | Wert |
|:--|:--|
| RGB_DI_PIN | PB15 (WS2812) |
| Kette | 6 LEDs: 2 Capslock/Scroll-Indikatoren + 4 Underglow (per realer Vendor-Firmware: `PHY_INDICATOR_NUM=2`, `RGBLED_NUM=4`) |
| RGBLIGHT_ENABLE | `yes` (nur damit der ws2812-Treiber gelinkt wird) |
| RGBLIGHT_DEFAULT_ON | `false` |

**Status (Stand 2026-08-30): ungelöst.** Die beiden Indikator-LEDs
(Capslock-Bereich, links-unten + rechts-oben am Switch, plus eine dritte
Status-LED oben rechts nahe Druck-Taste) sind Teil der WS2812-Kette, nicht
einfache GPIO-LEDs - Umpolen von PB14/PA8 änderte nur die angezeigte Farbe
(weiß → rot), nicht den An/Aus-Zustand, was zu Restdaten im
Schieberegister statt echtem GPIO-Effekt passt. Ein expliziter
`ws2812_set_color_all(0,0,0)` + `ws2812_flush()` in
`keyboard_post_init_kb()` (matrix.c) sollte die Kette leeren, hat aber
stattdessen alle drei LEDs hell weiß gezeigt statt sie auszuschalten.

**Arbeitshypothese:** QMKs Standard-WS2812-Bitbang-Treiber ist vermutlich
auf die für STM32F103-Boards übliche 72 MHz-Taktrate kalibriert; dieses
Board läuft aber bei 48 MHz (siehe oben, HSI-bedingt) - die Pulsbreiten im
Treiber dürften dadurch außerhalb der WS2812-Spezifikation liegen, was die
LEDs als "an/hell" statt "aus" interpretieren lässt. Die reale
Vendor-Firmware hat eine eigene, vermutlich für 48 MHz kalibrierte
ws2812-Implementierung (nicht übernommen). Nächster Schritt: Timing-Loop
des QMK-Treibers gegen die tatsächliche `STM32_SYSCLK` prüfen/kalibrieren,
oder alternative Clock-Konfiguration (72 MHz SYSCLK + USBPRE_DIV1P5 für
weiterhin 48 MHz USB) evaluieren.

## 🧰 Fehlerbehebung
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Keine Reaktion, kein USB | HSE-Takt konfiguriert, aber kein Quarz vorhanden | `mcuconf.h`: HSI statt HSE, siehe Kommentare dort |
| Tasten falsch/verschoben | Argument-Count-Mismatch in `ff_tkl_iso_kbd8x_mk3.h` | Physische Tastenzahl pro Zeile (`keyboard.json`) gegen die vom Formfactor gelieferte Argumentzahl prüfen |
| Bootloader nicht erkannt | `1209:db42` fehlt in `lsusb` | Board neu in Bootloader versetzen, USB-Berechtigung prüfen (`/etc/udev/rules.d/`) |
| Board bleibt nach Flash im Bootloader | Selten, aber möglich | USB-Kabel einmal aus-/wieder einstecken |
| Status-LEDs falsch/permanent an | WS2812-Timing bei 48 MHz (siehe RGB-Abschnitt) | Ungelöst - siehe oben |

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
