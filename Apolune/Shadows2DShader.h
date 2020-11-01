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
#include "Crayon.h"
#include "Art.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

class Light2DShader: public GLSLShader {
public:
 float position[2];
 Crayon color;
 float radius;
 
  // Uniform locations
 GLint UlightPos, UlightColor, Uradius;

 void Init() {
  position[0]=512.0f;
  position[1]=512.0f;
  color.Pick(alabaster);
  OUTPUT("Light2DShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 lightPos;\n")+
   string("uniform vec4 lightColor;\n")+
   string("uniform float radius;\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = gl_TexCoord[0].st;\n")+
   string(" float D =  1.0-(length(lightPos - texCoord))*(3.0/radius);//*30.0;\n")+
   string(" gl_FragColor =lightColor*D;\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  UlightPos=glGetUniformLocation(program,"lightPos");
  UlightColor=glGetUniformLocation(program,"lightColor");
  Uradius=glGetUniformLocation(program,"radius");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform2f(UlightPos, position[0], position[1]);
  glUniform4f(UlightColor,color.floats[0],color.floats[1],color.floats[2],color.floats[3]);
  glUniform1f(Uradius,radius);
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern Light2DShader light2dShader;

// A bank of 3 lights

#define GPUMULTILIGHTLIMIT 3

struct GPUMultiLight2d {
public:
 float position[2];
 Crayon color;
 float radius; // a percentage of the screen
 GPUMultiLight2d() {
  position[0]=0.0f;
  position[1]=0.0f;
  radius=0.0f;
  color.Pick(black);
 }
};

class GPULights2dSnapshot : public ListItem {
public:
 GPUMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 GPULights2dSnapshot() : ListItem() {}
};

struct GPULights2dUniformSet {
 GLint UlightPos, UlightColor, Uradius;
};

/*
 * A set of 3 CPU-controlled GPU-rendered lights.
 * Other variations will include time-based parameters. (See below)
 */
class Lights2DShader: public GLSLShader {
public:
 GPUMultiLight2d *bulbs; // GPULights2d[16] as input;
 GPULights2dUniformSet uniforms[GPUMULTILIGHTLIMIT];
 GLint posAttrib,texAttrib;

 void SetFromSnapshot( GPUMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i]=lights[i];
  }
 }

 void GetFromSnapshot( GPUMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   lights[i]=bulbs[i];
  }
 }

 void Init() {
  OUTPUT("Lights2DShader::Loading\n");
  string frag=
   string("#version 120\n")+
   string("#ifdef GL_ES\n")+
   string("precision highp float;\n")+
   string("#endif\n")+
   string("uniform vec2 lightPos1;\n")+
   string("uniform vec3 lightColor1;\n")+
   string("uniform float R1;\n")+
   string("uniform vec2 lightPos2;\n")+
   string("uniform vec3 lightColor2;\n")+
   string("uniform float R2;\n")+
   string("uniform vec2 lightPos3;\n")+
   string("uniform vec3 lightColor3;\n")+
   string("uniform float R3;\n")+
   string("/* frag uniform limit exceeded beyond this */\n")+
   string("vec3 addColor( vec3 a, vec3 b ) { return a+b; }\n")+
   string("float atten2d( vec2 p, float invR ) {\n")+
   string(" float t=(1.0 - sqrt(p.x*p.x + p.y*p.y));\n")+
   string(" return 1.0 / (1.0 + exp(-(t*100.0 - (100.0-invR))));\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = gl_TexCoord[0].st;\n")+
   string(" float D;\n")+
   string(" D = atten2d(lightPos1-texCoord,R1); vec3 mixed = (lightColor1 *D);\n")+
   string(" D = atten2d(lightPos2-texCoord,R2); mixed=addColor(mixed,(lightColor2*D));\n")+
   string(" D = atten2d(lightPos3-texCoord,R3); mixed=addColor(mixed,(lightColor3*D));\n")+
   string(" gl_FragColor=vec4(mixed,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  GL_Assert("entering Lights2DShader::InitUniforms");

  uniforms[0].UlightPos=glGetUniformLocation(program,"lightPos1");
  uniforms[0].UlightColor=glGetUniformLocation(program,"lightColor1");
  uniforms[0].Uradius=glGetUniformLocation(program,"R1");

  uniforms[1].UlightPos=glGetUniformLocation(program,"lightPos2");
  uniforms[1].UlightColor=glGetUniformLocation(program,"lightColor2");
  uniforms[1].Uradius=glGetUniformLocation(program,"R2");

  uniforms[2].UlightPos=glGetUniformLocation(program,"lightPos3");
  uniforms[2].UlightColor=glGetUniformLocation(program,"lightColor3");
  uniforms[2].Uradius=glGetUniformLocation(program,"R3");

  GL_Assert("exitting Lights2DShader::InitUniforms");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  LOOP(i,GPUMULTILIGHTLIMIT,{
   glUniform1f(uniforms[i].Uradius, bulbs[i].radius);
   glUniform2f(uniforms[i].UlightPos,    bulbs[i].position[0], bulbs[i].position[1]);
   glUniform3f(uniforms[i].UlightColor,
    bulbs[i].color.floats[0],
    bulbs[i].color.floats[1],
    bulbs[i].color.floats[2]);
  });
 }

 void Disable() {
  glUseProgram(0);
 }
};

extern Lights2DShader lights2dShader;


// A bank of 3 lights, with a hard-coded radius

#define GPUMULTILIGHTNORADIUSLIMIT 3

struct GPUNoRadiusMultiLight2d {
public:
 float position[2];
 Crayon color;
 GPUNoRadiusMultiLight2d() {
  position[0]=0.0f;
  position[1]=0.0f;
  color.Pick(black);
 }
};

class GPUNoRadiusLights2dSnapshot : public ListItem {
public:
 GPUMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 GPUNoRadiusLights2dSnapshot() : ListItem() {}
};

struct GPUNoRadiusLights2dUniformSet {
 GLint UlightPos, UlightColor;
};

/*
 * A set of 3 CPU-controlled GPU-rendered lights.
 * Other variations will include time-based parameters. (See below)
 */
class LightsNoRadius2DShader: public GLSLShader {
public:
 GPUNoRadiusMultiLight2d *bulbs; // GPULights2d[16] as input;
 GPULights2dUniformSet uniforms[GPUMULTILIGHTLIMIT];
 GLuint Utrans;
 GLint posAttrib,texAttrib;

