#include "ASCIIShader.h"

ASCIIShader asciiShader;
P2xShader p2xShader;

void ASCIIShader::Render( FBOColor *out, ASCIIProfile *in ) {
 out->Bind();
 asciiShader.Set(in);
 out->QuadYInverted(&asciiShader);
 asciiShader.Disable();
 out->Unbind();
}

void ShadedASCII( FBOColor *out, ASCIIProfile *in ) {
 out->Bind();
 asciiShader.Set(in);
 out->QuadYInverted(&asciiShader);
 out->Unbind();
}

void ShadedASCII( FBOColor *out, ASCIIProfile *in, GLuint *input ) {
 out->Bind();
 asciiShader.Set(in);
 asciiShader.tex0=input;
 out->QuadYInverted(&asciiShader);
 out->Unbind();
}

void P2xShader::Render( FBOColor *out, P2xProfile *in ) {
 out->Bind();
 p2xShader.Set(in);
 out->QuadYInverted(&p2xShader);
 out->Unbind();
}

void ShadedP2x( FBOColor *out, P2xProfile *in ) {
 out->Bind();
 p2xShader.Set(in);
 out->QuadYInverted(&p2xShader);
 out->Unbind();
}

void ShadedP2x( FBOColor *out, P2xProfile *in, GLuint *input ) {
 out->Bind();
 p2xShader.Set(in);
 p2xShader.tex0=input;
 out->QuadYInverted(&p2xShader);
 out->Unbind();
}