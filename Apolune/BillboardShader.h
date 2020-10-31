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
#include "GLSL2Shader.h"

// Source: http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Billboards
class BillboardShader : public GLSLShader {
public:
 GLuint *pTex0;
 GLuint Utex0;
 void Init() {
  pTex0=null;
  Utex0=0;
  OUTPUT("BillboardShader::Loading\n");
  string vert=
   string("#version 120\n")+
   string("varying vec2 UV;\n")+
   string("void main(void) {\n")+
   string(" gl_Position=gl_ProjectionMatrix*(gl_ModelViewMatrix*vec4(0,0,0,1)+vec4(gl_Vertex.x+0.5, gl_Vertex.y+0.5, 0,0));\n")+
   string(" UV=vec2(gl_Vertex.x+0.5,gl_Vertex.y+0.5);\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D tex0;\n")+
   string("varying vec2 UV;\n")+
   string("void main(void) {\n")+
   string(" gl_FragColor=texture2D(tex0,UV);\n")+
   string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&Utex0,"tex0");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
};

extern BillboardShader billboardShader;

// Source: glslang-library vertdisplace
class VertexDisplaceShader : public GLSLShader {
public:
 GLuint *pTex0;
 float scale;
 GLuint Utex0,Uscale;
 void Init() {
  pTex0=null;
  scale=0.0f;
  Uscale=Utex0=0;
  OUTPUT("VertexDisplaceShader::Loading\n");
  string vert=
   string("#version 120\n")+
   string("uniform float scale;\n")+
   string("varying vec2 UV;\n")+
   string("void main(void) {\n")+
   string("	vec3 axis1 = vec3(	gl_ModelViewMatrix[0][0],\n")+
   string("	gl_ModelViewMatrix[1][0],\n")+
   string("	gl_ModelViewMatrix[2][0]);\n")+
   string("	vec3 axis2 = vec3(	gl_ModelViewMatrix[0][1],\n")+
   string("	gl_ModelViewMatrix[1][1],\n")+
   string("	gl_ModelViewMatrix[2][1]);\n")+
   string("	vec3 corner = (gl_Vertex.x*axis1 + gl_Vertex.y*axis2)*scale + gl_Normal;\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * vec4(corner, 1.);\n")+
   string("	UV = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);\n")+
   string("	gl_FrontColor = gl_Color;\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D tex0;\n")+
   string("varying vec2 UV;\n")+
   string("void main(void) {\n")+
   string(" gl_FragColor=texture2D(tex0,UV);\n")+
   string("}")
  ;
  loadShaders(vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&Utex0,"tex0");
  Uniform(&Uscale,"scale");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Uscale,scale);
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *pTex0);
 }
};

extern VertexDisplaceShader vertexdisplaceShader;