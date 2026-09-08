# VIA & Features
VIA_ENABLE        = yes
ENCODER_ENABLE    = yes
RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE   = no

OPT_DEFS += -Wno-lto-type-mismatch

# Keychron's own keyboards/keychron/common/wireless/lkbt51.c (LKBT51 BT-Chip
# DFU-Treiber) hat eine ungenutzte Variable (checksum in lkbt51_dfu_tx) -
# mit dem hier installierten, sehr neuen arm-none-eabi-gcc wird das als
# -Werror=unused-but-set-variable ein harter Fehler statt nur Warnung.
# Gleiches Muster wie beim V1 Max-Fix - Vendor-Code, nicht unser Bug.
CFLAGS += -Wno-error=unused-but-set-variable

# Tetris-Prototyp auf eigenem Layer (_TETRIS, Zugang via TG(_TETRIS) auf
# _SYS), portiert von v1_max - siehe keyboards/keychron/v1_max/TETRIS.md.
# Standardmäßig aus, damit normale Builds unangetastet bleiben.
TETRIS_GAME_ENABLE ?= no
ifeq ($(strip $(TETRIS_GAME_ENABLE)), yes)
    OPT_DEFS += -DTETRIS_GAME_ENABLE
    SRC += tetris.c
endif
