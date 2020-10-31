#ifndef _H_SPRITEMGR_
#define _H_SPRITEMGR_

// Common includes
#include "Common.h"
#include "cSprite.h"
#include "cText.h"
#include "Particles.h"
#include "cTextEntry.h"
#include "AGKShader.h"
#include "Skeleton2D.h"
#include "Globals.h"

// Namespace
namespace AGK
{
	//class cSprite;
	//class cParticleEmitter;
	//class cText;
	class DebugDraw;
	class agk;

	extern DebugDraw g_DebugDraw;

	class _EXPORT_ cSpriteContainer
	{
		protected:
			UINT m_iType;
			void *m_pItem;
					
		public:
			cSpriteContainer *m_pPrev;
			cSpriteContainer *m_pNext;

			cSpriteContainer() { m_pNext = 0; m_iType = 0; m_pItem = 0; m_pPrev = 0; }
			~cSpriteContainer() {}
		
			void SetSprite( cSprite *pSprite )
			{
				m_iType = 1;
				m_pItem = pSprite;
			}

			void SetParticles( cParticleEmitter *pParticles )
			{
				m_iType = 2;
				m_pItem = pParticles;
			}

			void SetText( cText *pText )
			{
				m_iType = 3;
				m_pItem = pText;
			}

			void SetEditBox( cEditBox *pEditBox )
			{
				m_iType = 4;
				m_pItem = pEditBox;
			}

			void SetSkeleton2D( Skeleton2D *pSkeleton )
			{
				m_iType = 5;
				m_pItem = pSkeleton;
			}

			int GetType() { return m_iType; }

			/*
			float GetDepth()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetZ(); break;
					case 2: return ((cParticleEmitter*)m_pItem)->GetZ(); break;
					case 3: return ((cText*)m_pItem)->GetZ(); break;
					case 4: return ((cEditBox*)m_pItem)->GetZ(); break;
					case 5: return ((Skeleton2D*)m_pItem)->GetZ(); break;
					default: return 0;
				}
			}
			*/

