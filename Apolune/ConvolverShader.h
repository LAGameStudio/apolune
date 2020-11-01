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

void ShadedGaussian( GLuint *in, FBOColor *out );
void ShadedEmboss( GLuint *in, FBOColor *out );
void ShadedEmbossConvolveA( GLuint *in, FBOColor *out );
void ShadedEmbossPreserveA( GLuint *in, FBOColor *out );
void ShadedEdge( GLuint *in, FBOColor *out );
void ShadedSharpen( GLuint *in, FBOColor *out );
void ShadedSigma9Blur( GLuint *in, FBOColor *work, FBOColor *out );
void ShadedSigma7Blur( GLuint *in, FBOColor *work, FBOColor *out );
void ShadedSigma5Blur( GLuint *in, FBOColor *work, FBOColor *out );
void ShadedSigmaBlur( GLuint *in, FBOColor *work, FBOColor *out );

class EmbossShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("EmbossShader::Loading\n");
  pTex0=null;
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
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("float[9] k = float[] (2.,0.,0., 0., -1., 0., 0., 0., -1.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.),1.0);\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
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

extern EmbossShader embossShader;

class EmbossConvolveAShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("EmbossConvolveAShader::Loading\n");
  pTex0=null;
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
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("float[9] k = float[] (2.,0.,0., 0., -1., 0., 0., 0., -1.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.),Convolve(k,mata,1.,1./2.));\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
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

extern EmbossConvolveAShader embossconvolveAShader;

class EmbossPreserveAShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("EmbossPreserveAShader::Loading\n");
  pTex0=null;
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
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("vec4 here=texture2D(Texture0,Texcoord.xy);\n")+
   string("float[9] k = float[] (2.,0.,0., 0., -1., 0., 0., 0., -1.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.), Convolve(k,mata,1.,1./2.),here.a);\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
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

extern EmbossPreserveAShader embosspreserveAShader;

class SharpnessShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("SharpnessShader::Loading\n");
  pTex0=null;
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
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("float[9] k = float[] (-1.,-1.,-1., -1., 9., -1., -1., -1., -1.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,matr,1.,0.), Convolve(k,matg,1.,0.), Convolve(k,matb,1.,0.),1.0);\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SharpnessShader sharpnessShader;

class GaussianBlurShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("GaussianBlurShader::Loading\n");
  pTex0=null;
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
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("float[9] k = float[] (1.,2.,1., 2., 4., 2., 1., 2., 1.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,matr,16.,0.), Convolve(k,matg,16.,0.), Convolve(k,matb,16.,0.),1.0);\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern GaussianBlurShader gaussianblurShader;

class EdgeDetectShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 GLuint Utrans;
 GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  OUTPUT("EdgeDetectShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec3 color;")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec2 Texcoord;\n")
   +string("out vec3 Color;\n")
   +string("void main() {\n")
   +string(" Color = color;")
   +string(" Texcoord = texcoord;\n")
   +string(" gl_Position = trans * vec4(position, 0.0, 1.0);\n")
   +string("}\n")
  ;
  string frag=
    string("#version 330 core\n")
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("uniform sampler2D Texture0;")+
   string("vec4 get_pixel(in vec2 coords, in float dx, in float dy) { return texture2D(Texture0,coords + vec2(dx, dy)); }\n")+
   string("float Convolve(in float[9] kernel, in float[9] matrix, in float denom, in float offset) {\n")+
   string("float res = 0.0; for (int i=0; i<9; i++) { res += kernel[i]*matrix[i]; } return clamp(res/denom + offset,0.0,1.0); }\n")+
   string("float[9] GetData(in int channel) { float dxtex = 1.0 / float(textureSize(Texture0,0)); float dytex = 1.0 / float(textureSize(Texture0,0)); float[9] mat; int k = -1; for (int i=-1; i<2; i++) { for(int j=-1; j<2; j++) { k++; mat[k] = get_pixel(Texcoord.xy,float(i)*dxtex, float(j)*dytex)[channel]; } } return mat; }\n")+
   string("float[9] GetMean(in float[9] matr, in float[9] matg, in float[9] matb) { float[9] mat; for (int i=0; i<9; i++) { mat[i] = (matr[i]+matg[i]+matb[i])/3.; } return mat; }\n")+
   string("void main(void) {\n")+
   string("float[9] k = float[] (-1./8.,-1./8.,-1./8., -1./8., 1., -1./8., -1./8., -1./8., -1./8.);\n")+
   string("float matr[9] = GetData(0); float matg[9] = GetData(1); float matb[9] = GetData(2); float mata[9] = GetMean(matr,matg,matb);\n")+
   string("outColor = vec4(Convolve(k,mata,0.1,0.), Convolve(k,mata,0.1,0.), Convolve(k,mata,0.1,0.),1.0);\n")+
   string("}")
   ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "Texture0");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
  colAttrib = glGetAttribLocation(program, "color");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
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

extern EdgeDetectShader edgedetectShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaV9GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 9x9 is around 3 to 5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_height for a vertical blur.
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=5.0f;
  blurSize=1.0f/display.hf;
  OUTPUT("SigmaV9GaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
//#if defined(VERTICAL_BLUR_9)
   string("const float numBlurPixelsPerSide = 4.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(0.0f, 1.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaV9GaussianShader sigmaV9gaussianShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaH9GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 9x9 is around 3 to 5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_width for a horizontal blur, and
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=5.0f;
  blurSize=1.0f/display.wf;
  OUTPUT("SigmaH9GaussianShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec3 Color;\n")
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
   string("const float numBlurPixelsPerSide = 4.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(1.0f, 0.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaH9GaussianShader sigmaH9gaussianShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaV7GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 7x7 is around 2.5 to 4
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_height for a vertical blur.
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=4.0f;
  blurSize=1.0f/display.hf;
  OUTPUT("SigmaV7GaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
   string("const float numBlurPixelsPerSide = 3.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(0.0f, 1.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaV7GaussianShader sigmaV7gaussianShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaH7GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 7x7 is around 2.5 to 4
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_width for a horizontal blur
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=4.0f;
  blurSize=1.0f/display.wf;
  OUTPUT("SigmaH7GaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
   string("const float numBlurPixelsPerSide = 3.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(1.0f, 0.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaH7GaussianShader sigmaH7gaussianShader;


// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaV5GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 5x5 is around 2 to 3.5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_height for a vertical blur.
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=3.0f;
  blurSize=1.0f/display.hf;
  OUTPUT("SigmaV5GaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
//#if defined(VERTICAL_BLUR_9)
   string("const float numBlurPixelsPerSide = 2.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(0.0f, 1.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaV5GaussianShader sigmaV5gaussianShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaH5GaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 5x5 is around 2 to 3.5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_width for a horizontal blur, and
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=3.0f;
  blurSize=1.0f/display.hf;
  OUTPUT("SigmaH5GaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
   string("const float numBlurPixelsPerSide = 2.0f;\n")+
   string("const vec2  blurMultiplyVec      = vec2(1.0f, 0.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaH5GaussianShader sigmaH5gaussianShader;


// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaVGaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 9x9 is around 3 to 5
 // A good value for 7x7 is around 2.5 to 4
 // A good value for 5x5 is around 2 to 3.5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_height for a vertical blur.
 GLuint *pTex0;   // Texture that will be blurred by this shader
 float numBlurPixelsPerSide;
 GLuint Usigma,Utex0,UblurSize,UnumBlurPixelsPerSide;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=8.0f;
  blurSize=1.0f/display.hf;
  numBlurPixelsPerSide=8.0f;
  OUTPUT("SigmaVGaussianShader::Loading\n");
  string vert=
    string("#version 330\n")
   +string("in vec2 position;\n")
   +string("in vec2 texcoord;\n")
   +string("uniform mat4 trans;\n")
   +string("out vec3 Color;\n")
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
//#if defined(VERTICAL_BLUR_9)
   string("uniform float numBlurPixelsPerSide;\n")+
   string("const vec2  blurMultiplyVec      = vec2(0.0f, 1.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  UnumBlurPixelsPerSide=glGetUniformLocation(program, "numBlurPixelsPerSide");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  glUniform1f(UnumBlurPixelsPerSide,numBlurPixelsPerSide);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaVGaussianShader sigmaVgaussianShader;

// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
class SigmaHGaussianShader : public GLSLShader {
public:
 // The sigma value for the gaussian function: higher value means more blur
 // A good value for 9x9 is around 3 to 5
 // A good value for 7x7 is around 2.5 to 4
 // A good value for 5x5 is around 2 to 3.5
 float sigma; // ... play around with this based on what you need :)
 float blurSize;  // This should usually be equal to 1.0f / texture_pixel_width for a horizontal blur, and
 float numBlurPixelsPerSide;
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Usigma,Utex0,UblurSize,UnumBlurPixelsPerSide;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  sigma=8.0f;
  blurSize=1.0f/display.wf;
  numBlurPixelsPerSide=8.0f;
  OUTPUT("SigmaHGaussianShader::Loading\n");
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
   string("uniform float sigma;\n")+
   string("uniform float blurSize;\n")+ 
   string("uniform sampler2D blurSampler;\n")+
   string("const float pi = 3.14159265f;\n")+
   string("uniform float numBlurPixelsPerSide;\n")+
   string("const vec2  blurMultiplyVec      = vec2(1.0f, 0.0f);\n")+
   string("void main() {\n")+
   string(" vec3 incrementalGaussian;\n")+
   string(" incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);\n")+
   string(" incrementalGaussian.y = exp(-0.5f / (sigma * sigma));\n")+
   string(" incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;\n")+
   string(" vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n")+
   string(" float coefficientSum = 0.0f;\n")+
   string(" // Take the central sample first...\n")+
   string(" avgValue += texture2D(blurSampler, Texcoord.xy) * incrementalGaussian.x;\n")+
   string(" coefficientSum += incrementalGaussian.x;\n")+
   string(" incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" // Go through the remaining 8 vertical samples (4 on each side of the center)\n")+
   string(" for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) { \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy - i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   avgValue += texture2D(blurSampler, Texcoord.xy + i * blurSize * \n")+
   string("                         blurMultiplyVec) * incrementalGaussian.x;       \n")+
   string("   coefficientSum += 2 * incrementalGaussian.x;\n")+
   string("   incrementalGaussian.xy *= incrementalGaussian.yz;\n")+
   string(" }\n")+
   string(" outColor = avgValue / coefficientSum;\n")+
   string("}")
 ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "blurSampler");
  Usigma=glGetUniformLocation(program, "sigma");
  UblurSize=glGetUniformLocation(program, "blurSize");
  UnumBlurPixelsPerSide=glGetUniformLocation(program, "numBlurPixelsPerSide");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Usigma,sigma);
  glUniform1f(UblurSize,blurSize);
  glUniform1f(UnumBlurPixelsPerSide,numBlurPixelsPerSide);
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern SigmaHGaussianShader sigmaHgaussianShader;

//   -1 -1 -1
//   -1  8 -1
//   -1 -1 -1
class LaplacianEdgeShader : public GLSLShader {
public:
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Utex0,Uofs;
 GLfloat ofs[9*2];
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("LaplacianEdgeShader::Loading\n");
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
   string("uniform vec2 ofs[9];\n")+
   string("void main() {\n")+
   string(" vec4 sample[9];\n")+
   string(" for ( int i=0; i<9; i++) sample[i]=texture2D(tex0,Texcoord.st+ofs[i]);\n")+
   string(" outColor=(sample[4]*8.0)-(sample[0]+sample[1]+sample[2]+sample[3]+sample[5]+sample[6]+sample[7]+sample[8]);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Uofs=glGetUniformLocation(program, "ofs");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0); glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

  glUniform2fv(Uofs, 10, ofs);
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

//    -1 -1 -1       1 0 -1 
// H = 0  0  0   V = 1 0 -1
//     1  1  1       1 0 -1
class PrewittEdgeShader : public GLSLShader {
public:
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Utex0,Uofs;
 GLfloat ofs[9*2];
 GLuint Utrans;
 GLint posAttrib,colAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("PrewittEdgeShader::Loading\n");
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
   +string("out vec4 outColor;\n")+
   string("uniform sampler2D tex0;\n")+
   string("uniform vec2 ofs[9];\n")+
   string("void main() {\n")+
   string(" vec4 sample[9];\n")+
   string(" for ( int i=0; i<9; i++) sample[i]=texture2D(tex0,Texcoord.st+ofs[i]);\n")+
   string(" vec4 He=(sample[2]+sample[5]+sample[8]) - (sample[0]+sample[3]+sample[6]);\n")+
   string(" vec4 Ve=(sample[0]+sample[1]+sample[2]) - (sample[6]+sample[7]+sample[8]);\n")+
   string(" outColor.rgb=sqrt((He.rgb*He.rgb) + (Ve.rgb*Ve.rgb));\n")+
   string(" outColor.a=1.0;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Uofs=glGetUniformLocation(program, "ofs");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  colAttrib = glGetAttribLocation(program, "color");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

  glUniform2fv(Uofs, 10, ofs);

  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
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

//    -1 -2 -1       1 0 -1 
// H = 0  0  0   V = 2 0 -2
//     1  2  1       1 0 -1
class SobelEdgeShader : public GLSLShader {
public:
 GLuint *pTex0;   // Texture that will be blurred by this shader
 GLuint Utex0,Uofs;
 GLfloat ofs[9*2];
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  OUTPUT("SobelEdgeShader::Loading\n");
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
   string("uniform vec2 ofs[9];\n")+
   string("void main() {\n")+
   string(" vec4 sample[9];\n")+
   string(" for ( int i=0; i<9; i++) sample[i]=texture2D(tex0,Texcoord.st+ofs[i]);\n")+
   string(" vec4 He=(sample[2]+2.0*sample[5]+sample[8]) - (sample[0]+2.0*sample[3]+sample[6]);\n")+
   string(" vec4 Ve=(sample[0]+2.0*sample[1]+sample[2]) - (sample[6]+2.0*sample[7]+sample[8]);\n")+
   string(" outColor.rgb=sqrt((He.rgb*He.rgb) + (Ve.rgb*Ve.rgb));\n")+
   string(" outColor.a=1.0;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Uofs=glGetUniformLocation(program, "ofs");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

  glUniform2fv(Uofs, 10, ofs);

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

class NormalMapShader : public GLSLShader {
public:
 float resolution[2];
 float coef;
 GLuint *pTex0;
 GLuint Utex0,Ures,Ucoef;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  coef=1.0f;
  resolution[0]=resolution[1]=512.0;
  OUTPUT("NormalMapShader::Loading\n");
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
   string("uniform float coef;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
//   string("vec2 packNormal( in vec3 _normal ) {\n")+
//   string(" const float _scale = 1.7777;\n")+
//   string(" float scalar1 = (_normal.z+1.0)*(_scale*2.0);\n")+
//   string(" return _normal.xy / scalar1 + 0.5;\n")+
//   string("}\n")+
   string("float packNormal(float n) {	return (n + 1) / 2; }\n")+
   string("void main() {\n")+
   string(" float dU=1/resolution.x; float dV=1/resolution.y;\n")+
   string(" vec2 tc=Texcoord.xy;\n")+
   string(" float s0 = texture2D(tex0, tc).r;\n")+
   string("	float s1 = texture2D(tex0, vec2(tc.x - dU, tc.y)).r;\n")+
   string("	float s2 = texture2D(tex0, vec2(tc.x, tc.y - dV)).r;\n")+
   string("	float s3 = texture2D(tex0, vec2(tc.x + dU, tc.y)).r;\n")+
   string("	float s4 = texture2D(tex0, vec2(tc.x, tc.y + dV)).r;\n")+
   string(" vec2 n=normalize( vec2( (s1-s3), (s2-s4) ) ) * coef;\n")+
   string("outColor = vec4( packNormal(n.x), packNormal(n.y), packNormal(1.0), 0.0 );\n")+
   string("}\n")
  ;
//  OUTPUT(frag.c_str());
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Ucoef=glGetUniformLocation(program, "coef");
  Ures=glGetUniformLocation(program, "resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ucoef,coef);
  glUniform2f(Ures,resolution[0],resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern NormalMapShader normalmapShader;

/*
 * Same as above but puts height into alpha channel.
 */
class NormalMapHeightShader : public GLSLShader {
public:
 float resolution[2];
 float coef;
 GLuint *pTex0;
 GLuint Utex0,Ures,Ucoef;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  pTex0=null;
  coef=1.0f;
  resolution[0]=resolution[1]=512.0;
  OUTPUT("NormalMapHeightShader::Loading\n");
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
   string("uniform float coef;\n")+
   string("uniform vec2 resolution;\n")+
   string("uniform sampler2D tex0;\n")+
   string("float packNormal(float n) {	return (n + 1) / 2; }\n")+
   string("void main() {\n")+
   string(" float dU=1/resolution.x; float dV=1/resolution.y;\n")+
   string(" vec2 tc=Texcoord.xy;\n")+
   string(" float s0 = texture2D(tex0, tc).r;\n")+
   string("	float s1 = texture2D(tex0, vec2(tc.x - dU, tc.y)).r;\n")+
   string("	float s2 = texture2D(tex0, vec2(tc.x, tc.y - dV)).r;\n")+
   string("	float s3 = texture2D(tex0, vec2(tc.x + dU, tc.y)).r;\n")+
   string("	float s4 = texture2D(tex0, vec2(tc.x, tc.y + dV)).r;\n")+
   string(" vec2 n=normalize( vec2( (s1-s3), (s2-s4) ) ) * coef;\n")+
   string("outColor = vec4( packNormal(n.x), packNormal(n.y), packNormal(1.0), tc.r );\n")+
   string("}\n")
  ;
//  OUTPUT(frag.c_str());
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Utex0=glGetUniformLocation(program, "tex0");
  Ucoef=glGetUniformLocation(program, "coef");
  Ures=glGetUniformLocation(program, "resolution");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ucoef,coef);
  glUniform2f(Ures,resolution[0],resolution[1]);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);

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

extern NormalMapHeightShader normalmapheightShader;

class NormalMap3dShader : public GLSLShader {
public:
 /*
vert:
varying vec3 normal;
  
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; 
    normal = gl_NormalMatrix * gl_Normal;
}
frag:
varying vec3 normal;
  
void main()
{
    vec3 normal_normal = normalize(normal);
    outColor = vec4(normal_normal, 1.0);
}
  */
};