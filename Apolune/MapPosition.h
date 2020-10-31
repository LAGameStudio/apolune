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
#include "Vertex.h"

struct MapPosition {
 Vertexd camera; // camera's location
 Vertexd position,direction,velocity,acceleration,momentum,rotation,scale,spin,growth;
// MapPosition() { Default(); }
 SKEYSTART
  SKEY("camera",       { SKEYPAIR; camera.fromString((char *) key.c_str()); } )
  SKEY("position",     { SKEYPAIR; position.fromString((char *) key.c_str()); } )
  SKEY("direction",    { SKEYPAIR; direction.fromString((char *) key.c_str()); } )
  SKEY("velocity",     { SKEYPAIR; velocity.fromString((char *) key.c_str()); } )
  SKEY("acceleration", { SKEYPAIR; acceleration.fromString((char *) key.c_str()); } )
  SKEY("momentum",     { SKEYPAIR; momentum.fromString((char *) key.c_str()); } )
  SKEY("rotation",     { SKEYPAIR; rotation.fromString((char *) key.c_str()); } )
  SKEY("scale",        { SKEYPAIR; scale.fromString((char *) key.c_str()); } )
  SKEY("spin",         { SKEYPAIR; spin.fromString((char *) key.c_str()); } )
 SKEYEND("MapPosition");
 string toString() {
  incdent();
  string out=string("");
  out+=indention+string("camera {")+camera.toString()+string("}\n");
  out+=indention+string("position {")+position.toString()+string("}\n");
  out+=indention+string("direction {")+direction.toString()+string("}\n");
  out+=indention+string("velocity {")+velocity.toString()+string("}\n");
  out+=indention+string("acceleration {")+acceleration.toString()+string("}\n");
  out+=indention+string("momentum {")+momentum.toString()+string("}\n");
  out+=indention+string("rotation {")+rotation.toString()+string("}\n");
  out+=indention+string("scale {")+scale.toString()+string("}\n");
  out+=indention+string("spin {")+spin.toString()+string("}\n");
  decdent();
  return out;
 }
 void BinaryWrite( BinaryFile *open ) {
  camera.BinaryWrite(open);
  position.BinaryWrite(open);
  direction.BinaryWrite(open);
  velocity.BinaryWrite(open);
  acceleration.BinaryWrite(open);
  momentum.BinaryWrite(open);
  rotation.BinaryWrite(open);
  scale.BinaryWrite(open);
  spin.BinaryWrite(open);  
  growth.BinaryWrite(open);
 }
 void BinaryRead( BinaryFile *open ) {
  camera.BinaryRead(open);
  position.BinaryRead(open);
  direction.BinaryRead(open);
  velocity.BinaryRead(open);
  acceleration.BinaryRead(open);
  momentum.BinaryRead(open);
  rotation.BinaryRead(open);
  scale.BinaryRead(open);
  spin.BinaryRead(open);  
  growth.BinaryRead(open);
 }
 float Distance2df( Vertexd *p ) {
  return fdistance( (float) p->x, (float) p->y, (float) position.x, (float) position.y );
 }
 float Distance2df( MapPosition *p ) {
  return fdistance( (float) p->position.x, (float) p->position.y, (float) position.x, (float) position.y );
 }
 float Distance2di( Cartesian *p ) {
  return fdistance( (float) p->x, (float) p->y, (float) position.x, (float) position.y );
 }
 float Distance2df( Cartesian *p ) {
  return fdistance( p->fx, p->fy, (float) position.x, (float) position.y );
 }
 void Set( MapPosition *p ) {
  camera.Set(p->camera);
  position.Set(p->position);
  direction.Set(p->direction);
  velocity.Set(p->velocity);
  acceleration.Set(p->acceleration);
  momentum.Set(p->momentum);
  rotation.Set(p->rotation);
  scale.Set(p->scale);
 }
 void Default() {
  direction.None();
  velocity.None();
  acceleration.None();
  momentum.None();
  scale.Set(1.0,1.0,1.0);
 }
 // GL Positioning
 virtual void glPosition( MapPosition *m ) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(
   (camera.x+m->position.x),
   (camera.y+m->position.y),
   (camera.z+m->position.z)
  );
  glRotated(m->rotation.x, 1.0, 0.0, 0.0);
  glRotated(m->rotation.y, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
 }
 virtual void glUnposition( MapPosition *m ) {
  glPopMatrix();
 }
 virtual void glPosition() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(
   (camera.x+position.x),
   (camera.y+position.y),
   (camera.z+position.z)
  );
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
 }
 virtual void glUnposition() {
  glPopMatrix();
 }
 virtual void glPosition2() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(
   (camera.x+position.x),
   (camera.y+position.y),
   (camera.z+position.z)
  );
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
 }
 virtual void glUnposition2() {
  glPopMatrix();
 }
};