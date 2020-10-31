
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "LoadProgram.h"
#include "Matrix4x4.h"
#include "Art.h"
#include "Scene.h"

#include "WorldShader.h"

#if defined(NEVER)
// Deferred Shading ----------------------------------------------------------------------------

void Scene::DeferredShadingInit() {
  
 //Camera details - needed for scissor rectangle etc calculation
 camera->nearDistance=1.0f;
 camera->fovY=45.0f;
 camera->aspectRatio=(float) w/(float) h;	//set once window size is known

	//Load the vertex and fragment programs
	glGenProgramsARB(1, &standardShadingVP);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, standardShadingVP);
	if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, "data/shaders/StandardShading.vert"))		return;

	glGenProgramsARB(1, &standardShadingFP);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, standardShadingFP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_ARB, "data/shaders/StandardShading.frag"))		return;

	glGenProgramsARB(1, &deferredShadingPass1VP);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, deferredShadingPass1VP);
	if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, "data/shaders/DeferredShadingPass1.vert"))		return;

	glGenProgramsNV(1, &deferredShadingPass1FP);
	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass1FP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_NV, "data/shaders/DeferredShadingPass1.frag"))		return;

	glGenProgramsNV(1, &deferredShadingPass2FP);
	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass2FP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_NV, "data/shaders/DeferredShadingPass2.frag"))		return;
	
	//Create the pBuffer
	const int pbufferAttribIList[]={
  WGL_DRAW_TO_PBUFFER_ARB,	true,
		WGL_RED_BITS_ARB,		     	32,
		WGL_GREEN_BITS_ARB,   			32,
		WGL_BLUE_BITS_ARB,		    	32,
		WGL_ALPHA_BITS_ARB,		   	32,
		WGL_FLOAT_COMPONENTS_NV,	true,
		WGL_DEPTH_BITS_ARB,		   	24,
		WGL_STENCIL_BITS_ARB,	  	8,
		0
 };
	const float pbufferAttribFList[]={0};
	const int pbufferFlags[]={0};
	
	if(!pbuffer.Init(	w, h,	pbufferAttribIList, pbufferAttribFList, pbufferFlags))
	{
		OUTPUT("Scene:DeferredShadingInit was unable to create pbuffer");
		return;
	}

	//Share textures & programs between window and pbuffer contexts
	if(!wglShareLists(display->hglrc, pbuffer.hRC))
	{
		OUTPUT("Scene:DeferredShadingInit was unable to share lists between window and pbuffer contexts");
		return;
	}

	//Initialise the pbuffer context
	pbuffer.MakeCurrent();
	{
		glViewport(0, 0, pbuffer.width, pbuffer.height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(camera->fovY, camera->aspectRatio, camera->nearDistance, 100.0f);

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

		//Pbuffer context also needs vertex arrays for torus knot
//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, torusKnotVBO);
//		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, torusKnotIBO);
	
//		glVertexPointer(3, GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL);
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glNormalPointer(GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL + 12);
//		glEnableClientState(GL_NORMAL_ARRAY);
	}
	display->MakeCurrent();

	//Initialise the pbuffer texture
	glGenTextures(1, &pbufferTexture);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);
	glTexImage2D(	GL_TEXTURE_RECTANGLE_NV, 0, GL_FLOAT_RGBA32_NV, w,	h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Scene::DeferredShadingRender() {

	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	gluLookAt(	0.0f, 4.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Save the view matrix
	MATRIX4X4 viewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);	

	//Pass 1
	//Draw the geometry, saving parameters into the buffer

	//Make the pbuffer the current context
	pbuffer.MakeCurrent();

	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glLoadIdentity();
	gluLookAt(	0.0f, 4.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Bind and enable vertex & fragment programs
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, deferredShadingPass1VP);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass1FP);
	glEnable(GL_FRAGMENT_PROGRAM_NV);

	//Draw the torus knot
//		glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

	//Draw the "floor"
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f( 5.0f,-0.5f, 5.0f);
		glVertex3f( 5.0f,-0.5f,-5.0f);
		glVertex3f(-5.0f,-0.5f, 5.0f);
		glVertex3f(-5.0f,-0.5f,-5.0f);
	}
	glEnd();
	
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_NV);

	//Copy the pbuffer contents into the pbuffer texture
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);
	glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0,	pbuffer.width, pbuffer.height);

	//Make the window the current context
	display->MakeCurrent();

	//Pass 2
	//Draw a quad covering the region of influence of each light
	//Unpack the data from the buffer, perform the lighting equation and update
	//the framebuffer
	
	//Set orthographic projection, 1 unit=1 pixel
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(x, w, y, h);
	
	//Set identity modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Disable depth test
	glDisable(GL_DEPTH_TEST);

	//Bind the pbuffer texture
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);

	//Bind and enable fragment program
	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass2FP);
	glEnable(GL_FRAGMENT_PROGRAM_NV);

	//Loop through the lights
 int i=0;
	for( Light *L=(Light *) (lights.first); L; L=(Light *) L->next )	{
		//Calculate the rectangle to draw for this light
		int rectX, rectY, rectWidth, rectHeight;
  L->GetWindowRect(w, h,	viewMatrix, camera, rectX, rectY, rectWidth, rectHeight);

		//Enable additive blend if i>0
		if(i>0) Blending(L->blend);
		else i++;

		//Send the light's color to fragment program local parameter 0
		glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_NV, 0, L->color.floats);

		//Send 1/(light radius)^2 to fragment program local parameter 1
		float inverseSquareLightRadius=1.0f/(L->radius*L->radius);
		glProgramLocalParameter4fARB(	GL_FRAGMENT_PROGRAM_NV, 1,
										inverseSquareLightRadius, inverseSquareLightRadius,
										inverseSquareLightRadius, inverseSquareLightRadius);

		//Send the light's position to fragment program local parameter 2
		glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_NV, 2,	VECTOR4D(L->GetPosition()) );

  //Draw the rectangle
		glBegin(GL_TRIANGLE_STRIP);
		{
			glVertex2i(rectX,			rectY);
			glVertex2i(rectX+rectWidth,	rectY);
			glVertex2i(rectX,			rectY+rectHeight);
			glVertex2i(rectX+rectWidth,	rectY+rectHeight);
		}
		glEnd();
	}

	//Restore matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_FRAGMENT_PROGRAM_NV);
	glDisable(GL_BLEND);
}

