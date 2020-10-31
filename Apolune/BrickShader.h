#pragma once
#include "GLSetup.h"
#include "GLSL2Shader.h"

// Orange Book, Section 6: Procedural Brick Shader
// Vertex shader for procedural bricks
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Darwyn Peachey
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd. 
// See 3Dlabs-License.txt for license information

class BrickShader: public GLSLShader {
public:
 float position[3];
 float brick[3];
 float size[2];
 float ratio[2];
 float mortar[3];
 GLuint Uposition,Ubrick,Usize,Uratio,Umortar;
 void Init() {
  position[0]=0.0f;
  position[1]=0.0f;
  position[2]=0.0f;
  brick[0]=1.0f;
  brick[0]=0.0f;
  brick[0]=0.0f;
  size[0]=0.15f;
  size[1]=0.3f;
  ratio[0]=0.9f;
  ratio[1]=0.85f;
  mortar[0]=0.75f;
  mortar[1]=0.6f;
  mortar[2]=0.65f;
  OUTPUT("BrickShader::Loading\n");
  string vert=
   string("uniform vec3 LightPosition;\n")+
   string("const float SpecularContribution = 0.3;\n")+
   string("const float DiffuseContribution  = 1.0 - SpecularContribution;\n")+
   string("varying float LightIntensity;\n")+
   string("varying vec2  MCposition;\n")+
   string("void main()\n")+
   string("{\n")+
   string("    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);\n")+
   string("    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string("    vec3 lightVec   = normalize(LightPosition - ecPosition);\n")+
   string("    vec3 reflectVec = reflect(-lightVec, tnorm);\n")+
   string("    vec3 viewVec    = normalize(-ecPosition);\n")+
   string("    float diffuse   = max(dot(lightVec, tnorm), 0.0);\n")+
   string("    float spec      = 0.0;\n")+
   string("    if (diffuse > 0.0)\n")+
   string("    {\n")+
   string("        spec = max(dot(reflectVec, viewVec), 0.0);\n")+
   string("        spec = pow(spec, 16.0);\n")+
   string("    }\n")+
   string("    LightIntensity  = DiffuseContribution * diffuse +\n")+
   string("                      SpecularContribution * spec;\n")+
   string("    MCposition      = gl_Vertex.xy;\n")+
   string("    gl_Position     = ftransform();\n")+
   string("}\n")
  ;
  string frag=
   string("uniform vec3  BrickColor, MortarColor;\n")+
   string("uniform vec2  BrickSize;\n")+
   string("uniform vec2  BrickPct;\n")+
   string("varying vec2  MCposition;\n")+
   string("varying float LightIntensity;\n")+
   string("void main()\n")+
   string("{\n")+
   string("    vec3  color;\n")+
   string("    vec2  position, useBrick;\n")+
   string("    position = MCposition / BrickSize;\n")+
   string("    if (fract(position.y * 0.5) > 0.5)\n")+
   string("        position.x += 0.5;\n")+
   string("    position = fract(position);\n")+
   string("    useBrick = step(position, BrickPct);\n")+
   string("    color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);\n")+
   string("    color *= LightIntensity;\n")+
   string("    gl_FragColor = vec4(color, 1.0);\n")+
   string("}\n")
  ;
  loadShaders((char *) vert.c_str(),(char *) frag.c_str());
  InitUniforms();
 }

 void InitUniforms() {
  Uposition=glGetUniformLocation(program,"LightPosition");
  Ubrick=glGetUniformLocation(program,"BrickColor");
  Usize=glGetUniformLocation(program,"BrickSize");
  Uratio=glGetUniformLocation(program,"BrickPct");
  Umortar=glGetUniformLocation(program,"MortarColor");
 }

 void UpdateUniforms() {
  glUseProgram(program);
  glUniform3f(Uposition,position[0],position[1],position[2]);
  glUniform3f(Ubrick,brick[0],brick[1],brick[2]);
  glUniform3f(Umortar,mortar[0],mortar[1],mortar[2]);
  glUniform2f(Usize,size[0],size[1]);
  glUniform2f(Uratio,ratio[0],ratio[1]);
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern BrickShader brickShader;