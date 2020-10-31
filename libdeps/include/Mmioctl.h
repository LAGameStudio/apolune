// -----------------------------------------------------------------------------
// mmioctl.h    Include file for MonMouse Port I/O  Driver
//
// Copyright (c) 1995, 1996, 1997, 1998, 1999,2000, 2001, 2001  Elo Touch Solutions, Inc. 
// All rights reserved.
//
// Define the IOCTL codes we will use.  The IOCTL code contains a command
// identifier, plus other information about the device, the type of access
// with which the file must have been opened, and the type of buffering.
//
// NOTE: 
// All IO Control calls for MMWin95 require at least a header for both 
// input and output.  The input header is used to pass the touchscreen 
// number and the output is used for the error.  They both are used to 
// pass the version lock input to the driver, & output to the application
//   
// The way the driver varifies this is that both the input and output 
// sizes must at least be as big as a header, otherwise you will get an 
// error return code from DeviceIOControl.
//
// The driver then checks the version locking, if a call comes in to the 
// driver with a different version lock than the driver was compiled under 
// the driver simply sets the version lock error in the header and returns 
// to the application with out any work being done.
//   
//
// NOTE2: 
// There is a small set of calls that can be done if a controler is not 
// working:  The Set/Get reset flag and the diagnostic call, all others 
// will return a MMErrorTouchDeviceNotWorking code. In the case of the 
// diagnostic call only the controler status is known good, other values 
// may or may not be valid.
//
//
// Anthony Winner        01/01/96  VersionLock 1
//
// Anthony Winner AJW2    05/09/96  VersionLock 2
//    Added some Controller status for VComm Version
//    Added a flag in Zaxis supported struct
// 
// Anthony Winner AJW3    01/09/97  VersionLock 3
//    Added a ioctl to Set the mouse button
//
// Anthony Winner AJW4    11/03/97  VersionLock 4
// Started NT conversion changes follows: removed 
//
//  NOTE: VersionLock 4 was for the branch into NT.
//  The NT and Win95 versions of this file were incompatible.
//
// Peter C. Studt        2/12/98        
//    Added an ioctl to set the default drag delay parameter.
// 
// Scott Coleman       08/28/98  VersionLock 5
// This version of MMIOCTL.h works in Win95 and Win98 WDM 
// (and should work in NT)
//
// Scott Coleman       09/24/99     VersionLock 6
//                                 Start of the Universal driver API
//                                 Enhanced the Enum IOCtl
//                                 Added the USB Smartset IOCtls
//                                 Added device change query
//
// Peter C. Studt        11/5/99 Version Lock 7
// Peter C. Studt        1/21/99 Version Lock 8
// Scott Coleman        2/3/99  Version Lock 9
//                                Added Flags field to Enum structure
//
// This file is a consolidation of the Win95, Win98, and NT Ioctl files.
// Changed all FILE_READ_ACCESS to FILE_ANY_ACCESS
// -----------------------------------------------------------------------------

#ifndef _MMIOCTL_H_
#define _MMIOCTL_H_

// #define VERSIONLOCK  14    // Header version lock, used to make sure app and device are in sync.
// #define VERSIONLOCK  15
#define VERSIONLOCK  50  // 2008-10-17, for G2 1.0.0.0 RC2

//#pragma pack( push, enter_MMIOCTL,8 )
#pragma pack( push, enter_MMIOCTL,1 )

#ifndef _NTDDK_                // If not _NTDDK_ , then we are compiling for a 
    #include <winioctl.h>    // Win32 app. and need to include winioctl.h
#endif


#define DATA_FILE_MAX_PATH 256
#define DATA_FILE_MAX_NAME 32
#define PROGRAM_FILE_MAX_NAME 64

//#define ELO_BEEP_EVENT_NAME L"Global\\Elo Beep Event"
#define ELO_BEEP_EVENT_NAME L"Elo MT Beep Event"
#define ELO_BEEP_PARAMS_CHANGED    L"Elo MT Beep Params Changed"

// EloSawJr usb is using first 64 bytes + cal data of the NVRam scratch area,
// since the cal data is about 18 bytes, we use the start address of 128 bytes here
#define NVRam_PreCal_Offset 128
#define ELO_IOCTL_REPORT_ID   0x20 // self defined report ID

#define ELO_MT_MAX_COUNT 10 // maximum number of contacts (used by both apps and usb driver).

#define	REG_SMARTSET_COMMANDS L"SmartSetCommands"

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//    Header
//
//    Each IOCtl has it's own data structure used to pass the parameters in and out
//    of the driver. These data structures always start with the Header structure.

typedef enum _MM_OP_CODE
{
MM_OP_CODE_MIN,

MM_ENABLE,
MM_GETPOINTS,
MM_GETMULTITOUCH,
MM_CLEARGETPOINT,
MM_CALDATA,

MM_CLIPPING_MODE,
MM_FULL_SCREEN_CLIPPING,
MM_EDGE_ACCELERATION,

MM_MOUSEMODE,
MM_QUICK_TOUCH,
MM_DRAGDELAY,
MM_UNTOUCH_TIMEOUT,

MM_RIGHT_CLICK_ON_HOLD,
MM_RIGHT_CLICK_EVENT,
MM_SWAPBUTTONS,
MM_LEFT_HANDED_USER,
MM_BUTTON_SEQ,

MM_GET_DRIVER_BUILD_TIME,
MM_DIAGNOSTICS,
MM_DEBUG_LEVEL,
MM_BEEP_EVENT,
MM_BACKGAMMON_BUTTONS_EVENT,

//MM_SMARTSET_TIMEOUT,
MM_SMARTSET,
MM_DEVICE_CONTEXT,
MM_PLAY_MOUSE,
MM_PLAY_TOUCH,

// APR Only
MM_LOAD_USER_FILE,
MM_SET_INPUT_GAIN,
MM_GET_LIVE_SOUND,
MM_LIVE_SOUND_THRESHOLDS,
MM_MODE_SWITCHES,
MM_START_AUDIO_RECORD,
MM_GET_SERIAL_NUMBERS,
MM_GET_FLASH_PROGRESS,
MM_TRACKER_PARAMS,
MM_FW_CONTROL,
MM_FSR_STATE,
MM_LIFT_THRESHOLD,
MM_DRIVER_DEFAULT,
MM_REGISTER_ASYNC_EVENT,
MM_QUERY_ASYNC_EVENT,

MM_AUTOSIZE,
MM_DRIVER_BEEP,
MM_MAP_SCREEN,

MM_SET_CONTROLLER_REPORT_MODE,
MM_MAX_TOUCH,

MM_OP_CODE_MAX
} MM_OP_CODE;


