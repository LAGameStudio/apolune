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
#include "fx_FireAndForget.h"
#include "Trigger.h"
#include "Curve2d.h"
#include "Tweens.h"
#include "BloomShader.h"
#include "ConvolverShader.h"
#include "DreamBlurShader.h"
#include "FrostedShader.h"
#include "Crack2d.h"

class LineTree;
class LineTreeNodes;
class LineTreeNode : public ListItem {
public:
 Zint depth,maxDepth,maxBranches;
 Zdouble tParent;
 Line line;
 Zdisposable<LineTreeNodes> branches;
 Zp<LineTree> tree;
 Zp<LineTreeNode> parent;
 Zbool root;
 Zdouble R,branchFrequency,diminishing;
 Zdouble rads,limitAngle;
 LineTreeNode(
  LineTree *tree, int depth, LineTreeNode *base,
  double t, double R,
  double rads, double limitAngle,
  int maxBranches, double branchFrequency,
  double diminishing, int maxDepth
 );
 LineTreeNode( LineTreeNode *source, LineTree *cloned, LineTreeNode *parent );
 void Grow();
};

class LineTreeNodes : public LinkedList {
public:
 CLEARLISTRESET(LineTreeNode);
};

class LineTree : public ListItem {
public:
 LineTreeNodes roots;
 void Root( Line *segment, double rads, double limitAngle, double R, int maxBranches, double branchFrequency, double diminishing, int maxDepth ) {
  if ( maxDepth > 20 ) maxDepth=20;
  LineTreeNode *sample=new LineTreeNode(this,0,null,uniform(),R,rads,limitAngle,maxBranches,branchFrequency,diminishing,maxDepth);
  sample->line.Set(segment);
  roots.Append(sample);
  sample->root=true;
  sample->Grow();
 }
 virtual void OnTraverse( LineTreeNode *node ) {}
 virtual void OnMoveByTraverse( LineTreeNode *node, double x, double y ) {
  node->line.MoveBy(x,y);
 }
 virtual void OnTraverse( LineTreeNode *node, int mode ) {}
 virtual void OnTraverse( LineTreeNode *node, bool rendering ) {}
 virtual void OnClone( LineTreeNode *original, LineTreeNode *clone ) {}
 // Recursive ////////////////// start
 void Traversing( LineTreeNode *node, int mode ) {
  EACH(node->branches->first,LineTreeNode,branch) {
   OnTraverse(branch,mode);
   Traversing(branch,mode);
  }
 }
 void Traverse( int mode ) { EACH(roots.first,LineTreeNode,node) Traversing(node); }
 void Traversing( LineTreeNode *node, bool rendering ) {
  EACH(node->branches->first,LineTreeNode,branch) {
   OnTraverse(branch,rendering);
   Traversing(branch,rendering);
  }
 }
 void Traverse( bool rendering ) { EACH(roots.first,LineTreeNode,node) Traversing(node,rendering); }
 void Traversing( LineTreeNode *node ) {
  EACH(node->branches->first,LineTreeNode,branch) {
   OnTraverse(branch);
   Traversing(branch);
  }
 }
 void Traverse() { 
  EACH(roots.first,LineTreeNode,node) Traversing(node);
 }
 void MoveByTraversing( LineTreeNode *node, double x, double y ) {
  EACH(node->branches->first,LineTreeNode,branch) {
   OnMoveByTraverse(branch,x,y);
   MoveByTraversing(branch,x,y);
  }
 }
 void MoveBy( double x, double y ) { 
  EACH(roots.first,LineTreeNode,node) MoveByTraversing(node,x,y);
 }
 void Pruning( LineTreeNode *node, int pruneDepth ) {
  EACHN(node->branches->first,LineTreeNode,branch,{
   if ( branch->depth >= pruneDepth ) {
    node->branches->Remove(branch);
    delete branch;
   } else
   Pruning(branch,pruneDepth);
  });
 }
 void Prune( int pruneDepth ) { EACH(roots.first,LineTreeNode,node) Pruning(node,pruneDepth); }
 LineTreeNode *Cloning( LineTree *out, LineTreeNode *parent, LineTreeNode *source ) {
  LineTreeNode *clone=new LineTreeNode(source,out,parent);
  EACH(source->branches->first,LineTreeNode,branch) {
   LineTreeNode *newbranch=Cloning(out,clone,branch);
   clone->branches->Append(newbranch);
   OnClone(branch,newbranch);
  }
  OnClone(source,clone);
  return clone;
 }
 void Clone( LineTree *out ) {
  EACH(roots.first,LineTreeNode,node) {
   out->roots.Append(Cloning(out,null,node)); 
  }
 }
 int Counting( LineTreeNode *node, int tally ) {
  int total=0;
  EACH(node->branches->first,LineTreeNode,branch) total+=Counting(branch,tally);
 }
 int CountNodes() {
  int total=0;
  EACH(roots.first,LineTreeNode,node) total=Counting(node,0);
  return total;
 }
 void Clear() {
  roots.Clear();
 }
 // Recursive ////////////////// end
};

class LineTrees : public LinkedList {
public:
 CLEARLISTRESET(LineTree);
};


