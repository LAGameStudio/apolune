
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <stdio.h>
#include <malloc.h>

#include "ListItem.h"
#include "LinkedList.h"
#include "Shader.h"

#include "Strings.h"

#if defined(GL_DEBUG)
extern void glReportError( GLenum err );
#endif

GLenum ShaderType( ShaderTypes type ) {
 switch ( type ) {
 case Vert: return GL_VERTEX_SHADER;
 case Frag: return GL_FRAGMENT_SHADER;
  case Geo: return GL_GEOMETRY_SHADER;
 case Tess: return -1;
 default: return 0;
 }
}

// Set shader uniform values.  Called once per frame.
void ShaderProgram::Update( float time_elapsed )
{
 for ( Uniform *u=(Uniform *) uniforms.first; u; u=(Uniform *) (u->next) ) if ( u->location >=0 ) u->Update();
}

/* old = do not use
// Read entire contents of file into NULL-terminated string and return a pointer to it. Caller
// should free the string when it is done using it. Returns NULL on error.
int file_as_string( char *filename, char **result) 
{ 
	int size = 0;
	FILE *f;
 fopen_s(&f,filename,"r");
	if (f == NULL) { 
		*result = NULL;
		return -1; // -1 means file opening fail 
	} 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)calloc(size+1,sizeof(char));
	if (size != fread(*result, sizeof(char), size, f)) { 
		free(*result);
		return -2; // -2 means file reading fail 
	} 
	fclose(f);
	(*result)[size] = 0;
	return size;
}
*/

void Shader::Log( const char * sourcefile ) {
 GLint log_length;
 glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &log_length);
 if (log_length > 1) {
  char *log;
  OUTPUT("Shader:Compile log (length %d) for %s: \n", (int) log_length, sourcefile);
  log = (char *) malloc(log_length+1);
  if (log) {
   GLsizei log_return_length = 0;
   glGetShaderInfoLog(shader_object, log_length, &log_return_length, log);
   if (log_return_length) OUTPUT("  %s\n", log);
   else OUTPUT("Shader:Compile failed to get shader compilation log.\n");
   free(log);
  } else OUTPUT("Shader:Compile failed to allocate memory for compilation log.\n");
 }
}

