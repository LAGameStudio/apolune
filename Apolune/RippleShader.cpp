#include "RippleShader.h"

RippleShader rippleShader;

void ShadedRipple( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 rippleShader.pTex0=in;
 out->QuadYInverted(&rippleShader);
 out->Unbind();
}