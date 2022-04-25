#ifndef __SAFIRE_INPUT_H__
#define __SAFIRE_INPUT_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"
#include "keybindings.h"

typedef enum SFRaction SFRaction_t;
typedef enum SFRcursor_mode SFRcursor_mode_t;

// this is the same value as the glfw macro GLFW_RELEASE
enum SFRaction {
  SFR_INPUT_RELEASE       = 0, 
  SFR_INPUT_PRESS
};

SAFIRE_API void             sfr_input_init();

SAFIRE_API bool             sfr_input_keyboard_once(SFRaction_t action, SFRkeybinding_t input);
SAFIRE_API bool             sfr_input_mouse_once(SFRaction_t action, SFRmouse_binding_t input);
SAFIRE_API bool             sfr_input_keyboard(SFRaction_t action, SFRkeybinding_t input);
SAFIRE_API bool             sfr_input_mouse(SFRaction_t action, SFRmouse_binding_t input);

SAFIRE_API void             sfr_input_get_mouse_pos(vec2 mouse_pos);

// SAFIRE_API bool sfr_input_keyboard_mod(SFRaction_t action, SFRkey_mod_binding_t input, bool state); // TODO: don't need it for the game so out of scope

// SAFIRE_API bool sfr_input_gamepad(SFRaction_t action, SFRgamepad_binding_t input); // TODO: well out of scope of the program

SAFIRE_API SFRkeybinding_t*     
                            sfr_input_keyboard_get_current(SFRaction_t action);
SAFIRE_API SFRmouse_binding_t*  
                            sfr_input_mouse_get_current(SFRaction_t action);
// SAFIRE_API SFRkey_mod_binding_t* sfr_input_key_mod_get_current(SFRaction_t action);

enum SFRcursor_mode {
  SFR_CURSOR_NORMAL         = 0x00034001,
  SFR_CURSOR_HIDDEN                     ,
  SFR_CURSOR_DISABLED
};

SAFIRE_API void             sfr_input_cursor_mode(SFRcursor_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_INPUT_H__