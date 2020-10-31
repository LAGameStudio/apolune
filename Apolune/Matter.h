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

#include "Box3d.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "OldVBO.h"
#include "FBO.h"
#include "PLY.h"
#include "Primitive3d.h"
#include "LookAtPerspective.h"

#include "Chemistry.h"
#include "Globals.h" // for matter detail hack

#include "dhpowareMath.h"

class Scene;

class Matter : public ListItem {
public:
 Zp<Substance> substance;
 Zp<PLY> ply;
 Zp<Primitive3d> p3d;
 Vertexd position;
 Vertexd scale;
 Vertexd rotation;
 GLuint detailTexture;
 PROGRAMMABLE(Matter,RenderPtr,render,Render);
 PROGRAMMABLE(Matter,RenderDetailPtr,renderdetail,RenderDetail);
 PROGRAMMABLE(Matter,RenderColorCodedPtr,rendercolorcoded,RenderColorCoded);
 void Nothing(){
//#if defined(DEBUG)
//  return;
//#endif
 }
 Matter( BinaryFile *open ) {
  detailTexture=0;
  p3d=null;
  ply=null;
  substance=null;
  render=&Matter::Nothing;
  renderdetail=&Matter::Nothing;
  rendercolorcoded=&Matter::Nothing;
  BinaryRead(open);
 }
 Matter( string s ) : ListItem() {
  detailTexture=0;
  scale.Set(1.0,1.0,1.0);
  p3d=null;
  ply=null;
  substance=null;
  render=&Matter::Nothing;
  renderdetail=&Matter::Nothing;
  rendercolorcoded=&Matter::Nothing;
  fromString((char *) s.c_str());
 }
 Matter( PLY *p, Substance *s ) : ListItem() {
  detailTexture=0;
  scale.Set(1.0,1.0,1.0);
  ply=p;
  p3d=null;
  render=&Matter::RenderPLY;
  renderdetail=&Matter::RenderPLYDetailTexture;
  rendercolorcoded=&Matter::RenderPLYColorCoded;
  substance=s;
 }
 Matter( Primitive3d *p3d, Substance *s ) : ListItem() {
  detailTexture=0;
  scale.Set(1.0,1.0,1.0);
  this->substance=s;
  render=&Matter::RenderP3d;
  renderdetail=&Matter::RenderP3dDetailTexture;
  rendercolorcoded=&Matter::RenderP3dColorCoded;
  this->p3d=p3d;
  ply=null;
 }
 // Creates a copy
 Matter( Matter *m ) : ListItem() {
  detailTexture=0;
  position.Set(m->position);
  scale.Set(m->scale);
  rotation.Set(m->rotation);
  if ( m->p3d ) {
   ply=null;
   p3d=m->p3d;
   render=&Matter::RenderP3d;
   renderdetail=&Matter::RenderP3dDetailTexture;
   rendercolorcoded=&Matter::RenderP3dColorCoded;
   substance=m->substance;
  } else if ( m->ply ) {
   ply=m->ply;
   p3d=null;
   render=&Matter::RenderPLY;
   renderdetail=&Matter::RenderPLYDetailTexture;
   rendercolorcoded=&Matter::RenderPLYColorCoded;
   substance=m->substance;
  } else {
   p3d=null;
   ply=null;
   substance=null;
   render=&Matter::Nothing;
   renderdetail=&Matter::Nothing;
   rendercolorcoded=&Matter::Nothing;
  }
 }
 void BinaryRead( BinaryFile *open ) {
  position.BinaryRead(open);
  scale.BinaryRead(open);
  rotation.BinaryRead(open);
  int temp;
  open->read(&temp); if ( temp == 1 ) {
   string match; open->read(&match);
   substance=substances.findByMaterial((char *)match.c_str());
  }
  open->read(&temp); if ( temp == 1 ) {
   string match; open->read(&match);
   ply=lowPLYprimitives.find((char *)match.c_str());
   render=&Matter::RenderPLY;
   renderdetail=&Matter::RenderPLYDetailTexture;
   rendercolorcoded=&Matter::RenderPLYColorCoded;
  }
  open->read(&temp); if ( temp == 1 ) {
   string match; open->read(&match);
//   p3d=primitives.find((char *)match.c_str());
   render=&Matter::RenderP3d;
   renderdetail=&Matter::RenderP3dDetailTexture;
   rendercolorcoded=&Matter::RenderP3dColorCoded;
  }
 }
 void BinaryWrite( BinaryFile *open ) {
  position.BinaryWrite(open);
  scale.BinaryWrite(open);
  rotation.BinaryWrite(open);
  int temp;
  if ( substance ) { temp=1; open->write(&temp); open->scrawl(substance->material->name); }
  else { temp=0; open->write(&temp); }
  if ( ply ) { temp=1; open->write(&temp); open->scrawl(ply->name); }
  else { temp=0; open->write(&temp); }
  if ( p3d ) { temp=1; open->write(&temp); open->scrawl(p3d->name); }
  else { temp=0; open->write(&temp); }
 }
 string toString() {
  incdent();
  string out=string("");
  FORMAT(buf,2048,"position { %1.5f, %1.5f, %1.5f }\n",
   (float) position.x, (float) position.y, (float) position.z );
  out+=indention+string(buf);
  FORMAT(buf,2048,"scale { %1.5g, %1.5g, %1.5g }\n", 
   (float) scale.x, (float) scale.y, (float) scale.z );
  out+=indention+string(buf);
  FORMAT(buf,2048,"rotation { %1.5g, %1.5g, %1.5g }\n",
   (float) rotation.x, (float) rotation.y, (float) rotation.z );
  out+=indention+string(buf);
  if ( ply ) {
   FORMAT(buf,2048,"ply \"%s\"\n", ply->name.c_str() );
   out+=indention+string(buf);
  }
  if ( p3d ) {
   FORMAT(buf,2048,"p3d \"%s\"\n", p3d->name.c_str() );
   out+=indention+string(buf);
  }
  if ( substance ) {
   FORMAT(buf,2048,"substance \"%s\"\n", substance->material->name.c_str() );
   out+=indention+string(buf);
  }
  decdent();
  return out;
 }
 SKEYSTART
  SKEY("ply",      { SKEYPAIR; ply=lowPLYprimitives.find(key.c_str()); render=&Matter::RenderPLY; })
  SKEY("p3d",      { SKEYPAIR; /* p3d=primitives3d.find(); */ })
  SKEY("position", { SPARAMVertex(position);  })
  SKEY("scale",    { SPARAMVertexd(scale);    })
  SKEY("rotation", { SPARAMVertexd(rotation); })
  SKEY("substance",{ SKEYPAIR; substance=substances.findByMaterial(key.c_str()); })
 SKEYEND("Matter")
 /* This method draws one material per matter.
    We switched out of this to increase performance. */
#if defined(NEVER)
 void RenderPLY() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  if ( substance ) substance->material->Apply();
  ply->model.Render(); //dlist.Call();
  if ( substance ) substance->material->Unapply();
  glPopMatrix();
 }
 void RenderP3d() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  if ( substance ) substance->material->Apply();
  p3d->model.Render(); //dlist.Call();
  if ( substance ) substance->material->Unapply();
  glPopMatrix();
 }
