#ifndef _H_OBJECT3D_
#define _H_OBJECT3D_

// Common includes
#include "Common.h"
#include "AGKShader.h"

#define AGK_OBJECT_CUSTOM_SHADER 0x00000001
#define AGK_OBJECT_USE_LIGHTS 0x00000002
#define AGK_OBJECT_DRAW_BACKFACES 0x00000004
#define AGK_OBJECT_Z_WRITE 0x00000008
#define AGK_OBJECT_IDENTITY_TRANSFORM 0x00000010
#define AGK_OBJECT_UNIFORM_SCALE 0x00000020
#define AGK_OBJECT_VISIBLE 0x00000040
#define AGK_OBJECT_TRANSCHANGED 0x00000080


// Namespace
namespace AGK
{
	class cImage;
	class CollisionObject;
	class CollisionResults;

	struct cVertex
	{
		float x,y,z;
	};

	struct cVertexAttrib
	{
		uString m_sName; // position, normal, texcoord, color, etc
		unsigned char m_iType; // 0=float, 1=ubyte
		char m_iShaderLoc; // max 16 on most cards
		unsigned char m_iComponents; // valid values: 1,2,3,4
		char m_iOffset; // offset of this element in the combined vertex buffer, in bytes, -1 if it is not in the combined buffer
		bool m_bNormalize; // when attributes are converted to float should they be normalised to the range 0.0-1.0
		void* m_pData;
		//cVertexAttrib *m_pNextAttrib;

		cVertexAttrib() 
		{ 
			m_iType = 0;
			m_iShaderLoc = -1;
			m_iComponents = 4;
			m_iOffset = -1;
			m_bNormalize = false;
			m_pData = 0;
			//m_pNextAttrib = 0;
		}

		cVertexAttrib( cVertexAttrib* other, UINT numVertices )
		{
			m_iType = other->m_iType;
			m_iShaderLoc = other->m_iShaderLoc;
			m_iComponents = other->m_iComponents;
			m_iOffset = other->m_iOffset;
			m_bNormalize = other->m_bNormalize;
			m_sName.SetStr( other->m_sName );
			m_pData = 0;
			if ( other->m_pData )
			{
				if ( m_iType == 0 )
				{
					m_pData = new float[ numVertices*m_iComponents ];
					for ( UINT i = 0; i < numVertices*m_iComponents; i++ ) ((float*)m_pData)[ i ] = ((float*)other->m_pData)[ i ];
				}
				else if ( m_iType == 1 )
				{
					m_pData = new unsigned char[ numVertices*m_iComponents ];
					for ( UINT i = 0; i < numVertices*m_iComponents; i++ ) ((unsigned char*)m_pData)[ i ] = ((unsigned char*)other->m_pData)[ i ];
				}
			}
		}

		~cVertexAttrib()
		{
			ClearData();
		}

		void ClearData()
		{
			if ( m_pData )
			{
				if ( m_iType == 0 ) delete [] (float*)m_pData;
				else if ( m_iType == 1 ) delete [] (unsigned char*)m_pData;
			}
			m_pData = 0;
		}
	};

	// if a shader requires more attributes than the object provides, use these to fill the blanks with static data
	struct cDummyAttrib
	{
		char m_iShaderLoc;
		unsigned char *m_pData;
		uString m_sName;
		cDummyAttrib *m_pNextAttrib;

		cDummyAttrib()
		{
			m_iShaderLoc = -1;
			m_pData = 0;
			m_pNextAttrib = 0;
		}

		~cDummyAttrib()
		{
			if ( m_pData ) delete [] m_pData;
		}
	};

	class cObject3D
	{
		protected:

			friend class agk;
			friend class cObjectMgr;

			UINT m_iID;

			cObject3D *m_pSharedVerticesObject;
			
			// drawing arrays
			UINT m_iNumArrays;
			UINT m_iVertexStride;
			UINT *m_iNumVertices;
			float **m_ppVBOVertexData;
			UINT *m_iNumIndices;
			unsigned short **m_ppIndices;
			UINT *m_iVBOVertices;
			
			// textures
			cImage *m_pImage[ AGK_MAX_TEXTURES ];
			UINT m_bSharedImages;

			// shader
			AGKShader *m_pShader;
			
			// vertex attributes containing the raw vertex data (not for drawing), this is used to create the draw arrays and the collision data.
			int m_iPosAttrib; // index for the vertex positions (if any) for quick access.
			cVertexAttrib **m_pVertexAttribs;
			unsigned char m_iNumAttribs;
			cDummyAttrib *m_pDummyAttribs;
			UINT m_iNumAttribVertices;

			// raw indices, if any. Not for drawing, this is used to create the drawing arrays and collision data.
			UINT* m_pRawIndices;
			UINT m_iNumRawIndices;
			
			// transform
			AGKVector m_position;
			AGKQuaternion m_rotation;
			AGKVector m_scale;
			float m_fLargestScale;

			// attributes
			UINT m_bFlags;
			UINT m_iColor;
			unsigned char m_iTransparency;
			unsigned char m_iZFunc;
			unsigned char m_iCullMode; // 0=no culling, 1=front faces (default), 2=back faces

			// nested object pointers
			cObject3D *m_pSibling;
			cObject3D *m_pChild;
			cObject3D *m_pParent;

			// collision
			CollisionObject *m_pColObject;
			bool m_bCollisionOn;

			// global array of all objects
			cObject3D *g_pNextObject;
			static cObject3D *g_pAllObjects;

			bool m_bManaged;

			void PlatformGenBuffers();
			void PlatformDraw();