 void SetFromSnapshot( GPUNoRadiusMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i]=lights[i];
  }
 }

 void GetFromSnapshot( GPUNoRadiusMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   lights[i]=bulbs[i];
  }
 }

 void Init() {
  OUTPUT("LightsNoRadius2DShader::Loading\n");
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
   string("uniform vec2 lightPos1;\n")+
   string("uniform vec3 lightColor1;\n")+
   string("uniform vec2 lightPos2;\n")+
   string("uniform vec3 lightColor2;\n")+
   string("uniform vec2 lightPos3;\n")+
   string("uniform vec3 lightColor3;\n")+
   string("/* frag uniform limit exceeded beyond this */\n")+
   string("vec3 addColor( vec3 a, vec3 b ) { return a+b; }\n")+
   string("float atten2d( vec2 p ) {\n")+
   string("/* inversesq: */\n")+
   string(" float t=(1.0 - sqrt(p.x*p.x + p.y*p.y));\n")+
   string("/* sigmoid: */\n")+
   string(" return 1.0 / (1.0 + exp(-(t*100.0 - 95.0)));\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = Texcoord;\n")+
   string(" float D;\n")+
   string(" D = atten2d(lightPos1-texCoord); vec3 mixed = (lightColor1 *D);\n")+
   string(" D = atten2d(lightPos2-texCoord); mixed=addColor(mixed,(lightColor2*D));\n")+
   string(" D = atten2d(lightPos3-texCoord); mixed=addColor(mixed,(lightColor3*D));\n")+
   string(" outColor=vec4(mixed,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  GL_Assert("entering LightsNoRadius2DShader::InitUniforms");

  uniforms[0].UlightPos=glGetUniformLocation(program,"lightPos1");
  uniforms[0].UlightColor=glGetUniformLocation(program,"lightColor1");

  uniforms[1].UlightPos=glGetUniformLocation(program,"lightPos2");
  uniforms[1].UlightColor=glGetUniformLocation(program,"lightColor2");
  uniforms[1].Uradius=glGetUniformLocation(program,"R2");

  uniforms[2].UlightPos=glGetUniformLocation(program,"lightPos3");
  uniforms[2].UlightColor=glGetUniformLocation(program,"lightColor3");

  GL_Assert("exitting LightsNoRadius2DShader::InitUniforms");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  LOOP(i,GPUMULTILIGHTNORADIUSLIMIT,{
   glUniform2f(uniforms[i].UlightPos,    bulbs[i].position[0], bulbs[i].position[1]);
   glUniform3f(uniforms[i].UlightColor,
    bulbs[i].color.floats[0],
    bulbs[i].color.floats[1],
    bulbs[i].color.floats[2]);
  });

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
  glUseProgram(0);
 }
};

