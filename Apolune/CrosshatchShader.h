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

void ShadedCrosshatch( GLuint *in, FBOColor *out );

class CrosshatchShader : public GLSLShader {
public:
 float offset;
 float hatchsize;
 Crayon luminance;
 GLuint *pTex0;
 GLuint Uoffset, Uhatchsize, Ulum, Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  offset=0.0f;
  hatchsize=5.0f;
  luminance.Float(1.0f,0.7f,0.5f,0.3f);
  pTex0=null;
  OUTPUT("CrosshatchShader::Loading\n");
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
   string("uniform float vx_offset;\n")+
   string("uniform float hatch_y_offset; // 5.0\n")+
   string("uniform vec4 lumT;   // 1.0  0.7  0.5 0.3\n")+
   string("void main()\n")+
   string("{\n")+
   string("  vec2 uv = Texcoord;\n")+
   string("  vec3 tc = vec3(1.0, 0.0, 0.0);\n")+
   string("  if (uv.x < (vx_offset-0.005))\n")+
   string("  {\n")+
   string("    float lum = length(texture2D(sceneTex, uv).rgb);\n")+
   string("    tc = vec3(1.0, 1.0, 1.0);\n")+
   string("    if (lum < lumT.x)\n")+
   string("    {\n")+
   string("      if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)\n")+
   string("        tc = vec3(0.0, 0.0, 0.0);\n")+
   string("    }\n")+
   string("    if (lum < lumT.y)\n")+
   string("    {\n")+
   string("      if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)\n")+
   string("        tc = vec3(0.0, 0.0, 0.0);\n")+
   string("    }\n")+
   string("    if (lum < lumT.z)\n")+
   string("    {\n")+
   string("      if (mod(gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)\n")+
   string("        tc = vec3(0.0, 0.0, 0.0);\n")+
   string("    }\n")+
   string("    if (lum < lumT.w)\n")+
   string("    {\n")+
   string("      if (mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)\n")+
   string("        tc = vec3(0.0, 0.0, 0.0);\n")+
   string("    }\n")+
   string("  }\n")+
   string("  else if (uv.x>=(vx_offset+0.005))\n")+
   string("  {\n")+
   string("    tc = texture2D(sceneTex, uv).rgb;\n")+
   string("  }\n")+
   string("  outColor = vec4(tc, 1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uoffset=glGetUniformLocation(program,"vx_offset");
  Uhatchsize=glGetUniformLocation(program,"hatch_y_offset");
  Ulum=glGetUniformLocation(program,"lumT");
  Utex0=glGetUniformLocation(program,"sceneTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
 
  glUniform1f(Uoffset, offset);
  glUniform1f(Uhatchsize, hatchsize);
  glUniform4f(Ulum, luminance.rf, luminance.gf, luminance.bf, luminance.af);

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

extern CrosshatchShader crosshatchShader;