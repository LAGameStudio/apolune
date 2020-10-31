// Touch Header
#ifndef _H_TOUCH_
#define _H_TOUCH_

// Common includes
#include "Common.h"

// Maximum AGK allows
#define MAX_AGK_TOUCH_COUNT 10

// Namespace
namespace AGK
{
	class cTouch
	{
		protected:
			static float g_fX;
			static float g_fY;
			
			static int g_iIterIndex;
			static bool g_bIncludeUnknown;
			static cTouch g_pTouchEvents[ MAX_AGK_TOUCH_COUNT ];

		public:
			enum eTouchType
			{
				eUnknown = 0,
				eShort = 1,
				eHold = 2,
				eDrag = 3
			};

		protected:
			bool m_bActive;
			UINT m_iUniqueID;
			float m_fStartTime;
			float m_fTotalTime;
			float m_fStartX;
			float m_fStartY;
			float m_fCurrentX;
			float m_fCurrentY;
			float m_fLastX;
			float m_fLastY;
			eTouchType m_eType;
			bool m_bReleased;
			bool m_bDelete;
			bool m_bRead;
			int m_iData;
			bool m_bCaptured;

		public:
			cTouch() { Reset(); }
			~cTouch() {}
			
			void Reset();
			void Moved( float x, float y );
			void Pressed( float x, float y );
			void Released( float x, float y );
			void SetData( int data );
			void CaptureTouch( ) { m_bCaptured = true; }
			void ReleaseTouch( ) { m_bCaptured = false; }
			bool IsCaptured() { return m_bCaptured; }

			UINT GetID() { return m_iUniqueID; }
			float GetStartX() { return m_fStartX; }
			float GetStartY() { return m_fStartY; }
			float GetCurrentX() { return m_fCurrentX; }
			float GetCurrentY() { return m_fCurrentY; }
			float GetLastX() { return m_fLastX; }
			float GetLastY() { return m_fLastY; }
			eTouchType GetType() { return m_eType; }
			bool GetReleased() { return m_bReleased; }
			float GetTotalTime() { return m_fTotalTime; }
			int GetData();

			// static functions
			static float GetAnyX( );
			static float GetAnyY( );
			static bool GetAny( );
			static UINT GetCount( bool bIncludeUnknown=false );
			static cTouch* GetFirstEvent( bool bIncludeUnknown=false );
			static cTouch* GetNextEvent( );

			static void UpdateEvents();
			static cTouch* NewPoint( UINT iNewID );
			static cTouch* GetEvent( UINT iIndex );
            static void ClearAll();
	};
}

#endif