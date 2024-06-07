#
 GLFW 3.5 - www.glfw.org
 A library for OpenGL, window and input
------------------------------------------------------------------------
 Copyright (c) 2002-2006 Marcus Geelnard
 Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would
    be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not
    be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source
    distribution.
#

GLFW_VERSION_MAJOR: [internal] int32    = 3;
GLFW_VERSION_MINOR: [internal] int32    = 5;
GLFW_VERSION_REVISION: [internal] int32 = 0;

GLFW_TRUE: [internal] int32  = 1;
GLFW_FALSE: [internal] int32 = 0;

GLFW_RELEASE: [internal] int32 = 0;
GLFW_PRESS: [internal] int32   = 1;
GLFW_REPEAT: [internal] int32  = 2;

GLFW_HAT_CENTERED: [internal] int32   = 0;
GLFW_HAT_UP: [internal] int32         = 1;
GLFW_HAT_RIGHT: [internal] int32      = 2;
GLFW_HAT_DOWN: [internal] int32       = 4;
GLFW_HAT_LEFT: [internal] int32       = 8;
GLFW_HAT_RIGHT_UP: [internal] int32   = (GLFW_HAT_RIGHT | GLFW_HAT_UP);
GLFW_HAT_RIGHT_DOWN: [internal] int32 = (GLFW_HAT_RIGHT | GLFW_HAT_DOWN);
GLFW_HAT_LEFT_UP: [internal] int32    = (GLFW_HAT_LEFT  | GLFW_HAT_UP);
GLFW_HAT_LEFT_DOWN: [internal] int32  = (GLFW_HAT_LEFT  | GLFW_HAT_DOWN);

GLFW_KEY_UNKNOWN: [internal] int32 = -1;

GLFW_KEY_SPACE: [internal] int32         = 32;
GLFW_KEY_APOSTROPHE: [internal] int32    = 39;
GLFW_KEY_COMMA: [internal] int32         = 44;
GLFW_KEY_MINUS: [internal] int32         = 45;
GLFW_KEY_PERIOD: [internal] int32        = 46;
GLFW_KEY_SLASH: [internal] int32         = 47;
GLFW_KEY_0: [internal] int32             = 48;
GLFW_KEY_1: [internal] int32             = 49;
GLFW_KEY_2: [internal] int32             = 50;
GLFW_KEY_3: [internal] int32             = 51;
GLFW_KEY_4: [internal] int32             = 52;
GLFW_KEY_5: [internal] int32             = 53;
GLFW_KEY_6: [internal] int32             = 54;
GLFW_KEY_7: [internal] int32             = 55;
GLFW_KEY_8: [internal] int32             = 56;
GLFW_KEY_9: [internal] int32             = 57;
GLFW_KEY_SEMICOLON: [internal] int32     = 59;
GLFW_KEY_EQUAL: [internal] int32         = 61;
GLFW_KEY_A: [internal] int32             = 65;
GLFW_KEY_B: [internal] int32             = 66;
GLFW_KEY_C: [internal] int32             = 67;
GLFW_KEY_D: [internal] int32             = 68;
GLFW_KEY_E: [internal] int32             = 69;
GLFW_KEY_F: [internal] int32             = 70;
GLFW_KEY_G: [internal] int32             = 71;
GLFW_KEY_H: [internal] int32             = 72;
GLFW_KEY_I: [internal] int32             = 73;
GLFW_KEY_J: [internal] int32             = 74;
GLFW_KEY_K: [internal] int32             = 75;
GLFW_KEY_L: [internal] int32             = 76;
GLFW_KEY_M: [internal] int32             = 77;
GLFW_KEY_N: [internal] int32             = 78;
GLFW_KEY_O: [internal] int32             = 79;
GLFW_KEY_P: [internal] int32             = 80;
GLFW_KEY_Q: [internal] int32             = 81;
GLFW_KEY_R: [internal] int32             = 82;
GLFW_KEY_S: [internal] int32             = 83;
GLFW_KEY_T: [internal] int32             = 84;
GLFW_KEY_U: [internal] int32             = 85;
GLFW_KEY_V: [internal] int32             = 86;
GLFW_KEY_W: [internal] int32             = 87;
GLFW_KEY_X: [internal] int32             = 88;
GLFW_KEY_Y: [internal] int32             = 89;
GLFW_KEY_Z: [internal] int32             = 90;
GLFW_KEY_LEFT_BRACKET: [internal] int32  = 91;
GLFW_KEY_BACKSLASH: [internal] int32     = 92;
GLFW_KEY_RIGHT_BRACKET: [internal] int32 = 93;
GLFW_KEY_GRAVE_ACCENT: [internal] int32  = 96;
GLFW_KEY_WORLD_1: [internal] int32       = 16;
GLFW_KEY_WORLD_2: [internal] int32       = 16;

