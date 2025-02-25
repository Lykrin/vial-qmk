#include QMK_KEYBOARD_H


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
      [0] = LAYOUT(
	KC_ESC,  KC_1, KC_2, KC_3,    KC_4,    KC_5,                    KC_6,  KC_7,    KC_8,    KC_9,  KC_0,     KC_GRV,
	KC_TAB,  KC_Q, KC_W, KC_E,    KC_R,    KC_T,                    KC_Y,  KC_U,    KC_I,    KC_O,  KC_P,     KC_MINS,
	KC_LCTL, KC_A, KC_S, KC_D,    KC_F,    KC_G,                    KC_H,  KC_J,    KC_K,    KC_L,  KC_SCLN,  KC_QUOT,
	KC_LSFT, KC_Z, KC_X, KC_C,    KC_V,    KC_B,  KC_LBRC, KC_RBRC, KC_N,  KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
	                     KC_LALT, KC_LGUI, MO(1), KC_SPC,  KC_ENT,  MO(2), KC_BSPC, KC_RGUI
      ),
      [1] = LAYOUT(
	_______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
	KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
	KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD,
	_______, _______, _______, _______, _______, _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
	                           _______, _______, _______, _______, _______, MO(3), _______, _______
      ),
      [2] = LAYOUT(
	_______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
	KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
	KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
	KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
	                           _______, _______, MO(3),   _______, _______, _______, _______, _______
      ),
      [3] = LAYOUT(
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI,
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD,
	                           _______, _______, _______, _______, _______, _______, _______, _______
      )
};



#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [1] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [2] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [3] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
};
#endif

// Define constants
#define RGB_IDLE_TIMEOUT 30000 // 30 seconds
#define RGB_IDLE_BRIGHTNESS 64 // About 20% of 128 = ~25
#define RGB_NORMAL_BRIGHTNESS 128 // Normal brightness

// Global variables
static uint16_t rgb_timer = 0;
static bool is_rgb_dimmed = false;

void keyboard_post_init_user(void) {
    // Initialize RGB Matrix to static green at normal brightness
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_GREEN);
    rgb_timer = timer_read();
}

void housekeeping_task_user(void) {
    // Check if keyboard has been idle
    if (!is_rgb_dimmed && timer_elapsed(rgb_timer) > RGB_IDLE_TIMEOUT) {
        // First change to cycle all effect
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_LEFT_RIGHT);
        // Then set brightness without changing hue/saturation
        // Use the current HSV values but change only the Value (brightness)
        rgb_matrix_set_speed_noeeprom(64);
        rgb_matrix_sethsv_noeeprom(255, 255, RGB_IDLE_BRIGHTNESS);
        is_rgb_dimmed = true;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Any key press resets the timer and restores static green
        rgb_timer = timer_read();
        if (is_rgb_dimmed) {
            // Change back to static color
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
            is_rgb_dimmed = false;
        }
    }
    return true;
}
