/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
 #pragma once

#define USE_DIRECT_KEY   1
#define USE_DIRECT_JOY   1
#define USE_DIRECT_MOUSE 1
//#define USE_LOGITECH_KIT 1
#define USE_WINTOUCH     1
#define USE_WINGESTURE   1

#pragma warning (disable:4005) // Mutes the complaint that DIRECTINPUT_VERSION is not set (even though it IS)
#define DIRECTINPUT_VERSION 0x0800
#include <Windows.h>
#include <WinUser.h>
#include <dinput.h>
#if defined(USE_LOGITECH_KIT)
#include <LgKitApi.h>
#endif
#include "ListItem.h"
#include "LinkedList.h"
#include "Display.h"
#include "Crayon.h"

#include <Xinput.h>
#include <joystickapi.h>

class GLImage;
class Film;

// Names for each axis of movement for each analog item
enum AnalogButtons {
 LeftStickLeft,
 LeftStickRight,
 LeftStickUp,
 LeftStickDown,
 RightStickLeft,
 RightStickRight,
 RightStickUp,
 RightStickDown,
 LeftTrigger,
 RightTrigger
};

enum GamepadButtons {
 pad_up=XINPUT_GAMEPAD_DPAD_UP,
 pad_down=XINPUT_GAMEPAD_DPAD_DOWN,
 pad_left=XINPUT_GAMEPAD_DPAD_LEFT,
 pad_right=XINPUT_GAMEPAD_DPAD_RIGHT,
 pad_start=XINPUT_GAMEPAD_START,
 pad_back=XINPUT_GAMEPAD_BACK,
 pad_lthumb=XINPUT_GAMEPAD_LEFT_THUMB,
 pad_rthumb=XINPUT_GAMEPAD_RIGHT_THUMB,
 pad_lshoulder=XINPUT_GAMEPAD_LEFT_SHOULDER,
 pad_rshoulder=XINPUT_GAMEPAD_RIGHT_SHOULDER,
 pad_A=XINPUT_GAMEPAD_A,
 pad_B=XINPUT_GAMEPAD_B,
 pad_X=XINPUT_GAMEPAD_X,
 pad_Y=XINPUT_GAMEPAD_Y
};

enum XInputDeviceTypes {
 xin_unknown=XINPUT_DEVSUBTYPE_UNKNOWN,
 xin_wheel=XINPUT_DEVSUBTYPE_WHEEL,
 xin_arcade=XINPUT_DEVSUBTYPE_ARCADE_STICK,
 xin_flight=XINPUT_DEVSUBTYPE_FLIGHT_STICK,
 xin_dance=XINPUT_DEVSUBTYPE_DANCE_PAD,
 xin_guitar=XINPUT_DEVSUBTYPE_GUITAR,
 xin_guitar_alt=XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE,
 xin_drum=XINPUT_DEVSUBTYPE_DRUM_KIT,
 xin_bass=XINPUT_DEVSUBTYPE_GUITAR_BASS,
 xin_arcade_pad=XINPUT_DEVSUBTYPE_ARCADE_PAD
};

enum XInputDeviceKeys {
 xi_A=VK_PAD_A,
 xi_B=VK_PAD_B,
 xi_X=VK_PAD_X,
 xi_Y=VK_PAD_Y,
 xi_RShoulder=VK_PAD_RSHOULDER,
 xi_LShoulder=VK_PAD_LSHOULDER,
 xi_LTrigger=VK_PAD_LTRIGGER,
 xi_RTrigger=VK_PAD_RTRIGGER,
 xi_Up=VK_PAD_DPAD_UP,
 xi_Down=VK_PAD_DPAD_DOWN,
 xi_Left=VK_PAD_DPAD_LEFT,
 xi_Right=VK_PAD_DPAD_RIGHT,
 xi_Start=VK_PAD_START,
 xi_Back=VK_PAD_BACK,
 xi_LThumb=VK_PAD_LTHUMB_PRESS,
 xi_RThumb=VK_PAD_RTHUMB_PRESS,
 xi_LThumb_Up=VK_PAD_LTHUMB_UP,
 xi_LThumb_Down=VK_PAD_LTHUMB_DOWN,
 xi_LThumb_Right=VK_PAD_LTHUMB_RIGHT,
 xi_LThumb_Left=VK_PAD_LTHUMB_LEFT,
 xi_LThumb_UpLeft=VK_PAD_LTHUMB_UPLEFT,
 xi_LThumb_UpRight=VK_PAD_LTHUMB_UPRIGHT,
 xi_LThumb_DownRight=VK_PAD_LTHUMB_DOWNRIGHT,
 xi_LThumb_DownLeft=VK_PAD_LTHUMB_DOWNLEFT,
 xi_RThumb_U=VK_PAD_RTHUMB_UP,
 xi_RThumb_D=VK_PAD_RTHUMB_DOWN,
 xi_RThumb_R=VK_PAD_RTHUMB_RIGHT,
 xi_RThumb_L=VK_PAD_RTHUMB_LEFT,
 xi_RThumb_UL=VK_PAD_RTHUMB_UPLEFT,
 xi_RThumb_UR=VK_PAD_RTHUMB_UPRIGHT,
 xi_RThumb_DR=VK_PAD_RTHUMB_DOWNRIGHT,
 xi_RThumb_DL=VK_PAD_RTHUMB_DOWNLEFT
};

ONE(Gamepad,{
 deadzoneX=0.05f;
 deadzoneY=0.02f;
 cId=-1;
})
 Zint id;
 Zbool wasConnected,connected;
 Zfloat deadzoneX, deadzoneY;
 Gamepad( int i ) : ListItem() {
  id=i;
  deadzoneX=0.05f;
  deadzoneY=0.02f;
  cId=i;
  GetInfo();
 }
private:
 Zint cId;
 XINPUT_STATE state,previous;
 XINPUT_CAPABILITIES capabilities;
 XINPUT_BATTERY_INFORMATION battery;
 XINPUT_KEYSTROKE keystroke;
