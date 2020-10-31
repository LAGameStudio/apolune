
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#define WINDOWS_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
#include <Windows.h>   // include all the windows headers
#include <WindowsX.h>
#include <mmsystem.h>  // for joystick info
#include <dinput.h>

#include "macros.h"

#include "GLSetup.h"
#include "GLWindow.h"
#include "Interface.h"
#include "Crayon.h"

#include "CommandLine.h"

bool USE_SYSTEM_CURSOR_CONTROL = false;

#if defined(GL_DEBUG)
#include <stdio.h>
#endif

// Returns false if the controller has been disconnected
bool Gamepad::Refresh() {
 if ( connected ) {
  // Store previous state
  prevLeftStickX = leftStickX;
  prevLeftStickY = leftStickY;
  prevRightStickX = rightStickX;
  prevRightStickY = rightStickY;
  prevLeftTrigger = leftTrigger;
  prevRightTrigger = rightTrigger;
  previous = state; 
  ZeroMemory(&state, sizeof(XINPUT_STATE));
 }
 DegradeKeystrokes();
 if ( !Update() ) { Disconnected(); return false; }
 if (cId != -1) {
  connected=true;
  float normLX = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
  float normLY = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);
  leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
  leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));
  if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
  if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);
  float normRX = fmaxf(-1, (float) state.Gamepad.sThumbRX / 32767);
  float normRY = fmaxf(-1, (float) state.Gamepad.sThumbRY / 32767);
  rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
  rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));
  if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
  if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);
  leftTrigger = (float) state.Gamepad.bLeftTrigger / 255;
  rightTrigger = (float) state.Gamepad.bRightTrigger / 255;
  up=Button(XINPUT_GAMEPAD_DPAD_UP);
  down=Button(XINPUT_GAMEPAD_DPAD_DOWN);
  left=Button(XINPUT_GAMEPAD_DPAD_LEFT);
  right=Button(XINPUT_GAMEPAD_DPAD_RIGHT);
  start=Button(XINPUT_GAMEPAD_START);
  back=Button(XINPUT_GAMEPAD_BACK);
  a=Button(XINPUT_GAMEPAD_A);
  b=Button(XINPUT_GAMEPAD_B);
  x=Button(XINPUT_GAMEPAD_X);
  y=Button(XINPUT_GAMEPAD_Y);
  if ( vibrating ) {
   vibrationExpired+=FRAMETIME;
   if (vibrationExpired > vibrationDuration && vibrationDuration >= 0.0) {
    Vibrate(0,0);
    vibrating=false;
   }
  }
  return true;
 }
 return false;
}


//////////////////////////////////////////////////// KEYBOARD //////////////////////////////////////////////////////

WindowsKeyboard windowsKeyboard;

#define LOGITECH_DEBUG 1

bool use_lg_button_notifications=false;

// Key class

KEY::KEY(void) {
 prev=null;
 next=null;
 uc=0;
 ch=0;
 code=0;
 key= (Keys) 0;
}

KEY::KEY(int code) {
 prev=null;
 next=null;
 ch = codeToChar[code];
 this->code=code;
 key=(Keys) (uc=codeToUnichar[code]);
}

KEY::~KEY(void) {
}

// Key list class

bool KeyList::InList( int code )
{
 KEY *k;
 for ( k=(KEY *) first; k; k=(KEY *) (k->next) ) if ( k->code == code ) return true;
 return false;
}

bool KeyList::CharInList( char ch ) 
{
 KEY *k;
 for ( k=(KEY *) first; k; k=(KEY *) (k->next) ) if ( k->ch == ch ) return true;
 return false;
}

// Interface class

void Interface::Initialize(void)
{
 window=null;
 instance=null;
 input_main=null;

 hasKeyboard=false;
 key_main = null;
 ZeroMemory(key_data,sizeof(UCHAR[256]));
 ZeroMemory(prev_key_data,sizeof(UCHAR[256]));
 ZeroMemory(keypressed,sizeof(double[256]));
 ZeroMemory(mousepressed,sizeof(double[3]));
 hasMouse=false;
 hasTouch=false;
 mouse_main=null;
 x=y=z=oldx=oldy=oldz=0;
 oldmx=oldmy=mx=my=0.0f;
 mxd=myd=0.0;
 mxi=myi=0;
 left=right=middle=wasLeft=wasRight=wasMiddle=recentLeft=recentRight=recentMiddle=wheelUp=wheelDown=0;
// mouse_data;
 doubleclickFrames=30; // approximately 60 FPS, so 0.5 second on the double click memory
 doubleclickExpired=0;
 sensitivity=1.5f;
 cursor=true;
 leftHanded=false;
 SetCursorSize(2.0f);
 color.Float(0.0f,1.0f,0.0f,0.8f);
 leftDouble=rightDouble=middleDouble=false;

 hasJoystick=false;
 joy_num=0; // add multiple joystick support
// joy_info;
 joy_ID=0;
// joy_caps;
// joy_trip;
 joy_xcenter=joy_ycenter=0;
 joy_left=joy_right=joy_up=joy_down=joy_but1=joy_but2=joy_but3=joy_but4=0;

 hasLogitech=false;
 useLogitech=true;
 MaxLED=0;
 LEDMask=0; 
 MaxRes=0;
 CurrentRes=0;
 lifted=false;
 lgError=0;
 for ( int x=0; x<14; x++ ) lgButton[x]=false;
 for ( int x=0; x<7; x++ ) lgLED[x]=false;
}

Interface::Interface(void) {
 Initialize();
}

Interface::Interface(HWND h, HINSTANCE i) {
 Initialize();
 window=h;
 instance=i;
}

Interface::~Interface(void)
{
}

/////////////////////////////////////////////////////////////
// Logitech SDK 1.1 Support
/////////////////////////////////////////////////////////////

#if defined(USE_LOGITECH_KIT)

void Interface::printLGError( const char *function, int num ) {
 switch ( num ) {
 case LK_ERROR_NO_LOGITECH_LIB_FOUND: 
   OUTPUT("Interface::%s:lgError: LgKitApi.h did not locate the Logitech Library, is MSI redistributable installed?\n", function );
  break;
 case LK_ERROR_WRONG_LOGITECH_LIB_FOUND:
   OUTPUT("Interface::%s:lgError: LgKitApi.h did not locate a correct version of the Logitech Library\n", function );
  break;
 case LK_ERROR_NOT_IMPLEMENTED:
   OUTPUT("Interface::%s:lgError: This one you should not see :) [verbatim error code v1.1]\n", function );
  break;
 case LK_ERROR_MULTIPLE_REGISTRATION_NOT_AUTHORIZED:
   OUTPUT("Interface::%s:lgError: LgKit.dll does not support the same app registering multiple times for some events (buttons).\n", function );
  break;
 case LK_ERROR_INTERNAL_KHAL:
   OUTPUT("Interface::%s:lgError: This will be returned if one of the underlying Logitech functions fail.  Inform Logitech immediately.\n", function );
  break;
 case LK_ERROR_INVALID_PARAMETERS:
   OUTPUT("Interface::%s:lgError: This will be returned when a Logitech Library function is supplied an invalid parameter, such as an invalid window handle (HWND).\n", function );
  break;
 case 0: break;
 case LK_INFO_NO_MOUSE_WITH_LED_FOUND:
   OUTPUT("Interface::%s:lgError: Currently no mice with programmable LEDs found that can be controlled.\n", function );
  break;
 case LK_INFO_NO_MOUSE_FOUND:
   OUTPUT("Interface::%s:lgError: No Logitech mouse found.\n", function );
  break;
// case LK_INFO_NO_MOUSE_WITH_MAXRES_FOUND:
//   OUTPUT("Interface::%s:lgError: no Logitech mouse supporting this resolution is connected at the time of call. Other mice will be set to their highest possible resolution.\n" );
//  break;
 default:
   OUTPUT("Interface::%s:lgError: %d (not a documented error, could it be LK_INFO_NO_MOUSE_WITH_MAXRES_FOUND?)\n", function, num);
  break;
 }
}

