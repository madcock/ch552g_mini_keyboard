#include <Arduino.h>
#include "userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"
#include "led.h"
#include "keyboard.h"
#include "../configuration.h"

static int current_mode_s = 0;                                       // current mode of keyboard

static void set_menu_led(void);

// ===================================================================================
// create keyboard configuration
// ===================================================================================

const button_function_t button_function_null = {
    .type = BUTTON_NULL};

// ===================================================================================
// Menu section
// ===================================================================================

static void set_menu_led(void)
{

  int led_index = current_mode_s % 3;
  int color_background = (current_mode_s / 3) * 16;

  switch (led_index)
    {
    case 0:
      led_set_color_hue(NEO_RED + color_background, NEO_RED + color_background, NEO_RED + color_background);
      break;
    case 1:
      led_set_color_hue(NEO_GREEN + color_background, NEO_GREEN + color_background, NEO_GREEN + color_background);
      break;
    case 2:
      led_set_color_hue(NEO_BLUE + color_background, NEO_BLUE + color_background, NEO_BLUE + color_background);
      break;
    default:
      break;
    }
}

void button_menu_up(keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_CLICK)
  {
    if (current_mode_s >= NUM_CONFIGURATION - 1)
    {
      current_mode_s = NUM_CONFIGURATION - 1;
    }
    else
    {
      current_mode_s++;
    }
    set_menu_led();
  }
}

void button_menu_down(keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_CLICK)
  {
    if (current_mode_s <= 0)
    {
      current_mode_s = 0;
    }
    else
    {
      current_mode_s--;
    }
    set_menu_led();
  }
}

static void keyboard_run_key_sequence(button_sequence_t sequence, keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_RELEASE)
  {
    return;
  }

  for (uint8_t i = 0; i < sequence.length; i++)
  {
    Keyboard_press(sequence.sequence[i]);
    delay(10);
    if (sequence.delay > 0)
    {
      Keyboard_release(sequence.sequence[i]);
      delay(sequence.delay);
    }
  }
  Keyboard_releaseAll();
}

static void keyboard_run_mouse_sequence(button_mouse_t sequence, keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_RELEASE)
  {
    return;
  }
  if (sequence.keypress > 0)
  {
    Keyboard_press(sequence.keypress);
    delay(30);
  }
  for (uint8_t i = 0; i < sequence.length; i++)
  {
    switch (sequence.mouse_event_sequence[i].type)
    {
    case UP:
      Mouse_move(0, -sequence.mouse_event_sequence[i].value);
      break;
    case DOWN:
      Mouse_move(0, sequence.mouse_event_sequence[i].value);
      break;
    case LEFT:
      Mouse_move(-sequence.mouse_event_sequence[i].value, 0);
      break;
    case RIGH:
      Mouse_move(sequence.mouse_event_sequence[i].value, 0);
      break;
    case LEFT_CLICK:
      Mouse_click(MOUSE_LEFT);
      break;
    case RIGHT_CLICK:
      Mouse_click(MOUSE_RIGHT);
      break;
    case SCROLL_UP:
      Mouse_scroll(sequence.mouse_event_sequence[i].value);
      break;
    case SCROLL_DOWN:
      Mouse_scroll(-sequence.mouse_event_sequence[i].value);
      break;
    default:
      break;
    }
    if (sequence.keypress > 0)
    {
      Keyboard_releaseAll();
    }
    delay(sequence.delay);
  }
}

void keyboard_press_button(keyboard_button_t button, keyboard_button_keyboard_mode_t mode)
{
  if (button >= BTN_1 && button <= BTN_3)
  {
    if (mode == BTM_PRESS)
    {
      led_presskey(button);
    }
    else
    {
      led_presskey(-1);
    }
  }

  switch (configurations[current_mode_s].button[button].type)
  {
  case BUTTON_SEQUENCE:
    keyboard_run_key_sequence(configurations[current_mode_s].button[button].function.sequence, mode);
    break;
  case BUTTON_MOUSE:
    keyboard_run_mouse_sequence(configurations[current_mode_s].button[button].function.mouse, mode);
    break;
  case BUTTON_FUNCTION:
    configurations[current_mode_s].button[button].function.functionPointer(mode);
    break;
  case BUTTON_NULL:
    break;
  default:
    break;
  }
}
