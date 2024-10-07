// Adapted heavily from Manna Harbour's Miryoku layout.
// https://github.com/manna-harbour/miryoku

#include QMK_KEYBOARD_H

// ***DEFINITIONS***
// Define Keymap Layers
enum miryoku_layers {
    U_BASE,
    U_EXTRA,
    U_BUTTON,
    U_NAV,
    U_MOUSE,
    U_SYS,
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
    U_TD_U_NAV, // Layer Locks
    U_TD_U_MOUSE,
    U_TD_U_SYS,
    U_TD_U_NUM,
    U_TD_U_SYM,
    U_TD_U_FUN,
    U_TD_U_EXTRA, // Extra layer
    U_TD_MAC, U_TD_WIN, // Mac Mode
    U_TD_PST, // Paste Special
    U_TD_PSCR, // Screenshot
};

// Tap dance helper functions
// Tap dance helper functions: Layer Locks
void u_td_fn_U_NAV(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_NAV); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_NUM); // Opposite layer on double tap
    }
}
void u_td_fn_U_MOUSE(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_MOUSE); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_SYM); // Opposite layer on double tap
    }
}
void u_td_fn_U_SYS(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_SYS); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_FUN); // Opposite layer on double tap
    }
}
void u_td_fn_U_NUM(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_NUM); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_NAV); // Opposite layer on double tap
    }
}
void u_td_fn_U_SYM(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_SYM); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_MOUSE); // Opposite layer on double tap
    }
}
void u_td_fn_U_FUN(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_FUN); // Same layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_SYS); // Opposite layer on double tap
    }
}

// Tap dance helper functions: Base / Extra Layer
void u_td_fn_U_EXTRA(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_BASE); // Base layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_EXTRA); // Extra layer on double tap
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

    // Layer Locks
    [U_TD_U_NAV]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NAV),
    [U_TD_U_MOUSE]  = ACTION_TAP_DANCE_FN(u_td_fn_U_MOUSE),
    [U_TD_U_SYS]    = ACTION_TAP_DANCE_FN(u_td_fn_U_SYS),
    [U_TD_U_NUM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NUM),
    [U_TD_U_SYM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_SYM),
    [U_TD_U_FUN]    = ACTION_TAP_DANCE_FN(u_td_fn_U_FUN),

    // Extra Alphas
    [U_TD_U_EXTRA]  = ACTION_TAP_DANCE_FN(u_td_fn_U_EXTRA),
    
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


