// Adapted heavily from Manna Harbour's Miryoku layout.
// https://github.com/manna-harbour/miryoku

#include QMK_KEYBOARD_H

// ***DEFINITIONS***
// Define Keymap Layers
enum miryoku_layers {
    U_BASE,
    U_EXTRA,
    U_TAP,
    U_BUTTON,
    U_NAV,
    U_MOUSE,
    U_MEDIA,
    U_NUM,
    U_SYM,
    U_FUN
};

// Define default Windows clipboard actions. U_PST defined in Tap Dance section.
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Define boolean to track if Mac Mode is active
bool isMac = false; 

// ***TAP DANCE***
// Tap dance declarations
enum {
    // Double tap guard for Additional Features
    U_TD_BOOT,
    U_TD_U_BASE,
    U_TD_U_EXTRA,
    U_TD_U_TAP,
    U_TD_U_BUTTON,
    U_TD_U_NAV,
    U_TD_U_MOUSE,
    U_TD_U_MEDIA,
    U_TD_U_NUM,
    U_TD_U_SYM,
    U_TD_U_FUN,
    
    U_TD_MAC, U_TD_WIN, // Mac Mode
    U_TD_PST, // Paste Special
    U_TD_PSCR, // Screenshot
};

// Tap dance helper functions
// Tap dance helper functions: Additional Features double tap guard
void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    reset_keyboard();
  }
}
void u_td_fn_U_BASE(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_BASE);
  }
}
void u_td_fn_U_EXTRA(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_EXTRA);
  }
}
void u_td_fn_U_TAP(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_TAP);
  }
}
void u_td_fn_U_BUTTON(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_BUTTON);
  }
}
void u_td_fn_U_NAV(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_NAV);
  }
}
void u_td_fn_U_MOUSE(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_MOUSE);
  }
}
void u_td_fn_U_MEDIA(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_MEDIA);
  }
}
void u_td_fn_U_NUM(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_NUM);
  }
}
void u_td_fn_U_SYM(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_SYM);
  }
}
void u_td_fn_U_FUN(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    default_layer_set((layer_state_t)1 << U_FUN);
  }
}

// Tap dance helper functions: Mac Mode
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) { // Turn on Mac Mode
  if (state->count == 2) {
    isMac = true;
    keymap_config.swap_lctl_lgui = true; // Swap Left Control and GUI
    keymap_config.swap_rctl_rgui = true; // Swap Right Control and GUI
  }
}

void u_td_win_fn(tap_dance_state_t *state, void *user_data) { // Turn off Mac Mode
  if (state->count == 2) {
    isMac = false;
    keymap_config.swap_lctl_lgui = false;
    keymap_config.swap_rctl_rgui = false;
  }
}

// Tap dance helper functions: Paste Special tap dance action
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

// Tap dance helper functions: Screenshot
void u_td_pscr_fn(tap_dance_state_t *state, void *user_data) {
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

    // Additional Features double tap guard
    [U_TD_BOOT]     = ACTION_TAP_DANCE_FN(u_td_fn_boot),
    [U_TD_U_BASE]   = ACTION_TAP_DANCE_FN(u_td_fn_U_BASE),
    [U_TD_U_EXTRA]  = ACTION_TAP_DANCE_FN(u_td_fn_U_EXTRA),
    [U_TD_U_TAP]    = ACTION_TAP_DANCE_FN(u_td_fn_U_TAP),
    [U_TD_U_BUTTON] = ACTION_TAP_DANCE_FN(u_td_fn_U_BUTTON),
    [U_TD_U_NAV]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NAV),
    [U_TD_U_MOUSE]  = ACTION_TAP_DANCE_FN(u_td_fn_U_MOUSE),
    [U_TD_U_MEDIA]  = ACTION_TAP_DANCE_FN(u_td_fn_U_MEDIA),
    [U_TD_U_NUM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NUM),
    [U_TD_U_SYM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_SYM),
    [U_TD_U_FUN]    = ACTION_TAP_DANCE_FN(u_td_fn_U_FUN),

    // Mac Mode
    [U_TD_MAC]      = ACTION_TAP_DANCE_FN(u_td_mac_fn),
    [U_TD_WIN]      = ACTION_TAP_DANCE_FN(u_td_win_fn),

    // Paste Special
    [U_TD_PST]      = ACTION_TAP_DANCE_FN(u_td_pst_sp_fn),

    // Screenshot
    [U_TD_PSCR]     = ACTION_TAP_DANCE_FN(u_td_pscr_fn),
};