#endif
 void RenderPLY() {
  substance->material->Apply();
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  ply->model.Render(); //dlist.Call();
  glPopMatrix();
  substance->material->Unapply();
 }
 void RenderP3d() {
  substance->material->Apply();
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  p3d->model.Render(); //dlist.Call();
  glPopMatrix();
  substance->material->Unapply();
 }
 void RenderPLYDetailTexture() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  ply->model.Render(); //dlist.Call();
  glPopMatrix();
 }
 void RenderP3dDetailTexture() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  p3d->model.Render(); //dlist.Call();
  glPopMatrix();
 }
 void RenderPLYColorCoded() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  ply->model.Render(); //dlist.Call();
  glPopMatrix();
 }
 void RenderP3dColorCoded() {
  glPushMatrix();
  glTranslated(position.x,position.y,position.z);
  glRotated(rotation.x, 1.0, 0.0, 0.0);
  glRotated(rotation.y, 0.0, 1.0, 0.0);
  glRotated(rotation.z, 0.0, 0.0, 1.0);
  glScaled(scale.x,scale.y,scale.z);
  p3d->model.Render(); //dlist.Call();
  glPopMatrix();
 }
 void Scale( float x, float y, float z ) {
  scale.Set(x,y,z);
 }
 void Move( float x, float y, float z ) {
  position.Add(x,y,z);
 }
 void Rotate( Vertex *pivot, Angle *xy, Angle *xz ) {
 }
 void RotateThenMove( float x, float y, float z, Vertex *pivot, Angle *xy, Angle *xz ) {
 }
 float RoughVolume() {
  float scaleAvg=(float)(scale.x+scale.y+scale.z)/3.0f;
  if ( ply ) return ply->model.volume*scaleAvg;
  if ( p3d ) {
   if ( p3d->model.volume == 0.0f ) p3d->model.RoughVolume();
   return p3d->model.volume*scaleAvg;
  }
  return 1.0f*scaleAvg;
 }
 // Converts a vertex of the Matter's model to GL space
 void MatterPoint( Vertex *input, Vertex *output ) {
  Vertex temp;
  temp.Set(input);
  temp.Scale((float)scale.x,(float)scale.y,(float)scale.z);
  temp.Rotate((float)rotation.x,(float)rotation.y,(float)rotation.z);
  temp.Add((float)position.x,(float)position.y,(float)position.z);
  output->Set( &temp );
 }
};

