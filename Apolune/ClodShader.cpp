#include "ClodShader.h"

ClodShader clodShader;

void ShadedClod( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&clodShader);
 out->Unbind();
}