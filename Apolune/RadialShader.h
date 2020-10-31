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

void ShadedRadial( GLuint *in, FBOColor *out );

class RadialShader : public GLSLShader {
public:
 float rgbFactor[3];
 float mouse[2]; // normalized 0-1
 float time;
 float position[2];
 float resolution[2];
 GLuint *pTex0;

  // Uniform locations
 GLint UrgbFactor, Uposition, Uresolution, Umouse, Utime, Utex0, Utex1;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  rgbFactor[0]=1.0f;
  rgbFactor[1]=1.0f;
  rgbFactor[2]=1.0f;
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  pTex0=null;
  OUTPUT("RadialShader::Loading\n");
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
   string("uniform vec3 rgbFactor;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform vec2 mouse;\n")+
   string("uniform float time;\n")+
   string("uniform sampler2D tex0;\n")+
   string("vec3 deform( in vec2 p )\n")+
   string("{\n")+
   string("    vec2 uv;\n")+
   string("    vec2 q = vec2( sin(1.1*time+p.x),sin(1.2*time+p.y) );\n")+
   string("    float a = atan(q.y,q.x);\n")+
   string("    float r = sqrt(dot(q,q));\n")+
   string("    uv.x = sin(0.0+1.0*mouse.x)+p.x*sqrt(r*r+1.0);\n")+
   string("    uv.y = sin(0.6+1.1*mouse.y)+p.y*sqrt(r*r+1.0);\n")+
   string("    return texture2D(tex0,uv*.5).xyz;\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string("    vec2 p = -1.0 + 2.0 * ((position.xy / resolution.xy) + Texcoord.xy);\n")+
   string("    vec2 s = p;\n")+
   string("    vec3 total = vec3(0.0);\n")+
   string("    vec2 d = (vec2(0.0,0.0)-p)/40.0;\n")+
   string("    float w = 1.0;\n")+
   string("    for( int i=0; i<40; i++ ) {\n")+
   string("        vec3 res = deform(s);\n")+
   string("        res = smoothstep(0.1,1.0,res*res);\n")+
   string("        total += w*res;\n")+
   string("        w *= .99;\n")+
   string("        s += d;\n")+
   string("    }\n")+
   string("    total /= 40.0;\n")+
   string("    float r = 1.5/(1.0+dot(p,p));\n")+
   string("    outColor = vec4( vec3(total*r)*rgbFactor,1.0);\n")+
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

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern RadialShader radialShader;