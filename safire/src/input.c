#include "../include/safire/input.h"

#include "../include/safire/pipeline.h"

#include <GLFW/glfw3.h>

struct SFRinput {
  SFRkeybinding_t*          press_once_key;
  SFRmouse_binding_t*       press_once_mouse;

  uint32_t                  press_once_key_count;
  uint32_t                  press_once_mouse_count;

  SFRkeybinding_t*          release_once_key;
  SFRmouse_binding_t*       release_once_mouse;

  uint32_t                  release_once_key_count;
  uint32_t                  release_once_mouse_count;

  SFRwindow_t*              window;
};

static struct SFRinput sinput;

void sfr_input_init() {
  sinput.press_once_key = NULL;
  sinput.press_once_mouse = NULL;

  sinput.press_once_key_count = 0;
  sinput.press_once_mouse_count = 0;

  sinput.release_once_key = NULL;
  sinput.release_once_mouse = NULL;

  sinput.release_once_key_count = 0;
  sinput.release_once_mouse_count = 0;

  sinput.window = sfr_pipeline_get_window();
}

bool sfr_input_keyboard_once(SFRaction_t action, SFRkeybinding_t input) {
  // TODO: once presses/released, return true for that frame, once frame done, return false, until they press/release again 
  // in other words, only return the value once for that frame
  return false;
}

bool sfr_input_mouse_once(SFRaction_t action, SFRmouse_binding_t input) {
  // TODO: ...
  return false;
}

bool sfr_input_keyboard(SFRaction_t action, SFRkeybinding_t input) {
  if (glfwGetKey(sinput.window->window, (int)input) == (int)action) {
    return true;
  } else {
    return false;
  }
}

bool sfr_input_mouse(SFRaction_t action, SFRmouse_binding_t input) {
  if (glfwGetMouseButton(sinput.window->window, (int)input) == (int)action) {
    return true;
  } else {
    return false;
  }
}

void sfr_input_get_mouse_pos(vec2 mouse_pos) {
  double x, y;
  glfwGetCursorPos(sinput.window->window, &x, &y);
  glm_vec2_copy((vec2){ (float)x, (float)y }, mouse_pos);
}

SFRkeybinding_t* sfr_input_keyboard_get_current(SFRaction_t action) {
  // TODO: implementation of the key that is being pressed once needs to be done before doing this ...
  return NULL;
}

SFRmouse_binding_t* sfr_input_mouse_get_current(SFRaction_t action) {
  // TODO: implementation of the key that is being pressed once needs to be done before doing this ...
  return NULL;
}

void sfr_input_cursor_mode(SFRcursor_mode_t mode) {
  glfwSetInputMode(sinput.window->window, GLFW_CURSOR, (int)mode);
}
