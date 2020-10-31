#ifndef _H_TEXTENTRY_
#define _H_TEXTENTRY_

// Common includes
#include "Common.h"
#include "uString.h"

#define MAX_EDITBOXES 0x7fffffff

// Namespace
namespace AGK
{
	class cSprite;

	class cEditBox
	{
		protected:
			UINT m_iID;
			float m_fX;
			float m_fY;
			float m_fZ;
			int m_iDepth;
			float m_fWidth;
			float m_fHeight;
			float m_fBorder;
			bool m_bActive;
			bool m_bActivePrev;
			uString m_sCurrInput;
			bool m_bEnabled;
			bool m_bVisible;
			float m_fTextSize;
			bool m_bMultiLine;
			bool m_bDepthChanged;
			bool m_bFixed;
			bool m_bIsPassword;
            bool m_bUseAlternateInput;
            bool m_bSupportsExtendedAscii;
            bool m_bSupportWarned;
			int m_iWrapMode;
			char m_iTextChanged;
            int m_iLastLength;
			
			int m_iCursorPos;
			float m_fCursorBlinkTime;
			float m_fCursorBlinkDelay;
			int m_iShowCursor;
			UINT m_iMaxChars;
			UINT m_iMaxLines;
			int m_iLines;

			float m_fScissorX;
			float m_fScissorY;
			float m_fScissorX2;
			float m_fScissorY2;

			cSprite *m_pInputCursor;
			cSprite *m_pInputBorder;
			cSprite *m_pInputArea;
			cText *m_pInputText;

			static cImage *g_pDefaultFont;
			static cEditBox *g_pCurrentEditBox;

			void ParametersChanged();

			void PlatformStartText();
			bool PlatformUpdateText();
			void PlatformEndText();

		public:
			cEditBox();
			~cEditBox();

			void SetID( UINT iID );
			void SetPosition( float x, float y );
			void SetSize( float width, float height );
			void SetDepth( int depth );
			void SetBorderSize( float size );
			void SetBorderColor( UINT red, UINT green, UINT blue, UINT alpha );
			void SetBackgroundColor( UINT red, UINT green, UINT blue, UINT alpha );
			void SetTextColor( UINT red, UINT green, UINT blue );
			void SetCursorColor( UINT red, UINT green, UINT blue );
			void SetFontImage( cImage* image );
            void SetExtendedFontImage( cImage* image );
			void SetTextSize( float size );
			void SetActive( bool active );
			void SetFocus( bool active );
			void SetVisible( bool active );
			void SetCursorLocation( float x, float y );
			void SetBorderImage( cImage* image );
			void SetBackgroundImage( cImage* image );
			void SetCursorBlinkTime( float time );
			void SetCursorWidth( float width );
			void SetText( const char *str );
			void SetMaxChars( UINT max );
			void SetMultiLine( bool multiline );
			void SetMaxLines( UINT lines );
			void SetScissor( float x, float y ,float x2, float y2 );
			void FixToScreen( bool fix );
			void SetPasswordMode( int mode ) { m_bIsPassword = (mode==1); }
            void SetUseAlternateInput( int mode ) { m_bUseAlternateInput = (mode==1); }
			void SetWrapMode( int mode );

			UINT GetID() { return m_iID; }
			float GetX() { return m_fX; }
			float GetY() { return m_fY; }
			float GetZ() { return m_fZ; }
			int GetDepth() { return m_iDepth; }
			bool GetFixed() { return m_bFixed; }
			bool GetDepthChanged();
			bool CheckDepthChanged() { return m_bDepthChanged; }
			float GetWidth() { return m_fWidth; }
			float GetHeight() { return m_fHeight; }
			int GetText( uString &text ) { m_pInputText->GetText( text ); return text.GetLength(); }
			char* GetText(); // must delete returned string
			bool GetHitTest( float x, float y );
			bool GetActive() { return m_bEnabled; }
			bool GetTextChanged() { bool result = m_iTextChanged>0; m_iTextChanged = 0; return result; }
			bool GetVisible() { return m_bVisible; }
			bool HasFocus() { return m_bActive; }
			int GetLines() { return m_pInputText->GetLines(); }
            bool GetMultiLine() { return m_bMultiLine; }

			void Update();
			void Draw();
	};
}

#endif
