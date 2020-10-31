#ifndef _H_JOYSTICK_
#define _H_JOYSTICK_

#include "Common.h"

namespace AGK
{
	class cSprite;
	class cMouse;
	class cTouch;

	class _EXPORT_ cVirtualButton
	{
		protected:
			friend class agk;

			cText *m_pText;

			bool m_bPrevDown;
			bool m_bDown;

			float m_fVirtualPosX;
			float m_fVirtualPosY;
			float m_fVirtualSize;

			static cSprite *g_pButtonSprite;
			static cSprite *g_pButtonDownSprite;

			cSprite *m_pButtonSprite;
			cSprite *m_pButtonDownSprite;

			unsigned char m_iRed;
			unsigned char m_iGreen;
			unsigned char m_iBlue;
			unsigned char m_iAlpha;
			
			bool m_bActive;
			bool m_bVisible;

			cMouse *m_pCapturedMouse;
			cTouch *m_pCapturedTouch;

			void Zero();

		public:
			cVirtualButton( );
			cVirtualButton( float x, float y, float size );
			~cVirtualButton();

			void SetPosition( float x, float y );
			void SetSize( float size );
			void SetColor( UINT red, UINT green, UINT blue );
			void SetAlpha( UINT alpha );
			void SetActive( bool active );
			void SetVisible( bool visible );
			void SetUpImage( cImage *pImage );
			void SetDownImage( cImage *pImage );
			void SetText( const char *str );

			void CaptureMouse( cMouse *pMouse ) { m_pCapturedMouse = pMouse; }
			void CaptureTouch( cTouch *pTouch ) { m_pCapturedTouch = pTouch; }

			bool GetState() { return m_bDown; }
			bool GetPressed() { return !m_bPrevDown && m_bDown; }
			bool GetReleased() { return m_bPrevDown && !m_bDown; }
			bool GetHitTest( float x, float y );
			
			void Update();
			void Draw();
	};

	class _EXPORT_ cVirtualJoystick
	{
		protected:
			friend class agk;

			float m_fX;
			float m_fY;

			float m_fVirtualPosX;
			float m_fVirtualPosY;
			float m_fVirtualSize;

			static cSprite *g_pOuterSprite;
			static cSprite *g_pInnerSprite;

			cSprite *m_pOuterSprite;
			cSprite *m_pInnerSprite;

			static float g_fDeadZone;

			unsigned char m_iAlpha1;
			unsigned char m_iAlpha2;
			bool m_bActive;
			bool m_bVisible;

			cMouse *m_pCapturedMouse;
			cTouch *m_pCapturedTouch;

			void Zero();

		public:
			cVirtualJoystick( );
			cVirtualJoystick( float x, float y, float size );
			~cVirtualJoystick();

			static void SetDeadZone( float zone ) { g_fDeadZone = zone; }

			void SetPosition( float x, float y );
			void SetSize( float size );
			void SetAlpha( UINT alpha1, UINT alpha2 );
			void SetActive( bool active );
			void SetVisible( bool visible );
			void SetInnerImage( cImage *pImage );
			void SetOuterImage( cImage *pImage );

			void CaptureMouse( cMouse *pMouse ) { m_pCapturedMouse = pMouse; }
			void CaptureTouch( cTouch *pTouch ) { m_pCapturedTouch = pTouch; }

			float GetX();
			float GetY();
			bool GetHitTest( float x, float y );
			
			void Update();
			void Draw();
	};

	class _EXPORT_ cJoystick
	{
		protected:
			friend class agk;

			void *m_pDevice;
			int m_iDeviceType;

			int m_iConnected;

			float m_fX;
			float m_fY;
			float m_fZ;

			float m_fRX;
			float m_fRY;
			float m_fRZ;

			UINT m_iNumButtons;
			char m_iPrevButtons[ 32 ];
			char m_iButtons[ 32 ];
			char m_iResetButtons[ 32 ];

			void PlatformUpdate();
			//void PlatformDelete();

			static float g_fDeadZone;

			void Zero();

		public:
			//cJoystick( float x, float y, float size );
			cJoystick( void *pDevice );
			cJoystick( void *pDevice, int devicetype );
			~cJoystick() { /*PlatformDelete();*/ }

			static void SetDeadZone( float zone ) { g_fDeadZone = zone; }
			static void DetectJoysticks();

			int GetConnected() { return m_iConnected; }

			float GetX();
			float GetY();
			float GetZ();

			float GetRX();
			float GetRY();
			float GetRZ();

			UINT GetNumButtons() { return m_iNumButtons; }
			bool GetButtonPressed( UINT button )
			{
				if ( button > 32 ) return 0;
				if ( !m_iPrevButtons[ button ] && m_iButtons[ button ] ) return true;
				return false;
			}

			bool GetButtonReleased( UINT button )
			{
				if ( button > 32 ) return 0;
				if ( m_iPrevButtons[ button ] && !m_iButtons[ button ] ) return true;
				return false;
			}

			bool GetButtonState( UINT button )
			{
				if ( button > 32 ) return 0;
				return m_iButtons[ button ] != 0;
			}

			void Update();
	};
}

#endif
