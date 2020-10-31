#include "MotionShader.h"

MotionShader motionShader;

void ShadedMotion( GLuint *in, GLuint *in2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 motionShader.pTex0=in;
 motionShader.pTex1=in2;
 out->QuadYInverted(&motionShader);
 out->Unbind();
}