// Standard Shading --------------------------------------------------------------------------

void Scene::StandardShadingInit() {
  DeferredShadingInit();
}

void Scene::StandardShadingRender() {
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	gluLookAt(	0.0f, 4.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Save the view matrix
	MATRIX4X4 viewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);	

	//Make an initial pass to lay down Z
	glColorMask(0, 0, 0, 0);

	//Draw the torus knot
//	glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

	//Draw the "floor"
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f( 5.0f,-0.5f, 5.0f);
		glVertex3f( 5.0f,-0.5f,-5.0f);
		glVertex3f(-5.0f,-0.5f, 5.0f);
		glVertex3f(-5.0f,-0.5f,-5.0f);
	}
	glEnd();

	glColorMask(1, 1, 1, 1);

	//Bind and enable vertex & fragment programs
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, standardShadingVP);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, standardShadingFP);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	//Loop through the lights
 int i=0;
	for(Light *L=(Light *) (lights.first); L; L=(Light *) (L->next) ) {
	{
		//Calculate and set the scissor rectangle for this light
		int scissorX, scissorY, scissorWidth, scissorHeight;

		L->GetWindowRect(w,h,	viewMatrix, camera, scissorX, scissorY, scissorWidth, scissorHeight);

		glScissor(scissorX, scissorY, scissorWidth, scissorHeight);
		glEnable(GL_SCISSOR_TEST);

		//Enable additive blend if i>0
		if(i>0)
		{
   Blending(L->blend);
		}

		//Calculate the object space light position and send to
		//vertex program local parameter 0
		//Object space and world space are the same
		glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 0,
										VECTOR4D(L->GetPosition()));

		//Send the light's color to fragment program local parameter 0
		glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_ARB, 0, L->color.floats);

		//Send 1/(light radius)^2 to fragment program local parameter 1
		float inverseSquareLightRadius=1.0f/(L->radius*L->radius);
		glProgramLocalParameter4fARB(	GL_FRAGMENT_PROGRAM_ARB, 1,
										inverseSquareLightRadius, inverseSquareLightRadius,
										inverseSquareLightRadius, inverseSquareLightRadius);

  }

		//Draw the torus knot
