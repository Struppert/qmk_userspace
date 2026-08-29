# users/neo/rules.mk — global

# VIA / Unicode / QoL
# Only bella (ATmega32u4, 28KB flash / 1KB EEPROM) is actually memory-
# constrained. kbd8x_mk3 is STM32F103 with 512KB flash and belongs in the
# full-featured branch below; its own rules.mk only needs to override
# RGB_MATRIX_ENABLE (no RGB matrix hardware, just WS2812 underglow).
ifeq ($(filter kbdfans/bella/%,$(KEYBOARD)),)
  VIA_ENABLE      = yes
  UNICODE_ENABLE  = yes
  NKRO_ENABLE     = yes
else
  VIA_ENABLE      = no
  UNICODE_ENABLE  = no
  NKRO_ENABLE     = no
endif
EXTRAKEY_ENABLE = yes

# Optimierungen
ifeq ($(filter kbdfans/bella/%,$(KEYBOARD)),)
  LTO_ENABLE      = yes
else
  LTO_ENABLE      = no
endif
CONSOLE_ENABLE  = no
COMMAND_ENABLE  = no

# === Feature Flags — set only in conditional below ===

# RGB Matrix — conditional per keyboard
# Keychron keyboards + kbd8x_mk3: full features (kbd8x_mk3 has no RGB
# matrix hardware though, so RGB_MATRIX_ENABLE is forced off just for it)
# Bella: minimal features (AVR memory-constrained)
ifeq ($(filter kbdfans/bella/%,$(KEYBOARD)),)
  ifeq ($(filter kbdfans/kbd8x_mk3,$(KEYBOARD)),)
    RGB_MATRIX_ENABLE = yes
  else
    RGB_MATRIX_ENABLE = no
  endif
  LEADER_ENABLE = yes
  DYNAMIC_MACRO_ENABLE = yes
  TAP_DANCE_ENABLE = yes
  COMBO_ENABLE = yes
  KEYMAP_INTROSPECTION_ENABLE = yes
else
  RGB_MATRIX_ENABLE = no
  LEADER_ENABLE = yes
  DYNAMIC_MACRO_ENABLE = no
  # TAP_DANCE and COMBO must stay enabled for stub arrays!
  TAP_DANCE_ENABLE = yes
  COMBO_ENABLE = no
  KEYMAP_INTROSPECTION_ENABLE = no
  VIA_ENABLE = yes
endif

# Large source files (leader, intents) only for non-memory-constrained keyboards
ifeq ($(filter kbdfans/bella/%,$(KEYBOARD)),)
  SRC += leader/core.c \
			 leader/os_shell.c \
			 leader/help.c \
			 leader/table.c \
			 leader/leader.c \
			 leader/trie.c \
			 leader/intents_wezterm.c \
			 leader/intents_dev.c \
			 leader/intents_rg_fzf.c \
			 leader/intents_fs.c \
			 leader/intents_zellij.c \
			 leader/handlers_os_shell.c \
			 leader/handlers_wezterm.c \
			 leader/handlers_git.c \
			 leader/handlers_rg.c \
			 leader/handlers_fzf.c \
			 leader/handlers_fs.c \
			 leader/handlers_zellij.c \
			 leader/handlers_ctx.c \
			 leader/intents_sed.c \
			 leader/handlers_sed.c \
			 leader.c \
			 leader/leader.c \
			 os_state.c \
			 keymap_logic.c \
			 userspace-init.c \
			 tap_dance_impl.c
else
  # Bella (AVR, memory constrained): minimal leader source files
  SRC += userspace-init.c \
         os_state.c \
         leader/core.c \
         leader/leader.c \
         leader/trie.c \
         tap_dance_impl.c
endif