class MatterHandle : public Handle<Matter> {
public:
 MatterHandle( Matter * m ) : Handle() { p=m; }
};
class MatterHandles : public LinkedList {
public:
 CLEARLISTRESET(MatterHandle);
 void Push( Matter *m ) {
  Append(new MatterHandle(m));
 }
 void Pop( Matter *m ) {
  EACHN(first,MatterHandle,mh,{
   if ( mh->p == m ) {
    Remove(mh);
    delete mh;
   }
  });
 }
 bool Contains( Matter *m ) {
  FOREACH(MatterHandle,mh) if ( mh->p == m ) return true;
  return false;
 }
 bool Contains( double x, double y, double z ) {
  FOREACH(MatterHandle,mh)
   if ( mh->p->position.x == x
     && mh->p->position.y == y
     && mh->p->position.z == z ) return true;
  return false;
 }
 void PopNearPosition( double boxRange, Matter *m ) {
  EACHN(first,MatterHandle,mh,{
   if ( (mh->p->position.x-boxRange < m->position.x) && (mh->p->position.x+boxRange > m->position.x)
     && (mh->p->position.y-boxRange < m->position.y) && (mh->p->position.y+boxRange > m->position.y)
     && (mh->p->position.z-boxRange < m->position.z) && (mh->p->position.z+boxRange > m->position.z) ) {
    Remove(mh);
    delete mh;
   }
  });
 }
 void Combine( MatterHandles *input ) {
  EACHN(input->first,MatterHandle,mh,{
   input->Remove(mh);
   Append(mh);
  });
 }
 void Combine( MatterHandles *input, bool Delete ) {
  EACHN(input->first,MatterHandle,mh,{
   input->Remove(mh);
   Append(mh);
  });
  if ( Delete ) delete input;
 }
};

