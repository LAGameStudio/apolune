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

void ShadedLux( FBOColor *out );

class LuxShader : public GLSLShader {
public:
 float position[2];
 float resolution[2];
 float rgbFactor[3];
 float time;

  // Uniform locations
 GLuint UrgbFactor, Utime, Uresolution, Uposition;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  rgbFactor[0]=4.0f;
  rgbFactor[1]=1.0f;
  rgbFactor[2]=2.0f;
  time=0.0f;
  OUTPUT("LuxShader::Loading\n");
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
   string("uniform float time;\n")+
   string("uniform vec3 rgbFactor;\n")+
   string("uniform vec2 position;\n")+
   string("uniform vec2 resolution;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 Pos = (pos.xy/resolution.xy)+Texcoord.xy;\n")+
   string("    vec3 s[4];\n")+
   string("    s[0]=vec3(0);\n")+
   string("    s[3]=vec3(sin(time),cos(time),0);\n")+
   string("    s[1]=s[3].zxy;\n")+
   string("    s[2]=s[3].zzx;\n")+
   string("    float t,b,c,h=0.0;\n")+
   string("    vec3 m=vec3(0),n;\n")+
   string("    vec3 p=vec3(.2);\n")+
   string("    vec3 d=normalize(.001*vec3(Pos.xy,0.0)-p);\n")+
   string("    for(int i=0;i<4;i++)\n")+
   string("    {\n")+
   string("        t=2.0;\n")+
   string("        for(int i=0;i<4;i++)\n")+
   string("        {\n")+
   string("            b=dot(d,n=s[i]-p);\n")+
   string("            c=b*b+.2-dot(n,n);\n")+
   string("            if(b-c<t)\n")+
   string("            if(c>0.0)\n")+
   string("            {\n")+
   string("                m=s[i];t=b-c;\n")+
   string("            }\n")+
   string("        }\n")+
   string("    p+=t*d;\n")+
   string("    d=reflect(d,n=normalize(p-m));\n")+
   string("    h+=pow(n.x*n.x,44.)+n.x*n.x*.2;\n")+
   string("    }\n")+
   string("    outColor=vec4(pow(h,rgbFactor.x),pow(h,rgbFactor.y),pow(h,rgbFactor.z),1);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uposition=glGetUniformLocation(program,"pos");
  Uresolution=glGetUniformLocation(program,"resolution");
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  UrgbFactor=glGetUniformLocation(program,"rgbFactor");
  Utime=glGetUniformLocation(program,"time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform2f(Uposition, position[0], position[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform3f(UrgbFactor, rgbFactor[0], rgbFactor[1], rgbFactor[2]);
  glUniform1f(Utime,       time);

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

