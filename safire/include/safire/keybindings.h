#ifndef __SAFIRE_KEYBINDINGS_H__
#define __SAFIRE_KEYBINDINGS_H__

typedef enum SFRkeybinding SFRkeybinding_t;
typedef enum SFRkey_mod_binding SFRkey_mod_binding_t;
typedef enum SFRmouse_binding SFRmouse_binding_t;
typedef enum SFRgamepad_binding SFRgamepad_binding_t; // TODO: for now, this is well out of scope

/* these keybindings came directly from glfw3.h*/

enum SFRkeybinding {
  /* The unknown key */
  SFR_KEY_UNKNOWN            = -1,

  /* Printable keys */
  SFR_KEY_SPACE              = 32,
  SFR_KEY_APOSTROPHE         = 39,  /* ' */
  SFR_KEY_COMMA              = 44,  /* , */
  SFR_KEY_MINUS              = 45,  /* - */
  SFR_KEY_PERIOD             = 46,  /* . */
  SFR_KEY_SLASH              = 47,  /* / */
  SFR_KEY_0                  = 48,
  SFR_KEY_1                  = 49,
  SFR_KEY_2                  = 50,
  SFR_KEY_3                  = 51,
  SFR_KEY_4                  = 52,
  SFR_KEY_5                  = 53,
  SFR_KEY_6                  = 54,
  SFR_KEY_7                  = 55,
  SFR_KEY_8                  = 56,
  SFR_KEY_9                  = 57,
  SFR_KEY_SEMICOLON          = 59,  /* ; */
  SFR_KEY_EQUAL              = 61,  /* = */
  SFR_KEY_A                  = 65,
  SFR_KEY_B                  = 66,
  SFR_KEY_C                  = 67,
  SFR_KEY_D                  = 68,
  SFR_KEY_E                  = 69,
  SFR_KEY_F                  = 70,
  SFR_KEY_G                  = 71,
  SFR_KEY_H                  = 72,
  SFR_KEY_I                  = 73,
  SFR_KEY_J                  = 74,
  SFR_KEY_K                  = 75,
  SFR_KEY_L                  = 76,
  SFR_KEY_M                  = 77,
  SFR_KEY_N                  = 78,
  SFR_KEY_O                  = 79,
  SFR_KEY_P                  = 80,
  SFR_KEY_Q                  = 81,
  SFR_KEY_R                  = 82,
  SFR_KEY_S                  = 83,
  SFR_KEY_T                  = 84,
  SFR_KEY_U                  = 85,
  SFR_KEY_V                  = 86,
  SFR_KEY_W                  = 87,
  SFR_KEY_X                  = 88,
  SFR_KEY_Y                  = 89,
  SFR_KEY_Z                  = 90,
  SFR_KEY_LEFT_BRACKET       = 91,  /* [ */
  SFR_KEY_BACKSLASH          = 92,  /* \ */
  SFR_KEY_RIGHT_BRACKET      = 93,  /* ] */
  SFR_KEY_GRAVE_ACCENT       = 96,  /* ` */
  SFR_KEY_WORLD_1            = 161, /* non-US #1 */
  SFR_KEY_WORLD_2            = 162, /* non-US #2 */