// Permissive Hold only for home-row shift and layer tap-holds
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_T): // Base Colemak-DH home-row left shift
        case RSFT_T(KC_N): // Base Colemak-DH home-row right shift
        case LSFT_T(KC_F): // Extra QWERTY home-row left shift
        case RSFT_T(KC_J): // Extra QWERTY home-row right shift
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
    RGUI_T(KC_Q),       ALGR_T(KC_W),   RCTL_T(KC_F),     RSFT_T(KC_P),     KC_B,                                                   KC_J,               LSFT_T(KC_L),     LCTL_T(KC_U),     ALGR_T(KC_Y),   LGUI_T(KC_QUOT),
    LGUI_T(KC_A),       LALT_T(KC_R),   LCTL_T(KC_S),     LSFT_T(KC_T),     KC_G,                                                   KC_M,               RSFT_T(KC_N),     RCTL_T(KC_E),     LALT_T(KC_I),   RGUI_T(KC_O),
    LT(U_BUTTON,KC_Z),  KC_X,           KC_C,             KC_D,             KC_V,             KC_NO,              KC_NO,            KC_K,               KC_H,             KC_COMM,          KC_DOT,         LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,            LT(U_SYS,KC_ESC), LT(U_NAV,KC_SPC), LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT), LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL), KC_NO,            KC_NO,          KC_NO
  ),

  [U_EXTRA] = LAYOUT( // QWERTY
    RGUI_T(KC_Q),       ALGR_T(KC_W),   RCTL_T(KC_E),     RSFT_T(KC_R),     KC_T,                                                   KC_Y,               LSFT_T(KC_U),     LCTL_T(KC_I),     ALGR_T(KC_O),   LGUI_T(KC_P),
    LGUI_T(KC_A),       LALT_T(KC_S),   LCTL_T(KC_D),     LSFT_T(KC_F),     KC_G,                                                   KC_H,               RSFT_T(KC_J),     RCTL_T(KC_K),     LALT_T(KC_L),   RGUI_T(KC_QUOT),
    LT(U_BUTTON,KC_Z),  KC_X,           KC_C,             KC_V,             KC_B,             KC_NO,              KC_NO,            KC_N,               KC_M,             KC_COMM,          KC_DOT,         LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,            LT(U_SYS,KC_ESC), LT(U_NAV,KC_SPC), LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT), LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL), KC_NO,            KC_NO,          KC_NO
  ),

  [U_BUTTON] = LAYOUT(
    U_UND,              U_CUT,          U_CPY,            U_PST,            U_RDO,                                                  U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,          U_UND,                                                  U_UND,              KC_LSFT,          KC_LCTL,          KC_LALT,        KC_LGUI,
    U_UND,              U_CUT,          U_CPY,            U_PST,            U_RDO,            KC_NO,              KC_NO,            U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_NO,              KC_NO,          KC_NO,            KC_BTN3,          KC_BTN1,          KC_BTN2,            KC_BTN2,          KC_BTN1,            KC_BTN3,          KC_NO,            KC_NO,          KC_NO
  ),

  [U_NAV] = LAYOUT(
    KC_RGUI,            KC_ALGR,        KC_RCTL,          KC_RSFT,          KC_NO,                                                  U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,          KC_NO,                                                  CW_TOGG,            KC_LEFT,          KC_DOWN,          KC_UP,          KC_RGHT,
    KC_NO,              KC_NO,          TD(U_TD_U_NAV),   TD(U_TD_U_EXTRA), KC_NO,            KC_NO,              KC_NO,            KC_INS,             KC_HOME,          KC_PGDN,          KC_PGUP,        KC_END,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,            KC_NO,            KC_NO,              KC_ENT,           KC_BSPC,            KC_DEL,           KC_NO,            KC_NO,          KC_NO
  ),

  [U_MOUSE] = LAYOUT(
    KC_RGUI,            KC_ALGR,        KC_RCTL,          KC_RSFT,          KC_ACL2,                                                U_RDO,              U_PST,            U_CPY,            U_CUT,          U_UND,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,          KC_ACL1,                                                KC_CAPS,            KC_MS_L,          KC_MS_D,          KC_MS_U,        KC_MS_R,
    KC_NO,              KC_NO,          TD(U_TD_U_MOUSE), TD(U_TD_U_EXTRA), KC_ACL0,          KC_NO,              KC_NO,            KC_NO,              U_WH_L,           U_WH_D,           U_WH_U,         U_WH_R,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,            KC_NO,            KC_NO,              KC_BTN2,          KC_BTN1,            KC_BTN3,          KC_NO,            KC_NO,          KC_NO
  ),

  [U_SYS] = LAYOUT(
    KC_RGUI,            KC_ALGR,        KC_RCTL,          KC_RSFT,          KC_NO,                                                  U_XWIN,             U_BRWSR_BCK,      U_TABB,           U_TABF,         U_BRWSR_FWD,
    KC_LGUI,            KC_LALT,        KC_LCTL,          KC_LSFT,          KC_NO,                                                  U_SEARCH,           KC_MPRV,          KC_VOLD,          KC_VOLU,        KC_MNXT,
    TD(U_TD_WIN),       KC_NO,          TD(U_TD_U_SYS),   TD(U_TD_U_EXTRA), TD(U_TD_MAC),     KC_NO,              KC_NO,            U_XFRZ,             U_XOUT,           U_XDECDEC,        U_XDECINC,      U_XIND,
    KC_NO,              KC_NO,          KC_NO,            KC_NO,            KC_NO,            KC_NO,              KC_MSTP,          KC_MPLY,            KC_MUTE,          KC_NO,            KC_NO,          KC_NO

  ),

  [U_NUM] = LAYOUT(
    KC_LBRC,            KC_7,           KC_8,             KC_9,             KC_RBRC,                                                KC_NO,              KC_LSFT,          KC_LCTL,          KC_ALGR,        KC_LGUI,
    KC_SCLN,            KC_4,           KC_5,             KC_6,             KC_EQL,                                                 KC_SPC,             KC_RSFT,          KC_RCTL,          KC_LALT,        KC_RGUI,
    KC_GRV,             KC_1,           KC_2,             KC_3,             KC_BSLS,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_EXTRA), TD(U_TD_U_NUM),   KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,            KC_DOT,           KC_0,             KC_MINS,            KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

  [U_SYM] = LAYOUT(
    KC_LCBR,            KC_AMPR,        KC_ASTR,          KC_LPRN,          KC_RCBR,                                                KC_NO,              KC_LSFT,          KC_LCTL,          KC_ALGR,        KC_LGUI,
    KC_COLN,            KC_DLR,         KC_PERC,          KC_CIRC,          KC_PLUS,                                                KC_SPC,             KC_RSFT,          KC_RCTL,          KC_LALT,        KC_RGUI,
    KC_TILD,            KC_EXLM,        KC_AT,            KC_HASH,          KC_PIPE,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_EXTRA), TD(U_TD_U_SYM),   KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,            KC_LPRN,          KC_RPRN,          U_MDASH,            KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

  [U_FUN] = LAYOUT(
    KC_F12,             KC_F7,          KC_F8,            KC_F9,            TD(U_TD_PSCR),                                          KC_NO,              KC_LSFT,          KC_LCTL,          KC_ALGR,        KC_LGUI,
    KC_F11,             KC_F4,          KC_F5,            KC_F6,            KC_SCRL,                                                KC_NO,              KC_RSFT,          KC_RCTL,          KC_LALT,        KC_RGUI,
    KC_F10,             KC_F1,          KC_F2,            KC_F3,            KC_PAUS,          KC_NO,              KC_NO,            KC_NO,              TD(U_TD_U_EXTRA), TD(U_TD_U_FUN),   KC_NO,          KC_NO,
    KC_NO,              KC_NO,          KC_NO,            KC_APP,           KC_SPC,           KC_TAB,             KC_NO,            KC_NO,              KC_NO,            KC_NO,            KC_NO,          KC_NO
  ),

};