class MovingPoint2d : public ListItem {
public:
 Vertexd center;
 Vertexd actual;
 Zdouble skew,scale;
 Zdouble skewDelta;
 Zp<Tween> skewer;
 Zdouble skewerScale;
 Crayon core,diffuse;
 MovingPoint2d( double x, double y ) : ListItem() {
  center.Set(x,y);
  skewDelta=double_range(0.0,TWO_PI);
  skewer=tweens.randomNot1();
  scale=uniform()*60.0;
  Recalculate(0.0);
  core.Pick(alabaster);
  diffuse.Pick(red255);
  skewerScale=0.1;
 }
 MovingPoint2d( double x, double y, double scaleLow, double scaleHigh ) : ListItem() {
  center.Set(x,y);
  skewDelta=double_range(0.0,TWO_PI);
  skewer=tweens.randomNot1();
  scale=double_range(scaleLow,scaleHigh);
  Recalculate(0.0);
  core.Pick(alabaster);
  diffuse.Pick(red255);
  skewerScale=0.1;
 }
 void Recalculate( double expired ) {
  skew+=skewDelta;
  actual.Set(center.x,center.y);
  actual.x+=skewer->tweend(expired*skewerScale,true)*cos(skew)*scale;
  actual.y+=skewer->tweend(expired*skewerScale,true)*sin(skew)*scale;
 }
 void MoveBy(double x, double y) {
  center.Add(x,y);
  actual.Add(x,y);
 }
};

class MovingPoints2d : public LinkedList {
public:
 Blends blend;
 Zint core,diffusion;
 Line segment;
 MovingPoints2d() : LinkedList() {
  core=4;
  diffusion=32;
  blend=additive;
 }
 CLEARLISTRESET(MovingPoint2d);
 void MoveBy( double x, double y ) {
  FOREACH(MovingPoint2d,m) m->MoveBy(x,y);
 }
 void Add( Blends blend, Vertices *in, Crayon A, Crayon B ) {
  this->blend=blend;
  double d=1.0/(double)in->count;
  double t=0.0;
  EACH(in->first,Vertex,v) {
  }
 }
 void Add( Blends blend, int precision, Line *line, Crayon A, Crayon B, int transitionSize=0 ) {
  if ( precision < 1 ) precision=2;
  this->blend=blend;
  double d=1.0/(double)precision;
  double e=1.0-d;
  int mid=precision/2;
  int startTransition=mid-transitionSize;
  int endTransition=mid+transitionSize;
  int transitionWidth=transitionSize*2;
  int i=0;
  MovingPoint2d *m2=null;
  segment.Set(line);
  for ( double t=0.0; t<e; t+=d ) {
   Vertexd a,b;
   if ( i == 0 ) a.Set(line->x,line->y);
   else line->Point(t,&a);
   MovingPoint2d *m1=new MovingPoint2d(a.x,a.y);
   if ( i == 0 ) {
    if ( blend == transparency ) {
     m1->core.Alpha(0.0);
     m1->diffuse.Alpha(0.0);
    }
    else {
     m1->core.Pick(black);
     m1->diffuse.Pick(black);
    }
   }
   i++;
   line->Point(t+d,&b);
   m2=new MovingPoint2d(b.x,b.y);
   i++;
   Append(m1);
   Append(m2);
  }
  MovingPoint2d *m3=new MovingPoint2d(line->x2,line->y2,0.0,0.0);
  if ( blend == transparency ) {
   m3->core.Alpha(0.0);
   m3->diffuse.Alpha(0.0);
  } else {
   m3->core.Pick(black);
   m3->diffuse.Pick(black);
  }
  Append(m3);
 }

