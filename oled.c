 /* Copyright 2020 Josef Adamcik
  * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
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

//Sets up what the OLED screens display.



char wpm[16];

// Function to draw pixels when keys are pressed. Call this in oled_task_user

#define MATRIX_DISPLAY_X 8
#define MATRIX_DISPLAY_Y 110

static void print_matrix(void){
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        for (uint8_t y = 0; y < MATRIX_COLS; y++) {
            if (x >= 5) {
                oled_write_pixel(MATRIX_DISPLAY_X - y + 14, MATRIX_DISPLAY_Y + x - 3,(matrix_get_row(x) & (1 << y))> 0);
            } else {
                oled_write_pixel(MATRIX_DISPLAY_X + y + 2, MATRIX_DISPLAY_Y + x + 2,(matrix_get_row(x) & (1 << y))> 0);
            }
        }
    }

    for (uint8_t x = 0; x < 18; x++) {
        oled_write_pixel(MATRIX_DISPLAY_X + x, MATRIX_DISPLAY_Y - 2,true);
        oled_write_pixel(MATRIX_DISPLAY_X + x, MATRIX_DISPLAY_Y + 9,true);
    }
    for (uint8_t y = 0; y < 9; y++) {
        oled_write_pixel(MATRIX_DISPLAY_X - 1, MATRIX_DISPLAY_Y + y - 1, true);
        oled_write_pixel(MATRIX_DISPLAY_X + 18, MATRIX_DISPLAY_Y + y - 1, true);
    }
}

static void print_logo_narrow(void) {
    uint8_t n = get_current_wpm();
    char    wpm_str[4];
    oled_set_cursor(0, 14);
    wpm_str[3] = '\0';
    wpm_str[2] = '0' + n % 10;
    wpm_str[1] = '0' + (n /= 10) % 10;
    wpm_str[0] = '0' + n / 10;
    oled_write(" ", false);
    oled_write(wpm_str, false);

    oled_set_cursor(0, 15);
    oled_write(" WPM", false);
}

static void print_status_narrow(void) {
    /* Print current mode */
    oled_set_cursor(0, 0);
    oled_set_cursor(0, 3);

    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            // oled_write("QWRTY", false);
            oled_write("\n", false);
            break;
        // case _GAMING:
        //     oled_write("GAMES", false);
        //     break;
        default:
            oled_write("UNDEF", false);
    }

    oled_set_cursor(0, 5);

    /* Print current layer */
    oled_write("LAYER", false);

    oled_set_cursor(0, 6);

    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write("Base ", false);
            break;
        case _RAISE:
            oled_write("Raise", false);
            break;
        case _LOWER:
            oled_write("Lower", false);
            break;
        case _ADJUST:
            oled_write("Adj  ", false);
            break;
        default:
            oled_write("Undef", false);
    }

    /* caps lock */
    led_t led_usb_state = host_keyboard_led_state();
    oled_set_cursor(0, 9);
    oled_write("CPSLK", led_usb_state.caps_lock);

    print_matrix();
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        print_logo_narrow();
    }
    return false;
}

