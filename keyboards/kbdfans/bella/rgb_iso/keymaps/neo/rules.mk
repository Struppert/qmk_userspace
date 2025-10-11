# keyboards/kbdfans/bella/rgb_iso/keymaps/neo/rules.mk

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
#SRC_EXCLUDE += quantum/keymap_introspection.c

override RGB_MATRIX_ENABLE = no
# Falls das Keyboard es trotzdem reinzieht: Quelle explizit ausschließen
SRC_EXCLUDE += quantum/process_keycode/process_rgb_matrix.c \
							 quantum/rgb_matrix/rgb_matrix.c \
							 quantum/rgb_matrix/rgb_matrix_drivers.c \
							 quantum/keymap_introspection.c \
							 drivers/led/issi/is31fl3741.c

# Erzwinge die VIA-Budgets per Compiler-Define (ueberschreibt alles andere)
EXTRAFLAGS += \
							-DDYNAMIC_KEYMAP_LAYER_COUNT=0 \
							-DDYNAMIC_KEYMAP_MACRO_COUNT=0 \
							-DDYNAMIC_KEYMAP_MACRO_MAX_LENGTH=16 \
							-DVIA_EEPROM_LAYOUT_OPTIONS_SIZE=0 \
							-DKEYMAP_INTROSPECTION_ENABLE=0