typedef enum _MM_ERROR_CODE    // Error returns in the header     
{
    MMErrorSuccess=0,
    MMErrorInvalidCommand,
    MMErrorVersionLock,
    MMErrorBufferSize,
    MMErrorTouchDeviceNotWorking,
    MMErrorNotImplemented,
    MMErrorDeviceBusy,
    MMErrorNoActionTaken
} MM_ERROR_CODE;

typedef enum _DIRECTION_CODE        // Direction of data in the ioctl call     
{
    ToDriver=1,
    FromDriver
} DIRECTION_CODE ;

typedef struct _MMDevIOHeader            // The header structure
{
    UCHAR               ReportId;
    MM_OP_CODE          OpCode;
    ULONG               Size;
    USHORT              DriverVersion;      // OUT- Driver VERSIONLOCK number, 
                                            //      Returned by the driver.
    USHORT              ApplicationVersion; // IN-  Application VERSIONLOCK number, 
                                            //      Set by the application
    DIRECTION_CODE      DirectionFlag;      // IN-  Determines the direction of data being sent,
                                            //      To / From  Driver
    MM_ERROR_CODE       ReturnValue;        // OUT- Error code from driver, will 
                                            //      be in 'output buffer' header
    int                 nScreenIndex;       // Used by app only. Kernel ignors it.     
    unsigned __int64    DeviceContext;      // non-zero if MM_GETPOINTS returns valid touch data
} MMDevIOHeader, *PMMDevIOHeader;                

#define SIZEOF_MMDevIOHeader sizeof(MMDevIOHeader)

// -----------------------------------------------------------------------------
//    MM_ENUMTOUCHSCREENS
//
//    Enumerates all of the touchscreens for the called driver.
//    The TouchScreenNumber field in the header is used to index through each
//    touchscreen. Once the caller asks for a touchscreen index that does not 
//    exist, the driver will return the MMErrorNoMoreTouchScreens error.
//    Use the MMDevIOENUMTOUCHSCREENS data structure to pass the data.


#define    TOUCHSCREEN_TYPE_USB            0x01
#define    TOUCHSCREEN_TYPE_SERIAL         0x02
#define    TOUCHSCREEN_TYPE_APR             0x03

#define    TOUCHSCREEN_FLAG_READY            0x01    // The device is ready to process touch packets

#define EnumEnableTouch 1


typedef    struct
{
    UCHAR        T;                        // Leading 'T' for touch packet
    UCHAR        Status;                    // Touch status
    SHORT        X;                        // X value
    SHORT        Y;                        // Y value
    SHORT        Z;                        // Z value
} TOUCHPACKET, *PTOUCHPACKET;

typedef enum tagTouchStatus   // the blocked call returns data depending on this flag
{
    InitialTouch = 1,
    StreamTouch = 2,
    UnTouch = 4
} TOUCH_STATUS ;

typedef	struct tagTouch
{
	TOUCH_STATUS status; 
	UCHAR  id;     // touch id: 0 or 1 repressenting the touch finger
	USHORT x;      // x of a touch point
	USHORT y;      // y of a touch point      
} TOUCH;

typedef struct
{
	TOUCH touch [ELO_MT_MAX_COUNT]; // max number of touches (fingers)
    int count; // actual touch count 
} MT_TOUCH;

// -----------------------------------------------------------------------------
//    MMDevIOGETPOINTS
//
//    Gets the last touch point data from the specified touchscreen.
//    This is a blocking call.
//    Use the MMDevIOGETPOINTS data structure to pass the data.
typedef enum _GETPOINTS_CODE    // the blocked call returns data depending on this flag
{
    NoRequest = 0,
    ReturnImmediately,
    ReturnOnTouch,
    ReturnOnUntouch,
    ReturnOnNextValidTouch,
    
}GETPOINTS_CODE ;

typedef enum _GETPOINTS_TRANSLATION
{
    Raw =1,
    Translated,
    ControllerDirect
}GETPOINTS_TRANSLATION ;

typedef struct _MMDevIOGETPOINTS    // IN/OUT - Get Points buffer
{
    MMDevIOHeader            Header;
    BOOL                    ValidFlag;    // OUT-if TRUE, points are valid untouch points 
    GETPOINTS_TRANSLATION    Translation;// OUT-if TRUE, points are translated
    LONG                    XValue;        // OUT- x value of touch
    LONG                    YValue;        // OUT- x value of touch
    LONG                    ZValue;        // OUT- Z value of touch
    TOUCHPACKET                Controller_Packet;
    TOUCH_STATUS            Status;        
    GETPOINTS_CODE    GetPointsFlag; // IN - the blocked call returns data depending on this flag
} MMDevIOGETPOINTS, *PMMDevIOGETPOINTS;

#define SIZEOF_MMDevIOGETPOINTS sizeof(MMDevIOGETPOINTS)


typedef struct _MMDevIOGETMULTITOUCH
{
    MMDevIOHeader	Header;
    MT_TOUCH		mtTouch;
    int				return_count; // actual number of touches returned to user app
} MMDevIOGETMULTITOUCH;

#define SIZEOF_MMDevIOGETMULTITOUCH sizeof(MMDevIOGETMULTITOUCH)

typedef struct _MMDevIOSetControllerReportMode
{
	MMDevIOHeader	Header;
	UCHAR			ReportMode; // MODE_MOUSE or MODE_MULTI_TOUCH
} MMDevIOSetControllerReportMode;

#define SIZEOF_MMDevIOSetControllerReportMode sizeof(MMDevIOSetControllerReportMode)

// -----------------------------------------------------------------------------
//    MM_CLEARGETPOINT
//
//    Invalidates the last get point. See MMDevIOGETPOINTS.
//    Cancels the blocked GetPoints call.
//    Use the MMDevIOCLEARGETPOINT data structure to pass the data.
//


typedef struct _MMDevIOCLEARGETPOINT
{
    MMDevIOHeader    Header;
    GETPOINTS_TRANSLATION    Translation;    // OUT-if TRUE, points are translated
} MMDevIOCLEARGETPOINT, *PMMDevIOCLEARGETPOINT;

#define SIZEOF_MMDevIOCLEARGETPOINT sizeof(MMDevIOCLEARGETPOINT)


// -----------------------------------------------------------------------------
//    MM_SET_WIN2K_CALDATA
//
//    Sets the calibration for the specified touchscreen.
//    Use the MMDevWin2kCalData data structure to pass in the data.