void Interface::lgInitAndRegister() {

 // Init Logitech Input Devices SDK
 lgError=LgKitInit();
 printLGError("lgInitAndRegister",lgError);
 if ( lgError < 0 ) return;

 // After getting an error code, we stopped using this.
 // To turn it back on, the command line "-lgbuttons" must be used.
	lgError=
  LgKitRegisterMouseButtonsNotifications(window,
   WM_USER+432, true ) ;  
 printLGError("lgInitAndRegister",lgError);
    // WM_USER+432 is from LgKitTest
    // true indicates all buttons will notify,
    // may create duplicate notifications
    // thus, to receive events for all mouse buttons,
    // the OS will nolonger receive buttons 1 to 5.
 if ( lgError == LK_INFO_NO_MOUSE_FOUND ) {
  OUTPUT("No Logitech devices were detected.\n");
  return;
 } else hasLogitech=true;
 OUTPUT("Logitech device detected.\n");

 lgError=
	 LgKitRegisterLiftDetectionNotifications(window,WM_USER+433) ;
 printLGError("lgInitAndRegister",lgError);
   // WM_USER+433 is from LgKitTest

 //lgError=
 // LgKitRegisterGestureNotifications(window, WM_USER+434, true, 1) ;
 // bSoundFeedback, nOrientation=0 – Horizontal 1 – Vertical

 lgError = LgKitGetLEDNumber(&MaxLED) ;
	if (lgError < 0)	MaxLED = 0 ;
}

LRESULT Interface::OnLgLiftNotification( WPARAM wParam, LPARAM )
{
	BOOL boLifted= ( (BOOL) wParam );
 if ( boLifted ) lifted=true;
 else lifted=false;
	return TRUE ;
}

LRESULT Interface::OnLgButtonNotification( WPARAM wParam, LPARAM )
{
	PLK_MOUSE_INPUT pLKMI = (PLK_MOUSE_INPUT) wParam ;

	if ((pLKMI->wButtonNumber > 0) 
		&& (pLKMI->wButtonNumber < 11))
	{
		int button = (int) (pLKMI->wButtonNumber - 1);
  lgButton[button]=(LK_UP == pLKMI->ButtonStatus);
#if LOGITECH_DEBUG
  OUTPUT("Interface::OnLgButtonNotification: Button %d was %s\n",
   (int) button, (lgButton[button]?"pressed":"released"));
#endif
	}
	return TRUE ;
}

void Interface::SetLED() {
 if ( !hasLogitech || MaxLED==0 ) return;
 int setting=0;
 if ( lgLED[0] ) setting |=0x0001;
 if ( lgLED[1] ) setting |=0x0002;
 if ( lgLED[2] ) setting |=0x0004;
 if ( lgLED[3] ) setting |=0x0008;
 if ( lgLED[4] ) setting |=0x0010;
 if ( lgLED[5] ) setting |=0x0020;
// if ( lgLED[6] ) setting |=0x0030;
 LEDMask=setting;
	lgError = LgKitSetLED(LEDMask) ;
 printLGError("SetLED",lgError);
}

void Interface::GetLED() {
 if ( !hasLogitech ) return;
 lgError = LgKitGetLEDNumber(&LEDMask) ;
 if ( LEDMask | 0x0001 ) lgLED[0]=true;
 if ( LEDMask | 0x0002 ) lgLED[1]=true;
 if ( LEDMask | 0x0004 ) lgLED[2]=true;
 if ( LEDMask | 0x0008 ) lgLED[3]=true;
 if ( LEDMask | 0x0010 ) lgLED[4]=true;
 if ( LEDMask | 0x0020 ) lgLED[5]=true;
// if ( LEDMask | 0x0040 ) lgLED[6]=true;
 printLGError("GetLED",lgError);
}

void Interface::SetRes( int target ) {
 if ( !hasLogitech ) return;
 if ( target < 400 ) target=400;
 lgError = LgKitSetCurrentResolution(target) ;
 printLGError("SetRes",lgError);
}

void Interface::GetRes(void) {
 if ( !hasLogitech ) return;
 lgError = LgKitGetCurrentResolution(CurrentRes) ;
}

void Interface::lgUnregister() {
 if ( !hasLogitech ) return;
 lgError = LgKitUnRegisterLiftDetectionNotifications() ;
 printLGError("lgUnregister",lgError);
 lgError = LgKitUnRegisterMouseButtonsNotifications() ;
 printLGError("lgUnregister",lgError);
}

#endif

/////////////////////////////////////////////////////////////
// DirectInput Keyboard initialization
/////////////////////////////////////////////////////////////

