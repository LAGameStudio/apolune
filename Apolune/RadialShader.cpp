#include "RadialShader.h"

RadialShader radialShader;

void ShadedRadial( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 radialShader.pTex0=in;
 out->QuadYInverted(&radialShader);
 out->Unbind();
}