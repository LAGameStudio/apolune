#include "DeformerShader.h"

DeformerShader deformerShader;

void ShadedDeformer( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 deformerShader.pTex0=in;
 out->QuadYInverted(&deformerShader);
 out->Unbind();
}