#include "GLSetup.h"
#include "GLWindow.h"
#include "GLSL2Shader.h"

extern GLSetup gl;

GLSLShader::GLSLShader() {
	vShader		= 0;
	fShader		= 0;
    gShader     = 0;
    tcShader    = 0;
    teShader    = 0;
	program		= 0;
	location0	= 0;	// some variables to hold our locations if needed
	location1	= 0;
	location2	= 0;
	location3	= 0;
	location4	= 0;
	location5	= 0;
	location6	= 0;
	location7	= 0;
	location8	= 0;
	location9	= 0;
 blend=additive;
}

GLSLShader::~GLSLShader() {
 if(program) {
  if ( gShader>0 )  glDeleteObjectARB(gShader);
  if ( tcShader>0 ) glDeleteObjectARB(tcShader);
  if ( teShader>0 ) glDeleteObjectARB(teShader);
  if ( vShader>0 )  glDeleteObjectARB(vShader);
  glDeleteObjectARB(fShader);
  glDeleteObjectARB(program);
 }
}

bool GLSLShader::checkHardwareSupport() {

	//
	// check for required extensions
	//
 if( gl.hasExtension("GL_ARB_shader_objects")
  &&	gl.hasExtension("GL_ARB_shading_language_100")
  &&	gl.hasExtension("GL_ARB_vertex_shader")
  &&	gl.hasExtension("GL_ARB_fragment_shader") ) {
		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
		glGetHandleARB = (PFNGLGETHANDLEARBPROC) wglGetProcAddress("glGetHandleARB");
		glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC) wglGetProcAddress("glDetachObjectARB");
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
		glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC) wglGetProcAddress("glValidateProgramARB");
		glUniform1fARB = (PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
		glUniform2fARB = (PFNGLUNIFORM2FARBPROC) wglGetProcAddress("glUniform2fARB");
		glUniform3fARB = (PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB");
		glUniform4fARB = (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
		glUniform2iARB = (PFNGLUNIFORM2IARBPROC) wglGetProcAddress("glUniform2iARB");
		glUniform3iARB = (PFNGLUNIFORM3IARBPROC) wglGetProcAddress("glUniform3iARB");
		glUniform4iARB = (PFNGLUNIFORM4IARBPROC) wglGetProcAddress("glUniform4iARB");
		glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
		glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB");
		glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB");
		glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB");
		glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB");
		glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC) wglGetProcAddress("glUniform2ivARB");
		glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC) wglGetProcAddress("glUniform3ivARB");
		glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC) wglGetProcAddress("glUniform4ivARB");
		glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) wglGetProcAddress("glUniformMatrix2fvARB");
		glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) wglGetProcAddress("glUniformMatrix3fvARB");
		glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) wglGetProcAddress("glUniformMatrix4fvARB");
		glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) wglGetProcAddress("glGetObjectParameterfvARB");
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
		glGetAttachedObjectsARB = (PFNGLGETATTACHEDOBJECTSARBPROC) wglGetProcAddress("glGetAttachedObjectsARB");
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
		glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC) wglGetProcAddress("glGetActiveUniformARB");
		glGetUniformfvARB = (PFNGLGETUNIFORMFVARBPROC) wglGetProcAddress("glGetUniformfvARB");
		glGetUniformivARB = (PFNGLGETUNIFORMIVARBPROC) wglGetProcAddress("glGetUniformivARB");
		glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC) wglGetProcAddress("glGetShaderSourceARB");
		return true;
	}

	return false;
}