			int GetDepthInt()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetDepth(); break;
					case 2: return ((cParticleEmitter*)m_pItem)->GetDepth(); break;
					case 3: return ((cText*)m_pItem)->GetDepth(); break;
					case 4: return ((cEditBox*)m_pItem)->GetDepth(); break;
					case 5: return ((Skeleton2D*)m_pItem)->GetDepth(); break;
					default: return 0;
				}
			}

			UINT GetCreated()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetCreated(); break;
					case 2: return 0xffffffff; break;
					case 3: return 0xffffffff; break;
					case 4: return 0xffffffff; break;
					case 5: return 0xffffffff; break;
					default: return 0;
				}
			}

			int GetDrawMode()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetTransparencyMode(); break;
					case 2: return 1; break;
					case 3: return ((cText*)m_pItem)->GetTransparency(); break;
					case 4: return 1; break;
					case 5: return 1; break;
					default: return 0;
				}
			}

			UINT GetTextureID()
			{
				switch( m_iType )
				{
					case 1: 
					{
						if ( !((cSprite*)m_pItem)->GetImagePtr() ) return 0;
						else return ((cSprite*)m_pItem)->GetImagePtr()->GetTextureID();
						break;
					}

					case 2: return 0; break;
					case 3: return 0; break;
					case 4: return 0; break;
					case 5: return 0; break;
					default: return 0;
				}
			}

			AGKShader* GetShader()
			{
				switch( m_iType )
				{
					case 1: 
					{
						return ((cSprite*)m_pItem)->GetShader();
						break;
					}

					case 2: return AGKShader::g_pShaderTexColor; break;
					case 3: return AGKShader::g_pShaderTexColor; break;
					case 4: return 0; break;
					case 5: return 0; break;
					default: return 0;
				}
			}

			/*
			bool GetDepthChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetDepthChanged(); break;
					case 2: return ((cParticleEmitter*)m_pItem)->GetDepthChanged(); break;
					case 3: return ((cText*)m_pItem)->GetDepthChanged(); break;
					case 4: return ((cEditBox*)m_pItem)->GetDepthChanged(); break;
					default: return false;
				}
			}

			bool CheckDepthChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->CheckDepthChanged(); break;
					case 2: return ((cParticleEmitter*)m_pItem)->CheckDepthChanged(); break;
					case 3: return ((cText*)m_pItem)->CheckDepthChanged(); break;
					case 4: return ((cEditBox*)m_pItem)->CheckDepthChanged(); break;
					default: return false;
				}
			}

			bool GetTextureChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetTextureChanged(); break;
					case 2: return false; break;
					case 3: return false; break;
					case 4: return false; break;
					default: return false;
				}
			}

			bool CheckTextureChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->CheckTextureChanged(); break;
					case 2: return false; break;
					case 3: return false; break;
					case 4: return false; break;
					default: return false;
				}
			}

			bool GetTransparencyChanged()
			{
				switch( m_iType )
				{
					case 1: return ((cSprite*)m_pItem)->GetTransparencyChanged(); break;
					case 2: return false; break;
					case 3: return ((cText*)m_pItem)->GetTextureChanged(); break;
					case 4: return false; break;
					default: return false;
				}
			}
			*/

			cSprite* GetSprite() { return (cSprite*)m_pItem; }
			cParticleEmitter* GetParticles() { return (cParticleEmitter*)m_pItem; }
			cText* GetText() { return (cText*)m_pItem; }
			cEditBox* GetEditBox() { return (cEditBox*)m_pItem; }
			Skeleton2D* GetSkeleton2D() { return (Skeleton2D*)m_pItem; }
	};

	/*
	class _EXPORT_ cSpriteMgr
	{
		protected:

			GLfloat *pVertices;
			GLfloat *pUV;
			GLubyte *pColor;
			unsigned short *pIndices;
		
			int iCurrentCount;
			bool m_bSortTextures;
			bool m_bSortDepth;
			bool m_bSortTransparentDepth;
			float m_f3DDepth;
			bool m_bUseDepthBuffer;

			cSpriteContainer *m_pOpaqueSprites;
			cSpriteContainer *m_pAlphaSprites;

			cSpriteContainer *m_pBackOpaqueSprites;
			cSpriteContainer *m_pBackAlphaSprites;
			int m_iQuality;
		
			UINT m_iLastTotal;
			UINT m_iLastSorted;
			UINT m_iLastDrawn;
			UINT m_iLastDrawCalls;

			UINT iCurrentTexture;
			AGKShader *m_pCurrentShader;

			bool AddContainer( cSpriteContainer *pNewMember );
			void DrawList( cSpriteContainer *pList, int mode );
			
			void PlatformSetupOpaque();
			void PlatformSetupColorKey();
			void PlatformSetupAlpha();
			void PlatformDrawTriangles( UINT count );
			void PlatformDrawIndices( UINT count, unsigned short *pIndices );

		public:

			cSpriteMgr();
			~cSpriteMgr();

			void ClearAll();
		
			UINT GetTotalCount() { return m_iLastTotal; }
			UINT GetSortedCount() { return m_iLastSorted; }
			UINT GetDrawnCount() { return m_iLastDrawn; }
			UINT GetDrawCalls() { return m_iLastDrawCalls; }
		
			void SetSortTextures( bool sort );
			void SetSortDepth( bool sort );
			void SetSortTransparentDepth( bool sort );
			void SetQuality( int quality );
			void Set3DDepth( float fZ ) { m_f3DDepth = fZ; }
			void SetUseDepthBuffer( bool sort );

			void AddSprite( cSprite* sprite );
			void AddText( cText* text );
			void AddParticles( cParticleEmitter* particles );
			void AddEditBox( cEditBox* editbox );

			void RemoveSprite( cSprite* sprite );
			void RemoveText( cText* text );
			void RemoveParticles( cParticleEmitter* particles );
			void RemoveEditBox( cEditBox* editbox );

			void ResortAll();
			virtual void UpdateAll( float time );
			virtual void DrawBack();
			virtual void DrawFront();
			void DrawDebug();
	};
	*/

	class _EXPORT_ cSpriteMgrEx
	{
		protected:

			GLfloat *pVertices;
			GLfloat *pUV;
			GLubyte *pColor;
			unsigned short *pIndices;
		
			int iCurrentCount;
			int iCurrentCountAll;
			
			cSpriteContainer *m_pSprites;
			cSpriteContainer *m_pLastSprite;
			AGKSortValue *m_pDrawList;
			
			UINT m_iLastTotal;
			UINT m_iLastSorted;
			UINT m_iLastDrawn;
			UINT m_iLastDrawCalls;

			UINT iCurrentTexture;
			AGKShader *m_pCurrentShader;

			bool AddContainer( cSpriteContainer *pNewMember );
						
		public:

			cSpriteMgrEx();
			~cSpriteMgrEx();

			void ClearAll();
		
			UINT GetTotalCount() { return m_iLastTotal; }
			UINT GetSortedCount() { return m_iLastSorted; }
			UINT GetDrawnCount() { return m_iLastDrawn; }
			UINT GetDrawCalls() { return m_iLastDrawCalls; }
		
			void AddSprite( cSprite* sprite );
			void AddText( cText* text );
			void AddParticles( cParticleEmitter* particles );
			void AddEditBox( cEditBox* editbox );
			void AddSkeleton2D( Skeleton2D* skeleton );

			void RemoveSprite( cSprite* sprite );
			void RemoveText( cText* text );
			void RemoveParticles( cParticleEmitter* particles );
			void RemoveEditBox( cEditBox* editbox );
			void RemoveSkeleton2D( Skeleton2D* skeleton );

			virtual void UpdateAll( float time );
			virtual void DrawAll();
			void DrawDebug();
	};
}

#endif
