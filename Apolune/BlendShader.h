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

class FBOColor;

void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactorBlackEtch( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactorWhiteClear( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactor( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactor( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparentFactor( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedFBOBlendTransparent(  FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparent(  GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedFBOBlendTransparent(  GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedBlendTransparentFactor( double lerpFactor, FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedBlendTransparentFactor( double lerpFactor, GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedBlendTransparentFactor( double lerpFactor, GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedBlendTransparent(  FBOColor *applying, FBOColor *onto, FBOColor *out );
void ShadedBlendTransparent(  GLuint *applying, FBOColor *onto, FBOColor *out );
void ShadedBlendTransparent(  GLuint *applying, GLuint *onto, FBOColor *out );
void ShadedColorAbove( Crayon threshold, Crayon replace, GLuint *in, FBOColor *out );
void ShadedColorAboveB( float threshold, GLuint *in, FBOColor *out );
void ShadedColorBelow( Crayon threshold, Crayon replace, GLuint *in, FBOColor *out );
void ShadedColorBelowB( float threshold, GLuint *in, FBOColor *out );
void ShadedColorBelowAB( float a, float b, GLuint *in, FBOColor *out );
void ShadedColorAboveBelow( Crayon threshold, Crayon above, Crayon below, GLuint *in, FBOColor *out );
void ShadedBlend3Avg( GLuint *mix, GLuint *r, GLuint *g, GLuint *b, FBOColor *out );
void ShadedBlend3Add( GLuint *mix, GLuint *r, GLuint *g, GLuint *b, FBOColor *out );
void ShadedAlphaAvg( GLuint *in, FBOColor *out );
void ShadedAlphaAvgThreshold( Crayon threshold, GLuint *in, FBOColor *out );
void ShadedBlendMultiplyAlpha( GLuint *mixer, GLuint *sampler, FBOColor *out );
void ShadedBlendAlpha( GLuint *mixer, GLuint *sampler, FBOColor *out );
void ShadedBlendAlphaMult( GLuint *mixer, GLuint *sampler, FBOColor *out );
void ShadedBlendAlphaInv( GLuint *mixer, GLuint *sampler, FBOColor *out );
void ShadedBlendAlpha0( GLuint *mixer, GLuint *sampler, FBOColor *out );
void ShadedBlend2Way( GLuint *mixer, GLuint *tR, GLuint *fallback, FBOColor *out );
void ShadedBlend3Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *fallback, FBOColor *out );
void ShadedBlend4Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB, GLuint *fallback, FBOColor *out );
void ShadedBlend5Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB, GLuint *tBlack, GLuint *fallback, FBOColor *out );
void ShadedBlend6Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB, GLuint *tBlack, GLuint *tGB, GLuint *fallback, FBOColor *out );
void ShadedBlend7Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB, GLuint *tBlack, GLuint *tGB, GLuint *tRG, GLuint *fallback, FBOColor *out );
void ShadedBlend8Way( GLuint *mixer, GLuint *tR, GLuint *tG, GLuint *tB, GLuint *tBlack, GLuint *tGB, GLuint *tRG, GLuint *tRB, GLuint *fallback, FBOColor *out );


class FBOBlendTransparentFactorBlackEtchShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1,Ufactor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 Zdouble factor;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("FBOBlendTransparentFactorBlackEtchShader::Loading\n");
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
   string("uniform float factor;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec2 tcpaint=Texcoord.xy;\n")+
   string(" tcpaint.y=1.0-tcpaint.y;\n")+
   string(" vec4 paint=texture2D(tex1,tcpaint.xy);\n")+
   string(" float pavg=(paint.r+paint.g+paint.b)/3.0;\n")+
   string(" outColor=vec4(mix(canvas.rgb,paint.rgb*canvas.rgb,min(pavg,paint.a)*factor),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Ufactor=glGetUniformLocation(program, "factor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1f(Ufactor,(GLfloat)factor);
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

extern FBOBlendTransparentFactorBlackEtchShader fboblendtransparentfactorblacketchShader;

class FBOBlendTransparentFactorWhiteClearShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1,Ufactor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 Zdouble factor;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("FBOBlendTransparentFactorWhiteClearShader::Loading\n");
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
   string("uniform float factor;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec2 tcpaint=Texcoord.xy;\n")+
   string(" tcpaint.y=1.0-tcpaint.y;\n")+
   string(" vec4 paint=texture2D(tex1,tcpaint.xy);\n")+
   string(" float pavg=(paint.r+paint.g+paint.b)/3.0;\n")+
   string(" outColor=vec4(mix(canvas.rgb,paint.rgb,min(paint.a,pavg)*factor),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Ufactor=glGetUniformLocation(program, "factor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1f(Ufactor,(GLfloat)factor);
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

extern FBOBlendTransparentFactorWhiteClearShader fboblendtransparentfactorwhiteclearShader;


class FBOBlendTransparentShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("FBOBlendTransparentShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec2 tcpaint=Texcoord.xy;\n")+
   string(" tcpaint.y=1.0-tcpaint.y;\n")+
   string(" vec4 paint=texture2D(tex1,tcpaint.xy);\n")+
   string(" outColor=vec4(mix(paint.rgb,canvas.rgb,paint.a),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern FBOBlendTransparentShader fboblendtransparentShader;

class FBOBlendTransparentFactorShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1,Ufactor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 Zdouble factor;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("FBOBlendTransparentFactorShader::Loading\n");
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
   string("uniform float factor;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec2 tcpaint=Texcoord.xy;\n")+
   string(" tcpaint.y=1.0-tcpaint.y;\n")+
   string(" vec4 paint=texture2D(tex1,tcpaint.xy);\n")+
   string(" outColor=vec4(mix(canvas.rgb,paint.rgb,paint.a*factor),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Ufactor=glGetUniformLocation(program, "factor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1f(Ufactor,(GLfloat)factor);
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

extern FBOBlendTransparentFactorShader fboblendtransparentfactorShader;

class BlendTransparentFactorShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1,Ufactor;
 GLuint Utrans; GLint posAttrib,texAttrib;
 Zdouble factor;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendTransparentFactorShader::Loading\n");
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
   string("uniform float factor;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 paint=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(mix(canvas.rgb,paint.rgb,paint.a*factor),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Ufactor=glGetUniformLocation(program, "factor");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1f(Ufactor,(GLfloat)factor);
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

extern BlendTransparentFactorShader blendtransparentfactorShader;

class BlendTransparentShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendTransparentShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 canvas=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 paint=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(mix(paint.rgb,canvas.rgb,paint.a),canvas.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern BlendTransparentShader blendtransparentShader;

// Replaces color under a threshold, ignores alpha for test
class ColorAboveShader : public GLSLShader {
public:
 Crayon colorThreshold;
 Crayon colorReplace;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold,Ureplace;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("ColorAboveShader::Loading\n");
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
   string("uniform vec4 threshold;")+
   string("uniform vec4 replace;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r > threshold.r && sample.g > threshold.g && sample.b > threshold.b ) outColor=replace;\n")+
   string(" else outColor=sample.rgba;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uthreshold=glGetUniformLocation(program, "threshold");
  Ureplace=glGetUniformLocation(program, "replace");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform4f(Uthreshold,colorThreshold.rf,colorThreshold.gf,colorThreshold.bf,colorThreshold.af);
  glUniform4f(Ureplace,colorReplace.rf,colorReplace.gf,colorReplace.bf,colorReplace.af);

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

extern ColorAboveShader coloraboveShader;

// Expects a grayscale incoming image, outputs B above, and below the threshold with R
// Replaces color under a threshold, ignores alpha for test
class ColorAboveBShader : public GLSLShader {
public:
 float threshold;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("ColorAboveBShader::Loading\n");
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
   string("uniform float threshold;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r > threshold ) outColor=vec4(0.0,0.0,sample.r,1.0);\n")+
   string(" else outColor=vec4(sample.rgb,(sample.r+sample.g+sample.b)/3.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uthreshold=glGetUniformLocation(program, "threshold");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform1f(Uthreshold,threshold);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ColorAboveBShader coloraboveBShader;

// Replaces color under a threshold, ignores alpha for test
class ColorBelowShader : public GLSLShader {
public:
 Crayon colorThreshold;
 Crayon colorReplace;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold,Ureplace;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("ColorBelowShader::Loading\n");
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
   string("uniform vec4 threshold;")+
   string("uniform vec4 replace;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r < threshold.r && sample.g < threshold.g && sample.b < threshold.b ) outColor=replace;\n")+
   string(" else outColor=sample;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uthreshold=glGetUniformLocation(program, "threshold");
  Ureplace=glGetUniformLocation(program, "replace");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform4f(Uthreshold,colorThreshold.rf,colorThreshold.gf,colorThreshold.bf,colorThreshold.af);
  glUniform4f(Ureplace,colorReplace.rf,colorReplace.gf,colorReplace.bf,colorReplace.af);

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

extern ColorBelowShader colorbelowShader;

// Expects a grayscale incoming image, outputs B above, and below the threshold with R
// Replaces color under a threshold, ignores alpha for test
class ColorBelowBShader : public GLSLShader {
public:
 float threshold;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("ColorBelowBShader::Loading\n");
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
   string("uniform float threshold;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r < threshold ) outColor=vec4(0.0,0.0,sample.r,1.0);\n")+
   string(" else outColor=vec4(sample.rgb,(sample.r+sample.g+sample.b)/3.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uthreshold=glGetUniformLocation(program, "threshold");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform1f(Uthreshold,threshold);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ColorBelowBShader colorbelowBShader;

// Expects a grayscale incoming image, outputs R above, G in the middle, B and below the threshold
// Replaces color under a threshold, ignores alpha for test
class ColorBelowABShader : public GLSLShader {
public:
 float thresholdA,thresholdB;
 GLuint *pTex0;
 GLuint Utex0,UthresholdA,UthresholdB;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  thresholdA=0.66f;
  thresholdB=0.33f;
  pTex0=null;
  OUTPUT("ColorBelowABShader::Loading\n");
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
   string("uniform float thresholdA;")+
   string("uniform float thresholdB;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r > thresholdA ) outColor=vec4((sample.r-thresholdA)/(1.0-thresholdA),0.0,0.0,1.0);\n")+
   string(" else if ( sample.r > thresholdB ) outColor=vec4(0.0,(sample.g-thresholdB)/(thresholdA-thresholdB),0.0,1.0);\n")+
   string(" else outColor=vec4(0.0,0.0,sample.b/thresholdB,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  UthresholdA=glGetUniformLocation(program, "thresholdA");
  UthresholdB=glGetUniformLocation(program, "thresholdB");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform1f(UthresholdA,thresholdA);
  glUniform1f(UthresholdB,thresholdB);

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

extern ColorBelowABShader colorbelowABShader;

// Drops color 1 above and color 2 below
// Replaces color under a threshold, ignores alpha for test
class ColorAboveBelowShader : public GLSLShader {
public:
 Crayon colorThreshold;
 Crayon above,below;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold,Uabove,Ubelow;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("ColorAboveBelowShader::Loading\n");
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
   string("uniform vec4 threshold;")+
   string("uniform vec4 above,below;")+
   string("uniform sampler2D tex0;")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r < threshold.r && sample.g < threshold.g && sample.b < threshold.b ) outColor=below;\n")+
   string(" else outColor=above;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uthreshold=glGetUniformLocation(program, "threshold");
  Uabove=glGetUniformLocation(program, "above");
  Ubelow=glGetUniformLocation(program, "below");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform4f(Uthreshold,colorThreshold.rf,colorThreshold.gf,colorThreshold.bf,colorThreshold.af);
  glUniform4f(Uabove,above.rf,above.gf,above.bf,above.af);
  glUniform4f(Ubelow,below.rf,below.gf,below.bf,below.af);

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

extern ColorAboveBelowShader colorabovebelowShader;

class Blend3AddShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLuint Utex0,Utex1,Utex2,Utex3;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=null;
  OUTPUT("Blend3AddShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sampleR=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 sampleG=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 sampleB=texture2D(tex3,Texcoord.xy);\n")+
   string(" outColor=(sampleR*mixer.r+sampleG*mixer.g+sampleB*mixer.b);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
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

extern Blend3AddShader blend3addShader;

class Blend3AvgShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLuint Utex0,Utex1,Utex2,Utex3;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=null;
  OUTPUT("Blend3AvgShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sampleR=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 sampleG=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 sampleB=texture2D(tex3,Texcoord.xy);\n")+
   string(" outColor=(sampleR*mixer.r+sampleG*mixer.g+sampleB*mixer.b)/3.0;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
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

extern Blend3AvgShader blend3avgShader;

// Similar to ip_BlackBackgroundToTransparent, an alpha is calculated based on color component average
class AlphaAvgShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("AlphaAvgShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" outColor=vec4(sample.rgb,(sample.r+sample.g+sample.b)/3.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
	 glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern AlphaAvgShader alphaavgShader;

// Similar to ip_BlackBackgroundToTransparent, an alpha is calculated based on color component average
// Includes a threshold (ignores alpha for test)
class AlphaAvgThresholdShader : public GLSLShader {
public:
 Crayon colorThreshold;
 GLuint *pTex0;
 GLuint Utex0,Uthreshold;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("AlphaAvgThresholdShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform vec4 threshold;\n")+
   string("void main() {\n")+
   string(" vec4 sample=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( sample.r < threshold.r && sample.g < threshold.g && sample.b < threshold.b ) \n")+
   string("  outColor=vec4(sample.rgb,(sample.r+sample.g+sample.b)/3.0);\n")+
   string(" else outColor=sample;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Uthreshold=glGetUniformLocation(program, "threshold");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern AlphaAvgThresholdShader alphaavgthresholdShader;

class BlendMultiplyAlphaShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendMultiplyAlphaShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(sample.rgb*mixer.rgb,mixer.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern BlendMultiplyAlphaShader blendmultiplyalphaShader;

class BlendAlphaShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendAlphaShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(sample.rgb,mixer.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern BlendAlphaShader blendalphaShader;

class BlendAlphaMultShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendAlphaShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(sample.rgb,mixer.a*sample.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern BlendAlphaMultShader blendalphamultShader;

class BlendAlphaInvShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendAlphaInvShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=vec4(sample.rgb,1.0-mixer.a);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern BlendAlphaInvShader blendalphainvShader;

class BlendAlpha0Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1; // mixer, sampler
 GLuint Utex0,Utex1;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=null;
  OUTPUT("BlendAlpha0Shader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" if ( mixer.a == 0.0 ) outColor=texture2D(tex1,Texcoord.xy);\n")+
   string(" else outColor=vec4(0.0,0.0,0.0,0.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

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

extern BlendAlpha0Shader blendalpha0Shader;

/*
1: 255,0,0,x
2: "default"
*/
class Blend2WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2; // mixer, sampler
 GLuint Utex0,Utex1,Utex2;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=null;
  OUTPUT("Blend2WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor"); loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
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

extern Blend2WayShader blend2wayShader;

/*
1: 255,0,0,x
2: 0,255,0,x
3: "default"
*/
class Blend3WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=null;
  OUTPUT("Blend3WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
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

extern Blend3WayShader blend3wayShader;

/*
1: 255,0,0,x
2: 0,255,0,x
3: 0,0,255,x
4: "default"
*/
class Blend4WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3,Utex4;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=pTex4=null;
  OUTPUT("Blend4WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex4,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex3,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
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

extern Blend4WayShader blend4wayShader;

/*
1: 255,0,0,x
2: 0,255,0,x
3: 0,0,255,x
4: 0,0,0,x
5: "default"
*/
class Blend5WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4,*pTex5; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3,Utex4,Utex5;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=pTex4=pTex5=null;
  OUTPUT("Blend5WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("uniform sampler2D tex5;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex5,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex4,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex3,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 0.0 && mixer.g == 0.0 && mixer.b == 0.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Utex5=glGetUniformLocation(program, "tex5");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glUniform1i(Utex4, 4);
  glUniform1i(Utex5, 5);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
  glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, *pTex5);
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

extern Blend5WayShader blend5wayShader;


/*
1: 255,0,0,x
2: 0,255,0,x
3: 0,0,255,x
4: 0,0,0,x
5: 0,255,255,x
6: "default"
*/
class Blend6WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4,*pTex5,*pTex6; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3,Utex4,Utex5,Utex6;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=pTex4=pTex5=pTex6=null;
  OUTPUT("Blend6WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("uniform sampler2D tex5;\n")+
   string("uniform sampler2D tex6;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex6,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex5,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex4,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex3,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 0.0 && mixer.g == 0.0 && mixer.b == 0.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 && mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Utex5=glGetUniformLocation(program, "tex5");
  Utex6=glGetUniformLocation(program, "tex6");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glUniform1i(Utex4, 4);
  glUniform1i(Utex5, 5);
  glUniform1i(Utex6, 6);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
  glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, *pTex5);
  glActiveTexture(GL_TEXTURE6);	 glBindTexture(GL_TEXTURE_2D, *pTex6);
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

extern Blend6WayShader blend6wayShader;

/*
1: 255,0,0,x
2: 0,255,0,x
3: 0,0,255,x
4: 0,0,0,x
5: 0,255,255,x
6: 255,255,0,x
7: "default"
*/
class Blend7WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4,*pTex5,*pTex6,*pTex7; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3,Utex4,Utex5,Utex6,Utex7;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=pTex4=pTex5=pTex6=pTex7=null;
  OUTPUT("Blend7WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("uniform sampler2D tex5;\n")+
   string("uniform sampler2D tex6;\n")+
   string("uniform sampler2D tex7;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex7,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex6,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex5,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex4,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 0.0 && mixer.g == 0.0 && mixer.b == 0.0 ) {\n")+
   string("  vec4 sample=texture2D(tex3,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 && mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 1.0 && mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Utex5=glGetUniformLocation(program, "tex5");
  Utex6=glGetUniformLocation(program, "tex6");
  Utex7=glGetUniformLocation(program, "tex7");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glUniform1i(Utex4, 4);
  glUniform1i(Utex5, 5);
  glUniform1i(Utex6, 6);
  glUniform1i(Utex7, 7);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
  glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, *pTex5);
  glActiveTexture(GL_TEXTURE6);	 glBindTexture(GL_TEXTURE_2D, *pTex6);
  glActiveTexture(GL_TEXTURE7);	 glBindTexture(GL_TEXTURE_2D, *pTex7);
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

extern Blend7WayShader blend7wayShader;

/*
1: 255,0,0,x
2: 0,255,0,x
3: 0,0,255,x
4: 0,0,0,x
5: 0,255,255,x
6: 255,255,0,x
7: 255,0,255,x
8: "default"
*/
class Blend8WayShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4,*pTex5,*pTex6,*pTex7,*pTex8; // mixer, sampler
 GLuint Utex0,Utex1,Utex2,Utex3,Utex4,Utex5,Utex6,Utex7,Utex8;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init() {
  pTex0=pTex1=pTex2=pTex3=pTex4=pTex5=pTex6=pTex7=pTex8=null;
  OUTPUT("Blend8WayShader::Loading\n");
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
   string("uniform sampler2D tex0;\n")+
   string("uniform sampler2D tex1;\n")+
   string("uniform sampler2D tex2;\n")+
   string("uniform sampler2D tex3;\n")+
   string("uniform sampler2D tex4;\n")+
   string("uniform sampler2D tex5;\n")+
   string("uniform sampler2D tex6;\n")+
   string("uniform sampler2D tex7;\n")+
   string("uniform sampler2D tex8;\n")+
   string("void main() {\n")+
   string(" vec4 mixer=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 bg=texture2D(tex8,Texcoord.xy);\n")+
   string(" vec4 res;\n")+
   string(" if ( mixer.r == 1.0 && mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex7,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 1.0 && mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex6,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 && mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex5,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 0.0 && mixer.g == 0.0 && mixer.b == 0.0 ) {\n")+
   string("  vec4 sample=texture2D(tex4,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.r == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex1,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.g == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex2,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else if ( mixer.b == 1.0 ) {\n")+
   string("  vec4 sample=texture2D(tex3,Texcoord.xy);\n")+
   string("  res=mix(bg,sample,mixer.a);\n")+
   string(" } else {\n")+
   string("  res=bg;//vec4(bg.rgb,1.0-mixer.a);\n")+ 
   string(" }\n")+
   string(" outColor=res;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Utex1=glGetUniformLocation(program, "tex1");
  Utex2=glGetUniformLocation(program, "tex2");
  Utex3=glGetUniformLocation(program, "tex3");
  Utex4=glGetUniformLocation(program, "tex4");
  Utex5=glGetUniformLocation(program, "tex5");
  Utex6=glGetUniformLocation(program, "tex6");
  Utex7=glGetUniformLocation(program, "tex7");
  Utex8=glGetUniformLocation(program, "tex8");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glUniform1i(Utex1, 1);
  glUniform1i(Utex2, 2);
  glUniform1i(Utex3, 3);
  glUniform1i(Utex4, 4);
  glUniform1i(Utex5, 5);
  glUniform1i(Utex6, 6);
  glUniform1i(Utex7, 7);
  glUniform1i(Utex8, 8);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glActiveTexture(GL_TEXTURE4);	 glBindTexture(GL_TEXTURE_2D, *pTex4);
  glActiveTexture(GL_TEXTURE5);	 glBindTexture(GL_TEXTURE_2D, *pTex5);
  glActiveTexture(GL_TEXTURE6);	 glBindTexture(GL_TEXTURE_2D, *pTex6);
  glActiveTexture(GL_TEXTURE7);	 glBindTexture(GL_TEXTURE_2D, *pTex7);
  glActiveTexture(GL_TEXTURE8);	 glBindTexture(GL_TEXTURE_2D, *pTex8);
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

extern Blend8WayShader blend8wayShader;