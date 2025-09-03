# rules.mk für Keychron Q4 ISO — NeoQwertz Keymap

# VIA-Unterstützung (für dynamische Keymaps & VIA-Macros)
VIA_ENABLE = yes

# Unicode (für UC(0x....) in deiner Keymap, z. B. €)
UNICODE_ENABLE = yes

# Link-Time-Optimization: kleinere Firmware / leicht bessere Performance
LTO_ENABLE = yes

# RGB: Q-Serie nutzt i. d. R. RGB MATRIX (nicht RGBLIGHT)
RGB_MATRIX_ENABLE = yes
# Achtung: Nicht gleichzeitig RGBLIGHT_ENABLE aktivieren!

# Qualitäts-of-Life / Medienkeys
NKRO_ENABLE = yes         # N-Key-Rollover verfügbar
EXTRAKEY_ENABLE = yes     # Medien-/System-Tasten (Play, Vol, etc.)

# Optional: wenn du viele Combos/Tap-Dances willst (hattest du bereits)
COMBO_ENABLE =no
TAP_DANCE_ENABLE =no

# Alternative zu VIA-Macros (nur aktivieren, wenn du sie bewusst nutzen willst)
# DYNAMIC_MACRO_ENABLE = yes

# Speicher sparen / weniger Debug (optional, empfehlenswert)
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
