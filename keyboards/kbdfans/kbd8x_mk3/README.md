# KBDFans KBD8X MK3 (custom STM32 build)

## 🧩 Überblick
Dieses Verzeichnis enthält die vollständige QMK-Firmware für das **KBDFans KBD8X MK3 (STM32/UniCore-Variante)** mit **5-poligem VDSRG-Anschluss**.

- Matrix: 12×8 via Shift-Register (74HC165)
- Layout: ISO TKL (6.25u Space, normaler RShift, normaler Backspace)
- MCU: STM32F103xE-kompatibel, 512 KB Flash / 64 KB SRAM (empirisch bestätigt
  über `ld/STM32F103xE.ld` - baut und läuft stabil; die "CBT6"-Angabe einer
  früheren README-Version, die nur 128 KB/20 KB nahelegt, passt nicht dazu)
- Bootloader: UniCore-F1 (ydkb.io), UF2-Drag&Drop, VID:PID `1209:db42`,
  reserviert die ersten 16 KB Flash

## ⚙️ Build
```bash
qmk compile -kb kbdfans/kbd8x_mk3 -km neo
```
Alle nötigen Einstellungen (Speichergrößen, EEPROM-Treiber, Bootloader) stehen
bereits in `rules.mk`/`keymaps/neo/config.h` - keine `-e`-Overrides nötig.

## 🔌 Flashen
**Bootloader-Modus:** UniCore-F1 (ydkb.io), meldet sich als USB-Laufwerk
("UniCore-F1", VID:PID `1209:db42`). Board vom USB trennen, Esc (oben links)
halten, wieder anstecken, halten bis der Bootloader-Modus aktiv ist
(`lsusb` zeigt `1209:db42`). Dann:
```bash
qmk flash -kb kbdfans/kbd8x_mk3 -km neo
```
Falls das Board nach dem Schreiben nicht automatisch neu startet: Kabel
einmal aus- und wieder einstecken (normaler Power-Cycle).

**Alternativ:** ST-Link über SWDIO/SWCLK.

## 🔧 Hardware-Zusammenfassung
| Signal | MCU-Pin | Funktion | Kommentar |
|:--|:--|:--|:--|
| SR_LATCH | PB12 | Shift-Register Latch | R |
| SR_CLOCK | PB13 | Shift-Register Clock | S |
| SR_DATA  | PB14 | Shift-Register Data | D |
| RGB_DI | PB15 | WS2812 Data Out | optional |
| RST Pads | NRST↔GND | Hard-Reset | |
| SPI1 | PA5/6/7 + GND | Erweiterung | |
| SWD | PA13/14 + GND | Debug | |
| Connector VDSRG | 5-Pin | Verbindung Mainboard | V,D,S,R,G |

## 💡 VIA
- VIA aktiviert (`VIA_ENABLE = yes`, users/neo/rules.mk).
- Vial ist **nicht** konfiguriert (kein `VIAL_ENABLE`, kein `vial.json`) -
  der "(VIAL_DO4U)"-Zusatz im USB-Produktstring ist nur ein Textrest,
  keine tatsächliche Vial-Unterstützung.
- Lokale VIA-Definition liegt in `via.json` (manuell in der VIA-App laden,
  Design-Tab → Load Draft Definition - das Board ist nicht im offiziellen
  VIA-Katalog registriert).

## 🌈 RGB
| Parameter | Wert |
|:--|:--|
| RGB_DI_PIN | PB15 (WS2812, in config.h definiert) |
| RGBLIGHT_ENABLE | `no` (rules.mk) - Hardware vorhanden, Feature aktuell deaktiviert |

## 🧰 Fehlerbehebung
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Keine Reaktion | Firmware fehlt/korrupt | Board in Bootloader versetzen (Esc halten beim Anstecken), neu flashen |
| Tasten falsch | CLOCK/LATCH vertauscht | `config.h` SR_LATCH_PIN/SR_CLOCK_PIN anpassen |
| Bootloader nicht erkannt | `1209:db42` fehlt in `lsusb` | Board neu in Bootloader versetzen, USB-Berechtigung prüfen (`/etc/udev/rules.d/`) |
| Board bleibt nach Flash im Bootloader | UniCore-F1 resettet nicht automatisch | USB-Kabel einmal aus-/wieder einstecken |
| RGB tot | RGBLIGHT_ENABLE=no | In rules.mk aktivieren |

## 🧠 Erweiterung
- SPI1 (PA5/6/7) → FRAM, Flash, OLED
- I2C (PB6/PB7) → Sensoren
- RST Pads → optionaler Taster (aktuell nicht bestückt - Software-Bootloader-
  Eintritt über `QK_BOOT`-Taste auf der SYS-Ebene ist deshalb der einzige
  Weg ohne physischen Reset)
- VOUT 3V3 → Versorgung externer Module

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Based on: KBD8X MK2 (QMK) + custom STM32 port
```
