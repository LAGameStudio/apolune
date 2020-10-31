#include "PixelateShader.h"

PixelateShader pixelateShader;

void ShadedPixelate( FBOColor *out, GLuint *source, float sourceW, float sourceH ) {
 out->Bind();
 pixelateShader.pTex0=source;
 pixelateShader.resolution[0]=sourceW;
 pixelateShader.resolution[1]=sourceH;
// pixelateShader.pixel[0]=scalex;
// pixelateShader.pixel[1]=scaley;
 out->QuadYInverted(&pixelateShader);
 out->Unbind();
}

CoverageShader coverageShader;

void CoverageShader::Render( FBOColor *out, CoverageProfile *in ) {
 out->Bind();
 coverageShader.Set(in);
 out->QuadYInverted(&coverageShader);
 out->Unbind();
}

void ShadedCoverage( FBOColor *out, CoverageProfile *in ) {
 out->Bind();
 coverageShader.Set(in);
 out->QuadYInverted(&coverageShader);
 out->Unbind();
}

void ShadedCoverage( FBOColor *out, CoverageProfile *in, FBOColor *in2 ) {
 out->Bind();
 coverageShader.Set(in2,in);
 out->QuadYInverted(&coverageShader);
 out->Unbind();
}

HQ4XShader hq4xShader;

void HQ4XShader::Render( FBOColor *out, HQ4XProfile *in ) {
 out->Bind();
 hq4xShader.Set(in);
 out->QuadYInverted(&hq4xShader);
 out->Unbind();
}

void ShadedHQ4X( FBOColor *out, HQ4XProfile *in ) {
 out->Bind();
 hq4xShader.Set(in);
 out->QuadYInverted(&hq4xShader);
 out->Unbind();
}

void ShadedHQ4X( FBOColor *in, FBOColor *out ) {
 out->Bind();
 hq4xShader.tex=&in->texture;
 out->QuadYInverted(&hq4xShader);
 out->Unbind();
}

void ShadedHQ4X( GLImage *in, FBOColor *out ) {
 out->Bind();
 hq4xShader.tex=&in->texture;
 hq4xShader.resolution.Set(in->width,in->height);
 out->QuadYInverted(&hq4xShader);
 out->Unbind();
}