 void Add(
  Blends blend, int precision, Line *line,
  Crayon A, Crayon B,
  double scaleL, double scaleH,
  bool staticStart, bool staticEnd,
  int transitionSize=0
  ) {
  if ( precision < 1 ) precision=2;
  this->blend=blend;
  double d=1.0/(double)precision;
  double e=1.0-d;
  int mid=precision/2;
  int startTransition=mid-transitionSize;
  int endTransition=mid+transitionSize;
  int transitionWidth=transitionSize*2;
  int i=0;
  segment.Set(line);
  MovingPoint2d *m2=null;
  for ( double t=0.0; t<e; t+=d ) {
   Vertexd a,b;
   if ( i == 0 ) a.Set(line->x,line->y);
   else line->Point(t,&a);
   MovingPoint2d *m1=new MovingPoint2d(
    a.x,a.y,
    i==0 && staticStart ? 0.0 : scaleL,
    i==0 && staticStart ? 0.0 : scaleH
   );
   if ( i == 0 ) {
    if ( blend == transparency ) {
     m1->core.Alpha(0.0);
     m1->diffuse.Alpha(0.0);
    }
    else {
     m1->core.Pick(black);
     m1->diffuse.Pick(black);
    }
   }
   i++;
   line->Point(t+d,&b);
   m2=new MovingPoint2d(
    b.x,b.y,
    scaleL,
    scaleH
   );
   i++;
   Append(m1);
   Append(m2);
  }
  MovingPoint2d *m3=new MovingPoint2d(line->x2,line->y2,0.0,0.0);
  if ( blend == transparency ) {
   m3->core.Alpha(0.0);
   m3->diffuse.Alpha(0.0);
  } else {
   m3->core.Pick(black);
   m3->diffuse.Pick(black);
  }
  Append(m3);
 }
 Zdouble expired;
 void Between() {
  expired+=FRAMETIME;
  FOREACH(MovingPoint2d,m) m->Recalculate(expired);
 }
 void Between( double expired ) {
  this->expired=expired;
  FOREACH(MovingPoint2d,m) m->Recalculate(expired);
 }
 void Render() {
  Blending(blend);
  glEnable(GL_LINE_SMOOTH);
  FOREACH(MovingPoint2d,m) {
   if ( !m->next ) {
    Crayon color(blend==transparency?(clear):(black));
    glLineWidth(diffusion);
    DrawLine2Color(m->diffuse,color,m->actual.x,m->actual.y,(double)segment.x2,(double)segment.y2);
    glLineWidth(core);
    DrawLine2Color(m->core,color,m->actual.x,m->actual.y,(double)segment.x2,(double)segment.y2);
    break;
   }
   MovingPoint2d *m2=(MovingPoint2d *)m->next;
   if ( !m->prev ) {
    Crayon color(blend==transparency?(clear):(black));
    glLineWidth(diffusion);
    DrawLine2Color(color,m->diffuse,(double)segment.x,(double)segment.y,m->actual.x,m->actual.y);
    glLineWidth(core);
    DrawLine2Color(color,m->core,(double)segment.x,(double)segment.y,m->actual.x,m->actual.y);
   }
   glLineWidth(diffusion);
   DrawLine2Color(m->diffuse,m2->diffuse,m->actual.x,m->actual.y,m2->actual.x,m2->actual.y);
   glLineWidth(core);
   DrawLine2Color(m->core,m2->core,m->actual.x,m->actual.y,m2->actual.x,m2->actual.y);
  }
  glDisable(GL_LINE_SMOOTH);
  Blending(none);
 }
};

class LightningCurve2d : public ListItem {
public:
};

