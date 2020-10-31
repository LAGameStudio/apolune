#include "MonjoriShader.h"

MonjoriShader monjoriShader;

void ShadedMonjori( FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 out->QuadYInverted(&monjoriShader);
 out->Unbind();
}