extern LightsNoRadius2DShader lightsnoradius2dShader;

// A bank of 3 lights, with a hard-coded radius

#define GPUMULTILIGHTNOCOLORLIMIT 3

struct GPUNoColorMultiLight2d {
public:
 float position[2];
 GPUNoColorMultiLight2d() {
  position[0]=0.0f;
  position[1]=0.0f;
 }
};

class GPUNoColorLights2dSnapshot : public ListItem {
public:
 GPUMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 GPUNoColorLights2dSnapshot() : ListItem() {}
};

struct GPUNoColorLights2dUniformSet {
 GLint UlightPos;
};

/*
 * A set of 3 CPU-controlled GPU-rendered lights.
 * Other variations will include time-based parameters. (See below)
 */
class LightsNoColor2DShader: public GLSLShader {
public:
 GPUNoColorMultiLight2d *bulbs; // GPULights2d[16] as input;
 GPULights2dUniformSet uniforms[GPUMULTILIGHTLIMIT];
 GLuint Utrans;
 GLint posAttrib,texAttrib;

 void SetFromSnapshot( GPUNoColorMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i]=lights[i];
  }
 }

 void GetFromSnapshot( GPUNoColorMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   lights[i]=bulbs[i];
  }
 }

 void Init() {
  OUTPUT("LightsNoColor2DShader::Loading\n");
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
   string("uniform vec2 lightPos1;\n")+
   string("uniform vec2 lightPos2;\n")+
   string("uniform vec2 lightPos3;\n")+
   string("/* frag uniform limit exceeded beyond this */\n")+
   string("vec3 addColor( vec3 a, vec3 b ) { return a+b; }\n")+
   string("float atten2d( vec2 p ) {\n")+
   string(" float t=(1.0 - sqrt(p.x*p.x + p.y*p.y));\n")+
   string(" return 1.0 / (1.0 + exp(-(t*100.0 - 99.0)));\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = Texcoord;\n")+
   string(" vec3 lightColor=vec3(1.0,1.0,1.0);\n")+
   string(" float D;\n")+
   string(" D = atten2d(lightPos1-texCoord); vec3 mixed = (lightColor *D);\n")+
   string(" if ( lightPos2.x != 0.0 && lightPos2.y != 0.0 ) { D = atten2d(lightPos2-texCoord); mixed=addColor(mixed,(lightColor*D)); }\n")+
   string(" if ( lightPos3.x != 0.0 && lightPos3.y != 0.0 ) { D = atten2d(lightPos3-texCoord); mixed=addColor(mixed,(lightColor*D)); }\n")+
   string(" outColor=vec4(mixed,1.0);\n")+
   string("}\n")
  ;
  loadShaders(frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  GL_Assert("entering LightsNoColor2DShader::InitUniforms");

  uniforms[0].UlightPos=glGetUniformLocation(program,"lightPos1");
  uniforms[1].UlightPos=glGetUniformLocation(program,"lightPos2");
  uniforms[2].UlightPos=glGetUniformLocation(program,"lightPos3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

  GL_Assert("exitting LightsNoColor2DShader::InitUniforms");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  LOOP(i,GPUMULTILIGHTNOCOLORLIMIT,{
   glUniform2f(uniforms[i].UlightPos,    bulbs[i].position[0], bulbs[i].position[1]);
  });

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
  glUseProgram(0);
 }
};

extern LightsNoColor2DShader lightsnocolor2dShader;

// A bank of 3 lights, with a hard-coded radius

#define GPUMULTILIGHTPULSINGLIMIT 3

struct GPUPulsingMultiLight2d {
public:
 float position[2];
 Crayon color;
 float radius, cycle;
 GPUPulsingMultiLight2d() {
  position[0]=0.0f;
  position[1]=0.0f;
  radius=cycle=1.0f;
 }
};

class GPUPulsingLights2dSnapshot : public ListItem {
public:
 GPUPulsingMultiLight2d bulbs[GPUMULTILIGHTPULSINGLIMIT];
 GPUPulsingLights2dSnapshot() : ListItem() {}
};

struct GPUPulsingLights2dUniformSet {
 GLint UlightPos, UlightColor, Uradius, Ucycle;
};

/*
 * A set of 3 CPU-controlled GPU-rendered lights.
 * Other variations will include time-based parameters. (See below)
 */
class LightsPulsing2DShader: public GLSLShader {
public:
 GPUPulsingMultiLight2d *bulbs; // GPULights2d[16] as input;
 GPUPulsingLights2dUniformSet uniforms[GPUMULTILIGHTPULSINGLIMIT];
 GLint UT;
 float dt;
 GLuint Utrans;
 GLint posAttrib,texAttrib;

 void SetFromSnapshot( GPUPulsingMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i]=lights[i];
  }
 }

 void GetFromSnapshot( GPUPulsingMultiLight2d *lights ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   lights[i]=bulbs[i];
  }
 }

 void Init() {
  OUTPUT("LightsPulsing2DShader::Loading\n");
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
   string("uniform float T;\n")+
   string("uniform vec2 lightPos1;\n")+
   string("uniform vec3 lightColor1;\n")+
   string("uniform float R1, hz1;\n")+
   string("uniform vec2 lightPos2;\n")+
   string("uniform vec3 lightColor2;\n")+
   string("uniform float R2, hz2;\n")+
   string("uniform vec2 lightPos3;\n")+
   string("uniform vec3 lightColor3;\n")+
   string("uniform float R3, hz3;\n")+
   string("/* frag uniform limit exceeded beyond this */\n")+
   string("vec3 addColor( vec3 a, vec3 b ) { return a+b; }\n")+
   string("float atten2d( vec2 p, float invR ) {\n")+
   string(" float t=(1.0 - sqrt(p.x*p.x + p.y*p.y));\n")+
   string(" return 1.0 / (0.0001+ exp(-(t*100.0 - (100.0-invR))));\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = Texcoord;\n")+
   string(" float D;\n")+
   string(" D = atten2d(lightPos1-texCoord,R1); vec3 mixed = (lightColor1 *D*sin(T/hz1));\n")+
   string(" D = atten2d(lightPos2-texCoord,R2); mixed=addColor(mixed,(lightColor2*D*sin(T/hz2)));\n")+
   string(" D = atten2d(lightPos3-texCoord,R3); mixed=addColor(mixed,(lightColor3*D*sin(T/hz3)));\n")+
   string(" outColor=vec4(mixed,1.0);\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
  dt=0.0f;
 }

 void InitUniforms() {
  GL_Assert("entering LightsPulsing2DShader::InitUniforms");

  UT=glGetUniformLocation(program,"T");

  uniforms[0].UlightPos  =glGetUniformLocation(program,"lightPos1");
  uniforms[0].UlightColor=glGetUniformLocation(program,"lightColor1");
  uniforms[0].Uradius    =glGetUniformLocation(program,"R1");
  uniforms[0].Ucycle     =glGetUniformLocation(program,"hz1");

  uniforms[1].UlightPos  =glGetUniformLocation(program,"lightPos2");
  uniforms[1].UlightColor=glGetUniformLocation(program,"lightColor2");
  uniforms[1].Uradius    =glGetUniformLocation(program,"R2");
  uniforms[1].Ucycle     =glGetUniformLocation(program,"hz2");

  uniforms[2].UlightPos  =glGetUniformLocation(program,"lightPos3");
  uniforms[2].UlightColor=glGetUniformLocation(program,"lightColor3");
  uniforms[2].Uradius    =glGetUniformLocation(program,"R3");
  uniforms[2].Ucycle     =glGetUniformLocation(program,"hz3");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");

  GL_Assert("exitting LightsPulsing2DShader::InitUniforms");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  dt+=FRAMETIME;
  glUseProgram(program);
  glUniform1f(UT,dt);
  LOOP(i,GPUMULTILIGHTLIMIT,{
   glUniform2f(uniforms[i].UlightPos,    bulbs[i].position[0], bulbs[i].position[1]);
   glUniform3f(uniforms[i].UlightColor,  bulbs[i].color.rf, bulbs[i].color.gf, bulbs[i].color.bf);
   glUniform1f(uniforms[i].Uradius,      bulbs[i].radius );
   glUniform1f(uniforms[i].Ucycle,       bulbs[i].cycle );
  });

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
  glUseProgram(0);
 }
};

