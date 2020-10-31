
/***********************************************************************************************
 * OpenGL Window and Animation Library (formerly the Open Art Engine)                          *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "ListItem.h"
#include "LinkedList.h"
#include "Display.h"
#include "Crayon.h"

extern char codeToChar[256];
extern UCHAR codeToUnichar[256];


enum Keys {
 ESC=DIK_ESCAPE,//          0x01
 Key_1=DIK_1,//               0x02
 Key_2=DIK_2,//               0x03
 Key_3=DIK_3,//               0x04
 Key_4=DIK_4,//               0x05
 Key_5=DIK_5,//               0x06
 Key_6=DIK_6,//               0x07
 Key_7=DIK_7,//               0x08
 Key_8=DIK_8,//               0x09
 Key_9=DIK_9,//               0x0A
 Key_0=DIK_0,//               0x0B
 Minus=DIK_MINUS,//           0x0C    /* - on main keyboard */
 Equals=DIK_EQUALS,//          0x0D
 Backspace=DIK_BACK,//            0x0E    /* backspace */
 Tab=DIK_TAB,//             0x0F
 Q=DIK_Q,//               0x10
 W=DIK_W,//               0x11
 E=DIK_E,//               0x12
 R=DIK_R,//               0x13
 T=DIK_T,//               0x14
 Y=DIK_Y,//               0x15
 U=DIK_U,//               0x16
 I=DIK_I,//               0x17
 O=DIK_O,//               0x18
 P=DIK_P,//               0x19
 LBracket=DIK_LBRACKET,//        0x1A
 RBracket=DIK_RBRACKET,//        0x1B
 Enter=DIK_RETURN,//          0x1C    /* Enter on main keyboard */
 LCtrl=DIK_LCONTROL,//        0x1D
 A=DIK_A,//               0x1E
 S=DIK_S,//               0x1F
 D=DIK_D,//               0x20
 F=DIK_F,//               0x21
 G=DIK_G,//               0x22
 H=DIK_H,//               0x23
 J=DIK_J,//               0x24
 K=DIK_K,//               0x25
 L=DIK_L,//               0x26
 Semicolon=DIK_SEMICOLON,//       0x27
 Apostrophe=DIK_APOSTROPHE,//      0x28
 Grave=DIK_GRAVE,//           0x29    /* accent grave */
 LShift=DIK_LSHIFT,//          0x2A
 Backslash=DIK_BACKSLASH,//       0x2B
 Z=DIK_Z,//               0x2C
 X=DIK_X,//               0x2D
 C=DIK_C,//               0x2E
 V=DIK_V,//               0x2F
 B=DIK_B,//               0x30
 N=DIK_N,//               0x31
 M=DIK_M,//               0x32
 Comma=DIK_COMMA,//           0x33
 Period=DIK_PERIOD,//          0x34    /* . on main keyboard */
 Slash=DIK_SLASH,//           0x35    /* / on main keyboard */
 RShift=DIK_RSHIFT,//          0x36
 NumpadMultiply=DIK_MULTIPLY,//        0x37    /* * on numeric keypad */
 LAlt=DIK_LMENU,//           0x38    /* left Alt */
 Spacebar=DIK_SPACE,//           0x39
 CAPSLOCK=DIK_CAPITAL,//         0x3A
 F1=DIK_F1,//              0x3B
 F2=DIK_F2,//              0x3C
 F3=DIK_F3,//              0x3D
 F4=DIK_F4,//              0x3E
 F5=DIK_F5,//              0x3F
 F6=DIK_F6,//              0x40
 F7=DIK_F7,//              0x41
 F8=DIK_F8,//              0x42
 F9=DIK_F9,//              0x43
 F10=DIK_F10,//             0x44
 NUMLOCK=DIK_NUMLOCK,//         0x45
 SCROLLLOCK=DIK_SCROLL,//          0x46    /* Scroll Lock */
 Numpad7=DIK_NUMPAD7,//         0x47
 Numpad8=DIK_NUMPAD8,//         0x48
 Numpad9=DIK_NUMPAD9,//         0x49
 NumpadMinus=DIK_SUBTRACT,//        0x4A    /* - on numeric keypad */
 Numpad4=DIK_NUMPAD4,//         0x4B
 Numpad5=DIK_NUMPAD5,//         0x4C
 Numpad6=DIK_NUMPAD6,//         0x4D
 NumpadPlus=DIK_ADD,//             0x4E    /* + on numeric keypad */
 Numpad1=DIK_NUMPAD1,//         0x4F
 Numpad2=DIK_NUMPAD2,//         0x50
 Numpad3=DIK_NUMPAD3,//         0x51
 Numpad0=DIK_NUMPAD0,//         0x52
 NumpadDecimal=DIK_DECIMAL,//         0x53    /* . on numeric keypad */
 RT102=DIK_OEM_102,//         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
 F11=DIK_F11,//             0x57
 F12=DIK_F12,//             0x58
 F13=DIK_F13,//             0x64    /*                     (NEC PC98) */
 F14=DIK_F14,//             0x65    /*                     (NEC PC98) */
 F15=DIK_F15,//             0x66    /*                     (NEC PC98) */
 Kana=DIK_KANA,//            0x70    /* (Japanese keyboard)            */
 ABNT_C1=DIK_ABNT_C1,//         0x73    /* /? on Brazilian keyboard */
 Convert=DIK_CONVERT,//         0x79    /* (Japanese keyboard)            */
 NoConvert=DIK_NOCONVERT,//       0x7B    /* (Japanese keyboard)            */
 Yen=DIK_YEN,//             0x7D    /* (Japanese keyboard)            */
 ABNT_C2=DIK_ABNT_C2,//         0x7E    /* Numpad . on Brazilian keyboard */
 NumpadEquals=DIK_NUMPADEQUALS,//    0x8D    /* = on numeric keypad (NEC PC98) */
 PrevTrackOrCIRCUMFLEX=DIK_PREVTRACK,//       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
 At=DIK_AT,//              0x91    /*                     (NEC PC98) */
 Colon=DIK_COLON,//           0x92    /*                     (NEC PC98) */
 Underline=DIK_UNDERLINE,//       0x93    /*                     (NEC PC98) */
 Kanji=DIK_KANJI,//           0x94    /* (Japanese keyboard)            */
 Stop=DIK_STOP,//            0x95    /*                     (NEC PC98) */
 AX=DIK_AX,//              0x96    /*                     (Japan AX) */
 Unlabeled=DIK_UNLABELED,//       0x97    /*                        (J3100) */
 NextTrack=DIK_NEXTTRACK,//       0x99    /* Next Track */
 NumpadEnter=DIK_NUMPADENTER,//     0x9C    /* Enter on numeric keypad */
 RCtrl=DIK_RCONTROL,//        0x9D
 Mute=DIK_MUTE,//            0xA0    /* Mute */
 Calculator=DIK_CALCULATOR,//      0xA1    /* Calculator */
 PlayPause=DIK_PLAYPAUSE,//       0xA2    /* Play / Pause */
 MediaStop=DIK_MEDIASTOP,//       0xA4    /* Media Stop */
 VolumeDown=DIK_VOLUMEDOWN,//      0xAE    /* Volume - */
 VolumeUp=DIK_VOLUMEUP,//        0xB0    /* Volume + */
 WebHome=DIK_WEBHOME,//         0xB2    /* Web home */
 NumpadComma=DIK_NUMPADCOMMA,//     0xB3    /* , on numeric keypad (NEC PC98) */
 NumpadDivide=DIK_DIVIDE,//          0xB5    /* / on numeric keypad */
 SysRQ=DIK_SYSRQ,//           0xB7
 RAlt=DIK_RMENU,//           0xB8    /* right Alt */
 Pause=DIK_PAUSE,//           0xC5    /* Pause */
 Home=DIK_HOME,//            0xC7    /* Home on arrow keypad */
 Up=DIK_UP,//              0xC8    /* UpArrow on arrow keypad */
 PgUp=DIK_PRIOR,//           0xC9    /* PgUp on arrow keypad */
 Left=DIK_LEFT,//            0xCB    /* LeftArrow on arrow keypad */
 Right=DIK_RIGHT,//           0xCD    /* RightArrow on arrow keypad */
 End=DIK_END,//             0xCF    /* End on arrow keypad */
 Down=DIK_DOWN,//            0xD0    /* DownArrow on arrow keypad */
 Next=DIK_NEXT,//            0xD1    /* PgDn on arrow keypad */
 Insert=DIK_INSERT,//          0xD2    /* Insert on arrow keypad */
 Delete=DIK_DELETE,//          0xD3    /* Delete on arrow keypad */
 LWin=DIK_LWIN,//            0xDB    /* Left Windows key */
 RWin=DIK_RWIN,//            0xDC    /* Right Windows key */
 Apps=DIK_APPS,//            0xDD    /* AppMenu key */
 Power=DIK_POWER,//           0xDE    /* System Power */
 SysSleep=DIK_SLEEP,//           0xDF    /* System Sleep */
 Wake=DIK_WAKE,//            0xE3    /* System Wake */
 WebSearch=DIK_WEBSEARCH,//       0xE5    /* Web Search */
 WebFavorites=DIK_WEBFAVORITES,//    0xE6    /* Web Favorites */
 WebRefresh=DIK_WEBREFRESH,//      0xE7    /* Web Refresh */
 WebStop=DIK_WEBSTOP,//         0xE8    /* Web Stop */
 WebForward=DIK_WEBFORWARD,//      0xE9    /* Web Forward */
 WebBack=DIK_WEBBACK,//         0xEA    /* Web Back */
 MyComputer=DIK_MYCOMPUTER,//      0xEB    /* My Computer */
 Mail=DIK_MAIL,//            0xEC    /* Mail */
 MediaSelect=DIK_MEDIASELECT//     0xED    /* Media Select */
};

