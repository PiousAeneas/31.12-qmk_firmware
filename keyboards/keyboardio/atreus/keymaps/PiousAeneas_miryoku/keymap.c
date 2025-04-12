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
    U_FUN,
    U_NUMPAD
};

// Define default Windows clipboard actions. U_PST defined in Tap Dance section.
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)

// Define boolean to track if Mac Mode is active.
bool isMac = false; 

// ***TAP DANCE***
// Tap dance declarations
enum {
    U_TD_U_NAV,     // Layer Locks
    U_TD_U_MOUSE,
    U_TD_U_SYS,
    U_TD_U_NUM,
    U_TD_U_SYM,
    U_TD_U_FUN,
    U_TD_U_BASE,    // For extra alpha layer
    U_TD_MAC,       // Mac Mode
    U_TD_PST,       // Paste Special
    U_TD_PSCR,      // Screenshot
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
void u_td_fn_U_BASE(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        default_layer_set((layer_state_t)1 << U_BASE); // Base layer on single tap
    } else if (state->count == 2) {
        default_layer_set((layer_state_t)1 << U_EXTRA); // Extra layer on double tap
    }
}

// Tap dance helper functions: Mac Mode
void u_td_mac_fn(tap_dance_state_t *state, void *user_data) { // 1:Mac, 2:Win
    switch (state->count) { // Use state->count to determine tap count.
        case 1:
            isMac = true; // Turn on Mac Mode.
            keymap_config.swap_lctl_lgui = true; // Swap Left Control and Left GUI.
            keymap_config.swap_rctl_rgui = true; // Swap Right Control and Right GUI.
            break;
        case 2:
            isMac = false; // Turn off Mac Mode.
            keymap_config.swap_lctl_lgui = false;
            keymap_config.swap_rctl_rgui = false;
            break;
        default:
            break; // Do nothing for unexpected tap counts.
    }
}

// Tap dance helper functions: Paste Special tap dance action
void u_td_pst_sp_fn(tap_dance_state_t *state, void *user_data) { // 1:Paste, 2:Paste Special
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
    [U_TD_U_NAV]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NAV),   // Layer Locks
    [U_TD_U_MOUSE]  = ACTION_TAP_DANCE_FN(u_td_fn_U_MOUSE),
    [U_TD_U_SYS]    = ACTION_TAP_DANCE_FN(u_td_fn_U_SYS),
    [U_TD_U_NUM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_NUM),
    [U_TD_U_SYM]    = ACTION_TAP_DANCE_FN(u_td_fn_U_SYM),
    [U_TD_U_FUN]    = ACTION_TAP_DANCE_FN(u_td_fn_U_FUN),

    [U_TD_U_BASE]   = ACTION_TAP_DANCE_FN(u_td_fn_U_BASE),  // Extra Alphas
    [U_TD_MAC]      = ACTION_TAP_DANCE_FN(u_td_mac_fn),     // Mac Mode
    [U_TD_PST]      = ACTION_TAP_DANCE_FN(u_td_pst_sp_fn),  // Paste Special
    [U_TD_PSCR]     = ACTION_TAP_DANCE_FN(u_td_pscr_fn),    // Screenshot
};

// Define U_PST as paste special tap dance to work across all keymaps.
#define U_PST TD(U_TD_PST)


// ***CUSTOM KEYCODES***
// Custom keycode declarations
enum custom_keycodes {
    U_TABB = SAFE_RANGE, U_TABF,    // Tab navigation
    U_BRWSR_BCK, U_BRWSR_FWD,       // Browser navigation
    U_WH_L, U_WH_D, U_WH_U, U_WH_R, // Mouse scrolling
    U_SEARCH,                       // "Spotlight" search
    U_MDASH,                        // Em Dash
    U_XWIN, U_XFRZ,                 // Excel Shortcuts: New Window, Freeze Panes
    U_XIND, U_XOUT,                 // Excel Shortcuts: Indent, Outdent
    U_XDECINC, U_XDECDEC,           // Excel Shortcuts: Increase / Decrease Decimal
    U_NPON,                         // Activates Num Pad and turns on Num Lock
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

