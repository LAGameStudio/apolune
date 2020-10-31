#include "LuxShader.h"

LuxShader luxShader;

void ShadedLux( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&luxShader);
 out->Unbind();
}