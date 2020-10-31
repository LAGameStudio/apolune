
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "GLSetup.h"
#include "Shader.h"

#include "Material.h"

#include "Camera.h"
#include "Scene.h"
#include "OldVBO.h"

extern GLSetup gl;

Materials materials;

void Material::DetachShader() { if ( program ) delete program; program=null; glReportError( glGetError() ); }

void Material::AssignShader( const char *prefix ) {
 DetachShader();
 name=prefix;
 vertex=prefix+string(".vert");
 fragment=prefix+string(".frag");
 geometry=prefix+string(".geo");
 tesselation=prefix+string(".tess");
 // Load shader.
 if ( gl.hasShaders && (vertex[0] || fragment[0]) ) {
  program = shaders.find(&options,(char *)vertex.c_str(),(char *)fragment.c_str());
  if (!program) OUTPUT("Materal:AssignShader() failed to load shader for \"%s\".", name.c_str());
  lights=1;
 }
 InitUniforms();
}

void Material::AssignShader( const char *prefix, int num_lights ) {
 DetachShader();
 name=prefix;
 vertex=prefix+string(".vert");
 fragment=prefix+string(".frag");
 geometry=prefix+string(".geo");
 tesselation=prefix+string(".tess");
 if ( gl.hasShaders && (vertex[0] || fragment[0]) ) {
  program = shaders.find(&options,(char *)vertex.c_str(),(char *)fragment.c_str(),num_lights);
  if (!program) OUTPUT("Materal:AssignShader() failed to load shader for \"%s\".", name.c_str());
  lights=num_lights;
 } 
 InitUniforms();
}

void Material::AssignShader( const char *prefix, int num_lights, const char *equation, const char *lightloop ) {
 DetachShader();
 name=prefix;
 vertex=prefix+string(".vert");
 fragment=prefix+string(".frag");
 geometry=prefix+string(".geo");
 tesselation=prefix+string(".tess");
 if ( gl.hasShaders && (vertex[0] || fragment[0]) ) {
  program = shaders.find(&options,vertex.c_str(),fragment.c_str(),num_lights,equation,lightloop);
  if (!program) OUTPUT("Materal:AssignShader() failed to load shader for \"%s\".", name.c_str());
  eq=string(equation);
  ll=string(lightloop);
  lights=num_lights;
 }
 InitUniforms();
}

void Material::AssignShader( const char *v, const char *f ) {
 DetachShader();
 vertex=string(v)+string(".vert");
 fragment=string(f)+string(".frag");
 if ( gl.hasShaders && (vertex[0] || fragment[0]) ) {
  program = shaders.find(&options,(char *)vertex.c_str(),(char *)fragment.c_str(),1);
  if (!program) OUTPUT("Materal:AssignShader() failed to load shader for \"%s\".", name.c_str());
  lights=1;
 }
 glReportError( glGetError() );
 InitUniforms();
}

void Material::Apply( float dt, float rotationDegrees, Vertex *offset, Vertex *position, Vertex *scale, Vertex *rotation, Vector3 rotationAxis, Quaternionf * q, VBO *vbo, gCamera *camera, Scene *scene ) {
 glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_color.floats);
 glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_color.floats);
 glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_color.floats);
 glMaterialfv(GL_FRONT, GL_EMISSION,  emission_color.floats);
 glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
 Blending(blend);
 glGetError();
}