#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"
#include "TextureLibrary.h"


class SharpenShader: public GLSLShader {
public:
 float sharpen; // best results between 10.0 and 50.0
 GLuint *pTex0;
 GLuint Utex0, Usharpen;

 void Init() {
  sharpen=30.0f;
  OUTPUT("SharpenShader::Loading\n");
  string frag=
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform float Sharpenfactor;");
   string("uniform sampler2D s_texture;\n")+
   string("void main()\n")+
   string("{\n")+
   string(" vec4 Color = texture2D( s_texture, gl_TexCoord[0].xy );\n")+
   string(" Color.rgb -= texture2D( s_texture, v_texCoord.xy+0.0001).rgb*Sharpenfactor;\n")+
   string(" Color.rgb += texture2D( s_texture, v_texCoord.xy-0.0001).rgb*Sharpenfactor;\n")+
   string(" gl_FragColor = Color;\n")+
   string("}\n")
  ;
  loadShaders(null,(char *) frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  Utex0=glGetUniformLocation(program,"s_texture");
  Usharpen=glGetUniformLocation(program,"Sharpenfactor");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform1f(Usharpen, sharpen);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *pTex0);
 }

 void Disable() {
  glActiveTexture(GL_TEXTURE0);	
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern SharpenShader sharpenShader;