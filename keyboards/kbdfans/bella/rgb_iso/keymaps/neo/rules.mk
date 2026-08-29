# keyboards/kbdfans/bella/rgb_iso/keymaps/neo/rules.mk

RAW_ENABLE            = yes
DYNAMIC_KEYMAP_ENABLE = yes
VIA_ENABLE            = yes

# Arch Linux avr-gcc doesn't search avr-libc's paths by default;
# keyboard-level rules.mk in the userspace overlay is never loaded by QMK
# (only users/ and keymaps/ are overlaid), so this must live here.
CFLAGS  += -isystem/usr/lib/avr/include
ASFLAGS += -isystem/usr/lib/avr/include
LDFLAGS += -B/usr/lib/avr/lib

# QMK core bug: tap_dance_get_raw() returns non-const pointer into a const
# array (quantum/keymap_introspection.c) — newer GCC treats this as an error.
CFLAGS += -Wno-error=discarded-qualifiers
CFLAGS += -Wno-error=unused-but-set-variable