        // Activate Num Pad and Num Lock
        case U_NPON:
            if (record->event.pressed) {
                if (!host_keyboard_led_state().num_lock) {
                    tap_code(KC_NUM_LOCK); // Tap NumLock if NumLock is OFF.
                }
                layer_on(U_NUMPAD);
            }
            return false; // Skip all further processing of this key.

        default:
            return true;
    }
}

// ***FEATURE MODIFICATIONS***
// Permissive Hold only for home-row shift and layer tap-holds
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_T): // Base Colemak-DH home-row left shift
        case RSFT_T(KC_N): // Base Colemak-DH home-row right shift
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

// ***LAYER LOCK RETURN COMBOS***
// Combos for returning to Base or Extra Layer after a single-handed layer lock.
enum combos {
  NAV_TO_BASE,
  MOUSE_TO_BASE,
  SYS_TO_BASE,
  NUM_TO_BASE,
  SYM_TO_BASE,
  FUN_TO_BASE,
  NAV_TO_EXTRA,
  MOUSE_TO_EXTRA,
  SYS_TO_EXTRA,
  NUM_TO_EXTRA,
  SYM_TO_EXTRA,
  FUN_TO_EXTRA
};

const uint16_t PROGMEM nav_base_combo[]     = {KC_INS,  KC_HOME,    COMBO_END};
const uint16_t PROGMEM mouse_base_combo[]   = {KC_INS,  U_WH_L,     COMBO_END};
const uint16_t PROGMEM sys_base_combo[]     = {U_XFRZ,  U_XOUT,     COMBO_END};
const uint16_t PROGMEM num_base_combo[]     = {KC_3,    KC_BSLS,    COMBO_END};
const uint16_t PROGMEM sym_base_combo[]     = {KC_HASH, KC_PIPE,    COMBO_END};
const uint16_t PROGMEM fun_base_combo[]     = {KC_F3,   KC_F13,     COMBO_END};

const uint16_t PROGMEM nav_extra_combo[]    = {KC_INS,  KC_HOME,    KC_PGDN,    COMBO_END};
const uint16_t PROGMEM mouse_extra_combo[]  = {KC_INS,  U_WH_L,     U_WH_D,     COMBO_END};
const uint16_t PROGMEM sys_extra_combo[]    = {U_XFRZ,  U_XOUT,     U_XDECDEC,  COMBO_END};
const uint16_t PROGMEM num_extra_combo[]    = {KC_2,    KC_3,       KC_BSLS,    COMBO_END};
const uint16_t PROGMEM sym_extra_combo[]    = {KC_AT,   KC_HASH,    KC_PIPE,    COMBO_END};
const uint16_t PROGMEM fun_extra_combo[]    = {KC_F2,   KC_F3,      KC_F13,     COMBO_END};

combo_t key_combos[]    = {
  [NAV_TO_BASE]         = COMBO(nav_base_combo,     DF(U_BASE)),
  [MOUSE_TO_BASE]       = COMBO(mouse_base_combo,   DF(U_BASE)),
  [SYS_TO_BASE]         = COMBO(sys_base_combo,     DF(U_BASE)),
  [NUM_TO_BASE]         = COMBO(num_base_combo,     DF(U_BASE)),
  [SYM_TO_BASE]         = COMBO(sym_base_combo,     DF(U_BASE)),
  [FUN_TO_BASE]         = COMBO(fun_base_combo,     DF(U_BASE)),
  [NAV_TO_EXTRA]        = COMBO(nav_extra_combo,    DF(U_EXTRA)),
  [MOUSE_TO_EXTRA]      = COMBO(mouse_extra_combo,  DF(U_EXTRA)),
  [SYS_TO_EXTRA]        = COMBO(sys_extra_combo,    DF(U_EXTRA)),
  [NUM_TO_EXTRA]        = COMBO(num_extra_combo,    DF(U_EXTRA)),
  [SYM_TO_EXTRA]        = COMBO(sym_extra_combo,    DF(U_EXTRA)),
  [FUN_TO_EXTRA]        = COMBO(fun_extra_combo,    DF(U_EXTRA)),
};

