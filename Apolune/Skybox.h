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
#include "macros.h"

#include "GLSetup.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "Art.h"
#include "Vertex.h"
#include "TextureLibrary.h"


#define FRONT_ID  0
#define RIGHT_ID  1
#define BACK_ID   2
#define LEFT_ID   3
#define TOP_ID    4
#define BOTTOM_ID 5

#define GL_DEBUG 1

#if defined(NEVER) // "Prior art"
#include "DynamicCamera.h"
#include "aCamera.h"
#include "Camera.h"

class SkyboxForDummies : public ListItem {
public:
 GLImage *textures[6];
 Crayon tint;
 Blends blend;
 aCamera camera;
 gCamera gcamera;
 Vertex position,size,rotation;
 float cz,cx,r;
 SkyboxForDummies() : ListItem() {
  cz = -0.0f;
  cx = 1.0f;
  r = 1.0f; // If you have border issues change this to 1.005f
  camera.Position_Camera(
   0.0f, 2.5f, 5.0f, // Position
   0.0f, 2.5f, 0.0f, // View target
   0.0f, 1.0f, 0.0f  // Up
  );
  position.Set(0.0f,0.0f,-2.5f);
  size.Set(10.0f,10.0f,10.0f);
  rotation.Set(0.0f,90.0f,0.0f);
 }
 void Set( Crayon c, Blends b, GLImage *front, GLImage *right, GLImage *back, GLImage *left, GLImage *top, GLImage *bottom ) {
  textures[FRONT_ID]=front;
  textures[RIGHT_ID]=right;
  textures[BACK_ID]=back;
  textures[LEFT_ID]=left;
  textures[TOP_ID]=top;
  textures[BOTTOM_ID]=bottom;
  tint.fromCrayon(c);
  blend=b;
 }
/************************************************************************/
/*	Render a skybox with center point position and dimension sizes size */
/************************************************************************/
// djoubert187 _at_ hotmail.com
 void Render() {
  // Save Current Matrix
#if defined(GL_DEBUG) 
  GL_Assert( "SkyboxForDummies::Render() entering" );
  glReportError( glGetError() );
#endif
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 5, 100);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  // Begin DrawSkybox
  Blending(none);
  glColor4f(1.0, 1.0, 1.0, 1.0f);  
  
  // First apply scale matrix
  glScalef(size.x,size.y,size.z);

  glRotatef(rotation.x,1.0f,0.0f,0.0f);
  glRotatef(rotation.y,0.0f,1.0f,0.0f);
  glRotatef(rotation.z,0.0f,0.0f,1.0f);

  // Second Move the render space to the correct position (Translate)
  glTranslatef(position.x,position.y,position.z);

  // Common Axis Z - FRONT Side 
  glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]->texture);
  glBegin(GL_QUADS);	
  	glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
  	glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
  	glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r); 
  	glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
  glEnd();
  
  // Common Axis Z - BACK side
  glBindTexture(GL_TEXTURE_2D,textures[BACK_ID]->texture);
  glBegin(GL_QUADS);		
  	glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
  	glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
  	glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r); 
  	glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
  glEnd();
  
  // Common Axis X - Left side
  glBindTexture(GL_TEXTURE_2D,textures[LEFT_ID]->texture);
  glBegin(GL_QUADS);		
  	glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);	
  	glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r); 
  	glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
  	glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);		
  glEnd();
  
  // Common Axis X - Right side
  glBindTexture(GL_TEXTURE_2D,textures[RIGHT_ID]->texture);
  glBegin(GL_QUADS);		
  	glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);	
  	glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r); 
  	glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
  	glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
  glEnd();
  
  // Common Axis Y - Draw Up side
  glBindTexture(GL_TEXTURE_2D,textures[TOP_ID]->texture);
  glBegin(GL_QUADS);		
  	glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
  	glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f); 
  	glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
  	glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
  glEnd();
  
  // Common Axis Y - Down side
  glBindTexture(GL_TEXTURE_2D,textures[BOTTOM_ID]->texture);
  glBegin(GL_QUADS);		
  	glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
  	glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f); 
  	glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
  	glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
  glEnd();
#if defined(GL_DEBUG) 
  GL_Assert( "SkyboxForDummies::Render() leaving" );
  glReportError( glGetError() );