enum Modifiers {
 NONE=0,
 LALT=1,
 LCTRL=2,
 LSHIFT=4,
 RCTRL=8,
 RALT=16,
 RSHIFT=32,
 LCTRL_AND_LALT=3,
 LCTRL_AND_LSHIFT=6,
 LCTRL_LALT_LSHIFT=7
};

class KEY : public ListItem {
public:
 UCHAR uc;
 char ch;
 int code;
 Keys key;
 KEY(void);
 KEY(int code);
 ~KEY(void);
};

class KeyList : public LinkedList {
public:
 bool InList( int code );
 bool CharInList( char ch );
 void Push ( int code ) { Prepend( new KEY(code) ); }
 KeyList(void);
 ~KeyList(void);
};

class DI_Interface
{
private:
 bool KeyInit(void);
 bool MouseInit(void);
 void Initialize(void);
public:
 HWND window;
 HINSTANCE instance;
 LPDIRECTINPUT8          input_main;               // global direct input object

 bool hasKeyboard;
 LPDIRECTINPUTDEVICE8    key_main;                 // keyboard device
 UCHAR                   key_data[256];            // key data for keyboard status
 UCHAR                   prev_key_data[256];       // key data for keyboard status

 bool hasMouse;
 LPDIRECTINPUTDEVICE8    mouse_main;               // mouse device
 int x,y,z;
 int oldx,oldy,oldz;
 float mx,my, oldmx,oldmy;
 int mxi,myi, oldmxi,oldmyi;
 int left, right, middle, wasLeft, wasRight, wasMiddle, recentLeft, recentRight, recentMiddle;
 int wheelUp, wheelDown;
 DIMOUSESTATE            mouse_data;               // mouse data for mouse status
 int doubleclickFrames;
 int doubleclickExpired;
 Display *display;
 float sensitivity;
 Crayon color;
 bool cursor;
 float size;
 bool leftHanded;
 int ofsx,ofsy,ofsx2,ofsy2;

 bool hasJoystick;
 UINT    joy_num;    // number of joys
 JOYINFO joy_info;   // joy init info
 UINT    joy_ID;     // joy ID
 JOYCAPS joy_caps;   // joy capture info
 RECT    joy_trip;   // joy trip info
 DWORD joy_xcenter, joy_ycenter;
 int joy_left, joy_right, joy_up, joy_down, joy_but1, joy_but2, joy_but3, joy_but4;


 DI_Interface(void);
 DI_Interface(HWND h, HINSTANCE i);
 ~DI_Interface(void);

 void Detect( Display *d );
 void DrawGLCursor(void);
 void Snapshot(void);
 void Detach(void);

 inline int KeyDown( int code ) { return ( key_data[code] & 0x80 ); }
 inline int KeyUp( int code ) { return (( prev_key_data[code] & 0x80 ) && !( key_data[code] & 0x80 )); }

 void SetCursorSize( float s );
};

