/* Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* TABLE OF CONTENTS
 *  --- Definitions and Configuration
 *  --- Helper Functions
 *  --- Tap Dance Configuration
 *  --- Custom Keycodes and Logic
 *  --- Custom Combos
 *  --- Keymap Layer Definitions
 *  --- Hardware Setup
 */

#include QMK_KEYBOARD_H

// --- DEFINITIONS AND CONFIGURATION ---

// Define user_config_t as a union to store persistent user settings in EEPROM
typedef union {
    uint32_t raw;               // Raw 32-bit representation for EEPROM read/write
    struct {
        uint8_t scroll_div;     // Divisor for scroll speed vs. DPI
        uint8_t default_layer;  // Layer to switch to on reset
        bool mac_mode : 1;      // Mac mode toggle (1 bit)
    };
} user_config_t;

user_config_t user_config;      // Declare runtime instance used to read/write EEPROM config

// Configure persistent default hardcodes for Relative Scroll Speed and Mac Mode
void eeconfig_init_user(void) {
    user_config.scroll_div = PLOOPY_DRAGSCROLL_DIVISOR_H;  // Default scroll divisor from config.h
    user_config.mac_mode = false;                          // Default Mac mode (off = Win)
    eeconfig_update_user(user_config.raw);                 // Save to EECONFIG_USER
}

// Define Keymap Layers
enum layer_names {
    _RIGHT, // Default Layer
    _LEFT,
    _RCLIP,
    _LCLIP,
    _SCROLL,
};

// Configure persistent default layer hardcode
#define DEFAULT_LAYER _RIGHT                            // Hardcoded default layer
static uint8_t current_default_layer = DEFAULT_LAYER;   // Variable to track current default

// Define default Windows clipboard actions
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Define Combo Drag Scroll variables
extern bool is_drag_scroll;         // External variable from ploopyco.c
extern int8_t comboscroll_invert;   // Used to invert scroll directions for Mac
static uint16_t scroll_timer;       // Timer variable for COMBO_SCROLL

// Define timer variables for custom key combos
static uint16_t reset_timer;        // Timer for RESET combo
static uint16_t save_timer;         // Timer for SAVE combo

// Define variables for DPI adjustment
uint16_t custom_dpi_array[] = PLOOPY_DPI_OPTIONS;       // Get options from config.h
#define DPI_OPTION_SIZE ARRAY_SIZE(custom_dpi_array)    // Used in DPI config logic
static uint8_t current_dpi  = PLOOPY_DPI_DEFAULT;       // Variable to track DPI

// Define variables for Scroll Speed adjustment
extern uint16_t dragscroll_divisor_h;   // To track scroll speed adjustments
extern uint16_t dragscroll_divisor_v;   // To track  scroll speed adjustments

// Define boolean to track Mac Mode
bool isMac = false;

// --- HELPER FUNCTIONS ---

// Helper function for setting Mac Mode
void set_mac_mode(bool enable) {
    isMac = enable;
    keymap_config.swap_lctl_lgui = enable; // Swap Left Control and GUI
    keymap_config.swap_rctl_rgui = enable; // Swap Right Control and GUI
    comboscroll_invert = enable ? -1 : 1;  // Invert scroll directions
}

// Save and Reset helper functions
void quick_reset(void) { // Reset DPI, Scroll Speed, and Active Layer to last saved defaults
    current_dpi = keyboard_config.dpi_config;               // Reset to last saved DPI
    pointing_device_set_cpi(custom_dpi_array[current_dpi]); // Apply DPI

    dragscroll_divisor_h = user_config.scroll_div;          // Reset to last saved scroll speed
    dragscroll_divisor_v = user_config.scroll_div;          // Reset to last saved scroll speed
    
    is_drag_scroll = 0;                                     // Turn off drag scroll
    layer_clear();                                          // Clear all layers other than default
}