class LightningCircuit2d : public ListItem {
public:
 Zp<Tween> Tq1,Tq3,Th;
 Zdouble Tq1Scale,Tq3Scale,ThScale;
 Zdouble scaleq1,scaleq3,scaleh;
 Zdouble skewq1d,skewq3d,skewhd; // deltas
 Zdouble skewq1,skewq3,skewh; // angles
 Vertexd ra,rb,da,db;
 Zdouble expired;
 Crayon tintA,tintB,diffuseA,diffuseB,A,B;
 Zint core,diffusion;
 Zbool twigs;
 Zdouble twigLength;
 Zbool trace;
 LightningCircuit2d() : ListItem() {
  Tq1=tweens.randomNot1();
  Tq3=tweens.randomNot1();
  Th=tweens.randomNot1();
  skewq1d=double_range(0.0,TWO_PI);
  skewq3d=double_range(0.0,TWO_PI);
  skewhd=double_range(0.0,TWO_PI);
  scaleq1=30.0*uniform();
  scaleq3=30.0*uniform();
  scaleh=30.0*uniform();
  tintA.Pick(alabaster);
  tintB.Pick(alabaster);
  diffuseA.Pick(cyan);
  diffuseB.Pick(blue);
  A.Pick(red255);
  B.Pick(green255);
  core=1;
  diffusion=32;
  twigLength=10.0;
  ThScale=0.1;
  Tq1Scale=0.1;
  Tq3Scale=0.1;
  trace=true;
 }
 void Randomize( double sq1, double sq3, double sh ) {
  Tq1=tweens.randomNot1();
  Tq3=tweens.randomNot1();
  Th=tweens.randomNot1();
  skewq1d=double_range(0.0,TWO_PI);
  skewq3d=double_range(0.0,TWO_PI);
  skewhd=double_range(0.0,TWO_PI);
  scaleq1=sq1*uniform();
  scaleq3=sq3*uniform();
  scaleh=sh*uniform();
 }
 LightningCircuit2d( LightningCircuit2d *clone ) : ListItem() {
  Tq1=clone->Tq1;
  Tq3=clone->Tq3;
  Th=clone->Th;
  skewq1d=clone->skewq1d;
  skewq3d=clone->skewq3d;
  skewhd=clone->skewhd;
  scaleq1=clone->scaleq1;
  scaleq3=clone->scaleq3;
  scaleh=clone->scaleh;
  tintA.fromCrayon(clone->tintA);
  tintB.fromCrayon(clone->tintB);
  diffuseA.fromCrayon(clone->diffuseA);
  diffuseB.fromCrayon(clone->diffuseB);
  A.fromCrayon(clone->A);
  B.fromCrayon(clone->B);
  core=clone->core;
  diffusion=clone->diffusion;
  twigLength=clone->twigLength;
  ThScale=clone->ThScale;
  Tq1Scale=clone->Tq1Scale;
  Tq3Scale=clone->Tq3Scale;
  trace=clone->trace;
 }
 void TwigLength( double one ) {
  twigLength=ddistance(ra.x,ra.y,rb.x,rb.y)*one;
 }
 LightningCircuit2d( double scaleLow, double scaleHigh ) {
  Tq1=tweens.randomNot1();
  Tq3=tweens.randomNot1();
  Th=tweens.randomNot1();
  skewq1d=double_range(0.0,TWO_PI);
  skewq3d=double_range(0.0,TWO_PI);
  skewhd=double_range(0.0,TWO_PI);
  scaleq1=double_range(scaleLow,scaleHigh);
  scaleq3=double_range(scaleLow,scaleHigh);
  scaleh=double_range(scaleLow,scaleHigh);
  tintA.Pick(alabaster);
  tintB.Pick(alabaster);
  diffuseA.Pick(cyan);
  diffuseB.Pick(blue);
  A.Pick(red255);
  B.Pick(green255);
  core=1;
  diffusion=32;
  twigLength=10.0;
  ThScale=0.5;
  Tq1Scale=0.5;
  Tq3Scale=0.5;
 }
 Vertexd q1,h,q3;
 void Between() {
  skewq1+=skewq1d;
  skewq3+=skewq3d;
  skewhd+=skewhd;
  ra.Add(da.x,da.y);
  rb.Add(db.x,db.y);
  expired+=FRAMETIME;
  Line segment(ra.x,ra.y,rb.x,rb.y);
  segment.Point(0.25,&q1);
  segment.Point(0.5,&h);
  segment.Point(0.75,&q3);
  q1.x+=Tq1->tweend(expired*Tq1Scale,true)*cos(skewq1)*scaleq1;
  q1.y+=Tq1->tweend(expired*Tq1Scale,true)*sin(skewq1)*scaleq1;
  h.x+=Th->tweend(expired*ThScale,true)*cos(skewh)*scaleh;
  h.y+=Th->tweend(expired*ThScale,true)*sin(skewh)*scaleh;
  q3.x+=Tq3->tweend(expired*Tq3Scale,true)*cos(skewq3)*scaleq3;
  q3.y+=Tq3->tweend(expired*Tq3Scale,true)*sin(skewq3)*scaleq3;
 }
 void Between( double expired ) {
  skewq1+=skewq1d;
  skewq3+=skewq3d;
  skewhd+=skewhd;
  ra.Add(da.x,da.y);
  rb.Add(db.x,db.y);
  this->expired=expired;
  Line segment(ra.x,ra.y,rb.x,rb.y);
  segment.Point(0.25,&q1);
  segment.Point(0.5,&h);
  segment.Point(0.75,&q3);
  q1.x+=Tq1->tweend(expired*Tq1Scale,true)*cos(skewq1)*scaleq1;
  q1.y+=Tq1->tweend(expired*Tq1Scale,true)*sin(skewq1)*scaleq1;
  h.x+=Th->tweend(expired*ThScale,true)*cos(skewh)*scaleh;
  h.y+=Th->tweend(expired*ThScale,true)*sin(skewh)*scaleh;
  q3.x+=Tq3->tweend(expired*Tq3Scale,true)*cos(skewq3)*scaleq3;
  q3.y+=Tq3->tweend(expired*Tq3Scale,true)*sin(skewq3)*scaleq3;
 }
 void Render() {
  Blending(additive);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(core);
  DrawLine2Color(crayons.Pick(black),tintA,ra.x,ra.y,q1.x,q1.y);
  DrawLine2Color(tintA,tintB,q1.x,q1.y,h.x,h.y);
  DrawLine2Color(tintB,tintB,h.x,h.y,q3.x,q3.y);
  DrawLine2Color(tintB,crayons.Pick(black),q3.x,q3.y,rb.x,rb.y);
  glLineWidth(diffusion);
  DrawLine2Color(crayons.Pick(black),diffuseA,ra.x,ra.y,q1.x,q1.y);
  DrawLine2Color(diffuseA,diffuseB,q1.x,q1.y,h.x,h.y);
  DrawLine2Color(diffuseB,diffuseB,h.x,h.y,q3.x,q3.y);
  DrawLine2Color(diffuseB,crayons.Pick(black),q3.x,q3.y,rb.x,rb.y);
  glLineWidth(1);
  glDisable(GL_LINE_SMOOTH);
  if ( trace ) Trace();
 }
 void Trace() {
  DrawLine2Color(A,B,ra.x,ra.y,q1.x,q1.y);
  DrawLine2Color(A,B,q1.x,q1.y,h.x,h.y);
  DrawLine2Color(A,B,h.x,h.y,q3.x,q3.y);
  DrawLine2Color(A,B,q3.x,q3.y,rb.x,rb.y);
 }
 void MoveBy( double x, double y ) {
  ra.Add(x,y);
  rb.Add(x,y);
 }
};

