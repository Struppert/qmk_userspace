# 📘 README.md – KBD8X MK3 (STM32 Variant)

## 🧩 Überblick
Dieses Verzeichnis enthält die vollständige QMK-Firmware für das **KBDFans KBD8X MK3 (STM32/UniCore-Variante)** mit **APM32F103CBT6 MCU** und **5-poligem VDSRG-Anschluss**.

- Matrix: 12×8 via Shift-Register (74HC165)
- Layout: ISO TKL (6.25u Space, normaler RShift, normaler Backspace)
- MCU: APM32F103CBT6 (STM32F103-kompatibel)
- Bootloader: DFU

## ⚙️ Build
```bash
qmk setup
qmk compile -kb kbdfans/kbd8x_mk3_stm32 -km neo

erst mal so:
qmk clean -a
qmk compile -kb kbdfans/kbd8x_mk3 -km neo \
  -e OPT_DEFS+=-DSTM32F103xE \
  -e MCU_STM32_FLASH_SIZE=512 \
  -e MCU_STM32_SRAM_SIZE=64 \
  -e KEYMAP_INTROSPECTION_ENABLE=yes \
  -e OPT_DEFS+=-DKEYMAP_C=\"keyboards/kbdfans/kbd8x_mk3/keymaps/neo/keymap.c\" \
  -e EEPROM_ENABLE=yes \
  -e EEPROM_DRIVER=wear_leveling

```

## 🔌 Flashen
**DFU-Modus:**  
RESET-Pads kurzschließen oder `QMK Toolbox → Tools → Reset Keyboard`.  
Dann:
```bash
qmk flash -kb kbdfans/kbd8x_mk3_stm32 -km neo
```

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

## 💡 VIA / Vial
- VIA und Vial aktiviert.
- Vial erkennt das Layout automatisch.

## 🌈 RGB Licht
| Parameter | Wert |
|:--|:--|
| RGB_DI_PIN | PB15 |
| RGBLED_NUM | 16 |
| Default Mode | Rainbow Mood |
| Limit | 150 |

## 🧰 Fehlerbehebung
| Problem | Ursache | Lösung |
|:--|:--|:--|
| Keine Reaktion | Firmware fehlt | DFU flashen |
| Tasten falsch | CLOCK/LATCH vertauscht | `config.h` anpassen |
| DFU nicht erkannt | Bootloader defekt | ST-Link nutzen |
| RGB tot | PB15 unbeschaltet | RGB deaktivieren |

## 🧠 Erweiterung
- SPI1 (PA5/6/7) → FRAM, Flash, OLED
- I2C (PB6/PB7) → Sensoren
- RST Pads → optionaler Taster
- VOUT 3V3 → Versorgung externer Module

## 🧾 Lizenz & Autor
```
Maintainer: neo
License: GPLv2
Based on: KBD8X MK2 (QMK) + custom STM32 port
```
