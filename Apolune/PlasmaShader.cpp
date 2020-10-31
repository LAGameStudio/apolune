#include "PlasmaShader.h"

PlasmaShader plasmaShader;

void ShadedPlasma( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&plasmaShader);
 out->Unbind();
}