bool Interface::KeyInit(void)
{
    // Direct Input object succeeded so create device
   if (IDirectInput8_CreateDevice(input_main, GUID_SysKeyboard, &key_main, NULL) != DI_OK)
   {
      MessageBox(window, "Keyboard input could not be initialized -- Create Device Error","Input Error",MB_OK);
      return FALSE;
   }

   // Set coop level for device
   if (IDirectInputDevice8_SetCooperativeLevel(key_main, window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
   {
      MessageBox(window, "Keyboard could not be initialized -- Cooperative Level Error","Input Error",MB_OK);
      return FALSE;
   }

   // Set data format for device
   if (IDirectInputDevice8_SetDataFormat(key_main, &c_dfDIKeyboard) != DI_OK)
   {
      MessageBox(window, "Keyboard could not be initialized -- Set Data Format Error","Input Error",MB_OK);
      return FALSE;
   }

   // Acquire device
   if (IDirectInputDevice8_Acquire(key_main) != DI_OK)
   {
      MessageBox(window, "Keyboard could not be initialized -- Could Not Acquire Error","Input Error",MB_OK);
      return FALSE;
   }

   // keyboard init DI_OK and acquired
   return TRUE;
}


//////////////////////////////////////////////////////////////
// DirectInput Mouse initialization
//////////////////////////////////////////////////////////////

bool Interface::MouseInit(void)
{
   if ( CmdLine.Option("-use-system-mouse") ) {
    OUTPUT("Command Line Option:   USE SYSTEM MOUSE\n");
    USE_SYSTEM_CURSOR_CONTROL=true;
   }

   // Direct Input object succeeded so create device
   if (IDirectInput8_CreateDevice(input_main, GUID_SysMouse, &mouse_main, NULL) != DI_OK)
   {
      MessageBox(window, "Mouse input could not be initialized -- Create Device Error","Input Error",MB_OK);
      return FALSE;
   }

   // Set coop level for devices
   if (IDirectInputDevice8_SetCooperativeLevel(mouse_main, window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
   {
      MessageBox(window, "Mouse could not be initialized -- Cooperative Level Error","Input Error",MB_OK);
      return FALSE;
   }

   // Set data format for device
   if (IDirectInputDevice8_SetDataFormat(mouse_main, &c_dfDIMouse) != DI_OK)
   {
      MessageBox(window, "Mouse could not be initialized -- Set Data Format Error","Input Error",MB_OK);
      return FALSE;
   }

   if (IDirectInputDevice8_Acquire(mouse_main) != DI_OK)
   {
      MessageBox(window, "Mouse could not be initialized -- Could Not Acquire Error","Input Error",MB_OK);
      return FALSE;
   }

   // mouse init DI_OK and acquired
   return TRUE;
}

void Interface::Detect( Display *d )
{
   display=d;
   mx= display->wf/2.0f;
   my= display->hf/2.0f;
   // Create Direct Input main object

#if USE_DIRECT_JOY || USE_DIRECT_KEY || USE_DIRECT_MOUSE
   if (DirectInput8Create(this->instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&input_main, NULL) != DI_OK)
   {
      MessageBox(window, "Input could not be initialized -- Direct Input Create Error","Input Error",MB_OK);
      return;
   }
#endif

#if USE_DIRECT_KEY
   hasKeyboard   = KeyInit();
#else
   hasKeyboard=true;
#endif

#if USE_DIRECT_MOUSE
   hasMouse      = MouseInit();
#else
   hasMouse=true;
#endif

#if USE_LOGITECH_KIT
   if ( useLogitech ) lgInitAndRegister();
#else
   hasLogitech=false;
#endif

//   if (mouse_ok) MessageBox(window,"Mouse DI_OK.","OK",MB_OK);
//   if (key_ok) MessageBox(window,"Keyboard DI_OK.","OK",MB_OK);

   // initialize joystick
#if USE_DIRECT_JOY      
   // make sure joystick driver is present
   joy_num = joyGetNumDevs();
   if ( joy_num == 0 ) { hasJoystick=false; return; }

   // make sure the joystick is attached
   if (joyGetPos(JOYSTICKID1, &joy_info) != JOYERR_UNPLUGGED) joy_ID = JOYSTICKID1;
   else { hasJoystick=false; return; }

   // calculate the trip values
   joyGetDevCaps(joy_ID, &joy_caps, sizeof(JOYCAPS));
   joy_xcenter  = ((DWORD)joy_caps.wXmin + joy_caps.wXmax) / 2;
   joy_ycenter  = ((DWORD)joy_caps.wYmin + joy_caps.wYmax) / 2;
   joy_trip.left    = (joy_caps.wXmin + (WORD)joy_xcenter) / 2;
   joy_trip.right   = (joy_caps.wXmax + (WORD)joy_xcenter) / 2;
   joy_trip.top     = (joy_caps.wYmin + (WORD)joy_ycenter) / 2;
   joy_trip.bottom  = (joy_caps.wYmax + (WORD)joy_ycenter) / 2;

   // capture the joystick
   joySetCapture(window, joy_ID, NULL, TRUE);
#endif

#if defined(USE_WINTOUCH)
  // test for touch
  int value = GetSystemMetrics(SM_DIGITIZER);
  if (value & NID_READY){ 
   OUTPUT("The input digitizer is ready for input.\n");
   windowsTouch.ready=true;
  } else {
   OUTPUT("NID_READY is not set: If this value is unset, it may mean that the tablet service is stopped, the digitizer is not supported, or digitizer drivers have not been installed.\n");
  }
  if (value  & NID_MULTI_INPUT) {
   OUTPUT("Multitouch available.\n");
   windowsTouch.multitouch=true;
  }
  if (value & NID_INTEGRATED_TOUCH){
   OUTPUT("Integrated touch surface detected.\n");
   windowsTouch.integrated=true;
   hasTouch=true;
  }
  if (value & NID_EXTERNAL_TOUCH){
   OUTPUT("External touch surface detected.\n");
   windowsTouch.external=true;
  }
  if (value & NID_INTEGRATED_PEN){
   OUTPUT("Integrated pen digitizer detected.\n");
   windowsTouch.integrated=true;
   windowsTouch.pen=true;
  }
  if (value & NID_EXTERNAL_PEN){
   OUTPUT("External touch pen detected.\n");
   windowsTouch.external=true;
   windowsTouch.pen=true;
  }
  if ( windowsTouch.ready ) {
   ULONG flag=0;
   if ( CmdLine.Option("-finetouch") ) {
    OUTPUT("Interface.Detect() forcing TWF_FINETOUCH option by command line.");
    flag &= TWF_FINETOUCH;
   }
   if ( CmdLine.Option("-wantpalm") ) {
    OUTPUT("Interface.Detect() forcing TWF_WANTPALM option by command line.");
    flag &= TWF_WANTPALM;
   }
#if defined(USE_WINTOUCH)
   if ( !CmdLine.Option("-wm_gesture") ) {
    if ( !RegisterTouchWindow(window, flag) ) {
     OUTPUT("Interface::Detect() Cannot register application window for multi-touch input.\n");
    } else windowsTouch.registered=true;
   }
#endif
  }
#endif
}

void Interface::SetCursorSize( float s ) {
 size=s;
 ofsx=(int) (5.0f * size);
 ofsy=(int) (7.0f * size);
 ofsx2=(int) (7.0f * size);
 ofsy2=(int) (2.0f * size);
 if ( leftHanded ) {
  ofsx=-ofsx;
  ofsy=-ofsy;
  ofsx2=-ofsx2;
  ofsy2=-ofsy2;
 }
}

int alpha=0;

#include "Film.h"
#include "GLImage.h"

#include "GameOptions.h"

Zint cursorFrame;

void Gamepad::Vibrate(int leftVal, int rightVal, double duration) {
 if ( options.vibrate ) {
 	XINPUT_VIBRATION Vibration;
 	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
 	Vibration.wLeftMotorSpeed = leftVal;
 	Vibration.wRightMotorSpeed = rightVal;
 	XInputSetState((DWORD)(int)cId, &Vibration);
  vibrating=true;
  vibrationDuration=duration;
 }
}

void Interface::DrawGLCursor(void) {
 if ( USE_SYSTEM_CURSOR_CONTROL ) return;
 if ( hideCursor ) return;
 if ( cursorImage ) {
  StretchdNoAlpha(cursorImage,crayons.Pick(alabaster),transparency,
   (double)mx,(double)my,
   cursorImage->width,cursorImage->height
  );
  return;
 } else if ( cursorFilm ) {
  cursorFrame++;
  GLImage *c=cursorFilm->frame((int)cursorFrame);
  if ( c ) {
   StretchiNoAlphaXYPartial(c,crayons.Pick(alabaster),transparency,
    (int)mx,(int)my,c->width,c->height,0.0,0.98,0.0,1.0
   );
   return;
  }
 }
 alpha+=5; alpha%=196; alpha+=64;
#if defined(GL_DEBUG)
 GL_Assert( "Interface::DrawGLCursor" );
 GLint err = glGetError();
 if ( err != GL_NO_ERROR ) glReportError(err);
#endif
 // Draw mouse cursor
 glEnable(GL_BLEND);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_TEXTURE_2D);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
 glEnable(GL_MULTISAMPLE_ARB);
 if ( KeyDown(DX_LCTRL) ) color.Pick(skyBlue); else color.floats[3]=alpha/255.0f;
 glColor4fv(color.floats);
 glBegin(GL_POLYGON);
 glVertex2f(mx,my);
 glVertex2f(mx+ofsx*display->wf/1920.0f,my+ofsy*display->hf/1080.0f);
 glVertex2f(mx+ofsx2*display->wf/1920.0f,my+ofsy2*display->hf/1080.0f);
 glEnd();
 glColor4f(0.0f,0.0f,0.0f,1.0f);
 glBegin(GL_LINES);
 glVertex2f(mx,my);
 glVertex2f(mx+ofsx*display->wf/1920.0f,my+ofsy*display->hf/1080.0f);
 glEnd();
 glBegin(GL_LINES);
 glVertex2f(mx+ofsx*display->wf/1920.0f,my+ofsy*display->hf/1080.0f);
 glVertex2f(mx+ofsx2*display->wf/1920.0f,my+ofsy2*display->hf/1080.0f);
 glEnd();
 glBegin(GL_LINES);
 glVertex2f(mx+ofsx2*display->wf/1920.0f,my+ofsy2*display->hf/1080.0f);
 glVertex2f(mx,my);
 glEnd();
 glDisable(GL_MULTISAMPLE_ARB);
 glDisable(GL_BLEND);
#if defined(GL_DEBUG)
 GL_Assert( "Interface::DrawGLCursor (exiting)" );
 err = glGetError();
 if ( err != GL_NO_ERROR ) glReportError(err);
#endif
}

// Called once per frame
void Interface::Snapshot(void) {
 gamepads.Between();
 #if defined(USE_WINTOUCH)
   if ( hasTouch ) {
    // Clear old releases
    if ( oneTouchReleased && upSnapshotNumber != snapshotNumber ) {
     oneTouchReleased=false;
     wasLeft=0;
     // Move mouse off screen to NullTouchArea touchNullArea;
     SetMousePosition(nullTouchArea.x,nullTouchArea.y);
    }
    snapshotNumber++;
    // Clear old touch data.
    wereTouches.Clear();
    touches.ClearExpired(&wereTouches);
    touchFrame.Clear();
   }
#endif
   leftDouble=rightDouble=false;
#if USE_DIRECT_KEY
   //Keyboard
   memcpy(prev_key_data,key_data,sizeof(UCHAR[256]));
   if (IDirectInputDevice8_GetDeviceState(key_main, sizeof(UCHAR[256]), (LPVOID)key_data) == DI_OK)
   {
    hasKeyboard=true;
   } else
   {
    memset(key_data,0,sizeof(key_data)); // clear key data
    hasKeyboard=false;
   }
#endif
#if defined(USE_WINGESTURE) && defined(USE_DIRECT_MOUSE)
   if ( !hasTouch ) DirectMouse();
   else WinGestureMouse();
#elif defined(USE_WINGESTURE)
   WinGestureMouse();
#elif defined(USE_DIRECT_MOUSE)
   DirectMouse();
#endif

   // Key press duration, list
   keys.Clear();
   for ( int i=0; i<256; i++ ) {
    if ( key_data[i] ) {
      keypressed[i] = keypressed[i]+FRAMETIME;
      keys.Add(i);
    } else {
      keypressed[i] = 0.0;
    }
   }

   // Mouse press duration
   mousepressed[0] = recentLeft ? 0.0 : mousepressed[0]+FRAMETIME;
   mousepressed[1] = recentRight ? 0.0 : mousepressed[1]+FRAMETIME;
   mousepressed[2] = recentMiddle ? 0.0 : mousepressed[2]+FRAMETIME;
}

#if defined(USE_DIRECT_MOUSE)
void Interface::SetMousePosition( const int nmx, const int nmy ) {
 StorePreviousMouseState();
 mx=(float)nmx;
 my=(float)nmy;
 mxd=(double)nmx/display->wd;
 myd=(double)nmy/display->hd;
 mxi=nmx;
 myi=nmy; 
}

void Interface::StorePreviousMouseState() {
 oldx=x;
 oldy=y;
 oldz=z;
 oldmx=mx; 
 oldmy=my;
 oldmxi=mxi;
 oldmyi=myi;
 wasLeft=left;
 wasRight=right;
 wasMiddle=middle;
}

extern Zbool mouseEscapes;

void Interface::DirectMouse() {
    //Mouse
   if (IDirectInputDevice8_GetDeviceState(mouse_main, sizeof(DIMOUSESTATE), (LPVOID)&mouse_data) == DI_OK)
   {

//constants for mouse buttons
#define DIMOUSE_LEFTBUTTON   0
#define DIMOUSE_RIGHTBUTTON  1
#define DIMOUSE_MIDDLEBUTTON 2
#define DIMOUSE_4BUTTON      3
#define DIMOUSE_5BUTTON      4
#define DIMOUSE_6BUTTON      5
#define DIMOUSE_7BUTTON      6
#define DIMOUSE_8BUTTON      7

    hasMouse=true;
    wheelUp=wheelDown=false;
    StorePreviousMouseState();
    x=(int) (mouse_data.lX);
    y=(int) (mouse_data.lY);
    z=(int) (mouse_data.lZ);
	if (mouseEscapes) {} else {
     mx += ((float)x * sensitivity); if (mx < 0.0f) mx = 0.0f; else if (mx > display->w) mx = display->wf;
     my += ((float)y * sensitivity); if (my < 0.0f) my = 0.0f; else if (my > display->h) my = display->hf;
     mxd = (double)mx / display->wd;
     myd = (double)my / display->hd;
     mxi = (int)mx;
     myi = (int)my;
    }
    if ( z < 0 ) wheelDown=true;
    if ( z > 0 ) wheelUp=true;
    if ( doubleclickExpired ) { doubleclickExpired--; } else { recentLeft=recentRight=recentMiddle=false; }
#if USE_LOGITECH_KIT
    left= (mouse_data.rgbButtons[0] & 0x80) || lgButton[0];
    right= (mouse_data.rgbButtons[1] & 0x80) || lgButton[1];
    middle= (mouse_data.rgbButtons[2] & 0x80) || lgButton[2];
#else
    left= (mouse_data.rgbButtons[0] & 0x80);
    right= (mouse_data.rgbButtons[1] & 0x80);
    middle= (mouse_data.rgbButtons[2] & 0x80);
#endif
    if ( left   && !wasLeft )   { recentLeft=true;   doubleclickExpired=doubleclickFrames; }
    if ( right  && !wasRight )  { recentRight=true;  doubleclickExpired=doubleclickFrames; }
    if ( middle && !wasMiddle ) { recentMiddle=true; doubleclickExpired=doubleclickFrames; }
   } // end if mouse is DI_OK
   else
   {
//      sprintf(text,"ERROR: Mouse unplugged or could not be initialized");
//      TextOut(game_dc, 25, 200, text, strlen(text));
      memset(&mouse_data,0,sizeof(mouse_data));  // clear mouse data
      hasMouse=false;
   } // end else
}
#endif

#if defined(USE_WINGESTURE)
// Map the Windows Touch data to the mouse
void Interface::WinGestureMouse() {
 if ( windowsTouch.registered ) {
  if ( KeyDown(DX_LCTRL) ) DirectMouse();
  else {}
 }
 else if ( touches.count == 0 && KeyDown(DX_LCTRL) ) DirectMouse();
 else {
//  wasLeft=left;
//  left=0; // Equivalent of  StorePreviousMouseState();
  EACH(touches.first,Touch,t) {
   switch ( t->type ) {
    case gesture_none:
     break;
    case gesture_press:
      SetMousePosition((int)t->x,(int)t->y);
      left=1;
     break;
    case gesture_pressAndTap:
      SetMousePosition((int)t->x,(int)t->y);
      left=1;
     break;
    case gesture_twoFingerTap:
      SetMousePosition((int)t->x,(int)t->y);
      left=1;
     break;
    case gesture_Move:
     break;
    case gesture_Zoom:
     break;
    case gesture_Rotate:
     break;
    case gesture_Pan:
      SetMousePosition((int)t->x,(int)t->y);
      left=1;
     break;
   }
  }
 }
}
#endif

//////////////////////////////////////////////////////////////
// Windows Multitouch Events - based on MSDN
//////////////////////////////////////////////////////////////
#if defined(USE_WINTOUCH)
void Interface::WinTouchProcess(HWND hWnd, WPARAM wParam, LPARAM lParam) {
 // WM_TOUCH message can contain several messages from different contacts packed together.
 // Message parameters need to be decoded:
 unsigned int numInputs = (unsigned int) wParam; // Number of actual per-contact messages
 TOUCHINPUT* ti = new TOUCHINPUT[numInputs]; // Allocate the storage for the parameters of the per-contact messages
 if (ti == NULL) {
  OUTPUT("Could not allocate or locate TOUCHINPUT\n");
  return;
 }
 int index = GetContactIndex(ti->dwID);
// OUTPUT("TouchProcess: index was %d\n", (int) index );
 // Unpack message parameters into the array of TOUCHINPUT structures, each representing a message for one single contact.
 if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, ti, sizeof(TOUCHINPUT))) {
  // For each contact, dispatch the message to the appropriate message handler.
  for (unsigned int i = 0; i < numInputs; ++i) {
   if (ti[i].dwFlags & TOUCHEVENTF_DOWN) OnTouchDownHandler(hWnd, ti[i]);
   else if (ti[i].dwFlags & TOUCHEVENTF_MOVE) OnTouchMoveHandler(hWnd, ti[i]);
   else if (ti[i].dwFlags & TOUCHEVENTF_UP) OnTouchUpHandler(hWnd, ti[i]);
  }
 }
 CloseTouchInputHandle((HTOUCHINPUT)lParam);
 delete [] ti;
}


// Extracts contact point in client area coordinates (pixels) from a TOUCHINPUT
// structure. TOUCHINPUT structure uses 100th of a pixel units.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
// returns:
//      POINT with contact coordinates
POINT GetTouchPoint(HWND hWnd, const TOUCHINPUT& ti)
{
    POINT pt;
    pt.x = ti.x / 100;
    pt.y = ti.y / 100;
    ScreenToClient(hWnd, &pt);
    return pt;
}

// Extracts contact ID from a TOUCHINPUT structure.
// in:
//      ti          TOUCHINPUT structure (info about contact)
// returns:
//      ID assigned to the contact
inline int GetTouchContactID(const TOUCHINPUT& ti)
{
    return ti.dwID;
}

///////////////////////////////////////////////////////////////////////////////
// WM_TOUCH message handlers

// Handler for touch-down message.
// Starts a new stroke and assigns a color to it.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void Interface::OnTouchDownHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: point of contact and ID
    POINT pt = GetTouchPoint(hWnd, ti);
    int iCursorId = GetTouchContactID(ti);

    // We have just started a new stroke.
    left=1;
    SetMousePosition((int) pt.x,(int) pt.y);
}

