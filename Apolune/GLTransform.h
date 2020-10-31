#pragma once

#include "Vertex.h"
#include "Matrix16.h"

class GLTransform : public ListItem {
public:
 Vertexd position,rotation,scale;
 GLTransform() : ListItem() {
  Reset();
 }
 GLTransform( int x, int y ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y);
 }
 GLTransform( float x, float y ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y);
 }
 GLTransform( double x, double y ) : ListItem() {
  Reset();
  position.Set(x,y);
 }
 GLTransform( int x, int y, int z ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y,(double)z);
 }
 GLTransform( float x, float y, float z ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y,(double)z);
 }
 GLTransform( double x, double y, double z ) : ListItem() {
  Reset();
  position.Set(x,y,z);
 }
 GLTransform( int x, int y, int w, int h ) : ListItem() {
  position.Set((double)x,(double)y);
  scale.Set((double)w,(double)h,1.0);
 }
 GLTransform( float x, float y, float w, float h ) : ListItem() {
  position.Set((double)x,(double)y);
  scale.Set((double)w,(double)h,1.0);
 }
 GLTransform( double x, double y, double w, double h) : ListItem() {
  position.Set(x,y);
  scale.Set(w,h,1.0);
 }
 GLTransform( int x, int y, int z, int w, int h, int d ) : ListItem() {
  position.Set((double)x,(double)y,(double)z);
  scale.Set((double)w,(double)h,(double)d);
 }
 GLTransform( float x, float y, float z, float w, float h, float d ) : ListItem() {
  position.Set((double)x,(double)y,(double)z);
  scale.Set((double)w,(double)h,(double)d);
 }
 GLTransform( double x, double y, double z, float w, float h, float d ) : ListItem() {
  position.Set(x,y,z);
  scale.Set(w,h,d);
 }
 GLTransform( Euler &by, int x, int y ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, float x, float y ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, double x, double y ) : ListItem() {
  Reset();
  position.Set(x,y);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, int x, int y, int z ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y,(double)z);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, float x, float y, float z ) : ListItem() {
  Reset();
  position.Set((double)x,(double)y,(double)z);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, double x, double y, double z ) : ListItem() {
  Reset();
  position.Set(x,y,z);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, int x, int y, int w, int h ) : ListItem() {
  position.Set((double)x,(double)y);
  scale.Set((double)w,(double)h,1.0);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, float x, float y, float w, float h ) : ListItem() {
  position.Set((double)x,(double)y);
  scale.Set((double)w,(double)h,1.0);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, double x, double y, double w, double h) : ListItem() {
  position.Set(x,y);
  scale.Set(w,h,1.0);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, int x, int y, int z, int w, int h, int d ) : ListItem() {
  position.Set((double)x,(double)y,(double)z);
  scale.Set((double)w,(double)h,(double)d);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, float x, float y, float z, float w, float h, float d ) : ListItem() {
  position.Set((double)x,(double)y,(double)z);
  scale.Set((double)w,(double)h,(double)d);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( Euler &by, double x, double y, double z, float w, float h, float d ) : ListItem() {
  position.Set(x,y,z);
  scale.Set(w,h,d);
  rotation.Set(by.pitch,by.yaw,by.roll);
 }
 GLTransform( GLTransform *in ) : ListItem() { Set(in); }
 void Reset() {
  position.Set(0.0,0.0,0.0);
  rotation.Set(0.0,0.0,0.0);
  scale.Set(1.0,1.0,1.0);
 }
 void Set(GLTransform *in) {
  rotation.Set(&in->rotation);
  position.Set(&in->position);
  scale.Set(&in->scale);
 }
 virtual void SetDepthMode() {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);
  glDepthRange(0.001,1.0);
  glDepthFunc(GL_LEQUAL);
 }
 virtual void UnsetDepthMode() {
  glDisable(GL_DEPTH_TEST);
 }
 virtual void Translate() {
  glTranslated(position.x,position.y,position.z);
 }
 virtual void TranslateToCenter() {
  glTranslated(position.x+scale.x/2.0,position.y+scale.y/2.0,position.z+scale.z/2.0);
 }
 virtual void Scale() {
  glScaled(scale.x,scale.y,scale.z);
 }
 void RotateZY() { // Swaps the Z and Y
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0);
  glRotated(rotation.z, 0.0, 1.0, 0.0);
 }
 void RotateXYZ() {
  glRotated(rotation.x,1.0,0.0,0.0);
  glRotated(rotation.y,0.0,1.0,0.0);
  glRotated(rotation.z,0.0,0.0,1.0);
 }
 void Rotate2d() {
  glRotated(rotation.z,0.0,0.0,1.0);
 }
 virtual void Rotate() { RotateZY(); }
 // Use for 2d
 void Apply() {
  glPush(false);
  Translate();
  Scale();
  glPushMatrix();
  Rotate2d();
 }
 void Unapply() {
  glPopMatrix();
  glPopMatrix();
  glPop();
 }
 // Use for 3d
 void PushAll() {
  Translate();
  glPushMatrix();///
  Rotate();
  glPushMatrix();
  Scale();
 }
 virtual void Render() {}
 void PushAllRender() {
  PushAll();
  Render();
  PopAll();
 }
 void PopAll() {
  glPopMatrix();
  glPopMatrix();
 }
 void fromString( const char *s ) {
  Zstring in(s);
  const char *w=in.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"position") ) {
    w=in.Next();
    position.fromString(w);
   } else if ( !str_cmp(w,"scale") ) {
    w=in.Next();
    scale.fromString(w);
   } else if ( !str_cmp(w,"rotation") ) {
    w=in.Next();
    rotation.fromString(w);
   }
   w=in.Next();
  }
 }
 string toString() {
  return FORMAT("position {%s} scale {%s}  rotation {%s}",
   position.toString().c_str(),
   scale.toString().c_str(),
   rotation.toString().c_str()
  );
 }
 string toStringDisplay() {
  return FORMAT("P %s mm\nS %s mm\nR %s deg",
   position.toString().c_str(),
   scale.toString().c_str(),
   rotation.toString().c_str()
  );
 }
};

class GLTransforms : public LinkedList {
public:
 void fromString( const char *s ) {
  Zstring string(s);
  const char *w=string.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"transform") ) {
    w=string.Next();
    GLTransform *t=new GLTransform;
    t->fromString(w);
    Append(t);
   }
   w=string.Next();
  }
 }
 string toString() {
  Zstring out;
  FOREACH(GLTransform,t) out+=string("transform { ")+t->toString()+string(" }\n");
  return out;
 }
 CLEARLISTRESET(GLTransform);
};