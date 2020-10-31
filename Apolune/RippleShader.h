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

void ShadedRipple( GLuint *in, FBOColor *out );

class RippleShader : public GLSLShader {
public:
 float time;
 float scale;
 float resolution[2];
 GLuint *pTex0;

  // Uniform locations
 GLint Utime, Uscale, Uresolution, Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  scale=12.0f;
  pTex0=null;
  OUTPUT("RippleShader::Loading\n");
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
   string("uniform vec2 resolution; // Screen resolution\n")+
   string("uniform float time; // time in seconds\n")+
   string("uniform float scale; // scale (default: 12.0)\n")+
   string("uniform sampler2D tex0; // thing to ripple\n")+
   string("void main(void)\n")+
   string("{\n")+
   string(" vec2 tc = Texcoord.xy;\n")+
   string(" vec2 p = -1.0 + 2.0 * tc;\n")+
   string(" float len = length(p);\n")+
   string(" vec2 uv = tc + (p/len)*cos(len*scale-time*4.0)*0.03;\n")+
   string(" vec3 col = texture2D(tex0,uv).xyz;\n")+
   string(" outColor = vec4(col,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Uscale=glGetUniformLocation(program,"scale");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program); 
  glUniform1f(Utime, time);
  glUniform1f(Uscale, scale);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0); glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern RippleShader rippleShader;