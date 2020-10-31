// Sprite Header
#ifndef _H_SPRITE_
#define _H_SPRITE_

// Common includes
#include "Common.h"
#include "uString.h"

#define MAX_SPRITES 0x7fffffff

// Sprite status flags
#define AGK_SPRITE_VISIBLE		0x00000001
#define AGK_SPRITE_ACTIVE		0x00000002
#define AGK_SPRITE_WIDTHCALC	0x00000004
#define AGK_SPRITE_HEIGHTCALC	0x00000008
#define AGK_SPRITE_DEPTHCHANGED 0x00000010
#define AGK_SPRITE_TEXCHANGED	0x00000020
#define AGK_SPRITE_TRANSCHANGED 0x00000040
#define AGK_SPRITE_MANUALOFFSET 0x00000080
#define AGK_SPRITE_LOOP			0x00000100
#define AGK_SPRITE_PLAYING		0x00000200
#define AGK_SPRITE_SHAREDIMAGE	0x00000400
#define AGK_SPRITE_SENSOR		0x00000800
#define AGK_SPRITE_POSTOPLEFT	0x00001000
#define AGK_SPRITE_SCROLL		0x00002000
#define AGK_SPRITE_FLIPH		0x00004000
#define AGK_SPRITE_FLIPV		0x00008000
#define AGK_SPRITE_SNAP			0x00010000
#define AGK_SPRITE_MANAGE_IMAGES 0x00020000
#define AGK_SPRITE_FORCE_TRANSPARENCY 0x00040000
#define AGK_SPRITE_CUSTOM_SHADER 0x00080000


// Namespace
namespace AGK
{
	class cImage;
	class AGKShader;
	class Bone2D;

	// class for holding animation UV coordinates
	class _EXPORT_ cSpriteFrame
	{
		public:
			// top left corner
			float		m_fU1;
			float		m_fV1;

			// bottom right corner
			float		m_fU2;
			float		m_fV2;

			int			m_iWidth;
			int			m_iHeight;

			cImage		*m_pFrameImage;

			cSpriteFrame() { memset( this, 0, sizeof(cSpriteFrame) ); }
			cSpriteFrame( const cSpriteFrame* pOther ) { memcpy( this, pOther, sizeof(cSpriteFrame) ); }
			~cSpriteFrame() {}
	};

	class _EXPORT_ cSprite
	{
		protected:

			static cSprite* g_pAllSprites;
			static cSprite* g_pLastSprite;
			static UINT g_iCreated;

			friend class agk;
			friend class cSpriteMgr;
			friend class cSpriteMgrEx;

			// general variables
			uString     m_sName; // used for bone attachments
			UINT		m_iCreated;
			UINT		m_iID;
			bool		m_bManaged;
			float		m_fX;
			float		m_fY;
			float		m_fZ;
			int			m_iDepth;
			float		m_fWidth;
			float		m_fHeight;
			float		m_fOffsetX;
			float		m_fOffsetY;
			float		m_fAngle;
			float		m_fOrigWidth;
			float		m_fOrigHeight;
			float		m_fOrigRadius;
			UINT		m_bFlags;
			int			m_iTransparencyMode;
			float		m_fVisualRadius;
			float		m_fColRadius;

			Bone2D*		m_pBone;

			// image variables
			cImage*		m_pImage;
			UINT		m_iImageID;
			cImage*		m_pAdditionalImages[8];
			//UINT		m_iImageInternalID;
			UINT		m_iColor;

			// shader variables
			AGKShader*	m_pShader;

			float		m_fClipX;
			float		m_fClipY;
			float		m_fClipX2;
			float		m_fClipY2;
			
			// UV variables
			//int		m_iUVMode;
			float		m_fUVBorder;
			float		m_fUOffset;
			float		m_fVOffset;
			float		m_fUScale;
			float		m_fVScale;

			bool		m_bUVOverride;
			float		m_fU1;
			float		m_fV1;
			float		m_fU2;
			float		m_fV2;
			float		m_fU3;
			float		m_fV3;
			float		m_fU4;
			float		m_fV4;
			
			// collision variables
			int			m_iGroup;
			UINT		m_iCategories;
			UINT		m_iCategoryMask;
			float*		m_fPolygonPointsTemp;
			UINT		m_iPolygonPointsNum;

			// animation variables
			int			m_iFrameCount;
			int			m_iFrameArraySize;
			int			m_iFrameWidth;
			int			m_iFrameHeight;
			int			m_iCurrentFrame;
			int			m_iFrameStart;
			int			m_iFrameEnd;
			float		m_fFrameTimer;
			float		m_fFrameChangeTime;
			cSpriteFrame *m_pFrames;

			void*		m_pUserData;

