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
#include "LinkedList.h"
#include "Vertexd.h"
#include "fx_FireAndForget.h"
#include "VBO.h"
#include "Model.h"

class CrackNode : public ListItem {
public:
 Vertexd position,step;
 Zp<CrackNode> parent;
 Zp<int> totalDepth;
 Zdouble weight;
 Zint depth;
 CrackNode( double x, double y, int *totalDepth, int depth=0  ) : ListItem() {
  position.Set(x,y);
  weight=1.0;
  this->depth=depth+1;
  this->totalDepth=totalDepth;
  if ( depth > *totalDepth ) *totalDepth=depth;
 }
 CrackNode( CrackNode *base, CrackNode *sample, double limit=5.0 ) : ListItem() {
  step.Set(sample->position.x-base->position.x,sample->position.y-base->position.y,sample->position.z-base->position.z);
  step.Limit(limit);
  position.Set(base->position.x+step.x,base->position.y+step.y,base->position.z+step.z);
  parent=base;
  weight=1.0;
  totalDepth=base->totalDepth;
  depth=base->depth+1;
  if ( depth > *totalDepth ) *totalDepth=depth;
 }
 CrackNode( double x, double y, int *totalDepth, double weight, int depth=0  ) : ListItem() {
  position.Set(x,y);
  this->weight=weight;
  this->depth=depth+1;
  this->totalDepth=totalDepth;
  if ( depth > *totalDepth ) *totalDepth=depth;
 }
 CrackNode( CrackNode *base, CrackNode *sample, double weight, double limit=5.0 ) : ListItem() {
  step.Set(sample->position.x-base->position.x,sample->position.y-base->position.y,sample->position.z-base->position.z);
  step.Limit(limit);
  position.Set(base->position.x+step.x,base->position.y+step.y,base->position.z+step.z);
  parent=base;
  this->weight=weight;
  totalDepth=base->totalDepth;
  depth=base->depth+1;
  if ( depth > *totalDepth ) *totalDepth=depth;
 }
 double D( CrackNode *other ) {
  return ddistance(other->position.x,other->position.y,position.x,position.y);
 }
 void Render2d() {
  if ( !parent ) return;
  Crayon A,B;
  A.Pick(black); A.Alpha(1.0f-iratiof(depth,*totalDepth));
  B.Pick(black); B.Alpha(1.0f-iratiof(parent->depth,*totalDepth));
  glLineWidth((float)(1.0+log(weight)*0.5));
  glEnable(GL_LINE_SMOOTH);
  DrawLine2Color(B,A,parent->position.x,parent->position.y,position.x,position.y);
  glDisable(GL_LINE_SMOOTH);
 }
};

class CrackSystem : public LinkedList {
public:
 Blends blend;
 Zdouble minD;
 CrackSystem() : LinkedList() {
  blend=transparency;
  minD=1000000.0;
 }
 CrackNode *nearest( CrackNode *p ) {
  CrackNode *nearby=null;
  minD=1000000.0;
  FOREACH(CrackNode,c) {
   double d=p->D(c);
   if ( d<minD ) {
    minD=d;
    nearby=c;
   }
  }
  return nearby;
 }
 void Update() {
  FOREACH(CrackNode,c) c->weight=1;
  for ( CrackNode *c=(CrackNode *)last; c; c=(CrackNode *)c->prev )
   if ( c->parent ) c->weight=c->parent->weight+c->weight;
 }
 void ConvertToVBOVC( VBOStaticVC *vbo, int totalDepth ) {
  vbo->Clear();
  ImmediateModel working;
  mPolygon *vertexes=new mPolygon;
  working.addPolygon(vertexes);
  FOREACH(CrackNode,c) {
   if ( !c->parent ) continue;
   Crayon A,B;
   A.Pick(black); A.Alpha(1.0f-iratiof(c->depth,totalDepth));
   B.Pick(black); B.Alpha(1.0f-iratiof(c->parent->depth,totalDepth));
   Vertex *m=new Vertex;
   m->x=(float)c->parent->position.x;
   m->y=(float)c->parent->position.y;
   m->z=(float)c->parent->position.z;
   m->rgba[0]=B.rf;
   m->rgba[1]=B.gf;
   m->rgba[2]=B.bf;
   m->rgba[3]=B.af;
   Vertex *n=new Vertex;
   n->x=(float)c->position.x;
   n->y=(float)c->position.y;
   n->z=(float)c->position.z;
   n->rgba[0]=A.rf;
   n->rgba[1]=A.gf;
   n->rgba[2]=A.bf;
   n->rgba[3]=A.af;
   vertexes->addVertex(m);
   vertexes->addVertex(n);
  }
  vbo->fromModel(&working);
  vbo->Create();
 }
 void Render2d() {
  Blending(blend);
  FOREACH(CrackNode,c) c->Render2d();
 }
 void Render3d() {
 }
 CLEARLISTRESET(CrackNode);
};
 
class Crack2d : public FAFProce55or {
public:
 CrackSystem nodes;
 Zint totalDepth;
 Zdouble minimumDistance;
 Crack2d() : FAFProce55or() {
  duration=10.0f;
  minimumDistance=10.0;
 }
 bool Done() {
  return false;
 }
 void Grow( int gx, int gy ) { Grow((double)gx,(double)gy); }
 void Grow( double gx, double gy ) {
  double nx,ny;
  do {
   nx=double_range(0,80.0)-40.0;
   ny=double_range(0,80.0)-40.0;
  } while ( SQ(nx) + SQ(ny) > SQ(40) );
  nx+=gx;
  ny+=gy;
  CrackNode *sample = new CrackNode(nx,ny,&totalDepth.value);
  CrackNode *base = nodes.nearest(sample);
  if ( !base ) {
   nodes.Append(sample);
   return;
  } else if ( nodes.minD < minimumDistance ) return;
  CrackNode *node = new CrackNode(base,sample);
  nodes.Append(node);
  delete sample;
  nodes.Update();
 } 
 void Grow( int gx, int gy, double length ) { Grow((double)gx,(double)gy,length); }
 void Grow( double gx, double gy, double length ) {
  double nx,ny;
  do {
   nx=double_range(0,length*2.0)-length;
   ny=double_range(0,length*2.0)-length;
  } while ( SQ(nx) + SQ(ny) > SQ(length) );
  nx+=(double)gx;
  ny+=(double)gy;
  CrackNode *sample = new CrackNode(nx,ny,&totalDepth.value);
  CrackNode *base = nodes.nearest(sample);
  if ( !base ) {
   nodes.Append(sample);
   return;
  } else if ( nodes.minD < minimumDistance ) return;
  CrackNode *node = new CrackNode(base,sample);
  nodes.Append(node);
  delete sample;
  nodes.Update();
 }
 void Draw( GLWindow *surface ) { nodes.Render2d(); }
 void Render( GLWindow *surface ) { nodes.Render2d(); }
 void Render() {
  nodes.Render2d();
 }
};

class Crack2dTest : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 Zp<Crack2d> c2d;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  c2d=new Crack2d;
  Add(c2d);
  Init();
 }
 void Repopulate() {
  fx.Clear();
  c2d=new Crack2d;
  Add(c2d);
  Init();
 }
 void Between() {
  if ( input->left ) {
   c2d->Grow(input->mxi,input->myi);
  }
 }
 void PostProcessing() {
  FORMAT(buf,25,"%d", (int) c2d->totalDepth);
  glColor3d(1.0,1.0,1.0);
  Blending(none);
  Text(buf,10,10,8,8,false);
 }
};