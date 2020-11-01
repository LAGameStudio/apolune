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

#if defined(USE_PBUFFER)

#include "GLSetup.h"
#include "GLWindow.h"
#include "Camera.h"

class pBuffer {
public:
 Display *display;
	HGLRC hRC;						//rendering context
	HDC hDC;						//device context
	HPBUFFERARB hBuffer;			//buffer handle
	int width, height;				//pBuffer size
 bool shutdown;
 GLuint pBufferTexture;
 pBuffer() {
  display=null;
  hRC=NULL;
  hDC=NULL;
  hBuffer=0;
  width=height=0;
  shutdown=false;
  pBufferTexture=0;
 }
	~pBuffer() {  
  if ( pBufferTexture != 0 ) {
   glDeleteTextures(1,&pBufferTexture);
  }
  if ( shutdown ) Shutdown(); 
 }
 void Create( Display *d, int w, int h, Crayon bg ) {
  display=d;
  	//Create the pBuffer
  const int pbufferAttribIList[]={
   WGL_DRAW_TO_PBUFFER_ARB,	true,
   WGL_RED_BITS_ARB,        32,
   WGL_GREEN_BITS_ARB,      32,
   WGL_BLUE_BITS_ARB,       32,
   WGL_ALPHA_BITS_ARB,      32,
   WGL_FLOAT_COMPONENTS_NV,	true,
   WGL_DEPTH_BITS_ARB,      24,
   WGL_STENCIL_BITS_ARB,    8,
   0
  };
  const float pbufferAttribFList[]={0};
  const int pbufferFlags[]={0};
 	
  if(!Init(	w, h,	pbufferAttribIList, pbufferAttribFList, pbufferFlags)) {
   OUTPUT("pBuffer:CreateDefault was unable to create pbuffer");
   return;
  }
   
  MakeCurrent();
  
  GL_Assert( "pBuffer::Create() Pre" );	// initialize all your openGL stuff the same as your main window context here, example:
	glEnable(GL_TEXTURE_2D);		      // Enable Texture Mapping
	glEnable(GL_BLEND);		
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // enable transparency
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glOrtho(0, w, h, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);

  display->MakeCurrent();
  GL_Assert( "pBuffer::Create() Post" );
  glReportError( glGetError() );

  InitTexture(w,h);
 }

 virtual void CreateDefault( Display *d, int w, int h, gCamera *camera, Crayon background ) {
  display=d;

 	//Create the pBuffer
 	const int pbufferAttribIList[]={
        WGL_DRAW_TO_PBUFFER_ARB,    true,
 		WGL_RED_BITS_ARB,		    32,
 		WGL_GREEN_BITS_ARB,   		32,
 		WGL_BLUE_BITS_ARB,		    32,
 		WGL_ALPHA_BITS_ARB,		   	32,
 		WGL_FLOAT_COMPONENTS_NV,	true,
 		WGL_DEPTH_BITS_ARB,		   	24,
 		WGL_STENCIL_BITS_ARB,	  	8,
 		0
  };
	 const float pbufferAttribFList[]={0};
	 const int pbufferFlags[]={0};
	
	 if(!Init(	w, h,	pbufferAttribIList, pbufferAttribFList, pbufferFlags))
	 {
		 OUTPUT("pBuffer:CreateDefault was unable to create pbuffer");
	 	return;
 	}
  
 	MakeCurrent();
  {
 		glViewport(0, 0, width, height);

 		glMatrixMode(GL_PROJECTION);
	 	glLoadIdentity();
   if ( camera ) {
 	 	gluPerspective(camera->fovY, camera->aspectRatio, camera->nearDistance, 200.0f);
  		glMatrixMode(GL_MODELVIEW);
  		glLoadIdentity();
    glShadeModel(GL_SMOOTH);
  		glClearColor(background.rf, background.gf, background.bf, background.af);
  		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  		glClearDepth(1.0f);
  		glDepthFunc(GL_LEQUAL);
  		glEnable(GL_DEPTH_TEST);
  		glEnable(GL_CULL_FACE);

   } else {
//    glOrtho(0,display->w,display->h,0,0.0,1.0);
   }

  }
  display->MakeCurrent();
  glReportError( glGetError() );
  InitTexture(w,h);
 }

