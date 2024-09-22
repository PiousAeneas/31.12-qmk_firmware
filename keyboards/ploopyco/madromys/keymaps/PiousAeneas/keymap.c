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
 *  Definitions: Clipboard, Layers, Mac Mode
 *  Tap Dance: Declarations, Helper Functions, Definitions Array
 *  Custom Keycodes: Redo, Browser Nav, Combo Scroll
 *  Keymap Definitions
 */

#include QMK_KEYBOARD_H

// DEFINITIONS

// Define Keymap Layers
enum layer_names {
    _RIGHT,
    _LEFT,
    _RCLIP,
    _LCLIP,
    _SCROLL,
};

// Define default Windows clipboard actions
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Boolean to toggle Mac Mode
bool isMac = false;

// Combo Drag Scroll
extern bool is_drag_scroll; // External variable from ploopyco.c
extern int8_t comboscroll_invert; // Used to invert scroll directions for Mac
static uint16_t timer; // Timer variable for use witn COMBO_SCROLL

// TAP DANCE
// Tap dance declarations
enum {
    U_TD_MAC, // Toggle Mac Mode (default = Win)
    U_TD_CPYCUT, // 1:Copy, 2:Cut
    U_TD_PST, // 1:Paste, 2:Paste Special i.e. without formatting
};

// Tap dance helper functions
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) { // 1:Mac, 2:Win
    switch (state->count) { // Use state->count to determine tap count
        case 1:
            isMac = true; // Turn on Mac Mode
            keymap_config.swap_lctl_lgui = true; // Swap Left Control and GUI
            keymap_config.swap_rctl_rgui = true; // Swap Right Control and GUI
            comboscroll_invert = -1; // Invert scroll directions for Mac
            break;
        case 2:
            isMac = false; // Turn off Mac Mode
            keymap_config.swap_lctl_lgui = false;
            keymap_config.swap_rctl_rgui = false;
            comboscroll_invert = 1; // Return to default scroll directions
            break;
        default:
            break; // Do nothing for unexpected tap counts
    }
}

void u_td_cpycut_fn(tap_dance_state_t *state, void *user_data) { // 1:Copy, 2:Cut
    uint8_t modifier = isMac ? KC_LCMD : KC_LCTL; // Choose modifier based on Mac mode
    uint8_t keycode = (state->count == 1) ? KC_C : KC_X; // Choose keycode based on tap count

    register_code(modifier); // Press and hold modifier
    tap_code(keycode); // Press and release the relevant key (C for copy, X for cut)
    unregister_code(modifier); // Release modifier
}

void u_td_pst_sp_fn(tap_dance_state_t *state, void *user_data) { // 1:Paste, 2:Paste Special
    if (isMac) { 
        register_code(KC_LCMD); 
        if (state->count == 2) { // Paste Special requires additional modifiers
            register_code(KC_LSFT);
            register_code(KC_LALT);
        }
        tap_code(KC_V); // Press and release V key
        unregister_code(KC_LCMD);
        if (state->count == 2) {
            unregister_code(KC_LALT);
            unregister_code(KC_LSFT);
        }
    } else { // Windows shortcuts
        register_code(KC_LCTL);
        if (state->count == 2) { // Paste Special in Windows
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
    [U_TD_MAC]      = ACTION_TAP_DANCE_FN(u_td_mac_fn), // Mac Mode
    [U_TD_CPYCUT]   = ACTION_TAP_DANCE_FN(u_td_cpycut_fn), // Copy or Cut
    [U_TD_PST]      = ACTION_TAP_DANCE_FN(u_td_pst_sp_fn), // Paste Special
};

// CUSTOM KEYCODES
// Define custom keycodes
enum custom_keycodes {
    U_BRWSR_BCK = SAFE_RANGE, U_BRWSR_FWD, // Browser navigation
    COMBO_SCROLL, // Combo Drag Scroll
    RET_RGHT, RET_LEFT, // Return to Base layer from Scroll layer
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

    case LT(_LCLIP, KC_WBAK): // Custom Layer Tap for U_BRWSR_BCK (uses Mod-Tap Intercept)
        if (record->tap.count && record->event.pressed) {
            // This is a tap - execute U_BRWSR_BCK functionality
            if (isMac) {
                register_code(KC_LGUI);
                tap_code(KC_LBRC);
                unregister_code(KC_LGUI);
            } else {
                tap_code(KC_WBAK);
            }
            return false; // Skip all further processing of this key
        }
        return true; // If it's not a tap, let QMK handle the layer toggle

    case LT(_RCLIP, KC_WFWD): // Custom Layer Tap for U_BRWSR_FWD (uses Mod-Tap Intercept)
        if (record->tap.count && record->event.pressed) {
            // This is a tap - execute U_BRWSR_FWD functionality
            if (isMac) {
                register_code(KC_LGUI);
                tap_code(KC_RBRC);
                unregister_code(KC_LGUI);
            } else {
                tap_code(KC_WFWD);
            }
            return false; // Skip all further processing of this key
        }
        return true; // If it's not a tap, let QMK handle the layer toggle

    case COMBO_SCROLL: // Turns on drag scroll and Scroll layer on tap, momentary drag scroll on hold
        if (record->event.pressed) { // on key down
            timer = timer_read(); // read out the time of key down
            is_drag_scroll = 1; // turn on drag scroll
        } else { // on key release
            if (timer_elapsed(timer) < TAPPING_TERM) { // key was tapped
                layer_invert(_SCROLL); // toggle Scroll layer
            } else { // key was held
                is_drag_scroll = 0; // turn off drag scroll on key release
            }
        }
        return false; // Skip all further processing of this key

    case RET_RGHT:
    case RET_LEFT:
        if (record->event.pressed) { // on key down
            is_drag_scroll = 0; // turn off drag scroll
            layer_off(_SCROLL); // turn off Scroll layer
            default_layer_set(1UL << (keycode == RET_RGHT ? _RIGHT : _LEFT)); // set base layer
        }
        return false; // Skip all further processing of this key

    default:
        return true; // Process all other keycodes normally
  }
}


// KEYMAP DEFINITIONS
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
        KC_BTN1,                                                    DF(_LEFT)
    ),

    // Alternate left-handed base layer
    [_LEFT] = LAYOUT(
        KC_BTN2,            COMBO_SCROLL,       LT(_LCLIP,KC_WBAK), U_BRWSR_FWD,    
        DF(_RIGHT),                                                 KC_BTN1
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
        KC_PGUP,            RET_LEFT,           RET_RGHT,           KC_PGUP,
        KC_PGDN,                                                    KC_PGDN
    ),
};