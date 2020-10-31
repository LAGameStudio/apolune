#include "ImplodeShader.h"

ImplodeShader implodeShader;

void ShadedImplode(GLuint *in, FBOColor *out) {
 out->background.Pick(clear);
 out->Bind();
 implodeShader.pTex0=in;
 out->QuadYInverted(&implodeShader);
 out->Unbind();
}