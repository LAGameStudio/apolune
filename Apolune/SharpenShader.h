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