// Handler for touch-move message.
// Adds a point to the stroke in drawing and draws new stroke segment.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void Interface::OnTouchMoveHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: contact ID
    int iCursorId = GetTouchContactID(ti);

    // Extract contact info: contact point
    POINT pt;
    pt = GetTouchPoint(hWnd, ti);
    SetMousePosition((int) pt.x,(int) pt.y);
}

// Handler for touch-up message.
// Finishes the stroke and moves it to the collection of finished strokes.
// in:
//      hWnd        window handle
//      ti          TOUCHINPUT structure (info about contact)
void Interface::OnTouchUpHandler(HWND hWnd, const TOUCHINPUT& ti)
{
    // Extract contact info: contact ID
    int iCursorId = GetTouchContactID(ti);

    // Extract contact info: contact point
    POINT pt;
    pt = GetTouchPoint(hWnd, ti);

    left=0;
    wasLeft=1;
    upSnapshotNumber=snapshotNumber;
    oneTouchReleased=true;
    SetMousePosition((int) pt.x,(int) pt.y);
}


#endif

#if defined(USE_WINGESTURE)
LRESULT Interface::GestureProcess(HWND hWnd, WPARAM wParam, LPARAM lParam) {

    // helper variables
    POINT ptZoomCenter;
    double k;

    GESTUREINFO gi;
    gi.cbSize = sizeof(gi);
    BOOL bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);

    if (!bResult)
    {
        OUTPUT("Error in execution of GetGestureInfo");
        return FALSE;
    }

    switch (gi.dwID)
    {
    case GID_BEGIN:
            _dwArguments = LODWORD(gi.ullArguments);
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd,&_ptFirst);
            GesturePress(_ptFirst.x,_ptFirst.y);
        break;

    case GID_END:
        break;
    
    case GID_ZOOM:
        switch (gi.dwFlags)
        {
        case GF_BEGIN:
            _dwArguments = LODWORD(gi.ullArguments);
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd,&_ptFirst);
            break;

        default:
            // We read here the second point of the gesture. This is middle point between 
            // fingers in this new position.
            _ptSecond.x = gi.ptsLocation.x;
            _ptSecond.y = gi.ptsLocation.y;
            ScreenToClient(hWnd,&_ptSecond);

            // We have to calculate zoom center point 
            ptZoomCenter.x = (_ptFirst.x + _ptSecond.x)/2;
            ptZoomCenter.y = (_ptFirst.y + _ptSecond.y)/2;           
            
            // The zoom factor is the ratio between the new and the old distance. 
            // The new distance between two fingers is stored in gi.ullArguments 
            // (lower DWORD) and the old distance is stored in _dwArguments.
            k = (double)(LODWORD(gi.ullArguments))/(double)(_dwArguments);

            // Now we process zooming in/out of the object
            GestureZoom(k, ptZoomCenter.x, ptZoomCenter.y);

            //InvalidateRect(hWnd, NULL, TRUE);

            // Now we have to store new information as a starting information 
            // for the next step in this gesture.
            _ptFirst = _ptSecond;
            _dwArguments = LODWORD(gi.ullArguments);
            break;
        }
        break;
    
    case GID_PAN:
        switch (gi.dwFlags)
        {
        case GF_BEGIN:
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd, &_ptFirst);
            break;

        default:
            // We read the second point of this gesture. It is a middle point
            // between fingers in this new position
            _ptSecond.x = gi.ptsLocation.x;
            _ptSecond.y = gi.ptsLocation.y;
            ScreenToClient(hWnd, &_ptSecond);

            // We apply move operation of the object
            GestureMove(_ptSecond.x-_ptFirst.x, _ptSecond.y-_ptFirst.y);

            //InvalidateRect(hWnd, NULL, TRUE);

            // We have to copy second point into first one to prepare
            // for the next step of this gesture.
            _ptFirst = _ptSecond;
            break;
        }
        break;

    case GID_ROTATE:
        switch (gi.dwFlags)
        {
        case GF_BEGIN:
            _dwArguments = 0;
            break;

        default:
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd, &_ptFirst);           
            // Gesture handler returns cumulative rotation angle. However we
            // have to pass the delta angle to our function responsible 
            // to process the rotation gesture.
            GestureRotate(
                GID_ROTATE_ANGLE_FROM_ARGUMENT(LODWORD(gi.ullArguments)) 
                - GID_ROTATE_ANGLE_FROM_ARGUMENT(_dwArguments),
                _ptFirst.x,_ptFirst.y
            );
            //InvalidateRect(hWnd, NULL, TRUE);
            _dwArguments = LODWORD(gi.ullArguments);
            break;
        }
        break;

    case GID_TWOFINGERTAP:
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd, &_ptFirst);  
        GestureTwoFingerTap((double)(gi.ullArguments),
                _ptFirst.x,_ptFirst.y);
        //InvalidateRect(hWnd, NULL, TRUE);
        break;

    case GID_PRESSANDTAP:
        switch (gi.dwFlags)
        {
        case GF_BEGIN:
            _ptFirst.x = gi.ptsLocation.x;
            _ptFirst.y = gi.ptsLocation.y;
            ScreenToClient(hWnd, &_ptFirst);  
            GesturePressAndTap(
                GID_ROTATE_ANGLE_FROM_ARGUMENT(LODWORD(gi.ullArguments)) 
                - GID_ROTATE_ANGLE_FROM_ARGUMENT(_dwArguments),
                _ptFirst.x,_ptFirst.y);
            //InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    }

    CloseGestureInfoHandle((HGESTUREINFO)lParam);

    return TRUE;
}