			// fills the VBO arrays with chunks of 65535 vertices
			void CreateVBOLists( float *pVertices, UINT numVertices, UINT vertexStride, UINT *pIndices, UINT numIndices );
			void CreateCollisionData();

			int ParseFindChunk( const char *szFilename, char* data, int index, char* chunkName, UINT maxLength );
			int ParseChunkObject( const char *szFilename, char* data, int index );
			int ParseChunkUnknown( const char *szFilename, char* data, int index );
			int ParseChunkFloat( const char *szFilename, char* data, int index, float* f );
			int ParseChunkInt( const char *szFilename, char* data, int index, int* i );
			int ParseChunkString( const char *szFilename, char* data, int index, uString &out );
			int ParseChunkFloatArray( const char *szFilename, const char *szChunkName, char* data, int index, float* f, UINT elements );
			int ParseChunkIntArray( const char *szFilename, const char *szChunkName, char* data, int index, int* i, UINT elements );
			int ParseChunkVertexAttrib( const char *szFilename, char* data, int index, cVertexAttrib *pAttrib, bool &bSuccess );

			void InternalSetShader( AGKShader *pShader );

		public:
			static void ReloadAll();

			cObject3D();
			cObject3D( cObject3D *pOther, int share );
			~cObject3D();
			void ClearAttribs();
			void ClearRawVertexData(); // if the vertex data will never be modified we can throw the original arrays away.
			bool IsStandardSettings();

			void CheckShader();

			bool IsManaged() { return m_bManaged; }

			void CreateBox( float width, float height, float length );
			void CreateSphere( float diameter, int rows, int columns );
			//void CreateSphere2( float diameter, int rows, int columns ); // uses spherical texture projection
			void CreateCone( float height, float diameter, int segments );
			void CreateCylinder( float height, float diameter, int segments );
			void CreatePlane( float width, float height );
			void CreateQuad();

			void LoadObject( const char *szFilename, float height=0 );
			void LoadOBJ( const char *szFilename, float height=0 );
			void LoadAGOAscii( const char *szFilename, float height=0 );

			float GetX() { return m_position.x; }
			float GetY() { return m_position.y; }
			float GetZ() { return m_position.z; }

			float GetEulerX() { return m_rotation.GetEulerX(); }
			float GetEulerY() { return m_rotation.GetEulerY(); }
			float GetEulerZ() { return m_rotation.GetEulerZ(); }

			float GetScaleX() { return m_scale.x; }
			float GetScaleY() { return m_scale.y; }
			float GetScaleZ() { return m_scale.z; }

			const AGKQuaternion& GetRotation() { return m_rotation; }
			
			void SetPosition( float x, float y, float z );
			void SetScale( float x, float y, float z );
			void SetRotationQuat( const AGKQuaternion &rot );
			void SetRotationEuler( float angX, float angY, float angZ );

			void MoveLocalX( float amount );
			void MoveLocalY( float amount );
			void MoveLocalZ( float amount );

			void RotateLocalX( float amount ) { m_rotation.AddLocalRotation( 0, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }
			void RotateLocalY( float amount ) { m_rotation.AddLocalRotation( 1, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }
			void RotateLocalZ( float amount ) { m_rotation.AddLocalRotation( 2, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }

			void RotateGlobalX( float amount ) { m_rotation.AddGlobalRotation( 0, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }
			void RotateGlobalY( float amount ) { m_rotation.AddGlobalRotation( 1, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }
			void RotateGlobalZ( float amount ) { m_rotation.AddGlobalRotation( 2, amount ); m_bFlags &= ~AGK_OBJECT_IDENTITY_TRANSFORM; }

			void LookAt( float x, float y, float z, float roll );

			void SetShader( AGKShader *pShader );
			void SetImage( cImage *pImage, UINT stage=0 );
			void SetColor( UINT red, UINT green, UINT blue, UINT alpha );
			void SetRed( UINT iRed );
			void SetGreen( UINT iGreen );
			void SetBlue( UINT iBlue );
			void SetAlpha( UINT iAlpha );
			void SetLightMode( UINT mode );

			void SetDepthReadMode( UINT mode );
			void SetDepthWrite( UINT mode );
			void SetTransparency( UINT mode );
			void SetCullMode( UINT mode );
			void SetVisible( UINT mode );

			// get
			bool IsIdentityTransform() { return (m_bFlags & AGK_OBJECT_IDENTITY_TRANSFORM) != 0; }
			UINT GetDepthReadMode() { return m_iZFunc; }
			UINT GetDepthWrite() { return (m_bFlags & AGK_OBJECT_Z_WRITE) ? 1 : 0; }
			UINT GetTransparency() { return m_iTransparency; }
			UINT GetCullMode() { return m_iZFunc; }
			UINT GetVisible() { return (m_bFlags & AGK_OBJECT_VISIBLE) ? 1 : 0; }
			UINT GetInScreen() { return 1; }
			bool GetTransparencyChanged();
			bool CheckTransparencyChanged();

			AGKShader* GetShader() { return m_pShader; }
			
			// collision
			void SetCollisionMode( int mode );

			bool RayCast( float oldx, float oldy, float oldz, float x, float y, float z, CollisionResults *res );
			bool RayCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, CollisionResults *res );

			bool SphereCast( float oldx, float oldy, float oldz, float x, float y, float z, float rRadius, CollisionResults* cRes);
			bool SphereCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* cRes);

			bool SphereSlide( float oldx, float oldy, float oldz, float x, float y, float z, float rRadius, CollisionResults* cRes);
			bool SphereSlide( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* cRes);

			//bool Intersects( cObject3D* obj2 );

			void Update( float time );
			void Draw();
	};
}

#endif