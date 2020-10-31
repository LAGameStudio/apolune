#include "GlasscaveShader.h"

GlasscaveShader glasscaveShader;

void ShadedGlasscave( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&glasscaveShader);
 out->Unbind();
}