extern LightsPulsing2DShader lightspulsing2dShader;

class Shadows2DShader: public GLSLShader {
public:
 Zfloat position[2];
 Zfloat radius;
 Crayon color;
 Zp<GLuint> obstructions;
 
  // Uniform locations
 GLint Uradius, UlightPos, UlightColor, Uobstructions;
 GLuint Utrans;
 GLint posAttrib,texAttrib;

 void Init() {
  position[0]=512.0f;
  position[1]=512.0f;
  color.Pick(alabaster);
  OUTPUT("Shadows2DShader::Loading\n");
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
   string("uniform vec2 lightPos;\n")+
   string("uniform vec4 lightColor;\n")+
   string("uniform sampler2D obstructions;\n")+
   string("uniform float radius;\n")+
   string("vec2 lerp2(vec2 a, vec2 b, float s) {\n")+
   string(" return vec2(a + (b - a) * s);\n")+
   string("}\n")+
   string("void main(void) {\n")+
   string(" vec2 texCoord = Texcoord;\n")+
   string(" vec4 texColor = texture2D(obstructions,texCoord);\n")+
   string(" float D =  1.0-(length(lightPos - texCoord))*(3.0/radius);//*30.0;\n")+
   string(" float Brightness = 1.0;\n")+
   string(" float p=0.0;\n")+
   string(" // Cast a line from pos to light, if it hits somthing, render this one off..\n")+
   string(" for( ; p < 1.0; p+=0.005 ) {  // 0.0025 works, as does 0.001, but less is slow\n")+
   string("  vec4 test=texture2D(obstructions,lerp2(lightPos,texCoord,1.0 / (p+1.0)));\n")+
   string("  if(test.a != 0.0) {\n")+
   string("   Brightness=0.0;\n")+
   string("   break;\n")+
   string("  }\n")+
   string(" }\n")+
   string(" outColor =lightColor*D*Brightness;\n")+
   string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
  area.SetRect(0,0,display.w,display.h);
 }

