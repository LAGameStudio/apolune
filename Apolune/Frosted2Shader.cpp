#include "Frosted2Shader.h"

Frosted2Shader frosted2Shader;

void SimpleFrosted2( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 frosted2Shader.pTex0=in;
 out->QuadYInverted(&frosted2Shader);
 out->Unbind();
}