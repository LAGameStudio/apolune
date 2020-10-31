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


void ShadedNebula4( FBOColor *out );

class Nebula4LightShader : public GLSLShader {
public:
 Crayon L_Color1,L_Color2,L_Color3,L_Color4;
 Cartesiand position,L1,L2,L3,L4,LSizes;
 Zfloat R1,R2,R3,R4;
 Zfloat C1,C2,C3,C4;
 Cartesiand layerTimes,layerTimes2,layerBlurs,layerBlurs2;
 Zfloat time;
 GLuint UWorldPos, UL_Color1, UL_Color2, UL_Color3, UL_Color4,
  Ulight1, Ulight2, Ulight3, Ulight4, URim, UCloudIntensity,
  UL_Size, UlayerFactor, UlayerFactor2, UtimeScale, UtimeScale2,
  UiGlobalTime;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("Nebula4LightShader:Init():\n");
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
   string("uniform vec2 WorldPos; // 2d world offset in texture units\n")+
   string("uniform vec4 L_Color1; // color of light\n")+
   string("uniform vec4 L_Color2;\n")+
   string("uniform vec4 L_Color3;\n")+
   string("uniform vec4 L_Color4;\n")+
   string("uniform vec2 light1;  // x,y position, can be > or < 0-1 range for offscreen\n")+
   string("uniform vec2 light2;\n")+
   string("uniform vec2 light3;\n")+
   string("uniform vec2 light4;\n")+
   string("uniform vec4 Rim;             // x=L1, y=L2, z=L3, w=L4  low 0.0<1.0 makes it bigger than area, high makes it smaller\n")+
   string("//CloudIntensity now L_Color.a// x=L1..w=L4, 0.7 is default, 1.0 is 100%, how bright are cloud reflections\n")+
   string("uniform vec4 L_Size;          // X=L1..w=L4, size where 100.0 is pretty small and 1.0 fills the area.\n")+
   string("uniform vec4 layerFactor;     // Factors for the 6 layer cloud that effect blur\n")+
   string("uniform vec2 layerFactor2;    // ...continued for the levels with near-pixel element size\n")+
   string("uniform vec4 timeScale;       // Factors for the 6 layer cloud that effect permutation speed\n")+
   string("uniform vec2 timeScale2;      // ...continued for the levels with near-pixel element size\n")+
   string("uniform float iGlobalTime;    // Time marches on...\n")+
   string("vec3 mod289(vec3 x) {  return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")+
   string("vec4 mod289(vec4 x) {  return x - floor(x * (1.0 / 289.0)) * 289.0; }\n")+
   string("vec4 permute(vec4 x) {  return mod289(((x*34.0)+1.0)*x); }\n")+
   string("vec4 taylorInvSqrt(vec4 r) {  return 1.79284291400159 - 0.85373472095314 * r; }\n")+
   string("float snoise(vec3 v)  { \n")+
   string(" const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;\n")+
   string(" const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);\n")+
   string(" vec3 i  = floor(v + dot(v, C.yyy) );\n")+
   string(" vec3 x0 =   v - i + dot(i, C.xxx) ;\n")+
   string(" vec3 g = step(x0.yzx, x0.xyz);\n")+
   string(" vec3 l = 1.0 - g;\n")+
   string(" vec3 i1 = min( g.xyz, l.zxy );\n")+
   string(" vec3 i2 = max( g.xyz, l.zxy );\n")+
   string(" vec3 x1 = x0 - i1 + C.xxx;\n")+
   string(" vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y\n")+
   string(" vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y\n")+
   string(" i = mod289(i); \n")+
   string(" vec4 p = permute( permute( permute( \n")+
   string("            i.z + vec4(0.0, i1.z, i2.z, 1.0 ))\n")+
   string("          + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) \n")+
   string("          + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));\n")+
   string("// Gradients: 7x7 points over a square, mapped onto an octahedron.\n")+
   string("// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)\n")+
   string(" float n_ = 0.142857142857; // 1.0/7.0\n")+
   string(" vec3  ns = n_ * D.wyz - D.xzx;\n")+
   string(" vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)\n")+
   string(" vec4 x_ = floor(j * ns.z);\n")+
   string(" vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)\n")+
   string(" vec4 x = x_ *ns.x + ns.yyyy;\n")+
   string(" vec4 y = y_ *ns.x + ns.yyyy;\n")+
   string(" vec4 h = 1.0 - abs(x) - abs(y);\n")+
   string(" vec4 b0 = vec4( x.xy, y.xy );\n")+
   string(" vec4 b1 = vec4( x.zw, y.zw );\n")+
   string(" //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;\n")+
   string(" //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;\n")+
   string(" vec4 s0 = floor(b0)*2.0 + 1.0;\n")+
   string(" vec4 s1 = floor(b1)*2.0 + 1.0;\n")+
   string(" vec4 sh = -step(h, vec4(0.0));\n")+
   string(" vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;\n")+
   string(" vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;\n")+
   string(" vec3 p0 = vec3(a0.xy,h.x);\n")+
   string(" vec3 p1 = vec3(a0.zw,h.y);\n")+
   string(" vec3 p2 = vec3(a1.xy,h.z);\n")+
   string(" vec3 p3 = vec3(a1.zw,h.w);\n")+
   string(" vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));\n")+
   string(" p0 *= norm.x;\n")+
   string(" p1 *= norm.y;\n")+
   string(" p2 *= norm.z;\n")+
   string(" p3 *= norm.w;\n")+
   string(" vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);\n")+
   string(" m = m * m;\n")+
   string(" return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );\n")+
   string("}\n")+
   string("float normnoise(float noise) {\n")+
   string("	return 0.5*(noise+1.0);\n")+
   string("}\n")+
   string("float clouds(vec2 uv) {\n")+
   string(" uv += WorldPos; // Moves it around.\n")+
   string(" vec2 off1 = vec2(50.0,33.0);\n")+
   string(" vec2 off2 = vec2(0.0, 0.0);\n")+
   string(" vec2 off3 = vec2(-300.0, 50.0);\n")+
   string(" vec2 off4 = vec2(-100.0, 200.0);\n")+
   string(" vec2 off5 = vec2(400.0, -200.0);\n")+
   string(" vec2 off6 = vec2(100.0, -1000.0);\n")+
   string(" float scale1 = 3.0;\n")+
   string(" float scale2 = 6.0;\n")+
   string(" float scale3 = 12.0;\n")+
   string(" float scale4 = 24.0;\n")+
   string(" float scale5 = 48.0;\n")+
   string(" float scale6 = 96.0;\n")+
   string(" return normnoise(snoise(vec3((uv+off1)*scale1,iGlobalTime*0.5*timeScale.x))*0.8*layerFactor.x + \n")+
   string("  snoise(vec3((uv+off2)*scale2,iGlobalTime*0.04*timeScale.y))*0.4*layerFactor.y +\n")+
   string("  snoise(vec3((uv+off3)*scale3,iGlobalTime*0.01*timeScale.w))*0.2*layerFactor.w +\n")+
   string("  snoise(vec3((uv+off4)*scale4,iGlobalTime*0.07*timeScale.z))*0.1*layerFactor.z +\n")+
   string("  snoise(vec3((uv+off5)*scale5,iGlobalTime*0.02*timeScale2.x))*0.5*layerFactor2.x +\n")+
   string("  snoise(vec3((uv+off6)*scale6,iGlobalTime*0.03*timeScale2.y))*0.025*layerFactor2.y\n")+
   string(" );\n")+
   string("}\n")+
   string("float L_ci( float C, float R, float D ) { return C*(1.0-(R*D)); }\n")+
   string("float L_i( float S, float D ) { return 1.0/(S*D); }\n")+
   string("void main() {\n")+
   string(" vec2 uv = Texcoord.xy;\n")+
   string(" vec3 final=vec3(0,0,0);\n")+
   string(" vec4 D=vec4(distance(uv,light1),distance(uv,light2),distance(uv,light3),distance(uv,light4));\n")+
   string(" final += vec3(L_ci(L_Color1.a,Rim.x,D.x)*clouds(uv))*L_Color1.rgb + L_i(L_Size.x,D.x)*L_Color1.rgb;\n")+
   string(" final += vec3(L_ci(L_Color2.a,Rim.y,D.y)*clouds(uv))*L_Color2.rgb + L_i(L_Size.y,D.y)*L_Color2.rgb;\n")+
   string(" final += vec3(L_ci(L_Color3.a,Rim.z,D.z)*clouds(uv))*L_Color3.rgb + L_i(L_Size.w,D.w)*L_Color3.rgb;\n")+
   string(" final += vec3(L_ci(L_Color4.a,Rim.w,D.w)*clouds(uv))*L_Color4.rgb + L_i(L_Size.z,D.z)*L_Color4.rgb;\n")+
   string(" outColor=vec4(final.xyz,1.0);\n")+
   string("}")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  UWorldPos      =glGetUniformLocation(program,"WorldPos");
  UL_Color1      =glGetUniformLocation(program,"L_Color1");
  UL_Color2      =glGetUniformLocation(program,"L_Color2");
  UL_Color3      =glGetUniformLocation(program,"L_Color3");
  UL_Color4      =glGetUniformLocation(program,"L_Color4");
  Ulight1        =glGetUniformLocation(program,"light1");
  Ulight2        =glGetUniformLocation(program,"light2");
  Ulight3        =glGetUniformLocation(program,"light3");
  Ulight4        =glGetUniformLocation(program,"light4");
  URim           =glGetUniformLocation(program,"Rim");
//  UCloudIntensity=glGetUniformLocation(program,"CloudIntensity");  packed in L_ColorX
  UL_Size        =glGetUniformLocation(program,"L_Size");
  UlayerFactor   =glGetUniformLocation(program,"layerFactor");
  UlayerFactor2  =glGetUniformLocation(program,"layerFactor2");
  UtimeScale     =glGetUniformLocation(program,"timeScale");
  UtimeScale2    =glGetUniformLocation(program,"timeScale2");
  UiGlobalTime   =glGetUniformLocation(program,"iGlobalTime");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);  
  glUniform1f(UiGlobalTime, time.value);
  glUniform2f(UWorldPos, (float)position.x,(float)position.y);
  glUniform4f(UL_Color1,L_Color1.rf,L_Color1.gf,L_Color1.bf,C1.value);
  glUniform4f(UL_Color2,L_Color2.rf,L_Color2.gf,L_Color2.bf,C2.value);
  glUniform4f(UL_Color3,L_Color3.rf,L_Color3.gf,L_Color3.bf,C3.value);
  glUniform4f(UL_Color4,L_Color4.rf,L_Color4.gf,L_Color4.bf,C4.value);
  glUniform2f(Ulight1,(float)L1.x,(float)L1.y);
  glUniform2f(Ulight2,(float)L2.x,(float)L2.y);
  glUniform2f(Ulight3,(float)L3.x,(float)L3.y);
  glUniform2f(Ulight4,(float)L4.x,(float)L4.y);
  glUniform4f(URim,R1.value,R2.value,R3.value,R4.value);
  glUniform4f(UL_Size,(float)LSizes.x,(float)LSizes.y,(float)LSizes.z,(float)LSizes.w);
  glUniform4f(UlayerFactor,(float)layerBlurs.x,(float)layerBlurs.y,(float)layerBlurs.z,(float)layerBlurs.w);
  glUniform2f(UlayerFactor2,(float) layerBlurs2.x,(float)layerBlurs2.y);
  glUniform4f(UtimeScale,(float)layerTimes.x,(float)layerTimes.y,(float)layerTimes.z,(float)layerTimes.w);
  glUniform2f(UtimeScale2,(float) layerTimes2.x,(float)layerTimes2.y);

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
  glReportError(glGetError());
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern Nebula4LightShader nebula4Shader;