void Interface::GesturePress(const LONG lOx, const LONG lOy) {
  Touch *t=new Touch;
  t->type = gesture_press;
  t->x=(int) lOx;
  t->y=(int) lOy;
  touches.Append(t);
  OUTPUT("GesturePress\n");
}

void Interface::GesturePressAndTap(const double dAngle, const LONG lOx, const LONG lOy) {
  Touch *t=new Touch;
  t->type = gesture_pressAndTap;
  t->x=(int) lOx;
  t->y=(int) lOy;
  t->angle=dAngle;
  touches.Append(t);
  OUTPUT("GesturePressAndTap\n");
}

void Interface::GestureTwoFingerTap(const double dDistance, const LONG lOx, const LONG lOy) {
  Touch *t=new Touch;
  t->type = gesture_twoFingerTap;
  t->x=(int) lOx;
  t->y=(int) lOy;
  t->distance=dDistance;
  touches.Append(t);
  OUTPUT("GestureTwoFingerTap\n");
}

void Interface::GestureZoom(const double dZoomFactor, const LONG lZx, const LONG lZy) {
  Touch *t=new Touch;
  t->type = gesture_Zoom;
  t->x=(int) lZx;
  t->y=(int) lZy;
  touches.Append(t);
  OUTPUT("GestureTwoFingerTap\n");
}

