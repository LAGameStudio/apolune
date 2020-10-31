#include "ColorShader.h"

#include "VBO.h"

Color2dShader color2dShader;

void ShadedColor2d( Crayon color, MVBO *v, double x, double y, double w, double h, double resx, double resy ) {
 color2dShader.color.fromCrayon(color);
 color2dShader.rect.SetRect(x,y,w,h);
 color2dShader.resolution.Set(resx,resy);
 color2dShader.UpdateUniforms();
 v->Render();
 color2dShader.Disable();
}