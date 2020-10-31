#include "ShockwaveShader.h"

ShockwaveShader shockwaveShader;

void ShadedShockwave( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 shockwaveShader.pTex0=in;
 out->QuadYInverted(&shockwaveShader);
 out->Unbind();
}