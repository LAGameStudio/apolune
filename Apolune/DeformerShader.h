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

void ShadedDeformer( GLuint *in, FBOColor *out );

class DeformerShader : public GLSLShader {
public:
 float rgbFactor[3];
 float mouse[2]; // normalized 0-1
 float time;
 float position[2];
 float resolution[2];
 float textureOffset[2];
 GLuint *pTex0;
  // Uniform locations
 GLuint UrgbFactor, Uposition, Uresolution, Umouse, Utime, UtextureOffset, Utex0, Utex1;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  rgbFactor[0]=1.0f;
  rgbFactor[1]=1.0f;
  rgbFactor[2]=1.0f;
  position[0]=display.w2f;
  position[1]=display.h2f;
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  textureOffset[0]=0.0f;
  textureOffset[1]=0.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("DeformerShader::Loading\n");
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
   string("// Plane Deformer\n")+
   string("uniform vec3 rgbFactor;\n")+
   string("uniform vec2 mouse;\n")+
   string("uniform float time;\n")+
   string("uniform vec2 textureOffset;\n")+
   string("uniform vec2 pos;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 p = -1.0 + 2.0 * ((pos.xy / resolution.xy) + Texcoord);\n")+
   string("    vec2 m = -1.0 + 2.0 * (mouse.xy / resolution.xy);\n")+
   string("    float a1 = atan(p.y-m.y,p.x-m.x);\n")+
   string("    float r1 = sqrt(dot(p-m,p-m));\n")+
   string("    float a2 = atan(p.y+m.y,p.x+m.x);\n")+
   string("    float r2 = sqrt(dot(p+m,p+m));\n")+
   string("    vec2 uv;\n")+
   string("    uv.x = 0.2*time + (r1-r2)*0.25;\n")+
   string("    uv.y = sin(2.0*(a1-a2));\n")+
   string("    float w = r1*r2*0.8;\n")+
   string("    vec3 col = texture2D(tex0,uv+textureOffset).xyz;\n")+
   string("    outColor = vec4(rgbFactor*vec3(col/(.1+w)),1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  UrgbFactor=glGetUniformLocation(program,"rgbFactor");
  Utime=glGetUniformLocation(program,"time");
  Uposition=glGetUniformLocation(program,"position");
  Uresolution=glGetUniformLocation(program,"resolution");
  Umouse=glGetUniformLocation(program,"mouse");
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  UtextureOffset=glGetUniformLocation(program, "textureOffset");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UrgbFactor, rgbFactor[0], rgbFactor[1], rgbFactor[2]);
  glUniform1f(Utime, time);
  glUniform2f(Umouse, mouse[0], mouse[1]);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform2f(UtextureOffset, textureOffset[0], textureOffset[1]);

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

extern DeformerShader deformerShader;