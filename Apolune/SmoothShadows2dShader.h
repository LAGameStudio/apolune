/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

class SmoothShadows2dShader : public GLSLShader {
public:
 float resolution[2];
 void Init() {
  OUTPUT("SmoothShadows2dShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D walls;\n")+
   string("uniform sampler2D floors;\n")+
   string("vec4 HorizontalReductionPS(vec2 tc) {\n")+
   string(" vec2 color = texture2D(walls, tc);\n")+
   string(" vec2 colorR = texture2D(inputSampler, tc + vec2(resolution.x,0.0));\n")+
   string(" vec2 result = min(color,colorR);\n")+
   string(" return float4(result,0,1);\n")+
   string("}\n")+
   string("vec4 copy( vec2 tc ) { return texture2D(walls,tc); }\n")+
   string("void main() {\n")+
   string(" gl_FragColor = vec4(gl_FragCoord,1)+vec4(-resolution.x,resolution.y,0,0);\n")+
   string("}\n");
   ;

 }
};