GLFW_KEY_ESCAPE: [internal] int32        = 256;
GLFW_KEY_ENTER: [internal] int32         = 257;
GLFW_KEY_TAB: [internal] int32           = 258;
GLFW_KEY_BACKSPACE: [internal] int32     = 259;
GLFW_KEY_INSERT: [internal] int32        = 260;
GLFW_KEY_DELETE: [internal] int32        = 261;
GLFW_KEY_RIGHT: [internal] int32         = 262;
GLFW_KEY_LEFT: [internal] int32          = 263;
GLFW_KEY_DOWN: [internal] int32          = 264;
GLFW_KEY_UP: [internal] int32            = 265;
GLFW_KEY_PAGE_UP: [internal] int32       = 266;
GLFW_KEY_PAGE_DOWN: [internal] int32     = 267;
GLFW_KEY_HOME: [internal] int32          = 268;
GLFW_KEY_END: [internal] int32           = 269;
GLFW_KEY_CAPS_LOCK: [internal] int32     = 280;
GLFW_KEY_SCROLL_LOCK: [internal] int32   = 281;
GLFW_KEY_NUM_LOCK: [internal] int32      = 282;
GLFW_KEY_PRINT_SCREEN: [internal] int32  = 283;
GLFW_KEY_PAUSE: [internal] int32         = 284;
GLFW_KEY_F1: [internal] int32            = 290;
GLFW_KEY_F2: [internal] int32            = 291;
GLFW_KEY_F3: [internal] int32            = 292;
GLFW_KEY_F4: [internal] int32            = 293;
GLFW_KEY_F5: [internal] int32            = 294;
GLFW_KEY_F6: [internal] int32            = 295;
GLFW_KEY_F7: [internal] int32            = 296;
GLFW_KEY_F8: [internal] int32            = 297;
GLFW_KEY_F9: [internal] int32            = 298;
GLFW_KEY_F10: [internal] int32           = 299;
GLFW_KEY_F11: [internal] int32           = 300;
GLFW_KEY_F12: [internal] int32           = 301;
GLFW_KEY_F13: [internal] int32           = 302;
GLFW_KEY_F14: [internal] int32           = 303;
GLFW_KEY_F15: [internal] int32           = 304;
GLFW_KEY_F16: [internal] int32           = 305;
GLFW_KEY_F17: [internal] int32           = 306;
GLFW_KEY_F18: [internal] int32           = 307;
GLFW_KEY_F19: [internal] int32           = 308;
GLFW_KEY_F20: [internal] int32           = 309;
GLFW_KEY_F21: [internal] int32           = 310;
GLFW_KEY_F22: [internal] int32           = 311;
GLFW_KEY_F23: [internal] int32           = 312;
GLFW_KEY_F24: [internal] int32           = 313;
GLFW_KEY_F25: [internal] int32           = 314;
GLFW_KEY_KP_0: [internal] int32          = 320;
GLFW_KEY_KP_1: [internal] int32          = 321;
GLFW_KEY_KP_2: [internal] int32          = 322;
GLFW_KEY_KP_3: [internal] int32          = 323;
GLFW_KEY_KP_4: [internal] int32          = 324;
GLFW_KEY_KP_5: [internal] int32          = 325;
GLFW_KEY_KP_6: [internal] int32          = 326;
GLFW_KEY_KP_7: [internal] int32          = 327;
GLFW_KEY_KP_8: [internal] int32          = 328;
GLFW_KEY_KP_9: [internal] int32          = 329;
GLFW_KEY_KP_DECIMAL: [internal] int32    = 330;
GLFW_KEY_KP_DIVIDE: [internal] int32     = 331;
GLFW_KEY_KP_MULTIPLY: [internal] int32   = 332;
GLFW_KEY_KP_SUBTRACT: [internal] int32   = 333;
GLFW_KEY_KP_ADD: [internal] int32        = 334;
GLFW_KEY_KP_ENTER: [internal] int32      = 335;
GLFW_KEY_KP_EQUAL: [internal] int32      = 336;
GLFW_KEY_LEFT_SHIFT: [internal] int32    = 340;
GLFW_KEY_LEFT_CONTROL: [internal] int32  = 341;
GLFW_KEY_LEFT_ALT: [internal] int32      = 342;
GLFW_KEY_LEFT_SUPER: [internal] int32    = 343;
GLFW_KEY_RIGHT_SHIFT: [internal] int32   = 344;
GLFW_KEY_RIGHT_CONTROL: [internal] int32 = 345;
GLFW_KEY_RIGHT_ALT: [internal] int32     = 346;
GLFW_KEY_RIGHT_SUPER: [internal] int32   = 347;
GLFW_KEY_MENU: [internal] int32          = 348;

GLFW_KEY_LAST: [internal] int32 = GLFW_KEY_MENU;

GLFW_MOD_SHIFT: [internal] int32     = 0x0001;
GLFW_MOD_CONTROL: [internal] int32   = 0x0002;
GLFW_MOD_ALT: [internal] int32       = 0x0004;
GLFW_MOD_SUPER: [internal] int32     = 0x0008;
GLFW_MOD_CAPS_LOCK: [internal] int32 = 0x0010;
GLFW_MOD_NUM_LOCK: [internal] int32  = 0x0020;

