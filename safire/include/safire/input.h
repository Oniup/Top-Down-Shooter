#ifndef __SAFIRE_INPUT_H__
#define __SAFIRE_INPUT_H__

#include "utils.h"
#include "keybindings.h"

typedef enum SFRaction SFRaction_t;
typedef enum SFRcursor_mode SFRcursor_mode_t;

enum SFRaction {
  SFR_INPUT_RELEASE = 0, // this is the same value as the glfw macro GLFW_RELEASE
  SFR_INPUT_PRESS
};

extern void sfr_input_init();

extern bool sfr_input_keyboard_once(SFRaction_t action, SFRkeybinding_t input);
extern bool sfr_input_mouse_once(SFRaction_t action, SFRmouse_binding_t input);
extern bool sfr_input_keyboard(SFRaction_t action, SFRkeybinding_t input);
extern bool sfr_input_mouse(SFRaction_t action, SFRmouse_binding_t input);

extern void sfr_input_get_mouse_pos(vec2 mouse_pos);

// extern bool sfr_input_keyboard_mod(SFRaction_t action, SFRkey_mod_binding_t input, bool state); // TODO: don't need it for the game so out of scope

// extern bool sfr_input_gamepad(SFRaction_t action, SFRgamepad_binding_t input); // TODO: well out of scope of the program

extern SFRkeybinding_t* sfr_input_keyboard_get_current(SFRaction_t action);
extern SFRmouse_binding_t* sfr_input_mouse_get_current(SFRaction_t action);
// extern SFRkey_mod_binding_t* sfr_input_key_mod_get_current(SFRaction_t action);

enum SFRcursor_mode {
  SFR_CURSOR_NORMAL = 0x00034001,
  SFR_CURSOR_HIDDEN,
  SFR_CURSOR_DISABLED
};

extern void sfr_input_cursor_mode(SFRcursor_mode_t mode);


#endif // __SAFIRE_INPUT_H__