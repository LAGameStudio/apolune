#pragma once

#define WIN32_LEAN_AND_MEAN

#ifndef USE_STEAMWORKS
///#define USE_STEAMWORKS 1
#include "Steamworks.h"
#endif

#if defined(CRT_LEAKS)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // for joystick info
#include <direct.h>    // for getcwd()
#include <stdio.h>
#include <stdlib.h>    // for rand functions
#include <math.h>

#define DIRECTINPUT_VERSION         0x0800
#include <dinput.h>    // include DirectInput

#include "GLSetup.h"

#include <GdiPlus.h>

#include "macros.h"
#include "moremath.h"
#include "string_functions.h"
#include "ZeroTypes.h"

#include "CommandLine.h"
#include "GetTotalMemory.h"

#include "pThread.h"
#include "CTimer.h"

#include "Display.h"
#include "GLSetup.h"
#include "GLPrimitives.h"
#include "GLWindow.h"

#include "Audio.h"

#include "WindowsKeyboard.h"
#include "Interface.h"
#include "FastGUI.h"

#include "Vertexd.h"
#include "Shape.h"
#include "VBO.h"
#include "Scripting.h"

#include "WebPage.h"
#include "Load.h" // Preloading of textures and audio, window setup, et al
#include "QuadGridVBO.h"
#include "QuadGridVBOTest.h"

// Initialized in our main loop
#include "Tweens.h"
#include "BitBrush.h"
#include "fx_Shader2d.h" 
#include "SpherePainter.h"
#include "fx_UglyText.h"

#include "Atlas.h"

#include "DJ.h"

#define WINDOW_CLASS_NAME "WINCLASS1"

extern Zbool noFrameLimiter;
extern Zdouble globalExpired; // incremented in ticker()
extern Zint globalFrameCounter; // incremented in ticker()

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

extern Zbool exitSignal;  // Used to trigger the application's end.
extern char path[_MAX_PATH];

extern Zbool mouseEscapes;  // Mouse is allowed to escape the window.
extern bool GameWindowIsFocused;
extern int xPos;
extern int yPos;

// in string_functions.cpp
extern bool log_OUTPUT_to_file; 
extern bool stop_log_OUTPUT_to_file;

extern bool already_exitted;
int called_on_exit(void);

extern int netGameSeed;

#if defined(GRANULAR_DEBUG)
char *timeframe_names[8] = {
 "unused",
 "windows.Between()",
 "windows.BetweenFrames()",
 "input.Snapshot and ProcessInput()",
 "audio.Virtual()",
 "windows.Render()",
 "swapBuffers",
 "thread.Clean()"
};
#endif

#if defined(USE_CURL)
#define CURL_STATICLIB 1
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#endif

extern GLSetup gl;
extern Audio audio;
extern Audio *audioPointer;
extern Interface input;
extern Interface *inputPointer;
extern Display display;
extern Display *displayPointer;

extern Vertexd up;

#include "Application.h"
#include "HIDControlMapping.h"

class WindowsAppFramework {
public:

 Zbool permitScriptResetButton;
 Zfloat scriptResetDelay;
 Gdiplus::GdiplusStartupInput gdiplusStartupInput;
 ULONG_PTR gdiplusToken;

 HWND game_window;           // global game window handle
 HINSTANCE game_instance;    // global game instance handle
 HDC game_dc;                // global device context (GDI) handle

 DEVMODE game_screen;               // global for full screen mode

 HGLRC game_rc;              // global rendering context (OpenGL) handle
 HGLRC upload_rc;            // global uploading context (OpenGL) handle

 Zbool draw_ok;                      // for whether OpenGL init ok
 PIXELFORMATDESCRIPTOR pfd;

 WNDCLASSEX winclass; // this will hold the class we create
 HWND hwnd;           // generic window handle
 MSG msg;             // generic message

 HINSTANCE hinstance;
 HINSTANCE hprevinstance;

