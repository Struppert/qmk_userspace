# users/neo/rules.mk — global

# VIA / Unicode / QoL
VIA_ENABLE      = yes
UNICODE_ENABLE  = yes
NKRO_ENABLE     = yes
EXTRAKEY_ENABLE = yes

# Optimierungen
LTO_ENABLE      = yes
CONSOLE_ENABLE  = no
COMMAND_ENABLE  = no

COMBO_ENABLE     = yes
TAP_DANCE_ENABLE = yes
LEADER_ENABLE    = yes
DYNAMIC_MACRO_ENABLE = yes

RGB_MATRIX_ENABLE = yes

SRC += leader.c
SRC += os_state.c
SRC += keymap_logic.c
SRC += userspace-init.c
SRC += tap_dance_impl.c
