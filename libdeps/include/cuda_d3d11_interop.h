/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:   
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and 
 * international Copyright laws.  Users and possessors of this source code 
 * are hereby granted a nonexclusive, royalty-free license to use this code 
 * in individual and commercial software.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE 
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR 
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH 
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE 
 * OR PERFORMANCE OF THIS SOURCE CODE.  
 *
 * U.S. Government End Users.   This source code is a "commercial item" as 
 * that term is defined at  48 C.F.R. 2.101 (OCT 1995), consisting  of 
 * "commercial computer  software"  and "commercial computer software 
 * documentation" as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995) 
 * and is provided to the U.S. Government only as a commercial end item.  
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the 
 * source code with only those rights set forth herein. 
 *
 * Any use of this source code in individual and commercial software must 
 * include, in the user documentation and internal comments to the code,
 * the above Disclaimer and U.S. Government End Users Notice.
 */

#if !defined(__CUDA_D3D11_INTEROP_H__)
#define __CUDA_D3D11_INTEROP_H__

/** \cond impl_private */
#if !defined(__dv)

#if defined(__cplusplus)

#define __dv(v) \
        = v

#else /* __cplusplus */

#define __dv(v)

#endif /* __cplusplus */

#endif /* !__dv */
/** \endcond impl_private */

#if defined(_WIN32)