#endif
  
  // Load Saved Matrix
  glPopMatrix();
  glPopAttrib();

 }
};

class SkyboxesForDummies : public LinkedList {
public:
 void RotateBy( int x, int y ) {
  FOREACH(SkyboxForDummies,d) {
   d->rotation.x+=x;
   d->rotation.y+=y;
  }
 }
 void Render() {
  FOREACH(SkyboxForDummies,d) d->Render();
 }
 CLEARLISTRESET(SkyboxForDummies);
 ~SkyboxesForDummies() { Clear(); }
};

// works with aCamera class from Apron tutorial
class aSkybox : public ListItem {
public:
 GLImage *textures[6];
 Crayon tint;
 Blends blend;
 aCamera camera;
 gCamera gcamera;
 aSkybox() : ListItem() {
  for ( int i=0; i<6; i++ ) textures[i]=null;
  blend=none;
				      // Position      View(target)  Up
  camera.Position_Camera(0, 2.5f, 5,	0, 2.5f, 0,   0, 1, 0);
 }
 void Set( Crayon c, Blends b, GLImage *front, GLImage *right, GLImage *back, GLImage *left, GLImage *top, GLImage *bottom ) {
  textures[FRONT_ID]=front;
  textures[RIGHT_ID]=right;
  textures[BACK_ID]=back;
  textures[LEFT_ID]=left;
  textures[TOP_ID]=top;
  textures[BOTTOM_ID]=bottom;
  tint.fromCrayon(c);
  blend=b;
 }
 void Render() {
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() entered" );
  glReportError( glGetError() );
#endif
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() push" );
  glReportError( glGetError() );
#endif
  glClearDepth(1.0f); // Depth Buffer Setup
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glClearDepth(1.0f)" );
  glReportError( glGetError() );
#endif
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glEnable depth test" );
  glReportError( glGetError() );
#endif
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glDepthFunc" );
  glReportError( glGetError() );
#endif
  glEnable(GL_TEXTURE_2D);
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glEnable textures" );
  glReportError( glGetError() );
#endif

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glClear color and depth" );
  glReportError( glGetError() );
#endif
  glLoadIdentity();									// Reset The Current Modelview Matrix
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() glLoadIdentity" );
  glReportError( glGetError() );
#endif
  gcamera.LookAt( Vector3(camera.mPos.x,camera.mPos.y,camera.mPos.z),
   Vector3(camera.mView.x,camera.mView.y,camera.mView.z),
   Vector3(camera.mUp.x,camera.mUp.y,camera.mUp.z)
  );
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() gcamera.LookAt()" );
  glReportError( glGetError() );
#endif
  Draw_Skybox(0,0,0,100,100,100);	// Draw the Skybox
#if defined(GL_DEBUG) 
  GL_Assert( "aSkybox::Render() returning from Draw_Skybox" );
  glReportError( glGetError() );
#endif
  glPopAttrib();
  glPopMatrix();
 }
 void Draw_Skybox(float x, float y, float z, float width, float height, float length) {
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

    glColor4f(1.0f,1.0f,1.0f,1.0f); // white

    // Draw Front
	glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]->texture);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back
	glBindTexture(GL_TEXTURE_2D, textures[BACK_ID]->texture);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left
	glBindTexture(GL_TEXTURE_2D, textures[LEFT_ID]->texture);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right
	glBindTexture(GL_TEXTURE_2D, textures[RIGHT_ID]->texture);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up
	glBindTexture(GL_TEXTURE_2D, textures[TOP_ID]->texture);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Bottom
	glBindTexture(GL_TEXTURE_2D, textures[BOTTOM_ID]->texture);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();
 }
};

class aSkyboxes : public LinkedList {
public:
 void MoveBy( int x, int y ) {
  FOREACH(aSkybox,a) a->camera.MovedMouseTo(a->camera.oldMouse.x+x,a->camera.oldMouse.y+y); 
 }
 void MoveByParallax( int x, int y ) {
  int level=1;
  FOREACH(aSkybox,a) {
   a->camera.MovedMouseTo(a->camera.oldMouse.x+x*level,a->camera.oldMouse.y+y*level); 
   level++;
  }
 }
 void Render() {
  FOREACH(aSkybox,a) a->Render();
 }
 CLEARLISTRESET(aSkybox);
 ~aSkyboxes() { Clear(); }
};

