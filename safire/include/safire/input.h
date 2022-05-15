#ifndef __SAFIRE_INPUT_H__
#define __SAFIRE_INPUT_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "utils.h"
#include "keybindings.h"

typedef enum SFR_InputAction            SFR_InputAction;
typedef enum SFR_InputCursorMode        SFR_InputCursorMode;




// this is the same value as the glfw macro GLFW_RELEASE
enum SFR_InputAction {
  SFR_INPUT_RELEASE                     = 0, 
  SFR_INPUT_PRESS
};

SAFIRE_API void                         sfr_input_init();

SAFIRE_API bool                         sfr_input_keyboard_once(SFR_InputAction action, SFR_InputKeyBinding input);
SAFIRE_API bool                         sfr_input_mouse_once(SFR_InputAction action, SFR_InputMouseBinding input);
SAFIRE_API bool                         sfr_input_keyboard(SFR_InputAction action, SFR_InputKeyBinding input);
SAFIRE_API bool                         sfr_input_mouse(SFR_InputAction action, SFR_InputMouseBinding input);

SAFIRE_API void                         sfr_input_get_mouse_position(vec2 mouse, bool screen_space);




SAFIRE_API SFR_InputKeyBinding*         sfr_input_keyboard_get_current(SFR_InputAction action);
SAFIRE_API SFR_InputMouseBinding*       sfr_input_mouse_get_current(SFR_InputAction action);
// SAFIRE_API SFR_InputKeyModBinding* sfr_input_key_mod_get_current(SFR_InputAction action);

enum SFR_InputCursorMode {
  SFR_CURSOR_NORMAL                     = 0x00034001,
  SFR_CURSOR_HIDDEN,
  SFR_CURSOR_DISABLED
};

SAFIRE_API void                         sfr_input_cursor_mode(SFR_InputCursorMode mode);




#ifdef __cplusplus
}
#endif

#endif // __SAFIRE_INPUT_H__