// Never called! Mapped to GestureMove
void Interface::GesturePan(const double dDistance, const LONG lPx, const LONG lPy) {
  Touch *t=new Touch;
  t->type = gesture_Pan;
  t->x=(int) lPx;
  t->y=(int) lPy;
  t->distance=dDistance;
  touches.Append(t);
  OUTPUT("GesturePan\n");
}

void Interface::GestureMove(const LONG ldx, const LONG ldy) {
  Touch *t=new Touch;
  t->type = gesture_Move;
  t->dx=(int) ldx;
  t->dy=(int) ldy;
  touches.Append(t);
  OUTPUT("GestureMove\n");
}

void Interface::GestureRotate(const double dAngle, const LONG lOx, const LONG lOy) {
  Touch *t=new Touch;
  t->type = gesture_Rotate;
  touches.Append(t);
  OUTPUT("GestureRotate\n");
}
#endif // USE_WINGESTURE

void Interface::Detach(void)
{
#if USE_DIRECT_KEY
   // if keyboard acquired, release keyboard
   if (key_main)
   {
      IDirectInputDevice8_Unacquire(key_main);
      IDirectInputDevice8_Release(key_main);
      key_main = NULL;
   }
#endif

#if USE_DIRECT_MOUSE
   // if mouse acquired, release mouse
   if (mouse_main)
   {
      IDirectInputDevice8_Unacquire(mouse_main);
      IDirectInputDevice8_Release(mouse_main);
      mouse_main = NULL;
   }
#endif

#if USE_DIRECT_JOY || USE_DIRECT_MOUSE || USE_DIRECT_KEY
   // release main DirectInput object
   if (input_main) IDirectInput8_Release(input_main);
#endif

#if USE_DIRECT_JOY
   // release joystick
   if (hasJoystick) joyReleaseCapture(joy_ID);
#endif

#if USE_LOGITECH_KIT
   lgUnregister();
#endif
}



// Key definition tables.