void save_settings(void) {                          // Save DPI, Scroll Speed, Mac Mode, and Layer
    keyboard_config.dpi_config = current_dpi;       // Save current DPI
    eeconfig_update_kb(keyboard_config.raw);        // Write DPI to EEPROM (handled by base ploopyco.c code)
    
    user_config.scroll_div = dragscroll_divisor_h;  // Save current scroll divisor
    user_config.mac_mode   = isMac;                 // Save current Mac Mode state
    eeconfig_update_user(user_config.raw);          // Write custom user settings to EEPROM
    
    set_single_persistent_default_layer(current_default_layer);  // Write current default layer to EEPROM
}

void hard_reset(void) {  // Quick Reset + Mac Mode and Handedness but from hardcoded defaults
    eeconfig_init_kb();  // Reset keyboard and user config (DPI, scroll speed, Mac mode)
    user_config.raw = eeconfig_read_user();     // Read user config from EEPROM into RAM
    keyboard_config.raw = eeconfig_read_kb();   // Read keyboard config from EEPROM into RAM

    current_default_layer = DEFAULT_LAYER;      // Reset tracked default layer to hardcoded value
    set_single_persistent_default_layer(current_default_layer);  // Write to persistent memory

    quick_reset();      // Apply DPI, scroll speed, and layer settings via quick reset

    set_mac_mode(user_config.mac_mode);         // Reset Mac Mode from reset user config
}

// --- TAP DANCE CONFIGURATION ---

// Tap dance declarations
enum {
    U_TD_MAC,       // Toggle Mac Mode (default = Win)
    U_TD_CPYCUT,    // 1:Copy, 2:Cut
    U_TD_PST,       // 1:Paste, 2:Paste Special i.e. without formatting
};

// Tap dance helper functions
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1 || state->count == 2) {
        set_mac_mode(state->count == 1); // 1 tap = Mac mode, 2 taps = Win mode
    }
}

void u_td_cpycut_fn(tap_dance_state_t *state, void *user_data) { // 1:Copy, 2:Cut
    uint8_t modifier = isMac ? KC_LCMD : KC_LCTL; // Choose modifier based on Mac mode
    uint8_t keycode = (state->count == 1) ? KC_C : KC_X; // Choose keycode based on tap count

    register_code(modifier);    // Press and hold modifier
    tap_code(keycode);          // Press and release the relevant key (C for copy, X for cut)
    unregister_code(modifier);  // Release modifier
}

void u_td_pst_sp_fn(tap_dance_state_t *state, void *user_data) { // 1:Paste, 2:Paste Special
    if (isMac) { 
        register_code(KC_LCMD); 
        if (state->count == 2) {    // Paste Special requires additional modifiers
            register_code(KC_LSFT);
            register_code(KC_LALT);
        }
        tap_code(KC_V);             // Press and release V key
        unregister_code(KC_LCMD);
        if (state->count == 2) {
            unregister_code(KC_LALT);
            unregister_code(KC_LSFT);
        }
    } else {                        // Windows shortcuts
        register_code(KC_LCTL);
        if (state->count == 2) {    // Paste Special in Windows
            register_code(KC_LSFT); 
        }
        tap_code(KC_V);
        if (state->count == 2) {
            unregister_code(KC_LSFT);
        }
        unregister_code(KC_LCTL);
    }
}

// Tap dance actions array
tap_dance_action_t tap_dance_actions[] = {
    [U_TD_MAC]      = ACTION_TAP_DANCE_FN(u_td_mac_fn),     // Mac Mode
    [U_TD_CPYCUT]   = ACTION_TAP_DANCE_FN(u_td_cpycut_fn),  // Copy or Cut
    [U_TD_PST]      = ACTION_TAP_DANCE_FN(u_td_pst_sp_fn),  // Paste Special
};

// --- CUSTOM KEYCODES AND LOGIC ---

// Define custom keycodes
enum custom_keycodes {
    U_BRWSR_BCK = SAFE_RANGE, U_BRWSR_FWD, // Browser navigation
    COMBO_SCROLL,               // Combo Drag Scroll
    RET_RGHT, RET_LEFT,         // Return to Base layer from Scroll layer
    DPI_UP, DPI_DOWN,           // Adjust DPI up or down
    SCROLL_UP, SCROLL_DOWN,     // Adjust Scroll speed relative to DPI
    ALT_PGUP, ALT_PGDN,         // For reset combos
};