typedef struct _MMDevCalData        // IN Set calibrate data input buffer
{
    MMDevIOHeader    Header;

    LONG            EloDx;
    LONG            ScrDx;
    LONG            X_Offset;
    LONG            EloDy;
    LONG            ScrDy;
    LONG            Y_Offset;
    LONG            EloDz;
    LONG            ScrDz;
    LONG            Z_Offset;
    LONG            xyswap;    

    ULONG           WindowsMonitorNumber;
	ULONG           MonitorSerialNumber;
	unsigned short	Checksum;
    int				nScreenIndex;
} MMDevCalData, *PMMDevCalData;
#define SIZEOF_MMDevCalData sizeof(MMDevCalData)

// -----------------------------------------------------------------------------
// Set virtual desktop mode
// Define clipping rectangles

#ifndef _CLIPDEF
#define _CLIPDEF
typedef struct _ClippingBounds        // IN Set calibrate data input buffer
{
    long            X_Max;
    long            X_Min;
    long            Y_Max;
    long            Y_Min;
    long            Z_Max;
    long            Z_Min;
} ClippingBounds, *PClippingBounds;
#define MAX_BOUND    64
#endif

typedef enum 
{
    enumVrtlDeskDisabled = 0,    // no virtual desktop
    enumVrtlNoBounds,            // NO virtual desktop bounds, No Clipping, Cursor visible
    enumVrtlBoundsClipped,       // virtual desktop bounds enable, Clipping, Cursor moves at bounds
    enumVrtlBoundsFreeze        // virtual desktop bounds enable, Clipping, Cursor Freezes at bounds
} VrtlBoundMode;

// mn_note remember to devide by two for USB.
#define MIN_X_BOUNDS    0x0000
#define MAX_X_BOUNDS    0xFFFF
#define MIN_Y_BOUNDS    0x0000
#define MAX_Y_BOUNDS    0xFFFF
#define MIN_Z_BOUNDS    0x00
#define MAX_Z_BOUNDS    0xFF

typedef struct _MMDevIO_CLIPPING_MODE
{
    MMDevIOHeader    Header;
    VrtlBoundMode    ClippingMode;
    ULONG            NumBounds;
    ULONG            ExclusionFlag;
    ClippingBounds    Bounds[MAX_BOUND];
    int                MonitorNumber;
} MMDevIO_CLIPPING_MODE, *PMMDevIO_CLIPPING_MODE;

#define SIZEOF_MMDevIO_CLIPPING_MODE sizeof(MMDevIO_CLIPPING_MODE)
// -----------------------------------------------------------------------------
// Set full screen virtual desktop mode




typedef struct _MMDevIO_FULL_SCREEN_CLIPPING
{
    MMDevIOHeader    Header;
    VrtlBoundMode    ClippingMode;
    ClippingBounds    Bounds[MAX_BOUND];
} MMDevIO_FULL_SCREEN_CLIPPING, *PMMDevIO_FULL_SCREEN_CLIPPING;

#define SIZEOF_MMDevIO_FULL_SCREEN_CLIPPING sizeof(MMDevIO_FULL_SCREEN_CLIPPING)


#define SIZEOF_MMDevIO_CLIPPING_MODE sizeof(MMDevIO_CLIPPING_MODE)
// -----------------------------------------------------------------------------

typedef struct _MMDevIO_EDGE_ACCELERATION
{
    MMDevIOHeader    Header;
    ULONG            Enable;
    ULONG            Scale;
    ClippingBounds    Bounds[MAX_BOUND];
} MMDevIO_EDGE_ACCELERATION, *PMMDevIO_EDGE_ACCELERATION;

#define SIZEOF_MMDevIO_EDGE_ACCELERATION sizeof(MMDevIO_EDGE_ACCELERATION)

// -----------------------------------------------------------------------------
//    MM_MOUSEMODE
//
//    Sets /Gets the mouse mode for the specified touchscreen.
//    Use the MMDevIOMOUSEMODE data structure to pass in the data.

#define MODE_CLICK_ON_TOUCH     0
#define MODE_CLICK_ON_UNTOUCH   1
#define MODE_MOUSE_EMULATION    6

typedef struct _MMDevIOMOUSEMODE    // Mouse mode input buffer
{
    MMDevIOHeader    Header;
    ULONG            MouseMode;        // IN / OUT - Mouse mode, std Elo modes 0,1 5,and 6 
                                    // Depending on the direction flag in header
} MMDevIOMOUSEMODE, *PMMDevIOMOUSEMODE;

#define SIZEOF_MMDevIOMOUSEMODE sizeof(MMDevIOMOUSEMODE)


// -----------------------------------------------------------------------------
// Set/Get the quick touch mode and X, Y parameter data.
// Sets the current values and returns the previous values. 


typedef struct _MMDevIO_QUICK_TOUCH_DATA
{
    MMDevIOHeader    Header ;
    LONG            bEnable ;        
    ULONG            Dx ;        // Quick touch limit for X axis.
    ULONG            Dy ;        // Quick touch limit for Y axis.
} MMDevIO_QUICK_TOUCH_DATA, *PMMDevIO_QUICK_TOUCH_DATA;

#define SIZEOF_MMDevIO_QUICK_TOUCH_DATA sizeof(MMDevIO_QUICK_TOUCH_DATA)

// -----------------------------------------------------------------------------
//    MM_SWAPBUTTONS
//
//    Swaps the touch screen button from the the left to right of the specified 
//    touchscreen. 



typedef struct _MMDevIOSWAPBUTTONS    // IN - tell driver new swap button status
{
    MMDevIOHeader    Header;
    ULONG            SwapCount;        // IN - Touch Count for keeping button swapped
    ULONG            Active ;        // IN /OUT Saves if right button should be run on reboot 
} MMDevIOSWAPBUTTONS, *PMMDevIOSWAPBUTTONS;

#define SIZEOF_MMDevIOSWAPBUTTONS sizeof(MMDevIOSWAPBUTTONS)

// -----------------------------------------------------------------------------
// MM_LEFT_HANDED_USER
//
// Left handed-user mode


typedef struct _MMDevIO_LeftHandedUser
{
    MMDevIOHeader Header;
    ULONG Flag ;
} MMDevIO_LeftHandedUser, *PMMDevIO_LeftHandedUser;
#define SIZEOF_MMDevIO_LeftHandedUser sizeof(MMDevIO_LeftHandedUser)

// -----------------------------------------------------------------------------
//    MM_BUTTON_SEQ
//
//    Get / Set the button sequence

#define MAX_BUTTON_BOUND    64

