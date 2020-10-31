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



#ifndef NVENCODERAPI_H
#define NVENCODERAPI_H


#include "NVEncodeDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define NVENCAPI    __stdcall
#else
#define NVENCAPI
#endif


typedef unsigned char *(NVENCAPI *PFNACQUIREBITSTREAM)(int *pBufferSize, void *pUserdata);
typedef void (NVENCAPI *PFNRELEASEBITSTREAM)(int nBytesInBuffer, unsigned char *cb, void *pUserdata);
typedef void (NVENCAPI *PFNONBEGINFRAME)(const NVVE_BeginFrameInfo *pbfi, void *pUserdata);
typedef void (NVENCAPI *PFNONENDFRAME)(const NVVE_EndFrameInfo *pefi, void *pUserdata);

typedef struct _NVVE_CallbackParams
{
    PFNACQUIREBITSTREAM pfnacquirebitstream;
    PFNRELEASEBITSTREAM pfnreleasebitstream;
    PFNONBEGINFRAME pfnonbeginframe;
    PFNONENDFRAME pfnonendframe;
} NVVE_CallbackParams;



typedef void *NVEncoder;

int NVENCAPI NVCreateEncoder(NVEncoder *pNVEncoder);
int NVENCAPI NVDestroyEncoder(NVEncoder hNVEncoder);
int NVENCAPI NVIsSupportedCodec(NVEncoder hNVEncoder, unsigned long dwCodecType);
int NVENCAPI NVIsSupportedCodecProfile(NVEncoder hNVEncoder, unsigned long dwCodecType, unsigned long dwProfileType);
int NVENCAPI NVSetCodec(NVEncoder hNVEncoder, unsigned long dwCodecType);
int NVENCAPI NVGetCodec(NVEncoder hNVEncoder, unsigned long *pdwCodecType);
int NVENCAPI NVIsSupportedParam(NVEncoder hNVEncoder, unsigned long dwParamType);
int NVENCAPI NVSetParamValue(NVEncoder hNVEncoder, unsigned long dwParamType, void *pData);
int NVENCAPI NVGetParamValue(NVEncoder hNVEncoder, unsigned long dwParamType, void *pData);
int NVENCAPI NVSetDefaultParam(NVEncoder hNVEncoder);
int NVENCAPI NVCreateHWEncoder(NVEncoder hNVEncoder);
int NVENCAPI NVGetSPSPPS(NVEncoder hNVEncoder, unsigned char *pSPSPPSbfr, int nSizeSPSPPSbfr, int *pDatasize);
int NVENCAPI NVEncodeFrame(NVEncoder hNVEncoder, NVVE_EncodeFrameParams *pFrmIn, unsigned long flag, void *pData);
int NVENCAPI NVGetHWEncodeCaps(void);
void NVENCAPI NVRegisterCB(NVEncoder hNVEncoder, NVVE_CallbackParams cb, void *pUserdata);

#ifdef __cplusplus
}
#endif

#endif