GLFW_MOUSE_BUTTON_1: [internal] int32      = 0;
GLFW_MOUSE_BUTTON_2: [internal] int32      = 1;
GLFW_MOUSE_BUTTON_3: [internal] int32      = 2;
GLFW_MOUSE_BUTTON_4: [internal] int32      = 3;
GLFW_MOUSE_BUTTON_5: [internal] int32      = 4;
GLFW_MOUSE_BUTTON_6: [internal] int32      = 5;
GLFW_MOUSE_BUTTON_7: [internal] int32      = 6;
GLFW_MOUSE_BUTTON_8: [internal] int32      = 7;
GLFW_MOUSE_BUTTON_LAST: [internal] int32   = GLFW_MOUSE_BUTTON_8;
GLFW_MOUSE_BUTTON_LEFT: [internal] int32   = GLFW_MOUSE_BUTTON_1;
GLFW_MOUSE_BUTTON_RIGHT: [internal] int32  = GLFW_MOUSE_BUTTON_2;
GLFW_MOUSE_BUTTON_MIDDLE: [internal] int32 = GLFW_MOUSE_BUTTON_3;

GLFW_JOYSTICK_1: [internal] int32    = 0;
GLFW_JOYSTICK_2: [internal] int32    = 1;
GLFW_JOYSTICK_3: [internal] int32    = 2;
GLFW_JOYSTICK_4: [internal] int32    = 3;
GLFW_JOYSTICK_5: [internal] int32    = 4;
GLFW_JOYSTICK_6: [internal] int32    = 5;
GLFW_JOYSTICK_7: [internal] int32    = 6;
GLFW_JOYSTICK_8: [internal] int32    = 7;
GLFW_JOYSTICK_9: [internal] int32    = 8;
GLFW_JOYSTICK_10: [internal] int32   = 9;
GLFW_JOYSTICK_11: [internal] int32   = 10;
GLFW_JOYSTICK_12: [internal] int32   = 11;
GLFW_JOYSTICK_13: [internal] int32   = 12;
GLFW_JOYSTICK_14: [internal] int32   = 13;
GLFW_JOYSTICK_15: [internal] int32   = 14;
GLFW_JOYSTICK_16: [internal] int32   = 15;
GLFW_JOYSTICK_LAST: [internal] int32 = GLFW_JOYSTICK_16;

GLFW_GAMEPAD_BUTTON_A: [internal] int32            = 0;
GLFW_GAMEPAD_BUTTON_B: [internal] int32            = 1;
GLFW_GAMEPAD_BUTTON_X: [internal] int32            = 2;
GLFW_GAMEPAD_BUTTON_Y: [internal] int32            = 3;
GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: [internal] int32  = 4;
GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: [internal] int32 = 5;
GLFW_GAMEPAD_BUTTON_BACK: [internal] int32         = 6;
GLFW_GAMEPAD_BUTTON_START: [internal] int32        = 7;
GLFW_GAMEPAD_BUTTON_GUIDE: [internal] int32        = 8;
GLFW_GAMEPAD_BUTTON_LEFT_THUMB: [internal] int32   = 9;
GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: [internal] int32  = 10;
GLFW_GAMEPAD_BUTTON_DPAD_UP: [internal] int32      = 11;
GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: [internal] int32   = 12;
GLFW_GAMEPAD_BUTTON_DPAD_DOWN: [internal] int32    = 13;
GLFW_GAMEPAD_BUTTON_DPAD_LEFT: [internal] int32    = 14;
GLFW_GAMEPAD_BUTTON_LAST: [internal] int32         = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;

GLFW_GAMEPAD_BUTTON_CROSS: [internal] int32    = GLFW_GAMEPAD_BUTTON_A;
GLFW_GAMEPAD_BUTTON_CIRCLE: [internal] int32   = GLFW_GAMEPAD_BUTTON_B;
GLFW_GAMEPAD_BUTTON_SQUARE: [internal] int32   = GLFW_GAMEPAD_BUTTON_X;
GLFW_GAMEPAD_BUTTON_TRIANGLE: [internal] int32 = GLFW_GAMEPAD_BUTTON_Y;

GLFW_GAMEPAD_AXIS_LEFT_X: [internal] int32        = 0;
GLFW_GAMEPAD_AXIS_LEFT_Y: [internal] int32        = 1;
GLFW_GAMEPAD_AXIS_RIGHT_X: [internal] int32       = 2;
GLFW_GAMEPAD_AXIS_RIGHT_Y: [internal] int32       = 3;
GLFW_GAMEPAD_AXIS_LEFT_TRIGGER: [internal] int32  = 4;
GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: [internal] int32 = 5;
GLFW_GAMEPAD_AXIS_LAST: [internal] int32          = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;

GLFW_NO_ERROR: [internal] int32              = 0;
GLFW_NOT_INITIALIZED: [internal] int32       = 0x00010001;
GLFW_NO_CURRENT_CONTEXT: [internal] int32    = 0x00010002;
GLFW_INVALID_ENUM: [internal] int32          = 0x00010003;
GLFW_INVALID_VALUE: [internal] int32         = 0x00010004;
GLFW_OUT_OF_MEMORY: [internal] int32         = 0x00010005;
GLFW_API_UNAVAILABLE: [internal] int32       = 0x00010006;
GLFW_VERSION_UNAVAILABLE: [internal] int32   = 0x00010007;
GLFW_PLATFORM_ERROR: [internal] int32        = 0x00010008;
GLFW_FORMAT_UNAVAILABLE: [internal] int32    = 0x00010009;
GLFW_NO_WINDOW_CONTEXT: [internal] int32     = 0x0001000A;
GLFW_CURSOR_UNAVAILABLE: [internal] int32    = 0x0001000B;
GLFW_FEATURE_UNAVAILABLE: [internal] int32   = 0x0001000C;
GLFW_FEATURE_UNIMPLEMENTED: [internal] int32 = 0x0001000D;
GLFW_PLATFORM_UNAVAILABLE: [internal] int32  = 0x0001000E;