			cSprite*	m_pPrevSprite;
			cSprite*	m_pNextSprite;

#ifdef USE_BOX2D
			enum ePhysicsMode
			{
				eStatic,
				eDynamic,
				eKinematic
			};

			enum ePhysicsShape
			{
				eNone,
				eBox,
				eCircle,
				ePolygon,
				eManual
			};

			// physics variables
			b2Body *m_phyBody;
			b2Shape *m_phyShape;
			ePhysicsShape m_eShape;
			b2DistanceOutput *m_colResult;
			b2Contact *m_lastContact;
			b2ContactEdge *m_pContactIter;

			void ReplacePhysicsShape();
#endif

			// internal
			void Reset ( void );
			void RecalcVisualRadius();
			void RecalcColRadius();
			void CheckAndRemoveImage( cImage* pImage );
			void CheckTransparency();

			// platform specific
			void PlatformDraw( float *vertices, float *uv, unsigned char *color );
			void InternalSetShader( AGKShader* shader );

		public:

			static int g_iPixelsDrawn;

			static void UpdateDepth();
			static void RemoveImage( cImage *pDelImage );

			// construct/destruct
			cSprite();
			cSprite( const cSprite *pOtherSprite );
			cSprite( const uString &szImage );
			cSprite( cImage *pImage );
			~cSprite();

			void ImageDeleting( cImage *pImage );
		
			// get functions
			const char* GetName			( void ) { return m_sName.GetStr(); }
			UINT GetCreated				( void );
			UINT GetID					( void );
			float GetX					( void );
			float GetY					( void );
			float GetZ					( void );
			float GetXByOffset			( void );
			float GetYByOffset			( void );
			float GetOffsetX			( void );
			float GetOffsetY			( void );
			float GetAngle				( void );
			float GetAngleRad			( void );
			float GetWidth				( void );
			float GetHeight				( void );
			int GetDepth				( void );
			int GetFrameCount			( void );
			int GetCurrentFrame			( void );
			int GetTransparencyMode		( void );
			cImage* GetImagePtr			( void );
			bool GetDepthChanged		( void );
			bool CheckDepthChanged		( void );
			bool GetTextureChanged		( void );
			bool CheckTextureChanged	( void );
			bool GetTransparencyChanged	( void );
			bool GetVisible				( void );
			bool GetInScreen			( void );
			bool GetActive				( void );
			bool GetHitTest				( float x, float y );
			bool GetShouldCollide		( cSprite *pSprite2 );
			UINT GetColorRed			( void );
			UINT GetColorGreen			( void );
			UINT GetColorBlue			( void );
			UINT GetColorAlpha			( void );
			int GetScrollMode			( void );
			int GetPlaying				( void );
			bool GetScissorOn			( void );
			void GetClipValues			( int &x, int &y, int &width, int &height );
			int GetGroup				( void );
			int HasAdditionalImages		( void );

			AGKShader* GetShader() { return m_pShader; }
			void SetShader( AGKShader* shader ); 

			float GetXFromPixel( int x );
			float GetYFromPixel( int y );
			int GetPixelFromX( float x );
			int GetPixelFromY( float y );

			float GetWorldXFromPoint( float x, float y );
			float GetWorldYFromPoint( float x, float y );
			float GetXFromWorld( float x, float y );
			float GetYFromWorld( float x, float y );

			// set functions
			void SetBone				( Bone2D *bone );
			void SetName				( const char* name );
			void SetID					( UINT ID );
			void SetVisible				( bool bVisible );
			void SetActive				( bool bActive );
			void SetColor				( UINT iRed, UINT iGreen, UINT iBlue, UINT iAlpha=255 );
			void SetRed					( UINT iRed );
			void SetGreen				( UINT iGreen );
			void SetBlue				( UINT iBlue );
			void SetAlpha				( UINT iAlpha );
			void SetFlip				( int horz, int vert );
			void SetSnap				( int snap );
			void SetPosition			( float fX, float fY );
			void SetPositionByOffset	( float fX, float fY );
			void SetX					( float fX );
			void SetY					( float fY );
			void SetZ					( float fZ );
			void SetZShift				( float fShift );
			void SetAngle				( float fAngle );
			void SetAngleRad			( float fAngleRad );
			void SetDepth				( int iDepth );
			void SetSize				( float iWidth=-1, float iHeight=-1, bool bUpdateShape=true );
			void Rotate					( float fAngle );
			void SetScale				( float x, float y );
			void SetScaleByOffset		( float x, float y );
			void SetImage				( const uString &szImage, bool bUpdateCollisionShape=false );
			void SetImage				( cImage *pImage, bool bUpdateCollisionShape=false );
			void SetAdditionalImage		( cImage *pImage, int stage );
			void SwitchImage			( cImage *pImage, bool bUpdateCollisionShape=false );
			void SetTransparency		( int mode );
			void SetOffset				( float x, float y );
			void SetUVBorder			( float border );
			void SetUVOffset			( float u, float v );
			void SetUVScale				( float scaleU, float scaleV );
			void FixToScreen			( int mode );
			void SetScissor				( float x, float y, float x2, float y2 );
			void SetUV					( float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4 );
			void ResetUV				();
			void SetManageImages		( int mode );

