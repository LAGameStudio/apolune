#pragma once
/**************************************************************************************************************************************************************************************************************************
*                                                                       **                                                                       **                                                                       *
*               Copyright (C) 2002-2006  3Dlabs Inc. Ltd.               **             Copyright (C) 2002-2006  LightWork Design Ltd.            **              Copyright (C) 2003-2005  ATI Research, Inc.              *
*                                                                       **                                                                       **                                                                       *
*                        All rights reserved.                           **                        All rights reserved.                           **                        All rights reserved.                           *
*                                                                       **                                                                       **                                                                       *
* Redistribution and use in source and binary forms, with or without    ** Redistribution and use in source and binary forms, with or without    ** Redistribution and use in source and binary forms, with or without    *
* modification, are permitted provided that the following conditions    ** modification, are permitted provided that the following conditions    ** modification, are permitted provided that the following conditions    *
* are met:                                                              ** are met:                                                              ** are met:                                                              *
*                                                                       **                                                                       **                                                                       *
*     Redistributions of source code must retain the above copyright    **     Redistributions of source code must retain the above copyright    **     Redistributions of source code must retain the above copyright    *
*     notice, this list of conditions and the following disclaimer.     **     notice, this list of conditions and the following disclaimer.     **     notice, this list of conditions and the following disclaimer.     *
*                                                                       **                                                                       **                                                                       *
*     Redistributions in binary form must reproduce the above           **     Redistributions in binary form must reproduce the above           **     Redistributions in binary form must reproduce the above           *
*     copyright notice, this list of conditions and the following       **     copyright notice, this list of conditions and the following       **     copyright notice, this list of conditions and the following       *
*     disclaimer in the documentation and/or other materials provided   **     disclaimer in the documentation and/or other materials provided   **     disclaimer in the documentation and/or other materials provided   *
*     with the distribution.                                            **     with the distribution.                                            **     with the distribution.                                            *
*                                                                       **                                                                       **                                                                       *
*     Neither the name of 3Dlabs Inc. Ltd. nor the names of its         **     Neither the name of LightWork Design Ltd. nor the names of its    **     Neither the name of ATI Research, Inc. nor the names of its       *
*     contributors may be used to endorse or promote products derived   **     contributors may be used to endorse or promote products derived   **     contributors may be used to endorse or promote products derived   *
*     from this software without specific prior written permission.     **     from this software without specific prior written permission.     **     from this software without specific prior written permission.     *
*                                                                       **                                                                       **                                                                       *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        ** FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        ** FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, ** COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, ** COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, *
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  ** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  ** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      ** BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      ** BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      ** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      ** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    ** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    ** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     ** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     ** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       ** ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       ** ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                           ** POSSIBILITY OF SUCH DAMAGE.                                           ** POSSIBILITY OF SUCH DAMAGE.                                           *
*                                                                       **                                                                       **                                                                       *
**************************************************************************************************************************************************************************************************************************/

#include "GLSetup.h"
#include "Art.h"
#include "Vertex.h"
#include "Crayon.h"
#include "GLSL2Shader.h"

/// Authors: John Kessenich, Randi Rost
// if reflectDir.z >= 0.0, s will go from 0.25 to 0.75
// if reflectDir.z <  0.0, s will go from 0.75 to 1.25, and
// set the texture to wrap.
class EnvMapShader : public GLSLShader {
public:
 Vertex lightPosition;
 Crayon color;
 float mix;
 GLuint *environment;
 GLuint Uenvironment,UlightPosition,Ucolor,Umix;
 GLuint Utrans; GLint posAttrib, texAttrib;
 void Init() {
  lightPosition.Set( 0.0f, 5.0f, 0.0f );
  mix=0.5f;
  environment=null;
  string vert=
   string("#version 120\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying float LightIntensity;\n")+
   string("uniform vec3  LightPos;\n")+
   string("void main() {\n")+
   string(" gl_Position    = ftransform();\n")+
   string(" Normal         = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec4 pos       = gl_ModelViewMatrix * gl_Vertex;\n")+
   string(" EyeDir         = pos.xyz;\n")+
   string(" LightIntensity = max(dot(normalize(LightPos - EyeDir), Normal), 0.0);\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("const vec3 Xunitvec = vec3(1.0, 0.0, 0.0);\n")+
   string("const vec3 Yunitvec = vec3(0.0, 1.0, 0.0);\n")+
   string("uniform vec3  BaseColor;\n")+
   string("uniform float MixRatio;\n")+
   string("uniform sampler2D EnvMap;\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying float LightIntensity;\n")+
   string("void main() {\n")+
   string(" vec3 reflectDir = reflect(EyeDir, Normal);\n")+
   string(" vec2 index;\n")+
   string(" index.y = dot(normalize(reflectDir), Yunitvec);\n")+
   string(" reflectDir.y = 0.0;\n")+
   string(" index.x = dot(normalize(reflectDir), Xunitvec) * 0.5;\n")+
   string(" if (reflectDir.z >= 0.0) index = (index + 1.0) * 0.5;\n")+
   string(" else {\n")+
   string("  index.t = (index.t + 1.0) * 0.5;\n")+
   string("  index.s = (-index.s) * 0.5 + 1.0;\n")+
   string(" }\n")+
   string(" vec3 envColor = vec3(texture2D(EnvMap, index));\n")+
   string(" vec3 base = LightIntensity * BaseColor;\n")+
   string(" envColor = mix(envColor, base, MixRatio);\n")+
   string(" gl_FragColor = vec4(envColor, 1.0);\n")+
   string("}\n")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&UlightPosition,"LightPos");
  Uniform(&Ucolor,"BaseColor");
  Uniform(&Umix,"MixRatio");
  Uniform(&Uenvironment,"EnvMap");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUniform3f(UlightPosition,lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform3f(Ucolor,color.rf,color.gf,color.bf);
  glUniform1f(Umix,mix);
		glUniform1i(Uenvironment, 0); glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, *environment);
 } 
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};


/// P. Rideout
class ToonShader : public GLSLShader {
public:
 Crayon diffuse,phong;
 float edge,highlight;
 GLuint Udiffuse,Uphong,Uedge,Uhighlight;
 GLuint Utrans; GLint posAttrib, texAttrib;
 void Init() {
  diffuse.Float(0.8f,0.8f,0.8f,1.0f);
  phong.Float(1.0f,1.0f,0.0f,0.0f);
  edge=0.1f;
  highlight=0.5f;
  Udiffuse=Uphong=Uedge=Uhighlight=0;
  string vert=
   string("#version 120\n")+
   string("varying vec3 Normal;\n")+
   string("void main() {\n")+
   string("	Normal = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform vec3 DiffuseColor;\n")+
   string("uniform vec3 PhongColor;\n")+
   string("uniform float Edge;\n")+
   string("uniform float Phong;\n")+
   string("varying vec3 Normal;\n")+
   string("void main () {\n")+
   string("	vec3 color = DiffuseColor;\n")+
   string("	float f = dot(vec3(0,0,1),Normal);\n")+
   string("	if (abs(f) < Edge)	color = vec3(0);\n")+
   string("	if (f > Phong)	color = PhongColor;\n")+
   string("	gl_FragColor = vec4(color, 1);\n")+
   string("}\n")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&Udiffuse,"DiffuseColor");
  Uniform(&Uphong,"PhongColor");
  Uniform(&Uedge,"Edge");
  Uniform(&Uhighlight,"Phong");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(Udiffuse,diffuse.rf,diffuse.gf,diffuse.bf);
  glUniform3f(Uphong,phong.rf,phong.gf,phong.bf);
  glUniform1f(Uedge,edge);
  glUniform1f(Uhighlight,highlight);
 }
};

/// Antonio Tejada
class WobbleShader : public GLSLShader {
public:
 Vertex lightPosition;
 float rad;
 float freq[2];
 float amp[2];
 GLuint UlightPosition,Urad,Ufreq,Uamp;
 GLuint Utrans; GLint posAttrib, texAttrib;
 void Init() {
  lightPosition.Set(0.0f,5.0f,1.0f);
  rad=0.25f;
  freq[0]=0.002f;
  freq[1]=0.02f;
  amp[0]=0.5f;
  amp[1]=1.0f;
  string vert=
   string("#version 120\n")+
   string("varying float LightIntensity;\n")+
   string("uniform vec3 LightPosition;\n")+
   string("const float specularContribution = 0.1;\n")+
   string("const float diffuseContribution  = 1.0 - specularContribution;\n")+
   string("void main() {\n")+
   string(" vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);\n")+
   string(" vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec3 lightVec   = normalize(LightPosition - ecPosition);\n")+
   string(" vec3 reflectVec = reflect(-lightVec, tnorm);\n")+
   string(" vec3 viewVec    = normalize(-ecPosition);\n")+
   string(" float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);\n")+
   string(" spec            = pow(spec, 16.0);\n")+
   string(" LightIntensity  = diffuseContribution * max(dot(lightVec, tnorm), 0.0) + specularContribution * spec;\n")+
   string(" gl_TexCoord[0]  = gl_MultiTexCoord0;\n")+
   string(" gl_Position     = ftransform();\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("const float C_PI    = 3.1415;\n")+
   string("const float C_2PI   = 2.0 * C_PI;\n")+
   string("const float C_2PI_I = 1.0 / (2.0 * C_PI);\n")+
   string("const float C_PI_2  = C_PI / 2.0;\n")+
   string("varying float LightIntensity;\n")+
   string("uniform float StartRad;\n")+
   string("uniform vec2  Freq;\n")+
   string("uniform vec2  Amplitude;\n")+
   string("uniform sampler2D WobbleTex;\n")+
   string("void main () {\n")+
   string(" vec2  perturb;\n")+
   string(" float rad;\n")+
   string(" vec4  color;\n")+
   string(" rad = (gl_TexCoord[0].s + gl_TexCoord[0].t - 1.0 + StartRad) * Freq.x;\n")+
   string(" rad = rad * C_2PI_I;\n")+
   string(" rad = fract(rad);\n")+
   string(" rad = rad * C_2PI;\n")+
   string(" if (rad >  C_PI) rad = rad - C_2PI;\n")+
   string(" if (rad < -C_PI) rad = rad + C_2PI;\n")+
   string(" if (rad >  C_PI_2) rad =  C_PI - rad;\n")+
   string(" if (rad < -C_PI_2) rad = -C_PI - rad;\n")+
   string(" perturb.x  = (rad - (rad * rad * rad / 6.0)) * Amplitude.x;\n")+
   string(" rad = (gl_TexCoord[0].s - gl_TexCoord[0].t + StartRad) * Freq.y;\n")+
   string(" rad = rad * C_2PI_I;\n")+
   string(" rad = fract(rad);\n")+
   string(" rad = rad * C_2PI;\n")+
   string(" if (rad >  C_PI) rad = rad - C_2PI;\n")+
   string(" if (rad < -C_PI) rad = rad + C_2PI;\n")+
   string(" if (rad >  C_PI_2) rad =  C_PI - rad;\n")+
   string(" if (rad < -C_PI_2) rad = -C_PI - rad;\n")+
   string(" perturb.y  = (rad - (rad * rad * rad / 6.0)) * Amplitude.y;\n")+
   string(" color = texture2D(WobbleTex, perturb + gl_TexCoord[0].st);\n")+
   string(" gl_FragColor = vec4(color.rgb * LightIntensity, color.a);\n")+
   string("}\n")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&UlightPosition,"LightPosition");
  Uniform(&Urad,"StartRad");
  Uniform(&Ufreq,"Freq");
  Uniform(&Uamp,"Amp");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UlightPosition,lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(Urad,rad);
  glUniform2f(Ufreq,freq[0],freq[1]);
  glUniform2f(Uamp,amp[0],amp[1]);
 }
};

