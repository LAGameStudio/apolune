#include "TunnelShader.h"

TunnelShader tunnelShader;

void ShadedTunnel( GLuint *in, GLuint *in2, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 tunnelShader.pTex0=in;
 out->QuadYInverted(&tunnelShader);
 out->Unbind();
}
