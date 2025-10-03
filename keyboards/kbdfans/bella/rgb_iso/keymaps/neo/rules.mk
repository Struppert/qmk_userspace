LTO_ENABLE = yes
QMK_SETTINGS = no
RGB_MATRIX_ENABLE = no
RGBLIGHT_ENABLE   = no   # (bei Bella rgb_iso ist meist Matrix statt RGBlight)
CONSOLE_ENABLE    = no
COMMAND_ENABLE    = no
BOOTMAGIC_ENABLE  = lite
MOUSEKEY_ENABLE   = no
NKRO_ENABLE       = no
VIA_ENABLE        = yes

EXTRAFLAGS += -Wno-error=discarded-qualifiers

override KEYMAP_INTROSPECTION_ENABLE = no
SRC_EXCLUDE += quantum/keymap_introspection.c
