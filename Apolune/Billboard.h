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

#include "Art.h"
#include "Crayon.h"
#include "GLImage.h"
#include "MapPosition.h"
#include "LookAtPerspective.h"
#include "TextureLibrary.h"

static float	rotate_x = 30;
static float	rotate_y = 15;
static float	rotate_z = -5;

enum CheapBillboardModes {
 billboard_screen=0,
 billboard_axis=1,
 billboard_world=2,
};

class CheapBillboard : public ListItem {
public:
 Zp<Vertexd> rotation;
 Zp<GLImage> texture;
 Crayon tint;
 Blends blend;
 Vertexd position;
 CheapBillboardModes mode;
 Vertexd extents;
 PROGRAMMABLE(CheapBillboard,AlignPtr,align,Align);
 CheapBillboard() : ListItem() {
  mode=billboard_world;
  align=&CheapBillboard::AlignWorld;
  tint.Pick(alabaster);
  blend=none;
  extents.x=1.0;
  extents.y=1.5;
  extents.z=0.0;
 }
 void Mode( CheapBillboardModes mode ) {
  this->mode=mode;
  switch ( mode ) {
   case billboard_screen:
     align=&CheapBillboard::AlignScreen;
    break;
   case billboard_axis:
     align=&CheapBillboard::AlignAxis;
    break;
   case billboard_world:
     align=&CheapBillboard::AlignWorld;
    break;
  }
 }

 void Square() {
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D,texture->texture);
  glBegin (GL_QUADS);
  glTexCoord2i(0, 0); glVertex3d( extents.x,    extents.y, extents.z );
  glTexCoord2i(1, 0); glVertex3d(-extents.x,    extents.y, extents.z );
  glTexCoord2i(1, 1); glVertex3d(-extents.x,  - extents.y, -extents.z );
  glTexCoord2i(0, 1); glVertex3d( extents.x,  - extents.y, -extents.z );
  glEnd ();
 }

 void AlignAxis() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(-rotation->x, 1.0, 0.0, 0.0);
  Square();
  glPopMatrix();
 }
 void AlignWorld() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(-rotation->x, 1.0, 0.0, 0.0);
  glRotated(-rotation->y, 0.0, 1.0, 0.0);
  Square();
  glPopMatrix();
 }
 void AlignScreen() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(-rotation->x, 1.0, 0.0, 0.0);
  glRotated(-rotation->y, 0.0, 1.0, 0.0);
  glRotated(-rotation->z, 0.0, 0.0, 1.0);
  Square();
  glPopMatrix();
 }
};

class CheapBillboards : public LinkedList {
public:
 Zp<Vertexd> rotation;
 CheapBillboards() : LinkedList() { rotation=null; }
 void SetRotation( Vertexd*rotation ) { this->rotation=rotation; FOREACH(CheapBillboard,b) b->rotation=rotation; }
 void Render() { FOREACH(CheapBillboard,b) b->Align(); }
 CLEARLISTRESET(CheapBillboard);
};

class CheapBillboardsTest : public GLWindow {
public:
 LookAtPerspective lap;
 CheapBillboards billboards;
 MapPosition map;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  lap.eye.Set(0,0,15);
  lap.Near=0.0001;
  lap.Far=50.0;
//  map.position.z=-10; 
  map.Default();
  