typedef struct _MMDevIO_Button_Seq
{
    MMDevIOHeader    Header;
    USHORT            InitialTouchSeq[MAX_BUTTON_BOUND] ;
    ULONG            NumInitialTouchSeq ;
    USHORT            StreamTouchSeq[MAX_BUTTON_BOUND] ;
    ULONG            NumStreamTouchSeq ;
    USHORT            UnTouchSeq[MAX_BUTTON_BOUND] ;
    ULONG            NumUnTouchSeq ;
} MMDevIO_BUTTON_SEQ, *PMMDevIO_BUTTON_SEQ;

#define SIZEOF_MMDevIO_Button_Seq sizeof(MMDevIO_BUTTON_SEQ)

// -----------------------------------------------------------------------------
//    MM_DRAGDELAY
//
//    Sets the drag delay for the specified touchscreen.
//    Use the MMDevIODRAGDELAY data structure to pass the data.



typedef struct _MMDevIODRAGDELAY        // IN - Get drag delay parameter
{
    MMDevIOHeader    Header;
    ULONG            MaxDragDelay;        // Maximum Drag Delay
    ULONG            MinDragDelay;        // Minimum Drag Delay
    ULONG            DragDelay;            // New drag delay time.
} MMDevIODRAGDELAY, *PMMDevIODRAGDELAY;

#define SIZEOF_MMDevIODRAGDELAY sizeof(MMDevIODRAGDELAY)


// -----------------------------------------------------------------------------
//    MM_ENABLE
//
//    Sets the calibration for the specified touchscreen.
//    Use the MMDevIOENABLE data structure to pass the data.
//    To get or set data set the correct direction flag in the header.
//    Enables / Disables the the specified touch screen. While disabled, no movement or 
//    button information will be sent to the system.



typedef struct _MMDevIOENABLE    // IN/OUT Enable buffer
{
    MMDevIOHeader    Header;
    LONG            EnableTouch;
} MMDevIOENABLE, *PMMDevIOENABLE;

#define SIZEOF_MMDevIOENABLE sizeof(MMDevIOENABLE)


// -----------------------------------------------------------------------------
//    MM_DIAGNOSTICS
//
//    Reads the diagnostic information that the driver obtained from the 
//    specified touchscreen.
//    Use the MMDevIODIAGNOSTICS data structure to pass the data.



#ifndef _CONTRL_STAT_DEF
#define _CONTRL_STAT_DEF
typedef enum _CONTRL_STAT                // ctrl_status values
{
    CS_OK = 0,
    CS_ConstantTouch,
    CS_CanNotFindController,
    CS_NoResponse,
    CS_InvalidResponse,
    CS_CanNotSetBaudRate,
    CS_CommandNotSent,
    CS_SystemError,
    CS_InvalidCommPort,
    CS_CommPortFailedOpen,
    CS_CommPortCommandError,
    CS_CommPortNoController,
    CS_UndefinedController
} CONTRL_STAT;
#endif

/* Controller class information
Controller                ID (hex)        Description
2210                    01H            AccuTouch RS-232 serial (legacy)
2310                    03H            IntelliTouch RS-232 serial (legacy)
2310B                    05H            IntelliTouch RS-232 serial
2500S                    06H            IntelliTouch RS-232 serial, standard footprint
2500U                    07H            IntelliTouch USB, standard footprint
3000U                    08H            AccuTouch USB
4000U                    09H            IR USB
4000S                    0AH            IR RS-232 serial
NGRCOACh                0BH            NGR serial
                        0CH            NGR USB
Carroll SmartSet Lite    0DH            Carroll 8052 SmartSet Lite serial
SCOACh                    0EH            Accutouch serial, chipset only
2500SG                    0FH            IntelliTouch serial gaming controller
2500SP                    06H            Serial Plug’n Play (same as 2500S)
reserved                40H            Projected capacitive controller
2215                    01H            Small Form Factor AccuTouch RS-232 serial
4500U                    30H            IR USB
5000RSU                    41H            Surface capacitive controller
4500S                    31H            IR RS-232 serial
2216                    50H            Universal COACh: 5/4 wire, USB/serial
2700                    20H            ESAU controller -Serial/USB
Force ctlr                60H            Okunishi-san’s Force Sensor controller
*/

enum
{
    CONTROLLER_CLASS_2210 = 1,        // 2210 controller
    CONTROLLER_CLASS_2310 = 3,        // 2310 controller
    CONTROLLER_CLASS_2310B = 5,        // 2310B controller
    CONTROLLER_CLASS_2500S = 6,        // 2500S controller
    CONTROLLER_CLASS_2500U = 7,        // 2500U controller
    CONTROLLER_CLASS_3000U = 8,        // 3000U controller
    CONTROLLER_CLASS_4000U = 9,        // 4000U controller
    CONTROLLER_CLASS_4000S = 10,    // 4000S controller
    CONTROLLER_CLASS_NGRCOACHS = 11,    // NGRCOACh serial controller
    CONTROLLER_CLASS_NGRCOACHU = 12,    // NGRCOACh USB controller
    CONTROLLER_CLASS_CARROLL_SMARTSET_LITE = 13,    // Carroll SmartSet Lite controller
    CONTROLLER_CLASS_SCOACH = 14,    // SCOACh controller
    CONTROLLER_CLASS_2500SG = 15,    // 2500SG controller
    CONTROLLER_CLASS_2700U = 32,    // 2700U, 27001U & 27001S controllers have the same id 
    CONTROLLER_CLASS_ITPLUS_2510S = 0x21, // IntelliTouch Plus 2510S,  MT
    CONTROLLER_CLASS_ITPLUS_2515XYU = 0x22, // IntelliTouch Plus 2515U,  MT, Lenovo 23", XYU
    CONTROLLER_CLASS_ITOUCHPLUS_2515A = 0x23, // Plan-A, iTouch Plus 2515 - 19in, MT
    CONTROLLER_CLASS_ITOUCHPLUS_2515B = 0x24, // Plan-B, iTouch Plus 2515 - 19in, MT
    CONTROLLER_CLASS_ITOUCHPLUS_2515C = 0x26, // Plan-C
    CONTROLLER_CLASS_ITOUCHPLUS_2521 = 0x27, // Henry's true dual-touch HID (based on plan XYU's HID table)
    CONTROLLER_CLASS_4500U = 48,    // 4500U controller
    CONTROLLER_CLASS_4500S = 49,    // 4500S controller
    CONTROLLER_CLASS_PROJECTED_CAPACITIVE = 64,    // reserved, Projected capacitive controller
    CONTROLLER_CLASS_5000RSU = 65,    // 5000RSU controller
    CONTROLLER_CLASS_5020 =  66,    // Hampshire 5020 controller, 0x42
    CONTROLLER_CLASS_APR1 = 0x71,    // APR USB controller 0x7000
    CONTROLLER_CLASS_2216 = 80,        // 2216 controller
    CONTROLLER_CLASS_FORCE = 96,        // Force controller
    CONTROLLER_CLASS_APR73 = 0x73, // APR USB controller 7001
    CONTROLLER_CLASS_APR74 = 0x74, // APR controller with FSR
    CONTROLLER_CLASS_APR7010 = 0x7A, // APR Smartset controller
    CONTROLLER_CLASS_APR7020 = 0x7C, // APR Smartset controller
    CONTROLLER_CLASS_APR7021 = 0x7D, // APR Smartset controller
    CONTROLLER_CLASS_APR7002 = 0xFA,     // * * * 06-15-2010: Massoud will return these 2 PIDs (FA, FB) for app to distinguish
    CONTROLLER_CLASS_APR7002_FSR = 0xFB, // APR-7002 because the controller group does not have a separate PID for 7002.
    CONTROLLER_CLASS_UNKNOWN = 0xFF    // Unknown controller
};