class LightningSystem2d : public LinkedList {
public:
 LightningCircuit2d circuit;
 double sq1, sq3, sh; // sets lightning parameters
 Line segment;
 CLEARLISTRESET(LightningCircuit2d);
 LightningSystem2d() : LinkedList() {
  sq1=30.0;
  sq3=30.0;
  sh=30.0;
 }
 void Between( double expired ) {
  FOREACH(LightningCircuit2d,L) L->Between( expired );
 }
 void Between() {
  FOREACH(LightningCircuit2d,L) L->Between();
 }
 void Render() {
  FOREACH(LightningCircuit2d,L) L->Render();
 }
 void MoveBy( double x, double y ) {
  FOREACH(LightningCircuit2d,L) L->MoveBy(x,y);
 }
 LightningCircuit2d *Add( double x, double y, double x2, double y2, bool prepend=true ) {
  LightningCircuit2d *L=new LightningCircuit2d(&circuit);
  L->Randomize(sq1,sq3,sh);
  L->ra.Set(x,y);
  L->rb.Set(x2,y2);
  if ( prepend ) Prepend(L); else Append(L);
  L->Between();
  return L;
 }
 LightningCircuit2d *Add( Line *line, bool prepend=true ) {
  LightningCircuit2d *L=new LightningCircuit2d(&circuit);
  L->Randomize(sq1,sq3,sh);
  L->ra.Set(line->x,line->y);
  L->rb.Set(line->x2,line->y2);
  if ( prepend ) Prepend(L); else Append(L);
  L->Between();
  return L;
 }
 void Add( int N, double x, double y, double x2, double y2, bool prepend=true ) {
  double d=1.0/(double)N;
  double e=1.0-d;
  segment.Set(x,y,x2,y2);
  for ( double t=0.0; t<=e; t+=d ) {
   Vertexd a,b;
   segment.Point(t,&a);
   segment.Point(t+d,&b);
   Add( a.x,a.y,b.x,b.y, prepend );
  }
 }
 void Add( int N, double x, double y, double R, bool onCircle=false ) {
  Circle circle(x,y,R);
  Vertexd end;
  for ( int i=0; i<N; i++ ) {
   if ( onCircle ) circle.RandomPointOn(&end);
   else circle.RandomPoint(&end);
   Add( x,y,end.x,end.y );
  }
 }
 void Add( int N, double x, double y, double R, double cx, double cy, bool onCircle=false ) {
  Circle circle(x,y,R);
  Vertexd end;
  for ( int i=0; i<N; i++ ) {
   if ( onCircle ) circle.RandomPointOn(&end);
   else circle.RandomPoint(&end);
   Add( cx,cy,end.x,end.y );
  }
 }
 void Add( int N, double x, double y, double w, double h, double cx, double cy ) {
  Vertexd end;
  for ( int i=0; i<N; i++ ) {
   end.x=double_range(x,x+w);
   end.y=double_range(y,y+h);
   Add( cx,cy,end.x,end.y );
  }
 }
 void Add( bool cracking, double scale, double sx, double sy, double ex, double ey ) {
  Crack2d cracks;
  cracks.Grow(sx,sy);
  Cartesian mid;
  for ( int i=0; i<20; i++ ) {
   cracks.Grow(ex,ey);
  }
  EACH(cracks.nodes.first,CrackNode,crack) {
   if ( crack->parent )
   Add(
    crack->parent->position.x*scale,
    crack->parent->position.y*scale,
    crack->position.x*scale,
    crack->position.y*scale
   );
  }
 }
 /*
 void Add( int N, int branches, int tinies, double sx, double sy, double startLength, double startAngle ) {
  Line segment(
   sx,sy,
   sx+cos(deg2rad(startAngle))*startlength,
   sy+sin(deg2rad(startAngle))*startLength
  );
  Vertexd start;
  for ( int i=0; i<N; i++ ) {
   if ( i == 0 ) {
    Add( segment.x,segment.y,segment.x2,segment.y2 );
    for ( int j=0; j<branches; j++ ) {
    }
   } else {
   }
   segment.x=segment.x2;
   segment.y=segment.y2;
   startLength/=1.0+uniform();
  }
 }
 */
 void Replace( double x, double y, double x2, double y2 ) {
  double d=1.0/(double)count;
  segment.Set(x,y,x2,y2);
  double t=0.0;
  FOREACH(LightningCircuit2d,L) {
   segment.Point(t,&L->ra);
   segment.Point(t+d,&L->rb);
   t+=d;
  }
 }
};

class MovingLightningSystem2dTree : public LineTree {
public:
 Zp<LightningSystem2d> lightning;
 Zdisposable<MovingLightningSystem2dTree> temporary;
 Zp<Tween> mover;
 Zdouble move;
 Zdouble expired,percent,delta,timeScale;
 MovingLightningSystem2dTree() : LineTree() {
  move=50.0;
  delta=0.01; 
  timeScale=0.1;
  mover=tweens.randomNot1();
 }
 // Writes to the lightning
 void OnTraverse( LineTreeNode *node ) {
  lightning->Add(node->line.x,node->line.y,node->line.x2,node->line.y2);
 }
 // Cloned once per frame
 void OnClone( LineTreeNode *original, LineTreeNode *clone ) {
 }
 void Between() {
  expired+=FRAMETIME*timeScale;
  temporary.Recycle();
  temporary->lightning.pointer=lightning.pointer;
  temporary->expired=expired;
  percent=0.0;
  Clone(temporary.pointer);
  lightning->Clear();
  temporary->Traverse();
 }
 void Between( double expired ) {
  this->expired+=expired;
  temporary.Recycle();
  temporary->lightning.pointer=lightning.pointer;
  temporary->expired=expired;
  percent=0.0;
  Clone(temporary.pointer);
  lightning->Clear();
  temporary->Traverse();
 }
 // Handled elsewhere
 void Render() {}
};

