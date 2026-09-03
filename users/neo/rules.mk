# users/neo/rules.mk — global

# VIA / Unicode / QoL
# Bella (ATmega32u4, 28KB flash / 1KB EEPROM) and kbd8x_mk3 (STM32F103CBT6,
# 128KB flash / 20KB RAM - confirmed against the real YDKB firmware source,
# NOT the 512KB/64KB high-density chip assumed earlier) are both genuinely
# memory-constrained, unlike the Keychron boards below.
ifeq ($(filter kbdfans/kbd8x_mk3 kbdfans/bella/%,$(KEYBOARD)),)
  VIA_ENABLE      = yes
  UNICODE_ENABLE  = yes
  NKRO_ENABLE     = yes
else
  VIA_ENABLE      = no
  UNICODE_ENABLE  = no
  NKRO_ENABLE     = no
endif
EXTRAKEY_ENABLE = yes

# German send_string LUTs (ascii_to_keycode/shift/altgr/dead) - needed by
# send_string()/SEND_STRING() calls AND by VIA's dynamic-macro playback on
# every board, not just the ones that happen to compile leader/os_shell.c.
SRC += sendstring_de.c

# Optimierungen
ifeq ($(filter kbdfans/kbd8x_mk3 kbdfans/bella/%,$(KEYBOARD)),)
  LTO_ENABLE      = yes
else
  LTO_ENABLE      = no
endif
CONSOLE_ENABLE  = no
COMMAND_ENABLE  = no

# === Feature Flags — set only in conditional below ===

# RGB Matrix — conditional per keyboard
# Keychron keyboards: full features
# Bella, kbd8x_mk3: minimal features (memory-constrained)
ifeq ($(filter kbdfans/kbd8x_mk3 kbdfans/bella/%,$(KEYBOARD)),)
  RGB_MATRIX_ENABLE = yes
  LEADER_ENABLE = yes
  # leader.c (the real leader_start_user/leader_end_user glue) is only
  # compiled for these boards - LEADER_WIRED tells tap_dance_impl.c that
  # calling leader_start() on a double-tap actually leads somewhere.
  OPT_DEFS += -DLEADER_WIRED
  DYNAMIC_MACRO_ENABLE = yes
  TAP_DANCE_ENABLE = yes
  COMBO_ENABLE = yes
  KEYMAP_INTROSPECTION_ENABLE = yes
else
  RGB_MATRIX_ENABLE = no
  # LEADER_ENABLE stays on (QK_LEAD keycode + process_leader.c must still
  # compile). By default leader.c isn't in SRC below, so leader_start_user/
  # leader_end_user fall back to QMK's weak no-op stubs - a double-tap
  # would silently enter a dead leader capture that times out doing
  # nothing. LEADER_WIRED (left undefined here) tells tap_dance_impl.c to
  # skip that branch and just fall through to normal tap/hold behavior.
  # kbd8x_mk3 overrides this below with a real (but reduced) leader.
  LEADER_ENABLE = yes
  DYNAMIC_MACRO_ENABLE = no
  # TAP_DANCE and COMBO must stay enabled for stub arrays!
  TAP_DANCE_ENABLE = yes
  COMBO_ENABLE = no
  KEYMAP_INTROSPECTION_ENABLE = no
  VIA_ENABLE = yes
endif

# kbd8x_mk3 only (NOT bella - its ATmega32u4 has no room for any of this):
# close the remaining gap to full Keychron-board parity. These were all
# cut for the same now-corrected ~3K RAM estimate as the leader table
# (see README.md RAM-Budget). combos_bindings.inc is already #included
# from keymap.c (the F+J->Leader combo was compiled as a dead stub
# without COMBO_ENABLE); keymap_logic.c (tap/hold tuning for
# L3_ESC_NEO/SYS_ESC/SP_FN etc.) was never compiled in at all - those
# keys have been running on QMK's default weak-stub tap/hold behavior,
# not the tuned one Q3/V3 get.
ifeq ($(KEYBOARD),kbdfans/kbd8x_mk3)
  UNICODE_ENABLE = yes
  NKRO_ENABLE = yes
  # DYNAMIC_MACRO_ENABLE left off: its macro_buffer[] costs 2560B RAM
  # (DYNAMIC_MACRO_SIZE=256) for a volatile (non-EEPROM) DM_REC1/DM_PLY1
  # macro system that overlaps VIA's own dynamic keymap macros (12 slots,
  # EEPROM-persistent via wear_leveling, already active) - not worth the
  # budget on this board. See README.md RAM-Budget for the measured
  # tradeoff (6% vs 35% free).
  COMBO_ENABLE = yes
  KEYMAP_INTROSPECTION_ENABLE = yes
  SRC += keymap_logic.c
endif

# kbd8x_mk3 only (NOT bella - its ATmega32u4 only has 2.5K RAM total, no
# room for this): full leader table (see README.md RAM-Budget section -
# the ~3K headroom this used to be sized against was wrong, it double-
# counted the ChibiOS linker script's always-fills-to-RAM-end .heap
# section, which nothing here ever calls malloc() into; real headroom is
# ~9.1K). Full leader_table[] (138 entries, all groups) needs exactly
# 160 nodes/159 edges = 1916B (measured via a trie-build simulation over
# the real LENTRY sequences, not guessed); sized to 224/224 = 2688B for
# growth margin.
ifeq ($(KEYBOARD),kbdfans/kbd8x_mk3)
  OPT_DEFS += -DLEADER_WIRED
  OPT_DEFS += -DLEADER_TRIE_MAX_NODES=224 -DLEADER_TRIE_MAX_EDGES=224
endif

# Large source files (leader, intents) only for non-memory-constrained keyboards
ifeq ($(filter kbdfans/kbd8x_mk3 kbdfans/bella/%,$(KEYBOARD)),)
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
  # Bella, kbd8x_mk3 (memory constrained): minimal leader source files
  SRC += userspace-init.c \
         os_state.c \
         leader/core.c \
         leader/leader.c \
         leader/trie.c \
         tap_dance_impl.c
  ifeq ($(KEYBOARD),kbdfans/kbd8x_mk3)
    # Full leader wiring (see README.md RAM-Budget + LEADER_TRIE_MAX_NODES/
    # EDGES above for why this now fits) - leader/table.c, not table_min.c.
    # Never add both - they both define leader_table[]/leader_table_count().
    SRC += leader.c \
           leader/os_shell.c \
           leader/help.c \
           leader/table.c \
           leader/intents_wezterm.c \
           leader/intents_dev.c \
           leader/intents_rg_fzf.c \
           leader/intents_fs.c \
           leader/intents_zellij.c \
           leader/intents_sed.c \
           leader/handlers_os_shell.c \
           leader/handlers_wezterm.c \
           leader/handlers_git.c \
           leader/handlers_rg.c \
           leader/handlers_fzf.c \
           leader/handlers_fs.c \
           leader/handlers_zellij.c \
           leader/handlers_ctx.c \
           leader/handlers_sed.c
  endif
endif