char codeToChar[256] = {
 (char)0,
 (char)27,//          0x01
 '1',//               0x02
 '2',//               0x03
 '3',//               0x04
 '4',//               0x05
 '5',//               0x06
 '6',//               0x07
 '7',//               0x08
 '8',//               0x09
 '9',//               0x0A
 '0',//               0x0B
 '-',//               0x0C    /* - on main keyboard */
 '=',//               0x0D
 (char) 8,//          0x0E    /* backspace */
 (char) 9,//          0x0F
 'q',//               0x10
 'w',//               0x11
 'e',//               0x12
 'r',//               0x13
 't',//               0x14
 'y',//               0x15
 'u',//               0x16
 'i',//               0x17
 'o',//               0x18
 'p',//               0x19
 '[',//               0x1A
 ']',//               0x1B
 '\n',//              0x1C    /* Enter on main keyboard */
 (char) 17,//         0x1D
 'a',//               0x1E
 's',//               0x1F
 'd',//               0x20
 'f',//               0x21
 'g',//               0x22
 'h',//               0x23
 'j',//               0x24
 'k',//               0x25
 'l',//               0x26
 ';',//               0x27
 '\'',//              0x28
 '`',//               0x29    /* accent grave */
 (char) 14,//         0x2A // shift
 '\\',//              0x2B
 'z',//               0x2C
 'x',//               0x2D
 'c',//               0x2E
 'v',//               0x2F
 'b',//               0x30
 'n',//               0x31
 'm',//               0x32
 ',',//               0x33
 '.',//               0x34    /* . on main keyboard */
 '/',//               0x35    /* / on main keyboard */
 (char) 15,//         0x36
 '*',//               0x37    /* * on numeric keypad */
 (char) 0,//          0x38    /* left Alt */
 ' ',//               0x39
 (char) 26,//         0x3A
 (char) 241,//        0x3B
 (char) 242,//        0x3C
 (char) 243,//        0x3D
 (char) 244,//        0x3E
 (char) 245,//        0x3F
 (char) 246,//        0x40
 (char) 247,//        0x41
 (char) 248,//        0x42
 (char) 249,//        0x43
 (char) 250,//        0x44
 (char) 251,//        0x45
 (char) 252,//        0x46    /* Scroll Lock */
 '7',//               0x47
 '8',//               0x48
 '9',//               0x49
 '-',//               0x4A    /* - on numeric keypad */
 '4',//               0x4B
 '5',//               0x4C
 '6',//               0x4D
 '+',//               0x4E    /* + on numeric keypad */
 '1',//               0x4F
 '2',//               0x50
 '3',//               0x51
 '0',//               0x52
 '.',//               0x53    /* . on numeric keypad */
 (char) 224,//        0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
 (char) 251,//        0x57
 (char) 252,//        0x58
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 (char) 253,//        0x64    /*                     (NEC PC98) */
 (char) 254,//        0x65    /*                     (NEC PC98) */
 (char) 255,//        0x66    /*                     (NEC PC98) */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',//              0x70    /* (Japanese keyboard)            */
 '\0',
 '\0',
 (char) 168,//        0x73    /* /? on Brazilian keyboard */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',//              0x79    /* (Japanese keyboard)            */
 '\0',
 '\0',//              0x7B    /* (Japanese keyboard)            */
 '\0',
 (char) 157,//        0x7D    /* (Japanese keyboard)            */
 '.',//               0x7E    /* Numpad . on Brazilian keyboard */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '=',//              0x8D    /* = on numeric keypad (NEC PC98) */
 '\0',
 '\0',
 (char) 174,//       0x90    /* Previous Track (DX_CIRCUMFLEX on Japanese keyboard) */
 '@',//              0x91    /*                     (NEC PC98) */
 ':',//              0x92    /*                     (NEC PC98) */
 '_',//              0x93    /*                     (NEC PC98) */
 '\0',//             0x94    /* (Japanese keyboard)            */
 '\0',//             0x95    /*                     (NEC PC98) */
 '\0',//             0x96    /*                     (Japan AX) */
 (char) 255,//       0x97    /*                        (J3100) */
 (char) 175,//       0x99    /* Next Track */
 '\0',
 '\0',
 (char) 13,//        0x9C    /* Enter on numeric keypad */
 (char) 18,//        0x9D
 '\0',
 '\0',
 '\0',//             0xA0    /* Mute */
 '\0',//             0xA1    /* Calculator */
 '\0',//             0xA2    /* Play / Pause */
 '\0',
 '\0',//             0xA4    /* Media Stop */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',//             0xAE    /* Volume - */
 '\0',//             0xB0    /* Volume + */
 '\0',
 '\0',//             0xB2    /* Web home */
 ',',//              0xB3    /* , on numeric keypad (NEC PC98) */
 '\0',
 (char)246,//        0xB5    /* / on numeric keypad */
 '\0',
 '\0',//             0xB7
 '\0',//             0xB8    /* right Alt */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',//             0xC5    /* Pause */
 '\0',
 '\0',//             0xC7    /* Home on arrow keypad */
 '\0',//             0xC8    /* UpArrow on arrow keypad */
 '\0',//             0xC9    /* PgUp on arrow keypad */
 '\0',
 '\0',//             0xCB    /* LeftArrow on arrow keypad */
 '\0',
 '\0',//             0xCD    /* RightArrow on arrow keypad */
 '\0',
 '\0',//             0xCF    /* End on arrow keypad */
 '\0',//             0xD0    /* DownArrow on arrow keypad */
 '\0',//             0xD1    /* PgDn on arrow keypad */
 '\0',//             0xD2    /* Insert on arrow keypad */
 (char) 127,//       0xD3    /* Delete on arrow keypad */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',//             0xDB    /* Left Windows key */
 '\0',//             0xDC    /* Right Windows key */
 '\0',//             0xDD    /* AppMenu key */
 '\0',//             0xDE    /* System Power */
 '\0',//             0xDF    /* System Sleep */
 '\0',
 '\0',
 '\0',
 '\0',//             0xE3    /* System Wake */
 '\0',
 '\0',//             0xE5    /* Web Search */
 '\0',//             0xE6    /* Web Favorites */
 '\0',//             0xE7    /* Web Refresh */
 '\0',//             0xE8    /* Web Stop */
 '\0',//             0xE9    /* Web Forward */
 '\0',//             0xEA    /* Web Back */
 '\0',//             0xEB    /* My Computer */
 '\0',//             0xEC    /* Mail */
 '\0',//             0xED    /* Media Select */
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0',
 '\0'
};