#if defined(NEVER)
// This one requires a 3d texture, but no infrastructure is
// currently written to support one.  Incomplete. TODO
// Randi Rost
class ErodedShader : public GLSLShader {
public:
 Vertex lightPosition;
 float scale;
 Vertex offset;

 void Init() {
  string vert=
   string("#version 120\n")+
   string("varying float lightIntensity;\n")+
   string("varying vec3 Position;\n")+
   string("uniform vec3 LightPosition;\n")+
   string("uniform float Scale;\n")+
   string("void main() {\n")+
   string(" vec4 pos = gl_ModelViewMatrix * gl_Vertex;\n")+
   string(" Position = vec3(gl_Vertex) * Scale;\n")+
   string(" vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" float dotval = max(dot(normalize(LightPosition - vec3(pos)), tnorm), 0.0);\n")+
   string(" lightIntensity = dotval * 1.5;\n")+
   string(" gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("varying float lightIntensity;\n")+
   string("varying vec3 Position;\n")+
   string("uniform vec3 Offset;\n")+
   string("uniform sampler3D sampler3d;\n")+
   string("void main () {\n")+
   string(" vec4 noisevec;\n")+
   string(" vec3 color;\n")+
   string(" float intensity;\n")+
   string(" noisevec = texture3D(sampler3d, 1.2 * (vec3 (0.5) + Position));\n")+
   string(" intensity = 0.75 * (noisevec.x + noisevec.y + noisevec.z + noisevec.w);\n")+
   string(" intensity = 1.95 * abs(2.0 * intensity - 1.0);\n")+
   string(" intensity = clamp(intensity, 0.0, 1.0);\n")+
   string(" if (intensity < fract(0.5-Offset.x-Offset.y-Offset.z)) discard;\n")+
   string(" color = mix(vec3 (0.2, 0.1, 0.0), vec3(0.8, 0.8, 0.8), intensity);\n")+
   string(" color *= lightIntensity;\n")+
   string(" color = clamp(color, 0.0, 1.0); \n")+
   string(" gl_FragColor = vec4 (color, 1.0);\n")+
   string("}\n")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void Disable() {
  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
 }
};
#endif

/// Randi Rhost
class GoochShader : public GLSLShader {
public:
 Vertex lightPosition;
 Crayon surface,warm,cool;
 float diffuse[2]; // warm, cool
 GLuint UlightPosition,Usurface,Uwarm,Ucool,Udiffuse0,Udiffuse1;
 GLuint Utrans; GLint posAttrib, texAttrib;
 void Init() {
  lightPosition.Set(0.0f,10.0f,4.0f);
  surface.Float(0.75f,0.75f,0.75f,1.0f);
  warm.Float(0.6f,0.6f,0.0f,1.0f);
  cool.Float(0.0f,0.0f,0.6f,1.0f);
  diffuse[0]=0.45f;
  diffuse[1]=0.45f;
  string vert=
   string("#version 120\n")+
   string("uniform vec3  LightPosition;\n")+
   string("varying float NdotL;\n")+
   string("varying vec3  ReflectVec;\n")+
   string("varying vec3  ViewVec;\n")+
   string("void main() {\n")+
   string(" vec3 ecPos      = vec3 (gl_ModelViewMatrix * gl_Vertex);\n")+
   string(" vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec3 lightVec   = normalize(LightPosition - ecPos);\n")+
   string(" ReflectVec      = normalize(reflect(-lightVec, tnorm));\n")+
   string(" ViewVec         = normalize(-ecPos);\n")+
   string(" NdotL           = (dot(lightVec, tnorm) + 1.0) * 0.5;\n")+
   string(" gl_Position     = ftransform();\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
    string("uniform vec3  SurfaceColor;\n")+
    string("uniform vec3  WarmColor;\n")+
    string("uniform vec3  CoolColor;\n")+
    string("uniform float DiffuseWarm;\n")+
    string("uniform float DiffuseCool;\n")+
    string("varying float NdotL;\n")+
    string("varying vec3  ReflectVec;\n")+
    string("varying vec3  ViewVec;\n")+
    string("void main () {\n")+
    string(" vec3 kcool    = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);\n")+
    string(" vec3 kwarm    = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);\n")+
    string(" vec3 kfinal   = mix(kcool, kwarm, NdotL);\n")+
    string(" vec3 nreflect = normalize(ReflectVec);\n")+
    string(" vec3 nview    = normalize(ViewVec);\n")+
    string(" float spec    = max(dot(nreflect, nview), 0.0);\n")+
    string(" spec          = pow(spec, 32.0);\n")+
    string(" gl_FragColor = vec4 (min(kfinal + spec, 1.0), 1.0);\n")+
    string("}")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&UlightPosition,"LightPosition");
  Uniform(&Usurface,"SurfaceColor");
  Uniform(&Uwarm,"WarmColor");
  Uniform(&Ucool,"CoolColor");
  Uniform(&Udiffuse0,"DiffuseWarm");
  Uniform(&Udiffuse1,"DiffuseCool");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UlightPosition,lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform3f(Usurface,surface.rf,surface.gf,surface.bf);
  glUniform3f(Uwarm,warm.rf,warm.gf,warm.bf);
  glUniform3f(Ucool,cool.rf,cool.gf,cool.bf);
  glUniform1f(Udiffuse0,diffuse[0]);
  glUniform1f(Udiffuse1,diffuse[1]);
 }
};

/// Jon Kennedy
class HeatHaze : public GLSLShader {
public:
 float resolution[2];
 float freq,offset,fade;
 int speed;
 GLuint *tex0;
 GLuint
  Ures0,Ures1,Ufreq,Uoffset,Ufade,Uspeed,Utex0;
 GLuint Utrans; GLint posAttrib, texAttrib;
 void Init() {
  Ures0=Ures1=Ufreq=Uoffset=Ufade=Uspeed=Utex0=0;
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  tex0=null;
  freq=0.5f;
  offset=0.0f;
  fade=0.1f;
  speed=2;
  string vert=
   string("#version 120\n")+
   string("varying vec4  EyePos;\n")+
   string("void main() {\n")+
   string(" gl_Position = ftransform();\n")+
   string(" EyePos      = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform float FrameWidth;\n")+
   string("uniform float FrameHeight;\n")+
   string("uniform float Frequency;\n")+
   string("uniform float Offset;\n")+
   string("uniform float Fade;\n")+
   string("uniform int   Speed;\n")+
   string("uniform sampler2D FrameBuffer;\n")+
   string("varying vec4  EyePos;\n")+
   string("void main () {\n")+
   string(" vec2 index;\n")+
   string(" float recipW = 1.0 / EyePos.w;\n")+
   string(" vec2 eye = EyePos.xy * vec2(recipW);\n")+
   string(" float blend = max(1.0 - (eye.y + Fade), 0.0);\n")+
   string(" index.s = eye.x ;\n")+
   string(" index.t = eye.y + blend * sin(Frequency * 5.0 * eye.y + Offset * float(Speed)) * 0.06;\n")+
   string(" index.s = index.s / 2.0 + 0.5;\n")+
   string(" index.t = index.t / 2.0 + 0.5;\n")+
   string(" float recip1k = 1.0 / 2048.0;\n")+
   string(" index.s = clamp(index.s, 0.0, 1.0 - recip1k);\n")+
   string(" index.t = clamp(index.t, 0.0, 1.0 - recip1k);\n")+
   string(" index.s = index.s * FrameWidth * recip1k;\n")+
   string(" index.t = index.t * FrameHeight * recip1k;\n")+
   string(" vec3 RefractionColor = vec3 (texture2D(FrameBuffer, index));\n")+
   string(" gl_FragColor = vec4 (RefractionColor, 1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&Ures0,"FrameWidth");
  Uniform(&Ures1,"FrameHeight");
  Uniform(&Ufreq,"Frequency");
  Uniform(&Uoffset,"Offset");
  Uniform(&Ufade,"Fade");
  Uniform(&Uspeed,"Speed");
  Uniform(&Utex0,"FrameBuffer");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform1f(Ures0,resolution[0]);
  glUniform1f(Ures1,resolution[1]);
  glUniform1f(Ufreq,freq);
  glUniform1f(Uoffset,offset);
  glUniform1f(Ufade,fade);
  glUniform1i(Uspeed,speed);
  glUniform1i(Utex0,0); glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,*tex0);
 }
};

#if defined(NEVER)
// This shader uses a 3d texture sampler, but no
// infrastructure yet exists to handle this.
// Incomplete.  TODO
/// Randi Rost
class InfernoShader : public GLSLShader {
public:
 GLuint
  Uposition,Uoffset,Ucolor1,Ucolor2,Uscale,Usampler3d;
 void Init() {
  string vert=
   string("#version 120\n")+
   string("varying vec3 Position;\n")+
   string("uniform float Scale;\n")+
   string("void main() {\n")+
   string("	Position = vec3(gl_Vertex) * Scale;\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("varying vec3 Position;\n")+
   string("uniform float Offset;\n")+
   string("uniform vec3 FireColor1;\n")+
   string("uniform vec3 FireColor2;\n")+
//   string("uniform bool Teapot;\n")+
   string("uniform float Extent;\n")+
   string("uniform sampler3D sampler3d;\n")+
   string("void main () {\n")+
   string(" vec4 noisevec;\n")+
   string(" vec3 color;\n")+
   string(" float intensity;\n")+
   string(" float alpha;\n")+
   string(" noisevec = texture3D(sampler3d, Position);\n")+
   string(" noisevec = texture3D(sampler3d, vec3 (Position.x+noisevec[1], Position.y-noisevec[3]+Offset, Position.z+noisevec[1]));\n")+
   string(" intensity = 0.75 * (noisevec[0] + noisevec[1] + noisevec[2] + noisevec[3]);\n")+
   string(" intensity = 1.95 * abs(2.0 * intensity - 0.35);\n")+
   string(" intensity = clamp(intensity, 0.0, 1.0);\n")+
   string(" alpha = fract((Position.y+Extent)*0.65);\n")+
   string(" color = mix(FireColor1, FireColor2, intensity) * (1.0 - alpha) * 2.0;\n")+
   string(" color = clamp(color, 0.0, 1.0);\n")+
   string(" alpha = 1.0 - alpha  * intensity;\n")+
   string(" alpha *= alpha;\n")+
   string(" gl_FragColor = vec4(color, alpha);\n")+
   string("}\n")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
 }
 void UpdateUniforms( glm::mat4 trans ) {
 }
};

class FireShader {
 [vert]
//
// Vertex shader for producing a fire effect
//
// Author: Randi Rost
//
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity;
varying vec3  MCposition;

uniform vec3  LightPosition;
uniform float Scale;

void main(void)
{
    vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
    MCposition      = vec3 (gl_Vertex) * Scale;
    vec3 tnorm      = normalize(vec3 (gl_NormalMatrix * gl_Normal));
    LightIntensity  = dot(normalize(LightPosition - vec3 (ECposition)), tnorm) * 1.5;
    gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
}

 [frag]
//
// Fragment shader for procedural fire effect
//
// Author: Randi Rost
//
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity; 
varying vec3  MCposition;

uniform sampler3D Noise;
uniform vec3 Color1;       // (0.8, 0.7, 0.0)
uniform vec3 Color2;       // (0.6, 0.1, 0.0)
uniform float NoiseScale;  // 1.2

uniform vec3 Offset;

void main (void)
{
    vec4 noisevec = texture3D(Noise, NoiseScale * (MCposition + Offset));


    float intensity = abs(noisevec.x - 0.25) +
                      abs(noisevec.y - 0.125) +
                      abs(noisevec.z - 0.0625) +
                      abs(noisevec.w - 0.03125);

    intensity    = clamp(intensity * 6.0, 0.0, 1.0);
    vec3 color   = mix(Color1, Color2, intensity) * LightIntensity;
    color = clamp(color, 0.0, 1.0);
    gl_FragColor = vec4 (color, 1.0);
}

};


class GraniteShader {
 [vert]
//
// Vertex shader for producing a granite effect
//
// Authors: John Kessenich, Randi Rost
//
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity;
varying vec3  MCposition;

uniform vec3  LightPos;
uniform float Scale;

void main(void)
{
    vec3 ECposition = vec3 (gl_ModelViewMatrix * gl_Vertex);
    MCposition      = vec3 (gl_Vertex) * Scale;
    vec3 tnorm      = normalize(vec3 (gl_NormalMatrix * gl_Normal));
    LightIntensity  = dot(normalize(LightPos - ECposition), tnorm);
    LightIntensity *= 1.5;
    gl_Position     = ftransform();
}
 [frag]
 //
// Fragment shader for producing a granite effect
//
// Author: Randi Rost
//
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity; 
varying vec3  MCposition;

uniform sampler3D Noise;
uniform float NoiseScale;

void main(void)
{
    vec4  noisevec  = texture3D(Noise, NoiseScale * MCposition);
    float intensity = min(1.0, noisevec[3] * 18.0);
    vec3  color     = vec3 (intensity * LightIntensity);
    
    color = clamp(color, 0.0, 1.0); // clamp not needed for book shader

    gl_FragColor    = vec4 (color, 1.0);
}
};
#endif


/// Author: Jon Kennedy, based on the envmap shader by John Kessenich, Randi Rost
// if reflectDir.z >= 0.0, s will go from 0.25 to 0.75
// if reflectDir.z <  0.0, s will go from 0.75 to 1.25, and
// that's OK, because we've set the texture to wrap.
class GlassShader : public GLSLShader {
public:
 Vertex lightPosition;
 Crayon color;
 float resolution[2];
 float depth,mix;
 GLuint *environment,*refraction;
 GLuint UlightPosition, Ucolor, Ures0, Ures1, Udepth, Umix,Uenvironment,Urefraction;
 void Init() {
  UlightPosition=Ucolor=Ures0=Ures1=Udepth=Umix=Uenvironment=Urefraction=0;
  resolution[0]=display.wf;
  resolution[1]=display.hf;
  depth=0.4f;
  mix=0.5f;
  color.Float(0.6f,0.6f,0.0f,1.0f);
  environment=refraction=null;
  string vert=
   string("#version 120\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying vec4  EyePos;\n")+
   string("varying float LightIntensity;\n")+
   string("uniform vec3  LightPos;\n")+
   string("void main() {\n")+
   string(" gl_Position = ftransform();\n")+
   string(" Normal      = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec4 pos    = gl_ModelViewMatrix * gl_Vertex;\n")+
   string(" EyeDir      = pos.xyz;\n")+
   string(" EyePos		    = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string(" LightIntensity = max(dot(normalize(LightPos - EyeDir), Normal), 0.0);\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("const vec3 Xunitvec = vec3 (1.0, 0.0, 0.0);\n")+
   string("const vec3 Yunitvec = vec3 (0.0, 1.0, 0.0);\n")+
   string("uniform vec3  BaseColor;\n")+
   string("uniform float Depth;\n")+
   string("uniform float MixRatio;\n")+
   string("uniform float FrameWidth;\n")+
   string("uniform float FrameHeight;\n")+
   string("uniform sampler2D EnvMap;\n")+
   string("uniform sampler2D RefractionMap;\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying vec4  EyePos;\n")+
   string("varying float LightIntensity;\n")+
   string("void main () {\n")+
   string(" vec3 reflectDir = reflect(EyeDir, Normal);\n")+
   string(" vec2 index;\n")+
   string(" index.y = dot(normalize(reflectDir), Yunitvec);\n")+
   string(" reflectDir.y = 0.0;\n")+
   string(" index.x = dot(normalize(reflectDir), Xunitvec) * 0.5;\n")+
   string(" if (reflectDir.z >= 0.0) index = (index + 1.0) * 0.5;\n")+
   string(" else {\n")+
   string("  index.t = (index.t + 1.0) * 0.5;\n")+
   string("  index.s = (-index.s) * 0.5 + 1.0;\n")+
   string(" }\n")+
   string(" vec3 envColor = vec3 (texture2D(EnvMap, index));\n")+
   string(" float fresnel = abs(dot(normalize(EyeDir), Normal));\n")+
   string(" fresnel *= MixRatio;\n")+
   string(" fresnel = clamp(fresnel, 0.1, 0.9);\n")+
   string(" vec3 refractionDir = normalize(EyeDir) - normalize(Normal);\n")+
   string("	float depthVal = Depth / -refractionDir.z;\n")+
   string("	float recipW = 1.0 / EyePos.w;\n")+
   string("	vec2 eye = EyePos.xy * vec2(recipW);\n")+
   string("	index.s = (eye.x + refractionDir.x * depthVal);\n")+
   string("	index.t = (eye.y + refractionDir.y * depthVal);\n")+
   string("	index.s = index.s / 2.0 + 0.5;\n")+
   string("	index.t = index.t / 2.0 + 0.5;\n")+
   string("	float recip1k = 1.0 / 2048.0;\n")+
   string("	index.s = clamp(index.s, 0.0, 1.0 - recip1k);\n")+
   string("	index.t = clamp(index.t, 0.0, 1.0 - recip1k);\n")+
   string("	index.s = index.s * FrameWidth * recip1k;\n")+
   string("	index.t = index.t * FrameHeight * recip1k;\n")+
   string(" vec3 RefractionColor = vec3 (texture2D(RefractionMap, index));\n")+
   string(" vec3 base = LightIntensity * BaseColor;\n")+
   string(" envColor = mix(envColor, RefractionColor, fresnel);\n")+
   string(" envColor = mix(envColor, base, 0.2);\n")+
   string(" gl_FragColor = vec4 (envColor, 1.0);\n")+
   string("}");
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&UlightPosition,"LightPosition");
  Uniform(&Ucolor,"BaseColor");
  Uniform(&Ures0,"FrameWidth");
  Uniform(&Ures1,"FrameHeight");
  Uniform(&Udepth,"Depth");
  Uniform(&Umix,"MixRatio");
  Uniform(&Uenvironment,"EnvMap");
  Uniform(&Urefraction,"RefractionMap");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UlightPosition,lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform3f(Ucolor,color.rf,color.gf,color.bf);
  glUniform1f(Ures0,resolution[0]);
  glUniform1f(Ures1,resolution[1]);
  glUniform1f(Udepth,depth);
  glUniform1f(Umix,mix);
  glUniform1i(Uenvironment,0); glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *environment);
  glUniform1i(Urefraction,0);  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *refraction);
 }
};

#if defined (NEVER)
// Requires 3d textre noise infrastructure TODO
/// Authors: John Kessenich, Randi Rost
class Marble {
 [vert]
 //
// Vertex shader for producing a marble effect
//
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd. 
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity;
varying vec3  MCposition;

uniform vec3  LightPos;
uniform float Scale;

void main()
{
    vec3 ECposition = vec3(gl_ModelViewMatrix * gl_Vertex);
    MCposition      = vec3(gl_Vertex) * Scale;
    vec3 tnorm      = normalize(vec3(gl_NormalMatrix * gl_Normal));
    LightIntensity  = dot(normalize(LightPos - ECposition), tnorm);
    LightIntensity *= 1.5;
    gl_Position     = ftransform();
}
[frag]
//
// Fragment shader for producing a granite effect
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd. 
//
// See 3Dlabs-License.txt for license information
//

varying float LightIntensity; 
varying vec3  MCposition;

uniform sampler3D Noise;
uniform vec3 MarbleColor;
uniform vec3 VeinColor;

void main()
{
    vec4 noisevec   = texture3D(Noise, MCposition);

    float intensity = abs(noisevec[0] - 0.25) +
                      abs(noisevec[1] - 0.125) +
                      abs(noisevec[2] - 0.0625) +
                      abs(noisevec[3] - 0.03125);

    float sineval = sin(MCposition.y * 6.0 + intensity * 12.0) * 0.5 + 0.5;
    vec3 color    = mix(VeinColor, MarbleColor, sineval) * LightIntensity;
    gl_FragColor  = vec4(color, 1.0);
}
};

/// Author: Bert Freudenberg <bert@isg.cs.uni-magdeburg.de>
class HatchShaderH : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version #120\n")+
uniform vec3  LightPosition;
uniform float Time;
varying vec3  ObjPos;
varying float V;
varying float LightIntensity;
void main() {
 ObjPos          = (vec3(gl_Vertex) + vec3(0.0, 0.0, Time)) * 0.2;
 vec3 pos        = vec3(gl_ModelViewMatrix * gl_Vertex);
 vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
 vec3 lightVec   = normalize(LightPosition - pos);
 LightIntensity  = max(dot(lightVec, tnorm), 0.0);
 V = gl_MultiTexCoord0.t;  // try .s for vertical stripes
 gl_Position = ftransform();
}
[frag]
//
// Fragment shader for procedurally generated hatching or "woodcut" appearance.
//
// This is an OpenGL 2.0 implementation of Scott F. Johnston's "Mock Media"
// (from "Advanced RenderMan: Beyond the Companion" SIGGRAPH 98 Course Notes)
//
// Author: Bert Freudenberg <bert@isg.cs.uni-magdeburg.de>
//
// Copyright (c) 2002-2003 3Dlabs, Inc. 
//
// See 3Dlabs-License.txt for license information
//

const float frequency = 1.0;

varying vec3  ObjPos;               // object space coord (noisy)
varying float V;                    // generic varying
varying float LightIntensity;

uniform sampler3D Noise;            // value of Noise = 3;
uniform float Swidth;               // relative width of stripes = 16.0

void main()
{
    float dp       = length(vec2(dFdx(V * Swidth), dFdy(V * Swidth)));
    float logdp    = -log2(dp);
    float ilogdp   = floor(logdp);
    float stripes  = exp2(ilogdp);

    float noise    = texture3D(Noise, ObjPos).x;

    float sawtooth = fract((V + noise * 0.1) * frequency * stripes);
    float triangle = abs(2.0 * sawtooth - 1.0);

    // adjust line width
    float transition = logdp - ilogdp;

    // taper ends
    triangle = abs((1.0 + transition) * triangle - transition);

    const float edgew = 0.2;            // width of smooth step

    float edge0  = clamp(LightIntensity - edgew, 0.0, 1.0);
    float edge1  = clamp(LightIntensity, 0.0, 1.0);
    float square = 1.0 - smoothstep(edge0, edge1, triangle);

    gl_FragColor = vec4(vec3(square), 1.0);
}
};
#endif

/// Author: John Kessenich
class Wood1Shader : public GLSLShader {
public:
 Vertex lightPosition;
 float scale;
 float grain;
 Crayon color;
 Vertex spread;
 GLuint UlightPosition,Uscale,Ugrain,Ucolor,Uspread;
 void Init() {
  UlightPosition=Uscale=Ugrain=Ucolor=Uspread=0;
  lightPosition.Set(0.0f,5.0f,0.0f);
  scale=0.8f;
  grain=0.5f;
  color.Pick(macNcheese);
  spread.Set(0.5f,0.5f,0.5f);
  string vert=
   string("#version 120\n")+
   string("varying float lightIntensity;\n")+
   string("varying vec3 Position;\n")+
   string("uniform vec3 LightPosition;\n")+
   string("uniform float Scale;\n")+
   string("void main() {\n")+
   string("	vec4 pos = gl_ModelViewMatrix * gl_Vertex;\n")+
   string("	Position = vec3(gl_Vertex) * Scale;\n")+
   string("	vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string("	lightIntensity = max(dot(normalize(LightPosition - vec3(pos)), tnorm), 0.0) * 1.5;\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform float GrainSizeRecip;\n")+
   string("uniform vec3  DarkColor;\n")+
   string("uniform vec3  spread;\n")+
   string("varying float lightIntensity; \n")+
   string("varying vec3 Position;\n")+
   string("void main () {\n")+
   string(" vec3 location = Position;\n")+
   string(" vec3 floorvec = vec3(floor(10.0 * Position.x), 0.0, floor(10.0 * Position.z));\n")+
   string(" vec3 noise = Position * 10.0 - floorvec - 0.5;\n")+
   string(" noise *= noise;\n")+
   string(" location += noise * 0.12;\n")+
   string(" float dist = location.x * location.x + location.z * location.z;\n")+
   string(" float grain = dist * GrainSizeRecip;\n")+
   string(" float brightness = fract(grain);\n")+
   string(" if (brightness > 0.5) brightness = (1.0 - brightness);\n")+
   string(" vec3 color = DarkColor + brightness * spread;\n")+
   string(" brightness = fract(grain * 7.0);\n")+
   string(" if (brightness > 0.5) brightness = 1.0 - brightness;\n")+
   string(" color -= brightness * spread;\n")+
   string(" brightness = fract(grain * 47.0) * 0.60;\n")+
   string(" float line = fract(Position.z + Position.x);\n")+
   string(" float snap = floor(line * 20.0) * (1.0/20.0);\n")+
   string(" if (line < snap + 0.006)\n")+
   string(" color -= brightness * spread;\n")+
   string(" color = clamp(color * lightIntensity, 0.0, 1.0);\n")+
   string(" gl_FragColor = vec4(color, 1.0);\n")+
   string("}")
  ;
  loadShaders( vert.c_str(), frag.c_str() );
  InitUniforms();
 }
 void InitUniforms() {
  Uniform(&UlightPosition,"LightPosition");
  Uniform(&Uscale,"Scale");
  Uniform(&Ugrain,"GrainSizeRecip");
  Uniform(&Ucolor,"DarkColor");
  Uniform(&Uspread,"Spread");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(UlightPosition,lightPosition.x,lightPosition.y,lightPosition.z);
  glUniform1f(Uscale,scale);
  glUniform1f(Ugrain,grain);
  glUniform3f(Ucolor,color.rf,color.gf,color.bf);
  glUniform3f(Uspread,spread.x,spread.y,spread.z);
 }
};


#if defined(NEVER)
/// Authors: John Kessenich, Randi Rost
class Wood2Shader {
public:
 void Init() {
  string vert=
varying float LightIntensity;
varying vec3  MCposition;
uniform vec3  LightPos;
uniform float Scale;
void main() {
 vec3 ECposition = vec3 (gl_ModelViewMatrix * gl_Vertex);
 MCposition      = vec3 (gl_Vertex) * Scale;
 vec3 tnorm      = normalize(vec3 (gl_NormalMatrix * gl_Normal));
 LightIntensity  = dot(normalize(LightPos - ECposition), tnorm);
 LightIntensity *= 1.5;
 gl_Position     = ftransform();
}
  ;
  string frag=
varying float LightIntensity; 
varying vec3  MCposition;
uniform sampler3D Noise;
uniform vec3  LightWoodColor;
uniform vec3  DarkWoodColor;
uniform float RingFreq;
uniform float LightGrains;
uniform float DarkGrains;
uniform float GrainThreshold;
uniform vec3  NoiseScale;
uniform float Noisiness;
uniform float GrainScale;

void main(void)
{
    vec3 noisevec = vec3 (texture3D(Noise, MCposition * NoiseScale) *
                                                 Noisiness);
    vec3 location = MCposition + noisevec;

    float dist = sqrt(location.x * location.x + location.z * location.z);
    dist *= RingFreq;

    float r = fract(dist + noisevec[0] + noisevec[1] + noisevec[2]) * 2.0;

    if (r > 1.0)
        r = 2.0 - r;
 
    vec3 color = mix(LightWoodColor, DarkWoodColor, r);

    r = fract((MCposition.x + MCposition.z) * GrainScale + 0.5);
    noisevec[2] *= r;
    if (r < GrainThreshold)
        color += LightWoodColor * LightGrains * noisevec[2];
    else
        color -= LightWoodColor * 0.0 * noisevec[2];

//        color -= LightWoodColor * DarkGrains * noisevec[2];

    color *= LightIntensity;
    
    color = clamp(color, 0.0, 1.0); // clamp not needed for book shader

    gl_FragColor = vec4 (color, 1.0);
}
};
#endif

/// Author: Jon Kennedy
class BubblesShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform vec3  LocalBlobbyPos;\n")+
   string("uniform float RadiusOfInfluence;\n")+
   string("uniform int   BailOut;\n")+
   string("uniform vec3  BlobbyPos[5];\n")+
   string("uniform int   BlobbyCount;\n")+
   string("uniform vec3  LightPos;\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying vec4  EyePos;\n")+
   string("varying float LightIntensity;\n")+
   string("varying float Radius;\n")+
   string("float Threshold = 0.01 * RadiusOfInfluence;\n")+
   string("vec2 SphereMap(in vec3 ecPosition3, in vec3 normal) {\n")+
   string("	float m;\n")+
   string("	vec3 r, u, n;\n")+
   string("	u = normalize(ecPosition3);\n")+
   string("	r = reflect(u, normal);\n")+
   string("	m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z  + 1.0));\n")+
   string("	return vec2(r.x/m + 0.5, r.y / m + 0.5);\n")+
   string("}\n")+
   string("void main() {\n")+
   string("	int ii, count = 0;\n")+
   string("	float field = 0.0, fieldInc;\n")+
   string("	float radius = 0.0;\n")+
   string("	vec3 vert;\n")+
   string("	vec3 incVec = vec3(gl_Vertex) * 2.0;\n")+
   string("	vec4 eyeCoord;\n")+
   string("	vec3 eyeNorm3, eyeCoord3, norm;\n")+
   string("	bool tooBig = true;\n")+
   string("	bool found = false;\n")+
   string("	vert = vec3(gl_Vertex) + BlobbyPos[0];\n")+
   string("	while(found == false)	{\n")+
   string("  count++;\n")+
   string("  field = 0.0;\n")+
   string("  eyeNorm3 = vec3(0.0, 0.0, 0.0);\n")+
   string("		for(ii = 0; ii < BlobbyCount; ii++) {\n")+
   string("			norm = vert - BlobbyPos[ii];\n")+
   string("			radius = dot(norm, norm);\n")+
   string("			if((radius > 0.01)) {\n")+
   string("				fieldInc = 1.0 / radius;\n")+
   string("				field += fieldInc;\n")+
   string("				eyeNorm3 += (fieldInc * fieldInc) * norm;\n")+
   string("			}\n")+
   string("		}\n")+
   string("		if(field > (RadiusOfInfluence - Threshold))	{\n")+
   string("			if(field < (RadiusOfInfluence + Threshold))	{\n")+
   string("				found = true;\n")+
   string("			}\n")+
   string("		}\n")+
   string("		if(found == false)		{\n")+
   string("			if(field < RadiusOfInfluence)	{\n")+
   string("				if(tooBig == false)	incVec /= 2.0;\n")+
   string("				tooBig = true;\n")+
   string("				vert -= incVec;\n")+
   string("			} else	{\n")+
   string("				if(tooBig == true)	incVec /= 2.0;\n")+
   string("				tooBig = false;\n")+
   string("				vert += incVec;\n")+
   string("			}\n")+
   string("		}\n")+
   string("		if(count == BailOut && found != true)	{\n")+
   string("			found = true;\n")+
   string("		}\n")+
   string("	}\n")+
   string("	Normal = normalize(gl_NormalMatrix * eyeNorm3);\n")+
   string(" vec4 pos       = gl_ModelViewMatrix * vec4(vert, 1.0);\n")+
   string(" EyeDir         = pos.xyz;\n")+
   string(" Radius = 1.5 - Normal.z; //length(vert - LocalBlobbyPos) * 0.2;\n")+
   string(" gl_Position = EyePos = gl_ModelViewProjectionMatrix * vec4(vert, 1.0);\n")+
   string(" LightIntensity = max(dot(normalize(LightPos - EyeDir), Normal), 0.0);\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("const vec3 Xunitvec = vec3 (1.0, 0.0, 0.0);\n")+
   string("const vec3 Yunitvec = vec3 (0.0, 1.0, 0.0);\n")+
   string("const vec3 Luminance = vec3(0.2125, 0.7154, 0.0721);\n")+
   string("uniform vec3      BaseColor;\n")+
   string("uniform float     MixRatio;\n")+
   string("uniform sampler2D EnvMap;\n")+
   string("uniform sampler2D RefractionMap;\n")+
   string("uniform sampler2D RainbowMap;\n")+
   string("uniform float FrameWidth;\n")+
   string("uniform float FrameHeight;\n")+
   string("varying vec3  Normal;\n")+
   string("varying vec3  EyeDir;\n")+
   string("varying vec4  EyePos;\n")+
   string("varying float LightIntensity;\n")+
   string("varying float Radius;\n")+
   string("void main () {\n")+
   string(" vec3 reflectDir = reflect(EyeDir, Normal);\n")+
   string(" vec2 index;\n")+
   string(" index.y = dot(normalize(reflectDir), Yunitvec);\n")+
   string(" reflectDir.y = 0.0;\n")+
   string(" index.x = dot(normalize(reflectDir), Xunitvec) * 0.5;\n")+
   string(" if (reflectDir.z >= 0.0) index = (index + 1.0) * 0.5;\n")+
   string(" else {\n")+
   string("  index.t = (index.t + 1.0) * 0.5;\n")+
   string("  index.s = (-index.s) * 0.5 + 1.0;\n")+
   string(" }\n")+
   string(" vec3 envColor = vec3 (texture2D(EnvMap, index));\n")+
   string(" vec3 rainbowColor = vec3 (texture2D(RainbowMap, sin(EyeDir.xy) + index)) + 0.6;\n")+
   string(" float fresnel = abs(dot(normalize(EyeDir), Normal));\n")+
   string(" fresnel *= MixRatio;\n")+
   string(" fresnel = clamp(fresnel, 0.5, 0.9);\n")+
   string("	float recipW = 1.0 / EyePos.w;\n")+
   string("	vec2 eye = EyePos.xy * vec2(recipW);\n")+
   string("	index.s = eye.x / 2.0 + 0.5;\n")+
   string("	index.t = eye.y / 2.0 + 0.5;\n")+
   string("	float recip1k = 1.0 / 2048.0;\n")+
   string("	index.s = clamp(index.s, 0.0, 1.0 - recip1k);\n")+
   string("	index.t = clamp(index.t, 0.0, 1.0 - recip1k);\n")+
   string("	index.s = index.s * FrameWidth * recip1k;\n")+
   string("	index.t = index.t * FrameHeight * recip1k;\n")+
   string(" vec3 RefractionColor = vec3 (texture2D(RefractionMap, index));\n")+
   string(" float envIntensity = dot(envColor, Luminance) * 2.0;\n")+
   string(" rainbowColor *= envIntensity;\n")+
   string(" envColor = mix(rainbowColor, RefractionColor, fresnel);\n")+
   string(" vec3 base = LightIntensity * BaseColor;\n")+
   string(" envColor = mix(envColor, base, 0.2);\n")+
   string(" gl_FragColor = vec4 (envColor, 1.0);\n")+
   string("}")
  ;
 }
};


// Author: Jon Kennedy
class BlobbyCloudShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float RadiusOfInfluence;\n")+
   string("uniform int   BailOut;\n")+
   string("uniform vec3  BlobbyPos[5];\n")+
   string("uniform int   BlobbyCount;\n")+
   string("float Threshold = 0.01 * RadiusOfInfluence;\n")+
   string("vec2 SphereMap(in vec3 ecPosition3, in vec3 normal) {\n")+
   string("	float m;\n")+
   string("	vec3 r, u, n;\n")+
   string("	u = normalize(ecPosition3);\n")+
   string("	r = reflect(u, normal);\n")+
   string("	m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z  + 1.0));\n")+
   string("	return vec2(r.x/m + 0.5, r.y / m + 0.5);\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string("	int ii, count = 0;\n")+
   string("	float field = 0.0, fieldInc;\n")+
   string("	float radius = 0.0;\n")+
   string("	vec3 vert;\n")+
   string("	vec3 incVec = vec3(gl_Vertex) * 2.0;\n")+
   string("	vec4 eyeCoord;\n")+
   string("	vec3 eyeNorm3, eyeCoord3, norm;\n")+
   string("	bool tooBig = true;\n")+
   string("	bool found = false;\n")+
   string("	vert = vec3(gl_Vertex) + BlobbyPos[0];\n")+
   string("	while(found == false) {\n")+
   string("  count++;\n")+
   string("  field = 0.0;\n")+
   string("  eyeNorm3 = vec3(0.0, 0.0, 0.0);\n")+
   string("		for(ii = 0; ii < BlobbyCount; ii++) {\n")+
   string("			norm = vert - BlobbyPos[ii];\n")+
   string("			radius = dot(norm, norm);\n")+
   string("			if((radius > 0.01)) {\n")+
   string("				fieldInc = 1.0 / radius;\n")+
   string("				field += fieldInc;\n")+
   string("				eyeNorm3 += (fieldInc * fieldInc) * norm;\n")+
   string("			}\n")+
   string("		}\n")+
   string("		if(field > (RadiusOfInfluence - Threshold))	{\n")+
   string("			if(field < (RadiusOfInfluence + Threshold)) found = true;\n")+
   string("		}\n")+
   string("		if(found == false)	{\n")+
   string("			if(field < RadiusOfInfluence)	{\n")+
   string("				if(tooBig == false)	incVec /= 2.0;\n")+
   string("				tooBig = true;\n")+
   string("				vert -= incVec;\n")+
   string("		}	else	{\n")+
   string(" 		if(tooBig == true)	incVec /= 2.0;\n")+
   string("			tooBig = false;\n")+
   string("			vert += incVec;\n")+
   string("		}\n")+
   string("	}\n")+
   string("	if(count == BailOut && found != true)	{\n")+
   string("		found = true;\n")+
   string("	}\n")+
   string("	eyeNorm3 = normalize(gl_NormalMatrix * eyeNorm3);\n")+
   string("	eyeCoord = gl_ModelViewMatrix * vec4(vert, 1.0);\n")+
   string("	eyeCoord3 = eyeCoord.xyz / eyeCoord.w;\n")+
   string("	gl_TexCoord[0] = vec4(SphereMap(eyeCoord3, eyeNorm3), 0.0, 1.0);\n")+
   string("	gl_Position  = gl_ProjectionMatrix * eyeCoord;\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D EnvMap;\n")+
   string("void main (void) {\n")+
   string(" vec3 envColor = vec3(texture2D(EnvMap, gl_TexCoord[0].st));\n")+
   string(" gl_FragColor = vec4(envColor, 1.0);\n")+
   string("}")
  ;
 }
};

/// Philip Rideout
class SphereMorphShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("varying vec4 Color;\n")+
   string("uniform vec3 LightPosition;\n")+
   string("uniform vec3 SurfaceColor;\n")+
   string("const float twopi = 6.28318;\n")+
   string("const float pi = 3.14159;\n")+
   string("uniform float radius;\n")+
   string("uniform float blend;\n")+
   string("vec3 sphere(vec2 domain) {\n")+
   string(" vec3 range;\n")+
   string(" range.x = radius * cos(domain.y) * sin(domain.x);\n")+
   string(" range.y = radius * sin(domain.y) * sin(domain.x);\n")+
   string(" range.z = radius * cos(domain.x);\n")+
   string(" return range;\n")+
   string("}\n")+
   string("void main() {\n")+
   string(" vec2 p0 = gl_Vertex.xy * twopi;\n")+
   string(" vec3 normal = sphere(p0);;\n")+
   string(" vec3 r0 = radius * normal;\n")+
   string(" vec3 vertex = r0;\n")+
   string(" normal = normal * blend + gl_Normal * (1.0 - blend);\n")+
   string(" vertex = vertex * blend + gl_Vertex.xyz * (1.0 - blend);\n")+
   string(" normal = normalize(gl_NormalMatrix * normal);\n")+
   string(" vec3 position = vec3(gl_ModelViewMatrix * vec4(vertex,1.0));\n")+
   string(" vec3 lightVec   = normalize(LightPosition - position);\n")+
   string(" float diffuse   = max(dot(lightVec, normal), 0.0);\n")+
   string(" if (diffuse < 0.125) diffuse = 0.125;\n")+
   string(" Color = vec4(SurfaceColor * diffuse, 1.0);\n")+
   string(" gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex,1.0);\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("varying vec4 Color;\n")+
   string("void main() {\n")+
   string("	gl_FragColor = Color;\n")+
   string("}")
  ;
 }
};

