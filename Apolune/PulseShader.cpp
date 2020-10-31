#include "PulseShader.h"

PulseShader pulseShader;

void ShadedPulse( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 pulseShader.pTex0=in;
 out->QuadYInverted(&pulseShader);
 out->Unbind();
}