#pragma once
// Key colors (hue value: 0..191)
#define NEO_RED 0    // red
#define NEO_YEL 32   // yellow
#define NEO_GREEN 64 // green
#define NEO_CYAN 96  // cyan
#define NEO_BLUE 128 // blue
#define NEO_MAG 160  // magenta
#define NEO_WHITE 191  // white
#define NEO_BRIGHT_KEYS 2

// set led color in FIX mode
void led_set_color_hue(uint8_t led0, uint8_t led1, uint8_t led2);

// update led task
void led_update();

void led_presskey(int key);

