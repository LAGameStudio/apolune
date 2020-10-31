
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
#include "GLImage.h"

extern GLSetup gl;

void GLImage::Init(void) {
 image=null;
 pixels=null;
// mipmap=null;
 width=height=0;
 filename=string("");
 integer=-6666666;
 floating=-6666666.6666666f; 
 uploaded=false;
 deleteAfterUpload=true;
}

GLImage::GLImage(void)
{
 Init();
}

GLImage::GLImage( CPUImage *in ) {
 Init();
 image=in;
 width=image->getWidth();
 height=image->getHeight();
}

GLImage::GLImage( const char *name, CPUImage *in ) {
 Init();
 image=in;
 width=image->getWidth();
 height=image->getHeight();
 filename=string(name);
}

GLImage::GLImage( const char *name, int w, int h ) {
 Init();
#if defined(USE_CORONA)
 try {
  image=corona::CreateImage(w,h,corona::PF_R8G8B8A8);
 } catch (bad_alloc& ba) {
  OUTPUT("GLImage::GLImage / coronoa::CreateImage: bad_alloc caught: %s\n", ba.what());
 }
 width=image->getWidth();
 height=image->getHeight();
 pixels=image->getPixels();
#else
#pragma warning( push )
#pragma warning( disable : 4101 )
 NEW(image,CPUImage(w,h));
#pragma warning( pop )
 if ( ( pixels=image->getPixels() ) == null ) {
  OUTPUT("Image was blank (null pixels)\n");
 }
 width=image->getWidth();
 height=image->getHeight();
#endif
 filename=string(name);
}

GLImage::GLImage( float *rgb_buffer, int w, int h, const char *name, GLint mag, GLint min, GLint wrapS, GLint wrapT, bool mipmaps ) {
 Init();
 width=w;
 height=h;
 filename=name;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
 // Mipmaps
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 if ( mipmaps ) {
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
  if ( gl.EXT_FBO ) { // source http://www.opengl.org/wiki/Common_Mistakes#Creating_a_Texture
   glEnable(GL_TEXTURE_2D); // ATI bug
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, rgb_buffer);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glDisable(GL_TEXTURE_2D);
  } else {
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, rgb_buffer);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
  }
 } else {
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
 }
 uploaded=true;
}
GLImage::GLImage( having alpha, float *rgba_buffer, int w, int h, const char *name, GLint mag, GLint min, GLint wrapS, GLint wrapT, bool mipmaps ) {
 Init();
 width=w;
 height=h;
 filename=name;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
 // Mipmaps
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 if ( mipmaps ) {
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
  if ( gl.EXT_FBO ) { // source http://www.opengl.org/wiki/Common_Mistakes#Creating_a_Texture
   glEnable(GL_TEXTURE_2D); // ATI bug
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, rgba_buffer);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glDisable(GL_TEXTURE_2D);
  } else {
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, rgba_buffer);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
  }
 } else {
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
 }
 uploaded=true;
}

GLImage::GLImage( const char *filename ) {
 Init();
#if defined(DEBUG_OUTPUT)
 OUTPUT("GLImage::GLImage( `%s` )\n", filename );
#endif
 if ( !filename ) {
  OUTPUT("GLImage(filename) was called with a null filename\n");
  return;
 }
 if ( strlen(filename) == 0 ) {
  OUTPUT("GLImage(filename) was called with an empty filename.\n");
  return;
 }
 if ( !file_exists(filename) ) {
  OUTPUT("Image file '%s' missing\n",filename);
  return;
 }
#if defined(USE_CORONA)
 try {
  image=corona::OpenImage(filename, corona::PF_B8G8R8A8);
 } catch (bad_alloc& ba) {
  OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
 }
 if ( !image ) { OUTPUT("Image file '%s' could not be read\n",filename); return; }
 width=image->getWidth();
 height=image->getHeight();
 pixels=image->getPixels();
#else
 image=imageLoader.Load(filename);
 if ( !image ) {
  OUTPUT("Image file '%s' missing\n",filename);
  return;
 } else if ( image->error ) {
  OUTPUT("Image file '%s' was corrupt, ran out of memory, or an unsupported format.\n",filename);
  return;
 }
 if ( ( pixels=image->getPixels() ) == null ) {
  OUTPUT("Image was blank (null pixels)\n");
 }
 width=image->getWidth();
 height=image->getHeight();
#endif
 this->filename=string(filename);
}