#define    COMPORT_NAME_LENGTH                256
#define    STACKDEVICE_NAME_LENGTH            256
#define    SERIALNUMBER_NAME_LENGTH        32
#define USB_PRODUCT_STRING_LENGTH        64
#define SCF_HEADER_SERIAL_NUMBER_LENGTH 12

typedef struct _MMDevIODIAGNOSTICS
{
    MMDevIOHeader    Header;
    CONTRL_STAT        ctrl_status;    // OUT- Controller Status 
    ULONG            HardwareHandShaking; // OUT-Hardware handshaking turned on /off
    LONG            BaudRate;        // OUT- Baud rate of controler, 0 for bus
    unsigned char    crevminor;        // OUT- controller rev minor number
    unsigned char    crevmajor;        // OUT- controller rev major number
    unsigned char    trevminor;        // OUT- Unused
    unsigned char    trevmajor;        // OUT- Unused
    unsigned char    diagcodes[8];    // OUT- Diag codes ret from controller
    unsigned char    id[8];            // OUT- OEM ID string ret from controller
    unsigned char    cnt_id[8];        // OUT- Full Smartset controller ID packet 
    unsigned char    driver_id[32];    // OUT- Driver ID

    // add on fields from enum ioctl
    ULONG            Type;                        // OUT- TOUCHSCREEN_TYPE_USB, TOUCHSCREEN_NT_SERIAL, ...
    wchar_t            PortFriendlyName[COMPORT_NAME_LENGTH]; // OUT- Used for serial touchscreens
    wchar_t         wcUsbProductString [USB_PRODUCT_STRING_LENGTH];
    wchar_t            SerialNumber[SERIALNUMBER_NAME_LENGTH];    // OUT- ASCII 8-digit serial#

    // APR only
    char szCalFileName [DATA_FILE_MAX_NAME];
    int nCalFileVerMajor;
    int nCalFileVerMinor;
    char szSensorSN [SCF_HEADER_SERIAL_NUMBER_LENGTH];
    // End APR only
} MMDevIODIAGNOSTICS, *PMMDevIODIAGNOSTICS;

#define  SIZEOF_MMDevIODIAGNOSTICS sizeof(MMDevIODIAGNOSTICS)



// -----------------------------------------------------------------------------
//    MM_DEBUG_LEVEL
//



typedef struct _MMDevIODebug_Level    // IN/OUT Enable buffer
{
    MMDevIOHeader    Header;
    ULONG            DebugLevel ;
} MMDevIODebug_Level, *PMMDevIODebug_Level;

#define SIZEOF_MMDevIODebug_Level sizeof(MMDevIODebug_Level)

// -----------------------------------------------------------------------------
//    MM_RIGHT_CLICK_ON_HOLD
//



typedef struct _MMDevIORight_Click_On_Hold    // IN/OUT Enable buffer
{
    MMDevIOHeader    Header;
    ULONG            RightClickHW ;
    ULONG            InitialTimeout ;
    ULONG            DefaultRightClickDelay ;
    ULONG            MaxRightClickDelay ;
    ULONG            MinRightClickDelay ; 
    ULONG            ClickCount;        // IN - Touch Count for enabling right click on hold feature 
    ULONG            Active ;        // IN /OUT Saves if this feature should be turned on , on reboot 
} MMDevIORight_Click_On_Hold, *PMMDevIORight_Click_On_Hold;

#define SIZEOF_MMDevIORight_Click_On_Hold sizeof(MMDevIORight_Click_On_Hold)


// -----------------------------------------------------------------------------
//    MM_RIGHT_CLICK_EVENT
//

#define SZRIGHTCLICKSTARTEVENT "Global\\RightClickStart"
#define SZRIGHTCLICKSTOPEVENT "Global\\RightClickStop"



typedef struct _MMDevIORightClickEvent    
{
    MMDevIOHeader    Header;
    unsigned __int64        hStartRightClick ;
    unsigned __int64        hEndRightClick ;
} MMDevIORightClickEvent, *PMMDevIORightClickEvent ;

#define SIZEOF_MMDevIORightClickEvent sizeof(MMDevIORightClickEvent)

// -----------------------------------------------------------------------------
//    MM_UNTOUCH_TIMEOUT
//



typedef struct _MMDevIOUntouchTimeout    
{
    MMDevIOHeader    Header;
    ULONG            TimeOut;
    ULONG            Width;
    ULONG            Height;
} MMDevIOUntouchTimeout, *PMMDevIOUntouchTimeout ;

#define SIZEOF_MMDevIOUntouchTimeout sizeof(MMDevIOUntouchTimeout)


typedef struct _MMDevIOPlayMouse
{
    MMDevIOHeader    Header;
    LONG            X;        // IN- TRUE = Beep is on
    LONG            Y;        // IN- Fequency in Hertz
    LONG            Flags;        // IN- Duration in ms
} MMDevIOPlayMouse, *PMMDevIOPlayMouse;

#define SIZEOF_MMDevIOPlayMouse sizeof(MMDevIOPlayMouse)



// -----------------------------------------------------------------------------
//    MM_LOAD_USER_FILE
//

typedef enum _USER_FILE        // Direction of data in the ioctl call     
{
    SCF=1,
    LUT,
    AUDIO
} USER_FILE ;