// Custom keycode handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Perform redo action for Mac (Cmd+Shift+Z)
    case U_RDO:
        if (record->event.pressed) {
            if (isMac) {
                register_code(KC_LCMD);
                register_code(KC_LSFT);
                tap_code(KC_Z);
                unregister_code(KC_LSFT);
                unregister_code(KC_LCMD);
                return false;
            }
        }
        return true; // Else use default redo (Windows)
            
    // Browser navigation with U_BRWSR_BCK and U_BRWSR_FWD
    case U_BRWSR_BCK:
    case U_BRWSR_FWD:
        if (record->event.pressed) {
            if (isMac) {
                register_code(KC_LGUI); // Press and hold Left GUI
                tap_code(keycode == U_BRWSR_BCK ? KC_LBRC : KC_RBRC); // Tap [ for U_BRWSR_BCK or ] for U_BRWSR_FWD
                unregister_code(KC_LGUI); // Release Left GUI
            } else {
                tap_code(keycode == U_BRWSR_BCK ? KC_WBAK : KC_WFWD); // Tap Browser Back for U_BRWSR_BCK or Browser Forward for U_BRWSR_FWD
            }
        }
        return false; // Skip all further processing of this key

    // Custom Layer Taps for U_BRWSR_BCK and U_BRWSR_FWD using Mod-Tap Intercept
    case LT(_LCLIP, KC_WBAK):
    case LT(_RCLIP, KC_WFWD):
        if (record->tap.count && record->event.pressed) { // Execute browser nav on tap - copied from above
            if (isMac) {
                register_code(KC_LGUI);
                tap_code(keycode == LT(_LCLIP, KC_WBAK) ? KC_LBRC : KC_RBRC);
                unregister_code(KC_LGUI);
            } else {
                tap_code(keycode == LT(_LCLIP, KC_WBAK) ? KC_WBAK : KC_WFWD);
            }
            return false; // Skip all further processing of this key
        }
        return true; // If it's not a tap, let QMK handle the layer toggle

    // Custom Layer Taps for Chirality and Middle Click using Mod-Tap Intercept
    case LT(_RIGHT, KC_M):
    case LT(_LEFT, KC_M):
        if (record->tap.count && record->event.pressed) { // Set default layer to other hand on Tap
            current_default_layer = (keycode == LT(_RIGHT, KC_M)) ? _RIGHT : _LEFT; // toggle hands
            default_layer_set(1UL << current_default_layer); // set as default
            return false;
        } else { // Hold
            if (record->event.pressed) {
                register_code(KC_BTN3); // Press middle click on hold
            } else {
                unregister_code(KC_BTN3); // Release middle click on release
            }
        }
        return false; // Skip all further processing of this key

    case COMBO_SCROLL: // Turns on drag scroll and Scroll layer on tap, momentary drag scroll on hold
        if (record->event.pressed) { // on key down
            scroll_timer = timer_read(); // read out the time of key down
            is_drag_scroll = 1; // turn on drag scroll
        } else { // on key release
            if (timer_elapsed(scroll_timer) < TAPPING_TERM) { // key was tapped
                layer_on(_SCROLL); // turn on Scroll layer
            } else { // key was held
                is_drag_scroll = 0; // turn off drag scroll on key release
            }
        }
        return false; // Skip all further processing of this key

    // Custom keycodes for exiting Scroll layer
    case RET_RGHT:
    case RET_LEFT:
        if (record->event.pressed) { // On key down
            is_drag_scroll = 0;                          // Disable drag scroll mode
            layer_off(_SCROLL);                          // Turn off Scroll layer

            uint8_t target = (keycode == RET_RGHT ? _RIGHT : _LEFT);  // Determine target layer based on keycode

            if (current_default_layer != target) {       // If target layer differs from current default
                current_default_layer = target;          // Update the default layer tracking variable
                default_layer_set(1UL << current_default_layer);  // Set target as default layer
            }
        }
        return false; // Skip further processing of this key

    // Custom keycodes to adjust DPI and Scroll Speed
    case DPI_UP: // Increase DPI index, but cap at highest option
        if (record->event.pressed) {
            if (current_dpi < (DPI_OPTION_SIZE - 1)) {
                current_dpi++;
                pointing_device_set_cpi(custom_dpi_array[current_dpi]); // Apply new DPI
            }
        }
        return false; // Skip further processing

    case DPI_DOWN: // Decrease DPI index, but don't go below zero
        if (record->event.pressed) {
            if (current_dpi > 0) {
                current_dpi--;
                pointing_device_set_cpi(custom_dpi_array[current_dpi]); // Apply new DPI
            }
        }
        return false; // Skip further processing

    case SCROLL_UP: // Increase Scroll speed
        if (record->event.pressed) {
            if (dragscroll_divisor_h > DRAGSCROLL_DIV_MIN) dragscroll_divisor_h -= DRAGSCROLL_DIV_STEP;
            if (dragscroll_divisor_v > DRAGSCROLL_DIV_MIN) dragscroll_divisor_v -= DRAGSCROLL_DIV_STEP;
        }
        return false;

    case SCROLL_DOWN: // Decrease Scroll Speed
        if (record->event.pressed) {
            if (dragscroll_divisor_h < DRAGSCROLL_DIV_MAX) dragscroll_divisor_h += DRAGSCROLL_DIV_STEP;
            if (dragscroll_divisor_v < DRAGSCROLL_DIV_MAX) dragscroll_divisor_v += DRAGSCROLL_DIV_STEP;
        }
        return false;

    // Duplicate PGUP and PGDN keys for use with Scroll Layer combos
    case ALT_PGUP:
        if (record->event.pressed) tap_code(KC_PGUP);
        return false;
    case ALT_PGDN:
        if (record->event.pressed) tap_code(KC_PGDN);
        return false;

    default:
        return true; // Process all other keycodes normally
  }
}

