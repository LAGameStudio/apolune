// This file is the main entry point for the Logitech Api
// Designed to offer some controls on extra features of 
// Logitech Devices

// Support: 6464543200  /  1 support -> 1 windows -> 1 gaming  /  1 support -> 1 windows -> 2 cd
// Ref # 110616-000480
// 1-week auto-resolved

#pragma once

#ifndef LKITAPI_EXPORTS 
	#pragma comment( lib, "lgKit1.1.lib" )
#endif

#define LK_SUCCESS								0

// Errors control code
#define LK_ERROR_NO_LOGITECH_LIB_FOUND			-1
#define LK_ERROR_WRONG_LOGITECH_LIB_FOUND		-2
#define LK_ERROR_NOT_IMPLEMENTED				-3
#define LK_ERROR_MULTIPLE_REGISTRATION_NOT_AUTHORIZED -4
#define LK_ERROR_INTERNAL_KHAL					-5
#define LK_ERROR_INVALID_PARAMETERS				-6

// Information control code (this means the function succeed)
// Can be ignored and asssumed being equal to a return of LK_NO_ERROR
#define LK_INFO_NO_MOUSE_WITH_LED_FOUND			0x0002
#define LK_INFO_NO_MOUSE_FOUND					0x0003


typedef enum LK_BUTTON_STATUS
{
	LK_UP						= 0x00000000,
	LK_DOWN						= 0x00000001
} *PLK_BUTTON_STATUS;


typedef struct _LK_MOUSE_INPUT
{
	WORD wButtonNumber;
    LK_BUTTON_STATUS ButtonStatus;
} LK_MOUSE_INPUT, *PLK_MOUSE_INPUT;


#define LK_FCT_DEC	__cdecl

#ifdef LKITAPI_EXPORTS
#define LK_FCT_API	__declspec(dllexport) LK_FCT_DEC
#else
#define LK_FCT_API	__declspec(dllimport) LK_FCT_DEC
#endif

int LK_FCT_API LgKitInit() ;


// Extra Mouse Button functions
// Use this function to get notified of Extra Mouse buttons (buttons 5 and up)
// You will then receive notifications of extra buttons being pressed, released)
// using your regsitered wmUserMessage
// Parameter of the message received are :
//
//	WPARAM (PLK_MOUSE_INPUT*) pLKMI ;
//  LPARAM lParam;
int LK_FCT_API LgKitRegisterMouseButtonsNotifications(HWND hWnd, DWORD wmUserMessage, BOOL boAllButtons) ;
int LK_FCT_API LgKitUnRegisterMouseButtonsNotifications() ;

// Resolution control Api
// This function returns the currently maximum resolution supported
// by current devices.
// Will give a value of 400 and return LK_INFO_NO_MOUSE_FOUND if no Logitech
// Mouse is connected at the time of call.
int LK_FCT_API LgKitGetMaxResolution(int* maxRes) ;

// Allow to set the real resolution of connected Logitech Mice
// Will return LK_INFO_NO_MOUSE_FOUND if no Logitech
// Mouse is connected at the time of call.
// Will return LK_INFO_NO_MOUSE_WITH_MAXRES_FOUND if no Logitech
// Mouse supporting this resolution is connected at the time of call.
// Other mice will be set to their highest resolution possible
int LK_FCT_API LgKitSetCurrentResolution(int maxRes) ;
int LK_FCT_API LgKitGetCurrentResolution(int &maxRes) ;

// Mouse LED control Api
// This function returns the currently maximum number of LED supported
// by current devices. Max LED number is 32
int LK_FCT_API LgKitGetLEDNumber(int* numLED) ;

// Set the LED on and off according to the bit field passed as a parameter
// If there's 3 LEDs availables :
// LEDOnOfMask at 0x0000 (All LEDs are OFF)
// LEDOnOfMask at 0x0001 (first LED is ON, the other 2 are OFF)
// LEDOnOfMask at 0x0002 (second LED is ON, the other 2 are OFF)
// LEDOnOfMask at 0x0004 (third LED is ON, the other 2 are OFF)
// LEDOnOfMask at 0x0005 (First and third LED are ON, the second is OFF)
// LEDOnOfMask at 0x0007 (All LEDs are ON)
int LK_FCT_API LgKitSetLED(int LEDOnOfMask) ;

// Lift Detection functions
// Use this function to get notified of lift detection event.
// You will then receive notifications when the device is lifted or put down
// using your regsitered wmUserMessage
// Parameter of the message received are :
//
//	WPARAM (PLK_MOUSE_INPUT*) pLKMI ;
//  LPARAM lParam;
int LK_FCT_API LgKitRegisterLiftDetectionNotifications(HWND hWnd, DWORD wmUserMessage) ;
int LK_FCT_API LgKitUnRegisterLiftDetectionNotifications() ;