bool GLSLShader::loadShaders(const char *fragmentShader) {
 const char *vertexShader=null;

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 OUTPUT("%s",fragmentShader);
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

 glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
 else OUTPUT("No vertex shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}


bool GLSLShader::loadShaders(const char *vertexShader, const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

 glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
 else OUTPUT("No vertex shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}


bool GLSLShader::loadShaders(const char *vertexShader, const char *geometryShader, const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	gShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

    glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif
 
	if ( geometryShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&geometryShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( geometryShader!=null ) glCompileShaderARB(gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (frag)" );
 glReportError( glGetError() );
#endif


	if ( geometryShader!=null ) glAttachObjectARB(program, gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
    else OUTPUT("No vertex shader was provided.\n");
	if ( geometryShader!=null ) printInfoLog(geometryShader, gShader);
    else OUTPUT("No geometry shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}



bool GLSLShader::loadShaders(
 const char *vertexShader,
 const char *controlShader,
 const char *evalShader,
 const char *geometryShader,
 const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	gShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (tess_control)" );
 glReportError( glGetError() );
#endif
 
	tcShader = glCreateShaderObjectARB(GL_TESS_CONTROL_SHADER);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (tess_eval)" );
 glReportError( glGetError() );
#endif
 
	teShader = glCreateShaderObjectARB(GL_TESS_EVALUATION_SHADER);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

    glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif
 
	if ( geometryShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&geometryShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (geo)" );
 glReportError( glGetError() );
#endif
 
	if ( controlShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&controlShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (control)" );
 glReportError( glGetError() );
#endif
 
	if ( evalShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&evalShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (eval)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( geometryShader!=null ) glCompileShaderARB(gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (frag)" );
 glReportError( glGetError() );
#endif


	if ( geometryShader!=null ) glAttachObjectARB(program, gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( evalShader!=null ) glAttachObjectARB(program, teShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (tess_eval)" );
 glReportError( glGetError() );
#endif

	if ( controlShader!=null ) glAttachObjectARB(program, tcShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (tess_control)" );
 glReportError( glGetError() );
#endif

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
    else OUTPUT("No vertex shader was provided.\n");
	if ( geometryShader!=null ) printInfoLog(geometryShader, gShader);
    else OUTPUT("No geometry shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}


void GLSLShader::useShader() {
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::useShader (entering)" );
 glReportError( glGetError() );
#endif

	glUseProgramObjectARB(program);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::useShader:glUseProgramObjectARB (entering)" );
 glReportError( glGetError() );
#endif
}

void GLSLShader::disableShader() {
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::disableShader (entering)" );
 glReportError( glGetError() );
#endif

	glUseProgramObjectARB(0);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::disableShader:glUseProgramObjectARB(0)" );
 glReportError( glGetError() );
#endif
}

void GLSLShader::printInfoLog(const char *s, GLhandleARB obj)
{
 int infologLength = 0;
 int charsWritten  = 0;
 char *infoLog;

 OUTPUT("GLSLShader (id=%d) log:\n", (int) obj );
 glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infologLength);

 if (infologLength > 0)
 {
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		OUTPUT("%s\n",infoLog);
		free(infoLog);
 }
}

GLhandleARB GLSLShader::getProgram() {
	return program;
}




bool GLSLShader::loadShaders(Strings *in, const char *fragmentShader) {
 const char *vertexShader=null;

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 OUTPUT("%s",fragmentShader);
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

 glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

 int i=0; EACH(in->first,String,s) {
  glBindFragDataLocation(program,(GLuint)i,s->s.c_str()); i++;
  if ( i >= gl.maxDrawBuffers ) break;
 }

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
 else OUTPUT("No vertex shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}


bool GLSLShader::loadShaders(Strings *in, const char *vertexShader, const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

 glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB" );
 glReportError( glGetError() );
#endif

 int i=0; EACH(in->first,String,s) {
  glBindFragDataLocation(program,(GLuint)i,s->s.c_str()); i++;
  if ( i >= gl.maxDrawBuffers ) break;
 }

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
 else OUTPUT("No vertex shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}


bool GLSLShader::loadShaders(Strings *in, const char *vertexShader, const char *geometryShader, const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	gShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

    glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif
 
	if ( geometryShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&geometryShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( geometryShader!=null ) glCompileShaderARB(gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (frag)" );
 glReportError( glGetError() );
#endif


	if ( geometryShader!=null ) glAttachObjectARB(program, gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (geo)" );
 glReportError( glGetError() );
#endif

 int i=0; EACH(in->first,String,s) {
  glBindFragDataLocation(program,(GLuint)i,s->s.c_str()); i++;
  if ( i >= gl.maxDrawBuffers ) break;
 }


	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
    else OUTPUT("No vertex shader was provided.\n");
	if ( geometryShader!=null ) printInfoLog(geometryShader, gShader);
    else OUTPUT("No geometry shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}



bool GLSLShader::loadShaders(
 Strings *in,
 const char *vertexShader,
 const char *controlShader,
 const char *evalShader,
 const char *geometryShader,
 const char *fragmentShader) {

	if(!checkHardwareSupport()) return false;

	program = glCreateProgramObjectARB();
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateProgramObjectARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (frag)" );
 glReportError( glGetError() );
#endif

	gShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (tess_control)" );
 glReportError( glGetError() );
#endif
 
	tcShader = glCreateShaderObjectARB(GL_TESS_CONTROL_SHADER);
 
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (tess_eval)" );
 glReportError( glGetError() );
#endif
 
	teShader = glCreateShaderObjectARB(GL_TESS_EVALUATION_SHADER);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCreateShaderObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glShaderSourceARB(vShader, 1, (const GLcharARB **)&vertexShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (vert)" );
 glReportError( glGetError() );
#endif

    glShaderSourceARB(fShader, 1, (const GLcharARB **)&fragmentShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (frag)" );
 glReportError( glGetError() );
#endif
 
	if ( geometryShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&geometryShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (geo)" );
 glReportError( glGetError() );
#endif
 
	if ( controlShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&controlShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (control)" );
 glReportError( glGetError() );
#endif
 
	if ( evalShader!=null ) glShaderSourceARB(gShader, 1, (const GLcharARB **)&evalShader, NULL);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glShaderSourceARB (eval)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glCompileShaderARB(vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (vert)" );
 glReportError( glGetError() );
#endif

	glCompileShaderARB(fShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (frag)" );
 glReportError( glGetError() );
#endif

	if ( geometryShader!=null ) glCompileShaderARB(gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glCompileShaderARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) glAttachObjectARB(program, vShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (vert)" );
 glReportError( glGetError() );
#endif

	glAttachObjectARB(program, fShader);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (frag)" );
 glReportError( glGetError() );
#endif


	if ( geometryShader!=null ) glAttachObjectARB(program, gShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (geo)" );
 glReportError( glGetError() );
#endif

	if ( evalShader!=null ) glAttachObjectARB(program, teShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (tess_eval)" );
 glReportError( glGetError() );
#endif

	if ( controlShader!=null ) glAttachObjectARB(program, tcShader);

#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glAttachObjectARB (tess_control)" );
 glReportError( glGetError() );
#endif

 int i=0; EACH(in->first,String,s) {
  glBindFragDataLocation(program,(GLuint)i,s->s.c_str()); i++;
  if ( i >= gl.maxDrawBuffers ) break;
 }

	glLinkProgramARB(program);
  
#if defined(GL_DEBUG)
 GL_Assert( "GLSLShader::loadShaders:glLinkProgramARB" );
 glReportError( glGetError() );
#endif

	if ( vertexShader!=null ) printInfoLog(vertexShader, vShader);
    else OUTPUT("No vertex shader was provided.\n");
	if ( geometryShader!=null ) printInfoLog(geometryShader, gShader);
    else OUTPUT("No geometry shader was provided.\n");
	printInfoLog(fragmentShader, fShader);
	printInfoLog(null, program);

	return true;
}