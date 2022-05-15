#include "../include/safire/input.h"

#include "../include/safire/pipeline.h"

#include <GLFW/glfw3.h>




struct SFRinput {
  SFR_InputKeyBinding*                  press_once_key;
  SFR_InputMouseBinding*                press_once_mouse;

  uint32_t                              press_once_key_count;
  uint32_t                              press_once_mouse_count;

  SFR_InputKeyBinding*                  release_once_key;
  SFR_InputMouseBinding*                release_once_mouse;

  uint32_t                              release_once_key_count;
  uint32_t                              release_once_mouse_count;

  SFR_Window*                           window;
};


static struct SFRinput                  sinput;




void sfr_input_init() 
{
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

bool sfr_input_keyboard_once(SFR_InputAction action, SFR_InputKeyBinding input) 
{
  // TODO: once presses/released, return true for that frame, once frame done, return false, until they press/release again 
  // in other words, only return the value once for that frame
  return false;
}

bool sfr_input_mouse_once(SFR_InputAction action, SFR_InputMouseBinding input) 
{
  // TODO: ...
  return false;
}

bool sfr_input_keyboard(SFR_InputAction action, SFR_InputKeyBinding input) 
{
  if (glfwGetKey(sinput.window->window, (int)input) == (int)action) 
    return true;
  else 
    return false;
}

bool sfr_input_mouse(SFR_InputAction action, SFR_InputMouseBinding input) 
{
  if (glfwGetMouseButton(sinput.window->window, (int)input) == (int)action) 
    return true;
  else
    return false;
}

void sfr_input_get_mouse_position(vec2 mouse, bool screen_space) 
{
  // formula found here:
  // https://stackoverflow.com/questions/7692988/opengl-math-projecting-screen-space-to-world-space-coords

  SFR_Window* window = sfr_pipeline_get_window();
  // getting the screen space coords
  double x, y;
  glfwGetCursorPos(window->window, &x, &y);
  glm_vec2_copy((vec2) { (float)x, (float)y }, mouse);

  if (!screen_space) 
  {
    // converting to world space
    mat4 projection;
    mat4 view;
    mat4 inv;
    sfr_pipeline_get_projection_matrix(projection);
    sfr_pipeline_get_view_matrix(view);
    glm_mat4_mul(projection, view, inv);
    glm_mat4_inv(inv, inv);

    float width = (float)window->width;
    float height = (float)window->height;

    // convert the screen coords into a value between -1 and 1
    vec4 in = {
      (2.0f * (mouse[X]) / (width)) - 1.0f,
      1.0f - (2.0f * (mouse[Y] / height)),
      0.0f, 1.0f
    };
    
    glm_mat4_mulv(inv, in, in);

    glm_vec2_copy(
      (vec2) {
        in[X],
        in[Y]
      },
      mouse
    );
  }
}

SFR_InputKeyBinding* sfr_input_keyboard_get_current(SFR_InputAction action) 
{
  // TODO: implementation of the key that is being pressed once needs to be done before doing this ...
  return NULL;
}

SFR_InputMouseBinding* sfr_input_mouse_get_current(SFR_InputAction action) 
{
  // TODO: implementation of the key that is being pressed once needs to be done before doing this ...
  return NULL;
}

void sfr_input_cursor_mode(SFR_InputCursorMode mode) 
{
  glfwSetInputMode(sinput.window->window, GLFW_CURSOR, (int)mode);
}
