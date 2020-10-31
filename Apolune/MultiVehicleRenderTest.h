#pragma once
#include "GLWindow.h"
#include "Matter.h"
#include "fx_Object3d.h"
#include "Vehicle.h"
#include "LimitedProce55Viewer2d.h"

extern PLYs lowPLYprimitives;

class MultiVehicleRenderTest : public LimitedProce55Viewer2d {
public:
 MapObjects vehicles;
 BLUEPRINT b;
 Vertexd *camera;

 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);

  // Generate a test vehicle by creating a blueprint with a base and components plugged into it.
  Substance *s;
  b.base.component=new COMPONENT;
  b.base.component->matters.Append(new Matter(lowPLYprimitives.Random(),s=substances.Random()));
  b.base.component->matters.substance=s;
  for ( int j=0; j<10; j++ ) b.base.component->matters.Append(new Matter(lowPLYprimitives.Random(),s));
  for ( int i=0; i<10; i++ ) {
   COMPONENT *c=new COMPONENT;
   s=substances.Random();
   c->matters.substance=s;
   for ( int j=0; j<10; j++ ) c->matters.Append(new Matter(lowPLYprimitives.Random(),s));
   Plug *p=new Plug;
   p->component=c;
   p->parent=b.base.component;
   b.components.Append(p);
  }
  // Create some test vehicles and assign the nearest camera.
  for ( int i=0; i<10; i++ ) {
   Vehicle *v= new Vehicle;
   v->screen.w=512;
   v->screen.h=512;
   v->blueprint=&b;
   camera=&v->map.camera;
   v->map.position.x=uniform()*(double)display->w;
   v->map.position.y=uniform()*(double)display->h;
   v->Reset();
   vehicles.Append(v);
  }
 }
 void Render() {
  EACH(vehicles.first,Vehicle,v) {
   v->Render();
  }
 }
 void Between() {
  if ( input->KeyDown(DIK_LEFT) ) camera->x-=1;
  else
  if ( input->KeyDown(DIK_RIGHT) ) camera->x+=1;
  if ( input->KeyDown(DIK_UP) ) camera->y-=1;
  else
  if ( input->KeyDown(DIK_DOWN) ) camera->y+=1;
 }
};