  /* A cube made of 9 billboards */
  for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) for (int k = 0; k < 3; ++k) {
   CheapBillboard *b=new CheapBillboard;
  // if ( j==0 ) b->Mode(billboard_axis);
  // else if ( j==1 ) b->Mode(billboard_world);
  // else 
   b->Mode(billboard_screen);
   b->tint.Any();
   b->blend=transparency;
   b->texture=library.Load("data/images/flares/orange.png");
   b->position.x=(double)(i-2.0)/10.0;
   b->position.y=(double)(j-2.0)/10.0;
   b->position.z=(double)(k-2.0)/10.0;
   b->extents.Set(0.1,0.1,0.1);
   billboards.Append(b);
  }
  billboards.SetRotation(&map.rotation);
 }
 void Between() { MouseMove(); }
 void MouseMove() {
  if ( input->left ) {
   map.rotation.x+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
   map.rotation.y+=(double)(input->myi-input->oldmyi)/display->hd * 360.0;
//   if ( map.rotation.y < -90.0 ) map.rotation.y=-90.0;
//   if ( map.rotation.y > 90.0 ) map.rotation.y=90.0;
  }
  if ( input->right ) {
   map.rotation.z+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
  }
 }
 void WheelUp() { lap.eye.z-=1.0; }
 void WheelDown() { lap.eye.z+=1.0; }
 void Render() {
  glPush();
  lap.Apply();
  map.glPosition2();
  Coordinator();
  glEnable(GL_TEXTURE_2D);
  billboards.Render();
  map.glUnposition2();
  glPop();
  GL_Report("Billboard::Render");
 }
};

class CheapSphericalBillboard : public ListItem {
public:
 Crayon tint;
 Blends blend;
 Zp<GLImage> texture;
 Zp<LookAtPerspective> lap;
 Vertexd position;
 Zp<Vertexd> viewRotation;
 Vertexd extents;
 void Render( ) {
  //glDepthMask(GL_FALSE); // was recommended
  glPushMatrix();
  Blending(blend);
  glColor4fv(tint.floats);
  glTranslated(position.x,position.y,position.z);
  glRotated(viewRotation->x,-1.0,0.0,0.0);
  glRotated(viewRotation->y,0.0,-1.0,0.0);
  glBegin(GL_QUADS);
  glTexCoord2i(0,0); glVertex2d(-extents.x, extents.y);
  glTexCoord2i(0,1); glVertex2d(-extents.x,-extents.y);
  glTexCoord2i(1,1); glVertex2d( extents.x,-extents.y);
  glTexCoord2i(1,0); glVertex2d( extents.x, extents.y);
  glEnd();
  glPopMatrix();
 }
};

class CheapSphericalBillboards : public LinkedList {
public:
 Zp<Vertexd> viewRotation;
 void SetRotation( Vertexd*rotation ) { this->viewRotation=rotation; FOREACH(CheapSphericalBillboard,b) b->viewRotation=rotation; }
 void Render() { FOREACH(CheapSphericalBillboard,b) b->Render(); }
 CLEARLISTRESET(CheapSphericalBillboard);
};

class CheapSphericalBillboardsTest : public GLWindow {
public:
 LookAtPerspective lap;
 CheapSphericalBillboards billboards;
 Vertexd rotation,position;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  lap.eye.Set(0,0,15);
  lap.Near=0.0001;
  lap.Far=50.0;
//  map.position.z=-10; 
  
  /* A cube made of 9 billboards */
  for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) for (int k = 0; k < 3; ++k) {
   CheapSphericalBillboard *b=new CheapSphericalBillboard;
  // if ( j==0 ) b->Mode(billboard_axis);
  // else if ( j==1 ) b->Mode(billboard_world);
  // else 
   b->tint.Any();
   b->blend=transparency;
   b->texture=library.Load("data/images/flares/orange.png");
   b->position.x=(double)i-2.0;
   b->position.y=(double)j-2.0;
   b->position.z=(double)k-2.0;
   b->extents.Set(1.0,1.0);
   billboards.Append(b);
  }
  billboards.SetRotation(&rotation);
 }
 void MouseMove() {
  if ( input->left ) {
   rotation.x+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
   rotation.y+=(double)(input->myi-input->oldmyi)/display->hd * 360.0;
   if ( rotation.y < -90.0 ) rotation.y=-90.0;
   if ( rotation.y > 90.0 ) rotation.y=90.0;
  }
  if ( input->right ) {
   rotation.z+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
  }
 }
 void WheelUp() { lap.eye.z-=1.0; }
 void WheelDown() { lap.eye.z+=1.0; }
 void Between() {
  rotation.x+=1.0;
  if ( input->KeyDown(DX_LEFT)  ) position.x-=1.0;
  if ( input->KeyDown(DX_RIGHT) ) position.x+=1.0;
  if ( input->KeyDown(DX_UP)    ) position.y-=1.0;
  if ( input->KeyDown(DX_DOWN)  ) position.y+=1.0;
 }
 void Render() {
  glPush();
  lap.Apply();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x,1.0,0.0,0.0);
  glRotated(rotation.y,0.0,1.0,0.0);
  Coordinator();
  glEnable(GL_TEXTURE_2D);
  billboards.Render();
  glPop();
  GL_Report("Billboard::Render");
 }
};


