//////////////////////////////////////////////////////////////////////////////////////////
//	LoadProgram.cpp
//	Function to load a program (ARB_Vertex_program etc)
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <fstream>

#include "macros.h"
#include "GLSetup.h"
#include "Strings.h"
#include "LoadProgram.h"

bool SimpleShader::LoadProgram(GLenum target, const char * filename, int num_lights, const char *equation, const char *lightloop)
{
 //parse
 Strings *src=new Strings(filename);
 int eqLen = (int) strlen(equation);
 int llLen = (int) strlen(lightloop);
 parsed=string("");
 int i = 0;
 Equation=string(equation);
 LightLoop=string(lightloop);

 for ( String *sL=(String *) src->first; sL; sL=(String *) sL->next ) {
  if ( !str_infix( "#define NUM_LIGHTS", (char *) sL->s.c_str() ) ) {
   char buf[32];
   FORMAT(buf,32,"#define NUM_LIGHTS %d\n", (int) num_lights);
   parsed += string(buf);
  } else if ( !str_infix( "//loop//", (char *) sL->s.c_str() ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//loop//" );
   for ( i=0; i<num_lights; i++ ) {
    string t=s;
    int position=(int) t.find( "_n" );
    while ( position != string::npos ) 
    {
      char num[20];
      FORMAT( num,20, "_%d", (int) i );
      t.replace( position, 2, string(num) );
      position =(int) t.find( "_n", position + 2 );
    } 
    parsed += t + "\n";
   }
   sL=src->findNextSubstring( (String *) sL->next, "//loop//" );
  } else if ( eqLen > 0 && !str_infix( "//equation//", (char *) sL->s.c_str() ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//equation//" );
   parsed += Equation+"\n";
   sL=src->findNextSubstring( (String *) sL->next, "//equation//" );
  } else if ( llLen > 0 && !str_infix( "//lightloop//", (char *) sL->s.c_str() ) ) {
   string s=src->concatToNextSubstring( (String *) sL->next, "//lightloop//" );
   parsed += LightLoop+"\n";
   sL=src->findNextSubstring( (String *) sL->next, "//lightloop//" );
  } else {
   parsed += sL->s + "\n";
  }
 }

	//Send program string to OpenGL
	glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei) parsed.length(), (const void *) parsed.c_str() );//programText);
	
	//Output position of any error
	int programErrorPos;
	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &programErrorPos);
	if(programErrorPos!=-1) OUTPUT("Program error at position %d in %s",	(int) programErrorPos, filename);

	//Output error/warning messages if any
	const GLubyte * programErrorString=glGetString(GL_PROGRAM_ERROR_STRING_ARB);
	if(strlen((const char *)programErrorString)>0)
	{
		OUTPUT("Program Error String for %s:\n%s", filename, programErrorString);
	}

	//Is the program under native limits? (Not supported by NV_fragment_program)
	if(target!=GL_FRAGMENT_PROGRAM_NV)
	{
		GLint underNativeLimits;
		glGetProgramivARB(target, GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB, &underNativeLimits);
		if(underNativeLimits==0)
			OUTPUT("LoadProgram: %s exceeds native limits\n", filename);
	}

	//Return false in case of error
	if(programErrorPos!=-1)	return false;

	return true;
}