 void InitUniforms() {
  UlightPos=glGetUniformLocation(program,"lightPos");
  UlightColor=glGetUniformLocation(program,"lightColor");
  Uobstructions=glGetUniformLocation(program,"obstructions");
  Uradius=glGetUniformLocation(program,"radius");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }

 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);

  glUniform1f(Uradius,radius);
  glUniform2f(UlightPos, position[0], position[1]);
  glUniform4f(UlightColor,color.floats[0],color.floats[1],color.floats[2],color.floats[3]);

  // Assign texture IDs used by glActiveTexture (see Apply)
  glUniform1i(Uobstructions, 0);
  glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *obstructions);

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
  glUseProgram(0);
 }
};

extern Shadows2DShader shadows2dShader;

#include "TextureLibrary.h"

ONE(Shadow2DProfile,{})
 Cartesiand pos;
 Crayon ambience;
  Zp<GLImage> emap,addlightmap,lightmap,backlight,heightmap,colormap;
 KEYWORDS({
  SUBWORD("pos",pos)
  else TINTWORD("ambience",ambience)
  else TILEWORD("emap",emap)
  else TILEWORD("lightmap",lightmap)
  else TILEWORD("addlightmap",addlightmap)
  else TILEWORD("backlight",backlight)
  else TILEWORD("heightmap",heightmap)
  else TILEWORD("colormap",colormap)
 })
 WORDKEYS({
  SSUBKEY("pos",pos)
  TINTKEY("ambience",ambience)
  IMGKEY("lightmap",lightmap)
  IMGKEY("emap",emap)
  IMGKEY("addlightmap",addlightmap)
  IMGKEY("backlight",backlight)
  IMGKEY("heightmap",heightmap)
  IMGKEY("colormap",colormap)
 })
MANY(Shadow2DProfile,Shadow2DProfileHandle,Shadow2DProfileHandles,"Shadow2D",Shadow2DProfiles,{})
 KEYWORDSGroup("Shadow2D",Shadow2DProfile);
 WORDKEYSGroup("Shadow2D",Shadow2DProfile);
DONE(Shadow2DProfile);

