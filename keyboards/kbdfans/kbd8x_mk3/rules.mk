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
# set in users/neo/rules.mk. kbd8x_mk3's real chip only has 20K RAM (see
# below) - it's actually more constrained than bella, not less.
# The 2 caps/scroll indicator LEDs are WS2812 positions on the same 6-LED
# chain as the 4 underglow LEDs (PHY_INDICATOR_NUM=2 + RGBLED_NUM=4, per the
# real firmware's kbd8xmk3_vial keymap) - NOT simple GPIO LEDs (confirmed:
# toggling PB14/PA8 polarity changed nothing but which color garbage showed,
# white then red - that's uninitialized WS2812 shift-register data, not a
# GPIO on/off effect).
#
# Tried RGBLIGHT_ENABLE=yes + an explicit ws2812_set_color_all(0,0,0) in
# keyboard_post_init_kb() (matrix.c) to force a clean all-off frame - made
# it WORSE (all 3 LEDs on the chain went bright white instead of off).
# Working hypothesis: QMK's stock ws2812 bitbang driver is timed for the
# 72 MHz clock most STM32F103 QMK boards run at; this board runs HSI-only
# at 48 MHz (see mcuconf.h - no external crystal), so the driver's pulse
# widths are very likely out of WS2812 spec, and garbled data reads back
# as "on/white" instead of "off". Needs driver-timing investigation
# (see README.md RGB section) - reverted to =no for now so at least only
# the one pre-existing stale LED stays lit instead of all three.
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
# Confirmed against the real YDKB firmware source
# (github.com/yangdigi/vial-qmk-v5, keyboards/ydkb/unicore_f1): it reserves
# the first 16K of flash, app starts at 0x08004000 - see ld/STM32F103CB.ld.
BOOTLOADER = uf2boot

# QMK defaults UF2_FAMILY to the generic "STM32F1" id (0x5ee21072) for
# MCU=STM32F103. The real bootloader uses its own custom family id -
# confirmed by inspecting the official YDKB firmware .uf2
# (ydkb_kbdfans_kbd8xmk3_vial.uf2 from ydkb.io): block header family=
# 0x9d5bcf10 (note: 0x9d5b matches our USB VID). UF2_FAMILY=0x0 (no family
# id present in our blocks) did NOT work, meaning this bootloader fork's
# UF2_IS_MY_FAMILY check apparently requires an exact match rather than
# falling back to "accept any" when the FAMILYID_PRESENT flag is absent.
UF2_FAMILY = 0x9d5bcf10

# Workaround für QMK keymap_introspection.c const-qualifier bug
# Force NO-ERROR für diesen Build-Step
CFLAGS += -Wno-error=discarded-qualifiers
CFLAGS += -Wno-error

# Real chip is STM32F103CBT6 (Medium Density, 128K flash / 20K RAM), not
# the xE/high-density part previously assumed here (512K/64K) - confirmed
# against the real YDKB firmware source. That earlier wrong assumption
# meant our .data/.bss could silently overflow the real 20K RAM at runtime
# with no link-time error, which is very likely why every flash attempt
# never actually ran (bootloader jump landed on corrupted/overflowed state).
# STM32_F103_STM32DUINO's own board.h already #defines STM32F103xB, no
# need to (redundantly, conflictingly) do it again here.

# The real variable QMK's platform.mk uses to pick a linker script is
# MCU_LDSCRIPT, not BOARD_LINKER_FILE/MCU_STM32_FLASH_SIZE/MCU_STM32_SRAM_SIZE
# (those aren't read anywhere - dead config).
MCU_LDSCRIPT = STM32F103CB

# The real firmware source explicitly relocates the vector table offset
# register to match the 16K bootloader reservation - without this the CPU
# keeps using the bootloader's vector table after the jump, which would
# explain a silent crash/revert on every boot.
OPT_DEFS += -DCORTEX_VTOR_INIT=0x4000
