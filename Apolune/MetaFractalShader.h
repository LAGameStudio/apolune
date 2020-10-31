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

class MetaFractalShader
{
public:
 void Init() {
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D src_tex_unit0;\n")+
   string("uniform sampler2D src_tex_unit1;\n")+
   string("uniform sampler2D src_tex_unit2;\n")+
   string("uniform int numColors;\n")+
   string("uniform int MAX_PARTICLES;\n")+
   string("uniform int NUM_PARTICLES;\n")+
   string("float fmp = float(MAX_PARTICLES);\n")+
   string("float sumXY() {\n")+
   string("float sum = 0.0;\n")+
   string("for (int i=0; i<NUM_PARTICLES; i++) {\n")+
   string("	vec2 coordinate = vec2((float(i)+0.5)/fmp, 0.5);\n")+
   string("	vec4 p = texture2D(src_tex_unit1, coordinate).rgba;\n")+
   string("	sum += p.a * p.z / distance(p.xy, gl_FragCoord.xy);\n")+
   string("}\n")+
   string("	return sum;")+
   string("}\n")+
   string("void main() {\n")+
   string("	float s = sumXY();\n")+
   string(" vec4 offscreen_color = texture2D(src_tex_unit2, gl_TexCoord[2].st).rgba;\n")+
   string("	s *= smoothstep(0.0, 1.0, offscreen_color.r);\n")+
   string("	int colorSelect = int((float(numColors)-1.0)*clamp(s, 0.0, 1.0));\n")+
   string("	vec2 coordinate = vec2((float(colorSelect)+0.5)/float(numColors), 0.5);\n")+
   string("	gl_FragColor = texture2D(src_tex_unit0, coordinate).rgba;\n")+
   string("}\n")
  ;
 }
};

