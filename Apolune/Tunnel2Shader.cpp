#include "Tunnel2Shader.h"

Tunnel2Shader tunnel2Shader;

void ShadedTunnel2( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 tunnel2Shader.pTex0=in;
 out->QuadYInverted(&tunnel2Shader);
 out->Unbind();
}