# === Platform / MCU ===
PLATFORM = chibios
MCU      = STM32F103
# WICHTIG: keine angehängten Spaces!
BOARD    = STM32_F103_STM32DUINO
# (Kommentar in eigene Zeile, nicht hinter die Zuweisung)

#EEPROM_ENABLE        = yes
#EEPROM_DRIVER        = wear_leveling
#WEAR_LEVELING_ENABLE = yes
#EFL_ENABLE           = yes

EEPROM_ENABLE         = no
WEAR_LEVELING_ENABLE  = no
EFL_ENABLE            = no

# --- Board-Datei fest verdrahten (RELATIV ab Repo-Root) ---
# NICHT selbst includen; nur Variable setzen, platform.mk includet später.
BOARD_MK := platforms/chibios/boards/$(BOARD)/board/board.mk


# === Features ===
KEYMAP_INTROSPECTION_ENABLE = yes

LTO_ENABLE        = yes
VIA_ENABLE        = yes
RGBLIGHT_ENABLE   = yes
RGB_MATRIX_ENABLE = no
BACKLIGHT_ENABLE  = no
AUDIO_ENABLE      = no
CONSOLE_ENABLE    = no
COMMAND_ENABLE    = no
EXTRAKEY_ENABLE   = yes

# === Custom Matrix (74HC165) ===
CUSTOM_MATRIX = yes
SRC += matrix.c

# Keine STM32Cube/HAL anziehen
CFLAGS += -UUSE_HAL_DRIVER

RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no

BOOTLOADER = stm32-dfu
STM32_BOOTLOADER_ADDRESS = 0x1FFFF000

#OPT_DEFS += -DEEPROM_DRIVER=wear_leveling

OPT_DEFS += -DKEYMAP_C=\"keyboards/kbdfans/kbd8x_mk3/keymaps/neo/keymap.c\"
KEYMAP_INTROSPECTION_ENABLE = yes

OPT_DEFS += -DSTM32F103xE
# absolut oder relativ vom Repo-Root:

# >>> WICHTIG: Speichergrößen für rules_memory.ld setzen <<<
MCU_STM32_FLASH_SIZE = 512    # KiB (xE)
MCU_STM32_SRAM_SIZE  = 64     # KiB (xE)

BOARD_LINKER_FILE := $(PLATFORM_PATH)/boards/STM32_F103_STM32DUINO/ld/STM32F103xE.ld

#override EEPROM_DRIVER = wear_leveling
