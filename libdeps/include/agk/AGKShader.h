// Image Header
#ifndef _H_AGKSHADER_
#define _H_AGKSHADER_

// Common includes
#include "Common.h"
#include "3DMath.h"

#define AGK_SHADER_WORLD_CHANGED 0x00000001
#define AGK_SHADER_VIEW_CHANGED 0x00000002
#define AGK_SHADER_PROJ_CHANGED 0x00000004
#define AGK_SHADER_WORLDNORMAL_CHANGED 0x00000008
#define AGK_SHADER_ORTHO_CHANGED 0x00000010
#define AGK_SHADER_RELOAD_UNIFORMS 0x00000020
#define AGK_SHADER_CAMERA_CHANGED 0x00000040
#define AGK_SHADER_PER_SPRITE_UNIFORM 0x00000080

#define AGK_TRIANGLES 0
#define AGK_TRIANGLE_STRIP 1
#define AGK_TRIANGLE_FAN 2
#define AGK_LINE_LOOP 3
#define AGK_LINES 4
#define AGK_POINTS 5

#define AGK_MAX_TEXTURES 8
#define AGK_MAX_ATTRIBUTES 16

// Namespace
namespace AGK
{
	class cCamera;

	struct cShaderAttrib
	{
		uString m_sName; // position, normal, texcoord, color, etc
		int m_iType; // 0=float, 1=matrix
		int m_iLocation; // max 16 on most cards
		UINT m_iComponents; // valid values: 1,2,3,4 for float or 2,3,4 for matrix
		cShaderAttrib *m_pNextAttrib;

		cShaderAttrib() 
		{ 
			m_iType = 0;
			m_iLocation = -1;
			m_iComponents = 4;
			m_pNextAttrib = 0;
		}

		~cShaderAttrib() { }
	};

	struct cShaderUniform
	{
		uString m_sName; // custom name
		int m_iType; // 0=float, 1=matrix
		int m_iLocation;
		UINT m_iComponents; // valid values: 1,2,3,4 for float or 2,3,4 for matrix
		UINT m_iArrayMembers;
		cShaderUniform *m_pNextUniform;
		bool m_bChanged;
		float* m_pValues;

		cShaderUniform() 
		{ 
			m_iType = 0;
			m_iLocation = -1;
			m_iComponents = 4;
			m_iArrayMembers = 1;
			m_pNextUniform = 0;
			m_bChanged = false;
			m_pValues = 0;
		}

		~cShaderUniform() 
		{
			if ( m_pValues ) delete [] m_pValues;
		}
	};

	class AGKShader
	{
		protected:
			UINT m_iShaderID;
			uString m_sVSFilename;
			uString m_sPSFilename;
			bool m_bValid;
			bool m_bReloading;

			// uniforms (VS + PS)
			int m_iUniformWorldMat;
			int m_iUniformNormalMat;
			int m_iUniformViewMat;
			int m_iUniformOrthoMat;
			int m_iUniformProjMat;
			int m_iUniformVPMat;
			int m_iUniformWVPMat;
			int m_iUniformWVOMat;

			int m_iNumUniforms;
			cHashedList<cShaderUniform> m_cUniformList;
			cShaderUniform* m_pChangedUniforms;

			// agk uniforms
			int m_iAGKTime;
			int m_iAGKSinTime;
			int m_iAGKResolution;
			int m_iAGKInvert;
			
			// textures
			int m_iTexture2D[ AGK_MAX_TEXTURES ];
			int m_iTextureCube[ AGK_MAX_TEXTURES ];
			int m_iTextureBounds[ AGK_MAX_TEXTURES ];
			float m_fTextureU1[ AGK_MAX_TEXTURES ];
			float m_fTextureV1[ AGK_MAX_TEXTURES ];
			float m_fTextureU2[ AGK_MAX_TEXTURES ];
			float m_fTextureV2[ AGK_MAX_TEXTURES ];
			UINT m_bTextureChanged;

			// vertex attributes, copy some common attributes to separate variables
			int m_iPositionAttrib;
			int m_iNormalAttrib;
			int m_iColorAttrib;
			int m_iTexCoordAttrib;
			cHashedList<cShaderAttrib> m_cAttribList;
			int m_iNumAttribs;

			UINT m_bFlags;

			static cCamera *g_pCurrentCamera;

			// matrices
			static AGKMatrix4 g_matOrtho;
			static AGKMatrix4 g_matProj;
			AGKMatrix4 m_matView;
			AGKMatrix4 m_matWorld;
			AGKMatrix3 m_matWorldNormal;

			uString m_sVSLog;
			uString m_sPSLog;
			uString m_sLinkLog;

			AGKShader *m_pNextShader;

			static AGKShader *g_pAllShaders;
			static char g_iAttributeActive[ AGK_MAX_ATTRIBUTES ];

			void PlatformInit();
			void PlatformDelete();
			//void PlatformDrawTriangles( int first, UINT count );
			void PlatformDrawPrimitives( UINT primitive, int first, UINT count );
			void PlatformDrawIndices( UINT primitive, UINT count, unsigned short *pIndices );
			void PlatformSetUniform( int location, int size, float* values );

		public:

			static AGKShader *g_pCurrentShader;

