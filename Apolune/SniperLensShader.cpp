#include "SniperLensShader.h"

SniperLensShader sniperlensShader;

void ShadedSniperLens( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 sniperlensShader.pTex0=in;
 out->QuadYInverted(&sniperlensShader);
 out->Unbind();
}