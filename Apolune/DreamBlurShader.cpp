#include "DreamBlurShader.h"


DreamBlurShader dreamblurShader;

DreamBlurRGBShader dreamblurrgbShader;


void ShadedDreamBlur( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 dreamblurShader.pTex0=in;
 out->QuadYInverted(&dreamblurShader);
 out->Unbind();
}

void ShadedDreamBlurRGB( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 dreamblurrgbShader.pTex0=in;
 out->QuadYInverted(&dreamblurrgbShader);
 out->Unbind();
}