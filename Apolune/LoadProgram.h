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
//////////////////////////////////////////////////////////////////////////////////////////
//	LoadProgram.h
//	Function to load a program (ARB_Vertex_program etc)
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#pragma once

#define _CRT_SECURE_NO_WARNINGS 1 

#include <iostream>
#include <string>
using namespace std;

#include "macros.h"
#include "GLSetup.h"

#include "ListItem.h"

class SimpleShader : public ListItem {
public:
 string parsed, LightLoop, Equation;
 GLuint handle;
 GLuint vert1, frag1;
 bool LoadProgram(GLenum target, const char * filename, int num_lights, const char *equation, const char *lightloop );

 //Load the vertex and fragment programs
 bool Load( const char *prefix, int num_lights, const char *equation, const char *lightloop) {
  char fn[1024];

  if ( !( GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader ) ) {
   OUTPUT( "SimpleShader:Load GLSL not available (reported by GLEW).\n" );
   return false;
  }

  FORMAT( fn,1024, "%s.vert", prefix );
 	glGenProgramsARB(1, &vert1);
	 glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vert1);
	 if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, fn, num_lights, equation, lightloop))	{
   OUTPUT( "SimpleShader:Load error loading vertex program: %s\n", fn );
   return false;
  }

  FORMAT( fn,1024, "%s.frag", prefix );
	 glGenProgramsARB(1, &frag1);
 	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, frag1);
 	if(!LoadProgram(GL_FRAGMENT_PROGRAM_ARB, fn, num_lights, equation, lightloop))		{
   OUTPUT( "SimpleShader:Load error loading fragment program: %s\n", fn );
   return false;
  }


 }
};