GLImage::~GLImage(void)
{ 
 Unload();
 if ( image ) delete image;
// if ( mipmap ) delete mipmap;
}

void GLImage::Upload( GLSetup *glSetup, bool mipmaps )
{
 if ( !image ) return;
 mipmaps=mipmaps && !gl.noMipmapping;
 glGetError();
 glGenTextures(1, &texture);
 glBindTexture(GL_TEXTURE_2D, texture);
 glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Enable anisotropic texture filtering if supported.
	if ((gl.degreeOfAnisotropy > 1 || gl.degreeOfAnisotropy == BEST_DEGREE_OF_ANISOTROPY) && gl.maxDegreeOfAnisotropy > 1)
	{
		int anisotropy;
		if (gl.degreeOfAnisotropy == BEST_DEGREE_OF_ANISOTROPY)			anisotropy = gl.maxDegreeOfAnisotropy;
		else			anisotropy = min(gl.degreeOfAnisotropy, gl.maxDegreeOfAnisotropy);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
 // Mipmaps
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 if ( mipmaps ) {
  if ( glSetup->EXT_FBO ) { // source http://www.opengl.org/wiki/Common_Mistakes#Creating_a_Texture
   glEnable(GL_TEXTURE_2D); // ATI bug
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glDisable(GL_TEXTURE_2D);
  } else {
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
  }
 } else {
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
 }
 uploaded=true;
 // Delete CPU copy stored in system memory.
 if ( deleteAfterUpload ) DiscardCPUData();
}

void GLImage::Upload( GLSetup *glSetup=&gl, GLint magFilter=GL_LINEAR, GLint minFilter=GL_LINEAR_MIPMAP_LINEAR, GLint wrapS=GL_CLAMP_TO_EDGE, GLint wrapT=GL_CLAMP_TO_EDGE )
{
 if ( !image ) return;
 GL_Assert("GLImage::Upload");
 glReportError(glGetError());
 glGenTextures(1, &texture);
 glBindTexture(GL_TEXTURE_2D, texture);
 glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
 glReportError(glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
 glReportError(glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
 glReportError(glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
 glReportError(glGetError());
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
 glReportError(glGetError());
 	// Enable anisotropic texture filtering if supported.
	if ((gl.degreeOfAnisotropy > 1 || gl.degreeOfAnisotropy == BEST_DEGREE_OF_ANISOTROPY) && gl.maxDegreeOfAnisotropy > 1)
	{
		int anisotropy;
		if (gl.degreeOfAnisotropy == BEST_DEGREE_OF_ANISOTROPY)	anisotropy = gl.maxDegreeOfAnisotropy;
		else	anisotropy = min(gl.degreeOfAnisotropy, gl.maxDegreeOfAnisotropy);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
  glReportError(glGetError());
	}
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 glReportError(glGetError());
 // Mipmaps
 if ( minFilter == GL_LINEAR_MIPMAP_LINEAR && !gl.noMipmapping ) {
  if ( glSetup->EXT_FBO ) { // source http://www.opengl.org/wiki/Common_Mistakes#Creating_a_Texture
   glEnable(GL_TEXTURE_2D); // ATI bug
   glReportError(glGetError());
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   glReportError(glGetError());
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glReportError(glGetError());
   glDisable(GL_TEXTURE_2D);
  } else {
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   glReportError(glGetError());
   glGenerateMipmapEXT(GL_TEXTURE_2D);
  }
 } else {
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
 }
 uploaded=true;
 // Delete CPU copy stored in system memory.
 if ( deleteAfterUpload ) this->DiscardCPUData();
 GL_Assert("Leaving GLImage::Upload");
}

void GLImage::Unload() {
 if ( uploaded ) glDeleteTextures(1,&texture);
}