class MovingLightningSystem2d : public ListItem {
public:
 LightningSystem2d lightning;
 MovingLightningSystem2dTree bolt;
 MovingPoints2d lines;
 Zdouble expired;
 Zdouble movementRadiusA,movementRadiusB;
 Zint precision,transition;
 Crayon A,B;
 Zbool bowing,staticStartPoint,staticEndPoint;
 Zbool polarity;
 Zdouble bowStrength,bowSkew;
 Zp<Tween> bowTween,bowShape;
 Zdouble bowTweenScale,bowShapeScale;
 Line segment;
 MovingLightningSystem2d() : ListItem() {
  precision=5;
  transition=2;
  movementRadiusA=30.0;
  movementRadiusB=120.0;
  A.Pick(macNcheese);
  B.Pick(skyBlue);
  bowing=true;
  bowStrength=300.0;
  bowSkew=80.0+20.0*uniform();
  bowTween=tweens.randomNot1();
  bowTweenScale=0.05;
  bowShape=tweens.randomNot1();
  bowShapeScale=1.0;
  polarity=true;
  bolt.lightning=&lightning;
  staticEndPoint=true;
  staticStartPoint=true;
 }
 void Clear() {
  lightning.Clear();
  bolt.Clear();
  lines.Clear();
 }
 void MoveBy( double x, double y ) {
  lightning.MoveBy(x,y);
  lines.MoveBy(x,y);
  bolt.MoveBy(x,y);
 }
 void Create( double x, double y, double x2, double y2 ) {
  segment.Set(x,y,x2,y2);
  lines.Add(additive,precision,&segment,A,B,movementRadiusA,movementRadiusB,staticStartPoint,staticEndPoint,transition);
 }
 void Create( double x, double y, double x2, double y2, bool bowStrengthToDoubleL ) {
  segment.Set(x,y,x2,y2);
  lines.Add(additive,precision,&segment,A,B,movementRadiusA,movementRadiusB,staticStartPoint,staticEndPoint,transition);
  bowStrength=segment.length*2.0;
 }
 void Create( Line *line, double rads, double limitAngle, double R, int maxBranches, double branchFrequency, double diminishing, int maxDepth=3 ) {
  segment.Set(line);
  bolt.Clear();
  bolt.Root(line,rads,limitAngle,R,maxBranches,branchFrequency,diminishing,maxDepth);
 }
 void Between() {
  expired+=FRAMETIME;
  lightning.Clear();
  lines.Between(expired);
  Vertexd bowFrom;
  double d=1.0/(double)lines.count;
  double t=0.0;
  if ( bowing ) EACH(lines.first,MovingPoint2d,m) {
   if ( !m->next ) {    
    lightning.Add(
     bowFrom.x,bowFrom.y,
     staticEndPoint?segment.x2:m->actual.x,staticEndPoint?segment.y2:m->actual.y
    );
    break;
   }
   MovingPoint2d *n=(MovingPoint2d *)m->next;
   if ( !m->prev ) {
    double bowingTo=(polarity?1.0:-1.0)*bowStrength
                   *bowTween->tweend(expired*bowTweenScale,true)
                   *bowShape->tweend(t,true)*bowShapeScale;
    bowFrom.Set( m->actual.x+bowingTo*cos(bowSkew),m->actual.y+bowingTo*sin(bowSkew) );
    lightning.Add(
     staticStartPoint?segment.x:m->actual.x,staticStartPoint?segment.y:m->actual.y,
     bowFrom.x,bowFrom.y
    );
   }
   double bowingTo=(polarity?1.0:-1.0)*bowStrength
                  *bowTween->tweend(expired*bowTweenScale,true)
                  *bowShape->tweend((t+d),true)*bowShapeScale;
   Vertexd bowTemp( n->actual.x+bowingTo*cos(bowSkew),n->actual.y+bowingTo*sin(bowSkew) );
   lightning.Add(
    bowFrom.x,bowFrom.y,
    bowTemp.x,bowTemp.y
   );
   bowFrom.Set(&bowTemp);
   t+=d;
  } else {
   EACH(lines.first,MovingPoint2d,m) {
    if ( !m->next ) { 
     lightning.Add(
      m->actual.x,m->actual.y,
      staticEndPoint?segment.x2:m->actual.x,staticEndPoint?segment.y2:m->actual.y
     );
     break;
    }
    MovingPoint2d *n=(MovingPoint2d *)m->next;
    if ( !m->prev ) {
     lightning.Add(
      staticStartPoint?segment.x:m->actual.x,staticStartPoint?segment.y:m->actual.y,
      n->actual.x,n->actual.y
     );
    } else {
     lightning.Add(
      m->actual.x,m->actual.y,
      n->actual.x,n->actual.y
     );
    }
    t+=d;
   }
  }
  lightning.Between(expired);
  bolt.Between();
 }
 void Render() {
  lightning.Render();
  lines.Render();
  bolt.Render();
 }
};

