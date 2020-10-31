// Image Header
#ifndef _H_FRAMEBUFFER_
#define _H_FRAMEBUFFER_

// Common includes
#include "Common.h"

#define AGK_FBO_BOUND			0x01
#define AGK_FBO_SHARED_COLOR	0x02
#define AGK_FBO_SHARED_DEPTH	0x04
#define AGK_FBO_FORCE_DEPTH		0x08

// Namespace
namespace AGK
{
	class cImage;

	class FrameBuffer
	{
		public:
			FrameBuffer( cImage *pColor, bool bCShared, cImage* pDepth, bool bDShared, bool forceDepth );
			~FrameBuffer();

			static FrameBuffer* g_pBoundFBO;
			static FrameBuffer* g_pAllFrameBuffers;
			static void DeleteImage( cImage *pImage );
			static FrameBuffer* FindFrameBuffer( cImage *pColor, cImage* pDepth, bool forceDepth );

			bool IsBound() { return (m_bFlags & AGK_FBO_BOUND) != 0; }
			void Bind();
			void GenerateMipmaps();

			FrameBuffer *m_pNextFBO;

		protected:
			cImage *m_pColor;
			cImage *m_pDepth;
			int m_bFlags;

			UINT m_iFBO;
			UINT m_iRBODepth;			

			void PlatformCreateFrameBuffer( cImage *pColor, cImage *pDepth, bool forceDepth );
			void PlatformDeleteFrameBuffer();
			void PlatformBind();
		
	};
}

#endif