// --- CUSTOM COMBOS ---

// Layer independent combos - All layers except _SCROLL use _RIGHT as the combo reference layer.
uint8_t combo_ref_from_layer(uint8_t layer){
    switch (get_highest_layer(layer_state)){
        case _SCROLL: return _SCROLL;
        default: return _RIGHT;
    }
}

enum combo_events {
    CB_DPI_UP,
    CB_DPI_DOWN,
    CB_SCROLL_UP,
    CB_SCROLL_DOWN,
    CB_RESET_MAIN,
    CB_RESET_SCROLL,
    CB_SAVE_MAIN,
    CB_SAVE_SCROLL,
};

// Combo definitions
const uint16_t PROGMEM combo_dpi_up[]       = { U_BRWSR_BCK,  KC_BTN2,          COMBO_END };
const uint16_t PROGMEM combo_dpi_down[]     = { KC_BTN1,      LT(_LEFT, KC_M),  COMBO_END };
const uint16_t PROGMEM combo_scroll_up[]    = { KC_PGUP,      ALT_PGUP,         COMBO_END };
const uint16_t PROGMEM combo_scroll_down[]  = { KC_PGDN,      ALT_PGDN,         COMBO_END };
const uint16_t PROGMEM combo_reset_main[]   = { U_BRWSR_BCK,  KC_BTN2,  KC_BTN1, LT(_LEFT, KC_M), COMBO_END };
const uint16_t PROGMEM combo_reset_scroll[] = { KC_PGUP,      ALT_PGUP, KC_PGDN, ALT_PGDN,        COMBO_END };
const uint16_t PROGMEM combo_save_main[]    = { LT(_RCLIP, KC_WFWD),    COMBO_SCROLL,   COMBO_END };
const uint16_t PROGMEM combo_save_scroll[]  = { RET_LEFT,               RET_RGHT,       COMBO_END };