class CPUBillboard : public ListItem {
public:
 Crayon tint;
 Blends blend;
 Zp<GLImage> texture;
 Zp<LookAtPerspective> lap;
 Vertexd position;
 Zp<Vertexd> viewRotation;
 Vertexd extents;
 CPUBillboard() : ListItem() {
  extents.x=1.0;
  extents.y=1.0;
 }
 void Render( ) {
  //glDepthMask(GL_FALSE); // was recommended
//  glPushMatrix();
//  Blending(blend);
//  glColor4fv(tint.floats);
  glTranslated(position.x,position.y,position.z);
//  glRotated(viewRotation->x,-1.0,0.0,0.0);
//  glRotated(viewRotation->y,0.0,-1.0,0.0);
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D,texture->texture);
  glBegin(GL_QUADS);
  glTexCoord2i(0,0); glVertex2d(-extents.x, extents.y);
  glTexCoord2i(0,1); glVertex2d(-extents.x,-extents.y);
  glTexCoord2i(1,1); glVertex2d( extents.x,-extents.y);
  glTexCoord2i(1,0); glVertex2d( extents.x, extents.y);
  glEnd();
//  glPopMatrix();
 }
};

class CPUBillboardCollection : public LinkedList {
public:
 Vertexd objectToCameraProjection,objectToCamera;
 Vertexd upAux;
 void RenderSpherical( LookAtPerspective *lap ) {
  glMatrixMode(GL_MODELVIEW);
  FOREACH(CPUBillboard,b) {
   BeginSpherical( lap, &b->position );
   b->Render();
   End();
  }
 }
 void BeginSpherical( LookAtPerspective *lap, Vertexd *position ) {
  glPushMatrix();
  objectToCameraProjection.Set( lap->eye.x - position->x, 0.0, lap->eye.z - position->z );
  objectToCameraProjection.Normalize();   

  // Cross product of camera target and projection vector,
  // used to determine whether the angle is negative or positive,
  // for positive angles upAux will be a vector pointing in the 
  // positive y direction, otherwise upAux will point downwards
  // effectively reversing the rotation.
  upAux.Set(
   lap->center.y * objectToCameraProjection.z - objectToCameraProjection.y * lap->center.z,
   lap->center.z * objectToCameraProjection.x - objectToCameraProjection.z * lap->center.x,
   lap->center.x * objectToCameraProjection.y - objectToCameraProjection.x * lap->center.y
  );

  // Compute the angle using the inner product.
  double angleCosine = lap->center.x * objectToCameraProjection.x + lap->center.y * objectToCameraProjection.y + lap->center.z * objectToCameraProjection.z;
  if ( (angleCosine < 0.99990) && (angleCosine > -0.9999) ) glRotated(acos(angleCosine)*180.0/PI, upAux.x, upAux.y, upAux.z );
   
  // Tilts the object (quad or otherwise) so that it faces the camera.

  objectToCamera.Set(
   lap->eye.x - position->x,
   lap->eye.y - position->y,
   lap->eye.z - position->z
  );

  objectToCamera.Normalize();

  // Compute the angle between v and v2, the look-up vector, using the inner product.
  angleCosine= objectToCameraProjection.x * objectToCamera.x + objectToCameraProjection.y * objectToCamera.y + objectToCameraProjection.z * objectToCamera.z;

 // Tilt the object. The test is done to prevent instability when objToCam and objToCamProj have a very small angle between them
  if ((angleCosine < 0.99990) && (angleCosine > -0.9999)) {
   if (objectToCamera.y < 0)
    glRotated(acos(angleCosine)*180.0/PI,1.0,0.0,0.0);	
   else
    glRotated(acos(angleCosine)*180.0/PI,-1.0,0.0,0.0);
  }
 }
 void End() {
  glPopMatrix();
 }
};