  /* Function keys */
  SFR_KEY_ESCAPE             = 256,
  SFR_KEY_ENTER              = 257,
  SFR_KEY_TAB                = 258,
  SFR_KEY_BACKSPACE          = 259,
  SFR_KEY_INSERT             = 260,
  SFR_KEY_DELETE             = 261,
  SFR_KEY_RIGHT              = 262,
  SFR_KEY_LEFT               = 263,
  SFR_KEY_DOWN               = 264,
  SFR_KEY_UP                 = 265,
  SFR_KEY_PAGE_UP            = 266,
  SFR_KEY_PAGE_DOWN          = 267,
  SFR_KEY_HOME               = 268,
  SFR_KEY_END                = 269,
  SFR_KEY_CAPS_LOCK          = 280,
  SFR_KEY_SCROLL_LOCK        = 281,
  SFR_KEY_NUM_LOCK           = 282,
  SFR_KEY_PRINT_SCREEN       = 283,
  SFR_KEY_PAUSE              = 284,
  SFR_KEY_F1                 = 290,
  SFR_KEY_F2                 = 291,
  SFR_KEY_F3                 = 292,
  SFR_KEY_F4                 = 293,
  SFR_KEY_F5                 = 294,
  SFR_KEY_F6                 = 295,
  SFR_KEY_F7                 = 296,
  SFR_KEY_F8                 = 297,
  SFR_KEY_F9                 = 298,
  SFR_KEY_F10                = 299,
  SFR_KEY_F11                = 300,
  SFR_KEY_F12                = 301,
  SFR_KEY_F13                = 302,
  SFR_KEY_F14                = 303,
  SFR_KEY_F15                = 304,
  SFR_KEY_F16                = 305,
  SFR_KEY_F17                = 306,
  SFR_KEY_F18                = 307,
  SFR_KEY_F19                = 308,
  SFR_KEY_F20                = 309,
  SFR_KEY_F21                = 310,
  SFR_KEY_F22                = 311,
  SFR_KEY_F23                = 312,
  SFR_KEY_F24                = 313,
  SFR_KEY_F25                = 314,
  SFR_KEY_KP_0               = 320,
  SFR_KEY_KP_1               = 321,
  SFR_KEY_KP_2               = 322,
  SFR_KEY_KP_3               = 323,
  SFR_KEY_KP_4               = 324,
  SFR_KEY_KP_5               = 325,
  SFR_KEY_KP_6               = 326,
  SFR_KEY_KP_7               = 327,
  SFR_KEY_KP_8               = 328,
  SFR_KEY_KP_9               = 329,
  SFR_KEY_KP_DECIMAL         = 330,
  SFR_KEY_KP_DIVIDE          = 331,
  SFR_KEY_KP_MULTIPLY        = 332,
  SFR_KEY_KP_SUBTRACT        = 333,
  SFR_KEY_KP_ADD             = 334,
  SFR_KEY_KP_ENTER           = 335,
  SFR_KEY_KP_EQUAL           = 336,
  SFR_KEY_LEFT_SHIFT         = 340,
  SFR_KEY_LEFT_CONTROL       = 341,
  SFR_KEY_LEFT_ALT           = 342,
  SFR_KEY_LEFT_SUPER         = 343,
  SFR_KEY_RIGHT_SHIFT        = 344,
  SFR_KEY_RIGHT_CONTROL      = 345,
  SFR_KEY_RIGHT_ALT          = 346,
  SFR_KEY_RIGHT_SUPER        = 347,
  SFR_KEY_MENU               = 348,

  SFR_KEY_LAST               = SFR_KEY_MENU
};

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */
enum SFRkey_mod_binding {
  /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
  SFR_MOD_SHIFT           = 0x000,
  /*! @brief If this bit is set one or more Control keys were held down.
  *
  *  If this bit is set one or more Control keys were held down.
  */
  SFR_MOD_CONTROL         = 0x0002,
  /*! @brief If this bit is set one or more Alt keys were held down.
  *
  *  If this bit is set one or more Alt keys were held down.
  */
  SFR_MOD_ALT             = 0x0004,
  /*! @brief If this bit is set one or more Super keys were held down.
  *
  *  If this bit is set one or more Super keys were held down.
  */
  SFR_MOD_SUPER           = 0x0008,
  /*! @brief If this bit is set the Caps Lock key is enabled.
  *
  *  If this bit is set the Caps Lock key is enabled and the @ref
  *  SFR_LOCK_KEY_MODS input mode is set.
  */
  SFR_MOD_CAPS_LOCK       = 0x0010,
  /*! @brief If this bit is set the Num Lock key is enabled.
  *
  *  If this bit is set the Num Lock key is enabled and the @ref
  *  SFR_LOCK_KEY_MODS input mode is set.
  */
  SFR_MOD_NUM_LOCK        = 0x0020
};

enum SFRmouse_binding {
  SFR_MOUSE_BUTTON_1         = 0,
  SFR_MOUSE_BUTTON_2         = 1,
  SFR_MOUSE_BUTTON_3         = 2,
  SFR_MOUSE_BUTTON_4         = 3,
  SFR_MOUSE_BUTTON_5         = 4,
  SFR_MOUSE_BUTTON_6         = 5,
  SFR_MOUSE_BUTTON_7         = 6,
  SFR_MOUSE_BUTTON_8         = 7,
  SFR_MOUSE_BUTTON_LAST      = SFR_MOUSE_BUTTON_8,
  SFR_MOUSE_BUTTON_LEFT      = SFR_MOUSE_BUTTON_1,
  SFR_MOUSE_BUTTON_RIGHT     = SFR_MOUSE_BUTTON_2,
  SFR_MOUSE_BUTTON_MIDDLE    = SFR_MOUSE_BUTTON_3
/*! @} */
};