/// Author: Philip Rideout
class ParticleWave : public GLSLShader {
public:
 float time;
 GLuint Utime;
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float time;\n")+
   string("varying vec4 Color;\n")+
   string("const float radius = 0.3;\n")+
   string("void main() {\n")+
   string("	vec4 vertex = gl_Vertex;\n")+
   string("	float t1 = mod(time, 5.0);\n")+
   string("	vertex.x = radius * gl_Color.y * t1 * sin(gl_Color.x * 6.28);\n")+
   string("	vertex.z = radius * gl_Color.y * t1 * cos(gl_Color.x * 6.28);\n")+
   string("	float h = gl_Color.y * 1.25;\n")+
   string("	float t2 = mod(t1, h*2.0);\n")+
   string("	vertex.y = -(t2-h)*(t2-h)+h*h;\n")+
   string("	vertex.y -= 1.0;\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * vertex;\n")+
   string("	Color.r = 1.0;\n")+
   string("	Color.g = 1.0 - gl_Color.y;\n")+
   string("	Color.b = 0.0;\n")+
   string("	Color.a = 1.0 - t1 / 5.0;\n")+
   string("}")
  ;
  string frag=
   string("varying vec4 Color;\n")+
   string("void main () {\n")+
   string("	gl_FragColor = Color;\n")+
   string("}")
  ;
 }
};