UCHAR codeToUnichar[256] = {
 0,
 DX_ESCAPE,//          0x01
 DX_1,//               0x02
 DX_2,//               0x03
 DX_3,//               0x04
 DX_4,//               0x05
 DX_5,//               0x06
 DX_6,//               0x07
 DX_7,//               0x08
 DX_8,//               0x09
 DX_9,//               0x0A
 DX_0,//               0x0B
 DX_MINUS,//           0x0C    /* - on main keyboard */
 DX_EQUALS,//          0x0D
 DX_BACK,//            0x0E    /* backspace */
 DX_TAB,//             0x0F
 DX_Q,//               0x10
 DX_W,//               0x11
 DX_E,//               0x12
 DX_R,//               0x13
 DX_T,//               0x14
 DX_Y,//               0x15
 DX_U,//               0x16
 DX_I,//               0x17
 DX_O,//               0x18
 DX_P,//               0x19
 DX_LBRACKET,//        0x1A
 DX_RBRACKET,//        0x1B
 DX_RETURN,//          0x1C    /* Enter on main keyboard */
 DX_LCONTROL,//        0x1D
 DX_A,//               0x1E
 DX_S,//               0x1F
 DX_D,//               0x20
 DX_F,//               0x21
 DX_G,//               0x22
 DX_H,//               0x23
 DX_J,//               0x24
 DX_K,//               0x25
 DX_L,//               0x26
 DX_SEMICOLON,//       0x27
 DX_APOSTROPHE,//      0x28
 DX_GRAVE,//           0x29    /* accent grave */
 DX_LSHIFT,//          0x2A
 DX_BACKSLASH,//       0x2B
 DX_Z,//               0x2C
 DX_X,//               0x2D
 DX_C,//               0x2E
 DX_V,//               0x2F
 DX_B,//               0x30
 DX_N,//               0x31
 DX_M,//               0x32
 DX_COMMA,//           0x33
 DX_PERIOD,//          0x34    /* . on main keyboard */
 DX_SLASH,//           0x35    /* / on main keyboard */
 DX_RSHIFT,//          0x36
 DX_MULTIPLY,//        0x37    /* * on numeric keypad */
 DX_LMENU,//           0x38    /* left Alt */
 DX_SPACE,//           0x39
 DX_CAPITAL,//         0x3A
 DX_F1,//              0x3B
 DX_F2,//              0x3C
 DX_F3,//              0x3D
 DX_F4,//              0x3E
 DX_F5,//              0x3F
 DX_F6,//              0x40
 DX_F7,//              0x41
 DX_F8,//              0x42
 DX_F9,//              0x43
 DX_F10,//             0x44
 DX_NUMLOCK,//         0x45
 DX_SCROLL,//          0x46    /* Scroll Lock */
 DX_NUMPAD7,//         0x47
 DX_NUMPAD8,//         0x48
 DX_NUMPAD9,//         0x49
 DX_SUBTRACT,//        0x4A    /* - on numeric keypad */
 DX_NUMPAD4,//         0x4B
 DX_NUMPAD5,//         0x4C
 DX_NUMPAD6,//         0x4D
 DX_ADD,//             0x4E    /* + on numeric keypad */
 DX_NUMPAD1,//         0x4F
 DX_NUMPAD2,//         0x50
 DX_NUMPAD3,//         0x51
 DX_NUMPAD0,//         0x52
 DX_DECIMAL,//         0x53    /* . on numeric keypad */
 DX_OEM_102,//         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
 DX_F11,//             0x57
 DX_F12,//             0x58
 0x59,
 0x5A,
 0x5B,
 0x5C,
 0x5D,
 0x5E,
 0x5F,
 0x60,
 0x61,
 0x62,
 0x63,
 DX_F13,//             0x64    /*                     (NEC PC98) */
 DX_F14,//             0x65    /*                     (NEC PC98) */
 DX_F15,//             0x66    /*                     (NEC PC98) */
 0x67,
 0x68,
 0x69,
 0x6A,
 0x6B,
 0x6C,
 0x6D,
 0x6E,
 0x6F,
 DX_KANA,//            0x70    /* (Japanese keyboard)            */
 0x71,
 0x72,
 DX_ABNT_C1,//         0x73    /* /? on Brazilian keyboard */
 0x74,
 0x75,
 0x76,
 0x77,
 0x78,
 DX_CONVERT,//         0x79    /* (Japanese keyboard)            */
 0x7A,
 DX_NOCONVERT,//       0x7B    /* (Japanese keyboard)            */
 0x7C,
 DX_YEN,//             0x7D    /* (Japanese keyboard)            */
 DX_ABNT_C2,//         0x7E    /* Numpad . on Brazilian keyboard */
 0x7F,
 0x80,
 0x81,
 0x82,
 0x83,
 0x84,
 0x85,
 0x86,
 0x87,
 0x88,
 0x89,
 0x8A,
 0x8B,
 0x8C,
 DX_NUMPADEQUALS,//    0x8D    /* = on numeric keypad (NEC PC98) */
 0x8E,
 0x8F,
 DX_PREVTRACK,//       0x90    /* Previous Track (DX_CIRCUMFLEX on Japanese keyboard) */
 DX_AT,//              0x91    /*                     (NEC PC98) */
 DX_COLON,//           0x92    /*                     (NEC PC98) */
 DX_UNDERLINE,//       0x93    /*                     (NEC PC98) */
 DX_KANJI,//           0x94    /* (Japanese keyboard)            */
 DX_STOP,//            0x95    /*                     (NEC PC98) */
 DX_AX,//              0x96    /*                     (Japan AX) */
 DX_UNLABELED,//       0x97    /*                        (J3100) */
 DX_NEXTTRACK,//       0x99    /* Next Track */
 0x9A,
 0x9B,
 DX_NUMPADENTER,//     0x9C    /* Enter on numeric keypad */
 DX_RCONTROL,//        0x9D
 0x9E,
 0x9F,
 DX_MUTE,//            0xA0    /* Mute */
 DX_CALCULATOR,//      0xA1    /* Calculator */
 DX_PLAYPAUSE,//       0xA2    /* Play / Pause */
 0xA3,
 DX_MEDIASTOP,//       0xA4    /* Media Stop */
 0xA5,
 0xA6,
 0xA7,
 0xA8,
 0xA9,
 0xAA,
 0xAB,
 0xAC,
 0xAD,
 DX_VOLUMEDOWN,//      0xAE    /* Volume - */
 DX_VOLUMEUP,//        0xB0    /* Volume + */
 0xB1,
 DX_WEBHOME,//         0xB2    /* Web home */
 DX_NUMPADCOMMA,//     0xB3    /* , on numeric keypad (NEC PC98) */
 0xB4,
 DX_DIVIDE,//          0xB5    /* / on numeric keypad */
 0xB6,
 DX_SYSRQ,//           0xB7
 DX_RMENU,//           0xB8    /* right Alt */
 0xB9,
 0xBA,
 0xBB,
 0xBC,
 0xBD,
 0xBE,
 0xBF,
 0xC0,
 0xC1,
 0xC2,
 0xC3,
 0xC4,
 DX_PAUSE,//           0xC5    /* Pause */
 0xC6,
 DX_HOME,//            0xC7    /* Home on arrow keypad */
 DX_UP,//              0xC8    /* UpArrow on arrow keypad */
 DX_PRIOR,//           0xC9    /* PgUp on arrow keypad */
 0xCA,
 DX_LEFT,//            0xCB    /* LeftArrow on arrow keypad */
 0xCC,
 DX_RIGHT,//           0xCD    /* RightArrow on arrow keypad */
 0xCE,
 DX_END,//             0xCF    /* End on arrow keypad */
 DX_DOWN,//            0xD0    /* DownArrow on arrow keypad */
 DX_NEXT,//            0xD1    /* PgDn on arrow keypad */
 DX_INSERT,//          0xD2    /* Insert on arrow keypad */
 DX_DELETE,//          0xD3    /* Delete on arrow keypad */
 0xD4,
 0xD5,
 0xD6,
 0xD7,
 0xD8,
 0xD9,
 0xDA,
 DX_LWIN,//            0xDB    /* Left Windows key */
 DX_RWIN,//            0xDC    /* Right Windows key */
 DX_APPS,//            0xDD    /* AppMenu key */
 DX_POWER,//           0xDE    /* System Power */
 DX_SLEEP,//           0xDF    /* System Sleep */
 0xE0,
 0xE1,
 0xE2,
 DX_WAKE,//            0xE3    /* System Wake */
 0xE4,
 DX_WEBSEARCH,//       0xE5    /* Web Search */
 DX_WEBFAVORITES,//    0xE6    /* Web Favorites */
 DX_WEBREFRESH,//      0xE7    /* Web Refresh */
 DX_WEBSTOP,//         0xE8    /* Web Stop */
 DX_WEBFORWARD,//      0xE9    /* Web Forward */
 DX_WEBBACK,//         0xEA    /* Web Back */
 DX_MYCOMPUTER,//      0xEB    /* My Computer */
 DX_MAIL,//            0xEC    /* Mail */
 DX_MEDIASELECT,//      0xED    /* Media Select */
 0xEE,
 0xEF,
 0xF0,
 0xF1,
 0xF2,
 0xF3,
 0xF4,
 0xF5,
 0xF6,
 0xF7,
 0xF8,
 0xF9,
 0xFA,
 0xFB,
 0xFC,
 0xFD,
 0xFE,
 0xFF
};

bool MOUSEWITHIN( int x, int y, int x2, int y2 ) {
 return WITHIN(inputPointer->mxi,inputPointer->myi,x,y,x2,y2);
}
bool MOUSEWITHIN( float x, float y, float x2, float y2 ) {
 return WITHIN(inputPointer->mx,inputPointer->my,x,y,x2,y2);
}
bool MOUSEWITHIN( double x, double y, double x2, double y2 ) {
 return WITHIN(inputPointer->mxd,inputPointer->myd,x,y,x2,y2);
}

#if defined(USE_WINTOUCH)
int touchpoints[MAXPOINTS][2];
int tidLookup[MAXPOINTS];
int touching,touched=0;

void InitTouchPoints() {
 for ( int i=0; i<MAXPOINTS; i++ ) {
  touchpoints[i][0]=-1;
  touchpoints[i][1]=-1;
  tidLookup[i]=-1;
 }
}

// This function is used to return an index given an ID
int GetContactIndex(int dwID){
  for (int i=0; i < MAXPOINTS; i++){
    if (tidLookup[i] == -1){
      tidLookup[i] = dwID;
      return i;
    }else{
      if (tidLookup[i] == dwID){
        return i;
      }
    }
  }
  // Out of contacts
  return -1;
}
#endif