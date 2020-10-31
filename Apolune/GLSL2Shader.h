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

#include <Windows.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include "wglext.h"
#include "glext.h"

#include "Art.h"
#include "Strings.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLSLShader {
public:
 GLSLShader();
 ~GLSLShader();

	bool checkHardwareSupport();	//check for required extensions and set them up
	bool loadShaders(const char *fragmentShader);
	bool loadShaders(const char *vertexShader, const char *fragmentShader);
 bool loadShaders(const char *vertexShader, const char *geometryShader, const char *fragmentShader );
 bool loadShaders(const char *vertexShader, const char *controlShader, const char *evalShader, const char *geometryShader, const char *fragmentShader );
	bool loadShaders(Strings *bindFragDatas, const char *fragmentShader);
	bool loadShaders(Strings *bindFragDatas, const char *vertexShader, const char *fragmentShader);
 bool loadShaders(Strings *bindFragDatas, const char *vertexShader, const char *geometryShader, const char *fragmentShader );
 bool loadShaders(Strings *bindFragDatas, const char *vertexShader, const char *controlShader, const char *evalShader, const char *geometryShader, const char *fragmentShader );

 virtual void Tint( Crayon *color ) {}
 
 void Uniform( GLuint &location, const char *symbol ) {
  location=glGetUniformLocation(program,(GLchar*)symbol);
  if ( location == -1 ) {
   OUTPUT("Program id %d:Uniform('%s') could not find symbol\n",(int)program,symbol);
  }
 }
  bool Attribute( GLint &location, const char *symbol ) {
  location=glGetAttribLocation(program,(GLchar*)symbol);
  if ( location == -1 ) {
   OUTPUT("Program id %d:Attribute('%s') could not find symbol\n",(int)program,symbol);
  }
 }


 bool Uniform( GLint *location, const char *symbol ) {
  *location=0;
  GLint result=glGetUniformLocation(program,(GLchar*)symbol);
  glReportError(glGetError());
  if ( result == -1 ) {
   OUTPUT("Program id %d:Uniform('%s') could not find symbol\n",(int)program,symbol);
   return false;
  }
  *location=result;
  return true;
 }
 bool Attribute( GLint *location, const char *symbol ) {
  *location=0;
  GLint result=glGetAttribLocation(program,(GLchar*)symbol);
  glReportError(glGetError());
  if ( result == -1 ) {
   OUTPUT("Program id %d:Attribute('%s') could not find symbol\n",(int)program,symbol);
   return false;
  }
  *location=result;
  return true;
 }
 bool Uniform( GLuint *location, const char *symbol ) {
  *location=0;
  GLuint result=glGetUniformLocation(program,(GLchar*)symbol);
  glReportError(glGetError());
  if ( result == -1 ) {
   OUTPUT("Program id %d:Uniform('%s') could not find symbol\n",(int)program,symbol);
   return false;
  }
  *location=(GLuint) result;
  return true;
 }
 bool Attribute( GLuint *location, const char *symbol ) {
  *location=0;
  GLuint result=glGetAttribLocation(program,(GLchar*)symbol);
  glReportError(glGetError());
  if ( result == -1 ) {
   OUTPUT("Program id %d:Attribute('%s') could not find symbol\n",(int)program,symbol);
   return false;
  }
  *location=(GLuint) result;
  return true;
 }

 virtual void InitUniforms() {}
 virtual void SetDefaults() {}
 virtual void UpdateUniforms() { glUseProgram(program); }
 // Override this if #version > 130, otherwise we map it to the default, and assuem we're using deprecated information.
 virtual void UpdateUniforms( glm::mat4 trans ) { UpdateUniforms(); }
 virtual void UpdateUniforms( glm::mat4 trans, Crayon tint ) { UpdateUniforms(); }
 virtual void UpdateUniforms( glm::mat4 trans, GLImage *tex, Crayon tint ) { UpdateUniforms(); }
 virtual void UpdateUniforms( glm::mat4 trans, GLuint texure, Crayon tint ) { UpdateUniforms(); }
 virtual void Disable() {
  glReportError(glGetError());
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
	void useShader();
	void disableShader();

 Blends blend;
 Cartesian area;
 void Render( GLWindow *surface ) {
#if defined(GL_DEBUG)
  GL_Assert( "fx_Shader2d::Render (entering)" );
  glReportError( glGetError() );
#endif
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  Blending(blend);
  UpdateUniforms();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(area.x2, area.y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(area.x, area.y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(area.x, area.y2); 
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(area.x2, area.y2);  
  glEnd();
  disableShader();
  glPopAttrib();
#if defined(GL_DEBUG)
  GL_Assert( "fx_Shader2d::Render" );
  glReportError( glGetError() );
#endif
 }


	GLhandleARB getProgram();

	void printInfoLog(const char *s, GLhandleARB obj);

	GLhandleARB program;
	GLhandleARB vShader;
	GLhandleARB fShader;
    GLhandleARB gShader;
    GLhandleARB tcShader;
    GLhandleARB teShader;

	int location0;
	int location1;
	int location2;
	int location3;
	int location4;
	int location5;
	int location6;
	int location7;
	int location8;
	int location9;

	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
	PFNGLGETHANDLEARBPROC glGetHandleARB;
	PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
	PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
	PFNGLUNIFORM1FARBPROC glUniform1fARB;
	PFNGLUNIFORM2FARBPROC glUniform2fARB;
	PFNGLUNIFORM3FARBPROC glUniform3fARB;
	PFNGLUNIFORM4FARBPROC glUniform4fARB;
	PFNGLUNIFORM1IARBPROC glUniform1iARB;
	PFNGLUNIFORM2IARBPROC glUniform2iARB;
	PFNGLUNIFORM3IARBPROC glUniform3iARB;
	PFNGLUNIFORM4IARBPROC glUniform4iARB;
	PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
	PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
	PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
	PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
	PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
	PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
	PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
	PFNGLUNIFORM4IVARBPROC glUniform4ivARB; 
	PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
	PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
	PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
	PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
	PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
	PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB;
	PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
	PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
	PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB;
	PFNGLGETUNIFORMIVARBPROC glGetUniformivARB;
	PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB;
};