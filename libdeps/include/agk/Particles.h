#ifndef _H_PARTICLES_
#define _H_PARTICLES_

// Common includes
#include "Common.h"

// Namespace
namespace AGK
{
	class cSpriteMgrEx;

	class cParticle
	{
		protected:
			UINT m_iColor;

		public:
			float m_fX;
			float m_fY;
			float m_fVX;
			float m_fVY;
			float m_fAngle;
			float m_fAngleDelta;
			float m_fScale;
			float m_fTime;
			bool m_bAlive;
			
			cParticle();
			~cParticle() {}

			UINT GetColor() { return m_iColor; }

			UINT GetAlpha() { return m_iColor >> 24; }
			UINT GetBlue() { return (m_iColor >> 16) & 0xff; }
			UINT GetGreen() { return (m_iColor >> 8) & 0xff; }
			UINT GetRed() { return m_iColor & 0xff; }

			void SetColor( UINT red, UINT green, UINT blue, UINT alpha )
			{
				if ( red > 255 ) red = 255;
				if ( green > 255 ) green = 255;
				if ( blue > 255 ) blue = 255;
				if ( alpha > 255 ) alpha = 255;

				m_iColor = (((((alpha << 8) | blue) << 8) | green) << 8) | red;
			}
	};

	class cParticleForce
	{
		public:
			float m_fStartTime;
			float m_fEndTime;
			float m_fX;
			float m_fY;
			cParticleForce *m_pNext;
	};

	class cParticleColor
	{
		public:
			float m_fTime;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char alpha;
			cParticleColor *m_pNext;
	};

	class cParticleSize
	{
		public:
			float m_fTime;
			float m_fScale;
			cParticleSize *m_pNext;
	};

	class cParticleEmitter
	{
		protected:
			UINT m_iID;
			float m_fX;
			float m_fY;
			float m_fVX;
			float m_fVY;
			float m_fAngle;
			float m_fVMin;
			float m_fVMax;
			float m_fSize;
			float m_fDepth;
			int m_iDepth;
			float m_fLife;
			bool m_bInterpolateColor;
			float m_fFreq;
			float m_fNumStart;
			UINT m_iCurrParticle;
			UINT m_iNumParticles;
			bool m_bVisible;
			bool m_bActive;
			float m_fAMin;
			float m_fAMax;
			bool m_bFaceDir;

			int m_iMaxParticles;
			int m_iEmittedParticles;
			bool m_bSomeAlive;

			float m_fStartX1;
			float m_fStartY1;
			float m_fStartX2;
			float m_fStartY2;

			UINT m_iImageID;
			cImage *m_pImage;
			cParticle **m_pParticles;
			cParticleForce *m_pForces;
			cParticleColor *m_pColors;
			cParticleSize *m_pSizes;

			UINT m_iNumVertices;
			float *m_pVertexArray;
			float *m_pUVArray;
			UINT *m_pColorArray;
			unsigned short *m_pIndices;
			int m_iTransparencyMode;
			
			bool m_bDepthChanged;
			bool m_bTextureChanged;

			bool m_bFixedToScreen;
			bool m_bManagedDrawing;
			cSpriteMgrEx* m_pSpriteManager;
			void UpdateManager();

			void UpdateNumParticles();

			void PlatformDrawQuadParticles( UINT count, unsigned short *pIndices, float *pVertices, float *pUV, unsigned char *pColors );
			//void PlatformDrawPointParticles( UINT count, float *pVertices, unsigned char *pColors );
			static float GetMaxPointSize();
			
		public:
		
			static int m_iParticlesDrawn;
			static int m_iQuadParticlesDrawn;
			
			cParticleEmitter();
			~cParticleEmitter();

			UINT GetID() { return m_iID; }
			void SetID( UINT ID ) { m_iID = ID; }
			void SetSpriteManager( cSpriteMgrEx *pMgr );
			
			void SetPosition( float x, float y );
			void SetDepth( int depth );
			void SetFrequency( float freq );
			void SetStartZone( float x1, float y1, float x2, float y2 );
			void SetDirection( float vx, float vy );
			void SetVelocityRange( float v1, float v2 );
			void SetAngle( float angle );
			void SetAngleRad( float angle );
			void SetSize( float size );
			void SetLife( float time );
			void SetImage( cImage *pImage );
			void SetColorInterpolation( int mode );
			void SetMaxParticles( int max );
			void ResetParticleCount();
			void SetVisible( int visible ) { m_bVisible = (visible != 0); }
			void SetActive( int active ) { m_bActive = (active != 0); }
			void SetRotationRate( float a1, float a2 );
			void SetRotationRateRad( float a1, float a2 );
			void SetFaceDirection( int flag );
			void SetTransparency( int mode );

			float GetX() { return m_fX; }
			float GetY() { return m_fY; }
			float GetZ() { return m_fDepth; }
			int GetDepth();
			float GetFrequency() { return m_fFreq; }
			float GetDirectionX() { return m_fVX; }
			float GetDirectionY() { return m_fVY; }
			float GetAngle() { return m_fAngle * 180 / PI; }
			float GetAngleRad() { return m_fAngle; }
			float GetSize() { return m_fSize; }
			float GetLife() { return m_fLife; }
			int GetMaxParticlesReached();
			int GetVisible() { return m_bVisible ? 1 : 0; }
			int GetActive() { return m_bActive ? 1 : 0; }
			int GetTransparency() { return m_iTransparencyMode; }

			void Offset( float x, float y );

			bool CheckDepthChanged() 
			{
				bool bChanged = m_bDepthChanged;
				return bChanged; 
			}
			bool GetDepthChanged() 
			{
				bool bChanged = m_bDepthChanged;
				m_bDepthChanged = false;
				return bChanged; 
			}
			bool GetTextureChanged() 
			{ 
				bool bChanged = m_bTextureChanged;
				m_bTextureChanged = false;
				return bChanged; 
			}
			
			void AddForce( float starttime, float endtime, float vx, float vy );
			void ClearForces();
			void AddColorKeyFrame( float time, UINT red, UINT green, UINT blue, UINT alpha );
			void ClearColors();
			void AddScaleKeyFrame( float time, float scale );
			void ClearScales();

			void FixToScreen( bool fix );
			
			bool IsManaged() { return m_bManagedDrawing; }
			void Update( float time );
			void DrawAll();
	};
}

#endif