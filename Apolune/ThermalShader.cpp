#include "ThermalShader.h"

ThermalShader thermalShader;

void ShadedThermal( GLuint *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 thermalShader.pTex0=in;
 out->QuadYInverted(&thermalShader);
 out->Unbind();
}