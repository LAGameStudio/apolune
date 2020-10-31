// structs.h

#pragma once

#include "MMIoctl.h"

#define MAX_ELO_MONITORS 64
#define MAX_ELO_TOUCHSCREENS 64

// Beep related
#define BEEP_DEF_FREQUENCY  900
#define BEEP_DEF_DURATION   130

#define MMD_TOUCH		0
#define MMD_RELEASE		1
#define MMD_DRAG		5
#define	MMD_DCLICK		6

// -----------------------------------------------------------------------------
// Defines a touch co-ordinate buffer. 
typedef struct tagTOUCHPOINT
{
	LONG			x;
	LONG			y;
	LONG			z;
	TOUCH_STATUS	Status;			// OUT- Z value of touch
	int				nScreenIndex;
	int				nMonitorNumber; // 1-based Windows Monitor Number
} TOUCHPOINT;

typedef struct MONITORS_TAG
{
	int		win_num;	// Windows monitor number
	int		x;
	int		y;
	int		width;
	int		height;
	DWORD   orientation;
	DWORD   edid;		// Monitor serial number from EDID 
	HMONITOR hMon;
    HANDLE  hMappedPointerDevice;
	wchar_t device_id [MAX_PATH];
} MONITOR;

typedef struct _AccelBounds		// IN Set calibrate data input buffer
{
	LONG	X_Max;
	LONG	X_Min;
	LONG	Y_Max;
	LONG	Y_Min;
	LONG	Z_Max;
	LONG	Z_Min;
} AccelBounds;

typedef struct _ACCELDATA
{
	ULONG		Enable;
	ULONG		Scale;
	AccelBounds	Bounds[1];
} ACCELDATA;

typedef struct SCREEN_TAG
{
	HANDLE			hDevice;
	HANDLE			hThread;
    HANDLE          hDeviceNotify;
	int             nScreenIndex;       // enum dev index
    USHORT          uVendorID;          // HidD_GetAttributes(): HIDD_ATTRIBUTES.VendorID
    USHORT          uProductID;         // HidD_GetAttributes(): HIDD_ATTRIBUTES.ProductID
	USHORT          uVersionNumber;     // HidD_GetAttributes(): HIDD_ATTRIBUTES.VersionNumber == USB_DEVICE_DESCRIPTOR.bcdDevice from Elo USB and APR driver
	wchar_t			szDevicePath [MAX_PATH];
	ACCELDATA		accel_data;
	MMDevCalData	cal_data;
	LONG            lSmartSetToken;   // SmartSet token returned by the driver when handling SmartSet commands
    DWORD           dwSSTokenThreadId;// ID of the thread which is holding SmartSet token

	// Touch Rectangles
	MMDevIO_CLIPPING_MODE clip_mode; // defined in mmioctl.h
	MONITOR*		pMonitor; // pointer to the monitor item in MONITOR array in CMonitor

} SCREEN;

typedef struct _SmartsetData
{
	LONG ScreenNumber;

	union
	{
		char Command[8];
		USHORT Response;
	};
} SMARTSETDATA, *PSMARTSETDATA;

typedef struct _SmartsetPkt
{
	unsigned char pkt[8];
} SMARTSET_PKT;