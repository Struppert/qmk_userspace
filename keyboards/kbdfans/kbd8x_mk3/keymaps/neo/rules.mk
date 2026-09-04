# Tetris-Prototyp auf eigenem Layer (_TETRIS, Zugang via TG(_TETRIS) auf
# _SYS), portiert von v1_max - siehe keyboards/keychron/v1_max/TETRIS.md.
# Standardmäßig aus, damit normale Builds unangetastet bleiben.
TETRIS_GAME_ENABLE ?= no
ifeq ($(strip $(TETRIS_GAME_ENABLE)), yes)
    OPT_DEFS += -DTETRIS_GAME_ENABLE
    SRC += tetris.c
endif