GLFW_FOCUSED: [internal] int32                 = 0x00020001;
GLFW_ICONIFIED: [internal] int32               = 0x00020002;
GLFW_RESIZABLE: [internal] int32               = 0x00020003;
GLFW_VISIBLE: [internal] int32                 = 0x00020004;
GLFW_DECORATED: [internal] int32               = 0x00020005;
GLFW_AUTO_ICONIFY: [internal] int32            = 0x00020006;
GLFW_FLOATING: [internal] int32                = 0x00020007;
GLFW_MAXIMIZED: [internal] int32               = 0x00020008;
GLFW_CENTER_CURSOR: [internal] int32           = 0x00020009;
GLFW_TRANSPARENT_FRAMEBUFFER: [internal] int32 = 0x0002000A;
GLFW_HOVERED: [internal] int32                 = 0x0002000B;
GLFW_FOCUS_ON_SHOW: [internal] int32           = 0x0002000C;
GLFW_MOUSE_PASSTHROUGH: [internal] int32       = 0x0002000D;
GLFW_POSITION_X: [internal] int32              = 0x0002000E;
GLFW_POSITION_Y: [internal] int32              = 0x0002000F;

GLFW_RED_BITS: [internal] int32         = 0x00021001;
GLFW_GREEN_BITS: [internal] int32       = 0x00021002;
GLFW_BLUE_BITS: [internal] int32        = 0x00021003;
GLFW_ALPHA_BITS: [internal] int32       = 0x00021004;
GLFW_DEPTH_BITS: [internal] int32       = 0x00021005;
GLFW_STENCIL_BITS: [internal] int32     = 0x00021006;
GLFW_ACCUM_RED_BITS: [internal] int32   = 0x00021007;
GLFW_ACCUM_GREEN_BITS: [internal] int32 = 0x00021008;
GLFW_ACCUM_BLUE_BITS: [internal] int32  = 0x00021009;
GLFW_ACCUM_ALPHA_BITS: [internal] int32 = 0x0002100A;
GLFW_AUX_BUFFERS: [internal] int32      = 0x0002100B;
GLFW_STEREO: [internal] int32           = 0x0002100C;
GLFW_SAMPLES: [internal] int32          = 0x0002100D;
GLFW_SRGB_CAPABLE: [internal] int32     = 0x0002100E;
GLFW_REFRESH_RATE: [internal] int32     = 0x0002100F;
GLFW_DOUBLEBUFFER: [internal] int32     = 0x00021010;

GLFW_CLIENT_API: [internal] int32               = 0x00022001;
GLFW_CONTEXT_VERSION_MAJOR: [internal] int32    = 0x00022002;
GLFW_CONTEXT_VERSION_MINOR: [internal] int32    = 0x00022003;
GLFW_CONTEXT_REVISION: [internal] int32         = 0x00022004;
GLFW_CONTEXT_ROBUSTNESS: [internal] int32       = 0x00022005;
GLFW_OPENGL_FORWARD_COMPAT: [internal] int32    = 0x00022006;
GLFW_CONTEXT_DEBUG: [internal] int32            = 0x00022007;
GLFW_OPENGL_DEBUG_CONTEXT: [internal] int32     = GLFW_CONTEXT_DEBUG;
GLFW_OPENGL_PROFILE: [internal] int32           = 0x00022008;
GLFW_CONTEXT_RELEASE_BEHAVIOR: [internal] int32 = 0x00022009;
GLFW_CONTEXT_NO_ERROR: [internal] int32         = 0x0002200A;
GLFW_CONTEXT_CREATION_API: [internal] int32     = 0x0002200B;
GLFW_SCALE_TO_MONITOR: [internal] int32         = 0x0002200C;
GLFW_SCALE_FRAMEBUFFER: [internal] int32        = 0x0002200D;

GLFW_COCOA_RETINA_FRAMEBUFFER: [internal] int32 = 0x00023001;
GLFW_COCOA_FRAME_NAME: [internal] int32         = 0x00023002;
GLFW_COCOA_GRAPHICS_SWITCHING: [internal] int32 = 0x00023003;

GLFW_X11_CLASS_NAME: [internal] int32    = 0x00024001;
GLFW_X11_INSTANCE_NAME: [internal] int32 = 0x00024002;

GLFW_WIN32_KEYBOARD_MENU: [internal] int32 = 0x00025001;
GLFW_WIN32_SHOWDEFAULT: [internal] int32   = 0x00025002;

GLFW_WAYLAND_APP_ID: [internal] int32 = 0x00026001;

GLFW_NO_API: [internal] int32        =          0;
GLFW_OPENGL_API: [internal] int32    = 0x00030001;
GLFW_OPENGL_ES_API: [internal] int32 = 0x00030002;

