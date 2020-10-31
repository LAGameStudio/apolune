#include "GammaShader.h"

GammaShader gammaShader;


void ShadedGamma( GLuint *source, float gamma, FBOColor *out ) {
 gammaShader.pTex0=source;
 gammaShader.gamma=gamma;
 out->Bind();
 out->QuadYInverted(&gammaShader);
 out->Unbind();
}