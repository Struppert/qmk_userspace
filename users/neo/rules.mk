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