class SimpleSkybox : public ListItem {
public:
 void Set( Crayon c, Blends b, GLImage *front, GLImage *right, GLImage *back, GLImage *left, GLImage *top, GLImage *bottom ) {
  textures[FRONT_ID]=front;
  textures[RIGHT_ID]=right;
  textures[BACK_ID]=back;
  textures[LEFT_ID]=left;
  textures[TOP_ID]=top;
  textures[BOTTOM_ID]=bottom;
  tint.fromCrayon(c);
  blend=b;
 }
 DynamicCamera camera;
 Crayon tint;
 Blends blend;
 GLImage *textures[6];
 void Between() { 
  camera.Between();
 }
 void Render(float x, float y, float z, float width, float height, float length) {  
   glPushMatrix();
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, display.wf / display.hf, 0.1, 15000.0 );
 //  camera.View();
   Blending(blend);
   glColor4fv(tint.floats);     
     
   glEnable(GL_DEPTH_TEST);  
   glEnable(GL_TEXTURE_2D);  

   // Bind the BACK texture of the sky map to the BACK side of the cube  
   glBindTexture(GL_TEXTURE_2D, textures[BACK_ID]->texture);
   
   // Center the skybox  
   x = x - width  / 2;  
   y = y - height / 2;  
   z = z - length / 2;  
   glBegin(GL_QUADS);        
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,         z);  
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);   
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height, z);  
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,         z);  
   glEnd();
   glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]->texture);
   glBegin(GL_QUADS);     
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,         z + length);  
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height, z + length);  
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);   
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,         z + length);  
   glEnd();
   glBindTexture(GL_TEXTURE_2D, textures[BOTTOM_ID]->texture);
   glBegin(GL_QUADS);        
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,         z);  
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y,         z + length);  
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,         z + length);   
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,         z);  
   glEnd();
   glBindTexture(GL_TEXTURE_2D, textures[TOP_ID]->texture);
   glBegin(GL_QUADS);        
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);  
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);   
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height,   z + length);  
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height,   z);  
   glEnd();
   glBindTexture(GL_TEXTURE_2D, textures[LEFT_ID]->texture);
   glBegin(GL_QUADS);        
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x,         y + height,   z);     
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x,         y + height,   z + length);   
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x,         y,         z + length);  
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x,         y,         z);        
   glEnd();
   glBindTexture(GL_TEXTURE_2D, textures[RIGHT_ID]->texture);
   glBegin(GL_QUADS);     
      glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,         z);  
      glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,         z + length);  
      glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,   z + length);   
      glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,   z);  
   glEnd();

   glDisable(GL_TEXTURE_2D);  
   glDisable(GL_DEPTH_TEST);
   glPopMatrix();
   glPopAttrib();
 }  
};

class SimpleSkyboxes : public LinkedList {
public:
 CLEARLISTRESET(SimpleSkybox);
 void Render( float x, float y, float z, float w, float h, float l ) {
  FOREACH(SimpleSkybox,b) { b->Render(x,y,z,w,h,l); }
 }
};
#endif

class CubicSkyboxDefinition : public ListItem {
public:
 Cartesiand rotation;
 float radii,factor;
 Crayon tint;
 Blends blend;
 GLImage *textures[6];

 CubicSkyboxDefinition() {
  radii=5.0f;
  tint.Pick(alabaster);
  blend=none;
  int i;
  for ( i=0; i<6; i++ ) textures[i]=library.find("data/images/elements/white.png");
  factor=1.0f;
 }

 void Set( Crayon c, Blends b, GLImage *front, GLImage *right, GLImage *back, GLImage *left, GLImage *top, GLImage *bottom ) {
  textures[FRONT_ID]=front;
  textures[RIGHT_ID]=right;
  textures[BACK_ID]=back;
  textures[LEFT_ID]=left;
  textures[TOP_ID]=top;
  textures[BOTTOM_ID]=bottom;
  tint.fromCrayon(c);
  blend=b;
 }
 
