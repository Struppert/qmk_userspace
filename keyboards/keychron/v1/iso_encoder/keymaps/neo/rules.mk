# VIA & Features
VIA_ENABLE        = yes
ENCODER_ENABLE    = yes
RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE   = no

# quantum/rgb_matrix/animations/pixel_rain_anim.h (Core-Code, nicht
# unseres) hat eine ungenutzte Variable (region_mask) - mit dem hier
# installierten, sehr neuen arm-none-eabi-gcc wird das als
# -Werror=unused-but-set-variable ein harter Fehler statt nur Warnung.
# Gleiches Muster wie beim lkbt51.c-Fix für v1_max.
CFLAGS += -Wno-error=unused-but-set-variable

# Tetris-Prototyp auf eigenem Layer (_TETRIS, Zugang via TG(_TETRIS) auf
# _SYS), portiert von v1_max - siehe keyboards/keychron/v1_max/TETRIS.md.
# Standardmäßig aus, damit normale Builds unangetastet bleiben.
TETRIS_GAME_ENABLE ?= no
ifeq ($(strip $(TETRIS_GAME_ENABLE)), yes)
    OPT_DEFS += -DTETRIS_GAME_ENABLE
    SRC += tetris.c
endif
