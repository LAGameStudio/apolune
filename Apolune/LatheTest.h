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

/**************************************************************
  Author: H. Elwood Gilliland III
  3D Lathe tool example / test
 **************************************************************/

#include "GLWindow.h"

#include "Lathe.h"
#include "LatheTool.h"

#include "LookAtPerspective.h"
#include "ParallaxShader.h"
#include "ConvolverShader.h"
#include "TextureLibrary.h"
#include "Cube.h"

#include "fx_SectionalPercentages.h"
#include "fx_LineGraph.h"

class LatheTest : public GLWindow {
public:
 Zdisposable<Lathe> lathe;
 // Object
 Cartesiand position;
 Cartesiand rotation;
 ImmediateLighting lights;
 LookAtPerspective lap;
 Cube3d box;
 PolygonList pList;
 Zbool wireframe;
 Zfloat keyDelay;
 Zpointer<LatheGuide> guide;
 Zpointer<LatheLayer> layer;
 Zdisposable<Vertices> curve;
 fx_SectionalPercentages percentages;
 fx_LineGraph linegraph;
 Vertexd size;
 void OnLoad() {
  active=&a;
  background.Pick(indigo);
  Fullscreen();
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  lathe.Recycle();
  lathe->clockwise=true;
  position.x=-lathe->extents.x/2.0;
  position.z=-lathe->extents.y/2.0;
  wireframe=true;
  curve.Recycle(curve2d.Bezier(a.x,a.y,a.z,b.x,b.y,b.z,c.x,c.y,c.z,d.x,d.y,d.z));
  linegraph.SetExtents(16,16+256+8,384,384);
 }
 void ActionSave() {
  lathe->stl.ascii=true;
  lathe->stl.Write("data/out/test.stl");
 }
 void ActionNewGuideLine() {
  if ( !layer ) layer.pointer = lathe->stack.layers.Add();
  layer->guides.Add(&a,&b);
  lathe->Turn(&size,16,false,5.0);
 }
 void ActionNewGuideCurve() {
  if ( !layer ) layer.pointer = lathe->stack.layers.Add();
  layer->guides.Add(&a,&b,&c,&d);
  lathe->Turn(&size,16,false,5.0);
 }
 void ActionSelectGuide( bool next_guide ) {
  if ( layer ) {
   if ( next_guide ) {
    if ( guide ) {
     if ( guide->next ) {
      guide=(LatheGuide *) guide->next;
     } else {
      guide= (LatheGuide *) layer->guides.first;
     }
    }
   } else {
   }
  }
 }
 void ActionNewLayer() {
  layer.pointer = lathe->stack.layers.Add();
  percentages.AddDivision();
  lathe->Apply(&percentages.values);
 }
 void ActionSelectLayer( bool next_layer ) {
  if ( next_layer ) {
   if ( !layer->next ) layer.pointer = (LatheLayer *) lathe->stack.layers.first;
   else layer.pointer = (LatheLayer *) layer->next;
  } else {
  }
  if ( layer ) guide= (LatheGuide *) layer->guides.first;
 }
 void ActionDeleteSelectedGuide() {
 }
 void ActionDeleteSelectedLayer() {
 }
 void ActionRecreate() {
  lathe->Turn(&size,16,false,1.0);
  position.x=-lathe->extents.x/2.0;
  position.z=-lathe->extents.y/2.0;
 }
 void ActionRecreateFromPoints() {
  lathe->Turn(&linegraph.points,&size,16,false,1.0);
  position.x=-lathe->extents.x/2.0;
  position.z=-lathe->extents.y/2.0;
 }
 void Between() {
  linegraph.Between();
  if ( MOUSEWITHIN(16+256+8+256+8,16,16+256+8+256+8+16,16+256) ) { // Z
   if ( input->left ) {
    size.z=iratiod(input->myi-16,256)*100.0;
   }
  } else if ( MOUSEWITHIN(16+256+8,16,16+256+8+256,256+16) ) { // XY Resizer
   if( input->left ) {
    size.Set(iratiod(input->mxi-(16+256+8),256)*100.0,iratiod(input->myi-16,256)*100.0,size.z);
   } else if ( input->leftReleased() ) {
    ActionRecreate();
   }
  } else if ( MOUSEWITHIN(16,16,256+16,256+16) ) { // Curve-maker
   if( input->left && keyDelay <= 0.0f ) {
    keyDelay=0.25f;
    active->x=iratiod(input->mxi-16,256);
    active->y=iratiod(input->myi-16,256);
    if ( active == &a ) { a.y=0.0; active=&b; }
    else if ( active == &b ) active=&c;
    else if ( active == &c ) active=&d;
    else if ( active == &d ) { d.y=1.0; active=&a; }
    curve.Recycle(curve2d.Bezier(a.x,a.y,a.z,b.x,b.y,b.z,c.x,c.y,c.z,d.x,d.y,d.z));
   }
  }
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  if ( keyDelay > 0.0f ) keyDelay-=(float)FRAMETIME;
  else {
   if ( input->KeyDown(Up) ) { ActionSelectGuide(true); keyDelay=0.25f; }
   if ( input->KeyDown(Down) ) { ActionSelectGuide(false); keyDelay=0.25f; }
   if ( input->KeyDown(DX_C) ) { ActionNewGuideCurve(); keyDelay=0.25f; }
   if ( input->KeyDown(Slash) ) { ActionNewGuideLine(); keyDelay=0.25f; }
   if ( input->KeyDown(Spacebar) ) { ActionNewLayer(); keyDelay=0.25f; }
   if ( input->KeyDown(Enter) ) { ActionRecreateFromPoints(); keyDelay=0.25f; }
   if ( input->KeyDown(Left) ) { ActionSelectLayer(true); keyDelay=0.25f; }
   if ( input->KeyDown(Right) ) { ActionSelectLayer(false); keyDelay=0.25f; }
   //if ( input->KeyDown(DX_D) ) { ActionDeleteSelected(); keyDelay=0.25f; }
   //if ( input->KeyDown(DX_W) ) { wireframe=!wireframe; keyDelay=0.25f; }
   if ( input->KeyDown(NumpadPlus) ) { lap.eye.Set(0.0,0.0,lap.eye.z/2); keyDelay=0.25f; }
   if ( input->KeyDown(NumpadMinus) ) { lap.eye.Set(0.0,0.0,lap.eye.z*2); keyDelay=0.25f; }
  }
 }
 void Render() {
  Blending(none);
  glPush();
  lap.Apply();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();///
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0);
   glDepthFunc(GL_LEQUAL);
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
   lathe->RenderWireframe();
//   glDisable(GL_DEPTH_TEST);
  glPopMatrix();
  glPushMatrix();///
   glRotatef((GLfloat) rotation.x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation.y, 0.0, 0.0, 1.0);
   glLineWidth(4.2f);
   Coordinator();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) (lathe->extents.x*0.7), 0.0f,1.0f);
    Text("RIGHT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) -(lathe->extents.x*0.7), 0.0f,1.0f);
    Text("LEFT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) -(lathe->extents.y*0.7), 1.0f);
    Text("BACK",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) (lathe->extents.y*0.7), 1.0);
    Text("FRONT",0,0,2,3,true,0);
   glPopMatrix();
   glLineWidth(1.2f);
   glPushMatrix();//
    glTranslatef((GLfloat) -(lathe->extents.x/2)-10.0f, (GLfloat) -(lathe->extents.y*0.5), 0.0f);
    GLfloat cm=0.0f;
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("%-2d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > lathe->extents.y ) break;
    }
   glPopMatrix();//
   glPushMatrix();//
    glTranslatef( (GLfloat) -(lathe->extents.x*0.5), (GLfloat) -(lathe->extents.y*0.5), 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("____ %-2d cm",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > lathe->extents.x ) break;
    }
   glPopMatrix();//
  glPopMatrix();///
  glPop();
  Blending(none);
  glLineWidth(1.2f);
  glColor3d(0.0,1.0,0.0);
  RenderHUD();
 }
 Zpointer<Vertexd> active;
 Vertexd a,b,c,d;
 void RenderHUD() {
  Rectangle(crayons.jam(0,0.0f,0.15f,0.15f,0.8f),16,16,256+16,256+16);
  DrawLine2Color(crayons.Pick(richElectricBlue),crayons.Pick(macNcheese),
   16+(256*a.x),16+(256*a.y),
   16+(256*b.x),16+(256*b.y)
  );
  DrawLine2Color(crayons.Pick(richElectricBlue),crayons.Pick(macNcheese),
   16+(256*b.x),16+(256*b.y),
   16+(256*c.x),16+(256*c.y)
  );
  DrawLine2Color(crayons.Pick(richElectricBlue),crayons.Pick(macNcheese),
   16+(256*c.x),16+(256*c.y),
   16+(256*d.x),16+(256*d.y)
  );
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(green),
   16+(256*a.x),16+(256*0.0),
   16+(256*b.x),16+(256*1.0)
  );
  Text("A",16+256*a.x,16+256*a.y,8,10,false,0);
  Text("B",16+256*b.x,16+256*b.y,8,10,false,0);
  Text("b",16+256*b.x,16+256*1.0,8,10,false,0);
  Text("C",16+256*c.x,16+256*c.y,8,10,false,0);
  Text("D",16+256*d.x,16+256*d.y,8,10,false,0);
  glColor4f(1.0f,0.2f,0.9f,1.0f);
  glBegin(GL_LINES);
  EACH(curve->first,Vertex,p) if ( p->next ) {
   Vertex *n=(Vertex *) (p->next);
   glVertex2f( 16+256*p->x, 16+256*p->y );
   glVertex2f( 16+256*n->x, 16+256*n->y );
  }
  glEnd();
  if ( layer ) layer->Draw( 16+256+16, 16+256+16, 64, 64, false );
  unsigned int i=0;
  double h=display->hd / ( (display->hd-32-256.0) / 128.0 );
  EACH(lathe->stack.layers.first,LatheLayer,L) {
   crayons.Pick(red255);
   L->Draw( 16.0, 16.0+256+16+i*h, h, h, true );
   if ( L == layer ) Rectangle(red255,16.0,16.0+256+16+i*h,16+h,16+256+16+i*h+h );
   else Rectangle(blue255,16.0,16.0+256+16+i*h,16+h,16+256+16+i*h+h );
   crayons.Pick(green);
   Text(FORMAT("%d: %d guides",(int)i,(int)L->guides.count).c_str(),display->wd-128.0, 32.0+i*128.0,6,8,false,0);
   i++;
  }
  Text(FORMAT("%d",(int)lathe->stack.layers.count).c_str(), display->wd-128.0-16.0, 32.0+i*128.0, 8, 10, true, 0);
  // Draw resizer
  if ( MOUSEWITHIN(16+256+8+256+8,16,16+256+8+256+8+16,16+256) ) { // Z
   Area(crayons.jami(0,64,48,16,64),16+256+8+256+8,16,16+256+8+256+8+16,256+16);
  }
  Rectangle(crayons.Pick(macNcheese),16+256+8+256+8,16,16+256+8+256+8+16,256+16);
  DrawLine(crayons.Pick(magenta),
   16+256+8+256+8,
   16+(int)((size.z/100.0)*256.0),
   16+256+8+256+8+16,
   16+(int)((size.z/100.0)*256.0)
  );
  if ( MOUSEWITHIN(16+256+8,16,16+256+8+256,256+16) ) { // XY
   Area(crayons.jami(0,64,48,16,64),16+256+8,16,16+256+8+256,256+16);
  }
  Rectangle(crayons.Pick(macNcheese),16+256+8,16,16+256+8+256,256+16);
  crayons.gl(green255);
  DrawCircle(16+256+8+128,16+128,size.x,16);
  DrawLine(crayons.Pick(magenta),
   16+256+8,
   16+(int)((size.y/100.0)*256.0),
   16+256+8+256,
   16+(int)((size.y/100.0)*256.0)
  );
  DrawLine(crayons.Pick(mauve),
   16+256+8+(int)((size.x/100.0)*256.0),
   16,
   16+256+8+(int)((size.x/100.0)*256.0),
   16+256
  );
  Text(FORMAT("%1.2f by %1.2f by %1.2f tall",size.x,size.y,size.z).c_str(),16+256+8+128,256+16+16,8,10,false,0);
  linegraph.Render();
 }
};