class CPUBillboardsTest : public GLWindow {
public:
 LookAtPerspective lap;
 CPUBillboardCollection billboards;
 Vertexd rotation,position;
 bool crazyMode;
 void OnLoad() {
  crazyMode=false;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  lap.eye.Set(0,0,15);
  lap.center.Set(0,0,-15);
  lap.Near=0.001;
  lap.Far=500.0;
//  map.position.z=-10; 
  
  /* A cube made of 9 billboards */
  for (int i = 0; i < 100; ++i) for (int j = 0; j < 100; ++j) for (int k = 0; k < 1; ++k) {
   CPUBillboard *b=new CPUBillboard;
  // if ( j==0 ) b->Mode(billboard_axis);
  // else if ( j==1 ) b->Mode(billboard_world);
  // else 
   b->tint.Any();
   b->blend=transparency;
   b->texture=library.Load("data/images/flares/orange.png");
   b->position.x=(double)(i-50)/100*10;
   b->position.y=(double)(j-50)/100*10;
   b->position.z=(double)((i+j)-100)/100*10;
   b->extents.Set(1.0,1.0);
   billboards.Append(b);
  }
 }
 void MouseMove() {
  if ( input->left ) {
   rotation.x+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
   rotation.y+=(double)(input->myi-input->oldmyi)/display->hd * 360.0;
   if ( rotation.y < -90.0 ) rotation.y=-90.0;
   if ( rotation.y > 90.0 ) rotation.y=90.0;
  }
  if ( input->right ) {
   rotation.z+=(double)(input->mxi-input->oldmxi)/display->wd * 360.0;
  }
 }
 void WheelUp() { lap.eye.z-=1.0; }
 void WheelDown() { lap.eye.z+=1.0; }
 void Between() {
  rotation.x+=1.0;
  if ( input->middle ) {
   if ( input->KeyDown(DX_LEFT)  ) lap.center.x-=1.0;
   if ( input->KeyDown(DX_RIGHT) ) lap.center.x+=1.0;
   if ( input->KeyDown(DX_UP)    ) lap.center.y-=1.0;
   if ( input->KeyDown(DX_DOWN)  ) lap.center.y+=1.0;
  } else {
   if ( input->KeyDown(DX_LEFT)  ) lap.eye.x-=1.0;
   if ( input->KeyDown(DX_RIGHT) ) lap.eye.x+=1.0;
   if ( input->KeyDown(DX_UP)    ) lap.eye.y-=1.0;
   if ( input->KeyDown(DX_DOWN)  ) lap.eye.y+=1.0;
  }
  if ( input->KeyDown(DX_SPACE) ) {
   lap.eye.Set(0,0,15);
   lap.center.Set(0,0,-15);
  }
  if ( input->KeyDown(DX_RETURN) ) {
   crazyMode=!crazyMode;
   if ( crazyMode ) lap.eye.z=lap.Far;
  }
  if ( crazyMode ) lap.eye.z-=0.2;
 }
 void Render() {
  glDisable(GL_CULL_FACE);
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  billboards.RenderSpherical(&lap);
  glPopMatrix();
  glPop();
  GL_Report("Billboard::Render");
 }
};
