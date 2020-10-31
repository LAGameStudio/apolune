#include "CrosshatchShader.h"

CrosshatchShader crosshatchShader;

void ShadedCrosshatch( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 crosshatchShader.pTex0=in;
 out->QuadYInverted(&crosshatchShader);
 out->Unbind();
}