class Shadow2DShader : public GLSLShader {
public:
 Cartesiand pos;
 Crayon ambience;
 Zp<GLuint> emap,addlightmap,lightmap,backlight,heightmap,colormap;
 GLuint Upos,Uambience,Uemap,Uaddlightmap,Ulightmap,Ubacklight,Uheightmap,Ucolormap;
 GLuint Utrans;
 GLint posAttrib,texAttrib;
 void Init() {
  OUTPUT("Shadow2DShader::Loading\n");
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
   +string("out vec4 outColor;\n")
   +string("const int num = 256;\n")
   +string("const float i = 1.0;// intensity (should be between 1 and 0)\n")
   +string("const float val = 1.0/float(num);\n")
   +string("\n")
   +string("uniform vec3 pos;\n")
   +string("uniform vec3 ambience;\n")
   +string("uniform sampler2D colormap;\n")
   +string("uniform sampler2D lightmap;\n")
   +string("uniform sampler2D emap;\n")
   +string("uniform sampler2D addlightmap;\n")
   +string("uniform sampler2D backlight;\n")
   +string("uniform sampler2D heightmap;\n")
   +string("void main()\n")
   +string("{\n")
   +string("    float shadow=0.0;\n")
   +string("    float height = texture2D(heightmap,Texcoord).r-(1.0-pos.z);\n")
   +string("    for(int Z=0;Z<num;Z++)\n")
   +string("        {\n")
   +string("            shadow += clamp(texture2D(heightmap,Texcoord * (1.0-(float(Z)*val)*(i))\n")
   +string("            +(float(Z)*val*pos.xy*i*0.5)).r-float(Z)*val-height,0.0,1.0);\n")
   +string("        }\n")
   +string("    vec4 BS=texture2D(backlight,Texcoord);")
   +string("    float BL=(BS.r+BS.g+BS.b)/3.0;\n")
   +string("    vec4 CM=texture2D(colormap,Texcoord);\n")
   +string("    vec4 AM=vec4(ambience*texture2D(colormap,Texcoord).rgb,1.0);\n")
   +string("    vec4 LM=texture2D(lightmap,Texcoord);\n")
   +string("    vec4 ALM=texture2D(addlightmap,Texcoord);\n")
   +string("    vec4 EM=texture2D(emap,Texcoord);\n")
   +string("    float LI=(LM.r+LM.g+LM.b)/3.0;\n")
   +string("    float ALI=1.0-(ALM.r+ALM.g+ALM.b)/3.0;\n")
   +string("    float M=clamp(shadow+(BL-LI)+ALI,0.0,1.0);\n")
   +string("    outColor = mix( EM + CM * LM , AM, vec4(M) );\n")
   +string("}\n")
  ;
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Upos=glGetUniformLocation(program, "pos");
  Uambience=glGetUniformLocation(program, "ambience");
  Ulightmap=glGetUniformLocation(program, "lightmap");
  Uemap=glGetUniformLocation(program, "emap");
  Uaddlightmap=glGetUniformLocation(program, "addlightmap");
  Ubacklight=glGetUniformLocation(program, "backlight");
  Uheightmap=glGetUniformLocation(program, "heightmap");
  Ucolormap=glGetUniformLocation(program, "colormap");
  Utrans = glGetUniformLocation(program,"trans");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glUniform3f(Upos,(GLfloat)pos.x,(GLfloat)pos.y,(GLfloat)pos.z);
  glUniform3f(Uambience,(GLfloat)ambience.rf,(GLfloat)ambience.gf,(GLfloat)ambience.bf);
  glUniform1i(Ulightmap,    0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D,  (*lightmap.pointer));
  glUniform1i(Uaddlightmap, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D,  (*addlightmap.pointer));
  glUniform1i(Ubacklight,   2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*backlight.pointer));
  glUniform1i(Uheightmap,   3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*heightmap.pointer));
  glUniform1i(Ucolormap,    4);  glActiveTexture(GL_TEXTURE4);  glBindTexture(GL_TEXTURE_2D,  (*colormap.pointer));
  glUniform1i(Uemap,        5);  glActiveTexture(GL_TEXTURE5);  glBindTexture(GL_TEXTURE_2D,  (*emap.pointer));

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
 void Set( Shadow2DProfile *in ) {
  pos.Set(&in->pos);
  ambience.fromCrayon(in->ambience);
  lightmap=&in->lightmap->texture;
  emap=&in->emap->texture;
  addlightmap=&in->addlightmap->texture;
  backlight=&in->backlight->texture;
  heightmap=&in->heightmap->texture;
  colormap=&in->colormap->texture;
 }
 void Render( FBOColor *out, Shadow2DProfile *in );
};


extern Shadow2DShader shadow2dShader;

void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in );
void ShadedShadow2D( FBOColor *out, Shadow2DProfile *in, GLuint *color, GLuint *addlight, GLuint *light, GLuint *emap, GLuint *back, GLuint *height );


