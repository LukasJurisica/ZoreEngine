#pragma once

/*


// Digits
#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39

// Letters
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4A
#define KEY_K 0x4B
#define KEY_L 0x4C
#define KEY_M 0x4D
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5A

// Numpad
#define KEY_NUMPAD0 0x60
#define KEY_NUMPAD1 0x61
#define KEY_NUMPAD2 0x62
#define KEY_NUMPAD3 0x63
#define KEY_NUMPAD4 0x64
#define KEY_NUMPAD5 0x65
#define KEY_NUMPAD6 0x66
#define KEY_NUMPAD7 0x67
#define KEY_NUMPAD8 0x68
#define KEY_NUMPAD9 0x69

// Math Operations
#define KEY_MULTIPLY  0x6A
#define KEY_ADD       0x6B
#define KEY_SEPARATOR 0x6C
#define KEY_SUBTRACT  0x6D
#define KEY_DECIMAL   0x6E
#define KEY_DIVIDE    0x6F

// Function Keys
#define KEY_F1  0x122
#define KEY_F2  0x123
#define KEY_F3  0x124
#define KEY_F4  0x125
#define KEY_F5  0x126
#define KEY_F6  0x127
#define KEY_F7  0x128
#define KEY_F8  0x129
#define KEY_F9  0x12A
#define KEY_F10 0x12B
#define KEY_F11 0x12C
#define KEY_F12 0x12D
// Advanced Function Keys (NOT COMPLETE)
#define KEY_F13 0x7C
#define KEY_F14 0x7D
#define KEY_F15 0x7E
#define KEY_F16 0x7F
#define KEY_F17 0x80
#define KEY_F18 0x81
#define KEY_F19 0x82
#define KEY_F20 0x83
#define KEY_F21 0x84
#define KEY_F22 0x85
#define KEY_F23 0x86
#define KEY_F24 0x87

// MISC
#define KEY_SPACE  0x20

#define KEY_ENTER 257
#define KEY_TAB 258
#define KEY_CAPSLOCK 280

#define KEY_L_SHIFT 0x154
#define KEY_L_CTRL  0x155
#define KEY_L_ALT   0x156
#define KEY_R_SHIFT 0x158
#define KEY_R_CTRL  0x159
#define KEY_R_ALT   0x15A

#define KEY_MENU 348
#define KEY_WINDOWS 343
#define KEY_INSERT 260
#define KEY_PAGE_UP 266
#define KEY_PAGE_DOWN 267
#define KEY_HOME 268
#define KEY_END 269
#define KEY_DELETE 261

#define KEY_ARROW_LEFT 263
#define KEY_ARROW_DOWN 264
#define KEY_ARROW_UP 265
#define KEY_ARROW_RIGHT 262



#define KEY_PLUS 0xBB
#define KEY_MINUS 0xBD
#define KEY_ESC 0x1B
#define KEY_BACKSPACE 0x08
#define KEY_TAB 0x09
#define KEY_SHIFT 0x10
#define KEY_CAPSLOCK 0x14
#define KEY_LWIN 0x5B
#define KEY_RWIN 0x5C
#define KEY_APPS 0x5D
#define KEY_TILDE 0xC0

#define KEY_COUNT 0x200
*/

// Key Actions
#define KEY_INVALID            -1
#define KEY_RELEASE            0
#define KEY_PRESS              1
#define KEY_REPEAT             2

/* Printable keys */
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE              96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_ARROW_RIGHT        262
#define KEY_ARROW_LEFT         263
#define KEY_ARROW_DOWN         264
#define KEY_ARROW_UP           265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_NP_0               320
#define KEY_NP_1               321
#define KEY_NP_2               322
#define KEY_NP_3               323
#define KEY_NP_4               324
#define KEY_NP_5               325
#define KEY_NP_6               326
#define KEY_NP_7               327
#define KEY_NP_8               328
#define KEY_NP_9               329
#define KEY_NP_DECIMAL         330
#define KEY_NP_DIVIDE          331
#define KEY_NP_MULTIPLY        332
#define KEY_NP_SUBTRACT        333
#define KEY_NP_ADD             334
#define KEY_NP_ENTER           335
#define KEY_NP_EQUAL           336
#define KEY_L_SHIFT            340
#define KEY_L_CTRL             341
#define KEY_L_ALT              342
#define KEY_L_SUPER            343
#define KEY_R_SHIFT            344
#define KEY_R_CTRL             345
#define KEY_R_ALT              346
#define KEY_R_SUPER            347
#define KEY_MENU               348

#define KEY_LAST               KEY_MENU