class Matters : public LinkedList {
public:
 Zint seed;
 Vertex top,bottom;
 Vertex greatest,least;
 Box3df bounds;
 Zp<Substance> substance;
 Matters() : LinkedList() {
  cachedEye=15.0;
 }
 // To get granular debugging we cannot use CLEARLISTRESET(Matter);
 void Clear() {
   Matter *n;
   for ( Matter *p=(Matter *) first; p; p=n ) {
    n=(Matter *)(p->next);
    delete p;
       }
   first=null;
   last=null;
   count=0;
  }
 ~Matters() {
  Clear();
 }
 void Copy( Matters *in ) {
  EACH(in->first,Matter,m) {
   Append(new Matter(m));
  }
 }
 Matter *Any() {
  if ( count==0 ) return null;
  return (Matter *) Element(upto(count*1234)%count);
 }
 Matter *Any( int seed ) {
  if ( count == 0 ) return null;
  Random *set=randoms.GetSet(seed+666);
  return (Matter *) Element((int)((float)count*2.0f*set->unique(seed,seed+1,seed+2))%count);
 }
 Matter *Any( int seed, MatterHandles *excluding ) {
  int tries=0;
  Matter *candidate=(Matter *) this->Any(seed);
  while ( excluding->Contains(candidate) && tries < 10 ) {
   tries++;
   candidate=Any(seed+tries);
  }
  return candidate;
 }
 void BinaryWriteData( BinaryFile *open ) {
  int temp=(substance!=null?1:0);
  open->write(&temp);
  if ( substance ) open->scrawl(string(substance->material->name));
 }
 void BinaryReadData( BinaryFile *open ) {
  int temp; open->read(&temp);
  if ( temp == 1 ) {
   string match; open->read(&match);
   substance=substances.findByMaterial((char *)match.c_str());
  }
 }
 void ReadAppend( BinaryFile *open ) { Append(new Matter(open)); }
 string toString() {
  string out=string("");
  FOREACH(Matter,matter) out+=indention+string("matter {\n")+matter->toString()
   +indention+(matter->next != null ? string("}\n"):string("}\n"));
  return out;
 }
 SKEYSTART
  SKEY("matter", { SKEYLIST(Matter); })
 SKEYEND("Matters");
 void Render() {
  //substance->material->Apply();
  glColor4f(1.0f,1.0f,1.0f,1.0f); // added here because we have disabled per-vertex color
  FOREACH(Matter,matter) matter->Render();
  //substance->material->Unapply();
 }
 void RenderDetail() {
  int i=seed; if ( i == 0 ) i=1;
  FOREACH(Matter,matter) {
   GLImage *tex=matter_detail_textures.Any(i);
   glBindTexture(GL_TEXTURE_2D,tex->texture);
   matter->RenderDetail();
   i++;
  }
 }
 Matter *RenderColorCodedTest( LookAtPerspective *lap, Vertexd *scale, Vertexd *rotation, int tx, int ty, FBOColorDepthStencil *test ) {
  test->background.Int(255,0,255,0);
  test->blend=transparency;
  test->Bind();
  glPush();
  test->StartStenciling();
  glDisable (GL_BLEND);
  glDisable (GL_DITHER);
  glDisable (GL_FOG);
  glDisable (GL_LIGHTING);
  glDisable (GL_TEXTURE_1D);
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_TEXTURE_3D);
  glShadeModel (GL_FLAT);
  lap->Apply();
  if ( scale ) glScaled(scale->x,scale->y,scale->z);
  if ( rotation ) {
   glRotated(rotation->x,1.0,0.0,0.0);
   glRotated(rotation->y,0.0,0.0,1.0);
   glRotated(rotation->z,0.0,1.0,0.0);
  }
  int i=0;
  FOREACH(Matter,matter) {
   glColor3ub((GLubyte)i,(GLubyte)i,(GLubyte)i);
   i++;
   matter->RenderColorCoded();
  }
  test->StopStenciling();
  glPop();
  glFinish();
  test->Unbind();
  test->BindWithoutClearing();
  test->ReadPixel(gl_color0,tx,ty);
  test->Unbind();
  if ( !( test->ub_bgra[0] == 255 && test->ub_bgra[1] == 0 && test->ub_bgra[2] == 255 && test->ub_bgra[3] == 0 ) )
   return (Matter *) Element((int)test->ub_bgra[0]);
  return null;
 }
 // Test AABB before this, re-renders onto the provided FBO
 // using method similar to RenderColorCodedTest, but 3d style
 // similar to Render(), onto target FBO
 void PrepareColorCodedProfile( LookAtPerspective *lap, Vertexd *scale, Vertexd *rotation, FBOColorDepthStencil *test ) {   
  test->background.Int(255,0,255,0);
  test->blend=transparency;
  test->Bind();
  glPush();
  test->StartStenciling();
  glDisable (GL_BLEND);
  glDisable (GL_DITHER);
  glDisable (GL_FOG);
  glDisable (GL_LIGHTING);
  glDisable (GL_TEXTURE_1D);
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_TEXTURE_3D);
  glShadeModel (GL_FLAT);
  lap->Apply();
  glRotated(rotation->x,1.0,0.0,0.0);
  glRotated(rotation->y,0.0,0.0,1.0);
  glRotated(rotation->z,0.0,1.0,0.0);
  int i=0;
  FOREACH(Matter,matter) {
//   double value=iratiod(i,25);
   glColor3ub((GLubyte)i,(GLubyte)i,(GLubyte)i);
   i++;
   matter->RenderColorCoded();
  }
  test->StopStenciling();
  glPop();
  test->Unbind();
  glFinish();
 }
 // Called after PrepareColorCodedProfile, this function tests a single point on that profile, do this once for each object
 // we are colliding, too many = low performance
 bool CollidingColorCoded( Matter **result, double sx, double sy, FBOColorDepthStencil *test ) {
  test->BindWithoutClearing();
  test->ReadPixel(gl_color0,(int)(sx*(double)test->w),(int)(sy*(double)test->h));
  test->Unbind();
  Crayon litmus;
  litmus.Int((int)test->ub_bgra[0],(int)test->ub_bgra[1],(int)test->ub_bgra[2],(int)test->ub_bgra[3]);
  if ( !litmus.Equals(&test->background) ) {
   *result= (Matter *) Element((int)test->ub_bgra[0]); //(iratiod((int)test->ub_bgra[2],255)*(double)25.0));
   return *result != null;
  }
  *result= null;
  return false;
 }
 // Test AABB before this, assumes it has already been rendered, uses the FBO background to test for collision
 bool CollidingColor( int sx, int sy, FBOColorDepthStencil *test ) {
  test->BindWithoutClearing();
  test->ReadPixel(gl_color0,sx,sy);
  test->Unbind();
  Crayon litmus;
  litmus.Int((int)test->ub_bgra[0],(int)test->ub_bgra[1],(int)test->ub_bgra[2],(int)test->ub_bgra[3]);
  return !litmus.Equals(&test->background);
 }
 // Test AABB before this, assumes it has already been rendered
 bool CollidingStencil( int sx, int sy, FBOColorDepthStencil *test ) {
  test->BindWithoutClearing();
  test->ReadPixel(gl_depthstencil,sx,sy);
  test->Unbind();
  return ( test->r_stencil == 1 );
 }
 // Test AABB before this, assumes it has already been rendered
 int CollidingDepth( int sx, int sy, FBOColorDepthStencil *test ) {
  test->BindWithoutClearing();
  test->ReadPixel(gl_depthstencil,sx,sy);
  test->Unbind();
  if ( test->r_stencil == 0 ) return -666; // not colliding
  return (int) ( test->r_depth );
 }
 // Where viewing could be 0:side, 1:top, 2:side
 void RenderView2d( ViewModes viewing, int x, int y, int x2, int y2 ){
  LookAtPerspective camera;  
  camera.Near=0.0001;
  camera.Far=60.0;
  camera.halfFOV=55.0;
  camera.eye.Set( 0, 0, 10 );
  camera.center.Set( 0, 0, -50 );
  camera.ortho=true;
  camera.extents.SetCorners(-5.0,5.0,-5.0,5.0);
  // Draw a 2d representation of the primitives.
  fbo512d.background.Pick(black);
  fbo512d.Bind();
  glPush();
  camera.Apply();
  glMatrixMode(GL_MODELVIEW);
  switch ( viewing ) { case LEFT: glRotated(90.0,1.0,0.0,0.0); break; case TOP: /* do nothing */ break; }
//  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(2);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  Render();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_LINE_SMOOTH);
  glPop();
  fbo512d.Unbind();
  fbo512d.blend=additive;
  fbo512d.Render(x,y,x2,y2);
  glFinish();
 }
 void RenderSide( GLWindow *win, int x, int y, int w, int h, Crayon colorLine, bool vertexes ) {
//  FOREACH(Matter,s) s->model.RenderSide(win,x,y,w,h,colorLine,vertexes);
 }
 Matter *Add( Primitive3d *p3d, Substance *s ) {
  Matter *m=new Matter(p3d,s);
  Append(m);
  return m;
 }
 float RoughVolume() {
  float rv=0.0f;
  FOREACH(Matter,m) rv+=m->RoughVolume();
  return rv;
 }
 float Value() {
  float fiscal_value=0.0f;
  FOREACH(Matter,s) {
   if ( s->substance ) fiscal_value+=s->RoughVolume()*s->substance->Value;
  }
  return fiscal_value;
 }
 Matter *findSmallestByScale() {
  Matter *candidate=null;
  FOREACH(Matter,m) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.LessThan(&candidate->scale) ) candidate=m;
  }
  return candidate;
 }
 Matter *findLargestByScale() {
  Matter *candidate=null;
  FOREACH(Matter,m) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.GreaterThan(&candidate->scale) ) candidate=m;
  }
  return candidate;
 }
 Matter *findSmallestByScaleMagnitude() {
  Matter *candidate=null;
  FOREACH(Matter,m) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.x+m->scale.y+m->scale.z <
    candidate->scale.x+candidate->scale.y+candidate->scale.z )
    candidate=m;
  }
  return candidate;
 }
 Matter *findLargestByScaleMagnitude() {
  Matter *candidate=null;
  FOREACH(Matter,m) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.x+m->scale.y+m->scale.z >
    candidate->scale.x+candidate->scale.y+candidate->scale.z )
    candidate=m;
  }
  return candidate;
 }
 Matter *findGreatestPositionX() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.x > candidate->position.x )
    candidate=m;
  return candidate;
 }
 Matter *findGreatestPositionY() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.y > candidate->position.y  )
    candidate=m;
  return candidate;
 }
 Matter *findGreatestPositionZ() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.z > candidate->position.z )
    candidate=m;
  return candidate;
 }
 Matter *findLeastPositionX() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.x < candidate->position.x )
    candidate=m;
  return candidate;
 }
 Matter *findLeastPositionY() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.y < candidate->position.y )
    candidate=m;
  return candidate;
 }
 Matter *findLeastPositionZ() {
  Matter *candidate=null;
  FOREACH(Matter,m)
   if ( !candidate ) candidate=m;
   else if ( m->position.z < candidate->position.z )
    candidate=m;
  return candidate;
 }
 Matter *findSmallestByScale( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.LessThan(&candidate->scale) ) candidate=m;
  }
  return candidate;
 }
 Matter *findLargestByScale( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.GreaterThan(&candidate->scale) ) candidate=m;
  }
  return candidate;
 }
 Matter *findSmallestByScaleMagnitude( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.x+m->scale.y+m->scale.z <
    candidate->scale.x+candidate->scale.y+candidate->scale.z )
    candidate=m;
  }
  return candidate;
 }
 Matter *findLargestByScaleMagnitude( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->scale.x+m->scale.y+m->scale.z >
    candidate->scale.x+candidate->scale.y+candidate->scale.z )
    candidate=m;
  }
  return candidate;
 }
 Matter *findGreatestPositionX( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.x > candidate->position.x ) candidate=m;
  }
  return candidate;
 }
 Matter *findGreatestPositionY( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.y > candidate->position.y  ) candidate=m;
  }
  return candidate;
 }
 Matter *findGreatestPositionZ( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.z > candidate->position.z ) candidate=m;
  }
  return candidate;
 }
 Matter *findLeastPositionX( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.x < candidate->position.x ) candidate=m;
  }
  return candidate;
 }
 Matter *findLeastPositionY( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.y < candidate->position.y ) candidate=m;
  }
  return candidate;
 }
 Matter *findLeastPositionZ( MatterHandles *excluding ) {
  Matter *candidate=null;
  FOREACH(Matter,m) if ( !excluding->Contains(m) ) {
   if ( !candidate ) candidate=m;
   else if ( m->position.z < candidate->position.z ) candidate=m;
  }
  return candidate;
 }
 // Calculate the bounding box at default rotation
 void calculateBoundingBox() {
  FOREACH(Matter,m) {
   if ( m->ply ) {
    m->ply->model.polygons.Greatests(&greatest);
    m->ply->model.polygons.Leasts(&least);
   } else if ( m->p3d ) {
    m->p3d->model.polygons.Greatests(&greatest);
    m->p3d->model.polygons.Leasts(&least);
   } else continue;
   // Scale
   greatest.Scale((float)m->scale.x,(float)m->scale.y,(float)m->scale.z);
   greatest.Rotate((float)m->rotation.x,(float)m->rotation.y,(float)m->rotation.z);
   greatest.Add((float)m->position.x,(float)m->position.y,(float)m->position.z);
   least.Scale((float)m->scale.x,(float)m->scale.y,(float)m->scale.z);
   least.Rotate((float)m->rotation.x,(float)m->rotation.y,(float)m->rotation.z);
   least.Add((float)m->position.x,(float)m->position.y,(float)m->position.z);
   if ( top.x < greatest.x ) top.x=greatest.x;
   if ( top.x < least.x ) top.x=least.x;
   if ( top.y < greatest.y ) top.y=greatest.y;
   if ( top.y < least.y ) top.y=least.y;
   if ( top.z < greatest.z ) top.z=greatest.z;
   if ( top.z < least.z ) top.z=least.z;
   if ( bottom.x > greatest.x ) bottom.x=greatest.x;
   if ( bottom.x > least.x ) bottom.x=least.x;
   if ( bottom.y > greatest.y ) bottom.y=greatest.y;
   if ( bottom.y > least.y ) bottom.y=least.y;
   if ( bottom.z > greatest.z ) bottom.z=greatest.z;
   if ( bottom.z > least.z ) bottom.z=least.z;
  }
  // Calculate the box information from the top and bottom points
  bounds.x=bottom.x;
  bounds.y=bottom.y;
  bounds.z=bottom.z;
  bounds.x2=top.x;
  bounds.y2=top.y;
  bounds.z2=top.z;
 }
 double Within3d( Vertex *test, double box ) {
  return
    ( test->x < 0 ? test->x > -box : test->x < box )
  &&( test->y < 0 ? test->y > -box : test->y < box )
  &&( test->z < 0 ? test->z > -box : test->z < box )
  ;
 }
 // Calculates the Matter's "Best minimum eye distance" based on the assumption that
 // no vertex may be greater than 3.5 in the default settings of "lap"
 // This is used when drawing a 3d closeup of a "Matters"
 // r_max: maximum "sphere radius" calculate by viewing parameters
 // eye_factor: generalized eye factor at sphere radius (usually 15)
 double cachedEye;
 double eyeDistance( double halfFOV ) {
  float boundingRadius=0.0f;
  FOREACH(Matter,m) {
   if ( m->ply ) {
    m->ply->model.polygons.Greatests(&greatest);
    m->ply->model.polygons.Leasts(&least);
   } else if ( m->p3d ) {
    m->p3d->model.polygons.Greatests(&greatest);
    m->p3d->model.polygons.Leasts(&least);
   } else continue;
   greatest.Scale((float)m->scale.x,(float)m->scale.y,(float)m->scale.z);
   greatest.Rotate((float)m->rotation.x,(float)m->rotation.y,(float)m->rotation.z);
   greatest.Add((float)m->position.x,(float)m->position.y,(float)m->position.z);
   least.Scale((float)m->scale.x,(float)m->scale.y,(float)m->scale.z);
   least.Rotate((float)m->rotation.x,(float)m->rotation.y,(float)m->rotation.z);
   least.Add((float)m->position.x,(float)m->position.y,(float)m->position.z);
   float d1=fdistance(0.0f,0.0f,0.0f,greatest.x,greatest.y,greatest.z);
   float d2=fdistance(0.0f,0.0f,0.0f,least.x,least.y,least.z);
   if ( d1<d2 ) d1=d2;
   if ( d1 > boundingRadius ) boundingRadius=d1;
   /*
   ImmediateModel *model=m->ply?&m->ply->model:(m->p3d?&m->p3d->model:null);
   if ( !model ) continue;
   EACH(model->polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
    Vertex temp; temp.Set(v);
    temp.Scale((float)m->scale.x,(float)m->scale.y,(float)m->scale.z);
    temp.Rotate((float)m->rotation.x,(float)m->rotation.y,(float)m->rotation.z);
    temp.Add((float)m->position.x,(float)m->position.y,(float)m->position.z);
    float d=fdistance(0.0f,0.0f,0.0f,temp.x,temp.y,temp.z);
    if ( d > boundingRadius ) boundingRadius=d;
   }
   */
  }
  // Using the FBOBoxCameraTest, we discover that 3.5 is the object's maximum viewing radius, so calculate with this value.
  double result=abs(sin(90.0-halfFOV)*((double)boundingRadius*20.0/sin(halfFOV)));
  return (cachedEye=(result<15.0?15.0:result));
 }
};

HANDLED(Matters,MattersHandle,MattersHandles,MattersHandlesHandle,MattersHandlesHandles);

extern Matters emptyMatters;