public:
 Zfloat leftStickX, leftStickY, rightStickX, rightStickY, leftTrigger, rightTrigger;
 Zfloat prevLeftStickX, prevLeftStickY, prevRightStickX, prevRightStickY, prevLeftTrigger, prevRightTrigger;
 Zbool up,down,left,right,start,back,lthumb,rthumb,lshoulder,rshoulder,a,b,x,y;
 int  GetPort() { return cId + 1; }
 bool GetInfo() {
  DWORD result=XInputGetCapabilities((DWORD)(int)cId,0,&capabilities); 
  if ( result != ERROR_SUCCESS ) { Disconnected(); return false; }
  result=XInputGetBatteryInformation((DWORD)(int)cId,capabilities.Type,&battery);
  if ( result != ERROR_SUCCESS ) { Disconnected(); return false; }
  return true;
 }
 void Disconnected() {
  if( connected ) wasConnected=connected;
  connected=false;
//  cId=-1;
 }
 bool Update() {
  if (XInputGetState((DWORD)(int)cId, &state) != ERROR_SUCCESS) { Disconnected(); return false; }
  DWORD result=XInputGetKeystroke((DWORD)(int)cId, XINPUT_FLAG_GAMEPAD, &keystroke);
  if (result == ERROR_DEVICE_NOT_CONNECTED) { Disconnected(); return false; }
  else if ( result != ERROR_EMPTY ) CheckKeystroke();
  return true;
 }
 Zint
  bA,bB,bX,bY,
  dpadL,dpadU,dpadR,dpadD,
  Lthumb,Rthumb,
  LthumbU,LthumbD,LthumbL,LthumbR,LthumbUL,LthumbUR,LthumbDL,LthumbDR,
  RthumbU,RthumbD,RthumbL,RthumbR,RthumbUL,RthumbUR,RthumbDL,RthumbDR,
  bStart,bBack,Lshoulder,Rshoulder,Ltrigger,Rtrigger;
 void DegradeKeystrokes() {
  if ( bA == 0 ) bA=-1;if ( bB == 0 ) bB=-1; if ( bX == 0 ) bX=-1; if ( bY == 0 ) bY=-1;
  if ( dpadL == 0 ) dpadL=-1; if ( dpadU == 0 ) dpadU=-1; if ( dpadR == 0 ) dpadR=-1; if ( dpadD == 0 ) dpadD=-1;
  if ( Lthumb == 0 ) Lthumb=-1; if ( Rthumb == 0 ) Rthumb=-1;
  if ( LthumbU == 0 ) LthumbU=-1; if ( LthumbD == 0 ) LthumbD=-1; if ( LthumbL == 0 ) LthumbL=-1; if ( LthumbR == 0 ) LthumbR=-1; if ( LthumbUL == 0 ) LthumbUL=-1; if ( LthumbUR == 0 ) LthumbUR=-1; if ( LthumbDL == 0 ) LthumbDL=-1; if ( LthumbDR == 0 ) LthumbDR=-1;
  if ( RthumbU == 0 ) RthumbU=-1; if ( RthumbD == 0 ) RthumbD=-1; if ( RthumbL == 0 ) RthumbL=-1; if ( RthumbR == 0 ) RthumbR=-1; if ( RthumbUL == 0 ) RthumbUL=-1; if ( RthumbUR == 0 ) RthumbUR=-1; if ( RthumbDL == 0 ) RthumbDL=-1; if ( RthumbDR == 0 ) RthumbDR=-1;
  if ( bStart == 0 ) bStart=-1; if ( bBack == 0 ) bBack=-1; if ( Lshoulder == 0 ) Lshoulder=-1; if ( Rshoulder == 0 ) Rshoulder=-1; if ( Ltrigger == 0 ) Ltrigger=-1; if ( Rtrigger == 0 ) Rtrigger=-1;
 }
 void CheckKeystroke() {
  switch ( keystroke.VirtualKey ) {
   default: break;
    case VK_PAD_A                 : { bA=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_B                 : { bB=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_X                 : { bX=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_Y                 : { bY=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RSHOULDER         : { Rshoulder=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LSHOULDER         : { Lshoulder=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTRIGGER          : { Ltrigger=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTRIGGER          : { Rtrigger=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_DPAD_UP           : { dpadU=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_DPAD_DOWN         : { dpadD=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_DPAD_LEFT         : { dpadL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_DPAD_RIGHT        : { dpadR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_START             : { bStart=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_BACK              : { bBack=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_PRESS      : { Lthumb=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_PRESS      : { Rthumb=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_UP         : { LthumbU=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_DOWN       : { LthumbD=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_RIGHT      : { LthumbR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_LEFT       : { LthumbL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_UPLEFT     : { LthumbUL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_UPRIGHT    : { LthumbUR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_DOWNRIGHT  : { LthumbDR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_LTHUMB_DOWNLEFT   : { LthumbDL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_UP         : { RthumbU=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_DOWN       : { RthumbD=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_RIGHT      : { RthumbR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_LEFT       : { RthumbL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_UPLEFT     : { RthumbUL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_UPRIGHT    : { RthumbUR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_DOWNRIGHT  : { RthumbDR=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
    case VK_PAD_RTHUMB_DOWNLEFT   : { RthumbDL=(keystroke.Flags==XINPUT_KEYSTROKE_KEYDOWN?1:(keystroke.Flags==XINPUT_KEYSTROKE_KEYUP?0:2)); } break;
  }
 }
 bool Down( XInputDeviceKeys vk ) { return Stroke(vk) > 0; }
 bool Up( XInputDeviceKeys vk ) { return Stroke(vk) <= 0; }
 bool Released( XInputDeviceKeys vk ) { return Stroke(vk) == 0; }
 bool Repeat( XInputDeviceKeys vk ) { return Stroke(vk) == 2; }
 int Stroke( XInputDeviceKeys vk ) {
  switch ( (int)vk ) {
   default: { return -1; } break;
    case VK_PAD_A                 : { return bA; } break;
    case VK_PAD_B                 : { return bB; } break;
    case VK_PAD_X                 : { return bX; } break;
    case VK_PAD_Y                 : { return bY; } break;
    case VK_PAD_RSHOULDER         : { return Rshoulder; } break;
    case VK_PAD_LSHOULDER         : { return Lshoulder; } break;
    case VK_PAD_LTRIGGER          : { return Ltrigger; } break;
    case VK_PAD_RTRIGGER          : { return Rtrigger; } break;
    case VK_PAD_DPAD_UP           : { return dpadU; } break;
    case VK_PAD_DPAD_DOWN         : { return dpadD; } break;
    case VK_PAD_DPAD_LEFT         : { return dpadL; } break;
    case VK_PAD_DPAD_RIGHT        : { return dpadR; } break;
    case VK_PAD_START             : { return bStart; } break;
    case VK_PAD_BACK              : { return bBack; } break;
    case VK_PAD_LTHUMB_PRESS      : { return Lthumb; } break;
    case VK_PAD_RTHUMB_PRESS      : { return Rthumb; } break;
    case VK_PAD_LTHUMB_UP         : { return LthumbU; } break;
    case VK_PAD_LTHUMB_DOWN       : { return LthumbD; } break;
    case VK_PAD_LTHUMB_RIGHT      : { return LthumbR; } break;
    case VK_PAD_LTHUMB_LEFT       : { return LthumbL; } break;
    case VK_PAD_LTHUMB_UPLEFT     : { return LthumbUL; } break;
    case VK_PAD_LTHUMB_UPRIGHT    : { return LthumbUR; } break;
    case VK_PAD_LTHUMB_DOWNRIGHT  : { return LthumbDR; } break;
    case VK_PAD_LTHUMB_DOWNLEFT   : { return LthumbDL; } break;
    case VK_PAD_RTHUMB_UP         : { return RthumbU; } break;
    case VK_PAD_RTHUMB_DOWN       : { return RthumbD; } break;
    case VK_PAD_RTHUMB_RIGHT      : { return RthumbR; } break;
    case VK_PAD_RTHUMB_LEFT       : { return RthumbL; } break;
    case VK_PAD_RTHUMB_UPLEFT     : { return RthumbUL; } break;
    case VK_PAD_RTHUMB_UPRIGHT    : { return RthumbUR; } break;
    case VK_PAD_RTHUMB_DOWNRIGHT  : { return RthumbDR; } break;
    case VK_PAD_RTHUMB_DOWNLEFT   : { return RthumbDL; } break;
  }
 }
 bool Button( int val ) { if ( state.Gamepad.wButtons & (WORD) val ) return true; return false; }
 bool WasButton( int val ) { if ( previous.Gamepad.wButtons & (WORD) val ) return true; return false; }
 bool AnyButton() {
  if ( ButtonReleased(VK_PAD_A               )
   || ButtonReleased(VK_PAD_B               )
   || ButtonReleased(VK_PAD_X               )
   || ButtonReleased(VK_PAD_Y               )
   || ButtonReleased(VK_PAD_RSHOULDER       )
   || ButtonReleased(VK_PAD_LSHOULDER       )
   || ButtonReleased(VK_PAD_LTRIGGER        )
   || ButtonReleased(VK_PAD_RTRIGGER        )
   //|| ButtonReleased(VK_PAD_DPAD_UP         )
   //|| ButtonReleased(VK_PAD_DPAD_DOWN       )
   //|| ButtonReleased(VK_PAD_DPAD_LEFT       )
   //|| ButtonReleased(VK_PAD_DPAD_RIGHT      )
   || ButtonReleased(VK_PAD_START           )
   || ButtonReleased(VK_PAD_BACK            )
   || ButtonReleased(VK_PAD_LTHUMB_PRESS    )
   || ButtonReleased(VK_PAD_RTHUMB_PRESS    )/*
   || ButtonReleased(VK_PAD_LTHUMB_UP       )
   || ButtonReleased(VK_PAD_LTHUMB_DOWN     )
   || ButtonReleased(VK_PAD_LTHUMB_RIGHT    )
   || ButtonReleased(VK_PAD_LTHUMB_LEFT     )
   || ButtonReleased(VK_PAD_LTHUMB_UPLEFT   )
   || ButtonReleased(VK_PAD_LTHUMB_UPRIGHT  )
   || ButtonReleased(VK_PAD_LTHUMB_DOWNRIGHT)
   || ButtonReleased(VK_PAD_LTHUMB_DOWNLEFT )
   || ButtonReleased(VK_PAD_RTHUMB_UP       )
   || ButtonReleased(VK_PAD_RTHUMB_DOWN     )
   || ButtonReleased(VK_PAD_RTHUMB_RIGHT    )
   || ButtonReleased(VK_PAD_RTHUMB_LEFT     )
   || ButtonReleased(VK_PAD_RTHUMB_UPLEFT   )
   || ButtonReleased(VK_PAD_RTHUMB_UPRIGHT  )
   || ButtonReleased(VK_PAD_RTHUMB_DOWNRIGHT)
   || ButtonReleased(VK_PAD_RTHUMB_DOWNLEFT )*/ ) return true;
  return false;
 }
 bool ButtonReleased( int val ) { return !Button(val) && WasButton(val); }
 XINPUT_GAMEPAD *GetState() { return &state.Gamepad; }
 void SetDeadzone(float x, float y) { deadzoneX = x; deadzoneY = y; }
 bool IsPressed(int button) { return (state.Gamepad.wButtons & (WORD)button) != 0; }
 bool Refresh();
 void StopVibrating() { Vibrate(0,0); }
 Zdouble vibrationDuration,vibrationExpired;
 Zbool vibrating;
 void Vibrate(int leftVal, int rightVal, double duration=-1.0);
MANY(Gamepad,GamepadHandle,GamepadHandles,"",Gamepads,{
 for ( int i=0; i<XUSER_MAX_COUNT; i++ ) Append(new Gamepad(i));
})
 void Between() {
  FOREACH(Gamepad,g) g->Refresh();
 }
 int Connected() {
  int total=0;
  FOREACH(Gamepad,g) if ( g->connected ) total++;
  return total;
 }
 void Vibrate(int leftVal, int rightVal, double duration=-1.0) {
  FOREACH(Gamepad,g) g->Vibrate(leftVal,rightVal,duration);
 }
 int Down( XInputDeviceKeys vk ) {
  FOREACH(Gamepad,g) if ( g->Down(vk) ) return true;
  return false;
 }
 int Up( XInputDeviceKeys vk ) {
  FOREACH(Gamepad,g) if ( g->Up(vk) ) return true;
  return false;
 }
 int Released( XInputDeviceKeys vk ) {
  FOREACH(Gamepad,g) if ( g->Released(vk) ) return true;
  return false;
 }
 int Repeat( XInputDeviceKeys vk ) { 
  FOREACH(Gamepad,g) if ( g->Repeat(vk) ) return true;
  return false;
 }
 bool Button( int val ) {
  FOREACH(Gamepad,g) if ( g->Button(val) ) return true;
  return false;
 }
 bool WasButton( int val ) {
  FOREACH(Gamepad,g) if ( g->WasButton(val) ) return true;
  return false;
 }
 bool AnyButton() {
  FOREACH(Gamepad,g) if ( g->AnyButton() ) return true;
  return false;
 }
 bool ButtonReleased( int val ) {
  FOREACH(Gamepad,g) if ( g->ButtonReleased(val) ) return true;
  return false;
 }
DONE(Gamepad);

class WindowsKeyboard {
public:
 Strings lines;
 Zstring buffer;
 void WM_Character( char c, int quantity ) {
  for ( int i=0; i<quantity; i++ ) {
   if ( c == 13 ) {
    buffer="";
   } else buffer+=c;
  }
 }
};

extern WindowsKeyboard windowsKeyboard;

#if defined(USE_WINTOUCH)
// One of the fields in GESTUREINFO structure is type of ULONGLONG (8 bytes).
// The relevant gesture information is stored in lower 4 bytes. This
// macro is used to read gesture information from this field.
#define LODWORD(ull) ((DWORD)((ULONGLONG)(ull) & 0x00000000ffffffff))
#ifndef GID_PRESSANDTAP
    #define GID_PRESSANDTAP GID_ROLLOVER
#endif
#endif
//#define MAXTOUCHES       11 // 10?  10 fingers + 1 pen


#if defined(USE_LOGITECH_KIT)

extern bool use_lg_button_notifications;

#endif 

// Get the DIK out of the code (I'm sorry but I just cannot take it anymore.)

#define DX_ESCAPE       DIK_ESCAPE       
#define DX_1            DIK_1            
#define DX_2            DIK_2            
#define DX_3            DIK_3            
#define DX_4            DIK_4            
#define DX_5            DIK_5            
#define DX_6            DIK_6            
#define DX_7            DIK_7            
#define DX_8            DIK_8            
#define DX_9            DIK_9            
#define DX_0            DIK_0            
#define DX_MINUS        DIK_MINUS        
#define DX_EQUALS       DIK_EQUALS       
#define DX_BACK         DIK_BACK         
#define DX_TAB          DIK_TAB          
#define DX_Q            DIK_Q            
#define DX_W            DIK_W            
#define DX_E            DIK_E            
#define DX_R            DIK_R            
#define DX_T            DIK_T            
#define DX_Y            DIK_Y            
#define DX_U            DIK_U            
#define DX_I            DIK_I            
#define DX_O            DIK_O            
#define DX_P            DIK_P            
#define DX_LBRACKET     DIK_LBRACKET     
#define DX_RBRACKET     DIK_RBRACKET     
#define DX_RETURN       DIK_RETURN       
#define DX_ENTER        DIK_RETURN
#define DX_NUM_ENTER    DIK_NUMPADENTER
#define DX_LCONTROL     DIK_LCONTROL     
#define DX_LCTRL        DIK_LCONTROL
#define DX_A            DIK_A            
#define DX_S            DIK_S            
#define DX_D            DIK_D            
#define DX_F            DIK_F            
#define DX_G            DIK_G            
#define DX_H            DIK_H            
#define DX_J            DIK_J            
#define DX_K            DIK_K            
#define DX_L            DIK_L            
#define DX_SEMICOLON    DIK_SEMICOLON    
#define DX_APOSTROPHE   DIK_APOSTROPHE   
#define DX_GRAVE        DIK_GRAVE
#define DX_BACKTICK     DIK_GRAVE
#define DX_LSHIFT       DIK_LSHIFT       
#define DX_BACKSLASH    DIK_BACKSLASH    
#define DX_Z            DIK_Z            
#define DX_X            DIK_X            
#define DX_C            DIK_C            
#define DX_V            DIK_V            
#define DX_B            DIK_B            
#define DX_N            DIK_N            
#define DX_M            DIK_M            
#define DX_COMMA        DIK_COMMA        
#define DX_PERIOD       DIK_PERIOD       
#define DX_SLASH        DIK_SLASH        
#define DX_RSHIFT       DIK_RSHIFT       
#define DX_MULTIPLY     DIK_MULTIPLY     
#define DX_LMENU        DIK_LMENU        
#define DX_SPACE        DIK_SPACE        
#define DX_CAPITAL      DIK_CAPITAL      
#define DX_F1           DIK_F1           
#define DX_F2           DIK_F2           
#define DX_F3           DIK_F3           
#define DX_F4           DIK_F4           
#define DX_F5           DIK_F5           
#define DX_F6           DIK_F6           
#define DX_F7           DIK_F7           
#define DX_F8           DIK_F8           
#define DX_F9           DIK_F9           
#define DX_F10          DIK_F10          
#define DX_NUMLOCK      DIK_NUMLOCK      
#define DX_SCROLL       DIK_SCROLL       
#define DX_NUMPAD7      DIK_NUMPAD7      
#define DX_NUMPAD8      DIK_NUMPAD8      
#define DX_NUMPAD9      DIK_NUMPAD9      
#define DX_SUBTRACT     DIK_SUBTRACT     
#define DX_NUMPAD4      DIK_NUMPAD4      
#define DX_NUMPAD5      DIK_NUMPAD5      
#define DX_NUMPAD6      DIK_NUMPAD6      
#define DX_ADD          DIK_ADD          
#define DX_NUMPAD1      DIK_NUMPAD1      
#define DX_NUMPAD2      DIK_NUMPAD2      
#define DX_NUMPAD3      DIK_NUMPAD3      
#define DX_NUMPAD0      DIK_NUMPAD0      
#define DX_DECIMAL      DIK_DECIMAL      
#define DX_OEM_102      DIK_OEM_102      
#define DX_F11          DIK_F11          
#define DX_F12          DIK_F12          
#define DX_F13          DIK_F13          
#define DX_F14          DIK_F14          
#define DX_F15          DIK_F15          
#define DX_KANA         DIK_KANA         
#define DX_ABNT_C1      DIK_ABNT_C1      
#define DX_CONVERT      DIK_CONVERT      
#define DX_NOCONVERT    DIK_NOCONVERT    
#define DX_YEN          DIK_YEN          
#define DX_ABNT_C2      DIK_ABNT_C2      
#define DX_NUMPADEQUALS DIK_NUMPADEQUALS 
#define DX_PREVTRACK    DIK_PREVTRACK    
#define DX_AT           DIK_AT           
#define DX_COLON        DIK_COLON        
#define DX_UNDERLINE    DIK_UNDERLINE    
#define DX_KANJI        DIK_KANJI        
#define DX_STOP         DIK_STOP         
#define DX_AX           DIK_AX           
#define DX_UNLABELED    DIK_UNLABELED    
#define DX_NEXTTRACK    DIK_NEXTTRACK    
#define DX_NUMPADENTER  DIK_NUMPADENTER  
#define DX_RCONTROL     DIK_RCONTROL   
#define DX_RCTRL        DIK_RCONTROL
#define DX_MUTE         DIK_MUTE         
#define DX_CALCULATOR   DIK_CALCULATOR   
#define DX_PLAYPAUSE    DIK_PLAYPAUSE    
#define DX_MEDIASTOP    DIK_MEDIASTOP    
#define DX_VOLUMEDOWN   DIK_VOLUMEDOWN   
#define DX_VOLUMEUP     DIK_VOLUMEUP     
#define DX_WEBHOME      DIK_WEBHOME      
#define DX_NUMPADCOMMA  DIK_NUMPADCOMMA  
#define DX_DIVIDE       DIK_DIVIDE       
#define DX_SYSRQ        DIK_SYSRQ        
#define DX_RMENU        DIK_RMENU        
#define DX_PAUSE        DIK_PAUSE        
#define DX_HOME         DIK_HOME         
#define DX_UP           DIK_UP           
#define DX_PRIOR        DIK_PRIOR        
#define DX_LEFT         DIK_LEFT         
#define DX_RIGHT        DIK_RIGHT        
#define DX_END          DIK_END          
#define DX_DOWN         DIK_DOWN         
#define DX_NEXT         DIK_NEXT         
#define DX_INSERT       DIK_INSERT       
#define DX_DELETE       DIK_DELETE       
#define DX_LWIN         DIK_LWIN         
#define DX_RWIN         DIK_RWIN         
#define DX_APPS         DIK_APPS         
#define DX_POWER        DIK_POWER        
#define DX_SLEEP        DIK_SLEEP        
#define DX_WAKE         DIK_WAKE         
#define DX_WEBSEARCH    DIK_WEBSEARCH    
#define DX_WEBFAVORITES DIK_WEBFAVORITES 
#define DX_WEBREFRESH   DIK_WEBREFRESH   
#define DX_WEBSTOP      DIK_WEBSTOP      
#define DX_WEBFORWARD   DIK_WEBFORWARD   
#define DX_WEBBACK      DIK_WEBBACK      
#define DX_MYCOMPUTER   DIK_MYCOMPUTER   
#define DX_MAIL         DIK_MAIL         
#define DX_MEDIASELECT  DIK_MEDIASELECT  
#define DX_BACKSPACE    DIK_BACKSPACE    
#define DX_NUMPADSTAR   DIK_NUMPADSTAR   
#define DX_LALT         DIK_LALT         
#define DX_CAPSLOCK     DIK_CAPSLOCK     
#define DX_NUMPADMINUS  DIK_NUMPADMINUS  
#define DX_NUMPADPLUS   DIK_NUMPADPLUS   
#define DX_NUMPADPERIOD DIK_NUMPADPERIOD 
#define DX_NUMPADSLASH  DIK_NUMPADSLASH  
#define DX_RALT         DIK_RALT         
#define DX_UPARROW      DIK_UPARROW      
#define DX_PGUP         DIK_PGUP         
#define DX_LEFTARROW    DIK_LEFTARROW    
#define DX_RIGHTARROW   DIK_RIGHTARROW   
#define DX_DOWNARROW    DIK_DOWNARROW    
#define DX_PGDN         DIK_PGDN         
#define DX_CIRCUMFLEX   DIK_CIRCUMFLEX   

// Aliases   
#define DX_NUM_LOCK      DIK_NUMLOCK      
#define DX_SCROLL_LOCK   DIK_SCROLL   
#define DX_CAPS_LOCK     DIK_CAPSLOCK     

#define DX_NUMPAD_7        DIK_NUMPAD7
#define DX_NUMPAD_HOME     DIK_NUMPAD7
#define DX_NUMPAD_8        DIK_NUMPAD8
#define DX_NUMPAD_UP       DIK_NUMPAD8
#define DX_NUMPAD_9        DIK_NUMPAD9
#define DX_NUMPAD_PGUP     DIK_NUMPAD9
#define DX_NUMPAD_4        DIK_NUMPAD4
#define DX_NUMPAD_LEFT     DIK_NUMPAD4
#define DX_NUMPAD_5        DIK_NUMPAD5
#define DX_NUMPAD_CENTER   DIK_NUMPAD5
#define DX_NUMPAD_6        DIK_NUMPAD6
#define DX_NUMPAD_RIGHT    DIK_NUMPAD6
#define DX_NUMPAD_1        DIK_NUMPAD1
#define DX_NUMPAD_END      DIK_NUMPAD1
#define DX_NUMPAD_2        DIK_NUMPAD2
#define DX_NUMPAD_DOWN     DIK_NUMPAD2
#define DX_NUMPAD_3        DIK_NUMPAD3
#define DX_NUMPAD_PGDN     DIK_NUMPAD3
#define DX_NUMPAD_0        DIK_NUMPAD0
#define DX_NUMPAD_INS      DIK_NUMPAD0
#define DX_NUMPAD_EQUALS   DIK_NUMPADEQUALS
#define DX_NUMPAD_ASTERISK DIK_NUMPADSTAR   
#define DX_NUMPAD_STAR     DIK_NUMPADSTAR    
#define DX_NUMPAD_MULT     DIK_NUMPADSTAR  
#define DX_NUMPAD_MINUS    DIK_NUMPADMINUS
#define DX_NUMPAD_PLUS     DIK_NUMPADPLUS
#define DX_NUMPAD_PERIOD   DIK_NUMPADPERIOD
#define DX_NUMPAD_DEL      DIK_NUMPADPERIOD
#define DX_NUMPAD_SLASH    DIK_NUMPADSLASH  
#define DX_NUMPAD_DIV      DIK_NUMPADSLASH  
#define DX_NUMPAD_ADD      DIK_ADD          

/*
 * Logitech SDK 1.1 "Resource" Codes
 * Listed here only to avoid muddling the classes below.
 */

#define IDC_GET_MICE_CONTROL            1000
#define IDC_GET_MICE_CONTROL_RET        1001
#define IDC_RELEASE_MICE_CONTROL        1002
#define IDC_RELEASE_MICE_CONTROL_RET    1003
#define IDC_REGISTER_NOTIF              1004
#define IDC_REGISTER_NOTIF_RET          1005
#define IDC_UNREGISTER_NOTIF            1006
#define IDC_UNREGISTER_NOTIF_RET        1007
#define IDC_GET_MAX_RESOLUTION          1008
#define IDC_GET_MAX_RES_VAL             1009
#define IDC_SET_MAX_RESOLUTION          1010
#define IDC_GET_MAX_RES_RET             1011
#define IDC_SET_MAX_RES_RET             1012
#define IDC_SET_MAX_RES_VAL             1013
#define IDC_GET_MAX_LED                 1014
#define IDC_GET_MAX_LED_RET             1015
#define IDC_GET_MAX_LED_VAL             1016
#define IDC_SET_LED                     1017
#define IDC_SET_LED_RET                 1018
#define IDC_LED_0                       1019
#define IDC_SET_MAX_RESOLUTION2         1019
#define IDC_GET_CURRENT_RESOLUTION      1019
#define IDC_LED_1                       1020
#define IDC_LED_2                       1021
#define IDC_LED_3                       1022
#define IDC_LED_4                       1023
#define IDC_LED_5                       1024
#define IDC_BUTTON_1                    1025
#define IDC_BUTTON_2                    1026
#define IDC_BUTTON_3                    1027
#define IDC_BUTTON_4                    1028
#define IDC_BUTTON_5                    1029
#define IDC_BUTTON_6                    1030
#define IDC_BUTTON_7                    1031
#define IDC_BUTTON_8                    1032
#define IDC_BUTTON_9                    1033
#define IDC_BUTTON_10                   1034
#define IDC_LED_6                       1035
#define IDC_CHECK2                      1037
#define IDC_ALL_BUTTONS                 1037
#define IDC_REGISTER_LIFT               1039
#define IDC_LIFT_STATE                  1040
#define IDC_REGISTER_LIFT_RET           1041
#define IDC_UNREGISTER_LIFT             1042
#define IDC_REGISTER_LIFT_RET2          1043
#define IDC_UNREGISTER_LIFT_RET         1043

extern char codeToChar[256];
extern UCHAR codeToUnichar[256];

static unsigned int VK_to_DI[256] = {
  0,  0,
  0,  0,
  0,  0,
  0,  0,
  DX_BACK,    //#define VK_BACK           0x08
  DX_TAB,     //#define VK_TAB            0x09
  0,  0,       // 0x0A - 0x0B : reserved
  0,           //#define VK_CLEAR          0x0C
  DX_RETURN,
  0,  0,
  0,           //#define VK_SHIFT          0x10
  0,           //#define VK_CONTROL        0x11
  0,           //#define VK_MENU           0x12
  DX_PAUSE,   //#define VK_PAUSE          0x13
  DX_CAPITAL, //#define VK_CAPITAL        0x14
  0,           // VK_KANA, VK_HANGUEL, VK_HANGUL ...
  0,           // VK_JUNJA
  0,           // VK_FINAL
  0,           // VK_HANJA / VK_KANJI
  0,           // 0x1A unused
  DX_ESCAPE,  //#define VK_ESCAPE         0x1B
  DX_CONVERT, //#define VK_CONVERT        0x1C
  DX_NOCONVERT, //#define VK_NONCONVERT     0x1D
  0,           //#define VK_ACCEPT         0x1E
  0,           //#define VK_MODECHANGE     0x1F
  DX_SPACE,   //#define VK_SPACE          0x20
  DX_PRIOR,   //#define VK_PRIOR          0x21
  DX_NEXT,    //#define VK_NEXT           0x22
  DX_END,     //#define VK_END            0x23
  DX_HOME,    //#define VK_HOME           0x24
  DX_LEFT,    //#define VK_LEFT           0x25
  DX_UP,      //#define VK_UP             0x26
  DX_RIGHT,   //#define VK_RIGHT          0x27
  DX_DOWN,    //#define VK_DOWN           0x28
  0,           //#define VK_SELECT         0x29
  0,           //#define VK_PRINT          0x2A
  0,           //#define VK_EXECUTE        0x2B
  0,           //#define VK_SNAPSHOT       0x2C
  DX_INSERT,  //#define VK_INSERT         0x2D
  DX_DELETE,  //#define VK_DELETE         0x2E
  0,           //#define VK_HELP           0x2F
  DX_0,       // VK_0 0x30
  DX_1,       // VK_1 0x31
  DX_2,       // VK_2 0x32
  DX_3,       // VK_3 0x33
  DX_4,       // VK_4 0x34
  DX_5,       // VK_5 0x35
  DX_6,       // VK_6 0x36
  DX_7,       // VK_7 0x37
  DX_8,       // VK_8 0x38
  DX_9,       // VK_9 0x39
  0,           // 0x40 : unassigned
  DX_A,       // VK_A 0x41
  DX_B,       // VK_B 0x42
  DX_C,       // VK_C 0x43
  DX_D,       // VK_D 0x44
  DX_E,       // VK_E 0x45
  DX_F,       // VK_F 0x46
  DX_G,       // VK_G 0x47
  DX_H,       // VK_H 0x48
  DX_I,       // VK_I 0x49
  DX_J,       // VK_J 0x4A
  DX_K,       // VK_K 0x4B
  DX_L,       // VK_L 0x4C
  DX_M,       // VK_M 0x4D
  DX_N,       // VK_N 0x4E
  DX_O,       // VK_O 0x4F
  DX_P,       // VK_P 0x50
  DX_Q,       // VK_Q 0x51
  DX_R,       // VK_R 0x52
  DX_S,       // VK_S 0x53
  DX_T,       // VK_T 0x54
  DX_U,       // VK_U 0x55
  DX_V,       // VK_V 0x56
  DX_W,       // VK_W 0x57
  DX_X,       // VK_X 0x58
  DX_Y,       // VK_Y 0x59
  DX_Z,       // VK_Z 0x5A
  DX_LWIN,    //#define VK_LWIN           0x5B
  DX_RWIN,    //#define VK_RWIN           0x5C
  DX_APPS,    //#define VK_APPS           0x5D
  0,           // 0x5E : reserved
  DX_SLEEP,   //#define VK_SLEEP          0x5F
  DX_NUMPAD0, //#define VK_NUMPAD0        0x60
  DX_NUMPAD1, //#define VK_NUMPAD1        0x61
  DX_NUMPAD2, //#define VK_NUMPAD2        0x62
  DX_NUMPAD3, //#define VK_NUMPAD3        0x63
  DX_NUMPAD4, //#define VK_NUMPAD4        0x64
  DX_NUMPAD5, //#define VK_NUMPAD5        0x65
  DX_NUMPAD6, //#define VK_NUMPAD6        0x66
  DX_NUMPAD7, //#define VK_NUMPAD7        0x67
  DX_NUMPAD8, //#define VK_NUMPAD8        0x68
  DX_NUMPAD9, //#define VK_NUMPAD9        0x69
  DX_MULTIPLY,//#define VK_MULTIPLY       0x6A
  DX_ADD,     //#define VK_ADD            0x6B
  0,           //#define VK_SEPARATOR      0x6C
  DX_SUBTRACT,//#define VK_SUBTRACT       0x6D
  DX_DECIMAL, //#define VK_DECIMAL        0x6E
  DX_DIVIDE,  //#define VK_DIVIDE         0x6F
  DX_F1,      //#define VK_F1             0x70
  DX_F2,      //#define VK_F2             0x71
  DX_F3,      //#define VK_F3             0x72
  DX_F4,      //#define VK_F4             0x73
  DX_F5,      //#define VK_F5             0x74
  DX_F6,      //#define VK_F6             0x75
  DX_F7,      //#define VK_F7             0x76
  DX_F8,      //#define VK_F8             0x77
  DX_F9,      //#define VK_F9             0x78
  DX_F10,     //#define VK_F10            0x79
  DX_F11,     //#define VK_F11            0x7A
  DX_F12,     //#define VK_F12            0x7B
  DX_F13,     //#define VK_F13            0x7C
  DX_F14,     //#define VK_F14            0x7D
  DX_F15,     //#define VK_F15            0x7E
  0,           //#define VK_F16            0x7F
  0,           //#define VK_F17            0x80
  0,           //#define VK_F18            0x81
  0,           //#define VK_F19            0x82
  0,           //#define VK_F20            0x83
  0,           //#define VK_F21            0x84
  0,           //#define VK_F22            0x85
  0,           //#define VK_F23            0x86
  0,           //#define VK_F24            0x87
  0, 0, 0, 0, 
  0, 0, 0, 0,  // 0x88 - 0x8F : unassigned
  DX_NUMLOCK, //#define VK_NUMLOCK        0x90
  DX_SCROLL,  //#define VK_SCROLL         0x91
  DX_NUMPADEQUALS, // NEC PC-9800 kbd definitions  #define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
  0, 0, 0, 0, 0,    // Fujitsu/OASYS kbd definitions ; #define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key #define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key #define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key #define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key #define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
  0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x97 - 0x9F : unassigned
/* VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way. */
  DX_LSHIFT,       //#define VK_LSHIFT              0xA0
  DX_RSHIFT,       //#define VK_RSHIFT              0xA1
  DX_LCONTROL,     //#define VK_LCONTROL            0xA2
  DX_RCONTROL,     //#define VK_RCONTROL            0xA3
  DX_LMENU,        //#define VK_LMENU               0xA4
  DX_RMENU,        //#define VK_RMENU               0xA5
  DX_WEBBACK,      //#define VK_BROWSER_BACK        0xA6
  DX_WEBFORWARD,   //#define VK_BROWSER_FORWARD     0xA7
  DX_WEBREFRESH,   //#define VK_BROWSER_REFRESH     0xA8
  DX_WEBSTOP,      //#define VK_BROWSER_STOP        0xA9
  DX_WEBSEARCH,    //#define VK_BROWSER_SEARCH      0xAA
  DX_WEBFAVORITES, //#define VK_BROWSER_FAVORITES   0xAB
  DX_WEBHOME,      //#define VK_BROWSER_HOME        0xAC
  DX_MUTE,         //#define VK_VOLUME_MUTE         0xAD
  DX_VOLUMEDOWN,   //#define VK_VOLUME_DOWN         0xAE
  DX_VOLUMEUP,     //#define VK_VOLUME_UP           0xAF
  DX_NEXTTRACK,    //#define VK_MEDIA_NEXT_TRACK    0xB0
  DX_PREVTRACK,    //#define VK_MEDIA_PREV_TRACK    0xB1
  DX_MEDIASTOP,    //#define VK_MEDIA_STOP          0xB2
  DX_PLAYPAUSE,    //#define VK_MEDIA_PLAY_PAUSE    0xB3
  DX_MAIL,         //#define VK_LAUNCH_MAIL         0xB4
  DX_MEDIASELECT,  //#define VK_LAUNCH_MEDIA_SELECT 0xB5
  0,                //#define VK_LAUNCH_APP1         0xB6
  0,                //#define VK_LAUNCH_APP2         0xB7
  0, 0,             // 0xB8 - 0xB9 : reserved
  DX_SEMICOLON,    //#define VK_OEM_1          0xBA   // ';:' for US
  DX_EQUALS,       //#define VK_OEM_PLUS       0xBB   // '+' any country
  DX_COMMA,        //#define VK_OEM_COMMA      0xBC   // ',' any country
  DX_MINUS,        //#define VK_OEM_MINUS      0xBD   // '-' any country
  DX_PERIOD,       //#define VK_OEM_PERIOD     0xBE   // '.' any country
  DX_SLASH,        //#define VK_OEM_2          0xBF   // '/?' for US
  DX_GRAVE,        //#define VK_OEM_3          0xC0   // '`~' for US
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xC1 - 0xD7 : reserved
  0, 0, 0,          // 0xD8 - 0xDA : unassigned
  DX_LBRACKET,     //#define VK_OEM_4          0xDB  //  '[{' for US
  DX_BACKSLASH,    //#define VK_OEM_5          0xDC  //  '\|' for US
  DX_RBRACKET,     //#define VK_OEM_6          0xDD  //  ']}' for US
  DX_APOSTROPHE,   //#define VK_OEM_7          0xDE  //  ''"' for US
  0,                //#define VK_OEM_8          0xDF
  0,                // 0xE0 : reserved
/*Various extended or enhanced keyboards */
  DX_AX,           //#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
  DIKEYBOARD_OEM_102, //#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
  0,                //#define VK_ICO_HELP       0xE3  //  Help key on ICO
  0,                //#define VK_ICO_00         0xE4  //  00 key on ICO
  0,                //#define VK_PROCESSKEY     0xE5
  0,                //#define VK_ICO_CLEAR      0xE6
  0,                //#define VK_PACKET         0xE7
  0,                // 0xE8 : unassigned
/* Nokia/Ericsson definitions */
  0,                //#define VK_OEM_RESET      0xE9
  0,                //#define VK_OEM_JUMP       0xEA
  0,                //#define VK_OEM_PA1        0xEB
  0,                //#define VK_OEM_PA2        0xEC
  0,                //#define VK_OEM_PA3        0xED
  0,                //#define VK_OEM_WSCTRL     0xEE
  0,                //#define VK_OEM_CUSEL      0xEF
  0,                //#define VK_OEM_ATTN       0xF0
  0,                //#define VK_OEM_FINISH     0xF1
  0,                //#define VK_OEM_COPY       0xF2
  0,                //#define VK_OEM_AUTO       0xF3
  0,                //#define VK_OEM_ENLW       0xF4
  0,                //#define VK_OEM_BACKTAB    0xF5
  0,                //#define VK_ATTN           0xF6
  0,                //#define VK_CRSEL          0xF7
  0,                //#define VK_EXSEL          0xF8
  0,                //#define VK_EREOF          0xF9
  DX_PLAYPAUSE,    //#define VK_PLAY           0xFA
  0,                //#define VK_ZOOM           0xFB
  0,                //#define VK_NONAME         0xFC
  0,                //#define VK_PA1            0xFD
  0,                //#define VK_OEM_CLEAR      0xFE
  0                 // 0xFF : reserved
};

enum Keys {
 ESC=DX_ESCAPE,//          0x01
 Key_1=DX_1,//               0x02
 Key_2=DX_2,//               0x03
 Key_3=DX_3,//               0x04
 Key_4=DX_4,//               0x05
 Key_5=DX_5,//               0x06
 Key_6=DX_6,//               0x07
 Key_7=DX_7,//               0x08
 Key_8=DX_8,//               0x09
 Key_9=DX_9,//               0x0A
 Key_0=DX_0,//               0x0B
 Minus=DX_MINUS,//           0x0C    /* - on main keyboard */
 Equals=DX_EQUALS,//          0x0D
 Backspace=DX_BACK,//            0x0E    /* backspace */
 Tab=DX_TAB,//             0x0F
 Q=DX_Q,//               0x10
 W=DX_W,//               0x11
 E=DX_E,//               0x12
 R=DX_R,//               0x13
 T=DX_T,//               0x14
 Y=DX_Y,//               0x15
 U=DX_U,//               0x16
 I=DX_I,//               0x17
 O=DX_O,//               0x18
 P=DX_P,//               0x19
 LBracket=DX_LBRACKET,//        0x1A
 RBracket=DX_RBRACKET,//        0x1B
 Enter=DX_RETURN,//          0x1C    /* Enter on main keyboard */
 LCtrl=DX_LCONTROL,//        0x1D
 A=DX_A,//               0x1E
 S=DX_S,//               0x1F
 D=DX_D,//               0x20
 F=DX_F,//               0x21
 G=DX_G,//               0x22
 H=DX_H,//               0x23
 J=DX_J,//               0x24
 K=DX_K,//               0x25
 L=DX_L,//               0x26
 Semicolon=DX_SEMICOLON,//       0x27
 Apostrophe=DX_APOSTROPHE,//      0x28
 Grave=DX_GRAVE,//           0x29    /* accent grave */
 LShift=DX_LSHIFT,//          0x2A
 Backslash=DX_BACKSLASH,//       0x2B
 Z=DX_Z,//               0x2C
 X=DX_X,//               0x2D
 C=DX_C,//               0x2E
 V=DX_V,//               0x2F
 B=DX_B,//               0x30
 N=DX_N,//               0x31
 M=DX_M,//               0x32
 Comma=DX_COMMA,//           0x33
 Period=DX_PERIOD,//          0x34    /* . on main keyboard */
 Slash=DX_SLASH,//           0x35    /* / on main keyboard */
 RShift=DX_RSHIFT,//          0x36
 NumpadMultiply=DX_MULTIPLY,//        0x37    /* * on numeric keypad */
 LAlt=DX_LMENU,//           0x38    /* left Alt */
 Spacebar=DX_SPACE,//           0x39
 CAPSLOCK=DX_CAPITAL,//         0x3A
 F1=DX_F1,//              0x3B
 F2=DX_F2,//              0x3C
 F3=DX_F3,//              0x3D
 F4=DX_F4,//              0x3E
 F5=DX_F5,//              0x3F
 F6=DX_F6,//              0x40
 F7=DX_F7,//              0x41
 F8=DX_F8,//              0x42
 F9=DX_F9,//              0x43
 F10=DX_F10,//             0x44
 NUMLOCK=DX_NUMLOCK,//         0x45
 SCROLLLOCK=DX_SCROLL,//          0x46    /* Scroll Lock */
 Numpad7=DX_NUMPAD7,//         0x47
 Numpad8=DX_NUMPAD8,//         0x48
 Numpad9=DX_NUMPAD9,//         0x49
 NumpadMinus=DX_SUBTRACT,//        0x4A    /* - on numeric keypad */
 Numpad4=DX_NUMPAD4,//         0x4B
 Numpad5=DX_NUMPAD5,//         0x4C
 Numpad6=DX_NUMPAD6,//         0x4D
 NumpadPlus=DX_ADD,//             0x4E    /* + on numeric keypad */
 Numpad1=DX_NUMPAD1,//         0x4F
 Numpad2=DX_NUMPAD2,//         0x50
 Numpad3=DX_NUMPAD3,//         0x51
 Numpad0=DX_NUMPAD0,//         0x52
 NumpadDecimal=DX_DECIMAL,//         0x53    /* . on numeric keypad */
 RT102=DX_OEM_102,//         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
 F11=DX_F11,//             0x57
 F12=DX_F12,//             0x58
 F13=DX_F13,//             0x64    /*                     (NEC PC98) */
 F14=DX_F14,//             0x65    /*                     (NEC PC98) */
 F15=DX_F15,//             0x66    /*                     (NEC PC98) */
 Kana=DX_KANA,//            0x70    /* (Japanese keyboard)            */
 ABNT_C1=DX_ABNT_C1,//         0x73    /* /? on Brazilian keyboard */
 Convert=DX_CONVERT,//         0x79    /* (Japanese keyboard)            */
 NoConvert=DX_NOCONVERT,//       0x7B    /* (Japanese keyboard)            */
 Yen=DX_YEN,//             0x7D    /* (Japanese keyboard)            */
 ABNT_C2=DX_ABNT_C2,//         0x7E    /* Numpad . on Brazilian keyboard */
 NumpadEquals=DX_NUMPADEQUALS,//    0x8D    /* = on numeric keypad (NEC PC98) */
 PrevTrackOrCIRCUMFLEX=DX_PREVTRACK,//       0x90    /* Previous Track (DX_CIRCUMFLEX on Japanese keyboard) */
 At=DX_AT,//              0x91    /*                     (NEC PC98) */
 Colon=DX_COLON,//           0x92    /*                     (NEC PC98) */
 Underline=DX_UNDERLINE,//       0x93    /*                     (NEC PC98) */
 Kanji=DX_KANJI,//           0x94    /* (Japanese keyboard)            */
 Stop=DX_STOP,//            0x95    /*                     (NEC PC98) */
 AX=DX_AX,//              0x96    /*                     (Japan AX) */
 Unlabeled=DX_UNLABELED,//       0x97    /*                        (J3100) */
 NextTrack=DX_NEXTTRACK,//       0x99    /* Next Track */
 NumpadEnter=DX_NUMPADENTER,//     0x9C    /* Enter on numeric keypad */
 RCtrl=DX_RCONTROL,//        0x9D
 Mute=DX_MUTE,//            0xA0    /* Mute */
 Calculator=DX_CALCULATOR,//      0xA1    /* Calculator */
 PlayPause=DX_PLAYPAUSE,//       0xA2    /* Play / Pause */
 MediaStop=DX_MEDIASTOP,//       0xA4    /* Media Stop */
 VolumeDown=DX_VOLUMEDOWN,//      0xAE    /* Volume - */
 VolumeUp=DX_VOLUMEUP,//        0xB0    /* Volume + */
 WebHome=DX_WEBHOME,//         0xB2    /* Web home */
 NumpadComma=DX_NUMPADCOMMA,//     0xB3    /* , on numeric keypad (NEC PC98) */
 NumpadDivide=DX_DIVIDE,//          0xB5    /* / on numeric keypad */
 SysRQ=DX_SYSRQ,//           0xB7
 RAlt=DX_RMENU,//           0xB8    /* right Alt */
 Pause=DX_PAUSE,//           0xC5    /* Pause */
 Home=DX_HOME,//            0xC7    /* Home on arrow keypad */
 Up=DX_UP,//              0xC8    /* UpArrow on arrow keypad */
 PgUp=DX_PRIOR,//           0xC9    /* PgUp on arrow keypad */
 Left=DX_LEFT,//            0xCB    /* LeftArrow on arrow keypad */
 Right=DX_RIGHT,//           0xCD    /* RightArrow on arrow keypad */
 End=DX_END,//             0xCF    /* End on arrow keypad */
 Down=DX_DOWN,//            0xD0    /* DownArrow on arrow keypad */
 Prior=DX_PRIOR,//          0xC9    /* PgUp on arrow keypad */
 PgDn=DX_NEXT,//            0xD1    /* PgDn on arrow keypad */
 Next=DX_NEXT,//            0xD1    /* PgDn on arrow keypad */
 Insert=DX_INSERT,//          0xD2    /* Insert on arrow keypad */
 Delete=DX_DELETE,//          0xD3    /* Delete on arrow keypad */
 LWin=DX_LWIN,//            0xDB    /* Left Windows key */
 RWin=DX_RWIN,//            0xDC    /* Right Windows key */
 Apps=DX_APPS,//            0xDD    /* AppMenu key */
 Power=DX_POWER,//           0xDE    /* System Power */
 SysSleep=DX_SLEEP,//           0xDF    /* System Sleep */
 Wake=DX_WAKE,//            0xE3    /* System Wake */
 WebSearch=DX_WEBSEARCH,//       0xE5    /* Web Search */
 WebFavorites=DX_WEBFAVORITES,//    0xE6    /* Web Favorites */
 WebRefresh=DX_WEBREFRESH,//      0xE7    /* Web Refresh */
 WebStop=DX_WEBSTOP,//         0xE8    /* Web Stop */
 WebForward=DX_WEBFORWARD,//      0xE9    /* Web Forward */
 WebBack=DX_WEBBACK,//         0xEA    /* Web Back */
 MyComputer=DX_MYCOMPUTER,//      0xEB    /* My Computer */
 Mail=DX_MAIL,//            0xEC    /* Mail */
 MediaSelect=DX_MEDIASELECT,//     0xED    /* Media Select */
 GreaterSign=DX_COMMA,
 LessSign=DX_PERIOD
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
 bool isChar() { return ((int) ch>0) && ((int) ch<127); }
 ~KEY(void);
};

class KeyList : public LinkedList {
public:
 void Add( int code ) {
  Append(new KEY(code));
 }
 bool InList( int code );
 bool CharInList( char ch );
 char FirstCharInList() {
  KEY *k;
  for ( k=(KEY *) (first); k; k=(KEY *)(k->next) ) if ( k->isChar() ) return k->ch;
  return '\0';
 }
 void Push ( int code ) { Prepend( new KEY(code) ); }
 KeyList() : LinkedList() {}
 CLEARLISTRESET(KEY);
};

class KeyBinding : public NamedHandle<ListItem> {
public:
 Keys key;
};

class KeyBindings : public LinkedList {
public:
 CLEARLISTRESET(KeyBinding);
 ~KeyBindings() { Clear(); }
};

////////////////////////////////////////////////////////////// WINDOWS TOUCH /////////////////////////////////////////

class WindowsTouch {
public:
 Zbool ready;
 Zbool integrated;
 Zbool external;
 Zbool multitouch;
 Zbool pen;
 Zbool registered;
};

#if defined(USE_WINTOUCH)

#define MAXPOINTS 10
extern int touchpoints[MAXPOINTS][2];
extern int tidLookup[MAXPOINTS];
extern int touching,touched;
void InitTouchPoints();
int GetContactIndex(int dwID);

enum TouchTypes {
 gesture_none,
 gesture_press,
 gesture_pressAndTap,
 gesture_twoFingerTap,
 gesture_Move,
 gesture_Zoom,
 gesture_Rotate,
 gesture_Pan
};
/*
  MSDN Note:
   If the target hardware on a machine does not support hover, 
   when the TOUCHEVENTF_UP flag is set, the TOUCHEVENTF_INRANGE
   flag is cleared. If the target hardware on a machine supports
   hover, the TOUCHEVENTF_UP and TOUCHEVENTF_INRANGE flags will
   be set independently.
*/
class Touch : public ListItem {
public:
 TouchTypes type;
 Zdouble angle;      // Angle of rotation (Rotation Gesture)
 Zint dx,dy;         // Touch movement (Move Gesture)
 Zint x,y;           // Touch location (in pixels) (Rotate Gesture, Zoom Gesture)
 Zint expiry;
 Zdouble zoomFactor; // Touch zoom factor (Zoom Gesture)
 Zdouble distance;   // Finger 1 and 2 distance (Two-Finger Tap)
 Zint tx,ty;         // Raw touch information
 Zint cx,cy;         // Contact centroid
 Zdouble timeS,timeM,timeH;
 Zbool up;           // Removed
 Zbool down;         // Active
 Zbool moving;       // Moving
 Zbool primary;      // See below
 Zbool noCoalesce;   // WINAPI
 Zbool inRange;      // TOUCHEVENTF_INRANGE
 Zbool area;         // cx,cy are valid
 Zbool palm;         // From the user's palm?
 Zbool pen;          // From a pen tip?
 Touch() : ListItem() {
  type=gesture_none;
  expiry=10;
  InitTouchPoints();
 }
 void Timestamp() {
  SYSTEMTIME st;
  GetSystemTime(&st);
  timeS=(double)st.wMilliseconds/999.0;
  timeM=((double)st.wSecond+timeS)/59.0;
  timeH=((double)st.wMinute+timeM)/59.0;
 }
 std::string asString() {
  string s=string("");
  switch ( type ) {
           case gesture_none: s=string("none"); break;
          case gesture_press: s=string("press"); break;
    case gesture_pressAndTap: s=string("pressAndTap"); break;
   case gesture_twoFingerTap: s=string("twoFingerTap"); break;
           case gesture_Move: s=string("move"); break;
           case gesture_Zoom: s=string("zoom"); break;
         case gesture_Rotate: s=string("rotate"); break;
            case gesture_Pan: s=string("pan"); break;
                     default: s=string("invalid or unknown"); break;
  }
  return FORMAT("<%f d=%f xy: %d,%d dxy: %d,%d Zf:%d %s", (double) angle, (double) distance, (int) x, (int) y, (int) dx, (int) dy, (double) zoomFactor, s.c_str() );
 }
};

/*** Zbool Touch::primary;
A touch point is designated as primary when it is the first touch point to be
established from a previous state of no touch points. The TOUCHEVENTF_PRIMARY
flag continues to be set for all subsequent events for the primary touch point
until the primary touch point is released. Note that a TOUCHEVENTF_UP event on
the primary touch point does not necessarily designate the end of a Windows Touch
operation; the current Windows Touch operation proceeds from the establishment
of the primary touch point until the last touch point is released.

Note that a solitary touch point or, in a set of simultaneous touch points, the
first to be detected, is designated the primary. The system mouse position follows
the primary touch point and, in addition to touch messages, also generates
WM_LBUTTONDOWN, WM_MOUSEMOVE, and WM_LBUTTONUP messages in response to actions on
a primary touch point. The primary touch point can also generate WM_RBUTTONDOWN
and WM_RBUTTONUP messages using the press and hold gesture.

Note that the touch point identifier may be dynamic and is associated with a
given touch point only as long as the touch point persists. If contact is
broken and then resumed (for example, if a finger is removed from the surface
and then pressed down again), the same touch point (the same finger, pen, or
other such device) may receive a different touch point identifier.
***/

class Touches : public LinkedList {
public:
 CLEARLISTRESET(Touch);
 ~Touches() { Clear(); }
 void ClearExpired(Touches *were) {
  /*
  SYSTEMTIME st;
  GetSystemTime(&st);
  double timeS=(double)st.wMilliseconds/999.0;
  double timeM=((double)st.wSecond+timeS)/59.0;
  double timeH=((double)st.wMinute+timeM)/59.0;
  double our_time=timeS + timeM*59 * timeH*59*59;
  Touch *n=NULL;
  for ( Touch *t=(Touch *) first; t!= NULL; t=n ) {
   n=(Touch *) t->next;
   double touch_time=t->timeS + t->timeM*59 + t->timeH*59*59;
   if ( our_time - touch_time > 59 * 10 ) Remove(t); // Ten second rule
  }
  */
  Touch *n=NULL;
  for ( Touch *t=(Touch *) first; t!= NULL; t=n ) {
   t->expiry--;
   n=(Touch *) t->next;
   if ( t->expiry <= 0 ) {
    Remove(t); // Ten second rule
    were->Append(t);
   }
  }
 }
};
#endif

// Indicates not touching when set to these values
class NullTouchArea {
public:
 int x,y;
 NullTouchArea() {
  x=123456;
  y=456789;
 }
 bool isTouching( int x, int y ) {
  return (x != this->x && y != this->y);
 }
};



///////////////////////////////////////////////////// Interface input class //////////////////////////

class InputControlProfiles;
class Interface
{
private:
 bool KeyInit(void);
 bool MouseInit(void);
 void Initialize(void);
public:
 HWND window;
#if defined(USE_WINTOUCH)
 // Picture of the Windows Touch interface state.
 WindowsTouch windowsTouch;
#endif
 // Touch information.
 Touches touches,touchFrame,wereTouches;
 // Detected Gamepads list.
 Gamepads gamepads;
 HINSTANCE instance;
 LPDIRECTINPUT8          input_main;               // global direct input object

 // Keyboard device.
 bool hasKeyboard;
 LPDIRECTINPUTDEVICE8    key_main;                 // keyboard device
 UCHAR                   key_data[256];            // key data for keyboard status
 UCHAR                   prev_key_data[256];       // key data for keyboard status
 double                  keypressed[256];          // key pressed duration data
 KeyList                 keys;                     // current keys-pressed in a list, updated every frame
 Zstring                 keyedInput;               // Buffer input method, clears when excessive or on demand

 double                  mousepressed[3];          // mouse pressed duration data
 bool hasMouse,hasTouch;
 LPDIRECTINPUTDEVICE8    mouse_main;               // mouse device
 int x,y,z;
 int oldx,oldy,oldz;
 double mxd,myd;
 float mx,my, oldmx,oldmy;
 int mxi,myi, oldmxi,oldmyi;
 int left, right, middle, wasLeft, wasRight, wasMiddle, recentLeft, recentRight, recentMiddle;
 bool leftDouble,rightDouble,middleDouble;
 NullTouchArea nullTouchArea;
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
#if defined(USE_WINTOUCH)
 Zint snapshotNumber;
 Zint upSnapshotNumber;
 Zbool oneTouchReleased;
#endif
 Zp<GLImage> cursorImage;
 Zp<Film> cursorFilm;
 bool hasJoystick;
 UINT    joy_num;    // number of joys
 JOYINFO joy_info;   // joy init info
 UINT    joy_ID;     // joy ID
 JOYCAPS joy_caps;   // joy capture info
 RECT    joy_trip;   // joy trip info
 DWORD joy_xcenter, joy_ycenter;
 int joy_left, joy_right, joy_up, joy_down, joy_but1, joy_but2, joy_but3, joy_but4;

 bool hasLogitech,useLogitech;    // LgKit1.1
 int MaxLED;          // Max LEDs on the device
 int LEDMask;         // LED bitmasks indicating status
 int MaxRes;          // Maximum precision
 int CurrentRes;      // Current resolution setting
 bool lifted;         // Has the 3d mouse been lifted?
 int lgError;         // last error notification from LgKit
 bool lgButton[14];   // Logitech buttons
 bool lgLED[7];       // Logitech LEDs

 Zbool hideCursor;
 
#if defined(USE_LOGITECH_KIT)
 // Logitech specific functions
 void printLGError( const char *function, int num );
 void lgInitAndRegister(void);
 LRESULT OnLgLiftNotification( WPARAM wParam, LPARAM );
 LRESULT OnLgButtonNotification( WPARAM wParam, LPARAM );
 void SetLED(void);
 void GetLED(void);
 void SetRes( int target );
 void GetRes(void);
 void lgUnregister();
#endif

 // Agnostic class methods

 Interface(void);
 Interface(HWND h, HINSTANCE i);
 ~Interface(void);

 // In App class called before Init, detects interface information for a particular display/screen.
 void Detect( Display *d );
 // Internally called to handle drawing of the GL Cursor.
 virtual void DrawGLCursor(void);
 // In App class it is called once per frame, takes a picture of the entire interface and updates all counters.
 void Snapshot(void);
 // 
 void Detach(void);

#if defined(USE_WINGESTURE)
private:
 POINT _ptFirst;        // first significant point of the gesture
 POINT _ptSecond;       // second significant point of the gesture
 DWORD _dwArguments;    // 4 bytes long argument
public:
 // Internally called to handle WinGesture library. (legacy, deprecated)
 LRESULT GestureProcess(HWND wnd, WPARAM w, LPARAM l);
 void GesturePress(const LONG lOx, const LONG lOy);
 void GesturePressAndTap(const double dAngle, const LONG lOx, const LONG lOy);
 void GestureTwoFingerTap(const double dDistance, const LONG lOx, const LONG lOy);
 void GestureZoom(const double dZoomFactor, const LONG lZx, const LONG lZy);
 void GestureMove(const LONG ldx, const LONG ldy);
 void GestureRotate(const double dAngle, const LONG lOx, const LONG lOy);
 void GesturePan(const double dDistance, const LONG lPx, const LONG lPy);
#endif

#if defined(USE_DIRECT_MOUSE)
 // Internally called to handle use of the direct mouse driver (Windows)
 void SetMousePosition( const int mx, const int my );
 void StorePreviousMouseState();
 void DirectMouse();
#endif

#if defined(USE_WINTOUCH)
 // Internally called to handle WinTouch library (introduced Win7)
 void WinTouchProcess(HWND wnd, WPARAM w, LPARAM l);
 void OnTouchDownHandler(HWND hWnd, const TOUCHINPUT& ti);
 void OnTouchMoveHandler(HWND hWnd, const TOUCHINPUT& ti);
 void OnTouchUpHandler(HWND hWnd, const TOUCHINPUT& ti);
#endif

#if defined(USE_WINGESTURE)
 // Internally called to handle WinGesture library (legacy, deprecated)
 void WinGestureMouse();
#endif
 bool Contact() {
  if ( hasTouch ) {
   return ( nullTouchArea.isTouching(mxi,myi) || KeyDown(LCtrl) );
  } else {
   return true; // mouse is hovering
  }  
 }
 // Information acquired via drivers and post calculation
 // Query Mouse: Is mousepressed[0] (time since) equal to the current FRAMETIME?
 bool LeftClicked()            { return mousepressed[0] == FRAMETIME; }
 // Called Internally: Left is now down. Is left mouse currently down and was down? recentLeft=true, doubleClickExpired = doubleClickFrames
 void LeftDown(void)           { left=1; if ( left && !wasLeft )    { recentLeft=true;   doubleclickExpired=doubleclickFrames; }  }
 // Simulated / Called Internally: wasLeft is set equal to left, left is now "up" (not down)
 void LeftUp(void)             { wasLeft=left;   left=0; }
 // Simulated / Called Internally: Left double click is now happening.
 void LeftDouble(void)         { leftDouble=true;  }
 // Query Mouse: Is mousepressed[1] (time since) equal to the current FRAMETIME?
 bool RightClicked()           { return mousepressed[1] == FRAMETIME; }
 // Simulated / Called Internally:  Right is now down.  Is right mouse current down and was down? recentRight=true, doubleclickExpired = doubleClickFrames
 void RightDown(void)          { right=true; if ( right && !wasRight ) { recentRight=true;   doubleclickExpired=doubleclickFrames; } }
 // Simulated, / Called Internally: Right is now up.
 void RightUp(void)            { wasRight=right; right=0; }
 // Simulated / Called Internally: Right doubleclick.
 void RightDouble(void)        { rightDouble=true; }
 // Query Mouse: Is mousepressed[2] (time since) equal to the current FRAMETIME?
 bool MiddleClicked()          { return mousepressed[2] == FRAMETIME; }
 // Simulated / Called Internally: Middle is now down.  Is Middle down and was down?  recentMiddle=true, doubleclickExpired = doubleClickFrames
 void MiddleDown(void)         { middle=true; if ( middle && !wasMiddle ) { recentMiddle=true;  doubleclickExpired=doubleclickFrames; } }
 // Simulated / Called Internally: Middle is now up.
 void MiddleUp(void)           { wasMiddle=middle; middle=0; }
 // Simulated / Called Internally: Middle was double clicked. (middleDouble)
 void MiddleDouble(void)       { middleDouble=true; }
 // Simulated / Called Internally: Mouse is moving to xPos, yPos
 void MouseMoved(int xPos, int yPos) {
  oldx=x;
  oldy=y;
  x=(int) (oldx-xPos);
  y=(int) (oldy-yPos);
  oldmx=mx; 
  oldmy=my;
  oldmxi=mxi;
  oldmyi=myi;
  mx+=((float)x * sensitivity);
  if ( mx < 0.0f ) mx=0.0f;
  else if ( mx > display->w ) mx=display->wf;
  my+=((float)y * sensitivity);
  if ( my < 0.0f ) my=0.0f;
  else if ( my > display->h ) my=display->hf;
  mxi=(int) mx; 
  myi=(int) my;
 }
 // Set Windows Managed Keydown
 void WMKeyDown( int code ) {
  prev_key_data[VK_to_DI[code]]=0;
  key_data[VK_to_DI[code]]=0x80;
 }
 // Set Windows Managed KeyUp
 void WMKeyUp( int code ) {
  prev_key_data[VK_to_DI[code]]=key_data[VK_to_DI[code]];
  key_data[VK_to_DI[code]]=0;
 }
 // Returns null when no key has been pressed.  Set noCaps=false to calculate current mode of letter
 char keyboard( bool noCaps=false ) {
  bool shifted=!noCaps&&(KeyDown(DX_LSHIFT)||KeyDown(DX_RSHIFT));//||KeyDown(DX_CAPITAL));
  if ( Keypress(DX_0) ) return shifted ? ')' : '0';
  if ( Keypress(DX_1) ) return shifted ? '!' : '1';
  if ( Keypress(DX_2) ) return shifted ? '@' : '2';
  if ( Keypress(DX_3) ) return shifted ? '#' : '3';
  if ( Keypress(DX_4) ) return shifted ? '$' : '4';
  if ( Keypress(DX_5) ) return shifted ? '%' : '5';
  if ( Keypress(DX_6) ) return shifted ? '^' : '6';
  if ( Keypress(DX_7) ) return shifted ? '&' : '7';
  if ( Keypress(DX_8) ) return shifted ? '*' : '8';
  if ( Keypress(DX_9) ) return shifted ? '(' : '9';
  if ( Keypress(DX_A) ) return shifted ? 'A' : 'a';
  if ( Keypress(DX_B) ) return shifted ? 'B' : 'b';
  if ( Keypress(DX_C) ) return shifted ? 'C' : 'c';
  if ( Keypress(DX_D) ) return shifted ? 'D' : 'd';
  if ( Keypress(DX_E) ) return shifted ? 'E' : 'e';
  if ( Keypress(DX_F) ) return shifted ? 'F' : 'f';
  if ( Keypress(DX_G) ) return shifted ? 'G' : 'g';
  if ( Keypress(DX_H) ) return shifted ? 'H' : 'h';
  if ( Keypress(DX_I) ) return shifted ? 'I' : 'i';
  if ( Keypress(DX_J) ) return shifted ? 'J' : 'j';
  if ( Keypress(DX_K) ) return shifted ? 'K' : 'k';
  if ( Keypress(DX_L) ) return shifted ? 'L' : 'l';
  if ( Keypress(DX_M) ) return shifted ? 'M' : 'm';
  if ( Keypress(DX_N) ) return shifted ? 'N' : 'n';
  if ( Keypress(DX_O) ) return shifted ? 'O' : 'o';
  if ( Keypress(DX_P) ) return shifted ? 'P' : 'p';
  if ( Keypress(DX_Q) ) return shifted ? 'Q' : 'q';
  if ( Keypress(DX_R) ) return shifted ? 'R' : 'r';
  if ( Keypress(DX_S) ) return shifted ? 'S' : 's';
  if ( Keypress(DX_T) ) return shifted ? 'T' : 't';
  if ( Keypress(DX_U) ) return shifted ? 'U' : 'u';
  if ( Keypress(DX_V) ) return shifted ? 'V' : 'v';
  if ( Keypress(DX_W) ) return shifted ? 'W' : 'w';
  if ( Keypress(DX_X) ) return shifted ? 'X' : 'x';
  if ( Keypress(DX_Y) ) return shifted ? 'Y' : 'y';
  if ( Keypress(DX_Z) ) return shifted ? 'Z' : 'z';
  if ( Keypress(DX_SPACE) ) return ' ';
  if ( Keypress(DX_LBRACKET) ) return shifted ? '{' : '[';
  if ( Keypress(DX_RBRACKET) ) return shifted ? '}' : ']';
  if ( Keypress(DX_SEMICOLON) ) return shifted ? ':' : ';';
  if ( Keypress(DX_COMMA) ) return shifted ? '<' : ',';
  if ( Keypress(DX_PERIOD) ) return shifted ? '>' : '.';
  if ( Keypress(DX_SLASH) ) return shifted ? '?' : '/';
  if ( Keypress(DX_MINUS) ) return shifted ? '_' : '-';
  if ( Keypress(DX_EQUALS) ) return shifted ? '+' : '=';
  if ( Keypress(DX_BACKSLASH) ) return shifted ? '|' : '\\';
  if ( Keypress(DX_APOSTROPHE) ) return shifted ? '"' : '\'';
  return '\0';
 }
 // Any Key Released or Hit
 bool AnyKey() {
  for ( int code=0; code<256; code++ ) if ( (( prev_key_data[code] & 0x80 ) && !( key_data[code] & 0x80 )) ) return true;
  if ( leftReleased() ) return true;
  if ( rightReleased() ) return true;
  if ( middleReleased() ) return true;
  if ( gamepads.AnyButton() ) return true;
  return false;
 }
 // Key Was Just Pressed (first frame)
 bool KeyHit( int code ) { return keypressed[code] == FRAMETIME; }
 // Key Was Just Pressed
 inline int Keypress( int code ) { return ( key_data[code] & 0x80 ) && !(prev_key_data[code] & 0x80); }
 // Key Was Just Pressed
 inline int KeyPressed( int code ) { return ( key_data[code] & 0x80 ) && !(prev_key_data[code] & 0x80); }
 // Key In Pressed State
 inline int KeyDown( int code ) { return ( key_data[code] & 0x80 ); }
 // Key Was Just Released
 inline int KeyUp( int code ) { return (( prev_key_data[code] & 0x80 ) && !( key_data[code] & 0x80 )); }
 // Key Was Just Released
 inline int KeyReleased( int code ) { return (( prev_key_data[code] & 0x80 ) && !( key_data[code] & 0x80 )); }
 // Set the size of the mouse cursor
 void SetCursorSize( float s );
 // Mouse Query: Any button down? (set includeMiddle to true if you want to include the middle button check)
 bool click( bool includeMiddle=false ) { return !clicked(includeMiddle) && clicking(includeMiddle); }
 // Mouse Query: Right been pressed this frame?
 bool rightPressed()  { /*if ( hasTouch ) return right>0;*/ return wasRight==0 && right>0;   }
 // Mouse Query: Right been pressed this frame?
 bool leftPressed()   { /*if ( hasTouch ) return left>0;*/ return wasLeft==0 && left>0;     }
 // Mouse Query: Right been pressed this frame?
 bool middlePressed() { return wasMiddle==0 && middle>0; }
 // Mouse Query: Any button down? (set includeMiddle to true if you want to include the middle button check)
 bool clicked( bool includeMiddle=false ) { return leftReleased() || rightReleased() || (includeMiddle && middleReleased()); }
 // Mouse Query: Right been released this frame?
 bool rightReleased()  { /*if ( hasTouch ) return right>0;*/ return wasRight>0 && right==0;   }
 // Mouse Query: Right been released this frame?
 bool leftReleased()   { /*if ( hasTouch ) return left>0;*/ return wasLeft>0 && left==0;     }
 // Mouse Query: Right been released this frame?
 bool middleReleased() { return wasMiddle>0 && middle==0; }
 // Mouse Query: Any button down? (set includeMiddle to true if you want to include the middle button check)
 bool clicking( bool includeMiddle=false ) { return left>0 || right>0 || (includeMiddle && middle>0); }
 // Mouse Query: Right been pressed this frame?
 bool rightDepressed()  { /*if ( hasTouch ) return right>0;*/ return right>0;   }
 // Mouse Query: Right been pressed this frame?
 bool leftDepressed()   { /*if ( hasTouch ) return left>0;*/ return left>0;     }
 // Mouse Query: Right been pressed this frame?
 bool middleDepressed() { return wasMiddle==0 && middle>0; }
 // Key Query: Control held
 bool CTRL() { return KeyDown(DX_LCTRL) || KeyDown(DX_RCTRL); }
 // Key Query: Alt held
 bool ALT() { return KeyDown(DX_LALT) || KeyDown(DX_RALT); }
 // Key Query: Shift held
 bool SHIFT() { return KeyDown(DX_LSHIFT) || KeyDown(DX_RSHIFT); }
};

// Is the mouse cursor within this area?
bool MOUSEWITHIN( int x, int y, int x2, int y2 );
// Is the mouse cursor within this area?
bool MOUSEWITHIN( float x, float y, float x2, float y2 );
// Is the mouse cursor within this area?
bool MOUSEWITHIN( double x, double y, double x2, double y2 );

extern Interface *inputPointer;