ONE(MultiShadow2DProfile,{})
 Cartesiand pos,pos1,pos2,pos3,pos4,pos5,pos6,pos7;
 Crayon ambience;
  Zp<GLImage> emap,addlightmap,lightmap,backlight,heightmap,colormap;
  Zint activeLights;
  void Off() {
   activeLights=0;
   pos.Zero(); pos.x=-10001.0;
   pos1.Zero(); pos1.x=-10001.0;
   pos2.Zero(); pos2.x=-10001.0;
   pos3.Zero(); pos3.x=-10001.0;
   pos4.Zero(); pos4.x=-10001.0;
   pos5.Zero(); pos5.x=-10001.0;
   pos6.Zero(); pos6.x=-10001.0;
   pos7.Zero(); pos7.x=-10001.0;
  }
  void On( double x, double y, double z ) {
   if ( activeLights > 7 ) return;
   switch ( activeLights.value ) {
    case 0: pos.Set(x,y,z); break;
    case 1: pos1.Set(x,y,z); break;
    case 2: pos2.Set(x,y,z); break;
    case 3: pos3.Set(x,y,z); break;
    case 4: pos4.Set(x,y,z); break;
    case 5: pos5.Set(x,y,z); break;
    case 6: pos6.Set(x,y,z); break;
    case 7: pos7.Set(x,y,z); break;
   }
   activeLights++;
  }
 KEYWORDS({
  SUBWORD("pos",pos)
  else SUBWORD("pos",pos1)
  else SUBWORD("pos",pos2)
  else SUBWORD("pos",pos3)
  else SUBWORD("pos",pos4)
  else SUBWORD("pos",pos5)
  else SUBWORD("pos",pos6)
  else SUBWORD("pos",pos7)
  else TINTWORD("ambience",ambience)
  else TILEWORD("emap",emap)
  else TILEWORD("lightmap",lightmap)
  else TILEWORD("addlightmap",addlightmap)
  else TILEWORD("backlight",backlight)
  else TILEWORD("heightmap",heightmap)
  else TILEWORD("colormap",colormap)
 })
 WORDKEYS({
  SSUBKEY("pos",pos)
  SSUBKEY("pos1",pos1)
  SSUBKEY("pos2",pos2)
  SSUBKEY("pos3",pos3)
  SSUBKEY("pos4",pos4)
  SSUBKEY("pos5",pos5)
  SSUBKEY("pos6",pos6)
  SSUBKEY("pos7",pos7)
  TINTKEY("ambience",ambience)
  IMGKEY("lightmap",lightmap)
  IMGKEY("emap",emap)
  IMGKEY("addlightmap",addlightmap)
  IMGKEY("backlight",backlight)
  IMGKEY("heightmap",heightmap)
  IMGKEY("colormap",colormap)
 })
MANY(MultiShadow2DProfile,MultiShadow2DProfileHandle,MultiShadow2DProfileHandles,"MultiShadow2D",MultiShadow2DProfiles,{})
 KEYWORDSGroup("MultiShadow2D",MultiShadow2DProfile);
 WORDKEYSGroup("MultiShadow2D",MultiShadow2DProfile);
DONE(MultiShadow2DProfile);

