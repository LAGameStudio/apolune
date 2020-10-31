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

void ShadedGlasscave( FBOColor *out );

class GlasscaveShader : public GLSLShader {
public:
 float position[3];
 float resolution[2];
 float time;

  // Uniform locations
 GLint Uposition, Uresolution, Utime;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  position[0]=512.0f;
  position[1]=512.0f;
  position[2]=0.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  time=0.0f;
  OUTPUT("GlasscaveShader::Loading\n");
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
   string("uniform float time;\n")+
   string("//Object A (tunnel)\n")+
   string("float oa(vec3 q)\n")+
   string("{\n")+
   string(" return cos(q.x)+cos(q.y*1.5)+cos(q.z)+cos(q.y*20.)*.05;\n")+
   string("}\n")+
   string("//Object B (ribbon)\n")+
   string("float ob(vec3 q) {\n")+
   string(" return length(max(abs(q-vec3(cos(q.z*1.5)*.3,-.5+cos(q.z)*.2,.0))-vec3(.125,.02,time+3.),vec3(.0)));\n")+
   string("}\n")+
   string("//Scene\n")+
   string("float o(vec3 q) { return oa(q); //min(oa(q),ob(q)); \n")+
   string("}\n")+
   string("//Get Normal\n")+
   string("vec3 gn(vec3 q) {\n")+
   string(" vec3 f=vec3(.01,0,0);\n")+
   string(" return normalize(vec3(o(q+f.xyy),o(q+f.yxy),o(q+f.yyx)));\n")+
   string("}\n")+
   string("//MainLoop\n")+
   string("void main(void)\n")+
   string("{\n")+
   string(" vec2 p = -1.0 + 2.0 * ((pos.xy / resolution.xy) + Texcoord);\n")+
   string(" p.x *= resolution.x/resolution.y;\n")+
   string(" vec4 c=vec4(1.0);\n")+
   string(" vec3 org=vec3(sin(time)*.5,cos(time*.5)*.25+.25,0),dir=normalize(vec3(p.x*1.6,p.y,1.0)),q=org,pp;\n")+
   string(" float d=.0;\n")+
   string(" //First raymarching\n")+
   string(" for(int i=0;i<64;i++)\n")+
   string(" {\n")+
   string("  d=o(q);\n")+
   string("  q+=d*dir;\n")+
   string(" }\n")+
   string(" pp=q;\n")+
   string(" float f=length(q-org)*0.02;\n")+
   string(" //Second raymarching (reflection)\n")+
   string(" dir=reflect(dir,gn(q));\n")+
   string(" q+=dir;\n")+
   string(" for(int i=0;i<64;i++)\n")+
   string(" {\n")+
   string(" d=o(q);\n")+
   string(" q+=d*dir;\n")+
   string(" }\n")+
   string(" c=max(dot(gn(q),vec3(.1,.1,.0)),.0)+vec4(.3,cos(time*.5)*.5+.5,sin(time*.5)*.5+.5,1.)*min(length(q-org)*.04,1.);\n")+
   string(" //Ribbon Color\n")+
   string("// if(oa(pp)>ob(pp))c=mix(c,vec4(cos(time*.3)*.5+.5,cos(time*.2)*.5+.5,sin(time*.3)*.5+.5,1.),.3);\n")+
   string(" //Final Color\n")+
   string(" vec4 fcolor = ((c+vec4(f))+(1.-min(pp.y+1.9,1.))*vec4(1.,.8,.7,1.))*min(time*.5,1.);\n")+
   string(" outColor=vec4(fcolor.xyz,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uposition=glGetUniformLocation(program,"pos");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utime=glGetUniformLocation(program,"time");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform3f(Uposition, position[0], position[1], position[2]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
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

extern GlasscaveShader glasscaveShader;