typedef struct _MMDevIOLoadUserFile    
{
    MMDevIOHeader    Header;
    ULONG            Length;
    USER_FILE        UserFile;
    char            Filename[64];
    ULONG            CheckSum;
    ULONG            VerifyOnly;
    unsigned int    Buffer[1];
} MMDevIOLoadUserFile, *PMMDevIOLoadUserFile ;

#define SIZEOF_MMDevIOLoadUserFile sizeof(MMDevIOLoadUserFile)


// -----------------------------------------------------------------------------
//    MM_SET_INPUT_GAIN
//

typedef struct _MMDevIOSetInputGain    
{
    MMDevIOHeader    Header;
    ULONG            FeatureUnit;
    ULONG            InputGain;
} MMDevIOSetInputGain, *PMMDevIOSetInputGain ;

#define SIZEOF_MMDevIOSetInputGain sizeof(MMDevIOSetInputGain)

// -----------------------------------------------------------------------------
typedef enum _LIVE_SOUND_MODE    
{
    LiveSoundSignal = 1,
    LiveSoundSpectrum,
    LiveSoundMatchSpectrum,
    LiveSoundBalanceData
}LIVE_SOUND_MODE ;

// -----------------------------------------------------------------------------
//



typedef struct _MMDevIOGetLiveSound    
{
    MMDevIOHeader    Header;
    LIVE_SOUND_MODE    Mode;            
    LONG            Length;
    LONG            Channels;
    LONG            DriverResidual;
    char            *DriverPointer;
    UCHAR            Buffer[4];        
} MMDevIOGetLiveSound, *PMMDevIOGetLiveSound;

#define SIZEOF_MMDevIOGetLiveSound sizeof(MMDevIOGetLiveSound)





typedef struct _MMDevIOLiveSoundThresholds
{
    MMDevIOHeader    Header;
    ULONG            TouchThreshold;
    ULONG            UntouchThreshold;
    ULONG            VarianceThreshold;
} MMDevIOLiveSoundThresholds, *PMMDevIOLiveSoundThresholds;

#define SIZEOF_MMDevIOLiveSoundThresholds sizeof(MMDevIOLiveSoundThresholds)



typedef enum __MODE_SWITCH    
{
    eUseHashTests = 1,
    eUseLocalSearch
}MODE_SWITCH ;



typedef struct _MMDevIOModeSwitches
{
    MMDevIOHeader    Header;
    MODE_SWITCH            Switch;
    ULONG                Value;
} MMDevIOModeSwitches, *PMMDevIOModeSwitches;

#define SIZEOF_MMDevIOModeSwitches sizeof(MMDevIOModeSwitches)







typedef struct _MMDevIOSmartSetTimeout
{
    MMDevIOHeader    Header;
    ULONG                Timeout;
} MMDevIOSmartSetTimeout, *PMMDevIOSmartSetTimeout;

#define SIZEOF_MMDevIOSmartSetTimeout sizeof(MMDevIOSmartSetTimeout)



typedef struct _RECORD_ALL_FILE_HEADER
{
            char fft_type;         // J for Jeff fft, T for Tony fft
            short int number_frames;      //number of frames recorded
            char dummy[32-sizeof(char)-sizeof(short int)]; //reserved for expansion, always = 0
} RECORD_ALL_FILE_HEADER, *PRECORD_ALL_FILE_HEADER;


// Data format: begin frame

typedef struct _TONY_FFT_RECORD
{
    char pd[1024];
} TONY_FFT_RECORD, *PTONY_FFT_RECORD;


typedef struct {
    short r;
    short i;
}cpx;

typedef struct _JEFF_FFT_RECORD
{
    cpx lc[1024];
    cpx rc[1024];
} JEFF_FFT_RECORD, *PJEFF_FFT_RECORD;



// Touch data format: per frame

typedef struct _TOUCH_DATA_RECORD
{
        int status;  // 0 means no valid touch, 1 = valid touch
        int X;
        int Y;
        int level;    //include current values for level,variance, and score
        int variance;    //even if the touch is not valid
        int score;
        char dummy[32-6*sizeof(int)]; // reserved, always = 0
} TOUCH_DATA_RECORD, *PTOUCH_DATA_RECORD;

#define SIZE_OF_JEFF_FFT_RECORD_ALL ( (1024  * 2 * sizeof(short)) + sizeof(JEFF_FFT_RECORD) + sizeof(TOUCH_DATA_RECORD))
#define SIZE_OF_TONY_FFT_RECORD_ALL ( (1024  * 2 * sizeof(short)) + sizeof(TONY_FFT_RECORD) + sizeof(TOUCH_DATA_RECORD))



//End frame:
//=====================================================================        
// Repeat frame format for number_frames
 


typedef enum _AUDIO_RECORED        // Direction of data in the ioctl call     
{
    RECORD_SIG=1,
    RECORD_LVS,
    RECORD_ALL
} AUDIO_RECORD ;




typedef struct _MMDevIOStartAudioRecord
{
    MMDevIOHeader    Header;
    AUDIO_RECORD    Record;
    ULONG            Frames;
} MMDevIOStartAudioRecord, *PMMDevIOStartAudioRecord;

#define SIZEOF_MMDevIOStartAudioRecord sizeof(MMDevIOStartAudioRecord)







#define BUILD_TIME_LENGTH 128
typedef struct _MMDevIOGET_DRIVER_BUILD_TIME    
{
    MMDevIOHeader    Header;
    CHAR            BuildTime[BUILD_TIME_LENGTH];        
                                    
} MMDevIOGET_DRIVER_BUILD_TIME, *PMMDevIOGET_DRIVER_BUILD_TIME;

#define SIZEOF_MMDevIOGET_DRIVER_BUILD_TIME sizeof(MMDevIOGET_DRIVER_BUILD_TIME)




typedef struct _MMDevIOPointer_Stabilization    
{
    MMDevIOHeader    Header;
    LONG            Height;
    LONG            Width;
    
} MMDevIOIOPointer_Stabilization, *PMMDevIOIOPointer_Stabilization;

#define SIZEOF_MMDevIOIOPointer_Stabilization sizeof(MMDevIOIOPointer_Stabilization)





#define USB_SERIAL_NUMBER_LENGTH 10

typedef struct _MMDevIOGET_SERIAL_NUMBERS    
{
    MMDevIOHeader    Header;
    WCHAR            UsbSerialNumber[USB_SERIAL_NUMBER_LENGTH];        
    CHAR            SensorSerialNumber[SCF_HEADER_SERIAL_NUMBER_LENGTH];
    CHAR            CalFileInUse[DATA_FILE_MAX_NAME];
    UCHAR            Diagnostics;
    LONG            CalFileMajorVerion;
    LONG            CalFileMinorVerion;

                                    
} MMDevIOGET_SERIAL_NUMBERS, *PMMDevIOGGET_SERIAL_NUMBERS;