			void SetUserData			( void* data );
			void* GetUserData			( );

			void SetAnimation			( int iFrameWidth, int iFrameHeight, int iFrameCount );
			void AppendAnimation		( cImage *pImage, int iFrameWidth, int iFrameHeight, int iFrameCount );
			void AddAnimationFrame		( cImage *pImage );
			void ExpandAnimationArray	( int newTotalFrames );
			void ClearAnimationFrames   ();
			void SetFrame				( int iFrame );
			void SetSpeed				( float fFps );
			void Play					( float fFps, bool bLoop=true, int iStart=-1, int iEnd=-1 );
			void Stop					();
			void Resume					();
			//void SetBlendFunction		( GLenum src, GLenum dest );

			void CheckImages			();
			void Draw					();
			void BatchDraw				( GLfloat *pVertices, GLfloat *pUV, GLubyte *color );
			void BatchDrawQuad			( GLfloat *pVertices, GLfloat *pUV, GLubyte *color );

			UINT GetVectorPoints		();
			void BatchDrawVector		( GLfloat *pVertices, GLubyte *color );

			void DrawDebug				();
			void Update					( float time );
			void UpdateAnimation		( float time );

			// physics functions
#ifdef USE_BOX2D
			bool InBox( float x1, float y1, float x2, float y2 );
			bool InCircle( float x1, float y1, float radius );
			bool GetCollision			( cSprite *pSprite2 );

			float GetDistance			( cSprite *pSprite2 );
			float GetDistancePoint1X    ( void );
			float GetDistancePoint1Y    ( void );
			float GetDistancePoint2X    ( void );
			float GetDistancePoint2Y    ( void );

			bool GetPhysicsCollision( cSprite *pSprite2 );
			float GetPhysicsCollisionX( );
			float GetPhysicsCollisionY( );
			float GetPhysicsCollisionWorldX( );
			float GetPhysicsCollisionWorldY( );

			float GetPhysicsMass( );

			// can be used on its own without physics to detect hits and overlaps, sets main shape only
			void SetShape					( ePhysicsShape shape );
			void SetShapeBox				( float x, float y, float x2, float y2, float angle=0 );
			void SetShapeCircle				( float x, float y, float raduis );
			void SetShapePolygon			( UINT numPoints, float *pPoints );
			void SetShapePolygon			( UINT numPoints, UINT index, float x, float y );

			// adds an additional shape to the physics body, physics sprites only, not used on non-physics functions
			void AddShapeBox				( float x, float y, float x2, float y2, float angle=0 );
			void AddShapeCircle				( float x, float y, float raduis );
			void AddShapePolygon			( UINT numPoints, float *pPoints );
			void AddShapePolygon			( UINT numPoints, UINT index, float x, float y );
			void ClearShapes				();

			//void Create

			void CalculatePhysicsCOM		();
			void SetPhysicsCOM				( float x, float y );

			void SetGroup					( int group );
			void SetCategoryBits			( UINT categories );
			void SetCategoryBit				( UINT category, int flag );
			void SetCollideBits				( UINT mask );
			void SetCollideBit				( UINT category, int flag );

			// physics dynamics/response
			void SetPhysicsOn				( ePhysicsMode mode );
			void SetPhysicsOff				();
			void SetPhysicsDelete			();
			void SetPhysicsFriction			( float friction );
			void SetPhysicsRestitution		( float restitution );
			void SetPhysicsCanRotate		( bool rotate );
			void SetPhysicsVelocity			( float vx, float vy );
			void SetPhysicsAngularVelocity	( float va );
			void SetPhysicsDamping			( float damp );
			void SetPhysicsAngularDamping	( float damp );
			void SetPhysicsIsBullet			( bool bullet );
			void SetPhysicsMass				( float mass );
			void SetPhysicsIsSensor			( bool sensor );

			float GetPhysicsVelocityX();
			float GetPhysicsVelocityY();
			float GetPhysicsAngularVelocity();
			
			void SetPhysicsForce			( float x, float y, float vx, float vy );
			void SetPhysicsTorque			( float a );
			void SetPhysicsLinearImpulse	( float x, float y, float vx, float vy );
			void SetPhysicsAngularImpulse	( float a );

			int GetFirstContact				();
			int GetNextContact				();
			float GetContactWorldX			();
			float GetContactWorldY			();
			cSprite* GetContactSprite2		();
			
			void UpdatePhysics				();
#endif
	};
}

#endif
