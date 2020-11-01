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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class TexShader : public GLSLShader {
public:
 GLuint Utrans,Utex,Utint;
 GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  OUTPUT("TexShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec3 color;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec3 Color;\n")
   +string("out vec2 Texcoord;\n")
   +string("void main() {\n")
   +string(" Color = color;\n")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")
   +string("uniform sampler2D tex;\n")
   +string("uniform vec4 tint;\n")
   +string("void main() {\n")
   +string(" outColor = texture(tex,Texcoord)*tint*vec4(Color,1.0);\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utrans=glGetUniformLocation(program, "trans");
  Utex=glGetUniformLocation(program, "tex");
  Utint=glGetUniformLocation(program, "tint");    // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(program, "position");
  colAttrib = glGetAttribLocation(program, "color");
  texAttrib = glGetAttribLocation(program, "texcoord");
  glReportError(glGetError());
 }
 void UpdateUniforms( glm::mat4 trans, GLuint texture, Crayon tint ) {
  glReportError(glGetError());
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, texture);
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }
 void UpdateUniforms( glm::mat4 trans, GLImage *tex, Crayon tint ) {
  glReportError(glGetError());
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, tex->texture);
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
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

extern TexShader texShader;


class V2Shader : public GLSLShader {
public:
 GLuint Utrans,Utint;
 GLint posAttrib;
 void Init() {
  OUTPUT("V3Shader::Loading\n");
  string vert=
    string("#version 150\n")
   +string("in vec2 position;\n")
   +string("uniform mat4 trans;\n")
   +string("void main() {\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 150 core\n")
   +string("out vec4 outColor;\n")
   +string("uniform vec4 tint;\n")
   +string("void main() {\n")
   +string(" outColor = tint;\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utrans=glGetUniformLocation(program, "trans");
  Utint=glGetUniformLocation(program, "tint");    // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(program, "position");
 }
 void UpdateUniforms( glm::mat4 trans, Crayon tint ) {
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
 }
 void Disable() {
  glUseProgram(0);
 }
};

extern V2Shader v2Shader;

class V3Shader : public GLSLShader {
public:
 GLuint Utrans,Utint;
 GLint posAttrib;
 void Init() {
  OUTPUT("V3Shader::Loading\n");
  string vert=
    string("#version 150\n")
   +string("in vec3 position;\n")
   +string("uniform mat4 trans;\n")
   +string("void main() {\n")
   +string(" gl_Position = trans * vec4(position, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 150 core\n")
   +string("out vec4 outColor;\n")
   +string("uniform vec4 tint;\n")
   +string("void main() {\n")
   +string(" outColor = tint;\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utrans=glGetUniformLocation(program, "trans");
  Utint=glGetUniformLocation(program, "tint");    // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(program, "position");
 }
 void UpdateUniforms( glm::mat4 trans, Crayon tint ) {
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
 }
 void Disable() {
  glUseProgram(0);
 }
};

extern V3Shader v3Shader;

class VCShader : public GLSLShader {
public:
 GLuint Utrans,Utint;
 GLint posAttrib,colAttrib;
 void Init() {
  OUTPUT("VCShader::Loading\n");
  string vert=
    string("#version 150\n")
   +string("in vec3 position;\n")
   +string("in vec4 color;\n")
   +string("out vec4 Color;\n")
   +string("uniform mat4 trans;\n")
   +string("void main() {\n")
   +string(" Color = color;\n")
   +string(" gl_Position = trans * vec4(position, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 150 core\n")
   +string("in vec4 Color;\n")
   +string("out vec4 outColor;\n")
   +string("uniform vec4 tint;\n")
   +string("void main() {\n")
   +string(" outColor = tint;\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utrans=glGetUniformLocation(program, "trans");
  Utint=glGetUniformLocation(program, "tint");    // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(program, "position");
  colAttrib = glGetAttribLocation(program, "color");
 }
 void UpdateUniforms( glm::mat4 trans, Crayon tint ) {
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
 }
 void Disable() {
  glUseProgram(0);
 }
};

extern VCShader vcShader;



class TexTCShader : public GLSLShader {
public:
 GLuint Utrans,Utex,Utint,Utc;
 Cartesiand tc;
 GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  OUTPUT("TexTCShader::Loading\n");
  string vert=
    string("#version 150\n")
   +string("in vec2 position;\n")
   +string("in vec3 color;\n")
   +string("in vec2 texcoord;\n")
   +string("out vec3 Color;\n")
   +string("out vec2 Texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("uniform vec4 tc;\n")
   +string("void main() {\n")
   +string(" Color = color;\n")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 150 core\n")
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")
   +string("uniform sampler2D tex;\n")
   +string("uniform vec4 tint;\n")
   +string("uniform vec4 tc;\n")
   +string("void main() {\n")
   +string(" vec2 ntc=vec2(mix(tc.x,tc.z,Texcoord.x),mix(tc.y,tc.w,Texcoord.y));\n")
   +string(" outColor = texture(tex,ntc)*tint*vec4(Color,1.0);\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utrans=glGetUniformLocation(program, "trans");
  Utex=glGetUniformLocation(program, "tex");
  Utc=glGetUniformLocation(program,"tc");
  Utint=glGetUniformLocation(program, "tint");    // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(program, "position");
  colAttrib = glGetAttribLocation(program, "color");
  texAttrib = glGetAttribLocation(program, "texcoord");
  glReportError(glGetError());
 }
 void UpdateUniforms( glm::mat4 trans, GLuint texture, Crayon tint ) {
  glReportError(glGetError());
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, texture);
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
 }
 void UpdateUniforms( glm::mat4 trans, GLImage *tex, Crayon tint ) {
  glReportError(glGetError());
  glUseProgram(program);
  glUniform4fv(Utint,1,tint.floats);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glUniform1i(Utex, 0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, tex->texture);
  glUniform4f(Utc, (GLfloat)tc.x, (GLfloat)tc.y, (GLfloat)tc.x2, (GLfloat)tc.y2 );
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(colAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
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

extern TexTCShader texTCShader;