 HRESULT oleResult;
 Zbool OLE;

 WindowsAppFramework() {
  game_window = NULL;
  game_instance = NULL;
  game_dc = NULL;
  game_rc = NULL;
  upload_rc = NULL;
  windows.display = &display;
 }
 ///////////////////////////////////////////////////////////
 //
 // GAME INITIALIZATION
 //
 ///////////////////////////////////////////////////////////
 bool Init()
 {
  // CRT Leak Tracking (for a special debugging circumstance)
#if defined(CRT_LEAKS)
  if (CmdLine.Option("-leaks")) {
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  }
#endif
  
  // Logging.
  if ( CmdLine.Option("-logging") ) log_OUTPUT_to_file=true;
  else if ( CmdLine.Option("-keeplogging") ) stop_log_OUTPUT_to_file=false;

  if ( CmdLine.Option("-nolimit") ) noFrameLimiter=true;

  OUTPUT("Total available memory: %llu (%llu mb, or %lu gb)\n",
   (unsigned long long) totalSystemMemory.available,
   (unsigned long long) (totalSystemMemory.available / 1048576),
   (unsigned long)(totalSystemMemory.available / 1073741824)
  );

  application->PreInit();

  // Handle initialization of external libraries.

  // Init CURL
#if defined(USE_CURL)
  curl_global_init(CURL_GLOBAL_ALL);
#endif

  // Init Logitech Kit, if we're using it.
#if defined(USE_LOGITECH_KIT)
  if (CmdLine.Option("-lgbuttons") != NULL) {
   use_lg_button_notifications = true;
   OUTPUT("Forcing use of LgKitRegisterMouseButtonsNotifications for Logitech device support.\n");
  }
#endif

  // Initialize GDI+.
  Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  
  // Init work strings
  InitWorkStrings();
  {
   string test = string("");
   OUTPUT("Maximum size of a string is reported as %lu.\n", (unsigned long)test.max_size());
  }
  
  // Load 100MB of random data.
  randoms.LoadRandom("data/random.org/random-0.dat");
  /*
  randoms.LoadRandom( "data/random.org/random-1.dat" );
  randoms.LoadRandom( "data/random.org/random-2.dat" );
  randoms.LoadRandom( "data/random.org/random-3.dat" );
  randoms.LoadRandom( "data/random.org/random-4.dat" );
  randoms.LoadRandom( "data/random.org/random-5.dat" );
  randoms.LoadRandom( "data/random.org/random-6.dat" );
  randoms.LoadRandom( "data/random.org/random-7.dat" );
  randoms.LoadRandom( "data/random.org/random-8.dat" );
  randoms.LoadRandom( "data/random.org/random-9.dat" );
  */
  
  if (CmdLine.Option("-zerotypestest")) {
   ZeroTypesTest();
  }

  if (CmdLine.Option("-resetbutton")) {
   permitScriptResetButton = true;
  }

  if (CmdLine.Option("-window") != NULL) {
   gl.noFullscreen = true;
   OUTPUT("Fullscreen support disabled by command line.\n");
  }

  if (CmdLine.Option("-borderless") != NULL) {
   gl.borderless = true;
   OUTPUT("Borderless window selected.\n");
  }

  if (CmdLine.Option("-windowed") != NULL) {
   gl.noFullscreen = true;
   OUTPUT("Fullscreen support disabled by command line.\n");
  }

  if (CmdLine.Option("-resizable") != NULL) {
   gl.resizable = true;
   OUTPUT("Resizable window requested.\n");
  }

  if (CmdLine.Option("-nologitech") != NULL) {
   input.useLogitech = false;
   OUTPUT("Suppressing any support for Logitech devices.\n");
  }

  if (CmdLine.Option("-nomipmaps") != NULL) {
   gl.noMipmapping = true;
   OUTPUT("Disabling mipmap support by command line.\n");
  }
  
  string _title=CmdLine.OptionPair("-wintitle");
  if ( _title.length() > 0 ) gl.winTitle=_title;

  display.CommandLine();
  display.Resized(display.w,display.h);

  if (CmdLine.Option("-trapdoor")) {
   gl.escTrapdoor = true;
   OUTPUT("The emergency exit is unlocked.\n");
  }

  // Fill in the window class structure for testing display type.

  winclass.cbSize = sizeof(WNDCLASSEX);
  winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  winclass.lpfnWndProc = WinProc;
  winclass.cbClsExtra = 0;
  winclass.cbWndExtra = 0;
  winclass.hInstance = hinstance;
  winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  winclass.lpszMenuName = NULL;
  winclass.lpszClassName = WINDOW_CLASS_NAME;

  if (_getcwd(path, _MAX_PATH) == NULL)  OUTPUT("Could not get current working path.\n");
  else {
   OUTPUT("CWD: %s\n", path);
   specialPaths.working_directory=path+string("\\");
   specialPaths.data_folder=string(path)+string("\\data\\");
  }

  // Save the game instance handle
  display.hinstance = game_instance = hinstance;

  // Register the window class
  if (!RegisterClassEx(&winclass)) return(0);

  if (!gl.Init(hinstance, display.bits)) {
   return(0);
  }

  InitMath();
  // Generate nifty integer sqrt(), and some misc stuff
  init_isqrt();
  up.Set(0.0, 1.0, 0.0);

  // Get OLE going.
  oleResult = OleInitialize(NULL);
  if (SUCCEEDED(oleResult)) OLE=true;

  // Detect the display size and create the final display profile 

  DWORD winStyle=
   WS_EX_APPWINDOW |
   WS_EX_TOPMOST /*|
   WS_EX_ACCEPTFILES*/ ;

  // Adjust Window, Account For Window Borders
  int xPos = GetSystemMetrics(SM_CXSCREEN) - display.w;
  int yPos = GetSystemMetrics(SM_CYSCREEN) - display.h;
  RECT windowRect = {0, 0, display.w, display.h};	// Define Our Window Coordinates
  AdjustWindowRectEx (&windowRect, WS_POPUP, 0, winStyle );
  // Create the window
  if (!(hwnd = CreateWindowEx(
   winStyle,				      // extended style
   WINDOW_CLASS_NAME, // class
   gl.winTitle.c_str(),          // title
   ( gl.borderless || CmdLine.Option("-borderless") ) ? (WS_POPUPWINDOW | WS_VISIBLE)
     : (gl.noFullscreen ? ((CmdLine.Option("-bordered") ? WS_BORDER : 0) | WS_VISIBLE)
                        : (WS_POPUP | WS_VISIBLE)),  // use POPUP for full screen
   gl.noFullscreen && !CmdLine.Option("-recenter") ? xPos / 2 : 0,
   gl.noFullscreen && !CmdLine.Option("-recenter") ? yPos / 2 : 0,     // initial game window x,y
   display.w,         // initial game width
   display.h,         // initial game height
   HWND_DESKTOP,      // handle to parent
   NULL,              // handle to menu
   hinstance,         // instance of this application
   NULL)
    )         // extra creation parms
   ) {
   OUTPUT("WinAPI ERROR: Could not open window.\n");
   return(0);
  }

  if (gl.borderless || CmdLine.Option("-borderless") ) {
    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);
    LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
    SetWindowPos(hwnd, NULL, 0, 0, display.w, display.h, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
  }
  
  mouseEscapes = CmdLine.Option("-mouseescape") || CmdLine.Option("-mouseescapes");

  // Temporary change to full screen mode
  ZeroMemory(&game_screen, sizeof(game_screen)); // clear out size of DEVMODE struct
  game_screen.dmSize = sizeof(game_screen);
  game_screen.dmPelsWidth = display.w;
  game_screen.dmPelsHeight = display.h;
  game_screen.dmBitsPerPel = display.bits;
  game_screen.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
  ChangeDisplaySettings(&game_screen, CDS_FULLSCREEN);

  // save the game window handle
  display.hwnd = game_window = hwnd;
  display.hdc = game_dc = GetDC(display.hwnd = game_window); // get the GDI device context
                                                             // set up the pixel format desc struct

  pfd = {
   sizeof(PIXELFORMATDESCRIPTOR),   // size of this PFD
   1,                               // version number
   PFD_DRAW_TO_WINDOW |             // supports window
   PFD_SUPPORT_OPENGL |             // supports OpenGL
   PFD_DOUBLEBUFFER,                // support double buff
   PFD_TYPE_RGBA,                   // request RGBA format
   (BYTE)display.bits,              // select color depth
   0, 0, 0, 0, 0, 0,                // color bits ignored
   0,                               // no alpha buff
   0,                               // shift bit ignored
   0,                               // no accum buff
   0, 0, 0, 0,                      // accum bits ignored
   16,                              // 16-bit Z-buff (depth buff)
   0,                               // no stencil buff
   0,                               // no aux buff
   PFD_MAIN_PLANE,                  // main drawing layer
   0,                               // reserved
   0, 0, 0                          // layer masks ignored
  };
  int pf;  // pixel format

  if (!gl.arbMultisampleSupported) {
   if (!(pf = ChoosePixelFormat(game_dc, &pfd)))  // match the pixel format
   {
    MessageBox(game_window, "OpenGL could not be initialized -- ChoosePixelFormat Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
    return FALSE; // error returned
   }
  } else {
   pf = gl.arbMultisampleFormat;
  }
  if (!SetPixelFormat(game_dc, pf, &pfd))        // set the pixel format
  {
   MessageBox(game_window, "OpenGL could not be initialized -- SetPixelFormat Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
   return FALSE; // error returned
  }

  if (!(game_rc = wglCreateContext(game_dc)))    // create the rendering context
  {
   MessageBox(game_window, "OpenGL could not be initialized -- CreateContext Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
   return FALSE; // error returned
  }

  if (!(upload_rc = wglCreateContext(game_dc)))    // create the rendering context
  {
   MessageBox(game_window, "Multiple OpenGL contexts could not be initialized -- CreateContext Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
   return FALSE; // error returned
  } else { // Share as much as you can between two contexts
   if (!wglShareLists(game_rc, upload_rc)) {
    // could use GetLastError here
    MessageBox(game_window, "wglShareLists -- Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
    return FALSE; // error returned
   }
  }

  if (!wglMakeCurrent(game_dc, display.hglrc = game_rc))         // make it current
  {
   MessageBox(game_window, "OpenGL could not be initialized -- MakeCurrent Error ; report this to program authors for help!", "OpenGL Error", MB_OK);
   return FALSE; // error returned
  }

  ShowCursor(false);
  ShowWindow(game_window, SW_SHOWNORMAL);
  SetForegroundWindow(game_window);
  InitDefaultFBOs();
  vg.Resize(1920,1080); // Initialize the virtual graphics canvas and its 4 virtual FBOs.
  InitBasicVBOs();
  InitGLPrimitives();
  fx_UglyTextBoxInit();
  //PrecomputeSpheres();
  InitQuadGrids();

#if defined(USE_RAWINPUT)  // this is legacy support for pre-XP I think, not useful anymore
  OUTPUT("Registering RAWINPUTDEVICE for keyboard control.\n");
  // InitRawInput
  RAWINPUTDEVICE device;
  device.usUsagePage = 0x01;
  device.usUsage = 0x06;
  if (CmdLine.Option("-capturemouse")) { // If set, the mouse button click does not activate the other window.
   OUTPUT("Window: Capture Mouse\n");
   device.dwFlags |= RIDEV_CAPTUREMOUSE;
  }
  if (CmdLine.Option("-nohotkeys")) { // If set, the application-defined keyboard device hotkeys are not handled. However, the system hotkeys; for example, ALT+TAB and CTRL+ALT+DEL, are still handled. By default, all keyboard hotkeys are handled. RIDEV_NOHOTKEYS can be specified even if RIDEV_NOLEGACY is not specified
   OUTPUT("Window: No Hotkeys\n");
   device.dwFlags |= RIDEV_NOHOTKEYS;
  }
  if (CmdLine.Option("-nolegacyinput")) { // If RIDEV_NOLEGACY is set for a mouse or a keyboard, the system does not generate any legacy message for that device for the application
   OUTPUT("Window: No Legacy Input\n");
   device.dwFlags |= RIDEV_NOLEGACY;
   if (CmdLine.Option("-appkeys")) { // If set, the application command keys are handled. RIDEV_APPKEYS can be specified only if RIDEV_NOLEGACY is specified for a keyboard device.
    OUTPUT("Window: App Keys\n");
    device.dwFlags |= RIDEV_APPKEYS;
   }
  }
  device.hwndTarget = game_window;
  RegisterRawInputDevices(&device, 1, sizeof(device));
#endif

  GL_Assert("glSetup");

#if defined(USE_GLFW)
  // Initialise GLFW
  if (!glfwInit())
  {
   OUTPUT("Failed to initialize GLFW\n");
   return -1;
  }
#endif

  input.window = (game_window);
  input.instance = (game_instance);
  input.Detect(&display);
  display.gl = &gl;
  windows.input = &input;
  windows.display = &display;
  audio.path = path;

  // Attach the texture library to the context and connect 
  library.glSetup = &gl;
  library.InitStreaming();

  // Generate our tweens here because we need an active GL context to precompute display lists
  tweens.Load();
  TweensToTexture();

  // Load and initialize our singleton shaders.
///  InitShaders();   Moved to initialization before DrawLoading()

  // Init display lists of simple 2d primitives (circle, triangle, square, etc.. from Shape.h/cpp)
  InitShapes();

  application->Init();
  application->PostInit();

  GL_Assert("GameInit");
  glReportError(glGetError());

  InitShaders();

  return TRUE; // success since previous IFs all TRUE

 } // END OF GameInit


 ///////////////////////////////////////////////////////////
 //
 // GAME MAIN LOOP AND PROCESSING
 //
 ///////////////////////////////////////////////////////////

 // Called by the HRTimer class
 void Tick() {
  windows.BetweenFrames();
  atlasFonts.Between();
 }

 // Called in various places to get current frame time.
 // Holds the seconds as a ratio
 Zdouble tickerSecond;
 Zdouble tickerMinute;
 Zdouble tickerHour;
 void ticker() {
  SYSTEMTIME st;
  GetSystemTime(&st);
  tickerSecond = (double)st.wMilliseconds / 999.0;
  tickerMinute = ((double)st.wSecond + tickerSecond) / 59.0;
  tickerHour   = ((double)st.wMinute + tickerMinute) / 59.0;
  globalExpired+=FRAMETIME;
  if ( globalExpired < 0.0 || globalExpired > FRAMETIMEd*60.0*600.0 ) globalExpired=0.0;
  globalFrameCounter+=1;
 }

 Zbool MainCalled;
 inline void Main() {
  if ( MainCalled.value ) return;
  MainCalled=true;
  application->Entrance();
  draw_ok = Init();   // game initialization function called here
  if ( !draw_ok ) {
   string message = FORMAT(
    "An error has occurred in game engine initialization.  "
    "This is usually caused by attempting to load the game "
    "on hardware that is unsupported, but can be the result "
    "of other issues.\n "
    "An NVIDIA or AMD/ATI GPU is required along with an audio "
    "device with at least one playback and one recording channel.\n",
    (consulate.executed ? consulate.executed->filename.c_str() : "unknown"),
    ""
    );
   MessageBox(game_window, message.c_str(), "Unrecoverable error", MB_OK);
   exit(0);
  }

  application->Startup(); // Startup sound
  // Splash screen, then commence script compilation and ultimately Loop
  {
	  GLImage *Loading = library.Load("data/images/splash.png");
   bool FirstPass = true;
   bool SecondPass = true;
   while (TRUE) { // PRE-LOAD LOOP (2 frames)
#if defined(GL_DEBUG)
    FORMAT(gl_info, 1024, "DrawLoading (FirstPass: %s SecondPass: %s)",
     FirstPass ? "yes" : "no",
     SecondPass ? "yes" : "no");
    glReportError(glGetError());
#endif
    DrawLoading(Loading, &game_dc, &game_window, display.w, display.h);
    if (FirstPass) {
     FirstPass = false;
    } else if (SecondPass) {
     Load(&windows, &audio, &display, &input); SecondPass = false;
     // Parse and Run Global Scripts
     try {
      consulate.Execute();
     } catch (...) {
      string message = FORMAT(
       "An error has occurred in one of your Alacrity scripts.  "
       "This is usually caused by an unexpected NULL pointer to"
       "a non-existent Tween or Image.\n "
       "The scripts stopped executing.\n"
       "Execution Info:\nExecution stopped in the script:\n%s\nLast line was:\n%s",
       (consulate.executed ? consulate.executed->filename.c_str() : "unknown"),
       ""
       );
      MessageBox(game_window, message.c_str(), "Unrecoverable Alacrity script error", MB_OK);
     }
     if (consulate.errors.count > 0) {
      string message = string("Errors and/or warnings encountered in script:\n")
       + consulate.errors.Concatenate('\n');
      MessageBox(game_window, message.c_str(), "Alacrity script error messages", MB_OK);
     }
    } else break;

    // is there a message in queue, if so get it
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
     // test if this is a quit
     if (msg.message == WM_QUIT) break;
     TranslateMessage(&msg);
     DispatchMessage(&msg);

    } // end if

   if (GameWindowIsFocused) {
    if ( !mouseEscapes ) {
#if defined(DEBUG)
   SetCursorPos(display.midx, display.midy); // to avoid the mouse exitting the active view // fix by I_Trll but then broke again with the adding of openal ftw
#else
     RECT rect;
     if ( gl.noFullscreen && GetWindowRect(hwnd,&rect) ) {
      SetCursorPos(
       rect.left+(rect.right-rect.left)/2,
       rect.top+(rect.bottom-rect.top)/2
      ); // to avoid the mouse exitting the active view // fix by I_Trll but then broke again with the adding of openal ftw
     } else SetCursorPos(display.midx, display.midy); // to avoid the mouse exitting the active view // fix by I_Trll but then broke again with the adding of openal ftw

#endif
     }
    }
#if defined(GL_DEBUG)
    GL_Assert("SwapBuffers");
#endif
    SwapBuffers(game_dc);
   } // END PRE-LOAD LOOP (2 frames)
  }

  application->PreLoad();
  windows.Load();
  application->Load();
  application->PostLoad();
  OUTPUT("%d windows.\n", (int)windows.count);

  // Dummy init to insure DLL preload
  //   DummyPageTest();
  /* TESTING
  */

  /*OUTPUT("Testing base64 encode/decode:\n `%s` -> `%s` -> `%s`\n",
  "hello",
  base64("hello").c_str(),
  base64(base64("hello").c_str(),false).c_str()
  );

  OUTPUT("Testing Vigenere encode/decode:\n `%s` -> `%s` -> `%s`\n",
  "hello",
  vigenere("hello","key").c_str(),
  vigenere(vigenere("hello","key").c_str(),"key",false).c_str()
  );*/

  /* TESTING
  */

  _onexit(called_on_exit);

  // enter main event loop using PeekMessage() to retrieve messages
  //timers.Start();
  timer.Initialize();
  //   timer.Update(); // clears out the long wait time since initalization
  while (TRUE)
  {
   // reset moremath.h's ticker() (uses GetSystemTime)
   this->ticker();
   // do the framestart of fast gui
   fast.FrameStart();
   dJ.Between();

   // get initial tick count to keep game speed constant
   //      DWORD start_tick = GetTickCount();
   //      timers.Wait();

   // is there a message in queue, if so get it
   if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
   {
    // test if this is a quit
    if (msg.message == WM_QUIT) break;

    // translate any accelerator keys
    TranslateMessage(&msg);

    // send the message to WinProc
    DispatchMessage(&msg);

   } // end if

     /* Clear the window manager and reload the scripts */
   if (permitScriptResetButton) {
    if (scriptResetDelay > 0.0f) scriptResetDelay -= FRAMETIME;
    if (scriptResetDelay <= 0.0f
     && input.KeyDown(DX_F1) && input.KeyDown(DX_F2) && input.KeyDown(DX_F3)) {
     scriptResetDelay = 1.0f;
     consulate.Clear();
     OUTPUT("Discarding loaded scripts.\n");
     variableStore.Clear();
     OUTPUT("Clearing the window manager.\n");
     windows.Clear();
     OUTPUT("Reloading scripts...\n");
     LoadScripts();
     // Run Global Scripts
     try {
      consulate.Execute();
     } catch (...) {
      string message = FORMAT(
       "An error has occurred in one of your Alacrity scripts.  "
       "This is usually caused by an unexpected NULL pointer to"
       "a non-existent Tween or Image.\n "
       "The scripts stopped executing.\n"
       "Execution Info:\nExecution stopped in the script:\n%s\nLast line was:\n%s",
       (consulate.executed ? consulate.executed->filename.c_str() : "unknown"),
       ""
       );
      MessageBox(game_window, message.c_str(), "Unrecoverable Alacrity script error", MB_OK);
     }
    }
   }

   InitTICK(); // Initializes the high performance timer query, a worthless feature of Winblows.

   DWORD frame_started=0;
   if ( !noFrameLimiter ) frame_started= GetTickCount();
   //float wait=0.0f;
   Loop();  // game main processing function called here
   if ( !noFrameLimiter ) {
    DWORD frame_ended= GetTickCount();
    DWORD frame_diff= frame_ended-frame_started;
    if ( frame_diff < 1000/FPS ) Sleep( (1000/FPS) - frame_diff );
   }

                // check for <ESC> key and send quit game
   if (gl.escTrapdoor && input.KeyDown(DX_ESCAPE)) {
    OUTPUT("Free-falling.\n");
    SendMessage(hwnd, WM_CLOSE, 0, 0);
   }

   // check for shutdown signal file
   if (file_exists("data/shutdown.txt")) {
    switch (DeleteFile("data/shutdown.txt")) {
    case ERROR_ACCESS_DENIED: case ERROR_FILE_NOT_FOUND: case 0: break;
    default:
     OUTPUT("Shutdown signal file removed.  Shutting down.\n");
     SendMessage(hwnd, WM_CLOSE, 0, 0);
     break;
    }
   }

   // check for shutdown signal file (in exe folder)
   if (file_exists("shutdown.txt")) {
    switch (DeleteFile("shutdown.txt")) {
    case ERROR_ACCESS_DENIED: case ERROR_FILE_NOT_FOUND: case 0: break;
    default:
     OUTPUT("Shutdown signal file removed.  Shutting down.\n");
     SendMessage(hwnd, WM_CLOSE, 0, 0);
     break;
    }
   }

   // wait until we hit correct game speed frame rate
   timer.Update();
   //      OUTPUT("%f\n", (float) timer.dt);
   if (timer.dt < 1000.0f / FPS) Sleep((DWORD)(1000.0f / FPS - timer.dt));
   //timer.Update();
   //while ( (wait+=timer.dt) < 1000.0f/FPS ) { OUTPUT("%f\n", wait); timer.Update(); }

  } // end while

  application->AfterLoop();
  called_on_exit();
 } // END OF Main

 inline void Loop() {
  GameTICK();
#if defined(USE_STEAMWORKS)
  steamworks.Between();
#endif
  application->PreLock();
  pthreads.Loop();
  pthreads.Lock();
  application->PreLoop();
#if defined(GRANULAR_DEBUG)
  DWORD frameTime = timeGetTime();
  if (input.KeyDown(DX_PERIOD)) OUTPUT("--- Frame start\n");
  DWORD times[8];
  // timePeriodBegin(1);?
#endif
  application->Loop();
  audio.manager.BetweenFrames(); // was called in a timer previously .. augh
#if defined(GRANULAR_DEBUG)
  times[0] = timeGetTime();
#endif
  windows.Between();
#if defined(GRANULAR_DEBUG)
  times[1] = timeGetTime();
#endif
  windows.BetweenFrames();
#if defined(GRANULAR_DEBUG)
  times[2] = timeGetTime();
#endif
  if (GameWindowIsFocused) {
   input.Snapshot();
   controls.Between();
   if ( !mouseEscapes ) {
    RECT rect;
    if ( gl.noFullscreen && GetWindowRect(hwnd,&rect) ) {
     SetCursorPos(
      rect.left+(rect.right-rect.left)/2,
      rect.top+(rect.bottom-rect.top)/2
     ); // to avoid the mouse exitting the active view // fix by I_Trll but then broke again with the adding of openal ftw
    } else SetCursorPos(display.midx, display.midy); // to avoid the mouse exitting the active view // fix by I_Trll but then broke again with the adding of openal ftw
   }
   windows.ProcessInput(); // was before render
  }
#if defined(GRANULAR_DEBUG)
  times[3] = timeGetTime();
#endif
  audio.manager.Virtual();
#if defined(GRANULAR_DEBUG)
  times[4] = timeGetTime();
#endif
  windows.Render();
#if defined(GRANULAR_DEBUG)
  times[5] = timeGetTime();
#endif

  if (gl.WGL_EXT_swap_control_supported) {
   wglSwapIntervalEXT(gl.vSync ? 1 : 0);
  }
  SwapBuffers(game_dc);
#if defined(GRANULAR_DEBUG)
  times[6] = timeGetTime();
#endif
#if defined(GRANULAR_DEBUG)
  times[7] = timeGetTime();
  if (input.KeyDown(DX_PERIOD)) {
   int value = (int)(times[7] - frameTime);
   OUTPUT("--- Frame time was %d ms.\n", (int)value);
   for (int i = 1; i<8; i++) {
    value = (int)(times[i] - times[i - 1]);
    OUTPUT("Time counter %d was %d for %s\n", (int)i, (int)value, timeframe_names[i]);
   }
  }
#endif
  application->PostLoop();
  pthreads.Unlock();
  application->PostLock();
 } // END OF Loop


 ///////////////////////////////////////////////////////////
 //
 // GAME QUIT AND CLEAN UP
 //
 ///////////////////////////////////////////////////////////
 void Quit() {
  input.Detach();
  if (game_rc) // if rendering context (OpenGL) exists, delete it
  {
   wglMakeCurrent(NULL, NULL);
   wglDeleteContext(game_rc);
  }
  if (upload_rc) {
   wglMakeCurrent(NULL, NULL); // redundant
   wglDeleteContext(upload_rc);
  }
  ShowCursor(true);
  // release the device context (GDI) from the game window
  ReleaseDC(game_window, game_dc);
  // return to original display settings
  ChangeDisplaySettings(NULL, NULL);
  //timers.Stop();
 } // END OF Quit

};

extern WindowsAppFramework app;
