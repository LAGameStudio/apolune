/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

class MVBO;
void ShadedColor2d( Crayon color, MVBO *v, double x, double y, double w, double h, double resx, double resy );

class Color2dShader : public GLSLShader {
public:
 Crayon color;
 GLuint Ucolor,Uresolution,Uxywh;
 GLint posAttrib,texAttrib;
 Cartesiand rect,resolution;
 void Init() {
  OUTPUT("Color2dShader::Loading\n");
  /*
  string vert= string(
   "#version 150 core\n"
   "in vec2 position;"
   "void main() {"
   "    gl_Position = vec4(position, 0.0, 1.0);"
   "}"
  );
  */
  string vert=
   string("#version 120\n")+
   string("uniform vec4 xywh;")+
   string("uniform vec2 resolution;")+
   string("void main() {\n")+
   string(" vec2 t=1.0-gl_Vertex.xy;\n")+ // Invert because on an FBO it's upside down
//   string(" t=vec2(t.x*xywh.z,t.y*xywh.w);\n")+
//   string(" t=t/(resolution/8.0);\n")+
   string(" vec2 pixSize=1.0/resolution;\n")+
   string(" vec2 rectSize=pixSize*xywh.zw;\n")+
   string(" t=t+vec2(-0.5,-0.5);\n")+ // negative y moves us down, negative x moves us left
   string(" t*=2.0;\n")+ // now we're centered, full screen
   string(" t*=rectSize;\n")+  // now we're the right size
   string(" t+=vec2(-(1.0-xywh.z/resolution.x),(1.0-(xywh.w/resolution.y)));\n")+ // moving it to the left and up by 1.0-relativeh
//   string(" t+=vec2(pixSize.x*xywh.x,-pixSize.y*xywh.y);\n")+ //
   string(" t+=vec2(xywh.x/resolution.x,-xywh.y/resolution.y)*2.0;\n")+ 
//   string(" t+=vec2(rectPosition.x,-rectPosition.y);\n")+ //-rectPosition.y);\n")+
   string("	gl_Position = vec4(t,0.0,1.0);\n")+
   string("}\n")
  ;
  string frag= string(
   "#version 150 core\n"
   "out vec4 outColor;"
   "uniform vec4 color;"
   "void main() {"
   " outColor = color;"
   "}"
  );
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uresolution=glGetUniformLocation(program, "resolution");
  Ucolor=glGetUniformLocation(program, "color");
  Uxywh=glGetUniformLocation(program, "xywh");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);
  glUniform4f(Uxywh,(GLfloat)rect.x,(GLfloat)rect.y,(GLfloat)rect.w,(GLfloat)rect.h);
  glUniform2f(Uresolution,(GLfloat)resolution.x,(GLfloat)resolution.y);
 }
 void Disable() {
  glUseProgram(0);
 }
};

extern Color2dShader color2dShader;