#include "builtin_types.h"
#include "host_defines.h"
#include <d3d11.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup CUDART_D3D11 Direct3D 11 Interoperability
 * This section describes the Direct3D 11 interoperability functions of the CUDA
 * runtime application programming interface.
 *
 * @{
 */

/**
 * CUDA devices corresponding to a D3D11 device
 */
/*DEVICE_BUILTIN*/
enum cudaD3D11DeviceList
{
  cudaD3D11DeviceListAll           = 1, /**< The CUDA devices for all GPUs used by a D3D11 device */
  cudaD3D11DeviceListCurrentFrame  = 2, /**< The CUDA devices for the GPUs used by a D3D11 device in its currently rendering frame */
  cudaD3D11DeviceListNextFrame     = 3, /**< The CUDA devices for the GPUs to be used by a D3D11 device in the next frame  */
};

/**
 * \brief Gets the Direct3D device against which the current CUDA context was
 * created
 *
 * Returns in \p *ppD3D11Device the Direct3D device against which this CUDA
 * context was created in ::cudaD3D11SetDirect3DDevice().
 *
 * \param ppD3D11Device - Returns the Direct3D device for this thread
 *
 * \return
 * ::cudaSuccess,
 * ::cudaErrorUnknown
 * \notefnerr
 *
 * \sa 
 * ::cudaD3D11SetDirect3DDevice
 */
extern __host__ cudaError_t CUDARTAPI cudaD3D11GetDirect3DDevice(ID3D11Device **ppD3D11Device);

/**
 * \brief Register a Direct3D 11 resource for access by CUDA
 * 
 * Registers the Direct3D 11 resource \p pD3DResource for access by CUDA.  
 *
 * If this call is successful, then the application will be able to map and
 * unmap this resource until it is unregistered through
 * ::cudaGraphicsUnregisterResource(). Also on success, this call will increase the
 * internal reference count on \p pD3DResource. This reference count will be
 * decremented when this resource is unregistered through
 * ::cudaGraphicsUnregisterResource().
 *
 * This call is potentially high-overhead and should not be called every frame
 * in interactive applications.
 *
 * The type of \p pD3DResource must be one of the following.
 *
 * - ::ID3D11Buffer: may be accessed via a device pointer
 * - ::ID3D11Texture1D: individual subresources of the texture may be accessed via arrays
 * - ::ID3D11Texture2D: individual subresources of the texture may be accessed via arrays
 * - ::ID3D11Texture3D: individual subresources of the texture may be accessed via arrays
 *
 * The \p flags argument may be used to specify additional parameters at register
 * time.  The only valid value for this parameter is 
 *
 * - ::cudaGraphicsRegisterFlagsNone
 *
 * Not all Direct3D resources of the above types may be used for
 * interoperability with CUDA.  The following are some limitations.
 *
 * - The primary rendertarget may not be registered with CUDA.
 * - Resources allocated as shared may not be registered with CUDA.
 * - Textures which are not of a format which is 1, 2, or 4 channels of 8, 16,
 *   or 32-bit integer or floating-point data cannot be shared.
 * - Surfaces of depth or stencil formats cannot be shared.
 *
 * If Direct3D interoperability is not initialized using ::cudaD3D11SetDirect3DDevice then
 * ::cudaErrorInvalidDevice is returned. 
 * If \p pD3DResource is of incorrect type or is already registered, then 
 * ::cudaErrorInvalidResourceHandle is returned. 
 * If \p pD3DResource cannot be registered, then ::cudaErrorUnknown is returned.
 *
 * \param resource - Pointer to returned resource handle
 * \param pD3DResource - Direct3D resource to register
 * \param flags        - Parameters for resource registration
 * 
 * \return
 * ::cudaSuccess,
 * ::cudaErrorInvalidDevice,
 * ::cudaErrorInvalidValue,
 * ::cudaErrorInvalidResourceHandle,
 * ::cudaErrorUnknown
 * \notefnerr
 *
 * \sa 
 * ::cudaD3D11SetDirect3DDevice
 * ::cudaGraphicsUnregisterResource,
 * ::cudaGraphicsMapResources, 
 * ::cudaGraphicsSubResourceGetMappedArray, 
 * ::cudaGraphicsResourceGetMappedPointer 
 */
extern __host__ cudaError_t CUDARTAPI cudaGraphicsD3D11RegisterResource(struct cudaGraphicsResource **resource, ID3D11Resource *pD3DResource, unsigned int flags);

/**
 * \brief Sets the Direct3D 11 device to use for interoperability in this thread
 *
 * Records \p pD3D11Device as the Direct3D 11 device to use for Direct3D 11
 * interoperability on this host thread.
 * If the host thread has already initialized the CUDA runtime by
 * calling non-device management runtime functions or if there exists a CUDA 
 * driver context active on the host thread, then this call returns
 * ::cudaErrorSetOnActiveProcess.
 *
 * Successful context creation on \p pD3D11Device will increase the internal
 * reference count on \p pD3D11Device. This reference count will be decremented
 * upon destruction of this context through ::cudaThreadExit().
 *
 * \param pD3D11Device - Direct3D device to use for interoperability
 * \param device       - The CUDA device to use.  This device must be among the devices
 *                       returned when querying ::cudaD3D11DeviceListAll from ::cudaD3D11GetDevices,
 *                       may be set to -1 to automatically select an appropriate CUDA device.
 *
 * \return
 * ::cudaSuccess,
 * ::cudaErrorInitializationError,
 * ::cudaErrorInvalidValue,
 * ::cudaErrorSetOnActiveProcess
 * \notefnerr
 *
 * \sa 
 * ::cudaD3D11GetDevice,
 * ::cudaGraphicsD3D11RegisterResource
 */
extern __host__ cudaError_t CUDARTAPI cudaD3D11SetDirect3DDevice(ID3D11Device *pD3D11Device, int device __dv(-1));

/**
 * \brief Gets the device number for an adapter
 *
 * Returns in \p *device the CUDA-compatible device corresponding to the
 * adapter \p pAdapter obtained from ::IDXGIFactory::EnumAdapters. This call
 * will succeed only if a device on adapter \p pAdapter is Cuda-compatible.
 *
 * \param device   - Returns the device corresponding to pAdapter
 * \param pAdapter - D3D11 adapter to get device for
 *
 * \return
 * ::cudaSuccess,
 * ::cudaErrorInvalidValue,
 * ::cudaErrorUnknown
 * \notefnerr
 *
 * \sa 
 * ::cudaGraphicsUnregisterResource,
 * ::cudaGraphicsMapResources, 
 * ::cudaGraphicsSubResourceGetMappedArray, 
 * ::cudaGraphicsResourceGetMappedPointer 
 */
extern __host__ cudaError_t CUDARTAPI cudaD3D11GetDevice(int *device, IDXGIAdapter *pAdapter);

/**
 * \brief Gets the CUDA devices corresponding to a Direct3D 11 device
 * 
 * Returns in \p *pCudaDeviceCount the number of CUDA-compatible devices corresponding 
 * to the Direct3D 11 device \p pD3D11Device.
 * Also returns in \p *pCudaDevices at most \p cudaDeviceCount of the the CUDA-compatible devices 
 * corresponding to the Direct3D 11 device \p pD3D11Device.
 *
 * If any of the GPUs being used to render \p pDevice are not CUDA capable then the
 * call will return ::cudaErrorNoDevice.
 *
 * \param pCudaDeviceCount - Returned number of CUDA devices corresponding to \p pD3D11Device
 * \param pCudaDevices     - Returned CUDA devices corresponding to \p pD3D11Device
 * \param cudaDeviceCount  - The size of the output device array \p pCudaDevices
 * \param pD3D11Device     - Direct3D 11 device to query for CUDA devices
 * \param deviceList       - The set of devices to return.  This set may be
 *                           ::cudaD3D11DeviceListAll for all devices, 
 *                           ::cudaD3D11DeviceListCurrentFrame for the devices used to
 *                           render the current frame (in SLI), or
 *                           ::cudaD3D11DeviceListNextFrame for the devices used to
 *                           render the next frame (in SLI).
 *
 * \return
 * ::cudaSuccess,
 * ::cudaErrorNoDevice,
 * ::cudaErrorUnknown
 * \notefnerr
 *
 * \sa 
 * ::cudaGraphicsUnregisterResource,
 * ::cudaGraphicsMapResources, 
 * ::cudaGraphicsSubResourceGetMappedArray, 
 * ::cudaGraphicsResourceGetMappedPointer 
 */
extern __host__ cudaError_t CUDARTAPI cudaD3D11GetDevices(unsigned int *pCudaDeviceCount, int *pCudaDevices, unsigned int cudaDeviceCount, ID3D11Device *pD3D11Device, enum cudaD3D11DeviceList deviceList);

/** @} */ /* END CUDART_D3D11 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _WIN32 */

#undef __dv

#endif /* __CUDA_D3D11_INTEROP_H__ */
