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
// INvTranscodeFilterGUIDs.h
//
// NVIDIA Video Transcoder DirectShow filter GUIDs
//
//---------------------------------------------------------------------------

#ifndef __INVTRANSCODEFILTERGUIDS_H__
#define __INVTRANSCODEFILTERGUIDS_H__

#ifdef __cplusplus
extern "C" {
#endif

// {B63E31D0-87B5-477f-B224-4A35B6BECED6} 'Dshow Nvidia Video Encoder Filter'
DEFINE_GUID(CLSID_NVIDIA_VideoEncoderFilter, 
0xb63e31d0, 0x87b5, 0x477f, 0xb2, 0x24, 0x4a, 0x35, 0xb6, 0xbe, 0xce, 0xd6);

#ifdef __cplusplus
}
#endif

#endif // ndef __INVTRANSCODEFILTERGUIDS_H__