// Define U_PST as paste special tap dance to work across all keymaps.
#define U_PST TD(U_TD_PST)


// ***CUSTOM KEYCODES***
// Custom keycode declarations
enum custom_keycodes {
    U_TABB = SAFE_RANGE, U_TABF, // Tab navigation
    U_BRWSR_BCK, U_BRWSR_FWD, // Browser navigation
    U_WH_L, U_WH_D, U_WH_U, U_WH_R, // Mouse scrolling
    U_SEARCH, // "Spotlight" search
    U_MDASH, // Em Dash
    U_XWIN, U_XFRZ, // Excel Shortcuts: New Window, Freeze Panes
    U_XIND, U_XOUT, // Excel Shortcuts: Indent, Outdent
    U_XDECINC, U_XDECDEC, // Excel Shortcuts: Increase / Decrease Decimal
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
        
        // Tab navigation with U_TABB and U_TABF
        case U_TABB:
        case U_TABF:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                if (keycode == U_TABB) {
                    register_code(KC_LSFT); // If U_TABB, press and hold Left Shift
                }
                tap_code(KC_TAB); // Tap Tab
                if (keycode == U_TABB) {
                    unregister_code(KC_LSFT);
                }
                unregister_code(KC_LCTL);
            }
            return false;

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
            return false;

        // Reverse mouse scrolling directions when in Mac mode
        case U_WH_L:
            if (record->event.pressed) {
                register_code(isMac ? KC_WH_R : KC_WH_L);
            } else {
                unregister_code(isMac ? KC_WH_R : KC_WH_L);
            }
            return false;
        case U_WH_D:
            if (record->event.pressed) {
                register_code(isMac ? KC_WH_U : KC_WH_D);
            } else {
                unregister_code(isMac ? KC_WH_U : KC_WH_D);
            }
            return false;
        case U_WH_U:
            if (record->event.pressed) {
                register_code(isMac ? KC_WH_D : KC_WH_U);
            } else {
                unregister_code(isMac ? KC_WH_D : KC_WH_U);
            }
            return false;
        case U_WH_R:
            if (record->event.pressed) {
                register_code(isMac ? KC_WH_L : KC_WH_R);
            } else {
                unregister_code(isMac ? KC_WH_L : KC_WH_R);
            }
            return false;

        // Search with U_SEARCH
        case U_SEARCH:
            if (record->event.pressed) {
                if (isMac) {
                    register_code(KC_LCMD); // Press and hold Left Command
                    tap_code(KC_SPC); // Tap Space
                    unregister_code(KC_LCMD); // Release Left Command
                } else {
                    register_code(KC_LGUI); // Press and hold Left GUI (Windows key)
                    tap_code(KC_S); // Tap S
                    unregister_code(KC_LGUI); // Release Left GUI
                }
            }
            return false;

        // Em Dash
        case U_MDASH:
            if (record->event.pressed) {
                if (isMac) { /// Mac em dash
                    register_code(KC_LALT);
                    register_code(KC_LSFT);
                    tap_code(KC_MINS);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LALT);
                } else { /// Win em dash
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

        default:
            return true;
    }
}

// Permissive Hold per Key Settings: Off for pinky and ring finger HRM
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Exceptions to Permissive Hold.
        case LGUI_T(KC_A): // Home row mods for BASE
        case LALT_T(KC_R):
        case LALT_T(KC_I):
        case LGUI_T(KC_O):
        case LALT_T(KC_S): // Home row mods for QWERTY
        case LALT_T(KC_L):
        case LGUI_T(KC_QUOT):
            return false;
        default:
            // Defualit is to select the hold action when another key is tapped.
            return true; // 
    }
}

// Caps Word modifications
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
        case KC_UNDS:
        case KC_MINS:
        case KC_DOT:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

