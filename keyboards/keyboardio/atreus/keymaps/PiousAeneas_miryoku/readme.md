# PiousAeneas Custom Miryoku

![Custom Keymap](layout/keyboard-layout-PiousAeneas-miryoku.png)

A custom keyboard layout adapted heavily from Manna Harbour's [Miryoku](https://github.com/manna-harbour/miryoku) layout.
- Compile firmware by typing: `qmk compile -kb keyboardio/atreus -km PiousAeneas_miryoku`.

## Primary Alpha Layers

Default Base Layer alphas are [Colemak Mod-DH](https://web.archive.org/web/20250405062747/https://colemakmods.github.io/mod-dh/).

- **QWERTY**: Optional `Extra` Base Layer with QWERTY Alphas accessible from `System` Layer (see below).
- **Home Row Mods**: Default Base Layer takes advantage of dual-function [home row mods](https://web.archive.org/web/20250405055459/https://precondition.github.io/home-row-mods) i.e. tap for alpha, hold for the modifier indicated in the lower front legend. Opposite-hand modifiers are available on the top row. Tapping term is extended by 50ms for pinky-key home row mods to prevent disruptive accidental triggering of e.g. `Win+R` and `Win+L`.
- **Permissive Hold** is enabled for home-row shifts and all thumb key layer tap-holds, so these keys will immediately select the hold action if another key is tapped before the first key is released.
- **Auto-Repeat** is disabled on Primary Alpha Layers but available on secondary functional layers. For example, thumb tap keys are mirrored onto some functional layers for use with auto-repeat.
- **Tap Layer**: Tap Layer has only the tap functions of the Base Layer with no dual-function keys other than the Reset Combos. In other words, all keys on the Tap Layer should behave as regular single action keys without any delay and with auto-repeat enabled. This is useful for things like WASD navigation in games or in applications where keystroke timing is important.

### Reset Combos
- **Clear Layers**: Return to the current Default Base Layer from any layer by tapping the bottom two corner keys on any side i.e. the `KC_Z + KC_V` or `KC_K + KC_SLSH` positions based on Colemak Mod-DH Alphas.
- **Reset Base**: Reset keyboard to hardcoded Colemak Mod-DH Default Base Layer by tapping all four corner keys on any side i.e. `KC_Z + KC_V + KC_K + KC_SLSH` based on Colemak Mod-DH Alphas.
- **Reset Keyboard**: Reset keyboard - equivalent to unplugging the keyboard and then plugging it back in - by tapping all four corner keys from any layer i.e. the `KC_Q + KC_Z + KC_QUOT + KC_SLSH` positions based on Colemak Mod-DH Alphas.
- **Hard Reset**: Reset EEPROM to hardcoded Base Layer Colemak Mod-DH Alphas and Windows Mode (i.e. Mac Mode off) by *holding* all four corner keys from any layer.

## Secondary Functional Layers

Hold dual-function thumb keys to access secondary functional layers. Per Miryoku, layers are designed with a single purpose per hand and are accessed by holding a thumb key on the opposite hand. Modifiers available on the same hand as the thumb key means that any combination of modifiers and single keys can be produced without any finger contortions. All layers on the same hand are based on the same basic key arrangement.

- **One-shot Mods** for both hands are mirrored from the Base Layer on the same hand as the layer change thumb key. Mods are only enabled on the opposite hand, so auto-repeat is available on the home row on layers for use with cursor and mouse keys.
- **Layer Lock**: Tap bottom row index column once from any secondary layer to lock the current layer, tap twice to lock the opposite hand's layer. Unlock by tapping the layer lock key once or by initiating any Reset Combo.

**Numbers and Symbols** are available on the left hand with numerals in standard numbad locations with symbols in the remaing positions. **Auto Shift** is enabled for numbers and symbols, simply hold the key to get its shifted state. Symbol layer has shifted symbols in the same locations as Number Layer with the exception of `(` duplicated next to `)` and `—` replacing `_`.

**Navigation and Mouse Layers** are available on the right hand using Vim-style cursor keys on the home position. Thumb keys are duplicated from the base layer to avoid having to layer change mid edit and to enable **auto-repeat**.
- **Caps Word** capitalizes all alphas but unlike the [default QMK behavior](https://web.archive.org/web/20250328095706/https://docs.qmk.fm/features/caps_word), it does not also turn `-` into `_`. Caps Word automatically disables itself once a space or any key other than `KC_A--KC_Z`, `KC_0--KC_9`, `KC_MINS`, `KC_UNDS`, `KC_DELETE`, or `KC_BACKSPACE` is pressed. Caps Word also disables itself if the keyboard is idle for 5 seconds. Shift + Caps Word = Caps Lock.
- **Mouse Emulation** available from the Mouse Layer. Mouse movement mirrors cursor navigation on home row, and scroll wheel mirrors line and page movement below. **Mac Mode** reverses scroll directions for compatibility with Natural Scrolling on trackpad. Hold keys on same-hand inner column to [temporarily modify](https://web.archive.org/web/20250327175910/https://docs.qmk.fm/features/mouse_keys#combined-mode) mouse and scroll speed.

**System Layer**: Tertiary right-hand layer provides media control, browser navigation, system shortcuts, and Microsoft Excel keyboard macros that all mirror the directional keys from the Navigation and Mouse Layers. `Pause`, `Stop`, and `Mute` are on the primary, secondary, and tertiary thumbs, respectively. System keys are on the same-hand inner and bottom column:
- **Screen Shot**: Double-tap `Snip` for `Print Screen`.
- **Extra Alphas**: Single tap for Tap Layer, double tap for Extra QWERTY Layer, triple tap to save the current default layer into persistent memory.
- **Mac Mode**: Single tap for for Mac mode, double tap for to Windows mode, triple tap to save current mode into persistent memory. Mac Mode swaps `GUI` with `Control`, reverses scroll directions, and changes OS-specific shortcuts for clipboard functions, print screen, system search, and browser navigation.
- **Boot**: Tap same-hand pinky column for `QK_BOOT` which resets keyboard into bootloader mode (Keyboardio Atreus also requires you to simultaneously hold the bottom left key).

**Function Layer**: Function keys mirror numerals from the Number Layer with extras on the pinkie and inner columns. `App` Context Menu key is on the tertiary thumb key and other thumb keys are duplicated from the Base Layer to enable **auto-repeat**. Same-hand system keys are mirrored from the System Layer.

## Additional Layers

**Button Layer** provides mouse buttons, keyboard modifiers, and clipboard shortcuts for use with integrated or external pointing devices, used with either hand. It is activated by holding a bottom row pinkie key.

**Num Pad Layer**: Combo the primary and secodary left hand thumb keys (similar to the [Miryoku mod](https://web.archive.org/web/20250403090449/https://github.com/manna-harbour/miryoku_qmk/tree/miryoku/users/manna-harbour_miryoku#thumb-combos) for keyboards with only two thumb keys) to access the Num Bad Layer which provides all 
[number pad variants](https://web.archive.org/web/20250411163123/https://docs.qmk.fm/keycodes_basic#number-pad) of QMK keycodes for use in certain applications and for functionality such as alt codes.