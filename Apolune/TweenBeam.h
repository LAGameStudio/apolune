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
#include "TextureLibrary.h"
#include "SpecialPaths.h"

class TweenBeamProfile;
void ShadedTweenBeam( TweenBeamProfile *profile, FBOColor *out );

class TweenBeam : public GLSLShader {
public:
 float time;
 float overlap;
 float t0,t1,t2,t3,t4; // tween index (0-tweens.count)
 float ts0,ts1,ts2,ts3,ts4; // time scale per tween
 float a,b,c,d,e; // tween offset (0-1024)
 int n,m,m0,m1,m2,m3,m4;
 Crayon c0,c1,c2,c3,c4;
 GLuint *pTweens,*pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 int iterations;

  // Uniform locations
 GLint
  Utime,
  Uoverlap,
  Un,Um,
  Um0,Um1,Um2,Um3,Um4,
  Ut0,Ut1,Ut2,Ut3,Ut4,
  Uts0,Uts1,Uts2,Uts3,Uts4,
  Ua,Ub,Uc,Ud,Ue,
  Uc0,Uc1,Uc2,Uc3,Uc4,
  Utweens,
  Utex0,Utex1,Utex2,Utex3,Utex4,
  Uiterations;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  OUTPUT("TweenBeam::Loading\n");
  string vert=
    string("#version 150\n")
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
    string("#version 150 core\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Similar to GL_REPEAT, but lets you create a tint/brightness topography and overlap\n")+
   string("uniform int iterations;\n")+
   string("uniform int n,m,m0,m1,m2,m3,m4;\n")+
   string("uniform float time;\n")+
   string("uniform float overlap;\n")+
   string("uniform float ts0,ts1,ts2,ts3,ts4;\n")+
   string("uniform float t0,t1,t2,t3,t4; //tween indexes\n")+
   string("uniform float a,b,c,d,e; //tween offsets\n")+
   string("uniform vec4 c0;\n")+
   string("uniform vec4 c1;\n")+
   string("uniform vec4 c2;\n")+
   string("uniform vec4 c3;\n")+
   string("uniform vec4 c4;\n")+
   string("uniform sampler2D tweens;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("void main(void)\n")+
   string("{\n")+
   string(" float ratio=Texcoord.x;\n")+
   string(" vec3 f0=texture2D(tweens,vec2((a+time*ts0+ratio),t0)).rgb;\n")+
   string(" vec3 f1=texture2D(tweens,vec2((b+time*ts1+ratio),t1)).rgb;\n")+
   string(" vec3 f2=texture2D(tweens,vec2((c+time*ts2+ratio),t2)).rgb;\n")+
   string(" vec3 f3=texture2D(tweens,vec2((d+time*ts3+ratio),t3)).rgb;\n")+
   string(" vec3 f4=texture2D(tweens,vec2((e+time*ts4+ratio),t4)).rgb;\n")+
   string(" vec3 fc=vec3(0,0,0);\n")+
   string(" int i=iterations;\n")+
   string(" vec3 color0, color1, color2, color3, color4;\n")+
   string(" vec2 offset;\n")+
   string(" vec3 color;\n")+
   string(" while ( i > 0 ) {\n")+
   string("  offset=vec2(Texcoord.x*overlap/i,Texcoord.y);\n")+
   string("  color0 = c0.rgb*texture2D(tex0,offset).rgb*f0;\n")+
   string("  color1 = c1.rgb*texture2D(tex1,offset).rgb*f1;\n")+
   string("  color2 = c2.rgb*texture2D(tex2,offset).rgb*f2;\n")+
   string("  color3 = c3.rgb*texture2D(tex3,offset).rgb*f3;\n")+
   string("  color4 = c4.rgb*texture2D(tex4,offset).rgb*f4;\n")+
   string("  // blend layers\n")+
   string("  color=vec3(0.0,0.0,0.0);\n")+
   string("  if ( m0 == 0 ) color+=color0; else color-=color0;\n")+
   string("  if ( m1 == 0 ) color+=color1; else color-=color1;\n")+
   string("  if ( m2 == 0 ) color+=color2; else color-=color2;\n")+
   string("  if ( m3 == 0 ) color+=color3; else color-=color3;\n")+
   string("  if ( m4 == 0 ) color+=color4; else color-=color4;\n")+
   string("  if ( n == 0 ) fc+=color;\n")+
   string("  else if ( n == 1 ) fc-=color;\n")+
   string("  else fc+=(color)/5.0;\n")+
   string("  i-=1;\n")+
   string(" }\n")+
   string(" if ( m == 1 ) fc/=iterations;\n")+
   string(" ratio=1.0-ratio;\n")+
   string(" outColor = vec4(fc.r*ratio,fc.g*ratio,fc.b*ratio,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  time=0.0f;
  iterations=5;
  overlap=10.0f;
  t0=t1=t2=t3=t4=0.5f;
  ts0=ts1=ts2=ts3=ts4=1.0f;
  a=b=c=d=e=0.0f;
  n=0;
  m=0;
  m0=0;
  m1=1;
  m2=0;
  m3=1;
  m4=0;
  c0.Any();
  c1.Any();
  c2.Any();
  c3.Any();
  c4.Any();
  GLImage *defaults=library.Load("data/images/elements/white.png",tllto_Tiling);
  pTex0=pTex1=pTex2=pTex3=pTex4=&defaults->texture;
  Utime=glGetUniformLocation(program,"time");
  Uoverlap=glGetUniformLocation(program,"overlap");
  Ut0=glGetUniformLocation(program,"t0");
  Ut1=glGetUniformLocation(program,"t1");
  Ut2=glGetUniformLocation(program,"t2");
  Ut3=glGetUniformLocation(program,"t3");
  Ut4=glGetUniformLocation(program,"t4");
  Un=glGetUniformLocation(program,"n");
  Um=glGetUniformLocation(program,"m");
  Um0=glGetUniformLocation(program,"m0");
  Um1=glGetUniformLocation(program,"m1");
  Um2=glGetUniformLocation(program,"m2");
  Um3=glGetUniformLocation(program,"m3");
  Um4=glGetUniformLocation(program,"m4");
  Uts0=glGetUniformLocation(program,"ts0");
  Uts1=glGetUniformLocation(program,"ts1");
  Uts2=glGetUniformLocation(program,"ts2");
  Uts3=glGetUniformLocation(program,"ts3");
  Uts4=glGetUniformLocation(program,"ts4");
  Ua=glGetUniformLocation(program,"a");
  Ub=glGetUniformLocation(program,"b");
  Uc=glGetUniformLocation(program,"c");
  Ud=glGetUniformLocation(program,"d");
  Ue=glGetUniformLocation(program,"e");
  Uc0=glGetUniformLocation(program,"c0");
  Uc1=glGetUniformLocation(program,"c1");
  Uc2=glGetUniformLocation(program,"c2");
  Uc3=glGetUniformLocation(program,"c3");
  Uc4=glGetUniformLocation(program,"c4");
  Utweens=glGetUniformLocation(program,"tweens");
  GLImage *tweens=library.Load(specialPaths.historic("tweentable.png").c_str(),tllto_Tiling);
  pTweens=&tweens->texture;
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
  Uiterations=glGetUniformLocation(program,"iterations");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform1f(Utime, time);
  glUniform1f(Uoverlap, overlap);
  glUniform1i(Un, n);
  glUniform1i(Um, m);
  glUniform1i(Um0, m0);
  glUniform1i(Um1, m1);
  glUniform1i(Um2, m2);
  glUniform1i(Um3, m3);
  glUniform1i(Um4, m4);
  glUniform1f(Ut0, t0);
  glUniform1f(Ut1, t1);
  glUniform1f(Ut2, t2);
  glUniform1f(Ut3, t3);
  glUniform1f(Ut4, t4);
  glUniform1f(Uts0, ts0);
  glUniform1f(Uts1, ts1);
  glUniform1f(Uts2, ts2);
  glUniform1f(Uts3, ts3);
  glUniform1f(Uts4, ts4);
  glUniform1f(Ua, a);
  glUniform1f(Ua, b);
  glUniform1f(Ua, c);
  glUniform1f(Ua, d);
  glUniform1f(Ua, e);
  glUniform4f(Uc0,c0.rf,c0.gf,c0.bf,c0.af);
  glUniform4f(Uc1,c1.rf,c1.gf,c1.bf,c1.af);
  glUniform4f(Uc2,c2.rf,c2.gf,c2.bf,c2.af);
  glUniform4f(Uc3,c3.rf,c3.gf,c3.bf,c3.af);
  glUniform4f(Uc4,c4.rf,c4.gf,c4.bf,c4.af);
  glUniform1i(Utweens,0);
  glUniform1i(Utex0, 1);
  glUniform1i(Utex1, 2);
  glUniform1i(Utex2, 3);
  glUniform1i(Utex3, 4);
  glUniform1i(Utex4, 5);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTweens);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
  glUniform1i(Uiterations,iterations);

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
  glActiveTexture(GL_TEXTURE0);	
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern TweenBeam tweenBeam;