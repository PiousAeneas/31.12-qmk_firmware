# Adapted heavily from Manna Harbour's Miryoku layout.
# https://github.com/manna-harbour/miryoku

MOUSEKEY_ENABLE 	= yes
EXTRAKEY_ENABLE 	= yes # Audio control and System control
AUTO_SHIFT_ENABLE	= yes
TAP_DANCE_ENABLE 	= yes
CAPS_WORD_ENABLE 	= yes

# Optimizations to reduce firmware size: https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
LTO_ENABLE       	= yes   # Enables Link Time Optimization
CONSOLE_ENABLE   	= no    # Disables Console for debug
COMMAND_ENABLE   	= no    # Disables Commands for debug and configuration