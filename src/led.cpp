#include <Arduino.h>
#include "neo/neo.h"
#include "led.h"

// ===================================================================================
// Color section
// ============================================================================

static int color_hue_s[3] = {0, 0, 0}; // hue value: 0..191 color map
static int curretn_key_s = -1;         // current press key

void led_set_color_hue(uint8_t led0, uint8_t led1, uint8_t led2)
{
  color_hue_s[0] = led0;
  color_hue_s[1] = led1;
  color_hue_s[2] = led2;
}

void led_presskey(int key)
{
  curretn_key_s = key;
}

void led_update()
{
  for (int led = 0; led < 3; led++)
  {
    if (curretn_key_s == led)
    {
      NEO_writeColor(led, 255, 255, 255);
    }
    else
    {
      NEO_writeHue(led, color_hue_s[led], NEO_BRIGHT_KEYS);
    }
  }

  NEO_update();
}