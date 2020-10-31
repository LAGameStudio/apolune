/***********************************************************************\
 |*                                                                     *| 
 |*    Copyright (c) 1995-2008 by NVIDIA Corp.  All rights reserved.    *| 
 |*                                                                     *| 
 |*  This material  constitutes  the trade  secrets  and confidential,  *| 
 |*  proprietary information of NVIDIA, Corp.  This material is not to  *| 
 |*  be  disclosed,  reproduced,  copied,  or used  in any manner  not  *| 
 |*  permitted  under license from NVIDIA, Corp.                        *| 
 |*                                                                     *| 
 \***********************************************************************/



#ifndef NVENCODEDATATYPES_H
#define NVENCODEDATATYPES_H


#ifdef __cplusplus
extern "C" {
#endif

// Codec Type 
// Used in IsSupportedCodec, IsSupportedCodecProfile, SetCodecType,
// GetCodecType interface functions
#define NV_CODEC_TYPE_MPEG1                                      1//not supported
#define NV_CODEC_TYPE_MPEG2                                      2//not supported
#define NV_CODEC_TYPE_MPEG4                                      3//not supported
#define NV_CODEC_TYPE_H264                                       4
#define NV_CODEC_TYPE_VC1                                        5

// Codec Profile Type 
// Used in IsSupportedCodecProfile interface functions
#define NVVE_MPEG2_PROFILE_MAIN                                  0 //not supported
#define NVVE_H264_PROFILE_BASELINE                               1
#define NVVE_H264_PROFILE_MAIN                                   2
#define NVVE_H264_PROFILE_HIGH                                   3 
#define NVVE_VC1_PROFILE_SIMPLE                                  4
#define NVVE_VC1_PROFILE_MAIN                                    5
#define NVVE_VC1_PROFILE_ADVANCED                                6 //not supported

// Coded picture type
#define NVVE_PIC_TYPE_IFRAME                                     1
#define NVVE_PIC_TYPE_PFRAME                                     2
#define NVVE_PIC_TYPE_BFRAME                                     3

// Encoder error codes
enum NVVE_Result
{
    NVVE_SUCCESS = 0,
    NVVE_OUTOFMEMORY   = 0x8007000EL,
    NVVE_NOTSUPPORTED  = 0x80004002L,
    NVVE_BADPOINTER    = 0x80004003L,
    NVVE_FAILED        = 0x80004005L,
};

// Encoding Parameters
// Used in SetParamValue, GetParamValue interface functions
enum NVVE_EncodeParams
{
    NVVE_OUT_SIZE                           = 1,
    NVVE_ASPECT_RATIO                       = 2,
    NVVE_FIELD_ENC_MODE                     = 3,
    NVVE_P_INTERVAL                         = 4,
    NVVE_IDR_PERIOD                         = 5,
    NVVE_DYNAMIC_GOP                        = 6,
    NVVE_RC_TYPE                            = 7,
    NVVE_AVG_BITRATE                        = 8,
    NVVE_PEAK_BITRATE                       = 9,
    NVVE_QP_LEVEL_INTRA                     = 10,
    NVVE_QP_LEVEL_INTER_P                   = 11,
    NVVE_QP_LEVEL_INTER_B                   = 12,
    NVVE_FRAME_RATE                         = 13,
    NVVE_DEBLOCK_MODE                       = 14,
    NVVE_PROFILE_LEVEL                      = 15,
    NVVE_FORCE_INTRA                        = 16, //DShow only
    NVVE_FORCE_IDR                          = 17, //DShow only
    NVVE_CLEAR_STAT                         = 18, //DShow only
    NVVE_SET_DEINTERLACE                    = 19,
    NVVE_PRESETS                            = 20,
    NVVE_IN_SIZE                            = 21,
    NVVE_STAT_NUM_CODED_FRAMES              = 22, //DShow only
    NVVE_STAT_NUM_RECEIVED_FRAMES           = 23, //DShow only
    NVVE_STAT_BITRATE                       = 24, //DShow only
    NVVE_STAT_NUM_BITS_GENERATED            = 25, //DShow only
    NVVE_GET_PTS_DIFF_TIME                  = 26, //DShow only
    NVVE_GET_PTS_BASE_TIME                  = 27, //DShow only
    NVVE_GET_PTS_CODED_TIME                 = 28, //DShow only
    NVVE_GET_PTS_RECEIVED_TIME              = 29, //DShow only
    NVVE_STAT_ELAPSED_TIME                  = 30, //DShow only
    NVVE_STAT_QBUF_FULLNESS                 = 31, //DShow only
    NVVE_STAT_PERF_FPS                      = 32, //DShow only
    NVVE_STAT_PERF_AVG_TIME                 = 33, //DShow only
    NVVE_DISABLE_CABAC                      = 34,
    NVVE_CONFIGURE_NALU_FRAMING_TYPE        = 35,
    NVVE_DISABLE_SPS_PPS                    = 36,
    NVVE_SLICE_COUNT                        = 37,
    NVVE_GPU_OFFLOAD_LEVEL                  = 38,
    NVVE_GPU_OFFLOAD_LEVEL_MAX              = 39,
    NVVE_MULTI_GPU                          = 40,
    NVVE_GET_GPU_COUNT                      = 41,
    NVVE_GET_GPU_ATTRIBUTES                 = 42,
    NVVE_FORCE_GPU_SELECTION                = 43,
    NVVE_DEVICE_MEMORY_INPUT                = 44,
    NVVE_DEVICE_CTX_LOCK                    = 45,
};



enum NVVE_SurfaceFormat
{
    UYVY=0,
    YUY2,
    YV12,
    NV12,
    IYUV
};


enum NVVE_PicStruct
{
    TOP_FIELD          = 0x01,
    BOTTOM_FIELD       = 0x02,
    FRAME_PICTURE      = 0x03
};

// Rate Control Method
// Used for NVVE_RC_TYPE in SetParamValue, GetParamValue interface
// functions
enum NVVE_RateCtrlType
{
    RC_CQP = 0,
    RC_VBR,
    RC_CBR,
    RC_VBR_MINQP
};

// Frame Rate
// Used for NVVE_FRAME_RATE in SetParamValue, GetParamValue interface- Dshow Filter specific
// functions
enum NVVE_FrameRate
{
    NVVE_FRAME_RATE_12 = 0,
    NVVE_FRAME_RATE_12_5,
    NVVE_FRAME_RATE_14_98,
    NVVE_FRAME_RATE_15,
    NVVE_FRAME_RATE_23_97,
    NVVE_FRAME_RATE_24,
    NVVE_FRAME_RATE_25,
    NVVE_FRAME_RATE_29_97,
    NVVE_FRAME_RATE_30,
    NVVE_FRAME_RATE_50,
    NVVE_FRAME_RATE_59_94,
    NVVE_FRAME_RATE_60,
    NVVE_FRAME_RATE_NUMDEN,    // Frame rate in numerator and denominator format (To specify the frame rates other than the predefined values)
    NVVE_NUM_FRAME_RATES,
    NVVE_FRAME_RATE_UNKNOWN    // Unknown/unspecified frame rate (or variable)
};

// Frame rate descriptor
// Used for NVVE_FRAME_RATE in SetParamValue, GetParamValue
// interface functions
typedef struct _NVVE_FrameRateDescriptor
{
    NVVE_FrameRate eFrameRate;
    int lNumerator;
    int lDenominator;
} NVVE_FrameRateDescriptor;

// Field Encoding mode
// Used for NVVE_FIELD_ENC_MODE in SetParamValue, GetParamValue
// interface functions
enum NVVE_FIELD_MODE
{
    MODE_FRAME = 0,
    MODE_FIELD_TOP_FIRST,
    MODE_FIELD_BOTTOM_FIRST,
    MODE_FIELD_PICAFF,                  //not supported in V1.1
};

// Deinterlacing algorithm
// Used for NVVE_SET_DEINTERLACE in SetParamValue, GetParamValue
// interface functions
enum NVVE_DI_MODE
{
    DI_OFF,
    DI_MEDIAN,
};

// Encoding Presets
// Used for NVVE_PRESETS in SetParamValue, GetParamValue
// interface functions
enum NVVE_PRESETS_TARGET
{
    ENC_PRESET_PSP,             //width 320  height 240
    ENC_PRESET_IPOD,            //width 320  height 240
    ENC_PRESET_AVCHD,           //width 1920 height 1080
    ENC_PRESET_BD,              //width 1920 height 1080
    ENC_PRESET_HDV_1440,        //width 1440 height 1080
    ENC_PRESET_ZUNEHD,          //width 720  height 480
    ENC_PRESET_FLIP_CAM,
};

//Specifies whether Display Aspect Ratio (DAR) or Sample Aspect Ratio (SAR) is to be used
enum NVVE_ASPECT_RATIO_TYPE
{
    ASPECT_RATIO_DAR = 0,
    ASPECT_RATIO_SAR = 1
};

//GPU Offload Level: More value means greater offload, -1 is a default
enum NVVE_GPUOffloadLevel
{
    NVVE_GPU_OFFLOAD_DEFAULT = -1,  // default setting for pel processing 
    NVVE_GPU_OFFLOAD_ESTIMATORS = 8,// pel processing on CPU, Motion Estimation on GPU
    NVVE_GPU_OFFLOAD_ALL      = 16  // pel processing on GPU
};

//Aspect Ratio Parameters
typedef struct _NVVE_AspectRatioParams  // Dshow only
{
    float fAspectRatio;             // set as -1.0f for custom aspect ratio
    int iWidth;                     // parameter valid only for custom aspect ratio
    int iHeight;                    // parameter valid only for custom aspect ratio
    NVVE_ASPECT_RATIO_TYPE eType;   // parameter valid only for custom aspect ratio
}NVVE_AspectRatioParams;

//GPU attributes
typedef struct _NVVE_GPUAttributes
{
    int                     iGpuOrdinal;                // GPU device number
    char                    cName[256];                 // string identifying GPU device
    unsigned int            uiTotalGlobalMem;           // total global memory available on device in bytes
    int                     iMajor;                     // GPU device compute capability major version number
    int                     iMinor;                     // GPU device compute capability minor version number
    int                     iClockRate;                 // GPU clock frequency in kilohertz
    int                     iMultiProcessorCount;       // number of multiprocessors on the GPU device
    NVVE_GPUOffloadLevel    MaxGpuOffloadLevel;         // max offload level supported for this GPU device
} NVVE_GPUAttributes;

//Information passed on to EncodeFrame
typedef struct _NVVE_EncodeFrameParams
{
    int Width;
    int Height;
    int Pitch;
    NVVE_SurfaceFormat SurfFmt;
    NVVE_PicStruct PictureStruc;
    int topfieldfirst;
    int repeatFirstField;
    int progressiveFrame;
    int bLast;
    unsigned char   *picBuf;              // pointer to yuv buffer
}NVVE_EncodeFrameParams;

// Information passed to OnBeginFrame
typedef struct _NVVE_BeginFrameInfo
{
    int nFrameNumber;                   // Frame Number
    int nPicType;                       // Picture Type
}NVVE_BeginFrameInfo;

// Information passed to OnEndFrame
typedef struct _NVVE_EndFrameInfo
{
    int nFrameNumber;                   // Frame Number
    int nPicType;                       // Picture Type
}NVVE_EndFrameInfo;

typedef struct _CUcontextlock_st *CUvideoctxlock;

#ifdef __cplusplus
}
#endif

#endif