 void Render() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);  
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-.10,.10,-.10,.10,0.1,10000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, -1, 0, 0, 1, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
  glDisable(GL_DEPTH_TEST);
  Blending(blend);
  glColor4fv(tint.floats);
  glEnable(GL_TEXTURE_2D);
  
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);

  // Front
  glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0); glVertex3f(  radii, -radii, -radii );
  glTexCoord2d(1.0, 0.0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2d(1.0, 1.0); glVertex3f( -radii,  radii, -radii );
  glTexCoord2d(0.0, 1.0); glVertex3f(  radii,  radii, -radii );
  glEnd();
 
  // Left
  glBindTexture(GL_TEXTURE_2D, textures[LEFT_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0); glVertex3f(  radii, -radii,  radii );
  glTexCoord2d(1.0, 0.0); glVertex3f(  radii, -radii, -radii );
  glTexCoord2d(1.0, 1.0); glVertex3f(  radii,  radii, -radii );
  glTexCoord2d(0.0, 1.0); glVertex3f(  radii,  radii,  radii );
  glEnd();
 
  // Back
  glBindTexture(GL_TEXTURE_2D, textures[BACK_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0); glVertex3f( -radii, -radii,  radii );
  glTexCoord2d(1.0, 0.0); glVertex3f(  radii, -radii,  radii );
  glTexCoord2d(1.0, 1.0); glVertex3f(  radii,  radii,  radii );
  glTexCoord2d(0.0, 1.0); glVertex3f( -radii,  radii,  radii );
  glEnd();
 
  // Right
  glBindTexture(GL_TEXTURE_2D, textures[RIGHT_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2d(1.0, 0.0); glVertex3f( -radii, -radii,  radii );
  glTexCoord2d(1.0, 1.0); glVertex3f( -radii,  radii,  radii );
  glTexCoord2d(0.0, 1.0); glVertex3f( -radii,  radii, -radii );
  glEnd();
 
  // Bottom
  glBindTexture(GL_TEXTURE_2D, textures[BOTTOM_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(1.0, 0.0); glVertex3f( -radii,  radii, -radii );
  glTexCoord2d(1.0, 1.0); glVertex3f( -radii,  radii,  radii );
  glTexCoord2d(0.0, 1.0); glVertex3f(  radii,  radii,  radii );
  glTexCoord2d(0.0, 0.0); glVertex3f(  radii,  radii, -radii );
  glEnd();
 
  // Top
  glBindTexture(GL_TEXTURE_2D, textures[TOP_ID]->texture);
  glBegin(GL_QUADS);
  glTexCoord2d(1.0, 1.0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2d(1.0, 0.0); glVertex3f( -radii, -radii,  radii );
  glTexCoord2d(0.0, 0.0); glVertex3f(  radii, -radii,  radii );
  glTexCoord2d(0.0, 1.0); glVertex3f(  radii, -radii, -radii );
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopAttrib();
  glPopMatrix();
  // front  0
  // right  1
  // back   2
  // left   3
  // top    4
  // bottom 5
 }

 void draw() {
  
  // Front
  glBindTexture(GL_TEXTURE_2D, textures[0]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex3f(  radii, -radii, -radii );
  glTexCoord2i(1, 0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2i(1, 1); glVertex3f( -radii,  radii, -radii );
  glTexCoord2i(0, 1); glVertex3f(  radii,  radii, -radii );
  glEnd();
 
  // Left
  glBindTexture(GL_TEXTURE_2D, textures[3]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex3f(  radii, -radii,  radii );
  glTexCoord2i(1, 0); glVertex3f(  radii, -radii, -radii );
  glTexCoord2i(1, 1); glVertex3f(  radii,  radii, -radii );
  glTexCoord2i(0, 1); glVertex3f(  radii,  radii,  radii );
  glEnd();
 
  // Back
  glBindTexture(GL_TEXTURE_2D, textures[2]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex3f( -radii, -radii,  radii );
  glTexCoord2i(1, 0); glVertex3f(  radii, -radii,  radii );
  glTexCoord2i(1, 1); glVertex3f(  radii,  radii,  radii );
  glTexCoord2i(0, 1); glVertex3f( -radii,  radii,  radii );
  glEnd();
 
  // Right
  glBindTexture(GL_TEXTURE_2D, textures[1]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2i(1, 0); glVertex3f( -radii, -radii,  radii );
  glTexCoord2i(1, 1); glVertex3f( -radii,  radii,  radii );
  glTexCoord2i(0, 1); glVertex3f( -radii,  radii, -radii );
  glEnd();
 
  // Top
  glBindTexture(GL_TEXTURE_2D, textures[4]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1); glVertex3f( -radii,  radii, -radii );
  glTexCoord2i(0, 0); glVertex3f( -radii,  radii,  radii );
  glTexCoord2i(1, 0); glVertex3f(  radii,  radii,  radii );
  glTexCoord2i(1, 1); glVertex3f(  radii,  radii, -radii );
  glEnd();
 
  // Bottom
  glBindTexture(GL_TEXTURE_2D, textures[5]->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex3f( -radii, -radii, -radii );
  glTexCoord2i(0, 1); glVertex3f( -radii, -radii,  radii );
  glTexCoord2i(1, 1); glVertex3f(  radii, -radii,  radii );
  glTexCoord2i(1, 0); glVertex3f(  radii, -radii, -radii );
  glEnd();
 }

};

class CubicSkyboxDefinitions : public LinkedList {
public:
 float factor;
 CubicSkyboxDefinitions() { factor=1.0f; }
 // To a scalable maximum, about 6 at the time of original development 2011
 CubicSkyboxDefinition *New( Crayon c, Blends b, GLImage *front, GLImage *right, GLImage *back, GLImage *left, GLImage *top, GLImage *bottom ) {
  CubicSkyboxDefinition *n=new CubicSkyboxDefinition;
  n->textures[FRONT_ID]=front;
  n->textures[RIGHT_ID]=right;
  n->textures[BACK_ID]=back;
  n->textures[LEFT_ID]=left;
  n->textures[TOP_ID]=top;
  n->textures[BOTTOM_ID]=bottom;
  n->tint.fromCrayon(c);
  n->blend=b;
  Append(n);
  return n;
 }
 void Render() { FOREACH(CubicSkyboxDefinition,s) s->Render(); }
 void Rotate( double x, double y, double z ) {
  int i=0;
  FOREACH(CubicSkyboxDefinition,s) {
   i++;
   int idx=(count+1)-i;
   double delta=(double)idx/10.0;
   s->rotation.x+=x*delta;
   s->rotation.y+=y*delta;
   s->rotation.z+=z*delta;
  }
 }
 void Rotate( double x, double y ) {
  int i=0;
  FOREACH(CubicSkyboxDefinition,s) {
   i++;
   int idx=(count+1)-i;
   double delta=(double)idx/10.0;
   s->rotation.x+=x*delta;
   s->rotation.y+=y*delta;
  }
 }
 void RotateTo( double x, double y, double z ) {
  FOREACH(CubicSkyboxDefinition,s) {
   s->rotation.x=x;
   s->rotation.y=y;
   s->rotation.z=z;
  }
 } 
 void RotateTo( double x, double y ) {
  FOREACH(CubicSkyboxDefinition,s) {
   s->rotation.x=x;
   s->rotation.y=y;
  }
 } 
 void RotateBy( double x, double y ) {
  int i=0;
  FOREACH(CubicSkyboxDefinition,s) {
   i++;
   int idx=(count+1)-i;
   double delta=idx/10.0f;
   if ( x != 0.0f ) s->rotation.x+=(x < 0 ? -delta : delta);
   if ( y != 0.0f ) s->rotation.y+=(y < 0 ? -delta : delta);
  }
 } 
 void RotateBySame( double x, double y ) {
  int i=0;
  double delta=1/10.0f;
  FOREACH(CubicSkyboxDefinition,s) {
   if ( x != 0.0f ) s->rotation.x+=(x < 0 ? -delta : delta);
   if ( y != 0.0f ) s->rotation.y+=(y < 0 ? -delta : delta);
  }
 }
 void NormalizeFactors() {  
  int i=0;
  int countp1=count+1;
  FOREACH(CubicSkyboxDefinition,s) {
   i++;
   s->radii=10.0f*iratiof(i,countp1);
   s->factor=0.1f;
  }
 }
 void NormalizeRadii() {  
  int i=0;
  int countp1=count+1;
  FOREACH(CubicSkyboxDefinition,s) {
   i++;
   s->radii=15.0f-10.0f*iratiof(i,countp1);
  }
 }
 void RotateByFactor( double x, double y ) {
  FOREACH(CubicSkyboxDefinition,s) {
   double delta=s->factor;
   if ( x != 0.0 ) s->rotation.x+=(x < 0 ? -delta : delta);
   if ( y != 0.0 ) s->rotation.y+=(y < 0 ? -delta : delta);
  }
 }
 void Tint( Crayon c ) {
  FOREACH(CubicSkyboxDefinition,s) s->tint.fromCrayon(c);
 }
 void Blend( Blends b ) {
  FOREACH(CubicSkyboxDefinition,s) s->blend=b;
 }
 CLEARLISTRESET(CubicSkyboxDefinition);
 ~CubicSkyboxDefinitions() { Clear(); }
};

class CubicMultiSkybox : public GLWindow {
public:
 CubicSkyboxDefinitions skyboxes;
 void Render() { skyboxes.Render(); }
};

class SkyboxMap : public ListItem {
public:
 string name;
 string f,b,l,r,u,d;
 GLImage *front,*back,*left,*right,*up,*down;
 int Loaded;
 SkyboxMap() : ListItem() {
  name=string("");
  b=string("");
  l=string("");
  r=string("");
  u=string("");
  d=string("");
  front=back=left=right=up=down=null;
  Loaded=0;
 }
 void Load() {
  if ( Loaded == 0 ) {
   front =library.Load((char *)f.c_str());
   back  =library.Load((char *)b.c_str());
   left  =library.Load((char *)l.c_str());
   right =library.Load((char *)r.c_str());
   up    =library.Load((char *)u.c_str());
   down  =library.Load((char *)d.c_str());
  }
  Loaded++;
 }
 void Unload() {
  Loaded--;
  if ( Loaded == 0 ) {
  }
 }
};

class SkyboxMaps : public LinkedList {
public:
 SkyboxMap *Any( int seed ) {
  if ( count==0 ) return null;
  SkyboxMap *result=(SkyboxMap *) Element(upto(count*1234,seed)%count);
  if (! result ) OUTPUT("SkyboxMaps:Any() warns resulting SkyboxMap was null.\n");
  return result;
 }
 SkyboxMap *Any() {
  if ( count==0 ) return null;
  SkyboxMap *result=(SkyboxMap *) Element(upto(count*1234)%count);
  if (! result ) OUTPUT("SkyboxMaps:Any() warns resulting SkyboxMap was null.\n");
  return result;
 }
 void LoadDefinitions( const char *filename ) {
  static char buf[STRING_SIZE];
  int loaded=0;
  Strings file;
  file.Load(filename);
  EACH(file.first,String,s) {
   if ( !s->s.empty() ) {
    const char *p;
    p=s->s.c_str();
    p=one_argcase(p,buf);
    Define(buf);
    loaded++;
   }
  }
  OUTPUT("SkyboxMaps::LoadDefinitions(`%s`) loaded %d map definitions.\n", filename, (int) loaded);
 }
 // Loads a skybox postfixed with:
 // "_front5.png", "_back6.png", "_left2.png", "_right1.png", "_top3.png", "_bottom4.png"
 void Define( const char *pre ) {
  SkyboxMap *m=new SkyboxMap;
  m->name=string(pre);
  m->f=string(pre)+string("_front5.png");
  m->b=string(pre)+string("_back6.png");
  m->l=string(pre)+string("_left2.png");
  m->r=string(pre)+string("_right1.png");
  m->u=string(pre)+string("_top3.png");
  m->d=string(pre)+string("_bottom4.png");
  Append(m);
 }
 SkyboxMap *find( const char *pre ) {
  SkyboxMap *m=null;
  FOREACH(SkyboxMap,s) if ( !str_infix((char *) s->name.c_str(),pre) ) return m=s;
  return m;
 }
 CubicSkyboxDefinition *CubicSkybox( Blends b, Crayon t ) {
  SkyboxMap *m=(SkyboxMap *) this->Any();
  if ( m->Loaded<1 ) m->Load();
  CubicSkyboxDefinition *n=new CubicSkyboxDefinition;
  n->textures[FRONT_ID]=m->front;
  n->textures[RIGHT_ID]=m->right;
  n->textures[BACK_ID]=m->back;
  n->textures[LEFT_ID]=m->left;
  n->textures[TOP_ID]=m->up;
  n->textures[BOTTOM_ID]=m->down;
  n->tint.fromCrayon(t);
  n->blend=b;
  return n;
 }
CubicSkyboxDefinition *CubicSkybox( const char *name, Blends b, Crayon t ) {
  SkyboxMap *m=find( name );
  if ( !m ) {
   OUTPUT("SkyboxMaps::CubicSkybox() Warning: could not find skybox map named `%s`",name );
   return null;
  }
  if ( m->Loaded<1 ) m->Load();
  CubicSkyboxDefinition *n=new CubicSkyboxDefinition;
  n->textures[FRONT_ID]=m->front;
  n->textures[RIGHT_ID]=m->right;
  n->textures[BACK_ID]=m->back;
  n->textures[LEFT_ID]=m->left;
  n->textures[TOP_ID]=m->up;
  n->textures[BOTTOM_ID]=m->down;
  n->tint.fromCrayon(t);
  n->blend=b;
  return n;
 }
 CLEARLISTRESET(SkyboxMap);
 ~SkyboxMaps() { Clear(); }
};

extern SkyboxMaps skyboxesNear;
extern SkyboxMaps skyboxesMid;
extern SkyboxMaps skyboxesFar;

#include "CubeMappedSphere.h"
#include "FBO.h"
#include "LookAtPerspective.h"

class CubeMappedSkybox : public ListItem {
public:
  CubeMappedSphere3d sphere;
  Vertexd rotation,rotatingFrom,rotatingTo;
  Zdouble rotating,rotationTime;
  Zp<GLImage> F,L,B,R,U,D;
  LookAtPerspective lap;
  ImmediateLighting lights;
  Zbool mousemove;
  void Init( GLImage *F, GLImage *L, GLImage *B, GLImage *R, GLImage *U, GLImage *D ){
   lap.Near=0.001;
   lap.eye.Set(0.0,0.0,0.0);
   lap.center.Set(0.0,0.0,-5.0);
   sphere.r=0.5f;
   sphere.res=10;
   sphere.VertexBufferAndImmediate();
   this->U=U;
   this->D=D;
   this->L=L;
   this->R=R;
   this->F=F;
   this->B=B;
   rotation.Zero();
   rotatingFrom.Zero();
   rotatingTo.Zero();
   rotationTime=1.0;
   rotating=1.0;
  }
  void Render(){
   glPush();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   lap.Apply();
//   glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_NONE); 
   glDisable(GL_CULL_FACE); glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glScaled(1.0,1.0,1.0);
   glRotated(rotation.x, 1.0, 0.0, 0.0);
   glRotated(rotation.y, 0.0, 1.0, 0.0);
   glRotated(rotation.z, 0.0, 0.0, 1.0);
//   glRotated(rotation.x, 0.0, 1.0, 0.0);
//   glRotated(rotation.y, 1.0, 0.0, 0.0);
   glColor4d(1.0,1.0,1.0,1.0);
   glEnable(GL_TEXTURE_2D);
   for ( int group=0; group<6; group++ ) {
    switch( group ) {
     case 0: glBindTexture(GL_TEXTURE_2D,F->texture); break; //FRONT_ID 
     case 1: glBindTexture(GL_TEXTURE_2D,R->texture); break; //RIGHT_ID 
     case 2: glBindTexture(GL_TEXTURE_2D,B->texture); break; //BACK_ID  
     case 3: glBindTexture(GL_TEXTURE_2D,L->texture); break; //LEFT_ID  
     case 4: glBindTexture(GL_TEXTURE_2D,D->texture); break; //BOTTOM_ID
     case 5: glBindTexture(GL_TEXTURE_2D,U->texture); break; //TOP_ID   
     default: OUTPUT("%d OUT OF RANGE.", group); break;
    }
    sphere.vboVNT[group].Render();
   }
   glDisable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,0);
   glPop();
  }
  void RotateTo( double x, double y, double t ) {
   if ( t == 0.0 ) {
    rotating=1.0;
    rotationTime=1.0;
    rotation.Set(x,y);
    rotatingFrom.Set(&rotation);
    rotatingTo.Set(&rotation);
   } else {
    rotatingFrom.Set(&rotation);
    rotatingTo.Set(x,y);
    rotationTime=t;
    rotating=0.0;
   }
  }
  Vertexd temp;
  void Between() {
   double expired=rotating/rotationTime;
   if ( expired < 1.0 ) {
    rotating+=FRAMETIME;
    if ( rotating > rotationTime ) {
     rotating=rotationTime;
     rotatingFrom.Set(&rotatingTo);
     rotation.Set(&rotatingTo);
    } else {
     Line line(rotatingFrom.x,rotatingFrom.y,rotatingTo.x,rotatingTo.y);
     line.Point(expired,&rotation);
    }
   } else if ( mousemove ) MouseMove();
  }
  void MouseMove(){
   rotation.x=iratiod(input.mxi-display.w2, display.w2)*180.0;
   rotation.y=iratiod(input.myi-display.h2, display.h2)*180.0;
  }
};