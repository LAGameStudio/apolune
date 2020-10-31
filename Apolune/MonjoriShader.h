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

void ShadedMonjori( FBOColor *out );

class MonjoriShader : public GLSLShader {
public:
 float rgbFactor1[3],rgbFactor2[3],rgbFactor3[3];
 float position[2];
 float resolution[2];
 float time;

  // Uniform locations
 GLint Uposition, Uresolution, Utime, UrgbFactor1, UrgbFactor2, UrgbFactor3;

 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  rgbFactor1[0]=1.6f;
  rgbFactor1[1]=2.0f;
  rgbFactor1[2]=1.0f;
  rgbFactor2[0]=1.3f;
  rgbFactor2[1]=2.0f;
  rgbFactor2[2]=1.0f;
  rgbFactor3[0]=13.0f;
  rgbFactor3[1]=8.0f;
  rgbFactor3[2]=18.0f;
  position[0]=512.0f;
  position[1]=512.0f;
  resolution[0]=256.0f;
  resolution[1]=256.0f;
  time=0.0f;
  OUTPUT("MonjoriShader::Loading\n");
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
   string("uniform vec3 rgbFactor1;\n")+
   string("uniform vec3 rgbFactor2;\n")+
   string("uniform vec3 rgbFactor3;\n")+
   string("uniform vec2 pos;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform float time;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string("    vec2 p = -1.0 + 2.0 * ((pos.xy / resolution.xy) + Texcoord);\n")+
   string("    float a = time*40.0;\n")+
   string("    float d,e,f,g=1.0/40.0,h,i,r,q;\n")+
   string("    e=400.0*(p.x*0.5+0.5);\n")+
   string("    f=400.0*(p.y*0.5+0.5);\n")+
   string("    i=200.0+sin(e*g+a/150.0)*20.0;\n")+
   string("    d=200.0+cos(f*g/2.0)*18.0+cos(e*g)*7.0;\n")+
   string("    r=sqrt(pow(i-e,2.0)+pow(d-f,2.0));\n")+
   string("    q=f/r;\n")+
   string("    e=(r*cos(q))-a/2.0;f=(r*sin(q))-a/2.0;\n")+
   string("    d=sin(e*g)*176.0+sin(e*g)*164.0+r;\n")+
   string("    h=((f+d)+a/2.0)*g;\n")+
   string("    i=cos(h+r*p.x/1.3)*(e+e+a)+cos(q*g*6.0)*(r+h/3.0);\n")+
   string("    h=sin(f*g)*144.0-sin(e*g)*212.0*p.x;\n")+
   string("    h=(h+(f-e)*q+sin(r-(a+h)/7.0)*10.0+i/4.0)*g;\n")+
   string("    i+=cos(h*2.3*sin(a/350.0-q))*184.0*sin(q-(r*4.3+a/12.0)*g)+tan(r*g+h)*184.0*cos(r*g+h);\n")+
   string("    i=mod(i/5.6,256.0)/64.0;\n")+
   string("    if(i<0.0) i+=4.0;\n")+
   string("    if(i>=2.0) i=4.0-i;\n")+
   string("    d=r/350.0;\n")+
   string("    d+=sin(d*d*8.0)*0.52;\n")+
   string("    f=(sin(a*g)+1.0)/2.0;\n")+
   string("    outColor=vec4(\n")+
   string("      vec3(f*i/rgbFactor1.x,             i/rgbFactor1.y+d/rgbFactor3.y, i*rgbFactor1.z)*d*p.x\n")+
   string("	 +vec3(i/rgbFactor2.x+d/rgbFactor3.x,i/rgbFactor2.y+d/rgbFactor3.z, i*rgbFactor2.z)*d*(1.0-p.x),1.0);\n")+
   string("    outColor=vec4(vec3(f*i/1.6,i/2.0+d/13.0,i)*d*p.x+vec3(i/1.3+d/8.0,i/2.0+d/18.0,i)*d*(1.0-p.x),1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  UrgbFactor1=glGetUniformLocation(program,"rgbFactor1");
  UrgbFactor2=glGetUniformLocation(program,"rgbFactor2");
  UrgbFactor3=glGetUniformLocation(program,"rgbFactor3");
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
  glUniform3f(UrgbFactor1, rgbFactor1[0], rgbFactor1[1], rgbFactor1[2]);
  glUniform3f(UrgbFactor2, rgbFactor2[0], rgbFactor2[1], rgbFactor2[2]);
  glUniform3f(UrgbFactor3, rgbFactor3[0], rgbFactor3[1], rgbFactor3[2]);
  glUniform2f(Uposition, position[0], position[1]);
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

extern MonjoriShader monjoriShader;