// ***KEYMAP DEFINITIONS***
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [U_BASE] = LAYOUT( // Colemak-DH
    RGUI_T(KC_Q),       ALGR_T(KC_W),   RCTL_T(KC_F),       RSFT_T(KC_P),       KC_B,                                                       KC_J,               LSFT_T(KC_L),       LCTL_T(KC_U),   ALGR_T(KC_Y),   LGUI_T(KC_QUOT),
    LGUI_T(KC_A),       LALT_T(KC_R),   LCTL_T(KC_S),       LSFT_T(KC_T),       KC_G,                                                       KC_M,               RSFT_T(KC_N),       RCTL_T(KC_E),   LALT_T(KC_I),   RGUI_T(KC_O),
    LT(U_BUTTON,KC_Z),  KC_X,           KC_C,               KC_D,               KC_V,               KC_NO,              KC_NO,              KC_K,               KC_H,               KC_COMM,        KC_DOT,         LT(U_BUTTON,KC_SLSH),
    KC_NO,              KC_NO,          KC_NO,              LT(U_SYS,KC_ESC),   LT(U_NAV,KC_SPC),   LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT),   LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL),   KC_NO,          KC_NO,          KC_NO
  ),

  [U_EXTRA] = LAYOUT( // QWERTY
    KC_Q,               KC_W,           KC_E,               KC_R,               KC_T,                                                       KC_Y,               KC_U,               KC_I,           KC_O,           KC_P,
    KC_A,               KC_S,           KC_D,               KC_F,               KC_G,                                                       KC_H,               KC_J,               KC_K,           KC_L,           KC_QUOT,
    KC_Z,               KC_X,           KC_C,               KC_V,               KC_B,               KC_NO,              KC_NO,              KC_N,               KC_M,               KC_COMM,        KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              LT(U_SYS,KC_ESC),   LT(U_NAV,KC_SPC),   LT(U_MOUSE,KC_TAB), LT(U_SYM,KC_ENT),   LT(U_NUM,KC_BSPC),  LT(U_FUN,KC_DEL),   KC_NO,          KC_NO,          KC_NO
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
    KC_NUM_LOCK,        U_NPON,         TD(U_TD_U_NAV),     TD(U_TD_U_BASE),    KC_NO,              KC_NO,              KC_NO,              KC_INS,             KC_HOME,            KC_PGDN,        KC_PGUP,        KC_END,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_ENT,             KC_BSPC,            KC_DEL,             KC_NO,          KC_NO,          KC_NO
  ),

  [U_MOUSE] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),      MS_ACL2,                                                    U_RDO,              U_PST,              U_CPY,          U_CUT,          U_UND,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),      MS_ACL1,                                                    KC_CAPS,            MS_LEFT,            MS_DOWN,        MS_UP,          MS_RGHT,
    KC_NO,              KC_NO,          TD(U_TD_U_MOUSE),   TD(U_TD_U_BASE),    MS_ACL0,            KC_NO,              KC_NO,              KC_INS,             MS_WHLL,            MS_WHLD,        MS_WHLU,        MS_WHLR,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_BTN2,            KC_BTN1,            KC_BTN3,            KC_NO,          KC_NO,          KC_NO
  ),

  [U_SYS] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),      TD(U_TD_PSCR),                                              U_XWIN,             U_BRWSR_BCK,        U_TABB,         U_TABF,         U_BRWSR_FWD,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),      KC_SCRL,                                                    U_SEARCH,           KC_MPRV,            KC_VOLD,        KC_VOLU,        KC_MNXT,
    QK_BOOT,            TD(U_TD_MAC),   TD(U_TD_U_SYS),     TD(U_TD_U_BASE),    KC_PAUS,            KC_NO,              KC_NO,              U_XFRZ,             U_XOUT,             U_XDECDEC,      U_XDECINC,      U_XIND,
    KC_NO,              KC_NO,          KC_NO,              KC_NO,              KC_NO,              KC_NO,              KC_MSTP,            KC_MPLY,            KC_MUTE,            KC_NO,          KC_NO,          KC_NO

  ),

  [U_NUM] = LAYOUT(
    KC_LBRC,            KC_7,           KC_8,               KC_9,               KC_RBRC,                                                    KC_NO,              OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_SCLN,            KC_4,           KC_5,               KC_6,               KC_EQL,                                                     KC_SPC,             OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_GRV,             KC_1,           KC_2,               KC_3,               KC_BSLS,            KC_NO,              KC_NO,              KC_NO,              TD(U_TD_U_BASE),    TD(U_TD_U_NUM), KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              KC_DOT,             KC_0,               KC_MINS,            KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_SYM] = LAYOUT(
    KC_LCBR,            KC_AMPR,        KC_ASTR,            KC_LPRN,            KC_RCBR,                                                    KC_NO,              OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_COLN,            KC_DLR,         KC_PERC,            KC_CIRC,            KC_PLUS,                                                    KC_SPC,             OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_TILD,            KC_EXLM,        KC_AT,              KC_HASH,            KC_PIPE,            KC_NO,              KC_NO,              KC_NO,              TD(U_TD_U_BASE),    TD(U_TD_U_SYM), KC_DOT,         KC_SLSH,
    KC_NO,              KC_NO,          KC_NO,              KC_LPRN,            KC_RPRN,            U_MDASH,            KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_FUN] = LAYOUT(
    KC_F12,             KC_F7,          KC_F8,              KC_F9,              KC_F15,                                                     TD(U_TD_PSCR),      OSM(MOD_LSFT),      OSM(MOD_LCTL),  OSM(MOD_RALT),  OSM(MOD_LGUI),
    KC_F11,             KC_F4,          KC_F5,              KC_F6,              KC_F14,                                                     KC_SCRL,            OSM(MOD_RSFT),      OSM(MOD_RCTL),  OSM(MOD_LALT),  OSM(MOD_RGUI),
    KC_F10,             KC_F1,          KC_F2,              KC_F3,              KC_F13,             KC_NO,              KC_NO,              KC_PAUS,            TD(U_TD_U_BASE),    TD(U_TD_U_FUN), TD(U_TD_MAC),   QK_BOOT,
    KC_NO,              KC_NO,          KC_NO,              KC_APP,             KC_SPC,             KC_TAB,             KC_NO,              KC_NO,              KC_NO,              KC_NO,          KC_NO,          KC_NO
  ),

  [U_NUMPAD] = LAYOUT(
    OSM(MOD_RGUI),      OSM(MOD_RALT),  OSM(MOD_RCTL),      OSM(MOD_RSFT),    KC_NO,                                                        KC_PAST,            KC_P7,              KC_P8,          KC_P9,          KC_PMNS,
    OSM(MOD_LGUI),      OSM(MOD_LALT),  OSM(MOD_LCTL),      OSM(MOD_LSFT),    KC_BSPC,                                                      KC_PEQL,            KC_P4,              KC_P5,          KC_P6,          KC_PPLS,
    KC_NUM_LOCK,        TG(U_NUMPAD),   KC_NO,              TG(U_NUMPAD),     KC_NO,                KC_NO,              KC_NO,              KC_PSLS,            KC_P1,              KC_P2,          KC_P3,          KC_PENT,
    KC_NO,              KC_NO,          KC_NO,              KC_ESC,            KC_SPC,              KC_TAB,             KC_PCMM,            KC_P0,              KC_PDOT,            KC_NO,          KC_NO,          KC_NO
  ),
};
