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

class FBOColor;

void ShadedFBM( FBOColor *out );
void ShadedFBMCheapShadows( GLuint *in, FBOColor *out );

class FBMShader : public GLSLShader {
public:
 float resolution[2],position[2];
 float pattern1[4];
 float pattern2[4];
 float pattern3[4];
 float pattern4[4];
 float fade[3];
 float time,factor,colorAmp;
 GLuint Utime,Ufactor,Uresolution,Uposition,UcolorAmp,Up1,Up2,Up3,Up4,Ufade;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  position[0]=0.0f;
  position[1]=0.0f;
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  time=0.0f;
  factor=41.0f;
  colorAmp=3.5f;
  pattern1[0]=1.7f;
  pattern1[1]=9.2f;
  pattern1[2]=8.3f;
  pattern1[3]=2.8f;
  pattern2[0]=0.0f;
  pattern2[1]=0.0f;
  pattern2[2]=5.2f;
  pattern2[3]=1.3f;
  pattern3[0]=2.5f;
  pattern3[1]=0.4f;
  pattern3[2]=2.3f;
  pattern3[3]=0.4f;
  pattern4[0]=2.3f;
  pattern4[1]=34.0f;
  pattern4[2]=4.0f;
  pattern4[3]=289.0f; // mod value
  fade[0]=6.0f;
  fade[1]=15.0f;
  fade[2]=10.0f;  
  OUTPUT("FBMShader::Loading\n");
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
   string("uniform float time; \n")+
   string("uniform float colorAmp; \n")+
   string("uniform vec2 resolution,pos;\n")+
   string("uniform vec3 fader;\n")+
   string("uniform float factor;\n")+
   string("uniform vec4 p1,p2,p3,p4;\n")+
   string("vec4 mod289(vec4 x){    return x - floor(x * (1.0 / p4.w)) * p4.w;} \n")+
   string("vec4 permute(vec4 x){    return mod289(((x*p4.y)+1.0)*x);} \n")+
   string("vec4 taylorInvSqrt(vec4 r){    return 1.79284291400159 - 0.85373472095314 * r;} \n")+
   string("vec2 fade(vec2 t) {    return t*t*t*(t*(t*fader.x-fader.y)+fader.z);} \n")+
   string("// Classic Perlin noise\n")+
   string("float cnoise(vec2 P){\n")+
   string(" vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);\n")+
   string(" vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);\n")+
   string(" Pi = mod289(Pi); // To avoid truncation effects in permutation\n")+
   string(" vec4 ix = Pi.xzxz;\n")+
   string(" vec4 iy = Pi.yyww;\n")+
   string(" vec4 fx = Pf.xzxz;\n")+
   string(" vec4 fy = Pf.yyww;\n")+     
   string(" vec4 i = permute(permute(ix) + iy);\n")+     
   string(" vec4 gx = fract(i * (1.0 / factor)) * 2.0 - 1.0 ;\n")+
   string(" vec4 gy = abs(gx) - 0.5 ;\n")+
   string(" vec4 tx = floor(gx + 0.5);\n")+
   string(" gx = gx - tx;\n")+     
   string(" vec2 g00 = vec2(gx.x,gy.x);\n")+
   string(" vec2 g10 = vec2(gx.y,gy.y);\n")+
   string(" vec2 g01 = vec2(gx.z,gy.z);\n")+
   string(" vec2 g11 = vec2(gx.w,gy.w);\n")+     
   string(" vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));\n")+
   string(" g00 *= norm.x;  \n")+
   string(" g01 *= norm.y;  \n")+
   string(" g10 *= norm.z;  \n")+
   string(" g11 *= norm.w;  \n")+     
   string(" float n00 = dot(g00, vec2(fx.x, fy.x));\n")+
   string(" float n10 = dot(g10, vec2(fx.y, fy.y));\n")+
   string(" float n01 = dot(g01, vec2(fx.z, fy.z));\n")+
   string(" float n11 = dot(g11, vec2(fx.w, fy.w));\n")+
   string(" vec2 fade_xy = fade(Pf.xy);\n")+
   string(" vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);\n")+
   string(" float n_xy = mix(n_x.x, n_x.y, fade_xy.y);\n")+
   string(" return 2.3 * n_xy;\n")+
   string("}\n")+
   string("// Classic Perlin noise, periodic variant\n")+
   string("float pnoise(vec2 P, vec2 rep){ \n")+
   string("    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);\n")+
   string("    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);\n")+
   string("    Pi = mod(Pi, rep.xyxy); // To create noise with explicit period\n")+
   string("    Pi = mod289(Pi);        // To avoid truncation effects in permutation\n")+
   string("    vec4 ix = Pi.xzxz;\n")+
   string("    vec4 iy = Pi.yyww;\n")+
   string("    vec4 fx = Pf.xzxz;\n")+
   string("    vec4 fy = Pf.yyww;     \n")+
   string("    vec4 i = permute(permute(ix) + iy);     \n")+
   string("    vec4 gx = fract(i * (1.0 / factor)) * 2.0 - 1.0 ;\n")+
   string("    vec4 gy = abs(gx) - 0.5 ;\n")+
   string("    vec4 tx = floor(gx + 0.5);\n")+
   string("    gx = gx - tx;     \n")+
   string("    vec2 g00 = vec2(gx.x,gy.x);\n")+
   string("    vec2 g10 = vec2(gx.y,gy.y);\n")+
   string("    vec2 g01 = vec2(gx.z,gy.z);\n")+
   string("    vec2 g11 = vec2(gx.w,gy.w);     \n")+
   string("    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));\n")+
   string("    g00 *= norm.x;  \n")+
   string("    g01 *= norm.y;  \n")+
   string("    g10 *= norm.z;  \n")+
   string("    g11 *= norm.w;       \n")+
   string("    float n00 = dot(g00, vec2(fx.x, fy.x));\n")+
   string("    float n10 = dot(g10, vec2(fx.y, fy.y));\n")+
   string("    float n01 = dot(g01, vec2(fx.z, fy.z));\n")+
   string("    float n11 = dot(g11, vec2(fx.w, fy.w));     \n")+
   string("    vec2 fade_xy = fade(Pf.xy);\n")+
   string("    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);\n")+
   string("    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);\n")+
   string("    return p4.x * n_xy;\n")+
   string("} \n")+
   string("float fbm(vec2 P, int octaves, float lacunarity, float gain){ \n")+
   string("    float sum = 0.0;\n")+
   string("    float amp = 1.0;\n")+
   string("    vec2 pp = P;     \n")+
   string("    int i;     \n")+
   string("    for(i = 0; i < octaves; i+=1)    {\n")+
   string("        amp *= gain; \n")+
   string("        sum += amp * cnoise(pp);\n")+
   string("        pp *= lacunarity;\n")+
   string("    }\n")+
   string("    return sum; \n")+
   string("} \n")+
   string("float pattern(in vec2 p) {\n")+
   string("    float l = p3.x;\n")+
   string("    float g = p3.y;\n")+
   string("    int oc = 10;     \n")+
   string("    vec2 q = vec2( fbm( p + vec2(p2.x,p2.y),oc,l,g),fbm( p + vec2(p2.z,p2.w),oc,l,g));\n")+
   string("    vec2 r = vec2( fbm( p + p4.z*q + vec2(p1.x,p1.y),oc,l,g ), fbm( p + p4.z*q + vec2(p1.z,p1.w) ,oc,l,g));\n")+
   string("    return fbm( p + p4.z*r ,oc,l,g);    \n")+
   string("} \n")+
   string("float pattern2( in vec2 p, out vec2 q, out vec2 r , in float time){\n")+
   string("    float l = p3.z;\n")+
   string("    float g = p3.w;\n")+
   string("    int oc = 10;      \n")+
   string("    q.x = fbm( p + vec2(time,time),oc,l,g);\n")+
   string("    q.y = fbm( p + vec2(p2.z*time,p2.w*time) ,oc,l,g);     \n")+
   string("    r.x = fbm( p + p4.z*q + vec2(p1.x,p1.y),oc,l,g );\n")+
   string("    r.y = fbm( p + p4.z*q + vec2(p1.z,p1.w) ,oc,l,g);     \n")+
   string("    return fbm( p + p4.z*r ,oc,l,g);\n")+
   string("} \n")+
   string("void main() {\n")+
   string("    vec2 q = Texcoord.xy + pos.xy / resolution;\n")+
   string("    vec2 p = -1.0 + 2.0 * q;\n")+
   string("    vec2 qq;\n")+
   string("    vec2 r;\n")+
   string("    float color = pattern2(p,qq,r,time);     \n")+
   string("    vec4 c = vec4(color,color,color,color);\n")+
   string("    c *= colorAmp;     \n")+
   string("    outColor = c;\n")+
   string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  UcolorAmp=glGetUniformLocation(program,"colorAmp");
  Ufactor=glGetUniformLocation(program,"factor");
  Uresolution=glGetUniformLocation(program,"resolution");
  Uposition=glGetUniformLocation(program,"pos");
  Ufade=glGetUniformLocation(program,"fader");
  Up1=glGetUniformLocation(program,"p1");
  Up2=glGetUniformLocation(program,"p2");
  Up3=glGetUniformLocation(program,"p3");
  Up4=glGetUniformLocation(program,"p4");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);  
  glUniform1f(Utime,    time);
  glUniform1f(Ufactor,  factor);
  glUniform1f(UcolorAmp,  colorAmp);
  glUniform2f(Uresolution, resolution[0], resolution[1]);
  glUniform2f(Uposition, position[0], position[1]);
  glUniform4f(Up1, pattern1[0], pattern1[1], pattern1[2], pattern1[3]);
  glUniform4f(Up2, pattern2[0], pattern2[1], pattern2[2], pattern2[3]);
  glUniform4f(Up3, pattern3[0], pattern3[1], pattern3[2], pattern3[3]);
  glUniform4f(Up4, pattern4[0], pattern4[1], pattern4[2], pattern4[3]);
  glUniform3f(Ufade, fade[0], fade[1], fade[2]);

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

extern FBMShader fbmShader;

// Makes incoming RGB 1.0 into dim black, 0.0 into transparent.
// Used with FBM to create shadows.
class FBMCheapShadowShader : public GLSLShader {
public:
 float a;
 GLuint *pTex0;
 GLint Utex0,Ua;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  a=0.25f;
  pTex0=null;
  OUTPUT("FBMCheapShadowShader::Loading\n");
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
   string("uniform float a;\n")+
   string("uniform sampler2D tex0; // shadow image\n")+
   string("void main() {\n")+
   string(" vec4 c=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec3 d=vec3(1.0-c.r,1.0-c.g,1.0-c.b);\n")+
   string(" outColor= vec4(d,c.r*a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Ua=glGetUniformLocation(program,"a");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ua,a);

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

extern FBMCheapShadowShader fbmcheapshadowShader;