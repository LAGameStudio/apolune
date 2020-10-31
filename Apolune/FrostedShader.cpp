#include "FrostedShader.h"

FrostedShader frostedShader;
SimpleFrostShader simplefrostShader;

void ShadedFrosted( GLuint *in, GLuint *noise, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 frostedShader.pTex0=in;
 frostedShader.pTex1=noise;
 out->QuadYInverted(&frostedShader);
 out->Unbind();
}

void ShadedFrostedSimple( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 simplefrostShader.pTex0=in;
 out->QuadYInverted(&simplefrostShader);
 out->Unbind();
}