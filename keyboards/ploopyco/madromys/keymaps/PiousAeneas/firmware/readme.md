
# Ploopyco Adept Trackball Firmware Files

**PiousAeneas Firmware**:

**Default Firmware**: `ploopyco_madromys_rev1_001_via_mom_semaphore_4.uf2`

```c
/* Ploopy Adept Default Layout
 * +---------+-------+-------+---------+
 * |         |       |       |         |
 * | KC_BTN4 |KC_BTN5|DrgScrl| KC_BTN2 |
 * |         |       |       |         |
 * +---------+-------+-------+---------+
 * |                                   |
 * |                                   |
 * +-------------+       +-------------+
 * |             |       |             |
 * |   KC_BTN1   |       |   KC_BTN3   |
 * |             |       |             |
 * +-------------+-------+-------------+
 */
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN4, KC_BTN5, DRAG_SCROLL, KC_BTN2, KC_BTN1, KC_BTN3 )
};
```
