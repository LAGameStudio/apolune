#include "SwirlShader.h"

SwirlShader swirlShader;

void ShadedSwirl( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 swirlShader.pTex0=in;
 out->QuadYInverted(&swirlShader);
 out->Unbind();
}