// Register the combos
combo_t key_combos[] = {
    [CB_DPI_UP]         = COMBO(combo_dpi_up,       DPI_UP),
    [CB_DPI_DOWN]       = COMBO(combo_dpi_down,     DPI_DOWN),
    [CB_SCROLL_UP]      = COMBO(combo_scroll_up,    SCROLL_UP),
    [CB_SCROLL_DOWN]    = COMBO(combo_scroll_down,  SCROLL_DOWN),
    [CB_RESET_MAIN]     = COMBO_ACTION(combo_reset_main),
    [CB_RESET_SCROLL]   = COMBO_ACTION(combo_reset_scroll), // Scroll layer treated independently
    [CB_SAVE_MAIN]      = COMBO_ACTION(combo_save_main),
    [CB_SAVE_SCROLL]    = COMBO_ACTION(combo_save_scroll),
};

// Define and handle COMBO_ACTION events
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {

        case CB_RESET_MAIN:
        case CB_RESET_SCROLL:
            if (pressed) {
                reset_timer = timer_read(); // Start timer on combo press
            } else {
                if (timer_elapsed(reset_timer) < TAPPING_TERM) {
                    quick_reset();  // Tap → Quick Reset
                } else {
                    hard_reset();   // Hold → Hard Reset
                }
            }
            break;

        case CB_SAVE_MAIN:
        case CB_SAVE_SCROLL:
            if (pressed) {
                save_timer = timer_read(); // Start timer on combo press
            } else {
                if (timer_elapsed(save_timer) >= TAPPING_TERM) {
                    save_settings();  // Hold → Save Settings
                }
                // If released too quickly, do nothing (intentionally ignore tap)
            }
            break;
    }
}

// --- KEYMAP LAYER DEFINITIONS ---
/* Ploopy Adept Custom Layout = LAYOUT(#1, #2, #3, #4, #5, #6)
 * +---------+-------+-------+---------+
 * |         |       |       |         |
 * |    #1   |   #2  |   #3  |    #4   |
 * |         |       |       |         |
 * +---------+-------+-------+---------+
 * |                                   |
 * |                                   |
 * +-------------+       +-------------+
 * |             |       |             |
 * |      #5     |       |      #6     |
 * |             |       |             |
 * +-------------+-------+-------------+
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Default right-handed base layer
    [_RIGHT] = LAYOUT(
        U_BRWSR_BCK,        LT(_RCLIP,KC_WFWD), COMBO_SCROLL,       KC_BTN2,
        KC_BTN1,                                                    LT(_LEFT, KC_M)
    ),

    // Alternate left-handed base layer
    [_LEFT] = LAYOUT(
        KC_BTN2,            COMBO_SCROLL,       LT(_LCLIP,KC_WBAK), U_BRWSR_FWD,    
        LT(_RIGHT, KC_M),                                           KC_BTN1
    ),

    // Right hand clipboard layer - Copy, Cut, Undo, Redo, Paste, and Mac toggle
    [_RCLIP] = LAYOUT(
        TD(U_TD_CPYCUT),    KC_NO,              U_UND,              U_RDO,
        TD(U_TD_PST),                                               TD(U_TD_MAC)
    ),

    // Left hand clipboard layer - Mirror of right hand clipboard layer
    [_LCLIP] = LAYOUT(
        U_RDO,              U_UND,              KC_NO,              TD(U_TD_CPYCUT),    
        TD(U_TD_MAC),                                               TD(U_TD_PST)
    ),
    
    // Scroll layer - Activated when drag-scroll is toggled on
    [_SCROLL] = LAYOUT(
        KC_PGUP,            RET_LEFT,           RET_RGHT,           ALT_PGUP,
        KC_PGDN,                                                    ALT_PGDN
    ),
};

// --- HARDWARE SETUP ---
// Restore user settings from EEPROM after QMK and keyboard init
void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();             // Load saved user config from EEPROM
    set_mac_mode(user_config.mac_mode);                 // Apply saved Mac/Win mode setting
    dragscroll_divisor_h = user_config.scroll_div;      // Restore horizontal scroll speed
    dragscroll_divisor_v = user_config.scroll_div;      // Mirror to vertical scroll
}