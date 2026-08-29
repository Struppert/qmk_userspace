# === Platform / MCU ===
PLATFORM = chibios
MCU      = STM32F103
# WICHTIG: keine angehängten Spaces!
BOARD    = STM32_F103_STM32DUINO
# (Kommentar in eigene Zeile, nicht hinter die Zuweisung)

# EEPROM_DRIVER left at its default (vendor) like v3 - auto-selects
# wear_leveling + embedded_flash for STM32F1xx and sets WEAR_LEVELING_DRIVER
# correctly (a direct `EEPROM_DRIVER = wear_leveling` skips that step and
# breaks wear_leveling_drivers.h generation).

# --- Board-Datei fest verdrahten (RELATIV ab Repo-Root) ---
# NICHT selbst includen; nur Variable setzen, platform.mk includet später.
BOARD_MK := platforms/chibios/boards/$(BOARD)/board/board.mk


# === Features ===
# VIA/UNICODE/NKRO/LTO/TAP_DANCE/COMBO/KEYMAP_INTROSPECTION/RGB_MATRIX are
# set in users/neo/rules.mk (full-featured branch, kbd8x_mk3 is not memory
# constrained like bella). Only board-specific hardware flags here.
RGBLIGHT_ENABLE   = no
BACKLIGHT_ENABLE  = no
AUDIO_ENABLE      = no
COMMAND_ENABLE    = yes

# === Custom Matrix (74HC165) ===
CUSTOM_MATRIX = yes
# KEYBOARD_SRC is reset by build_keyboard.mk *after* rules.mk is loaded
# (builddefs/build_keyboard.mk:252) - anything appended to it here is
# silently dropped. SRC is the correct variable for extra source files.
SRC += keyboards/kbdfans/kbd8x_mk3/matrix.c
# Note: quantum/matrix_common.c must NOT be linked - it causes ARM/Thumb mode conflicts
# All required functions (matrix_get_row, etc) are defined in our matrix.c

CFLAGS += -mthumb
ASFLAGS += -mthumb

# Keine STM32Cube/HAL anziehen
CFLAGS += -UUSE_HAL_DRIVER

# Actual hardware bootloader is UniCore-F1 (ydkb.io), a UF2 drag-and-drop
# bootloader (1209:db42, mounts as a FAT drive "UniCore-F1") - NOT ST's own
# ROM DFU. `qmk flash` copies the .uf2 onto that drive automatically.
# It reserves the first 16K of flash (confirmed by the first successful
# flash, which linked against the stock STM32F103x8_uf2boot.ld by accident
# and booted correctly) - see ld/STM32F103xE_uf2boot.ld for the real xE
# memory layout with that same 16K reservation.
BOOTLOADER = uf2boot

# Workaround für QMK keymap_introspection.c const-qualifier bug
# Force NO-ERROR für diesen Build-Step
CFLAGS += -Wno-error=discarded-qualifiers
CFLAGS += -Wno-error

OPT_DEFS += -DSTM32F103xE

# The real variable QMK's platform.mk uses to pick a linker script is
# MCU_LDSCRIPT, not BOARD_LINKER_FILE/MCU_STM32_FLASH_SIZE/MCU_STM32_SRAM_SIZE
# (those aren't read anywhere - dead config). Without this, MCU=STM32F103
# defaults to MCU_LDSCRIPT=STM32F103x8 (20KB SRAM), which is why we hit a
# "cannot move location counter backwards" RAM overflow - the linker thought
# we had 20KB, not the real chip's 64KB.
MCU_LDSCRIPT = STM32F103xE
