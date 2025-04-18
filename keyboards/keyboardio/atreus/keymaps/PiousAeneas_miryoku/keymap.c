// Adapted heavily from Manna Harbour's Miryoku layout.
// https://github.com/manna-harbour/miryoku

#include QMK_KEYBOARD_H

// ***DEFINITIONS***

// Define user_config_t as a union to store persistent user settings in EEPROM
typedef union {
    uint32_t raw;               // Raw 32-bit representation for EEPROM read/write
    struct {
        bool mac_mode : 1;      // Mac mode toggle (1 bit)
    };
} user_config_t;

user_config_t user_config;      // Declare runtime instance used to read/write EEPROM config

// Set persistent default hardcodes
void eeconfig_init_user(void) {
    user_config.mac_mode = false;                          // Default Mac mode (off = Win)
    eeconfig_update_user(user_config.raw);                 // Save to EECONFIG_USER
}

// Define Keymap Layers
enum miryoku_layers {
    U_BASE,
    U_EXTRA,
    U_TAP,
    U_BUTTON,
    U_NAV,
    U_MOUSE,
    U_SYS,
    U_NUM,
    U_SYM,
    U_FUN,
    U_NUMPAD
};

// Define default Windows clipboard actions. U_PST defined in Tap Dance section.
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Configure persistent default layer hardcode
#define DEFAULT_LAYER U_BASE                            // Hardcoded default layer
static uint8_t current_default_layer = DEFAULT_LAYER;   // Variable to track current default

// Define helper variables
static uint16_t reset_timer;                            // Define timer for RESET_KEYBOARD combo
bool isMac = false;                                     // Define boolean to track Mac Mode

// Helper function for setting Mac Mode
void set_mac_mode(bool enable) {
    isMac = enable;
    keymap_config.swap_lctl_lgui = enable; // Swap Left Control and GUI
    keymap_config.swap_rctl_rgui = enable; // Swap Right Control and GUI
}

// ***TAP DANCE***
// Tap dance declarations
enum {
    U_TD_EXTRA,     // For additional base layers
    U_TD_LLCK,      // Layer Locks
    U_TD_MAC,       // Mac Mode
    U_TD_PST,       // Paste Special
    U_TD_PSCR,      // Screenshot
};

// Tap dance helper functions

// Tap dance helper functions: Layer Locks
static uint8_t get_inverse_layer(uint8_t layer) {
    switch (layer) {
        case U_NAV:   return U_NUM;     // Navigation ↔ Numbers
        case U_NUM:   return U_NAV;     // Numbers ↔ Navigation
        case U_MOUSE: return U_SYM;     // Mouse ↔ Symbols
        case U_SYM:   return U_MOUSE;   // Symbols ↔ Mouse
        case U_SYS:   return U_FUN;     // System ↔ Function
        case U_FUN:   return U_SYS;     // Function ↔ System
        default:      return UINT8_MAX; // No defined inverse (e.g., BASE layer)
    }
}

void u_td_fn_llck(tap_dance_state_t *state, void *user_data) {
    uint8_t current_layer = get_highest_layer(layer_state);  // Get currently active layer

    if (state->count == 1) {
        layer_lock_invert(current_layer);  // Toggle lock on current layer (lock/unlock)
    } else if (state->count == 2) {
        uint8_t inverse_layer = get_inverse_layer(current_layer);  // Find the paired layer

        if (inverse_layer != UINT8_MAX) {  // Check if a valid inverse exists
            layer_lock_on(inverse_layer);  // Lock and turn on invers layer
        }
    }
}

// Tap dance helper functions: Change Default Base Layer
void u_td_fn_extra(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        current_default_layer = U_TAP;
        set_single_default_layer(current_default_layer);            // Tap layer on single tap
    } else if (state->count == 2) {
        current_default_layer = U_EXTRA;
        set_single_default_layer(current_default_layer);            // Extra layer on double tap
    } else if (state->count == 3) {
        set_single_persistent_default_layer(current_default_layer); // Save layer to persistent memory
    }
}

