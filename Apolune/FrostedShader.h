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

void ShadedFrosted( GLuint *in, GLuint *noise, FBOColor *out );
void ShadedFrostedSimple( GLuint *in, FBOColor *out );

class FrostedShader : public GLSLShader {
public:
 float pixel[2];
 float resolution[2];
 float frequency;
 float offset;
 GLuint *pTex0,*pTex1; // scene, noise
 GLint Upixel,Uresolution,Ufrequency,Uoffset,Utex0,Utex1;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pixel[0]=2.0f;
  pixel[1]=2.0f;
  resolution[0]=1024.0f;
  resolution[1]=1024.0f;
  frequency=0.115f;
  offset=64.0f;
  pTex0=pTex1=null;
  OUTPUT("FrostedShader::Loading\n");
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
   string("//sceneTex (sampler2D): the final scene image.\n")+
   string("//NoiseTex (sampler2D): noise texture.\n")+
   string("//vx_offset (float): x position of the vertical red line\n")+
   string("//PixelX (float): effect param, default value: 2.0\n")+
   string("//PixelY (float): effect param, default value: 2.0\n")+
   string("//Freq (float): effect param, default value: 0.0115\n")+
   string("uniform sampler2D sceneTex; // 0\n")+
   string("uniform sampler2D NoiseTex; // 1\n")+
   string("uniform float vx_offset;\n")+
   string("uniform vec2 pixel; // 2.0,2.0\n")+
   string("uniform float Freq; // = 0.115;\n")+
   string("uniform vec2 resolution;// render target w/h\n")+
   string("vec4 spline(float x, vec4 c1, vec4 c2, vec4 c3, vec4 c4, vec4 c5, vec4 c6, vec4 c7, vec4 c8, vec4 c9)\n")+
   string("{\n")+
   string("  float w1, w2, w3, w4, w5, w6, w7, w8, w9;\n")+
   string("  w1 = 0.0;  w2 = 0.0;  w3 = 0.0;  w4 = 0.0;  w5 = 0.0;  w6 = 0.0;  w7 = 0.0;  w8 = 0.0;  w9 = 0.0;\n")+
   string("  float tmp = x * 8.0;\n")+
   string("  if (tmp<=1.0) { w1 = 1.0 - tmp; w2 = tmp; }\n")+
   string("  else if (tmp<=2.0) { tmp = tmp - 1.0; w2 = 1.0-tmp; w3 = tmp; }\n")+
   string("  else if (tmp<=3.0) { tmp = tmp - 2.0; w3 = 1.0-tmp; w4 = tmp; }\n")+
   string("  else if (tmp<=4.0) { tmp = tmp - 3.0; w4 = 1.0-tmp; w5 = tmp; }\n")+
   string("  else if (tmp<=5.0) { tmp = tmp - 4.0; w5 = 1.0-tmp; w6 = tmp; }\n")+
   string("  else if (tmp<=6.0) { tmp = tmp - 5.0; w6 = 1.0-tmp; w7 = tmp; }\n")+
   string("  else if (tmp<=7.0) { tmp = tmp - 6.0; w7 = 1.0-tmp; w8 = tmp; }\n")+
   string("  else {\n")+
   string("    //tmp = saturate(tmp - 7.0);\n")+
   string("    // http://www.ozone3d.net/blogs/lab/20080709/saturate-function-in-glsl/ \n")+
   string("    tmp = clamp(tmp - 7.0, 0.0, 1.0);\n")+
   string("    w8 = 1.0-tmp;\n")+
   string("    w9 = tmp;\n")+
   string("  }\n")+
   string("  return w1*c1 + w2*c2 + w3*c3 + w4*c4 + w5*c5 + w6*c6 + w7*c7 + w8*c8 + w9*c9;\n")+
   string("}\n")+
   string("vec3 NOISE2D(vec2 p) { return texture2D(NoiseTex,p).xyz; }\n")+
   string("void main()\n")+
   string("{\n")+
   string("  vec2 uv = Texcoord.xy;\n")+
   string("  vec3 tc = vec3(1.0, 0.0, 0.0);\n")+
   string("  {\n")+
   string("    vec2 Delta = vec2(pixel.x/resolution.x,pixel.y/resolution.y);\n")+
   string("    vec2 ox = vec2(Delta.x,0.0);\n")+
   string("    vec2 oy = vec2(0.0,Delta.y);\n")+
   string("    vec2 PP = uv - oy;\n")+
   string("    vec4 C00 = texture2D(sceneTex,PP - ox);\n")+
   string("    vec4 C01 = texture2D(sceneTex,PP);\n")+
   string("    vec4 C02 = texture2D(sceneTex,PP + ox);\n")+
   string("    PP = uv;\n")+
   string("    vec4 C10 = texture2D(sceneTex,PP - ox);\n")+
   string("    vec4 C11 = texture2D(sceneTex,PP);\n")+
   string("    vec4 C12 = texture2D(sceneTex,PP + ox);\n")+
   string("    PP = uv + oy;\n")+
   string("    vec4 C20 = texture2D(sceneTex,PP - ox);\n")+
   string("    vec4 C21 = texture2D(sceneTex,PP);\n")+
   string("    vec4 C22 = texture2D(sceneTex,PP + ox);\n")+
   string("    float n = NOISE2D(Freq*uv).x;\n")+
   string("    n = mod(n, 0.111111)/0.111111;\n")+
   string("    vec4 result = spline(n,C00,C01,C02,C10,C11,C12,C20,C21,C22);\n")+
   string("    tc = result.rgb;\n")+
   string("  }\n")+
   string("  outColor = vec4(tc, 1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Upixel=glGetUniformLocation(program,"pixel");
  Uresolution=glGetUniformLocation(program,"resolution");
  Ufrequency=glGetUniformLocation(program,"Freq");
  Uoffset=glGetUniformLocation(program,"vx_offset");
  Utex0=glGetUniformLocation(program,"sceneTex");
  Utex1=glGetUniformLocation(program,"NoiseTex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program); 
  glUniform1f(Uoffset, offset);
  glUniform1f(Ufrequency, frequency);
  glUniform2f(Upixel, pixel[0], pixel[1]);
  glUniform2f(Uresolution, resolution[0], resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);

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

extern FrostedShader frostedShader;

class SimpleFrostShader : public GLSLShader {
public:
 GLuint *pTex0; // scene, noise
 GLint Utex0,Uoffsetx,Uoffsety,Uscale,Uax,Uay,Ubx,Uby,Ud;
 float offsetx,offsety,scale,ax,ay,bx,by,d;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  offsetx=offsety=0.0f;
  scale=0.05f;
  ax=92.0f;
  ay=80.0f;
  bx=41.0f;
  by=62.0f;
  d=5.1f;
  OUTPUT("SimpleFrostShader::Loading\n");
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
   string("uniform sampler2D tex;\n")+
   string("uniform float offsetx,offsety,ax,ay,bx,by,d,scale;\n")+
   string("float rand(vec2 co){\n")+
   string("return fract(sin(offsetx+dot(co.xy ,vec2(ax,ay))) + cos(offsety+dot(co.xy ,vec2(bx,by))) * d);\n")+
   string("}\n")+
   string("void main() {\n")+
   string(" vec2 rnd = vec2(rand(Texcoord.xy),rand(Texcoord.xy));")+
   string(" outColor = texture2D(tex, Texcoord.xy+rnd*scale);")+
   string("}")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex");
  Uscale=glGetUniformLocation(program,"scale");
  Uoffsetx=glGetUniformLocation(program,"offsetx");
  Uoffsety=glGetUniformLocation(program,"offsety");
  Uax=glGetUniformLocation(program,"ax");
  Uay=glGetUniformLocation(program,"ay");
  Ubx=glGetUniformLocation(program,"bx");
  Uby=glGetUniformLocation(program,"by");
  Ud=glGetUniformLocation(program,"d");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Uscale,scale);
  glUniform1f(Uoffsetx,offsetx);
  glUniform1f(Uoffsety,offsety);
  glUniform1f(Uax,ax);
  glUniform1f(Uay,ay);
  glUniform1f(Ubx,bx);
  glUniform1f(Uby,by);
  glUniform1f(Ud,d);
  
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

extern SimpleFrostShader simplefrostShader;