class MultiShadow2DShader : public GLSLShader {
public:
  Cartesiand pos,pos1,pos2,pos3,pos4,pos5,pos6,pos7;
  Crayon ambience;
  Zint activeLights;
  Zp<GLuint> emap,addlightmap,lightmap,backlight,heightmap,colormap;
  GLuint Upos,Upos1,Upos2,Upos3,Upos4,Upos5,Upos6,Upos7,Ucount,
         Uambience,Uemap,Uaddlightmap,Ulightmap,Ubacklight,Uheightmap,Ucolormap,Utrans;
  GLint posAttrib, texAttrib;
 void Init();
 void InitUniforms() {
  glReportError( glGetError() );
  Upos=glGetUniformLocation(program, "pos");
  Upos1=glGetUniformLocation(program, "pos1");
  Upos2=glGetUniformLocation(program, "pos2");
  Upos3=glGetUniformLocation(program, "pos3");
  Upos4=glGetUniformLocation(program, "pos4");
  Upos5=glGetUniformLocation(program, "pos5");
  Upos6=glGetUniformLocation(program, "pos6");
  Upos7=glGetUniformLocation(program, "pos7");
  Ucount=glGetUniformLocation(program, "count");
  Uambience=glGetUniformLocation(program, "ambience");
  Ulightmap=glGetUniformLocation(program, "lightmap");
  Uemap=glGetUniformLocation(program, "emap");
  Uaddlightmap=glGetUniformLocation(program, "addlightmap");
  Ubacklight=glGetUniformLocation(program, "backlight");
  Uheightmap=glGetUniformLocation(program, "heightmap");
  Utrans = glGetUniformLocation(program,"trans");
  Ucolormap=glGetUniformLocation(program, "colormap");
  posAttrib = glGetAttribLocation(program, "position");
  texAttrib = glGetAttribLocation(program, "texcoord");
  glReportError( glGetError() );
 }
 void UpdateUniforms( glm::mat4 trans ) {
  glUseProgram(program);
  glReportError( glGetError() );
  glUniform3f(Upos,(GLfloat)pos.x,(GLfloat)pos.y,(GLfloat)pos.z);
  glUniform3f(Upos1,(GLfloat)pos1.x,(GLfloat)pos1.y,(GLfloat)pos1.z);
  glUniform3f(Upos2,(GLfloat)pos2.x,(GLfloat)pos2.y,(GLfloat)pos2.z);
  glUniform3f(Upos3,(GLfloat)pos3.x,(GLfloat)pos3.y,(GLfloat)pos3.z);
  glUniform3f(Upos4,(GLfloat)pos4.x,(GLfloat)pos4.y,(GLfloat)pos4.z);
  glUniform3f(Upos5,(GLfloat)pos5.x,(GLfloat)pos5.y,(GLfloat)pos5.z);
  glUniform3f(Upos6,(GLfloat)pos6.x,(GLfloat)pos6.y,(GLfloat)pos6.z);
  glUniform3f(Upos7,(GLfloat)pos7.x,(GLfloat)pos7.y,(GLfloat)pos7.z);
  glUniform1i(Ucount,(GLint)activeLights.value);
  glUniformMatrix4fv(Utrans, 1, GL_FALSE, glm::value_ptr(trans));
  glUniform3f(Uambience,(GLfloat)ambience.rf,(GLfloat)ambience.gf,(GLfloat)ambience.bf);
  glUniform1i(Ulightmap,    0);  glActiveTexture(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, (*lightmap.pointer));
  glUniform1i(Uaddlightmap, 1);  glActiveTexture(GL_TEXTURE1);  glBindTexture(GL_TEXTURE_2D, (*addlightmap.pointer));
  glUniform1i(Ubacklight,   2);  glActiveTexture(GL_TEXTURE2);  glBindTexture(GL_TEXTURE_2D, (*backlight.pointer));
  glUniform1i(Uheightmap,   3);  glActiveTexture(GL_TEXTURE3);  glBindTexture(GL_TEXTURE_2D, (*heightmap.pointer));
  glUniform1i(Ucolormap,    4);  glActiveTexture(GL_TEXTURE4);  glBindTexture(GL_TEXTURE_2D, (*colormap.pointer));
  glUniform1i(Uemap,        5);  glActiveTexture(GL_TEXTURE5);  glBindTexture(GL_TEXTURE_2D, (*emap.pointer));
  glReportError( glGetError() );
  glReportError(glGetError());
  glEnableVertexAttribArray(posAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glReportError(glGetError());
  glEnableVertexAttribArray(texAttrib);
  glReportError(glGetError());
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glReportError(glGetError());
  glReportError( glGetError() );
 }
 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
 void Set( MultiShadow2DProfile *in ) {
  pos.Set(&in->pos);
  pos1.Set(&in->pos1);
  pos2.Set(&in->pos2);
  pos3.Set(&in->pos3);
  pos4.Set(&in->pos4);
  pos5.Set(&in->pos5);
  pos6.Set(&in->pos6);
  pos7.Set(&in->pos7);
  activeLights=in->activeLights;
  ambience.fromCrayon(in->ambience);
  lightmap=&in->lightmap->texture;
  emap=&in->emap->texture;
  addlightmap=&in->addlightmap->texture;
  backlight=&in->backlight->texture;
  heightmap=&in->heightmap->texture;
  colormap=&in->colormap->texture;
 }
 void Render( FBOColor *out, MultiShadow2DProfile *in );
};


extern MultiShadow2DShader multishadow2dShader;

void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in );
void MultiShadedShadow2D( FBOColor *out, MultiShadow2DProfile *in, GLuint *colormap, GLuint *addlightmap, GLuint *lightmap, GLuint *emissionmap, GLuint *backlight, GLuint *heightmap );