/*! @defgroup joysticks Joysticks
 *  @brief Joystick IDs.
 *
 *  See [joystick input](@ref joystick) for how these are used.
 *
 *  @ingroup input
 *  @{ */

enum SFRgamepad_binding {
  SFR_JOYSTICK_1             = 0,
  SFR_JOYSTICK_2             = 1,
  SFR_JOYSTICK_3             = 2,
  SFR_JOYSTICK_4             = 3,
  SFR_JOYSTICK_5             = 4,
  SFR_JOYSTICK_6             = 5,
  SFR_JOYSTICK_7             = 6,
  SFR_JOYSTICK_8             = 7,
  SFR_JOYSTICK_9             = 8,
  SFR_JOYSTICK_10            = 9,
  SFR_JOYSTICK_11            = 10,
  SFR_JOYSTICK_12            = 11,
  SFR_JOYSTICK_13            = 12,
  SFR_JOYSTICK_14            = 13,
  SFR_JOYSTICK_15            = 14,
  SFR_JOYSTICK_16            = 15,
  SFR_JOYSTICK_LAST          = SFR_JOYSTICK_16,

  SFR_GAMEPAD_BUTTON_A               = 0,
  SFR_GAMEPAD_BUTTON_B               = 1,
  SFR_GAMEPAD_BUTTON_X               = 2,
  SFR_GAMEPAD_BUTTON_Y               = 3,
  SFR_GAMEPAD_BUTTON_LEFT_BUMPER     = 4,
  SFR_GAMEPAD_BUTTON_RIGHT_BUMPER    = 5,
  SFR_GAMEPAD_BUTTON_BACK            = 6,
  SFR_GAMEPAD_BUTTON_START           = 7,
  SFR_GAMEPAD_BUTTON_GUIDE           = 8,
  SFR_GAMEPAD_BUTTON_LEFT_THUMB      = 9,
  SFR_GAMEPAD_BUTTON_RIGHT_THUMB     = 10,
  SFR_GAMEPAD_BUTTON_DPAD_UP         = 11,
  SFR_GAMEPAD_BUTTON_DPAD_RIGHT      = 12,
  SFR_GAMEPAD_BUTTON_DPAD_DOWN       = 13,
  SFR_GAMEPAD_BUTTON_DPAD_LEFT       = 14,
  SFR_GAMEPAD_BUTTON_LAST            = SFR_GAMEPAD_BUTTON_DPAD_LEFT,

  SFR_GAMEPAD_BUTTON_CROSS       = SFR_GAMEPAD_BUTTON_A,
  SFR_GAMEPAD_BUTTON_CIRCLE      = SFR_GAMEPAD_BUTTON_B,
  SFR_GAMEPAD_BUTTON_SQUARE      = SFR_GAMEPAD_BUTTON_X,
  SFR_GAMEPAD_BUTTON_TRIANGLE    = SFR_GAMEPAD_BUTTON_Y,
  /*! @} */

  /*! @defgroup gamepad_axes Gamepad axes
  *  @brief Gamepad axes.
  *
  *  See @ref gamepad for how these are used.
  *
  *  @ingroup input
  *  @{ */
  SFR_GAMEPAD_AXIS_LEFT_X        = 0,
  SFR_GAMEPAD_AXIS_LEFT_Y        = 1,
  SFR_GAMEPAD_AXIS_RIGHT_X       = 2,
  SFR_GAMEPAD_AXIS_RIGHT_Y       = 3,
  SFR_GAMEPAD_AXIS_LEFT_TRIGGER  = 4,
  SFR_GAMEPAD_AXIS_RIGHT_TRIGGER = 5,
  SFR_GAMEPAD_AXIS_LAST          = SFR_GAMEPAD_AXIS_RIGHT_TRIGGER
};

#endif // __SAFIRE_KEY_BINDINGS_H__