GLFW_NO_ROBUSTNESS: [internal] int32         =          0;
GLFW_NO_RESET_NOTIFICATION: [internal] int32 = 0x00031001;
GLFW_LOSE_CONTEXT_ON_RESET: [internal] int32 = 0x00031002;

GLFW_OPENGL_ANY_PROFILE: [internal] int32    =          0;
GLFW_OPENGL_CORE_PROFILE: [internal] int32   = 0x00032001;
GLFW_OPENGL_COMPAT_PROFILE: [internal] int32 = 0x00032002;

GLFW_CURSOR: [internal] int32                  = 0x00033001;
GLFW_STICKY_KEYS: [internal] int32             = 0x00033002;
GLFW_STICKY_MOUSE_BUTTONS: [internal] int32    = 0x00033003;
GLFW_LOCK_KEY_MODS: [internal] int32           = 0x00033004;
GLFW_RAW_MOUSE_MOTION: [internal] int32        = 0x00033005;
GLFW_UNLIMITED_MOUSE_BUTTONS: [internal] int32 = 0x00033006;

GLFW_CURSOR_NORMAL: [internal] int32   = 0x00034001;
GLFW_CURSOR_HIDDEN: [internal] int32   = 0x00034002;
GLFW_CURSOR_DISABLED: [internal] int32 = 0x00034003;
GLFW_CURSOR_CAPTURED: [internal] int32 = 0x00034004;

GLFW_ANY_RELEASE_BEHAVIOR: [internal] int32   =          0;
GLFW_RELEASE_BEHAVIOR_FLUSH: [internal] int32 = 0x00035001;
GLFW_RELEASE_BEHAVIOR_NONE: [internal] int32  = 0x00035002;

GLFW_NATIVE_CONTEXT_API: [internal] int32 = 0x00036001;
GLFW_EGL_CONTEXT_API: [internal] int32    = 0x00036002;
GLFW_OSMESA_CONTEXT_API: [internal] int32 = 0x00036003;

GLFW_ANGLE_PLATFORM_TYPE_NONE: [internal] int32     = 0x00037001;
GLFW_ANGLE_PLATFORM_TYPE_OPENGL: [internal] int32   = 0x00037002;
GLFW_ANGLE_PLATFORM_TYPE_OPENGLES: [internal] int32 = 0x00037003;
GLFW_ANGLE_PLATFORM_TYPE_D3D9: [internal] int32     = 0x00037004;
GLFW_ANGLE_PLATFORM_TYPE_D3D11: [internal] int32    = 0x00037005;
GLFW_ANGLE_PLATFORM_TYPE_VULKAN: [internal] int32   = 0x00037007;
GLFW_ANGLE_PLATFORM_TYPE_METAL: [internal] int32    = 0x00037008;

GLFW_WAYLAND_PREFER_LIBDECOR: [internal] int32  = 0x00038001;
GLFW_WAYLAND_DISABLE_LIBDECOR: [internal] int32 = 0x00038002;

GLFW_ANY_POSITION: [internal] int32 = 0x80000000;

GLFW_ARROW_CURSOR: [internal] int32         = 0x00036001;
GLFW_IBEAM_CURSOR: [internal] int32         = 0x00036002;
GLFW_CROSSHAIR_CURSOR: [internal] int32     = 0x00036003;
GLFW_POINTING_HAND_CURSOR: [internal] int32 = 0x00036004;
GLFW_RESIZE_EW_CURSOR: [internal] int32     = 0x00036005;
GLFW_RESIZE_NS_CURSOR: [internal] int32     = 0x00036006;
GLFW_RESIZE_NWSE_CURSOR: [internal] int32   = 0x00036007;
GLFW_RESIZE_NESW_CURSOR: [internal] int32   = 0x00036008;
GLFW_RESIZE_ALL_CURSOR: [internal] int32    = 0x00036009;
GLFW_NOT_ALLOWED_CURSOR: [internal] int32   = 0x0003600A;
GLFW_HRESIZE_CURSOR: [internal] int32       = GLFW_RESIZE_EW_CURSOR;
GLFW_VRESIZE_CURSOR: [internal] int32       = GLFW_RESIZE_NS_CURSOR;
GLFW_HAND_CURSOR: [internal] int32          = GLFW_POINTING_HAND_CURSOR;

GLFW_CONNECTED: [internal] int32    = 0x00040001;
GLFW_DISCONNECTED: [internal] int32 = 0x00040002;

GLFW_JOYSTICK_HAT_BUTTONS: [internal] int32   = 0x00050001;
GLFW_ANGLE_PLATFORM_TYPE: [internal] int32    = 0x00050002;
GLFW_PLATFORM: [internal] int32               = 0x00050003;
GLFW_COCOA_CHDIR_RESOURCES: [internal] int32  = 0x00051001;
GLFW_COCOA_MENUBAR: [internal] int32          = 0x00051002;
GLFW_X11_XCB_VULKAN_SURFACE: [internal] int32 = 0x00052001;
GLFW_WAYLAND_LIBDECOR: [internal] int32       = 0x00053001;

