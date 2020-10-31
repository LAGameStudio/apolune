#include "StencilShader.h"
#include "FBO.h"

StencilShader stencilShader;
Stencil2Shader stencil2Shader;
Stencil3Shader stencil3Shader;
Stencil4Shader stencil4Shader;

StencilAddShader stencilAddShader;
StencilAdd2Shader stencilAdd2Shader;
StencilAdd3Shader stencilAdd3Shader;
StencilAdd4Shader stencilAdd4Shader;

StencilAvg2Shader stencilAvg2Shader;
StencilAvg3Shader stencilAvg3Shader;
StencilAvg4Shader stencilAvg4Shader;

StencilMult2Shader stencilMult2Shader;
StencilMult3Shader stencilMult3Shader;
StencilMult4Shader stencilMult4Shader;

StencilMult2AlphaShader stencilMult2AlphaShader;

void ShadedStencilR( GLuint *in, GLuint *stencilR, FBOColor *out ) {
 stencilShader.pTex0=in;
 stencilShader.pTex1=stencilR;
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&stencilShader);
 out->Unbind();
}

void ShadedStencilRG( GLuint *in, GLuint *stencilR, GLuint *stencilG, FBOColor *out ) {
 stencil2Shader.pTex0=in;
 stencil2Shader.pTex1=stencilR;
 stencil2Shader.pTex2=stencilG;
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&stencil2Shader);
 out->Unbind();
}

void ShadedStencilRGB( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, FBOColor *out ) {
 stencil3Shader.pTex0=in;
 stencil3Shader.pTex1=stencilR;
 stencil3Shader.pTex2=stencilG;
 stencil3Shader.pTex3=stencilB;
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&stencil3Shader);
 out->Unbind();
}

void ShadedStencilRGBA( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, GLuint *stencilA, FBOColor *out ) {
 stencil4Shader.pTex0=in;
 stencil4Shader.pTex1=stencilR;
 stencil4Shader.pTex2=stencilG;
 stencil4Shader.pTex3=stencilB;
 stencil4Shader.pTex4=stencilA;
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&stencil4Shader);
 out->Unbind();
}