/// Author: Philip Rideout
class ParticleSimpleShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float Time;\n")+
   string("uniform float RepeatFactor;\n")+
   string("uniform float Radius;\n")+
   string("uniform float Acceleration;\n")+
   string("varying vec4 Color;\n")+
   string("void main() {\n")+
   string("	vec4 vertex = vec4(0,0,0,1);\n")+
   string("	float t = max(Time - gl_Color.a, 0.0);\n")+
   string("	// modulo(a, b) = a - b * floor(a * (1 / b)).\n")+
   string("	t = t - RepeatFactor * floor(t * (1.0 / RepeatFactor));\n")+
   string("	vec3 velocity = Radius * (gl_Color.xyz - vec3(0.5));\n")+
   string("	vertex += vec4(velocity * t, 0.0);\n")+
   string("	vertex.y -= Acceleration * t * t;\n")+
   string("	Color = vec4(gl_Color.rgb, 1.0 - t);\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * vertex;\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("varying vec4 Color;\n")+
   string("void main (void) {	gl_FragColor = Color; }")
  ;
 }
};

/// Philip Rideout
class ParticleFountainShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float time;\n")+
   string("varying vec4 Color;\n")+
   string("const float maxy = 1.85;\n")+
   string("const float rad = 1.75;\n")+
   string("void main() {\n")+
   string("	float t = time;\n")+
   string("	t = clamp(t - gl_Color.a, 0.0, 10000.0);\n")+
   string("	t = mod(t, 1.0);\n")+
   string("	vec4 vertex = gl_Vertex;\n")+
   string("	vertex.x = rad * gl_Color.y * t * sin(gl_Color.x * 6.28);\n")+
   string("	vertex.z = rad * gl_Color.y * t * cos(gl_Color.x * 6.28);\n")+
   string("	float miny = ((gl_Color.y * t) >  1.0) ? -5000.0 : -1.0;\n")+
   string("	float h = gl_Color.z * maxy;\n")+
   string("	vertex.y = - (t - h) * (t - h) + h * h - 1.0;\n")+
   string("	vertex.y = clamp(vertex.y, miny, 100.0);\n")+
   string("	gl_Position = gl_ModelViewProjectionMatrix * vertex;\n")+
   string("	Color.r = 1.0;\n")+
   string("	Color.g = 1.0 - h / maxy;\n")+
   string("	Color.b = 0.0;\n")+
   string("	Color.a = 1.0 - t / 1.75;\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("varying vec4 Color;\n")+
   string("void main (void) {	gl_FragColor = Color; }")
  ;
 }
};