// Tap dance helper functions: Mac Mode
// 1 tap = Mac mode, 2 taps = Win mode, 3 taps = save to EEPROM
void u_td_fn_mac(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            set_mac_mode(true);  // Mac mode
            break;
        case 2:
            set_mac_mode(false); // Win mode
            break;
        case 3:
            user_config.mac_mode = isMac;                // Save current state
            eeconfig_update_user(user_config.raw);       // Write to EEPROM
            break;
    }
}

// Tap dance helper functions: Paste Special tap dance action
void u_td_fn_pst(tap_dance_state_t *state, void *user_data) { // 1:Paste, 2:Paste Special
    if (isMac) { 
        register_code(KC_LCMD); 
        if (state->count == 2) { // Paste Special requires additional modifiers.
            register_code(KC_LSFT);
            register_code(KC_LALT);
        }
        tap_code(KC_V); // Press and release V key.
        unregister_code(KC_LCMD);
        if (state->count == 2) {
            unregister_code(KC_LALT);
            unregister_code(KC_LSFT);
        }
    } else { // Windows shortcuts
        register_code(KC_LCTL);
        if (state->count == 2) { // Paste Special in Windows.
            register_code(KC_LSFT); 
        }
        tap_code(KC_V);
        if (state->count == 2) {
            unregister_code(KC_LSFT);
        }
        unregister_code(KC_LCTL);
    }
}

// Tap dance helper functions: Screenshot
void u_td_fn_pscr(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (isMac) {
                register_code(KC_LSFT);
                register_code(KC_LCMD);
                tap_code(KC_4);
                unregister_code(KC_LCMD);
                unregister_code(KC_LSFT);
            } else {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                tap_code(KC_S);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
            }
            break;
        case 2:
            if (isMac) {
                register_code(KC_LSFT);
                register_code(KC_LCMD);
                tap_code(KC_3);
                unregister_code(KC_LCMD);
                unregister_code(KC_LSFT);
            } else {
                tap_code(KC_PSCR);
            }
            break;
    }
}

// TAP DANCE ACTIONS ARRAY
tap_dance_action_t tap_dance_actions[] = {
    [U_TD_EXTRA]    = ACTION_TAP_DANCE_FN(u_td_fn_extra),   // Extra Alphas
    [U_TD_LLCK]     = ACTION_TAP_DANCE_FN(u_td_fn_llck),    // Layer Locks
    [U_TD_MAC]      = ACTION_TAP_DANCE_FN(u_td_fn_mac),     // Mac Mode
    [U_TD_PST]      = ACTION_TAP_DANCE_FN(u_td_fn_pst),     // Paste Special
    [U_TD_PSCR]     = ACTION_TAP_DANCE_FN(u_td_fn_pscr),    // Screenshot
};

// Define U_PST as paste special tap dance to work across all keymaps.
#define U_PST TD(U_TD_PST)


// ***CUSTOM KEYCODES***
// Custom keycode declarations
enum custom_keycodes {
    U_TABB = SAFE_RANGE, U_TABF,    // Tab navigation
    U_BRWSR_BCK, U_BRWSR_FWD,       // Browser navigation
    U_WHLL, U_WHLD, U_WHLU, U_WHLR, // Mouse scrolling
    U_SEARCH,                       // "Spotlight" search
    U_MDASH,                        // Em Dash
    U_XWIN, U_XFRZ,                 // Excel Shortcuts: New Window, Freeze Panes
    U_XIND, U_XOUT,                 // Excel Shortcuts: Indent, Outdent
    U_XDECINC, U_XDECDEC,           // Excel Shortcuts: Increase / Decrease Decimal
};