#define SIZEOF_MMDevIOGET_SERIAL_NUMBERS sizeof(MMDevIOGET_SERIAL_NUMBERS)





typedef struct _MMDevIOGET_FLASH_PROGRESS    
{
    MMDevIOHeader    Header;
    ULONG            CurrentPage;        
    ULONG            TotalPages;        
                                    
} MMDevIOGGET_FLASH_PROGRESS, *PMMDevIOGGET_FLASH_PROGRESS;

#define SIZEOF_MMDevIOGET_FLASH_PROGRESS sizeof(MMDevIOGGET_FLASH_PROGRESS)

// Recmnd, [POS]
#define APR_TRACKER_POS_DEFAULTS  { \
5,        55,        1200,    1600,    1000,    \
1500,    800,    1000,    180,    170,    \
5,        4,        0,        15,        3,        \
2,        4,        0,        15,        130,    \
80,        -40,    10,        275,    275,    \
270,    265,    260,    255,    250,    \
245,    240,    235,    230,    225,    \
220,    215,    210,    50,        0,        \
10,        750,    13,        500,    500,    \
75,        0,        0}

#if defined (_WINDLL) || defined (_WINDOWS)

#define APR_SET_COUNT 4
#define APR_PARAM_COUNT 48

const int gAprParamSetArray [APR_SET_COUNT] [APR_PARAM_COUNT] =
{
// Updated from Henry on 06-15-2010 
// SISAL, [iTouch]
{1,        67,        1000,    1000,    1000,
1000,    1000,    1500,    180,    170,
6,        6,        0,        15,        3,
2,        4,        0,        15,        130,
80,        -40,    10,        275,    275,
270,    265,    260,    255,    250,
245,    240,    235,    230,    225,
220,    215,    210,    50,        1,
10,        2000,    13,        2000,    3000,
75,        0,        0}, 

// C&B, [Best Signature] 
{4,        40,        500,    500,    1000,
1500,    400,    600,    165,    160,
1,        4,        0,        15,        3,
2,        4,        0,        15,        130,
80,        -40,    10,        275,    275,
270,    265,    260,    255,    250,
245,    240,    235,    230,    225,
220,    215,    210,    50,        0,
10,        500,    13,        500,    500,
75,        0,        0}, 

// ORIG, [General]
{3,        70,        2000,    3000,    3000,
4000,    600,    800,    180,    170,
4,        6,        0,        15,        3,
2,        4,        0,        15,        130,
80,        -40,    10,        275,    275,
270,    265,    260,    255,    250,
245,    240,    235,    230,    225,
220,    215,    210,    50,        0,
10,        500,    13,        500,    500,
75,        0,        0},

    // Recmnd, [POS]
    APR_TRACKER_POS_DEFAULTS,
};

#endif


#define MAX_TRACKER_PARAMS 128

typedef struct _MMDevIOTRACKER_PARAMS    
{
    MMDevIOHeader    Header;
    int            TrackerParamsUsed;
    int            TrackerParams[MAX_TRACKER_PARAMS];
    
} MMDevIOTRACKER_PARAMS, *PMMDevIOTRACKER_PARAMS;

#define SIZEOF_MMDevIOTRACKER_PARAMS sizeof(MMDevIOTRACKER_PARAMS)

typedef struct _MMDevIOBeep_Event    
{
    MMDevIOHeader       Header;
    //wchar_t       wsEventName[256];    
    unsigned __int64    Handle ;
} MMDevIOBeep_Event, *PMMDevIOBeep_Event;

#define SIZEOF_MMDevIOBeep_Event sizeof(MMDevIOBeep_Event)


#define MAX_BACKGAMMON_BUTTONS 4

typedef struct _MMDevIOBackgammonButtons_Event    
{
    MMDevIOHeader        Header;
    unsigned __int64    Handle[MAX_BACKGAMMON_BUTTONS] ;    
} MMDevIOBackgammonButtons_Event, *PMMDevIOBackgammonButtons_Event;

#define SIZEOF_MMDevIOBackgammonButtons_Event sizeof(MMDevIOBackgammonButtons_Event)



#define SWITCH_TO_FUSB 1



typedef struct _MMDevIOFwControl    
{
    MMDevIOHeader        Header;
    ULONG            Function;        
} MMDevIOFwControl, *PMMDevIOFwControl;

#define SIZEOF_MMDevIOFwControl sizeof(MMDevIOFwControl)


typedef enum _MM_USB_SMARTSET_COMMAND    
{
    MMUSB_GET_SMARTSET_SEND_STATUS,        
    MMUSB_GET_SMARTSET_RESPONSE_STATUS,    
    MMUSB_FLUSH_SMARTSET_RESPONSES,        
    MMUSB_SET_SMARTSET_TOUCH_FILTER,        
    MMUSB_GET_SMARTSET_TOUCH_FILTER,        
    MMUSB_SEND_SMARTSET_COMMAND,            
    MMUSB_GET_SMARTSET_RESPONSE,    
    MMUSB_SMARTSET_ACQUIRE_LOCK,
    MMUSB_SMARTSET_RELEASE_LOCK
}MM_USB_SMARTSET_COMMAND ;

#define SMARTSET_BUFFER_LENGTH 8
typedef struct _MMDevIOSmartSet    
{
    MMDevIOHeader    Header;
    UCHAR            UsbVendorCommand;
    UCHAR            Buffer[SMARTSET_BUFFER_LENGTH];
    LONG            Token;
} MMDevIOSmartSet, *PMMDevIOSmartSet;


#define SIZEOF_MMDevIOSmartSet sizeof(MMDevIOSmartSet)


typedef struct _MMDevIODeviceContext
{
    MMDevIOHeader    Header;
    unsigned __int64    DeviceContext;

} MMDevIODeviceContext, *PMMDevIODeviceContext;


#define SIZEOF_MMDevIODeviceContext sizeof(MMDevIODeviceContext)










typedef struct _MMDevIOLiftThreshold    
{
    MMDevIOHeader        Header;
    ULONG                LiftThreshold;        
} MMDevIOLiftThreshold, *PMMDevIOLiftThreshold;

#define SIZEOF_MMDevIOLiftThreshold sizeof(MMDevIOLiftThreshold)




////

