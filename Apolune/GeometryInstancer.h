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

#include "GLSetup.h"
#include "GLSL2Shader.h"

// Implements approaches to http://www.opengl.org/registry/specs/EXT/draw_instanced.txt
// and http://www.opengl.org/registry/specs/ARB/instanced_arrays.txt
// reviews to http://sol.gfxile.net/instancing.html

class GeometryInstancer
{
public:
 GLuint matrices;
 GeometryInstancer(void) {}
 void Init() {
  // Create the matrices texture
  /*
int pos = glGetAttribLocation(shader_instancedarrays.program, "transformmatrix");
int pos1 = pos + 0; 
int pos2 = pos + 1; 
int pos3 = pos + 2; 
int pos4 = pos + 3; 
glEnableVertexAttribArray(pos1);
glEnableVertexAttribArray(pos2);
glEnableVertexAttribArray(pos3);
glEnableVertexAttribArray(pos4);
glBindBuffer(GL_ARRAY_BUFFER, VBO_containing_matrices);
glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
glVertexAttribDivisor(pos1, 1);
glVertexAttribDivisor(pos2, 1);
glVertexAttribDivisor(pos3, 1);
glVertexAttribDivisor(pos4, 1);*/
 }
 ~GeometryInstancer(void) {
 }
};

