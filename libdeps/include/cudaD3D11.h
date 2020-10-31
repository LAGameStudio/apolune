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

#ifndef CUDAD3D11_H
#define CUDAD3D11_H

/**
 * CUDA API versioning support
 */
#if defined(CUDA_FORCE_API_VERSION)
    #if (CUDA_FORCE_API_VERSION == 3010)
        #define __CUDA_API_VERSION 3010
    #else
        #error "Unsupported value of CUDA_FORCE_API_VERSION"
    #endif
#else
    #define __CUDA_API_VERSION 3020
#endif /* CUDA_FORCE_API_VERSION */

#if defined(__CUDA_API_VERSION_INTERNAL) || __CUDA_API_VERSION >= 3020
    #define cuD3D11CtxCreate cuD3D11CtxCreate_v2
#endif /* __CUDA_API_VERSION_INTERNAL || __CUDA_API_VERSION >= 3020 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup CUDA_D3D11 Direct3D 11 Interoperability
 * \ingroup CUDA_DRIVER
 *
 * This section describes the Direct3D 11 interoperability functions of the
 * low-level CUDA driver application programming interface.
 *
 * @{
 */

/**
 * CUDA devices corresponding to a D3D11 device
 */
typedef enum CUd3d11DeviceList_enum {
    CU_D3D11_DEVICE_LIST_ALL            = 0x01, /**< The CUDA devices for all GPUs used by a D3D11 device */
    CU_D3D11_DEVICE_LIST_CURRENT_FRAME  = 0x02, /**< The CUDA devices for the GPUs used by a D3D11 device in its currently rendering frame */
    CU_D3D11_DEVICE_LIST_NEXT_FRAME     = 0x03, /**< The CUDA devices for the GPUs to be used by a D3D11 device in the next frame */
} CUd3d11DeviceList;

/**
 * \brief Gets the CUDA device corresponding to a display adapter.
 *
 * Returns in \p *pCudaDevice the CUDA-compatible device corresponding to the
 * adapter \p pAdapter obtained from ::IDXGIFactory::EnumAdapters.
 *
 * If no device on \p pAdapter is CUDA-compatible the call will return
 * ::CUDA_ERROR_NO_DEVICE.
 *
 * \param pCudaDevice - Returned CUDA device corresponding to \p pAdapter
 * \param pAdapter    - Adapter to query for CUDA device
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_NO_DEVICE,
 * ::CUDA_ERROR_UNKNOWN
 * \notefnerr
 *
 * \sa
 * ::cuD3D11CtxCreate
 */
CUresult CUDAAPI cuD3D11GetDevice(CUdevice *pCudaDevice, IDXGIAdapter *pAdapter);

/**
 * \brief Gets the CUDA devices corresponding to a Direct3D 11 device
 *
 * Returns in \p *pCudaDeviceCount the number of CUDA-compatible device corresponding
 * to the Direct3D 11 device \p pD3D11Device.
 * Also returns in \p *pCudaDevices at most \p cudaDeviceCount of the the CUDA-compatible devices
 * corresponding to the Direct3D 11 device \p pD3D11Device.
 *
 * If any of the GPUs being used to render \p pDevice are not CUDA capable then the
 * call will return ::CUDA_ERROR_NO_DEVICE.
 *
 * \param pCudaDeviceCount - Returned number of CUDA devices corresponding to \p pD3D11Device
 * \param pCudaDevices     - Returned CUDA devices corresponding to \p pD3D11Device
 * \param cudaDeviceCount  - The size of the output device array \p pCudaDevices
 * \param pD3D11Device     - Direct3D 11 device to query for CUDA devices
 * \param deviceList       - The set of devices to return.  This set may be
 *                           ::CU_D3D11_DEVICE_LIST_ALL for all devices,
 *                           ::CU_D3D11_DEVICE_LIST_CURRENT_FRAME for the devices used to
 *                           render the current frame (in SLI), or
 *                           ::CU_D3D11_DEVICE_LIST_NEXT_FRAME for the devices used to
 *                           render the next frame (in SLI).
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_NO_DEVICE,
 * ::CUDA_ERROR_UNKNOWN
 * \notefnerr
 *
 * \sa
 * ::cuD3D11CtxCreate
 */
CUresult CUDAAPI cuD3D11GetDevices(unsigned int *pCudaDeviceCount, CUdevice *pCudaDevices, unsigned int cudaDeviceCount, ID3D11Device *pD3D11Device, CUd3d11DeviceList deviceList);

#if __CUDA_API_VERSION >= 3020
/**
 * \brief Create a CUDA context for interoperability with Direct3D 11
 *
 * Creates a new CUDA context, enables interoperability for that context with
 * the Direct3D device \p pD3DDevice, and associates the created CUDA context
 * with the calling thread.
 * The created ::CUcontext will be returned in \p *pCtx.
 * Direct3D resources from this device may be registered and mapped through the
 * lifetime of this CUDA context.
 * If \p pCudaDevice is non-NULL then the ::CUdevice on which this CUDA context was
 * created will be returned in \p *pCudaDevice.
 *
 * On success, this call will increase the internal reference count on
 * \p pD3DDevice. This reference count will be decremented upon destruction of
 * this context through ::cuCtxDestroy().
 * This context will cease to function if \p pD3DDevice is destroyed or encounters
 * an error.
 *
 * \param pCtx        - Returned newly created CUDA context
 * \param pCudaDevice - Returned pointer to the device on which the context was created
 * \param Flags       - Context creation flags (see ::cuCtxCreate() for details)
 * \param pD3DDevice  - Direct3D device to create interoperability context with
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_INVALID_VALUE,
 * ::CUDA_ERROR_OUT_OF_MEMORY,
 * ::CUDA_ERROR_UNKNOWN
 * \notefnerr
 *
 * \sa
 * ::cuD3D11GetDevice,
 * ::cuGraphicsD3D11RegisterResource
 */
CUresult CUDAAPI cuD3D11CtxCreate(CUcontext *pCtx, CUdevice *pCudaDevice, unsigned int Flags, ID3D11Device *pD3DDevice);

/**
 * \brief Create a CUDA context for interoperability with Direct3D 11
 *
 * Creates a new CUDA context, enables interoperability for that context with
 * the Direct3D device \p pD3DDevice, and associates the created CUDA context
 * with the calling thread.
 * The created ::CUcontext will be returned in \p *pCtx.
 * Direct3D resources from this device may be registered and mapped through the
 * lifetime of this CUDA context.
 *
 * On success, this call will increase the internal reference count on
 * \p pD3DDevice. This reference count will be decremented upon destruction of
 * this context through ::cuCtxDestroy().
 * This context will cease to function if \p pD3DDevice is destroyed or encounters
 * an error.
 *
 * \param pCtx        - Returned newly created CUDA context
 * \param flags       - Context creation flags (see ::cuCtxCreate() for details)
 * \param pD3DDevice  - Direct3D device to create interoperability context with
 * \param cudaDevice  - The CUDA device on which to create the context.  This device
 *                      must be among the devices returned when querying
 *                      ::CU_D3D11_DEVICES_ALL from  ::cuD3D11GetDevices.
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_INVALID_VALUE,
 * ::CUDA_ERROR_OUT_OF_MEMORY,
 * ::CUDA_ERROR_UNKNOWN
 * \notefnerr
 *
 * \sa
 * ::cuD3D11GetDevices,
 * ::cuGraphicsD3D11RegisterResource
 */

CUresult CUDAAPI cuD3D11CtxCreateOnDevice(CUcontext *pCtx, unsigned int flags, ID3D11Device *pD3DDevice, CUdevice cudaDevice);

/**
 * \brief Get the Direct3D 11 device against which the current CUDA context was
 * created
 *
 * Returns in \p *ppD3DDevice the Direct3D device against which this CUDA context
 * was created in ::cuD3D11CtxCreate().
 *
 * \param ppD3DDevice - Returned Direct3D device corresponding to CUDA context
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_INVALID_CONTEXT
 * \notefnerr
 *
 * \sa
 * ::cuD3D11GetDevice
 */
CUresult CUDAAPI cuD3D11GetDirect3DDevice(ID3D11Device **ppD3DDevice);

#endif /* __CUDA_API_VERSION >= 3020 */

/**
 * \brief Register a Direct3D 11 resource for access by CUDA
 *
 * Registers the Direct3D 11 resource \p pD3DResource for access by CUDA and
 * returns a CUDA handle to \p pD3Dresource in \p pCudaResource.
 * The handle returned in \p pCudaResource may be used to map and unmap this
 * resource until it is unregistered.
 * On success this call will increase the internal reference count on
 * \p pD3DResource. This reference count will be decremented when this
 * resource is unregistered through ::cuGraphicsUnregisterResource().
 *
 * This call is potentially high-overhead and should not be called every frame
 * in interactive applications.
 *
 * The type of \p pD3DResource must be one of the following.
 * - ::ID3D11Buffer: may be accessed through a device pointer.
 * - ::ID3D11Texture1D: individual subresources of the texture may be accessed via arrays
 * - ::ID3D11Texture2D: individual subresources of the texture may be accessed via arrays
 * - ::ID3D11Texture3D: individual subresources of the texture may be accessed via arrays
 *
 * The \p Flags argument may be used to specify additional parameters at register
 * time.  The only valid value for this parameter is
 * - ::CU_GRAPHICS_REGISTER_FLAGS_NONE
 *
 * Not all Direct3D resources of the above types may be used for
 * interoperability with CUDA.  The following are some limitations.
 * - The primary rendertarget may not be registered with CUDA.
 * - Resources allocated as shared may not be registered with CUDA.
 * - Textures which are not of a format which is 1, 2, or 4 channels of 8, 16,
 *   or 32-bit integer or floating-point data cannot be shared.
 * - Surfaces of depth or stencil formats cannot be shared.
 *
 * If Direct3D interoperability is not initialized for this context using
 * ::cuD3D11CtxCreate then ::CUDA_ERROR_INVALID_CONTEXT is returned.
 * If \p pD3DResource is of incorrect type or is already registered then
 * ::CUDA_ERROR_INVALID_HANDLE is returned.
 * If \p pD3DResource cannot be registered then ::CUDA_ERROR_UNKNOWN is returned.
 * If \p Flags is not one of the above specified value then ::CUDA_ERROR_INVALID_VALUE
 * is returned.
 *
 * \param pCudaResource - Returned graphics resource handle
 * \param pD3DResource  - Direct3D resource to register
 * \param Flags         - Parameters for resource registration
 *
 * \return
 * ::CUDA_SUCCESS,
 * ::CUDA_ERROR_DEINITIALIZED,
 * ::CUDA_ERROR_NOT_INITIALIZED,
 * ::CUDA_ERROR_INVALID_CONTEXT,
 * ::CUDA_ERROR_INVALID_VALUE,
 * ::CUDA_ERROR_INVALID_HANDLE,
 * ::CUDA_ERROR_OUT_OF_MEMORY,
 * ::CUDA_ERROR_UNKNOWN
 * \notefnerr
 *
 * \sa
 * ::cuD3D11CtxCreate,
 * ::cuGraphicsUnregisterResource,
 * ::cuGraphicsMapResources,
 * ::cuGraphicsSubResourceGetMappedArray,
 * ::cuGraphicsResourceGetMappedPointer
 */
CUresult CUDAAPI cuGraphicsD3D11RegisterResource(CUgraphicsResource *pCudaResource, ID3D11Resource *pD3DResource, unsigned int Flags);

/** @} */ /* END CUDA_D3D11 */

/**
 * CUDA API versioning support
 */
#if defined(__CUDA_API_VERSION_INTERNAL)
    #undef cuD3D11CtxCreate
#endif /* __CUDA_API_VERSION_INTERNAL */

/**
 * CUDA API made obselete at API version 3020
 */
#if defined(__CUDA_API_VERSION_INTERNAL) || __CUDA_API_VERSION < 3020
CUresult CUDAAPI cuD3D11CtxCreate(CUcontext *pCtx, CUdevice *pCudaDevice, unsigned int Flags, ID3D11Device *pD3DDevice);
#endif /* __CUDA_API_VERSION_INTERNAL || __CUDA_API_VERSION < 3020 */

#ifdef __cplusplus
};
#endif

#undef __CUDA_API_VERSION

#endif