// Load shader from source and compile it into a shader object. Returns valid shader object
// reference, or 0 on error. 
bool Shader::Compile(Strings *src, const char *sourcefile, ShaderTypes t, int num_lights, const char *eq, const char *ll )
{
 type=t;
 source=src->toString();

 if (source.length() <=0 || src->count <=0 ) {
  OUTPUT("Shader:Compile: Failed to read shader source for \"%s\".\n", sourcefile);
  return false;
 }

 // Parse the incoming shader file

 // Study all classes declared in this file, WorldShader, Material and the .vert and .frag programs, to understand all
 // of the functionality, however here is a synopsis:
 //
 // In the .vert and .frag programs, the following keywords are used:
 //
 //  //loop//      designates the start and end of the lighting loop for multiple lights
 //                (this is a workaround since GLSL doesn't have array transport)
 //
 //  _n            a special symbol inside shaders that designates the area to replace with the current light #
 //
 //  //equation//  this code block is a specialized equation, in the case of this shader,
 //                the one that generates static on the surface, and can be replaced inside C++
 //
 //  //lightloop// may eventually provide a way to insert calculations into each light
 //
 //  #define NUM_LIGHTS   <- this line is replaced by the engine to reflect material settings
 //
 //  NUM_LIGHTS    on other lines, this keyword is replaced by the engine settings

 int eqLen = (int)strlen(eq);
 int llLen = (int)strlen(ll);
 int i = 0;

 parsed=string("");
 equation=string(eq);
 lightloop=string(ll);

 EACH(options.first,KeyValuePair,s) parsed = "#define " + s->s + " " + s->value + "\n";
 
 EACH(src->first,String,sL) {
  if ( !string_infix( "#define NUM_LIGHTS", sL->s ) ) {
   char buf[32];
   FORMAT(buf,32,"#define NUM_LIGHTS %d\n", (int)num_lights);
   parsed += string(buf);
  } else if ( !string_infix( "//loop//", sL->s ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//loop//" );
   sL=src->findNextSubstring( (String *) sL->next, "//loop//" );
   for ( i=0; i<num_lights; i++ ) {
    string t=s;
    int position=(int) t.find( "_n" );
    while ( position != string::npos ) 
    {
      char num[20];
      FORMAT( num,20, "_%d",(int) i );
      t.replace( position, 2, string(num) );
      position =(int) t.find( "_n", position + 2 );
    } 
    parsed += t + "\n";
   }
  } else if ( eqLen > 0 && !str_infix( "//equation//", (char *) sL->s.c_str() ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//equation//" );
   parsed += equation+"\n";
   sL=src->findNextSubstring( (String *) sL->next, "//equation//" );
  } else if ( llLen > 0 && !str_infix( "//lightloop//", (char *) sL->s.c_str() ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//lightloop//" );
   parsed += lightloop+"\n";
   sL=src->findNextSubstring( (String *) sL->next, "//lightloop//" );
  } else {
   parsed += sL->s + "\n";
  }
 }

// OUTPUT("%s\n", source);

 shader_object = glCreateShader( ShaderType(t) );
 if (!shader_object) {
  OUTPUT("Shader:Compile: An error occured trying to create shader object while processing \"%s\".\n", sourcefile);
#if defined(GL_DEBUG)
  glReportError( glGetError() );
#endif
  return false;
 }

 const GLchar *shader_source = parsed.c_str();// (char*)malloc( sizeof( char ) *(source.length() +1) );
// strcpy( (char *) shader_source, source.c_str() );
// OUTPUT( "Compiling: %s", shader_source );
 glShaderSource(shader_object, 1, &shader_source, NULL);
 glCompileShader(shader_object);
// free( (char *) shader_source);

 GLint shader_compiled;
 glGetShaderiv(shader_object, GL_COMPILE_STATUS, &shader_compiled);

 if (shader_compiled == GL_FALSE) Log(sourcefile);

 if (shader_compiled == GL_FALSE) {
  OUTPUT("Shader:Compile failed to compile while processing \"%s\".\n", sourcefile);
  glDeleteShader(shader_object);
  return false;
 }

 name   = string(sourcefile);
 handle = shader_object;
 lights = num_lights;
 return true;
}

void ShaderProgram::Log() {
 GLint log_length;
 glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
 if (log_length > 1) {
  char *log;
  OUTPUT("ShaderProgram:Link: Program log (length %d) for shaders\n%s, and %s: \n", 
         (int) log_length, vert ? vert->name.c_str() : "NULL", frag ? frag->name.c_str() : "NULL" );
  log = (char *) malloc(log_length+1);
  if (log) {
   GLsizei log_return_length = 0;
   glGetProgramInfoLog(handle, log_length, &log_return_length, log);
   if (log_return_length) OUTPUT("  %s\n", log);
   else
   OUTPUT("ShaderProgram:Link: Failed to get program info log.\n");
   free(log);
  } else OUTPUT("ShaderProgram:Link: Failed to allocate memory for program info log.\n");
 }
}

void ShaderProgram::Validate() {
// Validate program.
 // XXX: Should this maybe be done after uniforms have been assigned?
 glValidateProgram(handle);
 glGetProgramiv(handle, GL_VALIDATE_STATUS, &validate_status);

 if (!validate_status) {
  OUTPUT("ShaderProgram:Link: Failed to validate vert %s and frag %s.\n",
   vert ? vert->name : "NULL", frag ? frag->name : "NULL" );
  Log();
 }
}

// Create OpenGL shader program object using vshader and fshader. Either of them may be NULL, but
// not both. Returns 0 on failure, or a valid OpenGL shader program handle on success.
void ShaderProgram::Link(Strings *ops, Shader *vshader, Shader *fshader, int num_lights)
{
 glGetError();
 GLint program_linked = 0;
 vert=vshader;
 frag=fshader;
 if ( vshader || fshader ) {
  this->lights=num_lights;
  handle = glCreateProgram();
  if (handle) {
   if (vshader && vert->handle>0 ) glAttachShader(handle, vert->handle);
   if (fshader && frag->handle>0 ) glAttachShader(handle, frag->handle);
   glLinkProgram(handle);
   glGetProgramiv(handle, GL_LINK_STATUS, &program_linked);
			// Mark the two attached shaders for deletion. These two shaders
			// aren't deleted right now because both are already attached to a
			// shader program. When the shader program is deleted these two
			// shaders will be automatically detached and deleted.
 		//glDeleteShader(vshader->handle);
 		//glDeleteShader(fshader->handle);
   if (!program_linked) {
    Log(); 
    glDeleteProgram(handle);
    OUTPUT("ShaderProgram:Link: Failed to link program.\n");
#if defined(GL_DEBUG)
    glReportError( glGetError() );
#endif
    return;
   }
  } else {
   OUTPUT("ShaderProgram:Link: Failed to create new shader program.\n");
#if defined(GL_DEBUG)
   glReportError( glGetError() );
#endif
   return;
  }
 } else {
  OUTPUT("ShaderProgram:Link: Program called with both vert and frag as NULL...\n");
#if defined(GL_DEBUG)
  glReportError( glGetError() );
#endif
  return;
 }

 this->options.ClearThenCopy(ops);

 Validate();

  // Retrieve uniform/attrib locations.
 for ( Uniform *u=(Uniform *) (uniforms.first); u; u=(Uniform *) (u->next) ) u->Locate(handle);

 OUTPUT("ShaderProgram:Link reports validated successfully for:\nVertex: %s\nFragment: %s\n", vert->name.c_str(), frag->name.c_str() );

}



// Look up a compiled shader, or load and compile from disk if not found. If an error occurs or
// shader is an empty string, NULL is returned, otherwise a valid pointer to the shader.
Shader *Shaders::find(Strings *options, const char *shader, ShaderTypes type, int lights, const char *equation, const char *lightloop)
{
 Strings source;
 string e, l;
 e=string(equation);
 l=string(lightloop);

 FOREACH(Shader,s) {
  if ( !s->options.NotMatchesKeyValues(options) 
    && !str_cmp(s->name.c_str(),shader)
    && s->lights == lights
    && e==s->equation
    && l==s->lightloop ) return s;
 }

 // Not found, so load/add it.
 Shader *n=new Shader;
 string x=options->toString();
 OUTPUT("Shader:Compile:%s Loading shader \"%s\" %d lights from disk.\nWith options:\n%#x\n",
  type == Vert ? "Vert" : (type == Frag ? "Frag" : "unknown" ) ,
  shader, lights, x);
 source.Load(shader);
 n->options.ClearThenCopy(options);
 n->Compile(&source,shader,type,lights,equation,lightloop);
 Append(n);

 if (n) return n;
 OUTPUT("Shaders:find: Couldn't find: %s\n", shader );
 return NULL;
}

// Look up a shader program that matches vshader/fshader, or attempt to load it if not found.
// Returns NULL on failure. The vshader/fshader may not be NULL (but may be empty strings) and
// should each have no more than supported character length. Either vshader or fhader
// needs to be non-empty.
ShaderProgram *ShaderPrograms::find(Shaders *shaders, Strings *options, const char *vshader, const char *fshader, int num_lights, const char *equation, const char *lightloop )
{
 ShaderProgram *n;
 Shader *vertex_shader = NULL, *fragment_shader = NULL;

 EACH(first, ShaderProgram, c)
  if ( !c->options.NotMatchesKeyValues(options) 
    && num_lights == c->lights 
    && (c->vert && strcmp(c->vert->name.c_str(), vshader) == 0) 
    && (c->vert && strcmp(c->frag->name.c_str(), fshader) == 0) ) return c;

 // If not, lookup the vertex and fragment shader and link them into a new program.
 if ( strlen(vshader) && !(vertex_shader = shaders->find(options, vshader, Vert, num_lights, equation, lightloop)) ) {
  OUTPUT("ShaderProgram:find: Failed to load vertex shader \"%s\".\n", vshader);
  return NULL;
 }

 if (strlen(fshader) && !(fragment_shader = shaders->find(options, fshader, Frag, num_lights, equation, lightloop)) ) {
  OUTPUT("ShaderProgram:find: Failed to load fragment shader \"%s\".\n", fshader);
  return NULL;
 }

 n=new ShaderProgram;
 n->Link(options, vertex_shader, fragment_shader, num_lights);
 Append(n);
 return n;
}

ShaderProgram *ShaderPrograms::findv(Shaders *shaders, Strings *options, const char *vshader)
{
 ShaderProgram *c, *n;
 Shader *vertex_shader = NULL;

 c=(ShaderProgram *) first;
 while (c != NULL) {
  if ( !c->options.NotMatchesKeyValues(options) 
   && c->lights == 1
   && (c->vert && strcmp(c->vert->name.c_str(), vshader) == 0)
   && !c->frag ) return c;
  c = (ShaderProgram *) c->next;
 }

 // If not, lookup the vertex shader and link it into a new program.
 if ( strlen(vshader) && !(vertex_shader = shaders->find(options, vshader, Vert, 1, "", "")) ) {
  OUTPUT("ShaderProgram:find: Failed to load vertex shader \"%s\".\n", vshader);
  return NULL;
 }

 n=new ShaderProgram;
 n->Link(options, vertex_shader, NULL,1);
 Append(n);
 return n;
}

ShaderProgram *ShaderPrograms::findf(Shaders *shaders, Strings *options, const char *fshader)
{
 ShaderProgram *c, *n;
 Shader *fragment_shader = NULL;

 c=(ShaderProgram *) first;
 while (c != NULL) {
  if ( !c->options.NotMatchesKeyValues(options) 
   && c->lights == 1
   && (c->frag && strcmp(c->frag->name.c_str(), fshader) == 0)
   && !c->vert ) return c;
  c = (ShaderProgram *) c->next;
 }

 if (strlen(fshader) && !(fragment_shader = shaders->find(options, fshader, Frag, 1, "", "")) ) {
  OUTPUT("ShaderProgram:find: Failed to load fragment shader \"%s\".\n", fshader);
  return NULL;
 }

 n=new ShaderProgram;
 n->Link(options, NULL, fragment_shader,1);
 Append(n);
 return n;
}

ShaderProgram *ShaderPrograms::Add(Strings *options, Shader *vshader, Shader *fshader)
{
 ShaderProgram *n;
 if ( vshader == NULL && fshader == NULL ) {
  OUTPUT("ShaderPrograms:Add both shaders provided were NULL. Ignoring this call and returning NULL.\n" );
  return NULL;
 }
 n=new ShaderProgram;
 n->Link(options, vshader, fshader,1);
 Append(n);
 return n;
}


ShaderProgram *ShaderPrograms::Addv(Strings *options, Shader *vshader )
{
 ShaderProgram *n;
 if ( vshader == NULL ) {
  OUTPUT("ShaderPrograms:Addv vshader provided was NULL. Ignoring this call and returning NULL.\n" );
  return NULL;
 }
 n=new ShaderProgram;
 n->Link(options, vshader, NULL,1);
 Append(n);
 return n;
}


ShaderProgram *ShaderPrograms::Addf(Strings *options, Shader *fshader)
{
 ShaderProgram *n;
 if (  fshader == NULL ) {
  OUTPUT("ShaderPrograms:Addf fshader provided was NULL. Ignoring this call and returning NULL.\n" );
  return NULL;
 }
 n=new ShaderProgram;
 n->Link(options, NULL, fshader,1);
 Append(n);
 return n;
}

ShaderLibrary shaders;