// Custom keycode handling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
            
        // Perform redo action for Mac (Cmd+Shift+Z).
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
            return true; // Else use default redo (Windows).
        
        // Tab navigation with U_TABB and U_TABF
        case U_TABB:
        case U_TABF:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                if (keycode == U_TABB) {
                    register_code(KC_LSFT); // If U_TABB, press and hold Left Shift.
                }
                tap_code(KC_TAB); // Tap Tab.
                if (keycode == U_TABB) {
                    unregister_code(KC_LSFT);
                }
                unregister_code(KC_LCTL);
            }
            return false; // Skip all further processing of this key.

        // Browser navigation with U_BRWSR_BCK and U_BRWSR_FWD
        case U_BRWSR_BCK:
        case U_BRWSR_FWD:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LGUI); // Press and hold Left GUI.
                    tap_code(keycode == U_BRWSR_BCK ? KC_LBRC : KC_RBRC); // Tap [ for U_BRWSR_BCK or ] for U_BRWSR_FWD.
                    unregister_code(KC_LGUI); // Release Left GUI
                } else {
                    tap_code(keycode == U_BRWSR_BCK ? KC_WBAK : KC_WFWD); // Tap Browser Back for U_BRWSR_BCK or Browser Forward for U_BRWSR_FWD.
                }
            }
            return false;

        // Reverse mouse scrolling directions when in Mac mode
        case U_WHLL:
            if (record->event.pressed) {
                register_code(isMac ? MS_WHLR : MS_WHLL);
            } else {
                unregister_code(isMac ? MS_WHLR : MS_WHLL);
            }
            return false;
        case U_WHLD:
            if (record->event.pressed) {
                register_code(isMac ? MS_WHLU : MS_WHLD);
            } else {
                unregister_code(isMac ? MS_WHLU : MS_WHLD);
            }
            return false;
        case U_WHLU:
            if (record->event.pressed) {
                register_code(isMac ? MS_WHLD : MS_WHLU);
            } else {
                unregister_code(isMac ? MS_WHLD : MS_WHLU);
            }
            return false;
        case U_WHLR:
            if (record->event.pressed) {
                register_code(isMac ? MS_WHLL : MS_WHLR);
            } else {
                unregister_code(isMac ? MS_WHLL : MS_WHLR);
            }
            return false;

        // Search with U_SEARCH
        case U_SEARCH:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LCMD);     // Press and hold Left Command.
                    tap_code(KC_SPC);           // Tap Space.
                    unregister_code(KC_LCMD);   // Release Left Command.
                } else {
                    register_code(KC_LGUI);     // Press and hold Left GUI (Windows key).
                    tap_code(KC_S);             // Tap S.
                    unregister_code(KC_LGUI);   // Release Left GUI.
                }
            }
            return false;

        // Em Dash
        case U_MDASH:
            if (record->event.pressed) {
                if (isMac) {                    // Mac em dash
                    register_code(KC_LALT);
                    register_code(KC_LSFT);
                    tap_code(KC_MINS);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LALT);
                } else {                        // Win em dash
                    register_code(KC_LALT);
                    tap_code(KC_KP_0);
                    tap_code(KC_KP_1);
                    tap_code(KC_KP_5);
                    tap_code(KC_KP_1);
                    unregister_code(KC_LALT);
                }
            }
            return false;

        // Excel Shortcuts
        case U_XWIN:
        case U_XFRZ:
        case U_XIND:
        case U_XOUT:
        case U_XDECINC:
        case U_XDECDEC:
            if (record->event.pressed) {
                tap_code(KC_LALT);
                switch (keycode) {
                    case U_XWIN:
                        tap_code(KC_W);
                        tap_code(KC_N);
                        break;
                    case U_XFRZ:
                        tap_code(KC_W);
                        tap_code(KC_F);
                        tap_code(KC_F);
                        break;
                    case U_XIND:
                        tap_code(KC_H);
                        tap_code(KC_6);
                        break;
                    case U_XOUT:
                        tap_code(KC_H);
                        tap_code(KC_5);
                        break;
                    case U_XDECINC:
                        tap_code(KC_H);
                        tap_code(KC_0);
                        break;
                    case U_XDECDEC:
                        tap_code(KC_H);
                        tap_code(KC_9);
                        break;
                }
            }
            return false;

        // Process all other keycodes normally
        default:
            return true;
    }
}

