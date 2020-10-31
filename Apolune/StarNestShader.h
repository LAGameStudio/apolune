#pragma once
#include "Vertex.h"
#include "GLSetup.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"

void ShadedStarNest( float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out );        // draws to FBO
void ShadedStarNest2( float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out );       // draws to FBO
void ShadedStarNestSimple( float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out );  // draws to FBO
void ShadedStarNestSimple2(float time, float pos_x, float pos_y, float res_x, float res_y, FBOColor *out ); // draws to FBO

// Star Nest by Pablo Romn Andrioli
// Modified a lot.
// This content is under the MIT License.

class StarNestShader : public GLSLShader {
public:
 Zfloat time;
 Vertex position,resolution;
 GLuint Utime,Uposition,Uresolution;
 GLuint Utrans; GLint posAttrib,texAttrib;
 StarNestShader() : GLSLShader() {
  resolution.Set(1.0,1.0);
 }
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Uposition=glGetUniformLocation(program, "mouse");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
  glReportError(glGetError());
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Utime, (GLfloat) time.value);
  glUniform2f(Uposition,position.x,position.y);
  glUniform2f(Uresolution,resolution.x,resolution.y);

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
 }
 void Disable() {
  glUseProgram(0);
 }
};



class StarNest2Shader : public GLSLShader {
public:
 Zfloat time;
 Vertex position,resolution;
 GLuint Utime,Uposition,Uresolution;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Uposition=glGetUniformLocation(program, "mouse");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Utime, (GLfloat) time.value);
  glUniform2f(Uposition,position.x,position.y);
  glUniform2f(Uresolution,resolution.x,resolution.y);

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


// Star Nest by Pablo Romn Andrioli
// Modified a lot.
// This content is under the MIT License.

class StarNestSimpleShader : public GLSLShader {
public:
 Zfloat time;
 Vertex position,resolution;
 GLuint Utime,Uposition,Uresolution,Utex;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Uposition=glGetUniformLocation(program, "mouse");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utex=glGetUniformLocation(program,"tex");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Utime, (GLfloat) time.value);
  glUniform2f(Uposition,position.x,position.y);
  glUniform2f(Uresolution,resolution.x,resolution.y);

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


class StarNestSimple2Shader : public GLSLShader {
public:
 Zfloat time;
 Vertex position,resolution;
 GLuint Utime,Uposition,Uresolution;
 GLuint Utrans; GLint posAttrib,texAttrib;
 void Init();
 void InitUniforms() {
  Utime=glGetUniformLocation(program, "time");
  Uposition=glGetUniformLocation(program, "mouse");
  Uresolution=glGetUniformLocation(program,"resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Utime, (GLfloat) time.value);
  glUniform2f(Uposition,position.x,position.y);
  glUniform2f(Uresolution,resolution.x,resolution.y);

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

extern StarNestShader starNestShader;
extern StarNest2Shader starNest2Shader;
extern StarNestSimpleShader starNestSimpleShader;
extern StarNestSimple2Shader starNestSimple2Shader;