/// Author: Joshua Doss
// This is a fragment shader to show texture bombing.  It uses
//   a random texture to place glyphs of random color
//   in random locations on the model.  For more information
//   on Texture Bombing techniques see the following books:
//      1.  Texturing and Modeling - A Procedural Approach Copyright 1998 by Ebert et al.
//      2.  GPU Gems Copyright 2004 Edited by Randima Fernando
// Since we have random offset and random rotation, we must sample cells outside
//   of the cell we are currently in.  Since the offset is always in the same direction,
//   we know which four cells we need to sample.  If we add in rotation, the potential
//   exists for the glyph to exit the cell into any of the surrounding cells, thus we
//   need to iterate through all 9 cells.
//
// Below is a pictorial of the neighboring cells to be checked with rotation and
//   without rotation.  The "X" in a cell denotes the cell where the 
//   current fragment is located.
//
//     No Rotation                       With Rotation
//   ---------------          --------------------------------
//   |      |      |          |          |          |        |
//   |      |   X  |          |          |          |        |
//   |      |      |          |          |          |        |
//   ---------------          --------------------------------
//   |      |      |          |          |          |        |
//   |      |      |          |          |    X     |        |
//   |      |      |          |          |          |        |
//   ---------------          --------------------------------
//                            |          |          |        |
//                            |          |          |        |
//                            |          |          |        |
//                            --------------------------------
// If random rotation is turned on (RandomRotate == True == 1), 9 cells must be sampled per pass, 
// if random rotation is off (RandomRotate == False == 0), only 4 cells need to be sampled.
// We can use an integer constructor here with the boolean uniform RandomRotate in order to set the number
// of cells to sample.
//   We need a set of random values, to avoid sampling the same point
//      on our texture we multiply the current cell by a vec2 with
//      x, y components less than 1.  To allow for more user interaction
//      we will allow the user to adjust these through the use of uniform
//      variables (RO1, RO2) that are clamped from 0.0 to 1.0.  We will 
//      use the randomUV value to index into our random texture.
//   To give the option of sampling each cell multiple times, we need to loop
//      again for the number of times we wish to draw a glyph per cell.
//   A more random appearance is generated if some of the cells do not sample
//      the texture for a glyph.  Adjusting the Percentage uniform allows the
//      user to increase or decrease the likelihood of a cell getting sampled.
//   We need to index into our glyph texture.  The floor function will divide
//      our texture into 10 sections in each direction, thus giving us 100 glyphs
//      to access.  The variable colAdjust is a uniform that allows the user to select
//      which row to access glyphs from. 
//   If random rotation is turned on, multiply a random component of our random 
//      texture by roughly 2 PI (6.3) so the potential exists for a full rotation.
//      We use a standard 2D rotation matrix.
//  Here we do a random scaling of the glyph, if the user has selected this option.
//  Get the final coordinates in order to display proper glyph.  It is important that we clamp
//      the glyphIndex value, otherwise we will have artifacts.
//  This determines whether or not to draw the image from the texture, or use the background color
//      which is initialized to ModelColor uniform.  Since the texture is black and white, this
//      looks for white areas and does not draw them.  The mix function for the color gives
//      us nice antialiasing for our glyphs.
class GlyphBombShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float  SpecularContribution;\n")+
   string("uniform vec3   LightPosition;\n")+
   string("varying float  LightIntensity;\n")+
   string("varying vec2   uv;\n")+
   string("void main() {\n")+
   string(" float diffusecontribution  = 1.0 - SpecularContribution;\n")+
   string(" vec3  ecPosition           = vec3(gl_ModelViewMatrix * gl_Vertex);\n")+
   string(" vec3  tnorm                = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec3  lightVec             = normalize(LightPosition - ecPosition);\n")+
   string(" vec3  reflectVec           = reflect(-lightVec, tnorm);\n")+
   string(" vec3  viewVec              = normalize(-ecPosition);\n")+
   string(" float diffuse              = max(dot(lightVec, tnorm), 0.0);\n")+
   string(" float spec                 = 0.0;\n")+
   string(" if(diffuse > 0.0) {\n")+
   string("  spec = max(dot(reflectVec, viewVec), 0.0);\n")+
   string("  spec = pow(spec, 16.0);\n")+
   string(" }\n")+
   string(" LightIntensity  = diffusecontribution * diffuse * 1.5 + SpecularContribution * spec;\n")+
   string(" uv = gl_MultiTexCoord0.st;\n")+
   string(" gl_Position     = ftransform();\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform vec3      ModelColor;\n")+
   string("uniform sampler2D glyphTex;\n")+
   string("uniform sampler2D RandomTex;\n")+
   string("uniform float     ColAdjust;\n")+
   string("uniform float     ScaleFactor;\n")+
   string("uniform float     Percentage;\n")+
   string("uniform float     SamplesPerSquare;\n")+
   string("uniform float     RO1;\n")+
   string("uniform float     RO2;\n")+
   string("uniform bool      RandomScale;\n")+
   string("uniform bool      RandomRotate;\n")+
   string("varying vec2      uv;\n")+
   string("varying float     LightIntensity;\n")+
   string("void main() {\n")+
   string(" vec4 color    = vec4(ModelColor, 1.0);\n")+
   string(" vec2 scaledUV = (uv * ScaleFactor);\n")+
   string(" vec2 cell     = floor(scaledUV);\n")+
   string(" vec2 offSet   = scaledUV - cell;\n")+
   string(" for(int i = -1; i<=int (RandomRotate); i++) {\n")+
   string("  for(int j = -1; j<=int (RandomRotate); j++) {\n")+
   string("   vec2 currentCell    = cell + vec2 ( float ( i ) , float ( j ) );\n")+
   string("   vec2 currentOffSet  = offSet - vec2 ( float ( i ) , float ( j ) );\n")+
   string("   vec2 randomUV       = currentCell.xy * vec2(RO1, RO2);\n")+
   string("   for(int k = 0; k < int (SamplesPerSquare); k++) {\n")+
   string("    randomUV           += vec2(0.79, 0.388);\n")+
   string("    vec4 random         = texture2D(RandomTex, randomUV);\n")+
   string("    if(random.x < Percentage) {\n")+
   string("     vec2 glyphIndex;\n")+
   string("     mat2 rotator;\n")+
   string("     float indexs        = floor(random.b * 10.0);\n")+
   string("     float indext        = floor(ColAdjust * 10.0);\n")+
   string("     if(RandomRotate) {\n")+
   string("      float rotationAngle = 6.3 * random.g;\n")+
   string("      rotator[0]          = vec2(cos(rotationAngle), sin(rotationAngle));\n")+
   string("      rotator[1]          = vec2(-sin(rotationAngle), cos(rotationAngle));\n")+
   string("      glyphIndex          = -rotator * (currentOffSet.st - random.rg);\n")+
   string("     } else glyphIndex       = currentOffSet.st - random.rg;\n")+
   string("     if(RandomScale) glyphIndex /= vec2(1.3 * random.r);\n")+
   string("     glyphIndex.s               = (clamp(glyphIndex.s, 0.0, 1.0) + indexs) / 10.0;\n")+
   string("     glyphIndex.t               = (clamp(glyphIndex.t, 0.0, 1.0) + indext) / 10.0;\n")+
   string("     vec4 image                 = texture2D(glyphTex, glyphIndex);\n")+
   string("     if(image.x < 0.9 || image.y < 0.9 || image.z < 0.9)\n")+
   string("      color.rgb = mix(random.rgb, ModelColor, image.rgb);\n")+
   string("    }\n")+
   string("   }\n")+
   string("  }\n")+
   string(" }\n")+
   string(" gl_FragColor   = color * LightIntensity;\n")+
   string("}")
  ;
 }
};

