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

void ShadedDreamBlur( GLuint *in, FBOColor *out );
void ShadedDreamBlurRGB( GLuint *in, FBOColor *out );

class DreamBlurShader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0;
 GLint Ucolor,Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  color.Pick(alabaster);
  pTex0=null;
  OUTPUT("DreamBlurShader::Loading\n");
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
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform vec4 color;\n")+
   string("void main ()\n")+
   string("{\n")+
   string("  vec2 uv = Texcoord.xy;\n")+
   string("  vec4 c = texture2D(sceneTex, uv);\n")+
   string("  c += texture2D(sceneTex, uv+0.001);\n")+
   string("  c += texture2D(sceneTex, uv+0.003);\n")+
   string("  c += texture2D(sceneTex, uv+0.005);\n")+
   string("  c += texture2D(sceneTex, uv+0.007);\n")+
   string("  c += texture2D(sceneTex, uv+0.009);\n")+
   string("  c += texture2D(sceneTex, uv+0.011);\n")+
   string("  c += texture2D(sceneTex, uv-0.001);\n")+
   string("  c += texture2D(sceneTex, uv-0.003);\n")+
   string("  c += texture2D(sceneTex, uv-0.005);\n")+
   string("  c += texture2D(sceneTex, uv-0.007);\n")+
   string("  c += texture2D(sceneTex, uv-0.009);\n")+
   string("  c += texture2D(sceneTex, uv-0.011);\n")+
   string("  c.rgb = vec3((c.r+c.g+c.b)/3.0);\n")+
   string("  c = c / 9.5;\n")+
   string("  outColor = c*color;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"sceneTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform4f(Ucolor, color.rf, color.gf, color.bf, color.af);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern DreamBlurShader dreamblurShader;

class DreamBlurRGBShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLint Ucolor,Utex0,Ua,Ub,Uc,Ud,Ue,Uf,Ufactor,Uoffset;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 float a,b,c,d,e,f,factor;
 float offset[2];
 void Init() {
  a=0.001f;
  b=0.003f;
  c=0.005f;
  d=0.007f;
  e=0.009f;
  f=0.011f;
  factor=9.5f;
  offset[0]=0.0f;//16.0f;
  offset[1]=0.0f;//-16.0f;
  pTex0=null;
  OUTPUT("DreamBlurRGBShader::Loading\n");
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
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform float a,b,c,d,e,f,factor;\n")+
   string("uniform vec2 offset;\n")+
   string("void main () {\n")+
   string("  vec2 uv = Texcoord.xy+offset.xy;\n")+
   string("  vec4 color = texture2D(sceneTex, uv);\n")+
   string("  color += texture2D(sceneTex, uv+a);\n")+
   string("  color += texture2D(sceneTex, uv+b);\n")+
   string("  color += texture2D(sceneTex, uv+c);\n")+
   string("  color += texture2D(sceneTex, uv+d);\n")+
   string("  color += texture2D(sceneTex, uv+e);\n")+
   string("  color += texture2D(sceneTex, uv+f);\n")+
   string("  color += texture2D(sceneTex, uv-a);\n")+
   string("  color += texture2D(sceneTex, uv-b);\n")+
   string("  color += texture2D(sceneTex, uv-c);\n")+
   string("  color += texture2D(sceneTex, uv-d);\n")+
   string("  color += texture2D(sceneTex, uv-e);\n")+
   string("  color += texture2D(sceneTex, uv-f);\n")+
   string("  outColor = color / vec4(factor);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"sceneTex");
  Ua=glGetUniformLocation(program,"a");
  Ub=glGetUniformLocation(program,"b");
  Uc=glGetUniformLocation(program,"c");
  Ud=glGetUniformLocation(program,"d");
  Ue=glGetUniformLocation(program,"e");
  Uf=glGetUniformLocation(program,"f");
  Ufactor=glGetUniformLocation(program,"factor");
  Uoffset=glGetUniformLocation(program,"offset");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ua,a);
  glUniform1f(Ub,b);
  glUniform1f(Uc,c);
  glUniform1f(Ud,d);
  glUniform1f(Ue,e);
  glUniform1f(Uf,f);
  glUniform1f(Ufactor,factor);
  glUniform2fv(Uoffset,1,offset);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern DreamBlurRGBShader dreamblurrgbShader;