#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"

class SAI2xShader : public GLSLShader {
public:
 float size[2];
 GLuint *pTex0;
 GLint Utex0,Uwh,Utc;
 void Init() {
  pTex0=null;
  size[0]=512.0f;
  size[1]=512.0f;
  OUTPUT("SAI2xShader::Loading\n");
  string frag=
//   string("precision mediump float;\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D s_texture;\n")+
   string("uniform vec2 wh;\n")+
   string("void main() {\n")+
   string("	vec2 UL, UR, DL, DR;\n")+
   string(" float dx = pow(wh.x, -1.0) * 0.25;\n")+
   string(" float dy = pow(wh.y, -1.0) * 0.25;\n")+
   string(" vec3 dt = vec3(1.0, 1.0, 1.0);\n")+
   string(" UL =   gl_TexCoord[0] + vec2(-dx, -dy);\n")+
   string(" UR =   gl_TexCoord[0] + vec2( dx, -dy);\n")+
   string(" DL =   gl_TexCoord[0] + vec2(-dx,  dy);\n")+
   string(" DR =   gl_TexCoord[0] + vec2( dx,  dy);\n")+
   string(" vec3 c00 = texture2D(s_texture, UL).xyz;\n")+
   string(" vec3 c20 = texture2D(s_texture, UR).xyz;\n")+
   string(" vec3 c02 = texture2D(s_texture, DL).xyz;\n")+
   string(" vec3 c22 = texture2D(s_texture, DR).xyz;\n")+
   string(" float m1=dot(abs(c00-c22),dt)+0.001;\n")+
   string(" float m2=dot(abs(c02-c20),dt)+0.001;\n")+
   string(" gl_FragColor = vec4((m1*(c02+c20)+m2*(c22+c00))/(2.0*(m1+m2)),1.0);\n")+
   string("}\n")
  ;
  loadShaders(null,(char *) frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uwh=glGetUniformLocation(program,"wh");
  Utex0=glGetUniformLocation(program,"s_texture");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uwh, size[0], size[1]);
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern SAI2xShader sai2xShader;

class SAI2x2Shader : public GLSLShader {
public:
 float size[2];
 GLuint *pTex0;
 GLint Utex0,Uwh;
 void Init() {
  pTex0=null;
  size[0]=512.0f;
  size[1]=512.0f;
  OUTPUT("SAI2x2Shader::Loading\n");
  string frag=
//   string("precision mediump float;\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D s_texture;\n")+
   string("uniform vec2 wh;\n")+
   string("void main()\n")+
   string("{\n")+
   string(" vec3 E = texture2D(s_texture, gl_TexCoord[0]).xyz;\n")+
   string(" vec3 D = texture2D(s_texture, gl_TexCoord[1]).xyz;\n")+
   string(" vec3 F = texture2D(s_texture, gl_TexCoord[2]).xyz;\n")+
   string(" vec3 H = texture2D(s_texture, gl_TexCoord[3]).xyz;\n")+
   string(" vec3 B = texture2D(s_texture, gl_TexCoord[4]).xyz;\n")+
   string(" vec3 dt = vec3(1.0,1.0,1.0);\n")+
   string(" float k1=dot(abs(D-F),dt);\n")+
   string(" float k2=dot(abs(H-B),dt);\n")+
   string(" vec3 color = (k1*(H+B)+k2*(D+F)+0.001*E)/(2.0*(k1+k2)+0.001);\n")+
   string(" gl_FragColor.xyz = color;\n")+
   string("}\n")
  ;
  loadShaders(null,(char *) frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uwh=glGetUniformLocation(program,"wh");
  Utex0=glGetUniformLocation(program,"s_texture");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Uwh, size[0], size[1]);
  glUniform1i(Utex0, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

// not yet complete (SAI2x2Shader)