//		glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

		//Draw the "floor"
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		{
			glVertex3f( 5.0f,-0.5f, 5.0f);
			glVertex3f( 5.0f,-0.5f,-5.0f);
			glVertex3f(-5.0f,-0.5f, 5.0f);
			glVertex3f(-5.0f,-0.5f,-5.0f);
		}
		glEnd();
	}

	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
}


#endif


void Scene::DefaultRender() {

  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);

	 glViewport(x, y, w, h);
	 glClearColor(background.rf, background.gf, background.bf, background.af);
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
  glEnable(GL_MULTISAMPLE_ARB);
  

 	//Set fog 
 	glFogi(GL_FOG_MODE, GL_LINEAR);		// Fog Mode
 	glFogfv(GL_FOG_COLOR,  fogColor);			// Set Fog Color
 	glFogf(GL_FOG_DENSITY, fogDensity);				// How Dense Will The Fog Be
 	glHint(GL_FOG_HINT,    fogHint);			// Fog Hint Value
 	glFogf(GL_FOG_START,   fogStart);				// Fog Start Depth
 	glFogf(GL_FOG_END,     fogEnd);				// Fog End Depth
 	glEnable(GL_FOG);
 	//End fog

  Perspective(rad2deg((float)camera->fov), (float)(w) / (float)(h), 0.1f, 20000.0f);
 	camera->LookAt(Vector3(camera->cPosition.x, camera->cPosition.y, camera->cPosition.z),
                 target, up);

  glEnable(GL_DEPTH_TEST);
//  glDisable(GL_CULL_FACE);
	 glEnable(GL_CULL_FACE);

  if ( drawLights ) {
   glPushMatrix();
   glEnable(GL_BLEND);
   for ( Light *L=(Light *) (lights.first); L; L=(Light *) L->next ) {
    color->Luminance = 1.0f;
    color->LuminanceTint.fromCrayon( L->color );
    //sphereVBO->scale.Set( (L->radius), (L->radius), (L->radius) );
    sphereVBO->position.Set( L->position.x, L->position.y, L->position.z );
    color->UpdateUniforms();
 //   sphere.Render( &zero, &q, camera, this );
   }   
   glDisable(GL_BLEND);
   glPopMatrix();
  }

//  for ( MapObject *o=(MapObject *) (objects.first); o; o=(MapObject *) o->next ) {
   // Render the base
   //if ( o->schematic->base ) o->schematic->base->matters.Render( &(o->transform.position), &(o->rotation), camera, this);
   // Walk through the components
   //EACH(o->schematic->components.first,Plug,p)
   // p->component->matters.Render( &(o->transform.position), &(o->rotation), camera, this);
 // }

  if ( showAxis ) {
   // Draw an X,Y,Z axis

   glBegin(GL_LINES); //X
   glColor3f(1.0f,0.0f,0.0f);
   glVertex3f(0.0f,0.0f,0.0f);
   glVertex3f(100.0f,0.0f,0.0f);
   glEnd();
   
   glBegin(GL_LINES); //Y
   glColor3f(0.0f,1.0f,0.0f);
   glVertex3f(0.0f,0.0f,0.0f);
   glVertex3f(0.0f,100.0f,0.0f);
   glEnd();
   
   glBegin(GL_LINES); //Z
   glColor3f(0.0f,0.0f,1.0f);
   glVertex3f(0.0f,0.0f,0.0f);
   glVertex3f(0.0f,0.0f,100.0f);
   glEnd();

  }

  glReportError( glGetError() );
  glPopAttrib();
  glPopMatrix();

  if ( showCoords ) {
   static char buf[64];
   FORMAT( buf,64, "%1.5f %1.5f %1.5f", 
    (float) camera->cPosition.x, (float) camera->cPosition.y, (float) camera->cPosition.z );
   WriteTexFont( this, microFont, crayons.Pick(alabaster), additive, 5, 5, 10, 10, 2, buf );
   FORMAT( buf,64, "%1.5f %1.5f %1.5f",
    (float) target.x, (float) target.y, (float) target.z );
   WriteTexFont( this, microFont, crayons.Pick(alabaster), additive, 18, 18, 10, 10, 2, buf );
  }
  
  ((Material *)materials.first)->ShaderDebugger(this);

}