// Vertex shader for texture bombing
// Uses a random texture to place 3D polka dots of random color
//	in random locations on the model.  For more information
//	on Texture Bombing techniques see the following books:
//		1.  Texturing and Modeling - A Procedural Approach	Copyright 1998 by Ebert et al.
//		2.  GPU Gems Copyright 2005 Edited by Randima Fernando
/// Author: Joshua Doss
class TextureBombShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("uniform float SpecularContribution;\n")+
   string("uniform vec3 LightPosition;\n")+
   string("varying vec3 MCPosition;\n")+
   string("varying float LightIntensity;\n")+
   string("void main() {\n")+
   string(" float diffusecontribution  = 1.0 - SpecularContribution;\n")+
   string(" vec3  ecPosition           = vec3(gl_ModelViewMatrix * gl_Vertex);\n")+
   string(" vec3  tnorm                = normalize(gl_NormalMatrix * gl_Normal);\n")+
   string(" vec3  lightVec             = normalize(LightPosition - ecPosition);\n")+
   string(" vec3  reflectVec           = reflect(-lightVec, tnorm);\n")+
   string(" vec3  viewVec              = normalize(-ecPosition);\n")+
   string(" float diffuse              = max(dot(lightVec, tnorm), 0.0);\n")+
   string(" float spec                 = 0.0;\n")+
   string(" if(diffuse > 0.0) {\n")+
   string("  spec = max(dot(reflectVec, viewVec), 0.0);\n")+
   string("  spec = pow(spec, 16.0);\n")+
   string(" }\n")+
   string(" LightIntensity  = diffusecontribution * diffuse * 1.5 + SpecularContribution * spec;\n")+
   string(" MCPosition      = vec3(gl_Vertex);\n")+
   string(" gl_Position     = ftransform();\n")+
   string("}")
  ;
  string frag=
   string("#version 120\n")+
   string("varying float LightIntensity;\n")+
   string("varying vec3 MCPosition;\n")+
   string("uniform float Scale, DotSize;\n")+
   string("uniform sampler2D RandomTex;\n")+
   string("uniform vec3 ModelColor, PolkaDotColor;\n")+
   string("uniform vec3 DotOffset;\n")+
   string("uniform vec2 ColourTextureAdditive,  PositionTextureMultiplier;\n")+
   string("uniform bool RandomDots, RandomDotColours;\n")+
   string("void main() {\n")+
   string(" float radius2;\n")+
   string(" vec2  randomXY;\n")+
   string(" vec3  finalcolor = ModelColor.xyz;\n")+
   string(" vec3  dotSpacing = vec3(Scale);\n")+
   string(" vec3  scaledXYZ = MCPosition * dotSpacing;\n")+
   string(" vec3  cell = floor(scaledXYZ);\n")+
   string(" vec3  offset = scaledXYZ - cell;\n")+
   string(" vec3  currentOffset;\n")+
   string(" vec4  random;\n")+
   string(" float priority = -1.0;\n")+
   string(" for(float i = -1.0; i <= 0.0; i++) {\n")+
   string("  for(float j = -1.0; j <= 0.0; j++) {\n")+
   string("   for(float k = -1.0; k <= 0.0; k++) {\n")+
   string("    vec3 currentCell = cell + vec3(i, j, k);\n")+
   string("    vec3 cellOffset = offset - vec3(i, j, k);\n")+
   string("    randomXY = currentCell.xy * PositionTextureMultiplier + currentCell.z * 0.003;\n")+
   string("    random = texture2D(RandomTex, randomXY);\n")+
   string("    if(RandomDots) {\n")+
   string("     currentOffset = cellOffset - (vec3(0.5, 0.5, 0.5) + vec3(random));\n")+
   string("    } else {\n")+
   string("     currentOffset = cellOffset - (vec3(0.5, 0.5, 0.5) + DotOffset);\n")+
   string("    }\n")+
   string("    radius2 = dot(currentOffset, currentOffset);\n")+
   string("    if(random.w > priority && radius2 < DotSize) {\n")+
   string("     if (RandomDotColours) {\n")+
   string("      finalcolor = texture2D(RandomTex, randomXY + ColourTextureAdditive).xyz;\n")+
   string("     } else {\n")+
   string("      finalcolor = PolkaDotColor.xyz;\n")+
   string("     }\n")+
   string("     priority = random.w;\n")+
   string("    }\n")+
   string("   }\n")+
   string("  }\n")+
   string(" }\n")+
   string(" gl_FragColor      = (vec4( finalcolor, 1.0 ) * LightIntensity);\n")+
   string("}")
  ;
 }
};