class GyratingLightningTree : public ListItem {
public:
 LightningSystem2d lightning;
 MovingLightningSystem2dTree bolt;
 Line segment;
 Zdouble rads,limitAngle,R,branchFrequency,diminishing;
 Zint maxDepth,maxBranches;
 Zp<Tween> radController,RController,limitAngleOscillator;
 Zdouble radScale,radOffset,RScale,ROffset,limitAngleScale,limitAngleOffset;
 Zdouble expired,timeScale;
 GyratingLightningTree() : ListItem() {
  radController=tweens.randomNot1();
  radScale=0.001;
  radOffset=QUARTER_PI/8.0;
  R=300.0;
  RScale=0.01;
  ROffset=10.0;
  timeScale=0.1;
  maxDepth=3;
  maxBranches=2;
  branchFrequency=0.75;
  diminishing=1.1;
  limitAngleOscillator=tweens.randomNot1();
  limitAngle=QUARTER_PI;
  limitAngleScale=0.1;
  limitAngleOffset=QUARTER_PI/3.0;
  RController=tweens.randomNot1();
  bolt.lightning=&lightning;
  segment.Set(display.w2d-1.0,display.h2d,display.w2d+1.0,display.h2d);
  rads=segment.LineAngle()+HALF_PI;
 }
 void Between() {
  expired+=FRAMETIME*timeScale;
  double _rads=rads+(-radOffset+2.0*radOffset*radController->tweend(expired*radScale,true));
  double _R=R+(-ROffset+2.0*ROffset*radController->tweend(expired*RScale,true));
  double _limitAngle=(-limitAngleOffset+2.0*limitAngleOffset*limitAngleOscillator->tweend(expired*limitAngleScale,true));
  bolt.Clear();
  bolt.Root(&segment,_rads,_limitAngle,_R,maxBranches,branchFrequency,diminishing,maxDepth);
  bolt.Between(expired);
 }
 void Render() {
  lightning.Render();
 }
};

class LightningCircuit2dWalker : public ListItem {
};

