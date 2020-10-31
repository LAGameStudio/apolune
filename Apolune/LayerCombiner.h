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
#include "TextureLibrary.h"
#include "SpecialPaths.h"

class LayerCombinerProfile;
void ShadedLayerCombiner( LayerCombinerProfile *in, FBOColor *out );

class LayerCombiner : public GLSLShader {
public:
 float time;
 float ts1,ts2,ts3,ts4,ts5;
 float t1,t2,t3,t4,t5; // tween index (0-tweens.count)
 float a1,b1,a2,b2,a3,b3,a4,b4,a5,b5; // scale factor
 float w1,w2,w3,w4,w5;
 Crayon c1,c2,c3,c4,c5;
 GLuint *pTweens,*pTex0,*pTex1,*pTex2,*pTex3,*pTex4;

  // Uniform locations
 GLint
  Utime,
  Uts1,Uts2,Uts3,Uts4,Uts5,
  Ut1,Ut2,Ut3,Ut4,Ut5,
  Ua1,Ub1,Ua2,Ub2,Ua3,Ub3,Ua4,Ub4,Ua5,Ub5,
  Uw1,Uw2,Uw3,Uw4,Uw5,
  Uc1,Uc2,Uc3,Uc4,Uc5,
  Utweens,
  Utex0,Utex1,Utex2,Utex3,Utex4;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init() {
  OUTPUT("LayerCombiner::Loading\n");
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
   string("// Replaces code that layered/faded textures of different widths and heights around a point\n")+
   string("uniform float time;\n")+
   string("uniform float ts1,ts2,ts3,ts4,ts5;\n")+
   string("uniform float t1,t2,t3,t4,t5;\n")+
   string("uniform sampler2D tweens;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("uniform float a1,b1;\n")+
   string("uniform float a2,b2;\n")+
   string("uniform float a3,b3;\n")+
   string("uniform float a4,b4;\n")+
   string("uniform float a5,b5;\n")+
   string("uniform float w1;\n")+
   string("uniform float w2;\n")+               
   string("uniform float w3;\n")+
   string("uniform float w4;\n")+
   string("uniform float w5;\n")+
   string("uniform vec3 c1,c2,c3,c4,c5;\n")+
   string("void main() {\n")+
   string(" float\n")+
   string("  W1=1.1+((1.0-w1)*25.0),\n")+
   string("  W2=1.1+((1.0-w2)*25.0),\n")+
   string("  W3=1.1+((1.0-w3)*25.0),\n")+
   string("  W4=1.1+((1.0-w4)*25.0),\n")+
   string("  W5=1.1+((1.0-w5)*25.0);\n")+
   string(" vec3 f0=texture2D(tweens,vec2((b1+ts1*time*(b1-a1)),t1)).rgb;\n")+
   string(" vec3 f1=texture2D(tweens,vec2((b2+ts2*time*(b2-a2)),t2)).rgb;\n")+
   string(" vec3 f2=texture2D(tweens,vec2((b3+ts3*time*(b3-a3)),t3)).rgb;\n")+
   string(" vec3 f3=texture2D(tweens,vec2((b4+ts4*time*(b4-a4)),t4)).rgb;\n")+
   string(" vec3 f4=texture2D(tweens,vec2((b5+ts5*time*(b5-a5)),t5)).rgb;\n")+
   string(" vec2 trans=vec2(Texcoord.x-0.5,Texcoord.y-0.5);\n")+
   string(" vec2 ww1=vec2(W1*trans.x,W1*trans.y);\n")+
   string(" vec2 ww2=vec2(W2*trans.x,W2*trans.y);\n")+
   string(" vec2 ww3=vec2(W3*trans.x,W3*trans.y);\n")+
   string(" vec2 ww4=vec2(W4*trans.x,W4*trans.y);\n")+
   string(" vec2 ww5=vec2(W5*trans.x,W5*trans.y);\n")+
   string(" vec3 cc1=vec3(f0)*c1*texture2D(tex0,ww1+vec2(0.5,0.5)).rgb;\n")+
   string(" vec3 cc2=vec3(f1)*c2*texture2D(tex1,ww2+vec2(0.5,0.5)).rgb;\n")+
   string(" vec3 cc3=vec3(f2)*c3*texture2D(tex2,ww3+vec2(0.5,0.5)).rgb;\n")+
   string(" vec3 cc4=vec3(f3)*c4*texture2D(tex3,ww4+vec2(0.5,0.5)).rgb;\n")+
   string(" vec3 cc5=vec3(f4)*c5*texture2D(tex4,ww5+vec2(0.5,0.5)).rgb;\n")+
   string(" vec3 added=(cc1+cc2+cc3+cc4+cc5);\n")+
   string(" outColor=vec4(added,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  time=0.0f;
  ts1=ts2=ts3=ts4=ts5=1.0f;
  t1=t2=t3=t4=t5=0.0f;
  a1=a2=a3=a4=a5=0.0f;
  b1=b2=b3=b4=b5=1.0f;
  w1=w2=w3=w4=w5=1.0f;
  c1.Any();
  c2.Any();
  c3.Any();
  c4.Any();
  c5.Any();
  GLImage *defaults=library.Load("data/images/flares/neoflare1.png");
  pTex0=pTex1=pTex2=pTex3=pTex4=&defaults->texture;
  Utime=glGetUniformLocation(program,"time");
  Uts1=glGetUniformLocation(program,"ts1");
  Uts2=glGetUniformLocation(program,"ts2");
  Uts3=glGetUniformLocation(program,"ts3");
  Uts4=glGetUniformLocation(program,"ts4");
  Uts5=glGetUniformLocation(program,"ts5");
  Ut1=glGetUniformLocation(program,"t1");
  Ut2=glGetUniformLocation(program,"t2");
  Ut3=glGetUniformLocation(program,"t3");
  Ut4=glGetUniformLocation(program,"t4");
  Ut5=glGetUniformLocation(program,"t5");
  Ua1=glGetUniformLocation(program,"a1");
  Ub1=glGetUniformLocation(program,"b1");
  Ua2=glGetUniformLocation(program,"a2");
  Ub2=glGetUniformLocation(program,"b2");
  Ua3=glGetUniformLocation(program,"a3");
  Ub3=glGetUniformLocation(program,"b3");
  Ua4=glGetUniformLocation(program,"a4");
  Ub4=glGetUniformLocation(program,"b4");
  Ua5=glGetUniformLocation(program,"a5");
  Ub5=glGetUniformLocation(program,"b5");
  Uw1=glGetUniformLocation(program,"w1");
  Uw2=glGetUniformLocation(program,"w2");
  Uw3=glGetUniformLocation(program,"w3");
  Uw4=glGetUniformLocation(program,"w4");
  Uw5=glGetUniformLocation(program,"w5");
  Uc1=glGetUniformLocation(program,"c1");
  Uc2=glGetUniformLocation(program,"c2");
  Uc3=glGetUniformLocation(program,"c3");
  Uc4=glGetUniformLocation(program,"c4");
  Uc5=glGetUniformLocation(program,"c5");
  Utweens=glGetUniformLocation(program,"tweens");
  GLImage *tweens=library.Load(specialPaths.historic("tweentable.png").c_str(),tllto_Tiling);
  pTweens=&tweens->texture;
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utex4=glGetUniformLocation(program,"tex4");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform1f(Utime, time);
  glUniform1f(Uts1, ts1);
  glUniform1f(Uts2, ts2);
  glUniform1f(Uts3, ts3);
  glUniform1f(Uts4, ts4);
  glUniform1f(Uts5, ts5);
  glUniform1f(Ut1, t1);
  glUniform1f(Ut2, t2);
  glUniform1f(Ut3, t3);
  glUniform1f(Ut4, t4);
  glUniform1f(Ut5, t5);
  glUniform1f(Ua1, a1);
  glUniform1f(Ua2, a2);
  glUniform1f(Ua3, a3);
  glUniform1f(Ua4, a4);
  glUniform1f(Ua5, a5);
  glUniform1f(Ub1, b1);
  glUniform1f(Ub2, b2);
  glUniform1f(Ub3, b3);
  glUniform1f(Ub4, b4);
  glUniform1f(Ub5, b5);
  glUniform1f(Uw1, w1);
  glUniform1f(Uw2, w2);
  glUniform1f(Uw3, w3);
  glUniform1f(Uw4, w4);
  glUniform1f(Uw5, w5);
  glUniform3f(Uc1,c1.rf,c1.gf,c1.bf);
  glUniform3f(Uc2,c2.rf,c2.gf,c2.bf);
  glUniform3f(Uc3,c3.rf,c3.gf,c3.bf);
  glUniform3f(Uc4,c4.rf,c4.gf,c4.bf);
  glUniform3f(Uc5,c5.rf,c5.gf,c5.bf);
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

extern LayerCombiner layerCombiner;