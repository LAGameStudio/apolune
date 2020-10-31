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
#include "Crayon.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

// Turned off, wasn't fully working when I did but was bound correctly.
// Not needed (deprecated / obsolete) use glLineWidth and glSmooth

#if defined(NEVER)
class WireframeShader : public GLSLShader {
public:
 Crayon wire,fill;
 float scale[2];

 GLuint
  Uwire,Ufill,Uscale;

 void Init() {
  wire.Float(1.0f,0.0f,0.0f,1.0f);
  fill.Pick(alabaster);
  scale[0]=512.0f;
  scale[1]=512.0f;
  OUTPUT("WireframeShader::Loading\n");
  string vert=file_as_string("data/shaders/wireframe.vert");
  string geo=file_as_string("data/shaders/wireframe.geo");
  string frag=file_as_string("data/shaders/wireframe.frag");
  loadShaders(
   vert.c_str(),
   geo.c_str(),
   frag.c_str()
  );
  InitUniforms();
 }

 void InitUniforms() {
  Uwire=glGetUniformLocation(program,"wire");
  Ufill=glGetUniformLocation(program,"fill");
  Uscale=glGetUniformLocation(program,"scale");
 }

 void UpdateUniforms() {
  
  glUniform2f(Uscale, scale[0], scale[1]);
  glUniform4f(Uwire,wire.rf,wire.gf,wire.bf,wire.af);
  glUniform4f(Ufill,fill.rf,fill.gf,fill.bf,fill.af);
 }

 void Disable() {
 }
};

extern WireframeShader wireframeShader;
#endif