#include "ConvolverShader.h"
#include "FBO.h"

EmbossShader embossShader;
EmbossConvolveAShader embossconvolveAShader;
EmbossPreserveAShader embosspreserveAShader;
GaussianBlurShader gaussianblurShader;
SharpnessShader sharpnessShader;
EdgeDetectShader edgedetectShader;
SigmaV9GaussianShader sigmaV9gaussianShader;
SigmaH9GaussianShader sigmaH9gaussianShader;
SigmaV7GaussianShader sigmaV7gaussianShader;
SigmaH7GaussianShader sigmaH7gaussianShader;
SigmaV5GaussianShader sigmaV5gaussianShader;
SigmaH5GaussianShader sigmaH5gaussianShader;
SigmaVGaussianShader sigmaVgaussianShader;
SigmaHGaussianShader sigmaHgaussianShader;
NormalMapShader normalmapShader;
NormalMapHeightShader normalmapheightShader;

void ShadedEmboss( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embossShader.pTex0=in;
 out->QuadYInverted(&embossShader);
 out->Unbind();
}

void ShadedEmbossConvolveA( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embossconvolveAShader.pTex0=in;
 out->QuadYInverted(&embossconvolveAShader);
 out->Unbind();
}

void ShadedEmbossPreserveA( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 embosspreserveAShader.pTex0=in;
 out->QuadYInverted(&embosspreserveAShader);
 out->Unbind();
}

void ShadedGaussian( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 gaussianblurShader.pTex0=in;
 out->QuadYInverted(&gaussianblurShader);
 out->Unbind();
}

void ShadedSharpen( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 sharpnessShader.pTex0=in;
 out->QuadYInverted(&sharpnessShader);
 out->Unbind();
}

void ShadedEdge( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 edgedetectShader.pTex0=in;
 out->QuadYInverted(&edgedetectShader);
 out->Unbind();
}

void ShadedSigma9Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV9gaussianShader.pTex0=in;
 work->QuadYInverted(&sigmaV9gaussianShader);
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH9gaussianShader.pTex0=&work->texture;
 out->QuadYInverted(&sigmaH9gaussianShader);
 out->Unbind();
}

void ShadedSigma7Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV7gaussianShader.pTex0=in;
 work->QuadYInverted(&sigmaV7gaussianShader);
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH7gaussianShader.pTex0=&work->texture;
 out->QuadYInverted(&sigmaH7gaussianShader);
 out->Unbind();
}

void ShadedSigma5Blur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaV5gaussianShader.pTex0=in;
 work->QuadYInverted(&sigmaV5gaussianShader);
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaH5gaussianShader.pTex0=&work->texture;
 out->QuadYInverted(&sigmaH5gaussianShader);
 out->Unbind();
}

void ShadedSigmaBlur( GLuint *in, FBOColor *work, FBOColor *out ) {
 work->background.Pick(clear);
 work->Bind();
 sigmaVgaussianShader.pTex0=in;
 work->QuadYInverted(&sigmaVgaussianShader);
 work->Unbind();
 out->background.Pick(clear);
 out->Bind();
 sigmaHgaussianShader.pTex0=&work->texture;
 out->QuadYInverted(&sigmaHgaussianShader);
 out->Unbind();
}