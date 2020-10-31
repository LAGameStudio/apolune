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

void ShadedStencilR( GLuint *in, GLuint *stencilR, FBOColor *out );
void ShadedStencilRG( GLuint *in, GLuint *stencilR, GLuint *stencilG, FBOColor *out );
void ShadedStencilRGB( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, FBOColor *out );
void ShadedStencilRGBA( GLuint *in, GLuint *stencilR, GLuint *stencilG, GLuint *stencilB, GLuint *stencilA, FBOColor *out );

class StencilShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilShader::Loading\n");
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
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor= fr*f0.r;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
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

extern StencilShader stencilShader;

class Stencil2Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilShader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" outColor= fr*f0.r +fg*f0.g;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
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

extern Stencil2Shader stencil2Shader;

class Stencil3Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("Stencil3Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" outColor= fr*f0.r +fg*f0.g +fb*f0.b;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
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

extern Stencil3Shader stencil3Shader;

class Stencil4Shader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("Stencil4Shader::Loading\n");
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
   +string("in vec3 Color;\n")
   +string("in vec2 Texcoord;\n")
   +string("out vec4 outColor;\n")+
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 fa=texture2D(tex4,Texcoord.xy);\n")+
   string(" outColor= fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
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

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);

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

extern Stencil4Shader stencil4Shader;

class StencilAddShader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilAddShader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
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

extern StencilAddShader stencilAddShader;

class StencilAdd2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilAdd2Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
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

extern StencilAdd2Shader stencilAdd2Shader;

class StencilAdd3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilAdd3Shader::Loading\n");
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
   +string("out vec4 outColor;\n")+   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g +fb*f0.b;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
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

extern StencilAdd3Shader stencilAdd3Shader;

class StencilAdd4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilAdd4Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;     // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 fa=texture2D(tex4,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
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
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);

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

extern StencilAdd4Shader stencilAdd4Shader;

class StencilAvg2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilAvg2Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g)/2.0;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
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

extern StencilAvg2Shader stencilAvg2Shader;

class StencilAvg3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilAvg3Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g +fb*f0.b)/3.0;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
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

extern StencilAvg3Shader stencilAvg3Shader;

class StencilAvg4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilAvg4Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 fa=texture2D(tex4,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r +fg*f0.g +fb*f0.b +fa*f0.a)/4.0;\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
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
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);

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

extern StencilAvg4Shader stencilAvg4Shader;

class StencilMult2Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2;
 GLint Utex0, Utex1, Utex2, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  OUTPUT("StencilMult2Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r) * (fg*f0.g);\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
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

extern StencilMult2Shader stencilMult2Shader;

class StencilMult3Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 GLint Utex0, Utex1, Utex2, Utex3, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  OUTPUT("StencilMult3Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r)*(fg*f0.g)*(fb*f0.b);\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utex2=glGetUniformLocation(program,"tex2");
  Utex3=glGetUniformLocation(program,"tex3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
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

extern StencilMult3Shader stencilMult3Shader;

class StencilMult4Shader : public GLSLShader {
public:
 Crayon color;
 GLuint *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 GLint Utex0, Utex1, Utex2, Utex3, Utex4, Ucolor;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  pTex2=null;
  pTex3=null;
  pTex4=null;
  OUTPUT("StencilMult4Shader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform vec4 color;             // final color to multiply by\n")+
   string("uniform sampler2D tex0; // special stencil\n")+
   string("uniform sampler2D tex1; // R value, where 0=transparent and 1=opaque\n")+
   string("uniform sampler2D tex2; // G value\n")+
   string("uniform sampler2D tex3; // B value\n")+
   string("uniform sampler2D tex4; // A value\n")+
   string("void main() {\n")+
   string(" vec4 f0=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fr=texture2D(tex1,Texcoord.xy);\n")+
   string(" vec4 fg=texture2D(tex2,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex3,Texcoord.xy);\n")+
   string(" vec4 fa=texture2D(tex4,Texcoord.xy);\n")+
   string(" vec4 stenciled;\n")+
   string(" stenciled= (fr*f0.r)*(fg*f0.g)*(fb*f0.b)*(fa*f0.a);\n")+
   string(" outColor=stenciled*color;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Ucolor=glGetUniformLocation(program,"color");
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
  
  glUniform4f(Ucolor,color.rf,color.gf,color.bf,color.af);

  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
  glUniform1i(Utex2, 2);
  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *pTex2);
  glUniform1i(Utex3, 3);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex3);
  glUniform1i(Utex4, 4);
  glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *pTex4);

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

extern StencilMult4Shader stencilMult4Shader;

// Where tex0 is multiplied by tex1 preserving the alpha channel (0 on either = transparent)
class StencilMult2AlphaShader : public GLSLShader {
public:
 GLuint *pTex0,*pTex1;
 GLint Utex0, Utex1;
 GLint posAttrib,texAttrib,Utrans;
 void Init() {
  pTex0=null;
  pTex1=null;
  OUTPUT("StencilMult2AlphaShader::Loading\n");
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
   string("// (c)2012 H. Elwood Gilliland III - All rights reserved.\n")+
   string("// Performs color-based stenciling operations and other effects in real-time\n")+
   string("uniform sampler2D tex0; // A\n")+
   string("uniform sampler2D tex1; // B\n")+
   string("void main() {\n")+
   string(" vec4 fa=texture2D(tex0,Texcoord.xy);\n")+
   string(" vec4 fb=texture2D(tex1,Texcoord.xy);\n")+
   string(" outColor=fa*fb;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"tex0");
  Utex1=glGetUniformLocation(program,"tex1");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  
  // Assign texture IDs used by glActiveTexture
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glUniform1i(Utex1, 1);
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
//  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern StencilMult2AlphaShader stencilMult2AlphaShader;