class LightningSystem2dTest : public GLWindow {
public:
 GyratingLightningTree lightningTree;
 MovingPoints2d curved;
 LightningSystem2d lightning;
 MovingLightningSystem2d movinglightning;
 Zp<LightningCircuit2d> L;
 Zdouble time;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 void Between() {
  time+=FRAMETIME;
  curved.Between();
  lightningTree.Between();
  lightning.Between();
  movinglightning.Between();
  DebugKeys();
 }
 Zdouble clickDelay,angle;
 void DebugKeys () {  
  if ( clickDelay > 0.0 ) clickDelay-=FRAMETIME;
  else
  if ( input->KeyDown(DX_SPACE) ) {
   lightning.Clear();
   curved.Clear();
  } else
  if ( input->left ) {
   if ( !L ) {
    L=new LightningCircuit2d;
    L->ra.x=input->mxi;
    L->ra.y=input->myi;
   } else {
    L->rb.x=input->mxi;
    L->rb.y=input->myi;
    if ( input->KeyDown(DX_LCTRL) ) {
     movinglightning.Clear();
     movinglightning.Create(L->ra.x,L->ra.y,L->rb.x,L->rb.y);
     delete L;
     L=null;
    } else if ( input->KeyDown(DX_LSHIFT) ) {
     Line line(L->ra.x,L->ra.y,L->rb.x,L->rb.y);
     movinglightning.Clear();
     movinglightning.Create(&line,line.LineAngle()+HALF_PI,QUARTER_PI,300.0,3,0.75,0.9,5);
     delete L;
     L=null;
    } else {
     lightning.Append(L);
     L->Between();
     L=null;
    }
   }
   clickDelay=0.2;
  } else
  if ( input->right ) {
   if ( !L ) {
    L=new LightningCircuit2d;
    L->ra.x=input->mxi;
    L->ra.y=input->myi;
   } else {
    L->rb.x=input->mxi;
    L->rb.y=input->myi;
    if ( input->KeyDown(DX_LSHIFT) ) lightning.Replace(L->ra.x,L->ra.y,L->rb.x,L->rb.y);
    else lightning.Add(5,L->ra.x,L->ra.y,L->rb.x,L->rb.y);
    delete L;
    L=null;
   }
   clickDelay=0.2;
  } else if ( input->middle ) {
   if ( input->KeyDown(DX_LALT) ) {
    lightningTree.segment.Set(input->mxi-1,input->myi,input->mxi+1,input->myi);
   } else {
    curved.Clear();
    Line segment(input->mxi,input->myi,display->w2-input->mxi,display->h2-input->myi);
    curved.Add( input->KeyDown(DX_LSHIFT) ? additive : transparency, 20, &segment, crayons.Any(), crayons.Any() );
    clickDelay=0.2;
   }
  } else if ( input->KeyDown(DX_1) ) {
   lightning.Add(true,10.0,input->mxi/10.0,input->myi/10.0,input->mxi/10.0,(input->myi*2)/10.0);
   clickDelay=0.2;
  } else if ( input->KeyDown(DX_2) ) {
   lightning.Add(5, input->mxi,input->myi, 100.0, 100.0, input->mxi, input->myi-75.0 );
   clickDelay=0.2;
  } else if ( input->KeyDown(DX_3) ) {
   lightning.Add(10, input->mxi, input->myi, 100.0 );
   clickDelay=0.2;
  } else if ( input->KeyDown(DX_4) ) {
   lightning.Add(10, input->mxi, input->myi, input->mxi, input->myi-75.0, 100.0 );
   clickDelay=0.2;
  } else if ( input->KeyDown(DX_5) ) {
   clickDelay=0.2;
  }
  DebugArrows();
 }
 void Render() {
  fboScreen3.background.Pick(clear);
  fboScreen3.Bind();
  lightningTree.Render();
  lightning.Render();
  movinglightning.Render();
  curved.Render();
  fboScreen3.Unbind();

 // fboScreen4.background.Pick(clear);
 // fboScreen4.Bind();
 // simplefrostShader.pTex0=&fboScreen3.texture;
 // simplefrostShader.ax=(76.0f+(float)uniform())*(float)cos(deg2rad(time));
 // simplefrostShader.ay=(50.0f+(float)uniform() )*(float)sin(deg2rad(time));
 // simplefrostShader.bx=(66.0f+(float)uniform() )*(float)cos(deg2rad(time));
 // simplefrostShader.by=(77.0f+(float)uniform())*(float)sin(deg2rad(time));
 // simplefrostShader.d=10.0f;
 // simplefrostShader.UpdateUniforms();
 // FullscreenQuadYInverted();
 // simplefrostShader.Disable();
 // fboScreen4.Unbind();

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  dreamblurrgbShader.pTex0=&fboScreen3.texture;
  art.Stretch(&dreamblurrgbShader,0.0,0.0,display->wd,display->hd);
  
  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();  
  bloomShader.a=0.0004f;
  bloomShader.b=0.12f;
  bloomShader.c=0.09f;
  bloomShader.d=0.075f;
  bloomShader.e=0.3f;
  bloomShader.f=0.5f;
  bloomShader.g=0.125f;
  bloomShader.kernel1=4;
  bloomShader.kernel2=3;
  bloomShader.pTex0=&fboScreen5.texture;
  fboScreen1.QuadYInverted(&bloomShader);
  fboScreen1.Unbind();

  fboScreen1.blend=additive;
  fboScreen1.RenderUpsideDown(0,0,display->w,display->h);

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  sigmaV9gaussianShader.pTex0=&fboScreen3.texture;
  fboScreen5.QuadYInverted(&sigmaV9gaussianShader);
  sigmaV9gaussianShader.Disable();
  fboScreen5.Unbind();

  fboScreen4.background.Pick(clear);
  fboScreen4.Bind();
  sigmaH9gaussianShader.pTex0=&fboScreen3.texture;
  fboScreen4.QuadYInverted(&sigmaH9gaussianShader);
  sigmaH9gaussianShader.Disable();
  fboScreen4.Unbind();

  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();
  fboScreen5.RenderUpsideDown(additive,0,0,fboScreen.w,fboScreen.h);
  fboScreen4.RenderUpsideDown(additive,0,0,fboScreen.w,fboScreen.h);
  fboScreen2.Unbind();

  /*
  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();  
  
  gaussianblurShader.pTex0=&fboScreen3.texture;
  gaussianblurShader.UpdateUniforms();
  FullscreenQuadYInverted();
  gaussianblurShader.Disable();
  fboScreen2.Unbind();
  */

  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();  
  bloomShader.a=0.0004f;
  bloomShader.b=0.12f;
  bloomShader.c=0.09f;
  bloomShader.d=0.075f;
  bloomShader.e=0.3f;
  bloomShader.f=0.5f;
  bloomShader.g=0.125f;
  bloomShader.kernel1=4;
  bloomShader.kernel2=3;
  bloomShader.pTex0=&fboScreen2.texture;
  fboScreen1.QuadYInverted(&bloomShader);
  fboScreen1.Unbind();

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  sigmaVgaussianShader.pTex0=&fboScreen1.texture;
  fboScreen5.QuadYInverted(&sigmaVgaussianShader);
  fboScreen5.Unbind();

  fboScreen4.background.Pick(clear);
  fboScreen4.Bind();
  sigmaHgaussianShader.pTex0=&fboScreen1.texture;
  fboScreen4.QuadYInverted(&sigmaHgaussianShader);
  fboScreen4.Unbind();

  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();
  fboScreen5.RenderUpsideDown(additive,0,0,fboScreen.w,fboScreen.h);
  fboScreen4.RenderUpsideDown(additive,0,0,fboScreen.w,fboScreen.h);
  fboScreen2.Unbind();

  fboScreen1.blend=additive;
  fboScreen1.RenderUpsideDown(0,0,display->w,display->h);
 }
 void DebugArrows() {
  Cartesiand move;
  if ( input->KeyDown(DX_LEFT) )  {
   move.x=-1.0;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   move.x=1.0;
  }
  if ( input->KeyDown(DX_UP) ) {
   move.y=-1.0;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   move.y=1.0;
  }
  if ( move.x != 0 || move.y != 0 ) {
   lightning.MoveBy(move.x,move.y);
   curved.MoveBy(move.x,move.y);
   movinglightning.MoveBy(move.x,move.y);
  }
 }
};