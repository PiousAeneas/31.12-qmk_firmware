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
 *  Custom Keycode Handling
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
            break;
        case 2:
            isMac = false; // Turn off Mac Mode
            keymap_config.swap_lctl_lgui = false;
            keymap_config.swap_rctl_rgui = false;
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
};

// Custom keycode handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
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
        U_BRWSR_BCK,        U_BRWSR_FWD,    DRAG_SCROLL,    KC_BTN2,
        KC_BTN1,                                            DF(_LEFT)
    ),

    // Alternate left-handed base layer
    [_LEFT] = LAYOUT(
        KC_BTN2,            DRAG_SCROLL,    U_BRWSR_BCK,    U_BRWSR_FWD,    
        DF(_RIGHT),                                         KC_BTN1
    ),

    // Right hand clipboard layer - Copy, Cut, Undo, Redo, Paste, and Mac toggle
    [_RCLIP] = LAYOUT(
        TD(U_TD_CPYCUT),    KC_NO,          U_UND,          U_RDO,
        TD(U_TD_PST),                                       TD(U_TD_MAC)
    ),

    // Left hand clipboard layer - Mirror of right hand clipboard layer
    [_LCLIP] = LAYOUT(
        U_RDO,              U_UND,          KC_NO,          TD(U_TD_CPYCUT),    
        TD(U_TD_MAC),                                       TD(U_TD_PST)
    ),
    
    // Scroll layer - Activated when drag-scroll is toggled on
    [_SCROLL] = LAYOUT(
        KC_PGUP,            DF(_LEFT),      DF(_RIGHT),     KC_PGUP,
        KC_PGDN,                                            KC_PGDN
    ),
};