// ***KEYMAP DEFINITIONS***

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


  [U_BASE] = LAYOUT( // Colemak-DH
    KC_Q,               KC_W,           KC_F,             KC_P,               KC_B,                                                   KC_J,               KC_L,             KC_U,             KC_Y,           KC_QUOT,
    LGUI_T(KC_A),       LALT_T(KC_R),   LCTL_T(KC_S),     LSFT_T(KC_T),       KC_G,                                                   KC_M,               LSFT_T(KC_N),     LCTL_T(KC_E),     LALT_T(KC_I),   LGUI_T(KC_O),
    LT(U_BUTTON,KC_Z),  ALGR_T(KC_X),   KC_C,             KC_D,               KC_V,             KC_NO,              KC_NO,            KC_K,               KC_H,             KC_COMM,          ALGR_T(KC_DOT), LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,            LT(U_MEDIA,KC_ESC), LT(U_NAV,KC_SPC), LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT), LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL), KC_NO,            KC_NO,          KC_NO
  ),

  [U_EXTRA] = LAYOUT( // QWERTY
    KC_Q,               KC_W,           KC_E,             KC_R,               KC_T,                                                   KC_Y,               KC_U,             KC_I,             KC_O,           KC_P,
    LGUI_T(KC_A),       LALT_T(KC_S),   LCTL_T(KC_D),     LSFT_T(KC_F),       KC_G,                                                   KC_H,               LSFT_T(KC_J),     LCTL_T(KC_K),     LALT_T(KC_L),   LGUI_T(KC_QUOT),
    LT(U_BUTTON,KC_Z),  ALGR_T(KC_X),   KC_C,             KC_V,               KC_B,             KC_NO,              KC_NO,            KC_N,               KC_M,             KC_COMM,          ALGR_T(KC_DOT), LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,            LT(U_MEDIA,KC_ESC), LT(U_NAV,KC_SPC), LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT), LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL), KC_NO,            KC_NO,          KC_NO
  ),

  [U_TAP] = LAYOUT( // QWERTY
    KC_Q,               KC_W,           KC_E,             KC_R,               KC_T,                                                   KC_Y,               KC_U,             KC_I,             KC_O,           KC_P,
    KC_A,               KC_S,           KC_D,             KC_F,               KC_G,                                                   KC_H,               KC_J,             KC_K,             KC_L,           KC_QUOT,
    KC_Z,               KC_X,           KC_C,             KC_V,               KC_B,             KC_NO,              KC_NO,            KC_N,               KC_M,             KC_COMM,          KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,            KC_ESC,             KC_SPC,           KC_TAB,             KC_ENT,           KC_BSPC,            KC_DEL,           KC_NO,            KC_NO,          KC_NO
  ),

  [U_BUTTON] = LAYOUT(
    U_UND,              U_CUT,          U_CPY,            U_PST,              U_RDO,                                                  U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,            KC_NO,                                                  KC_NO,              KC_LSFT,          KC_LCTL,          KC_LALT,        KC_LGUI,
    U_UND,              U_CUT,          U_CPY,            U_PST,              U_RDO,            KC_NO,              KC_NO,            U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_NO,              KC_NO,          KC_NO,            KC_BTN3,            KC_BTN1,          KC_BTN2,            KC_BTN2,          KC_BTN1,            KC_BTN3,          KC_NO,            KC_NO,          KC_NO
  ),

  [U_NAV] = LAYOUT(
    TD(U_TD_BOOT),      TD(U_TD_U_TAP), TD(U_TD_U_EXTRA), TD(U_TD_U_BASE),    KC_NO,                                                  U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,            KC_NO,                                                  CW_TOGG,            KC_LEFT,          KC_DOWN,          KC_UP,          KC_RGHT,
    KC_NO,              KC_ALGR,        TD(U_TD_U_NUM),   TD(U_TD_U_NAV),     KC_NO,            KC_NO,              KC_NO,            KC_INS,             KC_HOME,          KC_PGDN,          KC_PGUP,        KC_END,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,              KC_NO,            KC_NO,              KC_ENT,           KC_BSPC,            KC_DEL,           KC_NO,            KC_NO,          KC_NO
  ),

  [U_MOUSE] = LAYOUT(
    TD(U_TD_BOOT),      TD(U_TD_U_TAP), TD(U_TD_U_EXTRA), TD(U_TD_U_BASE),    KC_ACL2,                                                U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,            KC_ACL1,                                                KC_CAPS,            KC_MS_L,          KC_MS_D,          KC_MS_U,        KC_MS_R,
    KC_NO,              KC_ALGR,        TD(U_TD_U_SYM),   TD(U_TD_U_MOUSE),   KC_ACL0,          KC_NO,              KC_NO,            KC_NO,              U_WH_L,           U_WH_D,           U_WH_U,         U_WH_R,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,              KC_NO,            KC_NO,              KC_BTN2,          KC_BTN1,            KC_BTN3,          KC_NO,            KC_NO,          KC_NO
  ),

  [U_MEDIA] = LAYOUT(
    TD(U_TD_BOOT),      TD(U_TD_U_TAP), TD(U_TD_U_EXTRA), TD(U_TD_U_BASE),    KC_NO,                                                  U_XWIN,             U_BRWSR_BCK,      U_TABB,           U_TABF,         U_BRWSR_FWD,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,            KC_NO,                                                  U_SEARCH,           KC_MPRV,          KC_VOLD,          KC_VOLU,        KC_MNXT,
    TD(U_TD_WIN),       KC_ALGR,        TD(U_TD_U_FUN),   TD(U_TD_U_MEDIA),   TD(U_TD_MAC),     KC_NO,              KC_NO,            U_XFRZ,             U_XOUT,           U_XDECDEC,        U_XDECINC,      U_XIND,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,              KC_NO,            KC_NO,              KC_MSTP,          KC_MPLY,            KC_MUTE,          KC_NO,            KC_NO,          KC_NO

  ),

  [U_NUM] = LAYOUT(
    KC_LBRC,            KC_7,           KC_8,             KC_9,               KC_RBRC,                                                KC_NO,              TD(U_TD_U_BASE),  TD(U_TD_U_EXTRA), TD(U_TD_U_TAP), TD(U_TD_BOOT),
    KC_SCLN,            KC_4,           KC_5,             KC_6,               KC_EQL,                                                 KC_SPC,             KC_LSFT,          KC_LCTL,          KC_LALT,        KC_LGUI,
    KC_GRV,             KC_1,           KC_2,             KC_3,               KC_BSLS,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_NUM),   TD(U_TD_U_NAV),   KC_ALGR,        KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,            KC_DOT,             KC_0,             KC_MINS,            KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

  [U_SYM] = LAYOUT(
    KC_LCBR,            KC_AMPR,        KC_ASTR,          KC_LPRN,            KC_RCBR,                                                KC_NO,              TD(U_TD_U_BASE),  TD(U_TD_U_EXTRA), TD(U_TD_U_TAP), TD(U_TD_BOOT),
    KC_COLN,            KC_DLR,         KC_PERC,          KC_CIRC,            KC_PLUS,                                                KC_SPC,             KC_LSFT,          KC_LCTL,          KC_LALT,        KC_LGUI,
    KC_TILD,            KC_EXLM,        KC_AT,            KC_HASH,            KC_PIPE,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_SYM),   TD(U_TD_U_MOUSE), KC_ALGR,        KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,            KC_LPRN,            KC_RPRN,          U_MDASH,            KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

  [U_FUN] = LAYOUT(
    KC_F12,             KC_F7,          KC_F8,            KC_F9,              TD(U_TD_PSCR),                                          KC_NO,              TD(U_TD_U_BASE),  TD(U_TD_U_EXTRA), TD(U_TD_U_TAP), TD(U_TD_BOOT),
    KC_F11,             KC_F4,          KC_F5,            KC_F6,              KC_SCRL,                                                KC_NO,              KC_LSFT,          KC_LCTL,          KC_LALT,        KC_LGUI,
    KC_F10,             KC_F1,          KC_F2,            KC_F3,              KC_PAUS,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_FUN),   TD(U_TD_U_MEDIA), KC_ALGR,        KC_NO,
    KC_NO,              KC_NO,          KC_NO,            KC_APP,             KC_SPC,           KC_TAB,             KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

};
