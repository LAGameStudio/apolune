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

//---------------------------------------------------------------------------
// INVVESetting.h
//
// NVIDIA INVVESetting
//
//---------------------------------------------------------------------------

//
// Refer NVVideoEnc_DSFilter.doc
//

#ifndef INVVESETTING_H
#define INVVESETTING_H

#include "NVEncodeDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif



//
// INVVESetting interface IID for getting interface to set/get 
// encoder capability
//
// {4597F768-F60-4E5B-B697-67EB2614DCB5} INVVESetting interface
DEFINE_GUID(IID_INVVESetting, 
            0x4597f768, 0xf60, 0x4e5b, 0xb6, 0x97, 0x67, 0xeb, 0x26, 0x14, 0xdc, 0xb5);

// 
// INVVESetting interface Functions
// Interface used to set/get capability of the video encoder
//
DECLARE_INTERFACE_(INVVESetting, IUnknown)
{
// Function :
//      IsSupportedCodec
// Description:
//      Query if the codec format is supported by the encoder
// Parameter:
//      dwCodecType ( I )
// Return:
//      S_OK                : The format is supported
//      E_NOINTERFACE_FAIL  : The format is not supported
//      E_FAIL              : No CUDA capability present
//
    STDMETHOD(IsSupportedCodec)(THIS_ DWORD dwCodecType) PURE;

// Function :
//      IsSupportedCodecProfile
// Description:
//      Query if the profile for codec format is supported by the encoder
// Parameter:
//      dwCodecType ( I )
//      dwProfileType ( I )
// Return:
//      S_OK                : The profile is supported
//      E_NOINTERFACE_FAIL  : The profile is not supported
//      E_FAIL              : No CUDA capability present
//
    STDMETHOD(IsSupportedCodecProfile)(THIS_ DWORD dwCodecType, DWORD dwProfileType) PURE;

// Function :
//      SetCodecType
// Description:
//      Set encoder codec format
// Parameter:
//      dwCodecType( I )
// Return:
//      S_OK                : Successful
//      E_FAIL              : Fail
//
    STDMETHOD(SetCodecType)(THIS_ DWORD dwCodecType) PURE;

// Function :
//      GetCodecType
// Description:
//      Get the current encoding format
// Parameter:
//      pwdCodecType( O )
// Return:
//      S_OK                : Successful
//      E_FAIL              : The encoding format is not initialized
//      E_POINTER           : pwdCodecType is NULL pointer.
//
    STDMETHOD(GetCodecType)(THIS_ DWORD *pdwCodecType) PURE;

// Function :
//      IsSupportedParam
// Description:
//      Query if the parameter type is supported. This depends on the encoding 
//      format as well as the version of the codec library
// Parameter:
//      dwParamType ( I )
// Return:
//      S_OK                : The parameter is supported
//      E_FAIL              : The parameter is not supported
//
    STDMETHOD(IsSupportedParam)(THIS_ DWORD dwParamType) PURE;

// Function :
//      SetParamValue
// Description:
//      Set the value of the specified parameter type. The pData points to a 
//      memory region storing the value of the parameter. The parameter can be 
//      a data structure, which must match the size of the parameter type
// Parameter:
//      dwParamType( I )
//      pData( I )
// Return:
//      S_OK                : Successful
//      E_FAIL              : Fail to set the value
//      E_NOTIMPL           : Parameter is not adjustable
//      E_UNEXPECTED        : The encoding format is not initialized yet
//      E_POINTER           : pData is NULL pointer
//
    STDMETHOD(SetParamValue)(THIS_ DWORD dwParamType, LPVOID pData) PURE;

// Function :
//      GetParamValue
// Description:
//      Query the current value of the specified parameter type
// Parameter:
//      dwParamType( I )
//      pData( O )
// Return:
//      S_OK                : Successful
//      E_NOTIMPL           : The parameter is not supported
//      E_UNEXPECTED        : The encoding format is not initialized
//      E_POINTER           : pData is NULL pointer
//
    STDMETHOD(GetParamValue)(THIS_ DWORD dwParamType, LPVOID pData) PURE;

// Function :
//      SetDefaultParam
// Description:
//      Applies default settings of the encoding format
// Parameter:
//      None
// Return:
//      S_OK                : Successful
//      E_UNEXPECTED        : The encoding format is not set yet
//
    STDMETHOD(SetDefaultParam)(THIS_ void) PURE;

// Function :
//      GetSPSPPS
// Description:
//      Fetches the buffer containing SPS and PPS
// Parameter:
//      pSPSPPSbfr( O )
//      nSizeSPSPPSbfr ( I )
//      pDatasize( O )
// Return:
//      S_OK                : Successful
//      E_UNEXPECTED        : The encoder is not initialized
//      E_POINTER           : NULL input pointer
//
    STDMETHOD(GetSPSPPS)(THIS_ unsigned char *pSPSPPSbfr, int nSizeSPSPPSbfr, int *pDatasize) PURE;
};

#ifdef __cplusplus
}
#endif

#endif