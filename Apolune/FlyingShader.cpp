#include "FlyingShader.h"

FlyingShader flyingShader;

void ShadedFlying( GLuint *in, GLuint *in2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 flyingShader.pTex0=in;
 flyingShader.pTex1=in2;
 out->QuadYInverted(&flyingShader);
 out->Unbind();
}