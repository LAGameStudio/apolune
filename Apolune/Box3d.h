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
#include "Vertexd.h"

class VBONormalsPolysGroup;
class ImmediateModel;
class Box3df {
public:
 float x, y, z;
 float x2, y2, z2;
 float w, h, d;
 Box3df() {
  x=y=z=x2=y2=z2=w=h=d=0.0f;
 }
 Box3df( Vertexd *extents ) {
  Vertexd position;
  x=y=z=x2=y2=z2=w=h=d=0.0f;
  Set(&position,extents);  
 }
 Box3df( Vertexd *extents, bool zeroed ) {
  Vertexd position;
  x=y=z=x2=y2=z2=w=h=d=0.0f;
  Set(&position,extents); 
  Zero();
 }
 Box3df( Vertexd *position, Vertexd *scale ) {
  x=y=z=x2=y2=z2=w=h=d=0.0f;
  Set(position,scale);  
 }
 Box3df( bool unitCube, bool centered=true, float scale=1.0f ) {
  if ( unitCube ) UnitCube(centered);
  else x=y=z=x2=y2=z2=w=h=d=0.0f;
 }
 void UnitCube( bool centered=true, float scale=1.0f ) {
  x=0.0f;  y=0.0f;  z=0.0f;
  w=1.0f;  h=1.0f;  d=1.0f;
  x2=x+w; y2=y+h;  z2=z+d;
  if ( centered ) {
   MoveBy(-w/2.0f,-h/2.0f,-d/2.0f);
  }
 }
 void Zero() {
  Vertexd position;
  Vertexd size(x2-x,y2-y,z2-z);
  Set(&position,&size);
 }
 void Set( Vertexd *position, Vertexd *size ) {
  x=(float)position->x;
  y=(float)position->y;
  z=(float)position->z;
  w=(float)size->x;
  h=(float)size->y;
  d=(float)size->z;
  x2=x+w;
  y2=y+h;
  z2=z+d;
 }
 void MoveTo( float nx, float ny, float nz ) {
  x=nx;   y=ny;   z=nz;  x2=x+w; y2=y+h; z2=z+d;
 }
 void MoveBy( float dx, float dy, float dz ) {
  x+=dx;  y+=dy;  z+=dz;  x2+=dx; y2+=dy; z2+=dz;
 }
 void Scale( float dx, float dy, float dz ) {
  Size( w*dx, h*dy, d*dz );
 }
 void Size( float sx, float sy, float sz ) {
  w=sx;   h=sy;   d=sz;  x2=x+w; y2=y+h; z2=z+d;
 }
 bool Inside( float tx, float ty, float tz ) {
  return ( x < tx && x2 > tx && y < ty && y2 > ty && z < tz && z2 > tz );
 }
 void toModel( ImmediateModel *out, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD );
 void toLines( ImmediateModel *out, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD );
 void toVBOGroup( VBONormalsPolysGroup *vbos, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD );
 void toVBOGroup( VBONormalsPolysGroup *vbos );
};

extern Box3df unitCube;
extern VBONormalsPolysGroup unitCubeVBOGroup;