typedef struct _MMDevIOFsrState    
{
    MMDevIOHeader        Header;
    unsigned int FSR_enabled;
    unsigned char FSR_Supported;
    unsigned char FSR_val; //
    int FSR_baseline; //
    int FSR_average; //
    int FSR_long_average; //
    int FSR_state;  //
    unsigned char FSR_calibrated; //
    int FSR_waitForRelease; //

    int FSR_lastCalibrateTime;
    int FSR_liftThreshold;
    int FSR_resistance;
    int FSR_sensitivity;
    int FSR_gearCode;

} MMDevIOFsrState, *PMMDevIOFsrState;

#define SIZEOF_MMDevIOFsrState sizeof(MMDevIOFsrState)

typedef struct _MMDevIODriverDefault    
{
    MMDevIOHeader        Header;
    LONG                ApplyDefault; // TRUE when apply default to a device, FALSE to save device params as default     
} MMDevIODriverDefault, *PMMDevIODriverDefault;

#define SIZEOF_MMDevIODriverDefault sizeof(MMDevIODriverDefault)

typedef struct _MMDevIORegisterAsync_Event    
{
    MMDevIOHeader        Header;
    unsigned __int64    Handle ;    
} MMDevIORegisterAsync_Event, *PMMDevIORegisterAsync_Event;

#define SIZEOF_MMDevIORegisterAsync_Event sizeof(MMDevIORegisterAsync_Event)

#define ASYN_REASON_APR_LOW_SCORE    1
#define ASYN_REASON_APR_OVERCOUNT    2


typedef struct _MMDevIOQueryAsync_Event    
{
    MMDevIOHeader        Header;
    LONG                AsyncReasonsFlag ;    
} MMDevIOQueryAsync_Event, *PMMDevIOQueryAsync_Event;

#define SIZEOF_MMDevIOQueryAsync_Event sizeof(MMDevIOQueryAsync_Event)

#define PLAY_TOUCH_DISABLE    0
#define PLAY_TOUCH_ENABLE    1
#define PLAY_TOUCH_PLAY        2
#define PLAY_TOUCH_QUERY    3
#define PLAY_TOUCH_SET        4
#define PLAY_TOUCH_LOAD        5
#define PLAY_TOUCH_SAVE        6
#define PLAY_TOUCH_LOG_BEGIN        7
#define PLAY_TOUCH_LOG_END        8

#define PLAY_TOUCH_MAX_FILENAME 256

typedef struct _MMDevIOPlayTouch
{
    MMDevIOHeader        Header;
    ULONG                OpsCode;
    ULONG                Start;    
    ULONG                End;
    WCHAR                Filename[PLAY_TOUCH_MAX_FILENAME];
} MMDevIOPlayTouch, *PMMDevIOPlayTouch;

#define SIZEOF_MMDevIOPlayTouch sizeof(MMDevIOPlayTouch)


typedef struct _MMDevIOAutosize
{
	MMDevIOHeader Header;
	// ToDriver
	BOOL          bDoAutoSizing;
	BOOL          bLockAutoSizing; // if bDoAutoSizing is true, at end of auto
                                   // sizing status will be set to bLockAutoSizing
	// FromDriver
	BOOL          bIsLocked;
	USHORT        CycleCount;    
	USHORT        DurationX;
	USHORT        DurationY;
} MMDevIOAutoSize;

#define SIZEOF_MMDevIOAutoSize sizeof(MMDevIOAutoSize)

#define PHYS_SIZE_TAG    'syhP'
#define DURATION_TAG    'nruD'

#define Nvram_Header_Offset            0
#define Nvram_CheckSum_Offset        32
#define Nvram_PhysicalSize_Offset    36
#define Nvram_Duration_Offset        44
#define Nvram_Data_Offset            64


typedef struct _SAWJR_FW_PARAMS
{
    ULONG            XPhysicalSize;    
    ULONG            XDuration;    
    ULONG            YPhysicalSize;    
    ULONG            YDuration;    
} SAWJR_FW_PARAMS, *PSAWJR_FW_PARAMS;

// -----------------------------------------------------------------------------
//    MM_DRIVER_BEEP
//
typedef struct _MMDevIODRIVER_BEEP
{
    MMDevIOHeader   Header;
    ULONG           Duration;
    ULONG           Frequency;
} MMDevIODRIVER_BEEP, *PMMDevIODRIVER_BEEP;
#define SIZEOF_MMDevIODRIVER_BEEP sizeof(MMDevIODRIVER_BEEP)

#define SCREEN_MAPPING_MAX_PATH 256
// -----------------------------------------------------------------------------
//    MM_MAP_SCREEN
//
typedef struct _MMDevIOMapScreen
{
    MMDevIOHeader   Header;
    WCHAR           DeviceId[SCREEN_MAPPING_MAX_PATH];
    WCHAR           MonitorId[SCREEN_MAPPING_MAX_PATH];
} MMDevIOMapScreen, *PMMDevIOMapScreen;

#define SIZEOF_MMDevIOMapScreen sizeof(MMDevIOMapScreen)

// -----------------------------------------------------------------------------
//    MM_MAX_TOUCH
//
typedef struct _MMDevIOMAX_TOUCH
{
    MMDevIOHeader    Header;
    ULONG            MaxTouch;

} MMDevIOMAX_TOUCH, *PMMDevIOMAX_TOUCH;

#define SIZEOF_MMDevIOMAX_TOUCH sizeof(MMDevIOMAX_TOUCH)

// Driver Only Defines common between drivers.
#if 1

typedef enum _GETPOINTS_STATE
{
    GETPOINTS_STATE_IDLE =0,
    GETPOINTS_STATE_PENDING,
    GETPOINTS_STATE_CANCEL_PENDING,
    GETPOINTS_STATE_CLEAR_PENDING
} GETPOINTS_STATE;

#ifdef _ELO_KERNEL_MODE_DRIVER

typedef 
VOID
(*PELOUSB_PRIVATE_INTERFACE_HANDLER)
(

    IN PDEVICE_OBJECT    DeviceObject,        // Ptr to the device object
    IN PPRIVATE_INTERFACE_DESCRIPTOR        PrivateInterface        // Ptr to the 

 );


typedef struct _ELOUSB_PRIVATE_INTERFACE_EXPORT
{
    PELOUSB_PRIVATE_INTERFACE_HANDLER Handler;    
    PDEVICE_OBJECT    DeviceObject;
} ELOUSB_PRIVATE_INTERFACE_EXPORT, *PELOUSB_PRIVATE_INTERFACE_EXPORT;
#endif
#endif
#pragma pack( pop, enter_MMIOCTL )

#endif

// -------------------------  End of Mmioctl.h  --------------------------------


