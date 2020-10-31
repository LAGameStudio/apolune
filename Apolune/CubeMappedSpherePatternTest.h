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
#include "ListItem.h"
#include "LinkedList.h"
#include "CubeMappedSphere.h"
#include "FBO.h"
#include "LookAtPerspective.h"

/*
 * CubeMappedSphere is generating a sphere whose texcoords
 * (when facing out, the default - thus anything rendered inside will be inverted,
 * if you wish to flip you must invert some of the coordinates)
 *
 * The ball is rendered thusly:
 *  
 *    U    <- except the U is vertically flipped
 *   RFLB
 *    d
 *
 * Attachments (with default texcoords):
 *
 *       F                  F
 *       v                  v
 *  R -> d <- L         L-> U <- R
 *       ^                  ^
 *       B                  B
 *   (bottoms)           (tops)
 *
 * if you take cards and lay them out BdFLRU, you should note that the top
 * of the sphere (U) is horizontally inverted based on extensive tests.
 * see cms.psd if the file is around, or you can recreate this by using
 * a test pattern with uniform multi-color corner dots, ie:
 *  r       g
 *  | side  |
 *  |  name |
 *  y       b
 *
 * However, further testing revealed U and D are inverted in the FBO.  No idea.
 */
class CubeMappedSpherePatternTest : public GLWindow {
public:
  CubeMappedSphere3d sphere;
  Crayon color;
  Substance s;
  float yAngle;
  float xAngle;
  GLImage *F,*L,*B,*R,*U,*D;
  LookAtPerspective lap;
  ImmediateLighting lights;
  void OnLoad(){
   lap.Near=0.0001;
   yAngle=0;
   xAngle=0;
   color.Pick(indigo);
   Anchor(0.0f, 0.0f, 0.0f, 0.0f);
   sphere.r=0.5f;
   sphere.res=10;
   sphere.VertexBufferAndImmediate();
   U=library.find("data/images/skyboxes/test/U.png");
   D=library.find("data/images/skyboxes/test/d.png");
   L=library.find("data/images/skyboxes/test/L.png");
   R=library.find("data/images/skyboxes/test/R.png");
   F=library.find("data/images/skyboxes/test/F.png");
   B=library.find("data/images/skyboxes/test/B.png");
  }
  void Render(){
   glPush();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   int face=0;
   int i=0;
   int j=0;
   lap.Apply();
   glTranslated(0.0,0.0,-1.3);
//   lights.On();
   glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_NONE); 
   glRotatef((GLfloat) xAngle, 0.0f, 1.0f, 0.0f);
   glRotatef((GLfloat) yAngle, 1.0f, 0.0f, 0.0f);
   glColor4f(1.0f,1.0f,1.0f,1.0f);
   glEnable(GL_TEXTURE_2D);
   for ( int group=0; group<6; group++ ) {
    switch( group ) {
     case 0: glBindTexture(GL_TEXTURE_2D,F->texture); break; //FRONT_ID 
     case 1: glBindTexture(GL_TEXTURE_2D,R->texture); break; //RIGHT_ID 
     case 2: glBindTexture(GL_TEXTURE_2D,B->texture); break; //BACK_ID  
     case 3: glBindTexture(GL_TEXTURE_2D,L->texture); break; //LEFT_ID  
     case 4: glBindTexture(GL_TEXTURE_2D,U->texture); break; //TOP_ID   
     case 5: glBindTexture(GL_TEXTURE_2D,D->texture); break; //BOTTOM_ID
     default: OUTPUT("%d OUT OF RANGE.", face); break;
    }
    sphere.vboVNT[group].Render();
   }
//   lights.Off();
//   glDisable(GL_CULL_FACE);
   glDisable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,0);
   glPop();
  }
  void MouseMove(){
   xAngle=iratiof(input->mxi-display->w/2, display->w/2)*180.0f;
   yAngle=iratiof(input->myi-display->h/2, display->h/2)*180.0f;
  }
};