#pragma once
#include "Crayon.h"
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

class HQ2xShader : public GLSLShader {
public:
 float size[2];
 GLuint *pTex0,*pTex1;
 GLuint Utex0,Utex1,Usize;

 void Init() {
  size[0]=512.0f;
  size[1]=512.0f;
  OUTPUT("SharpenShader::Loading\n");
  string frag=
//string("precision mediump float;\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform sampler2D s_texture;\n")+
   string("uniform sampler2D s_palette;\n")+
   string("uniform vec2 size;\n")+
   string(" const float mx = 0.325;      // start smoothing wt.\n")+
   string(" const float k = -0.250;      // wt. decrease factor\n")+
   string(" const float max_w = 0.25;    // max filter weigth\n")+
   string(" const float min_w =-0.05;    // min filter weigth\n")+
   string(" const float lum_add = 0.25;  // effects smoothing\n")+
   string("void main()\n")+
   string("{\n")+
   string(" float x = 0.5 * (1.0 / size.x);\n")+
   string(" float y = 0.5 * (1.0 / size.y);\n")+
   string(" vec2 dg1 = vec2( x, y);\n")+
   string(" vec2 dg2 = vec2(-x, y);\n")+
   string(" vec2 dx = vec2(x, 0.0);\n")+
   string(" vec2 dy = vec2(0.0, y);\n")+
   string(" vec4 TexCoord[5];\n")+
   string(" TexCoord[0] = vec4(gl_TexCoord[0].xy, 0.0, 0.0);\n")+
   string(" TexCoord[1].xy = TexCoord[0].xy - dg1;\n")+
   string(" TexCoord[1].zw = TexCoord[0].xy - dy;\n")+
   string(" TexCoord[2].xy = TexCoord[0].xy - dg2;\n")+
   string(" TexCoord[2].zw = TexCoord[0].xy + dx;\n")+
   string(" TexCoord[3].xy = TexCoord[0].xy + dg1;\n")+
   string(" TexCoord[3].zw = TexCoord[0].xy + dy;\n")+
   string(" TexCoord[4].xy = TexCoord[0].xy + dg2;\n")+
   string(" TexCoord[4].zw = TexCoord[0].xy - dx;\n")+
   string(" vec3 c00 = texture2D(s_texture, TexCoord[1].xy).xyz;\n")+
   string(" vec3 c10 = texture2D(s_texture, TexCoord[1].zw).xyz;\n")+
   string(" vec3 c20 = texture2D(s_texture, TexCoord[2].xy).xyz;\n")+
   string(" vec3 c01 = texture2D(s_texture, TexCoord[4].zw).xyz;\n")+
   string(" vec3 c11 = texture2D(s_texture, TexCoord[0].xy).xyz;\n")+
   string(" vec3 c21 = texture2D(s_texture, TexCoord[2].zw).xyz;\n")+
   string(" vec3 c02 = texture2D(s_texture, TexCoord[4].xy).xyz;\n")+
   string(" vec3 c12 = texture2D(s_texture, TexCoord[3].zw).xyz;\n")+
   string(" vec3 c22 = texture2D(s_texture, TexCoord[3].xy).xyz;\n")+
   string(" vec3 dt = vec3(1.0, 1.0, 1.0);\n")+
   string(" float md1 = dot(abs(c00 - c22), dt);\n")+
   string(" float md2 = dot(abs(c02 - c20), dt);\n")+
   string(" float w1 = dot(abs(c22 - c11), dt) * md2;\n")+
   string(" float w2 = dot(abs(c02 - c11), dt) * md1;\n")+
   string(" float w3 = dot(abs(c00 - c11), dt) * md2;\n")+
   string(" float w4 = dot(abs(c20 - c11), dt) * md1;\n")+
   string(" float t1 = w1 + w3;\n")+
   string(" float t2 = w2 + w4;\n")+
   string(" float ww = max(t1, t2) + 0.0001;\n")+
   string(" c11 = (w1 * c00 + w2 * c20 + w3 * c22 + w4 * c02 + ww * c11) / (t1 + t2 + ww);\n")+
   string(" float lc1 = k / (0.12 * dot(c10 + c12 + c11, dt) + lum_add);\n")+
   string(" float lc2 = k / (0.12 * dot(c01 + c21 + c11, dt) + lum_add);\n")+
   string(" w1 = clamp(lc1 * dot(abs(c11 - c10), dt) + mx, min_w, max_w);\n")+
   string(" w2 = clamp(lc2 * dot(abs(c11 - c21), dt) + mx, min_w, max_w);\n")+
   string(" w3 = clamp(lc1 * dot(abs(c11 - c12), dt) + mx, min_w, max_w);\n")+
   string(" w4 = clamp(lc2 * dot(abs(c11 - c01), dt) + mx, min_w, max_w);\n")+
   string(" gl_FragColor = vec4(w1 * c10 + w2 * c21 + w3 * c12 + w4 * c01 + (1.0 - w1 - w2 - w3 - w4) * c11, 1.0);\n")+
   string("}\n")
  ;
  loadShaders(null,(char *) frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"s_texture");
  Utex1=glGetUniformLocation(program,"s_palette");
  Usize=glGetUniformLocation(program,"size");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform2f(Usize, size[0], size[1]);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *pTex1);
 }

 void Disable() {
  glActiveTexture(GL_TEXTURE0);	
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern HQ2xShader hq2xShader;