#if defined (NOTNOW)
class StarBomb {
 [vert]
 /******************************************************************************************* 
*  StarBombGL2.vert                                                                        *
*                                                                                          *
*   Copyright (C) 2002-2006  3Dlabs Inc. Ltd.                                              *
*                                                                                          *
*   Based on wallpaper.sl by Darwyn Peachy.                                                *
*   Converted to OpenGL Shading Language by Joshua Doss                                    *
*                                                                                          *
*   From the book Texturing and Modeling: A Procedural Approach_, by David S. Ebert, ed.,  *
*   F. Kenton Musgrave, Darwyn Peachey, Ken Perlin, and Steven Worley.                     *
*   Academic Press, 1994.  ISBN 0-12-228760-6                                              *
*******************************************************************************************/

uniform vec3 LightPosition;
uniform float SpecularContribution;

varying vec2 TexCoord;
varying float LightIntensity;

void main(void)
{
    TexCoord        = -gl_MultiTexCoord0.st;
    
    float diffuseContribution  = 1.0 - SpecularContribution;
    
    // compute the vertex position in eye coordinates
    vec3  ecPosition           = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // compute the transformed normal
    vec3  tnorm                = normalize(gl_NormalMatrix * gl_Normal);
    
    // compute a vector from the model to the light position
    vec3  lightVec             = normalize(LightPosition - ecPosition);
    
    // compute the reflection vector
    vec3  reflectVec           = reflect(-lightVec, tnorm);
    
    // compute a unit vector in direction of viewing position
    vec3  viewVec              = normalize(-ecPosition);
    
    // calculate amount of diffuse light based on normal and light angle
    float diffuse              = max(dot(lightVec, tnorm), 0.0);
    float spec                 = 0.0;
    
    // if there is diffuse lighting, calculate specular
    if(diffuse > 0.0)
       {
          spec = max(dot(reflectVec, viewVec), 0.0);
          spec = pow(spec, 16.0);
       }
    
    // add up the light sources, since this is a varying (global) it will pass to frag shader     
    LightIntensity  = diffuseContribution * diffuse * 1.5 +
                          SpecularContribution * spec;
    
    // send vertex information
    gl_Position     = ftransform();
}


[frag]
/******************************************************************************************* 
*   StarBombGL2.frag                                                                        *
*                                                                                          *
*   Copyright (C) 2002-2006  3Dlabs Inc. Ltd.                                              *
*                                                                                          *
*   Based on wallpaper.sl by Darwyn Peachy.                                                *
*   Converted to OpenGL Shading Language by Joshua Doss                                    *
*                                                                                          *
*   From the book Texturing and Modeling: A Procedural Approach_, by David S. Ebert, ed.,  *
*   F. Kenton Musgrave, Darwyn Peachey, Ken Perlin, and Steven Worley.                     *
*   Academic Press, 1994.  ISBN 0-12-228760-6                                              *
*******************************************************************************************/

uniform float     NumberCells, NumberPoints, RadiusMin, RadiusMax;
uniform sampler2D RandomTexture;
uniform vec4      StarColor, ModelColor;
uniform float     OffsetScale;

varying vec2  TexCoord;
varying float LightIntensity;

const float PI = 3.14159265358979;

void main(void)
{
   float cellSize = 1.0 / NumberCells;
   float angle, r, a; 
   float in_out = 0.0;
   float starAngle = ((2.0 * PI)/NumberPoints);
   
   vec2 randomXY, cellCenter, cell, center, sstt;
   
   vec3 p0 = RadiusMax * vec3 (cos(0.0), sin(0.0), 0.0);
   vec3 p1 = RadiusMin * vec3 (cos(starAngle/2.0), sin(starAngle/2.0), 0.0);
   vec3 d0 = (p1 - p0);
   vec3 d1;
   
   vec4 useColor, outColor, randomCS, random, color;
   
   cellCenter = vec2(floor(TexCoord.st * NumberCells));
   
   color = ModelColor;
   
   
   for ( float i = -1.0; i <= 0.0; i += 1.0)
   {
      for ( float j = -1.0; j <= 0.0; j += 1.0)
      {
         cell = cellCenter + vec2 ( i, j );
         
         vec2 randomCellSelect = vec2(.0032 * cell.s, .87 * cell.t);
         randomCS = texture2D(RandomTexture, randomCellSelect);
         randomXY = vec2 (cell.s / (cell.s + 0.5), cell.t / (cell.t + 0.5));
         float priority = -1.0;
         random = texture2D(RandomTexture, randomXY);
         if(randomCS.r < 0.5)
         {
             random *= OffsetScale;
             center  = cellSize * (cell + 0.5 + 0.6 * random.rg);
             sstt    = TexCoord.st - center;
             
             angle = atan(sstt.s, sstt.t) + PI;
             r     = sqrt(sstt.s*sstt.s + sstt.t*sstt.t);
             a     = mod (angle, starAngle) / starAngle;
             if(a >= 0.5)
             {
                a = 1.0 - a;
             }
             d1            = r * vec3 (cos(a), sin(a), 0.0) - p0;
             float checkin = step(0.0, cross(d0, d1).z);
             if (checkin  >= 0.5 && random.w > priority)
             {  
               color    = texture2D(RandomTexture, cellSize * (cell + 0.5));
               priority = random.w;
             }          
             in_out += checkin;      
         }           
      }
   }  
   outColor = mix(ModelColor, color, step(0.5, in_out));
   gl_FragColor = vec4( outColor * LightIntensity);
}


};


