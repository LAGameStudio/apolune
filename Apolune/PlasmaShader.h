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

void ShadedPlasma( FBOColor *out );

class PlasmaShader : public GLSLShader {
public:
 float position[2];
 float resolution[2];
 float scale[2];
 float factor[4];
 float factor2[2];
 float time;

  // Uniform locations
 GLint Uposition, Uresolution, Utime, Uscale, Ufactor, Ufactor2;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Reset() {
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  scale[0]=0.2f;
  scale[1]=0.2f;
  factor[0]=1000.0f;
  factor[1]=100.0f;
  factor[2]=40.0f;
  factor[3]=2.0f;
  factor2[0]=3.0f;
  factor2[1]=2.0f;
  time=0.0f;
 }

 void Init() {
  Uposition=Uresolution=Utime=Uscale=Ufactor=Ufactor2=0;
  Reset();
  OUTPUT("PlasmaShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("void main() {\n")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("uniform vec2 pos;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform vec2 scale;\n")+
   string("uniform vec4 factor;\n")+
   string("uniform vec2 factor2;\n")+
   string("uniform float time;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("   float x = pos.x/resolution.x+Texcoord.x;\n")+
   string("   float y = pos.y/resolution.y+Texcoord.y;\n")+
   string("   float mov0 = x+y+cos(sin(time)*factor.w)*factor2.y+sin(x/factor.y)*factor.x;\n")+
   string("   float mov1 = y / resolution.y / scale.y + time;\n")+
   string("   float mov2 = x / resolution.x / scale.x;\n")+
   string("   float c1 = abs(sin(mov1+time)/factor.w+mov2/factor.w-mov1-mov2+time);\n")+
   string("   float c2 = abs(sin(c1+sin(mov0/factor.x+time)+sin(y/factor.z+time)+sin((x+y)/factor.y)*factor2.x));\n")+
   string("   float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/factor.x)));\n")+
   string("   outColor = vec4( c1,c2,c3,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uniform(&Uposition,"pos");
  Uniform(&Uresolution,"resolution");
  Uniform(&Utime,"time");
  Uniform(&Uscale,"scale");
  Uniform(&Ufactor,"factor");
  Uniform(&Ufactor2,"factor2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform1f(Utime, time);
  glUniform2f(Uscale, scale[0], scale[1]);
  glUniform4f(Ufactor, factor[0], factor[1], factor[2], factor[3]);
  glUniform2f(Ufactor2, factor2[0], factor2[1]);

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern PlasmaShader plasmaShader;