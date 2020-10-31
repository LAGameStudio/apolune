#include "NightvisionShader.h"

NightvisionShader nightvisionShader;

void ShadedNightVision( GLuint *in, GLuint *noise, GLuint *mask, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 nightvisionShader.pTex0=in;
 nightvisionShader.pTex1=noise;
 nightvisionShader.pTex2=mask;
 out->QuadYInverted(&nightvisionShader);
 out->Unbind();
}