GLFW_ANY_PLATFORM: [internal] int32     = 0x00060000;
GLFW_PLATFORM_WIN32: [internal] int32   = 0x00060001;
GLFW_PLATFORM_COCOA: [internal] int32   = 0x00060002;
GLFW_PLATFORM_WAYLAND: [internal] int32 = 0x00060003;
GLFW_PLATFORM_X11: [internal] int32     = 0x00060004;
GLFW_PLATFORM_NULL: [internal] int32    = 0x00060005;

GLFW_DONT_CARE: [internal] int32 = -1;

def GLFWglproc = ()(void)*;
def GLFWvkproc = ()(void)*;
def GLFWmonitor;
def GLFWwindow;
def GLFWcursor;
def GLFWallocatefun = (size: size_t, user: void*)(void*)*;
def GLFWreallocatefun = (block: void*, size: size_t, user: void*)(void*)*;
def GLFWdeallocatefun = (block: void*, user: void*)(void)*;
def GLFWerrorfun = (error_code: int32, description: int8*)(void)*;
def GLFWwindowposfun = (window: GLFWwindow*, xpos: int32, ypos: int32)(void)*;
def GLFWwindowsizefun = (window: GLFWwindow*, width: int32, height: int32)(void)*;
def GLFWwindowclosefun = (window: GLFWwindow*)(void)*;
def GLFWwindowrefreshfun = (window: GLFWwindow*)(void)*;
def GLFWwindowfocusfun = (window: GLFWwindow*, focused: int32)(void)*;
def GLFWwindowiconifyfun = (window: GLFWwindow*, iconified: int32)(void)*;
def GLFWwindowmaximizefun = (window: GLFWwindow*, maximized: int32)(void)*;
def GLFWframebuffersizefun = (window: GLFWwindow*, width: int32, height: int32)(void)*;
def GLFWwindowcontentscalefun = (window: GLFWwindow*, xscale: flt32, yscale: flt32)(void)*;
def GLFWmousebuttonfun = (window: GLFWwindow*, button: int32, action: int32, mods: int32)(void)*;
def GLFWcursorposfun = (window: GLFWwindow*, xpos: flt64, ypos: flt64)(void)*;
def GLFWcursorenterfun = (window: GLFWwindow*, entered: int32)(void)*;
def GLFWscrollfun = (window: GLFWwindow*, xoffset: flt64, yoffset: flt64)(void)*;
def GLFWkeyfun = (window: GLFWwindow*, key: int32, scancode: int32, action: int32, mods: int32)(void)*;
def GLFWcharfun = (window: GLFWwindow*, codepoint: int32)(void)*;
def GLFWcharmodsfun = (window: GLFWwindow*, codepoint: int32, mods: int32)(void)*;
def GLFWdropfun = (window: GLFWwindow*, path_count: int32, paths: int8**)(void)*;
def GLFWmonitorfun = (monitor: GLFWmonitor*, event: int32)(void)*;
def GLFWjoystickfun = (jid: int32, event: int32)(void)*;

def PFN_vkGetInstanceProcAddr;
def VkInstance;
def VkPhysicalDevice;
def VkAllocationCallbacks;
def VkSurfaceKHR;
def VkResult;

def GLFWvidmode {
    width: int32,
    height: int32,
    redBits: int32,
    greenBits: int32,
    blueBits: int32,
    refreshRate: int32,
}

def GLFWgammaramp {
    red: int16*,
    green: int16*,
    blue: int16*,
    size: int32,
}

def GLFWimage {
    width: int32,
    height: int32,
    pixels: int8*,
}

def GLFWgamepadstate {
    buttons: int8[15],
    axes: flt32[6],
}

def GLFWallocator {
    allocate: GLFWallocatefun,
    reallocate: GLFWreallocatefun,
    deallocate: GLFWdeallocatefun,
    user: void*,
}