// ***FEATURE MODIFICATIONS***
// Permissive Hold only for home-row shift and layer tap-holds
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_T): // Colemak-DH home-row left shift
        case RSFT_T(KC_N): // Colemak-DH home-row right shift
        case LSFT_T(KC_F): // QWERTY home-row left shift
        case RSFT_T(KC_J): // QWERTY home-row right shift
        case LT(U_BUTTON,KC_Z): // Layer tap-holds for Base and Extra
        case LT(U_BUTTON,KC_SLSH):
        case LT(U_SYS,KC_ESC):
        case LT(U_NAV,KC_SPC):
        case LT(U_MOUSE,KC_TAB):
        case LT(U_SYM,KC_ENT):
        case LT(U_NUM,KC_BSPC):
        case LT(U_FUN,KC_DEL):
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

// Caps Word modifications: Changes default behavior which shifts KC_MINS.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_MINS:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

// Tapping Term per Key modifications
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_A): // Left home-row pinky
        case RGUI_T(KC_O): // Right home-row pinky
            return TAPPING_TERM + 50; // Add 50ms to pinky tapping term.
        default:
            return TAPPING_TERM;
    }
}

// ***RESET COMBOS***
// All combos are evaluated from QWERTY Tap Layer (Layer 2)
enum combos {
  LAYER_CLEAR_L,
  LAYER_CLEAR_R,
  RESET_BASE_L,
  RESET_BASE_R,
  RESET_KEYBOARD,
};

const uint16_t PROGMEM layer_clear_L_combo[]    = {KC_Z,    KC_B,       COMBO_END};
const uint16_t PROGMEM layer_clear_R_combo[]    = {KC_N,    KC_SLSH,    COMBO_END};
const uint16_t PROGMEM reset_base_L_combo[]     = {KC_Q,    KC_T,       KC_Z,   KC_B,       COMBO_END};
const uint16_t PROGMEM reset_base_R_combo[]     = {KC_Y,    KC_P,       KC_N,   KC_SLSH,    COMBO_END};
const uint16_t PROGMEM reset_kb_combo[]         = {KC_Q,    KC_P,       KC_Z,   KC_SLSH,    COMBO_END};

combo_t key_combos[]    = {
  [LAYER_CLEAR_L]       = COMBO_ACTION(layer_clear_L_combo),
  [LAYER_CLEAR_R]       = COMBO_ACTION(layer_clear_R_combo),
  [RESET_BASE_L]        = COMBO_ACTION(reset_base_L_combo),
  [RESET_BASE_R]        = COMBO_ACTION(reset_base_R_combo),
  [RESET_KEYBOARD]      = COMBO_ACTION(reset_kb_combo),
};

// Define and handle COMBO_ACTION events
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {

        case LAYER_CLEAR_L:
        case LAYER_CLEAR_R:
            if (pressed) {
                layer_clear();      // Clears all layers (turns them all off).
            }
            break;

        case RESET_BASE_L:          // Resets to hardcoded base default layer
        case RESET_BASE_R:
            if (pressed) {
                current_default_layer = DEFAULT_LAYER;
                set_single_default_layer(current_default_layer);
            }
            break;

        case RESET_KEYBOARD:
        // Tap resets keyboard without loading the bootloader.
        // Hold resets persistent memory to hardcoded default layer and Mac mode.
            if (pressed) {
                reset_timer = timer_read(); // Start timer on combo press
            } else {
                if (timer_elapsed(reset_timer) < TAPPING_TERM) {
                    soft_reset_keyboard();
                } else {
                    current_default_layer = DEFAULT_LAYER;  // Reset default layer from hardcode
                    set_single_persistent_default_layer(current_default_layer);

                    eeconfig_init_user();                   // Reset user config (Mac Mode)
                    user_config.raw = eeconfig_read_user(); // Read user config from EEPROM into RAM
                    set_mac_mode(user_config.mac_mode);     // Reset Mac Mode from reset user config
                }
            }
            break;

    }
}

