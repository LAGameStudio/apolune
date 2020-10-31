#include "FBMShader.h"
#include "FBO.h"

FBMShader fbmShader;
FBMCheapShadowShader fbmcheapshadowShader;

void ShadedFBM( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&fbmShader);
 out->Unbind();
}

void ShadedFBMCheapShadows( GLuint *in, FBOColor *out )  {
 out->background.Pick(clear);
 out->Bind();
 fbmcheapshadowShader.pTex0=in;
 out->QuadYInverted(&fbmcheapshadowShader);
 out->Unbind();
}