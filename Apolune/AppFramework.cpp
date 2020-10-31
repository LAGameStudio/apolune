#include "AppFramework.h"

Zdouble globalExpired;
Zint globalFrameCounter;

Zbool noFrameLimiter;

Zbool mouseEscapes;

GLSetup gl;

Audio audio;
Audio *audioPointer = &audio;
Interface input;
Interface *inputPointer = &input;
Display display;
Display *displayPointer = &display;

Zbool exitSignal;  // Used to trigger the application's end.
char path[_MAX_PATH];

bool already_exitted = false;

bool GameWindowIsFocused = true;
int xPos = 0;
int yPos = 0;

int netGameSeed=0;

WindowsAppFramework app;

// WINPROC ////////////////////////////////////////////////

LRESULT CALLBACK WinProc(HWND hwnd,
 UINT msg,
 WPARAM wparam,
 LPARAM lparam)
{  // this is the main message handler of the system
 HDC         hdc; // handle to a device context
 PAINTSTRUCT ps;  // used in WM_PAINT

 switch (msg) // what is the message
 {
#if defined(USE_WINTOUCH)
 case WM_TOUCH:
  input.WinTouchProcess(hwnd, wparam, lparam); break;
  break;
#endif
#if defined(USE_WINGESTURE)
  //      case WM_TOUCH: input.Touch(hwnd,wparam,lparam); break;
 case WM_GESTURE: return input.GestureProcess(hwnd, wparam, lparam); break;
 case WM_GESTURENOTIFY:
 {
  // This is the right place to define the list of gestures that this
  // application will support. By populating GESTURECONFIG structure 
  // and calling SetGestureConfig function. We can choose gestures 
  // that we want to handle in our application. In this app we
  // decide to handle all gestures.
  GESTURECONFIG gc = {
   0,              // gesture ID
   GC_ALLGESTURES, // settings related to gesture ID that are to be turned on
   0               // settings related to gesture ID that are to be turned off
  };

  BOOL bResult = SetGestureConfig(
   hwnd,                 // window for which configuration is specified  
   0,                    // reserved, must be 0
   1,                    // count of GESTURECONFIG structures
   &gc,                  // array of GESTURECONFIG structures, dwIDs will be processed in the
                         // order specified and repeated occurances will overwrite previous ones
   sizeof(GESTURECONFIG) // sizeof(GESTURECONFIG)
   );

  if (!bResult)
  {
   OUTPUT("WinProc: Error in execution of SetGestureConfig\n");
  }
 }
 break;
#endif

 case WM_MOUSEMOVE:
	  if ( mouseEscapes ) input.SetMousePosition(
	   (int) GET_X_LPARAM(lparam),
	   (int) GET_Y_LPARAM(lparam)
      ); 
	 break;

 case WM_SYSCOMMAND:												// Intercept System Commands
 {
  switch (wparam)												// Check System Calls
  {
  case SC_SCREENSAVE:										// Screensaver Trying To Start?
  case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
   return 0;												// Prevent From Happening
  }
  break;														// Exit
 }
 return 0;														// Return (this line is unreachable according to warning level 4)

 case WM_KILLFOCUS:
  GameWindowIsFocused = false;
  ShowCursor(true);
  SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  break;
 case WM_SETFOCUS:
  GameWindowIsFocused = true;
  ShowCursor(false);
  SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
  break;
 case WM_MOVE: {
  display.midx -= xPos;
  display.midy -= yPos;
  display.midx += (xPos = (int)(short)LOWORD(lparam));
  display.midy += (yPos = (int)(short)HIWORD(lparam));
 } break;

 case WM_CREATE:
 {
  // do initialization stuff here
  CREATESTRUCT* creation = (CREATESTRUCT*)(lparam); // Store Window Structure Pointer
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(creation->lpCreateParams));
  return(0); // return success
 } break;

 case WM_TIMER: // was WM_PAINT, changed to WM_TIMER because of http://www.opengl.org/wiki/Platform_specifics:_Windows
 {
  hdc = BeginPaint(hwnd, &ps); // validate the window

  EndPaint(hwnd, &ps);

  return(0); // return success
 }  break;

 case WM_DESTROY:
 {
#if defined(USE_WINTOUCH)
  if (input.windowsTouch.registered)
   if (!UnregisterTouchWindow(hwnd))
    OUTPUT("Could not unregister application window for touch input.\n");
#endif
  PostQuitMessage(0); // kill the application, sends a WM_QUIT message

  return(0); // return success
 }  break;

 // Input signaling
#if !(USE_DIRECT_MOUSE)

 case WM_LBUTTONDBLCLK: { input.LeftDouble();   return (0); } break;
 case WM_RBUTTONDBLCLK: { input.RightDouble();  return (0); } break;
 case WM_RBUTTONUP: { input.RightUp();      return (0); } break;
 case WM_LBUTTONUP: { input.LeftUp();       return (0); } break;
 case WM_RBUTTONDOWN: { input.RightDown();    return (0); } break;
 case WM_LBUTTONDOWN: { input.LeftDown();     return (0); } break;
 case WM_MOVE:
 {
  input.MouseMoved(LOWORD(lparam), HIWORD(lparam));
  return (0);
 } break;

#endif
#if !(USE_DIRECT_KEY)
 case WM_KEYDOWN:       if ((wparam >= 0) && (wparam <= 255)) { input.WMKeyDown((int)wparam); return (0); } break;
 case WM_KEYUP:         if ((wparam >= 0) && (wparam <= 255)) { input.WMKeyUp((int)wparam);   return (0); } break;
#endif

 case WM_SIZE:
 {
//  RECT rect;
//  if ( GetClientRect(hwnd,&rect) ) {
//   display.Resized((int)rect.right,(int)rect.bottom);
//  }
  //if ( GetWindowRect( hwnd, &rect ) ) {
  // display.Resized((int)ADIFF(rect.left,rect.right),(int)ADIFF(rect.top,rect.bottom));
  //}
  display.Resized(LOWORD(lparam), HIWORD(lparam));
  return (0);
 }
 break;

 case WM_CHAR:
 {
  // OUTPUT("WM_CHAR: %c %d repeated %d times\n", (char)wparam, (int)wparam, (int) LOWORD(lparam) );
  windowsKeyboard.WM_Character((char)wparam, (int)LOWORD(lparam));
 }
 break;

#if defined(USE_RAWINPUT)
 case WM_INPUT:
 {
 }
 break;
#endif

 default:break;

 } // end switch

   // process any messages that we didn't take care of

 return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

int called_on_exit(void) {
 if (!already_exitted) {
  app.Quit();  // game quit function and clean up before exit called here

#if defined(USE_STEAMWORKS)
  steamworks.Shutdown();
#endif

 // Shutdown CURL
#if defined(USE_CURL)
  OUTPUT("CURL: Closing.\n");
  curl_global_cleanup();
#endif
#if defined(CRT_LEAKS)
  if (CmdLine.Option("-leaks")) {
   _CrtDumpMemoryLeaks();
  }
#endif
  Gdiplus::GdiplusShutdown(app.gdiplusToken);
  already_exitted = true;
 }
 return 1;
}