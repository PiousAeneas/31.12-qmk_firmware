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
 *  Keymap Definitions
 */

#include QMK_KEYBOARD_H

// DEFINITIONS
// Define default Windows clipboard actions
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_PST C(KC_V)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Keymap Layers
enum layer_names {
    _RIGHT,
    _LEFT,
    _RCLIP,
    _LCLIP,
    _SCROLL,
};

// Boolean to toggle Mac Mode
bool isMac = false;

// TAP DANCE
// Tap dance declarations
enum {
    U_TD_MAC, // Toggle Mac Mode
};

// Tap dance helper functions
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) { // Toggle Mac Mode
    switch (state->count) {
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
    }
}

// Tap dance actions array
tap_dance_action_t tap_dance_actions[] = {
    [U_TD_MAC] = ACTION_TAP_DANCE_FN(u_td_mac_fn), // Mac Mode
};

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
        KC_BTN4,    KC_BTN5,        DRAG_SCROLL,    KC_BTN2,
        KC_BTN1,                                    DF(_LEFT)
    ),

    // Alternate left-handed base layer
    [_LEFT] = LAYOUT(
        KC_BTN2,    DRAG_SCROLL,    KC_BTN4,        KC_BTN5,    
        DF(_RIGHT),                                 KC_BTN1
    ),

    // Right hand clipboard layer
    [_RCLIP] = LAYOUT(
        U_CPY,      KC_BTN5,        U_UND,          U_RDO,
        U_PST,                                      TD(U_TD_MAC)
    ),

    // Left hand clipboard layer
    [_LCLIP] = LAYOUT(
        U_RDO,      U_UND,          KC_BTN4,        U_CPY,    
        TD(U_TD_MAC),                               U_PST
    ),

    // Scroll layer when drag-scroll is toggled on
    [_SCROLL] = LAYOUT(
        KC_PGUP,    DF(_LEFT),      DF(_RIGHT),     KC_PGUP,
        KC_PGDN,                                    KC_PGDN
    ),
};