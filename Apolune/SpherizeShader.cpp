#include "SpherizeShader.h"

SpherizeShader spherizeShader;
SpherizeShader2 spherizeShader2;


void ShadedSpherize( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 spherizeShader.pTex0=in;
 out->QuadYInverted(&spherizeShader);
 out->Unbind();
}


void ShadedSpherize2( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 spherizeShader2.pTex0=in;
 out->QuadYInverted(&spherizeShader2);
 out->Unbind();
}