@[c]glfwInit::int32;
@[c]glfwTerminate::void;
@[c]glfwInitHint(hint: int32, value: int32): void;
@[c]glfwInitAllocator(allocator: GLFWallocator*): void;
@[c]glfwInitVulkanLoader(loader: PFN_vkGetInstanceProcAddr): void;
@[c]glfwGetVersion(major: int32*, minor: int32*, rev: int32*): void;
@[c]glfwGetVersionString::int8*;
@[c]glfwGetError(description: int8**): int32;
@[c]glfwSetErrorCallback(callback: GLFWerrorfun): GLFWerrorfun;
@[c]glfwGetPlatform::int32;
@[c]glfwPlatformSupported(platform: int32): int32;
@[c]glfwGetMonitors(count: int32*): GLFWmonitor**;
@[c]glfwGetPrimaryMonitor::GLFWmonitor*;
@[c]glfwGetMonitorPos(monitor: GLFWmonitor*, xpos: int32*, ypos: int32*): void;
@[c]glfwGetMonitorWorkarea(monitor: GLFWmonitor*, xpos: int32*, ypos: int32*, width: int32*, height: int32*): void;
@[c]glfwGetMonitorPhysicalSize(monitor: GLFWmonitor*, widthMM: int32*, heightMM: int32*): void;
@[c]glfwGetMonitorContentScale(monitor: GLFWmonitor*, xscale: flt32*, yscale: flt32*): void;
@[c]glfwGetMonitorName(monitor: GLFWmonitor*): int8*;
@[c]glfwSetMonitorUserPointer(monitor: GLFWmonitor*, pointer: void*): void;
@[c]glfwGetMonitorUserPointer(monitor: GLFWmonitor*): void*;
@[c]glfwSetMonitorCallback(callback: GLFWmonitorfun): GLFWmonitorfun;
@[c]glfwGetVideoModes(monitor: GLFWmonitor*, count: int32*): GLFWvidmode*;
@[c]glfwGetVideoMode(monitor: GLFWmonitor*): GLFWvidmode*;
@[c]glfwSetGamma(monitor: GLFWmonitor*, gamma: flt32): void;
@[c]glfwGetGammaRamp(monitor: GLFWmonitor*): GLFWgammaramp*;
@[c]glfwSetGammaRamp(monitor: GLFWmonitor*, ramp: GLFWgammaramp*): void;
@[c]glfwDefaultWindowHints::void;
@[c]glfwWindowHint(hint: int32, value: int32): void;
@[c]glfwWindowHintString(hint: int32, value: int8*): void;
@[c]glfwCreateWindow(width: int32, height: int32, title: int8*, monitor: GLFWmonitor*, share: GLFWwindow*): GLFWwindow*;
@[c]glfwDestroyWindow(window: GLFWwindow*): void;
@[c]glfwWindowShouldClose(window: GLFWwindow*): int32;
@[c]glfwSetWindowShouldClose(window: GLFWwindow*, value: int32): void;
@[c]glfwGetWindowTitle(window: GLFWwindow*): int8*;
@[c]glfwSetWindowTitle(window: GLFWwindow*, title: int8*): void;
@[c]glfwSetWindowIcon(window: GLFWwindow*, count: int32, images: GLFWimage*): void;
@[c]glfwGetWindowPos(window: GLFWwindow*, xpos: int32*, ypos: int32*): void;
@[c]glfwSetWindowPos(window: GLFWwindow*, xpos: int32, ypos: int32): void;
@[c]glfwGetWindowSize(window: GLFWwindow*, width: int32*, height: int32*): void;
@[c]glfwSetWindowSizeLimits(window: GLFWwindow*, minwidth: int32, minheight: int32, maxwidth: int32, maxheight: int32): void;
@[c]glfwSetWindowAspectRatio(window: GLFWwindow*, numer: int32, denom: int32): void;
@[c]glfwSetWindowSize(window: GLFWwindow*, width: int32, height: int32): void;
@[c]glfwGetFramebufferSize(window: GLFWwindow*, width: int32*, height: int32*): void;
@[c]glfwGetWindowFrameSize(window: GLFWwindow*, left: int32*, top: int32*, right: int32*, bottom: int32*): void;
@[c]glfwGetWindowContentScale(window: GLFWwindow*, xscale: flt32*, yscale: flt32*): void;
@[c]glfwGetWindowOpacity(window: GLFWwindow*): flt32;
@[c]glfwSetWindowOpacity(window: GLFWwindow*, opacity: flt32): void;
@[c]glfwIconifyWindow(window: GLFWwindow*): void;
@[c]glfwRestoreWindow(window: GLFWwindow*): void;
@[c]glfwMaximizeWindow(window: GLFWwindow*): void;
@[c]glfwShowWindow(window: GLFWwindow*): void;
@[c]glfwHideWindow(window: GLFWwindow*): void;
@[c]glfwFocusWindow(window: GLFWwindow*): void;
@[c]glfwRequestWindowAttention(window: GLFWwindow*): void;
@[c]glfwGetWindowMonitor(window: GLFWwindow*): GLFWmonitor*;
@[c]glfwSetWindowMonitor(window: GLFWwindow*, monitor: GLFWmonitor*, xpos: int32, ypos: int32, width: int32, height: int32, refreshRate: int32): void;
@[c]glfwGetWindowAttrib(window: GLFWwindow*, attrib: int32): int32;
@[c]glfwSetWindowAttrib(window: GLFWwindow*, attrib: int32, value: int32): void;
@[c]glfwSetWindowUserPointer(window: GLFWwindow*, pointer: void*): void;
@[c]glfwGetWindowUserPointer(window: GLFWwindow*): void*;
@[c]glfwSetWindowPosCallback(window: GLFWwindow*, callback: GLFWwindowposfun): GLFWwindowposfun;
@[c]glfwSetWindowSizeCallback(window: GLFWwindow*, callback: GLFWwindowsizefun): GLFWwindowsizefun;
@[c]glfwSetWindowCloseCallback(window: GLFWwindow*, callback: GLFWwindowclosefun): GLFWwindowclosefun;
@[c]glfwSetWindowRefreshCallback(window: GLFWwindow*, callback: GLFWwindowrefreshfun): GLFWwindowrefreshfun;
@[c]glfwSetWindowFocusCallback(window: GLFWwindow*, callback: GLFWwindowfocusfun): GLFWwindowfocusfun;
@[c]glfwSetWindowIconifyCallback(window: GLFWwindow*, callback: GLFWwindowiconifyfun): GLFWwindowiconifyfun;
@[c]glfwSetWindowMaximizeCallback(window: GLFWwindow*, callback: GLFWwindowmaximizefun): GLFWwindowmaximizefun;
@[c]glfwSetFramebufferSizeCallback(window: GLFWwindow*, callback: GLFWframebuffersizefun): GLFWframebuffersizefun;
@[c]glfwSetWindowContentScaleCallback(window: GLFWwindow*, callback: GLFWwindowcontentscalefun): GLFWwindowcontentscalefun;
@[c]glfwPollEvents::void;
@[c]glfwWaitEvents::void;
@[c]glfwWaitEventsTimeout(timeout: flt64): void;
@[c]glfwPostEmptyEvent::void;
@[c]glfwGetInputMode(window: GLFWwindow*, mode: int32): int32;
@[c]glfwSetInputMode(window: GLFWwindow*, mode: int32, value: int32): void;
@[c]glfwRawMouseMotionSupported::int32;
@[c]glfwGetKeyName(key: int32, scancode: int32): int8*;
@[c]glfwGetKeyScancode(key: int32): int32;
@[c]glfwGetKey(window: GLFWwindow*, key: int32): int32;
@[c]glfwGetMouseButton(window: GLFWwindow*, button: int32): int32;
@[c]glfwGetCursorPos(window: GLFWwindow*, xpos: flt64*, ypos: flt64*): void;
@[c]glfwSetCursorPos(window: GLFWwindow*, xpos: flt64, ypos: flt64): void;
@[c]glfwCreateCursor(image: GLFWimage*, xhot: int32, yhot: int32): GLFWcursor*;
@[c]glfwCreateStandardCursor(shape: int32): GLFWcursor*;
@[c]glfwDestroyCursor(cursor: GLFWcursor*): void;
@[c]glfwSetCursor(window: GLFWwindow*, cursor: GLFWcursor*): void;
@[c]glfwSetKeyCallback(window: GLFWwindow*, callback: GLFWkeyfun): GLFWkeyfun;
@[c]glfwSetCharCallback(window: GLFWwindow*, callback: GLFWcharfun): GLFWcharfun;
@[c]glfwSetCharModsCallback(window: GLFWwindow*, callback: GLFWcharmodsfun): GLFWcharmodsfun;
@[c]glfwSetMouseButtonCallback(window: GLFWwindow*, callback: GLFWmousebuttonfun): GLFWmousebuttonfun;
@[c]glfwSetCursorPosCallback(window: GLFWwindow*, callback: GLFWcursorposfun): GLFWcursorposfun;
@[c]glfwSetCursorEnterCallback(window: GLFWwindow*, callback: GLFWcursorenterfun): GLFWcursorenterfun;
@[c]glfwSetScrollCallback(window: GLFWwindow*, callback: GLFWscrollfun): GLFWscrollfun;
@[c]glfwSetDropCallback(window: GLFWwindow*, callback: GLFWdropfun): GLFWdropfun;
@[c]glfwJoystickPresent(jid: int32): int32;
@[c]glfwGetJoystickAxes(jid: int32, count: int32*): flt32*;
@[c]glfwGetJoystickButtons(jid: int32, count: int32*): int8*;
@[c]glfwGetJoystickHats(jid: int32, count: int32*): int8*;
@[c]glfwGetJoystickName(jid: int32): int8*;
@[c]glfwGetJoystickGUID(jid: int32): int8*;
@[c]glfwSetJoystickUserPointer(jid: int32, pointer: void*): void;
@[c]glfwGetJoystickUserPointer(jid: int32): void*;
@[c]glfwJoystickIsGamepad(jid: int32): int32;
@[c]glfwSetJoystickCallback(callback: GLFWjoystickfun): GLFWjoystickfun;
@[c]glfwUpdateGamepadMappings(string: int8*): int32;
@[c]glfwGetGamepadName(jid: int32): int8*;
@[c]glfwGetGamepadState(jid: int32, state: GLFWgamepadstate*): int32;
@[c]glfwSetClipboardString(window: GLFWwindow*, string: int8*): void;
@[c]glfwGetClipboardString(window: GLFWwindow*): int8*;
@[c]glfwGetTime::flt64;
@[c]glfwSetTime(time: flt64): void;
@[c]glfwGetTimerValue::int64;
@[c]glfwGetTimerFrequency::int64;
@[c]glfwMakeContextCurrent(window: GLFWwindow*): void;
@[c]glfwGetCurrentContext::GLFWwindow*;
@[c]glfwSwapBuffers(window: GLFWwindow*): void;
@[c]glfwSwapInterval(interval: int32): void;
@[c]glfwExtensionSupported(extension: int8*): int32;
@[c]glfwGetProcAddress(procname: int8*): GLFWglproc;
@[c]glfwVulkanSupported::int32;
@[c]glfwGetRequiredInstanceExtensions(count: int32*): int8**;
@[c]glfwGetInstanceProcAddress(instance: VkInstance, procname: int8*): GLFWvkproc;
@[c]glfwGetPhysicalDevicePresentationSupport(instance: VkInstance, device: VkPhysicalDevice, queuefamily: int32): int32;
@[c]glfwCreateWindowSurface(instance: VkInstance, window: GLFWwindow*, allocator: VkAllocationCallbacks*, surface: VkSurfaceKHR*): VkResult;
