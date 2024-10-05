// Adapted heavily from Manna Harbour's Miryoku layout.
// https://github.com/manna-harbour/miryoku

#pragma once

#define CAPS_WORD_INVERT_ON_SHIFT // Holding Shift continues Caps Word but inverts the shift state
#define PERMISSIVE_HOLD // Selects the hold action when another key is pressed and then released while the dual-role key is held down, even if within the tapping term
#define MK_COMBINED // Enables momentary keys to set mouse speed to different constants

// Optimizations to reduce firmware size: https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
#define NO_ACTION_MACRO     // Required for LTO. Disables deprecated action_get_macro method and M(id) keycodes.
#define NO_ACTION_FUNCTION  // Required for LTO. Disables deprecated action_function method and AF(id) keycodes.
#define DISABLE_LEADER

#define TAPPING_TERM 200 // Reiterates default value.
#define QUICK_TAP_TERM 0 // Disables auto-repeat after quick tap for faster home row mods.

// Auto Shift
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP

// Mouse key speed and acceleration.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY          0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL       16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    64
