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

#include <Windows.h>
#include "GLSetup.h"

//#include "dhpowareGL3.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"

#include "Matrix16.h"

#define SHADER_NORMALMAP    1
#define SHADER_SPECULARMAP  2
#define SHADER_GOOCH        4
#define SHADER_OVERGOOCH    8
#define SHADER_PARALLAX     16
#define SHADER_GLOSS        32
#define SHADER_DITHER       64
#define SHADER_ADDITIVE     128
#define SHADER_SUBTRACTIVE  256
#define SHADER_ADDITIVE2    512
#define SHADER_SUBTRACTIVE2 1024
#define SHADER_LUMINANCE    2048

enum ShaderTypes {
 Vert=GL_VERTEX_SHADER,
 Geo=GL_GEOMETRY_SHADER,
 Frag=GL_FRAGMENT_SHADER,
 Tess=-1//GL_TESSELATION_SHADER
};

GLenum ShaderType( ShaderTypes type );

enum UniformMatrices { m2x1=0, m3x1=1, m4x1=2, m2x3=3, m3x2=4, m2x4=5, m3x4=6, m4x3=7 };

class Uniform : public ListItem {
public:
 GLint location;
 int matrixW, matrixH, size;
 float *matrixFloat;
 int *matrixInt;
 bool transpose;
 int number;
 string name;
 float value[4];
 int integer[4];
 Uniform() {
  name=string("");
  prev=next=null;
  matrixW=matrixH=size=0;
  matrixFloat=null;
  matrixInt=null;
  transpose=false;
  number=0;
  update=&Uniform::Uniform0;
  location=0;
  for ( int i=0; i<4; i++ ){ 
   value[i]=0.0f;
   integer[i]=0;
  }
 }
 void Uniform0() { }
 void Uniform1f()          { glUniform1f( location, value[0] ); }
 void Uniform2f()          { glUniform2f( location, value[0], value[1] ); }
 void Uniform3f()          { glUniform3f( location, value[0], value[1], value[2] ); }
 void Uniform4f()          { glUniform4f( location, value[0], value[1], value[2], value[3] ); }
 void Uniform1i()          { glUniform1i( location, integer[0] ); }
 void Uniform2i()          { glUniform2i( location, integer[0], integer[1] ); }
 void Uniform3i()          { glUniform3i( location, integer[0], integer[1], integer[2] ); }
 void Uniform4i()          { glUniform4i( location, integer[0], integer[1], integer[2], integer[3] ); }
 void Uniform1fv()         { glUniform1fv( location, size, matrixFloat ); }
 void Uniform2fv()         { glUniform1fv( location, size, matrixFloat ); }
 void Uniform3fv()         { glUniform1fv( location, size, matrixFloat ); }
 void Uniform4fv()         { glUniform1fv( location, size, matrixFloat ); }
 void Uniform1iv()         { glUniform1iv( location, size, matrixInt ); }
 void Uniform2iv()         { glUniform2iv( location, size, matrixInt ); }
 void Uniform3iv()         { glUniform3iv( location, size, matrixInt ); }
 void Uniform4iv()         { glUniform4iv( location, size, matrixInt ); }
 void UniformMatrix2fv()   { glUniformMatrix2fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix3fv()   { glUniformMatrix3fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix4fv()   { glUniformMatrix4fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix2x3fv() { glUniformMatrix2x3fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix3x2fv() { glUniformMatrix3x2fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix2x4fv() { glUniformMatrix2x4fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix3x4fv() { glUniformMatrix3x4fv( location, size, transpose, matrixFloat ); }
 void UniformMatrix4x3fv() { glUniformMatrix4x3fv( location, size, transpose, matrixFloat ); }
 typedef void (Uniform::*FuncPtr)(); 
 Uniform::FuncPtr update;
 void Update() {
   (((Uniform *)this)->*(Uniform::update))();
 }
 void Locate( GLuint shaderProgramHandle ) {
  location = glGetUniformLocation(shaderProgramHandle, (const GLchar *) name.c_str());
 }
 float *Matrix( UniformMatrices width ) {
  switch ( width ) {
   case m2x1: size=2;  matrixW=2;  matrixH=1; update=&Uniform::UniformMatrix2fv; break;
   case m3x1: size=3;  matrixW=3;  matrixH=1; update=&Uniform::UniformMatrix3fv; break;
   case m4x1: size=4;  matrixW=4;  matrixH=1; update=&Uniform::UniformMatrix4fv; break;
   case m2x3: size=6;  matrixW=6;  matrixH=3; update=&Uniform::UniformMatrix2x3fv; break;
   case m3x2: size=6;  matrixW=6;  matrixH=2; update=&Uniform::UniformMatrix3x2fv; break;
   case m2x4: size=8;  matrixW=8;  matrixH=4; update=&Uniform::UniformMatrix2x4fv; break;
   case m3x4: size=12; matrixW=12; matrixH=4; update=&Uniform::UniformMatrix3x4fv; break;
   case m4x3: size=12; matrixW=12; matrixH=3; update=&Uniform::UniformMatrix4x3fv; break;
   default: OUTPUT( "Uniform:Matrix:UniformMatrices Invalid Type\n" ); break;
  }
  /// Cppcheck notes: Common realloc mistake: 'matrixInt' nulled but not freed upon failure
  /*
  2015 Code analyzer says:
  c:\users\moops\documents\visual studio 2015\projects\framework\framework\shader.h(125): warning C6308: 'realloc' might return null pointer: assigning null pointer to 'matrixInt', which is passed as an argument to 'realloc', will cause the original memory block to be leaked.
  */
  matrixInt=(int *) realloc(matrixInt,size*sizeof(int));
  for ( int i=0; i<size; i++ ) matrixFloat[i]=0;
  return matrixFloat;
 }
 void Float( float v ) {
  update=&Uniform::Uniform1f;
  value[0]=v;
 }
 void Integer( int v ) {
  update=&Uniform::Uniform1i;
  integer[0]=v;
 }
 float *FloatArray( int parameter_count ) {  
  switch ( parameter_count ) {
   case 1: update=&Uniform::Uniform1fv; break;
   case 2: update=&Uniform::Uniform2fv; break;
   case 3: update=&Uniform::Uniform3fv; break;
   case 4: update=&Uniform::Uniform4fv; break;
   default: OUTPUT("Uniform:FloatArray() given incorrect parameter count.\n"); break;
  }
  return value;
 }
 void SetArrayf( float a, float b, float c, float d ) {
  value[0]=a;
  value[1]=b;
  value[2]=c;
  value[3]=d;
 }
 void SetArrayi( int a, int b, int c, int d ) {
  integer[0]=a;
  integer[1]=b;
  integer[2]=c;
  integer[3]=d;
 }
 int *IntegerArray( int parameter_count ) {  
  switch ( parameter_count ) {
   case 1: update=&Uniform::Uniform1iv; break;
   case 2: update=&Uniform::Uniform2iv; break;
   case 3: update=&Uniform::Uniform3iv; break;
   case 4: update=&Uniform::Uniform4iv; break;
   default: OUTPUT("Uniform:IntegerArray() given incorrect parameter count.\n"); break;
  }
  /// Cppcheck notes: Common realloc mistake: 'matrixInt' nulled but not freed upon failure
  /*
  MSVC 2015 Code analyzer says:
  c:\users\moops\documents\visual studio 2015\projects\framework\framework\shader.h(168): warning C6308: 'realloc' might return null pointer: assigning null pointer to 'matrixInt', which is passed as an argument to 'realloc', will cause the original memory block to be leaked.
  */
  matrixInt=(int *) realloc(matrixInt,size*sizeof(int));
  return integer;
 }
 ~Uniform() {
  if ( matrixInt ) free(matrixInt);
  if ( matrixFloat ) free(matrixFloat);
 }
};

class Uniforms : public LinkedList {
public:
 void Add( const char *a, float value ) {
  Uniform *u=new Uniform;
  u->name=string(a);
  u->value[0]=value;
  Append(u);
 }
 void Add( const char *a, int value ) {
  Uniform *u=new Uniform;
  u->name=string(a);
  u->integer[0]=value;
  Append(u);
 }
 CLEARLISTRESET(Uniform);
 ~Uniforms() { Clear(); }
};

class Attribute : public ListItem {
public:
 string name;
};

class Attributes : public LinkedList {
public:
 void Add( const char *a ) {
  Attribute *u=new Attribute;
  u->name=string(a);
  Append(u);
 }
 CLEARLISTRESET(Attribute);
 ~Attributes() { Clear(); }
};

class Shader : public ListItem {
public:
 Strings options;
 string name,source;
 GLuint handle;
 GLuint shader_object;
 ShaderTypes type;
 int lights;
 char *filecontents;
 string parsed,equation,lightloop;
 void Log( const char *sourcefile );
 bool Compile(Strings *src, const char *sourcefile, ShaderTypes t, int num_lights, const char *equation, const char *lightloop);
 Shader(void)
 {
  name=string("");
  source=string("");
  filecontents=null;
  parsed=string("");
  handle=0;
  shader_object=0;
  next=prev=null;
  type=Vert;
  lights=0;
 }
 ~Shader(void)
 {
  if ( filecontents ) free(filecontents);
  if (handle != 0) {
   glDeleteShader(handle);
   glGetError();
  }
 }
};

class Shaders : public LinkedList {
public:
 Shader *find(Strings *options, const char *shader, ShaderTypes type, int num_lights, const char *equation, const char *lightloop);
 CLEARLISTRESET(Shader);
 ~Shaders(void) {
  Clear();
 }
};

class ShaderProgram : public ListItem {
public:
 Shader *vert, *frag;
 Strings options;
 unsigned int lights;
 GLuint handle;             // OpenGL handle for linked program object. 
 GLint validate_status;
 unsigned int last_updated; // The frame number for which this shader's uniforms were most 
                            // recently updated. Since frame-counting starts at 1, 0 means that
                            // they haven't been updated..
 // Uniform locations that are looked up when the shader program is linked. There is a
 // pre-defined set of uniforms (see ShaderUniform enum), all of which get looked up, since most
 // shaders will not use all of these uniforms, some will be set to -1.
 Uniforms uniforms;
 Attributes attributes;
 void Update(float time_elapsed);
 void Log(); // Called by Link
 void Validate(); // Called by Link
 void Link(Strings *ops, Shader *vshader, Shader *fshader, int num_lights);
 ShaderProgram() {
  vert=frag=null;
  handle=0;
  validate_status=0;
  last_updated=0;
  lights=0;
 }
 ~ShaderProgram() {
  glDeleteProgram(handle);
 }
};

class ShaderPrograms : public LinkedList {
public:
 ShaderProgram *findv(Shaders *shaders, Strings *ops, const char *vshader);
 ShaderProgram *findf(Shaders *shaders, Strings *ops, const char *fshader);
 ShaderProgram *find( Shaders *shaders, Strings *ops, const char *vshader, const char *fshader, int num_lights, const char*equation, const char*lightloop );
 ShaderProgram *Add(  Shaders *shaders, Strings *ops, const char *vshader, const char *fshader);
 ShaderProgram *Addv( Shaders *shaders, Strings *ops, const char *vshader );
 ShaderProgram *Addf( Shaders *shaders, Strings *ops, const char *fshader );
 ShaderProgram *Add(  Strings *ops, Shader *vshader, Shader *fshader);
 ShaderProgram *Addv( Strings *ops, Shader *vshader);
 ShaderProgram *Addf( Strings *ops, Shader *fshader);
 void Update( float dt ) {
  FOREACH(ShaderProgram,s) s->Update(dt);
 }
 CLEARLISTRESET(ShaderProgram);
 ~ShaderPrograms() { Clear(); }
};

class ShaderLibrary {
public:
 Shaders shaders;
 ShaderPrograms shaderprograms;
 ShaderProgram *find(Strings *options, const char *vshader, const char *fshader) {
  return shaderprograms.find(&shaders,options,vshader,fshader,1,"","");
 }
 ShaderProgram *find(Strings *options, const char *vshader, const char *fshader, int num_lights) {
  return shaderprograms.find(&shaders,options,vshader,fshader,num_lights,"","");
 }
 ShaderProgram *find(Strings *options, const char *vshader, const char *fshader, int num_lights, const char *equation, const char *lightloop) {
  return shaderprograms.find(&shaders,options,vshader,fshader,num_lights, equation, lightloop);
 }
 Shader *AddShader(Strings *options, const char *pre, ShaderTypes type ) {
  return shaders.find(options,pre,type,1,"","");
 }
 ShaderProgram *AddProgramv( Strings *options, const char *v ) {
  return shaderprograms.findv(&shaders,options,v);
 }
 ShaderProgram *AddProgramf( Strings *options, const char *f ) {
  return shaderprograms.findf(&shaders,options,f);
 }
 ShaderProgram *AddProgram( Strings *options, const char *v, const char *f ) {
  return shaderprograms.find(&shaders,options,v,f,1,"","");
 }
 ShaderProgram *AddProgramv( Strings *options, Shader *v ) {
  return shaderprograms.Addv(options,v);
 }
 ShaderProgram *AddProgramf( Strings *options, Shader *f ) {
  return shaderprograms.Addf(options,f);
 }
 ShaderProgram *AddProgram( Strings *options, Shader *v, Shader *f ) {
  return shaderprograms.Add(options,v,f);
 }
};

extern ShaderLibrary shaders;