 void InitTexture( int w, int h ) {
	 //Initialise the pbuffer texture, called after Init()
 	glGenTextures(1, &pBufferTexture);
 	glBindTexture(GL_TEXTURE_RECTANGLE_NV, pBufferTexture);
 	glTexImage2D(	GL_TEXTURE_RECTANGLE_NV, 0, GL_FLOAT_RGBA32_NV, w,	h, 0, GL_RGBA, GL_FLOAT, NULL);
 	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glReportError( glGetError() );
 }

 bool Init( int newWidth, int newHeight, const int * attribIList, const float * attribFList, const int * flags) {
  if( !gl.deferredShading ) {
   OUTPUT("pBuffer:Init() needs extensions for pBuffer which are unsupported.\n");
   return false;
  } 

  width=newWidth;
  height=newHeight;
 
  //Get the current device context
  HDC hCurrentDC=wglGetCurrentDC();
  if(!hCurrentDC)	{
  	OUTPUT("pBuffer:Init() unable to get current device context.\n");
  	return false;
  }
 
  //choose pixel format
  GLint pixelFormat;
  unsigned int numFormats;
 
  if(!wglChoosePixelFormatARB(hCurrentDC, attribIList, attribFList, 1,	&pixelFormat, &numFormats))	{
 	OUTPUT("pBuffer:Init() unable to find a pixel format for the pBuffer.\n");
 	return false;
  }
 
  //Create the pbuffer
  hBuffer=wglCreatePbufferARB(hCurrentDC, pixelFormat, width, height, flags);
  if(!hBuffer)	{
 	OUTPUT("pBuffer:Init() is unable to create pBuffer.\n");
 	return false;
  }
 
  //Get the pbuffer's device context
  hDC=wglGetPbufferDCARB(hBuffer);
  if(!hDC)	{
 	OUTPUT("pBuffer:Init() is unable to get pBuffer's device context.\n");
 	return false;
  }
 
  //Create a rendering context for the pbuffer
  hRC=wglCreateContext(hDC);
  if(!hRC)	{
 	OUTPUT("pBuffer:Init() Unable to create pBuffer's rendering context.\n");
 	return false;
  }
 
  //Set and output the actual pBuffer dimensions
  wglQueryPbufferARB(hBuffer, WGL_PBUFFER_WIDTH_ARB, &width);
  wglQueryPbufferARB(hBuffer, WGL_PBUFFER_HEIGHT_ARB, &height);
  OUTPUT("pBuffer:Init() pBuffer Created: (%d x %d)\n",
   (int) width, (int) height);
  shutdown=true;
  return shutdown;
 }

 void Shutdown(void) {
  if(hRC) {
   if(!wglDeleteContext(hRC))
    OUTPUT("pBuffer:Shutdown release of pBuffer rendering context failed.\n");
   hRC=NULL;
  }
  if(hDC && !wglReleasePbufferDCARB(hBuffer, hDC)) {
   OUTPUT("pBuffer:Shutdown release of pBuffer device context failed.\n");
   hDC=NULL;
  }	
  if(!wglDestroyPbufferARB(hBuffer)) {
   OUTPUT("pBuffer:Shutdown unable to destroy pBuffer.\n");
  }
  shutdown=false;
 }

 void Render( Blends blend, Crayon tint, int x, int y, int x2, int y2 ) {
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV, pBufferTexture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,0.0f);     glVertex2i(x,y);
  glTexCoord2f(1.0f,0.0f);     glVertex2i(x2,y);
  glTexCoord2f(1.0f,1.0f);     glVertex2i(x2,y2);
  glTexCoord2f(0.0f,1.0f);     glVertex2i(x,y2);
  glEnd();
   
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }

 void Renderf( Blends blend, Crayon tint, float x, float y, float x2, float y2 ) {
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_RECTANGLE_NV, pBufferTexture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,0.0f);     glVertex2f(x,y);
  glTexCoord2f(1.0f,0.0f);     glVertex2f(x2,y);
  glTexCoord2f(1.0f,1.0f);     glVertex2f(x2,y2);
  glTexCoord2f(0.0f,1.0f);     glVertex2f(x,y2);
  glEnd();
   
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }

 bool MakeCurrent(void) {
  if(!wglMakeCurrent(hDC, hRC)) {
   OUTPUT("pBuffer:MakeCurrent is unable to change current context.\n");
   return false;
  } 
  return true;
 }

};

#endif