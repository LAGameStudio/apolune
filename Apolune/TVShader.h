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

void ShadedTV( GLuint *in, FBOColor *out );

class OldTVProfile;
void ShadedOldTV( FBOColor *out, OldTVProfile *in );

class TVShader : public GLSLShader {
public:
 float time;
 GLuint *pTex0;

  // Uniform locations
 GLint Utime, Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 
 void Init();

 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  Utex0=glGetUniformLocation(program, "tex0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
 
  glUniform1f(Utime, time);

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
extern TVShader tvShader;

ONE(OldTVProfile,{
 vertJerkOpt    = 1.0;
 vertMovementOpt= 1.0;
 bottomStaticOpt= 1.0;
 scalinesOpt    = 1.0;
 rgbOffsetOpt   = 1.0;
 horzFuzzOpt    = 1.0;
})
 Zdouble time,vertJerkOpt,vertMovementOpt,bottomStaticOpt,scalinesOpt,rgbOffsetOpt,horzFuzzOpt;
 Zp<GLuint> tex;
 KEYWORDS({
  KEYWORD("vertJerkOpt",{vertJerkOpt=w;})
  else KEYWORD("vertMovementOpt",{vertMovementOpt=w;})
  else KEYWORD("bottomStaticOpt",{bottomStaticOpt=w;})
  else KEYWORD("scalinesOpt",{scalinesOpt=w;})
  else KEYWORD("rgbOffsetOpt",{rgbOffsetOpt=w;})
  else KEYWORD("horzFuzzOpt",{horzFuzzOpt=w;})
 })
 WORDKEYS({
  NUMBKEY("vertJerkOpt",vertJerkOpt)
  NUMBKEY("vertMovementOpt",vertMovementOpt)
  NUMBKEY("bottomStaticOpt",bottomStaticOpt)
  NUMBKEY("scalinesOpt",scalinesOpt)
  NUMBKEY("rgbOffsetOpt",rgbOffsetOpt)
  NUMBKEY("horzFuzzOpt",horzFuzzOpt)
 })
MANY(OldTVProfile,OldTVProfileHandle,OldTVProfileHandles,"OldTV",OldTVProfiles,{})
 KEYWORDSGroup("OldTV",OldTVProfile);
 WORDKEYSGroup("OldTV",OldTVProfile);
DONE(OldTVProfile);

class OldTVShader : public GLSLShader {
public:
 Zdouble time,vertJerkOpt,vertMovementOpt,bottomStaticOpt,scalinesOpt,rgbOffsetOpt,horzFuzzOpt;
 Zp<GLuint> tex;
 GLuint Utime,UvertJerkOpt,UvertMovementOpt,UbottomStaticOpt,UscalinesOpt,UrgbOffsetOpt,UhorzFuzzOpt,Utex;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program,"time");
  UvertJerkOpt=glGetUniformLocation(program, "vertJerkOpt");
  UvertMovementOpt=glGetUniformLocation(program, "vertMovementOpt");
  UbottomStaticOpt=glGetUniformLocation(program, "bottomStaticOpt");
  UscalinesOpt=glGetUniformLocation(program, "scalinesOpt");
  UrgbOffsetOpt=glGetUniformLocation(program, "rgbOffsetOpt");
  UhorzFuzzOpt=glGetUniformLocation(program, "horzFuzzOpt");
  Utex=glGetUniformLocation(program, "tex");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Utime,(GLfloat)time);
  glUniform1f(UvertJerkOpt,(GLfloat)vertJerkOpt);
  glUniform1f(UvertMovementOpt,(GLfloat)vertMovementOpt);
  glUniform1f(UbottomStaticOpt,(GLfloat)bottomStaticOpt);
  glUniform1f(UscalinesOpt,(GLfloat)scalinesOpt);
  glUniform1f(UrgbOffsetOpt,(GLfloat)rgbOffsetOpt);
  glUniform1f(UhorzFuzzOpt,(GLfloat)horzFuzzOpt);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *tex);

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
 void Set( OldTVProfile *in ) {
  time=in->time;
  vertJerkOpt=in->vertJerkOpt;
  vertMovementOpt=in->vertMovementOpt;
  bottomStaticOpt=in->bottomStaticOpt;
  scalinesOpt=in->scalinesOpt;
  rgbOffsetOpt=in->rgbOffsetOpt;
  horzFuzzOpt=in->horzFuzzOpt;
  tex=in->tex;
 }
 void Render( FBOColor *out, OldTVProfile *in );
};

extern OldTVShader oldtvShader;