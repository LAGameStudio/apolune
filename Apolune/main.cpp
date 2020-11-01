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
#pragma warning (disable:4100)

#include "main.h"
#include "AppFramework.h"
#include <Xinput.h>

#if defined(DEBUG)
#if defined(USE_STEAMWORKS)
#include "Steamworks.h"
#endif
#endif

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(_In_ HINSTANCE hinstance,
                   _In_opt_ HINSTANCE hprevinstance,
                   _In_ LPSTR lpCmdLine,
                   _In_ int ncmdshow)
{
 // Initialize COM
 CoInitialize(0);

 // Initialize XInput
 XInputEnable(TRUE);

 // Initialize GDI+.
 Gdiplus::GdiplusStartup(&app.gdiplusToken, &app.gdiplusStartupInput, NULL); 

 // Get OLE going.
 app.oleResult = OleInitialize(NULL);
 if (SUCCEEDED(app.oleResult)) app.OLE = true;

  // Handle initialization of external libraries.
  // CRT Leak Tracking (for a special debugging circumstance)
#if defined(CRT_LEAKS)
  if (CmdLine.Option("-leaks")) {
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  }
#endif

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

#if defined(USE_STEAMWORKS)
 if ( !CmdLine.Option("-nosteam") ) if ( !steamworks.Init() ) return 0;
#endif

 // Change application to your Application child here if you want.

#if !defined(USE_GLFW) // Fucking WinAPI.
 app.hinstance = hinstance;
 app.hprevinstance = hprevinstance;
#endif

 app.Main();

 // Clear Out COM
 CoUninitialize();

 return (int) (app.msg.wParam); // return to Windows
} // end WinMain