class Wood3Shader {
 [frag]
 // Based on the wood shader in the Orange Book

varying vec3 normal;
varying vec4 pos;
varying vec4 rawpos;

uniform float scale;

float noise(vec4);
float snoise(vec4);
float noise(vec3);
float snoise(vec3);
float unsign(float x);


void main() {

	vec3 LightWood = vec3(0.6, 0.3, 0.1);
	vec3 DarkWood = vec3(0.4, 0.2, 0.07);
	//vec3 LightWood = vec3(0.46, 0.35, 0.19);
	//vec3 DarkWood = vec3(0.29, 0.27, 0.06);
	float RingFreq = 4.0;
	//float RingFreq = 0.30;
	float LightGrains = 1.0;
	float DarkGrains = 0.0;
	float GrainThreshold = 0.8;
	vec3 NoiseScale= vec3(0.5, 0.1, 0.1);
	float Noisiness = 3.0;
	float GrainScale = 17.0;

	vec4 color;
	vec4 matspec = gl_FrontMaterial.specular;
	float shininess = gl_FrontMaterial.shininess;
	vec4 lightspec = gl_LightSource[0].specular;
	vec4 lpos = gl_LightSource[0].position;
	vec4 s = -normalize(pos-lpos); 	//not sure why this needs to 
									// be negated, but it does.
	vec3 light = s.xyz;
	vec3 n = normalize(normal);
	vec3 r = -reflect(light, n);
	r = normalize(r);
	vec3 v = -pos.xyz; // We are in eye coordinates,
					   // so the viewing vector is
					   // [0.0 0.0 0.0] - pos
	v = normalize(v);

	float scalelocal;
	if (scale == 0.0) {
		scalelocal = 2.0; //default value
	} else {
		scalelocal = scale;
	}

	vec4 tp = gl_TexCoord[0] * scalelocal;
	//vec3 rp = rawpos.xyz * scalelocal* 0.8;
	vec3 rp = rawpos.xyz * scalelocal * NoiseScale;
	
	vec3 noisevec = vec3(noise(rp), noise(rp + 3.33), noise(rp + 7.77)) * Noisiness;
	vec3 location = rp + noisevec;
	
	float dist = sqrt(location.x * location.x + location.z * location.z);
	dist *= RingFreq;
	
	float rf = fract(dist + unsign(noisevec[0])/256.0 + unsign(noisevec[1])/32.0 + unsign(noisevec[2])/16.0) * 2.0;
	//float rf = fract(dist)*2.0;
	if (rf > 1.0) {
		rf = 2.0 - rf;
	}
	
	color = vec4(mix(LightWood, DarkWood, rf), 1.0);
	
	rf = fract((rp.x + rp.z) * GrainScale +0.5);
	noisevec[2] *= rf;
	
	if( rf < GrainThreshold) {
		color.xyz += LightWood * LightGrains * unsign(noisevec[2]);
	} else {
		color.xyz -= LightWood * DarkGrains * unsign(noisevec[2]);
	}
	
	//float intensity = min(1.0, noise(rp) * 5.0);
	//color = vec4(intensity, intensity, intensity, 1.0);
	
	vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
	vec4 specular;
	if (shininess != 0.0) {
		specular = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
	} else {
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	gl_FragColor = diffuse + specular;


}


};


class Granite2Shader {
 [vert]
 [frag]
// Very loosely based on the granite shader in the Orange Book.

varying vec3 normal;
varying vec4 pos;
varying vec4 rawpos;

uniform float scale;

float noise(vec4);
float snoise(vec4);
float noise(vec3);
float snoise(vec3);
float unsign(float x);

vec4 noise4d(vec4);

void main() {
	vec4 color;
	vec4 matspec = gl_FrontMaterial.specular;
	float shininess = gl_FrontMaterial.shininess;
	vec4 lightspec = gl_LightSource[0].specular;
	vec4 lpos = gl_LightSource[0].position;
	vec4 s = -normalize(pos-lpos); 	//not sure why this needs to 
									// be negated, but it does.
	vec3 light = s.xyz;
	vec3 n = normalize(normal);
	vec3 r = -reflect(light, n);
	r = normalize(r);
	vec3 v = -pos.xyz; // We are in eye coordinates,
					   // so the viewing vector is
					   // [0.0 0.0 0.0] - pos
	v = normalize(v);

	float scalelocal;
	if (scale == 0.0) {
		scalelocal = 1.0; //default value
	} else {
		scalelocal = scale;
	}

	vec4 tp = gl_TexCoord[0] * scalelocal;
	vec3 rp = rawpos.xyz * scalelocal * 20.0;
	
	//vec4 noisevec = vec4(noise(rp), noise(rp + 3.33), noise(rp + 7.77), noise(rp + 13.32));
	
//	float intensity = min(1.0, noise(rp) * 5.0);
	float intensity = min(1.0, (unsign(noise(rp))/16.0) * 18.0);
	color = vec4(intensity, intensity, intensity, 1.0);
	
	vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
	vec4 specular;
	if (shininess != 0.0) {
		specular = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
	} else {
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	gl_FragColor = diffuse + specular;


}


};

/// Sean
class SwellShader {
 // SwellVert.glsl
//

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_TextureMatrix[2] * gl_MultiTexCoord2;
	gl_TexCoord[3] = gl_TextureMatrix[3] * gl_MultiTexCoord3;
}


[frag]
 // SwellFrag.glsl
//

uniform sampler3D s3Tex0;

void main()
{
/*
	gl_FragColor.r = gl_Color.r * texture3D(s3Tex0, gl_TexCoord[0].xyz).a;
	gl_FragColor.g = gl_Color.g * texture3D(s3Tex0, gl_TexCoord[1].xyz).a;
	gl_FragColor.b = gl_Color.b * texture3D(s3Tex0, gl_TexCoord[2].xyz).a;
	gl_FragColor.a = 1.0;
*/
	gl_FragColor = gl_Color * (
		0.5 * texture3D(s3Tex0, gl_TexCoord[0].xyz) +
		0.25 * texture3D(s3Tex0, gl_TexCoord[1].xyz) +
		0.125 * texture3D(s3Tex0, gl_TexCoord[2].xyz) +
		0.0625 * texture3D(s3Tex0, gl_TexCoord[3].xyz));
}


};


class GraynoiseShader {
 [frag]
 #define NNOISE 4

#define PI 3.141592653

#define PALE_BLUE vec4(0.25, 0.25, 0.35, 1.0)
//#define PALE_BLUE vec4(0.90, 0.90, 1.0, 1.0)
#define MEDIUM_BLUE vec4(0.10, 0.10, 0.30, 1.0)
#define DARK_BLUE vec4(0.05, 0.05, 0.26, 1.0)
#define DARKER_BLUE vec4(0.03, 0.03, 0.20, 1.0)

#define SKYCOLOR vec4(0.15, 0.15, 0.6, 1.0)
#define CLOUDCOLOR vec4(1.0, 1.0, 1.0, 1.0)

varying vec3 normal;
varying vec4 pos;
varying vec4 rawpos;
uniform float time;

float noise(vec4);
float snoise(vec4);
float noise(vec3);
float snoise(vec3);
vec4 marble_color(float);
vec4 cloud_color(float);
vec4 mossyrock_color(float);
vec4 spline(float x, int y, vec4 z[]);

void main() {
	//vec4 color = gl_FrontMaterial.diffuse;
	//vec4 matspec = gl_FrontMaterial.specular;
	//float shininess = gl_FrontMaterial.shininess;
	//vec4 lightspec = gl_LightSource[0].specular;
	//vec4 lpos = gl_LightSource[0].position;
	//vec4 s = -normalize(pos-lpos); 	//not sure why this needs to 
									// be negated, but it does.
	//vec3 light = s.xyz;
	//vec3 n = normalize(normal);
	//vec3 r = -reflect(light, n);
	//r = normalize(r);
	//vec3 v = -pos.xyz; // We are in eye coordinates,
					   // so the viewing vector is
					   // [0.0 0.0 0.0] - pos
	//v = normalize(v);
	
	float scale=0.009;
	//vec4 tp = gl_TexCoord[0] * scale;
	//vec3 rp = rawpos.xyz * scale;
	vec3 rp = vec3(gl_FragCoord.xy, 0.0) * scale;
	
	// create the grayscale marbling here
	float marble=0.0;
	float f = 1.0;
//	for(int i=0; i < NNOISE; i++) {
//		marble += noise(rp*f)/f;
//		f *= 2.17;
//	}

	float marble1 = noise(rp*f)/f;
	f *= 2.17;
	float marble2 = marble1 + noise(rp*f)/f;
	f *= 2.17;
	float marble3 = marble2 + noise(rp*f)/f;
	f *= 2.17;
	float marble4 = marble3 + noise(rp*f)/f;
	f *= 2.17;
	float marble5 = marble4 + noise(rp*f)/f;
		
	float r1 = (marble1 / 3.542113654178) + 0.5;
	float r2 = ((marble2-marble1) / 3.542113654178) + 0.5;
	float r3 = ((marble3-marble2) / 3.542113654178) + 0.5;
	float r4 = ((marble4-marble3) / 3.542113654178) + 0.5;
	
	vec4 color;
	//color = marble_color(marble4);
	//color = cloud_color(marble4);
	color = mossyrock_color(marble4);
	
	// for some reason the colors are awfully dark
	// I think it looks better this way
	//color *= 2.85;
/*
	vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
	vec4 specular;
	if (shininess != 0.0) {
		specular = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
	} else {
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
*/	
	//gl_FragColor = vec4((marble4 / 3.542113654178) + 0.5);
	
	float gr = gl_FragCoord.x/790.0;
	vec4 col;
	if (gl_FragCoord.y < 572.0 / 2.0) {
		//col = marble_color((gr - 0.5) * 3.542113654178) * 2.85;
		//col = cloud_color((gr - 0.5) * 3.542113654178);
		col =mossyrock_color((gr - 0.5) * 3.542113654178);
	} else {
		col = vec4(gr);
	}
	gl_FragColor = col;
//	gl_FragColor = noise4(pos) != 0.0 ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(0.0, 0.0, 1.0, 1.0);

}

vec4 marble_color(float m) {
	vec4 c[25];
	
	c[0] = PALE_BLUE;
	c[1] = PALE_BLUE;
	c[2] = MEDIUM_BLUE;
	c[3] = MEDIUM_BLUE;
	c[4] = MEDIUM_BLUE;
	c[5] = PALE_BLUE;
	c[6] = PALE_BLUE;
	c[7] = DARK_BLUE;
	c[8] = DARK_BLUE;
	c[9] = DARKER_BLUE;
	c[10] = DARKER_BLUE;
	c[11] = PALE_BLUE;
	c[12] = DARKER_BLUE;
	
	vec4 res = spline(clamp(2.0*m + 0.75, 0.0, 1.0), 13, c);
	
	return res;
}

// roughly based on the clouds in tex&mod, page 449
vec4 cloud_color(float value) {
	float threshold = -0.15;
	vec4 res = mix(SKYCOLOR, CLOUDCOLOR, smoothstep(threshold, 1.0, value));
	return res;
}

vec4 mossyrock_color(float value) {
	vec4 c[25];
	
	c[0]=vec4(0.241, 0.255, 0.212, 1.0);
	c[1]=vec4(0.241, 0.255, 0.212, 1.0);
	c[2]=vec4(0.290, 0.300, 0.265, 1.0);
	c[3]=vec4(0.305, 0.321, 0.275, 1.0);
	c[4]=vec4(0.293, 0.314, 0.240, 1.0);
	c[5]=vec4(0.125, 0.325, 0.135, 1.0);
	c[6]=vec4(0.125, 0.325, 0.135, 1.0);
	c[6]=vec4(0.115, 0.335, 0.130, 1.0);

	vec4 res = spline((value/3.542113654178)+ 0.5, 8, c);
	return res;
}
};
#endif