			// default sprite shaders
			static AGKShader *g_pShaderColor;
			static AGKShader *g_pShaderTexColor;

			// default object shaders
			static AGKShader *g_pObject0T0D0P;
			static AGKShader *g_pObject1T0D0P;
			static AGKShader *g_pObject0T1D0P;
			static AGKShader *g_pObject1T1D0P;
			static AGKShader *g_pObject0T1D1P;
			static AGKShader *g_pObject1T1D1P;
			static AGKShader *g_pObjectQuad;

			static void SetProjMatrices( const float *matrix4x4 );
			static void SetOrthoMatrices( const float *matrix4x4 );
			static void NoShader();
			static void CreateDefaultShaders();
			static void SetCurrentCamera( cCamera* pCamera );

			static void ReloadAll();

			AGKShader();
			~AGKShader();

			const char* GetVSLog() { return m_sVSLog.GetStr(); }
			const char* GetPSLog() { return m_sPSLog.GetStr(); }
			const char* GetVSFilename() { return m_sVSFilename.GetStr(); }
			const char* GetPSFilename() { return m_sPSFilename.GetStr(); }

			void LoadShader( const char* vertexFile, const char* pixelFile );
			void LoadFullScreenShader( const char* pixelFile );
			void LoadSpriteShader( const char* pixelFile );
			void SetShaderSource( const char* vertex, const char* pixel );
			void MakeColorShader();
			void MakeTexColorShader();
			
			void MakeObject0T0D0P(); // 0 texture, no lights
			void MakeObject1T0D0P(); // 1 texture, no lights
			void MakeObject0T1D0P(); // 0 texture, 1 directional light
			void MakeObject1T1D0P(); // 1 texture, 1 directional light
			void MakeObject0T1D1P(); // 0 texture, 1 directional light, 1 point light
			void MakeObject1T1D1P(); // 1 texture, 1 directional light, 1 point light
			void MakeObjectQuad(); // fullscreen quad
			
			void MakeActive();
			//void NewObject();
			void UpdateMatrices();
			void UpdateUniforms();
			void UpdateAGKUniforms();

			int GetAttribByName( const char* name ); 
			int GetAttribPosition() { return GetAttribByName( "position" ); } 
			int GetAttribNormal() { return GetAttribByName( "normal" ); } 
			int GetAttribColor() { return GetAttribByName( "color" ); } 
			int GetAttribTexCoord() { return GetAttribByName( "uv" ); } 

			int GetNumAttribs() { return m_iNumAttribs; }
			int GetNumConstants() { return m_iNumUniforms; }

			const char* GetFirstConstantName();
			const char* GetNextConstantName();

			cShaderUniform* GetFirstConstant();
			cShaderUniform* GetNextConstant();

			cShaderAttrib* GetFirstAttribute();
			cShaderAttrib* GetNextAttribute();

			int GetConstantExists( const char* name );
			bool HasPerSpriteUniforms() { return (m_bFlags & AGK_SHADER_PER_SPRITE_UNIFORM) != 0; }

			// set by objects to temporary use a specific value, shader must be active
			void SetTempConstantByName( const char* szName, float f1, float f2, float f3, float f4 );
			void SetTempConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 );
			void SetTempConstantMatrixByName( const char* szName, const float* values );
			void SetTempConstantMatrixArrayByName( const char* szName, UINT index, const float* values );

			// set by the user for objects that don't specify a temporary value
			void SetConstantByName( const char* szName, float f1, float f2, float f3, float f4 );
			void SetConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 );
			void SetConstantMatrixByName( const char* szName, const float* values );
			void SetConstantMatrixArrayByName( const char* szName, UINT index, const float* values );

			void SetAttribFloat( UINT location, int size, int stride, const float* ptr ); // position, normal, texcoord, etc
			void SetAttribUByte( UINT location, int size, int stride, bool normalise, const unsigned char* ptr ); // color

			void SetWorldMatrix( const float *matrix4x4 ) { if ( m_matWorld == matrix4x4 ) return; m_matWorld.Set( matrix4x4 ); m_bFlags |= AGK_SHADER_WORLD_CHANGED; }
			void SetViewMatrix( const float *matrix4x4 ) { if ( m_matView == matrix4x4 ) return; m_matView.Set( matrix4x4 ); m_bFlags |= AGK_SHADER_VIEW_CHANGED; }
			void SetProjMatrix() { m_bFlags |= AGK_SHADER_PROJ_CHANGED; }
			void SetOrthoMatrix() { m_bFlags |= AGK_SHADER_ORTHO_CHANGED; }
			void SetNormalMatrix( const float *matrix3x3 ) { m_matWorldNormal.Set( matrix3x3 ); m_bFlags |= AGK_SHADER_WORLDNORMAL_CHANGED; }

			void SetTextureStage( cImage *pImage, UINT stage );

			void DrawTriangles( int first, UINT count );
			void DrawPrimitives( int type, int first, UINT count ); // 0=TRIANGLES, 1=TRIANGLE_STRIP, 2=TRIANGLE_FAN, 3=LINE_LOOP, 4=LINES, 5=POINTS
			void DrawIndices( UINT count, unsigned short *pIndices );
		};
}

#endif