// ***KEYMAP DEFINITIONS***
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [U_BASE] = LAYOUT( // Colemak-DH
    RGUI_T(KC_Q),       ALGR_T(KC_W),   RCTL_T(KC_F),       RSFT_T(KC_P),       KC_B,                                                       KC_J,               LSFT_T(KC_L),       LCTL_T(KC_U),   ALGR_T(KC_Y),   LGUI_T(KC_QUOT),
    LGUI_T(KC_A),       LALT_T(KC_R),   LCTL_T(KC_S),       LSFT_T(KC_T),       KC_G,                                                       KC_M,               RSFT_T(KC_N),       RCTL_T(KC_E),   LALT_T(KC_I),   RGUI_T(KC_O),
    LT(U_BUTTON,KC_Z),  KC_X,           KC_C,               KC_D,               KC_V,               KC_NO,              KC_NO,              KC_K,               KC_H,               KC_COMM,        KC_DOT,         LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,              LT(U_SYS,KC_ESC),   LT(U_NAV,KC_SPC),   LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT),   LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL),   KC_NO,          KC_NO,          KC_NO
  ),

  [U_EXTRA] = LAYOUT( // QWERTY
    RGUI_T(KC_Q),       ALGR_T(KC_W),   RCTL_T(KC_E),       RSFT_T(KC_R),       KC_T,                                                       KC_Y,               LSFT_T(KC_U),       LCTL_T(KC_I),   ALGR_T(KC_O),   LGUI_T(KC_P),
    LGUI_T(KC_A),       LALT_T(KC_S),   LCTL_T(KC_D),       LSFT_T(KC_F),       KC_G,                                                       KC_H,               RSFT_T(KC_J),       RCTL_T(KC_K),   LALT_T(KC_L),   RGUI_T(KC_QUOT),
    LT(U_BUTTON,KC_Z),  KC_X,           KC_C,               KC_V,               KC_B,               KC_NO,              KC_NO,              KC_N,               KC_M,               KC_COMM,        KC_DOT,         LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,              LT(U_SYS,KC_ESC),   LT(U_NAV,KC_SPC),   LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT),   LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL),   KC_NO,          KC_NO,          KC_NO
  ),

  [U_TAP]   = LAYOUT( // Single-Action QWERTY
    KC_Q,               KC_W,           KC_E,               KC_R,               KC_T,                                                       KC_Y,               KC_U,               KC_I,           KC_O,           KC_P,
    KC_A,               KC_S,           KC_D,               KC_F,               KC_G,                                                       KC_H,               KC_J,               KC_K,           KC_L,           KC_QUOT,
    KC_Z,               KC_X,           KC_C,               KC_V,               KC_B,               KC_NO,              KC_NO,              KC_N,               KC_M,               KC_COMM,        KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              KC_ESC,             KC_SPC,             KC_TAB,             KC_ENT,             KC_BSPC,            KC_DEL,             KC_NO,          KC_NO,          KC_NO
  ),

  [U_BUTTON] = LAYOUT(
    U_UND,              U_CUT,          U_CPY,              U_PST,              U_RDO,                                                      U_RDO,              U_PST,              U_CPY,          U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,            KC_LSFT,            U_UND,                                                      U_UND,              KC_LSFT,            KC_LCTL,        KC_LALT,        KC_LGUI,
    U_UND,              U_CUT,          U_CPY,              U_PST,              U_RDO,              KC_NO,              KC_NO,              U_RDO,              U_PST,              U_CPY,          U_CUT,          U_UND,
    KC_NO,              KC_NO,          KC_NO,              KC_BTN3,            KC_BTN1,            KC_BTN2,            KC_BTN2,            KC_BTN1,            KC_BTN3,            KC_NO,          KC_NO,          KC_NO
  ),

  [U_NAV] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),      KC_NO,                                                      U_RDO,              U_PST,              U_CPY,          U_CUT,          U_UND,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),      KC_NO,                                                      CW_TOGG,            KC_LEFT,            KC_DOWN,        KC_UP,          KC_RGHT,
    KC_NO,              KC_NO,          KC_NO,              TD(U_TD_LLCK),      KC_NO,              KC_NO,              KC_NO,              KC_INS,             KC_HOME,            KC_PGDN,        KC_PGUP,        KC_END,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_ENT,             KC_BSPC,            KC_DEL,             KC_NO,          KC_NO,          KC_NO
  ),

  [U_MOUSE] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),      MS_ACL2,                                                    U_RDO,              U_PST,              U_CPY,          U_CUT,          U_UND,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),      MS_ACL1,                                                    KC_CAPS,            MS_LEFT,            MS_DOWN,        MS_UP,          MS_RGHT,
    KC_NO,              KC_NO,          KC_NO,              TD(U_TD_LLCK),      MS_ACL0,            KC_NO,              KC_NO,              KC_INS,             U_WHLL,             U_WHLD,         U_WHLU,         U_WHLR,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_BTN2,            KC_BTN1,            KC_BTN3,            KC_NO,          KC_NO,          KC_NO
  ),

  [U_SYS] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),      TD(U_TD_PSCR),                                              U_XWIN,             U_BRWSR_BCK,        U_TABB,         U_TABF,         U_BRWSR_FWD,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),      KC_SCRL,                                                    U_SEARCH,           KC_MPRV,            KC_VOLD,        KC_VOLU,        KC_MNXT,
    QK_BOOT,            TD(U_TD_MAC),   TD(U_TD_EXTRA),     TD(U_TD_LLCK),      KC_PAUS,            KC_NO,              KC_NO,              U_XFRZ,             U_XOUT,             U_XDECDEC,      U_XDECINC,      U_XIND,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_MSTP,            KC_MPLY,            KC_MUTE,            KC_NO,          KC_NO,          KC_NO

  ),

  [U_NUM] = LAYOUT(
    KC_LBRC,            KC_7,           KC_8,               KC_9,               KC_RBRC,                                                    KC_BSPC,            OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_SCLN,            KC_4,           KC_5,               KC_6,               KC_EQL,                                                     DM_PLY1,            OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_GRV,             KC_1,           KC_2,               KC_3,               KC_BSLS,            KC_NO,              KC_NO,              KC_SPC,             TD(U_TD_LLCK),      KC_COMM,        KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              DM_REC1,            KC_0,               KC_MINS,            KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_SYM] = LAYOUT(
    KC_LCBR,            KC_AMPR,        KC_ASTR,            KC_LPRN,            KC_RCBR,                                                    KC_BSPC,            OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_COLN,            KC_DLR,         KC_PERC,            KC_CIRC,            KC_PLUS,                                                    DM_PLY2,            OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_TILD,            KC_EXLM,        KC_AT,              KC_HASH,            KC_PIPE,            KC_NO,              KC_NO,              KC_SPC,             TD(U_TD_LLCK),      KC_COMM,        KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              DM_REC2,            KC_RPRN,            U_MDASH,            KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_FUN] = LAYOUT(
    KC_F12,             KC_F7,          KC_F8,              KC_F9,              KC_F15,                                                     TD(U_TD_PSCR),      OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_F11,             KC_F4,          KC_F5,              KC_F6,              KC_F14,                                                     KC_SCRL,            OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_F10,             KC_F1,          KC_F2,              KC_F3,              KC_F13,             KC_NO,              KC_NO,              KC_PAUS,            TD(U_TD_LLCK),      TD(U_TD_EXTRA), TD(U_TD_MAC),   QK_BOOT,
    KC_NO,              KC_NO,          KC_NO,              KC_APP,             KC_SPC,             KC_TAB,             KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_NUMPAD] = LAYOUT(
    KC_PAST,            KC_P7,          KC_P8,              KC_P9,              KC_PMNS,                                                    KC_BSPC,            OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_PEQL,            KC_P4,          KC_P5,              KC_P6,              KC_PPLS,                                                    KC_NUM,             OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_PSLS,            KC_P1,          KC_P2,              KC_P3,              KC_PENT,            KC_NO,              KC_NO,              KC_SPC,             TD(U_TD_LLCK),      KC_COMM,        KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              KC_PCMM,            KC_P0,              KC_PDOT,            KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),
};

// --- HARDWARE SETUP ---
// Restore user settings from EEPROM after QMK and keyboard init
void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();             // Load saved user config from EEPROM
    set_mac_mode(user_config.mac_mode);                 // Apply saved Mac/Win mode setting
}