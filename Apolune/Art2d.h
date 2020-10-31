#pragma once
#include "ListItem.h"
#include "Display.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "VBO_VC.h"
#include "TexShader.h"
#include "Zindexed.h"
#include "Trigger.h"
#include "Interface.h"
#include "STLFile.h"
#include "Curve2d.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TwoDeeTriangles;
ONE(Rotated2dQuad,{
 tintA.Pick(alabaster);
 tintB.Pick(alabaster);
 tintC.Pick(alabaster);
 tintD.Pick(alabaster);
})
 Zdouble x,y,w,h,a;
 Zint integer;
 Cartesiand tc,tcB,tcC,tcD;
 Crayon tintA,tintB,tintC,tintD;
 KEYWORDS({
  NUMWORD("x",x)
  NUMWORD("y",y)
  NUMWORD("w",this->w)
  NUMWORD("h",h)
  NUMWORD("a",a)
  NUMWORD("i",integer)
  SUBWORD("tc",tc)
  SUBWORD("tcb",tcB)
  SUBWORD("tcc",tcC)
  SUBWORD("tcd",tcD)
  TINTWORD("tintA",tintA)
  TINTWORD("tintB",tintB)
  TINTWORD("tintC",tintC)
  TINTWORD("tintD",tintD)
 })
 WORDKEYS({
  NUMBKEY("x",x)
  NUMBKEY("y",y)
  NUMBKEY("w",w)
  NUMBKEY("h",h)
  NUMBKEY("a",a)
  NUMBKEY("i",integer)
  SSUBKEY("tc",tc)
  SSUBKEY("tcb",tcB)
  SSUBKEY("tcc",tcC)
  SSUBKEY("tcd",tcD)
  TINTKEY("tintA",tintA)
  TINTKEY("tintB",tintB)
  TINTKEY("tintC",tintC)
  TINTKEY("tintD",tintD)
 })
 CLONE(Rotated2dQuad,{
  DUPE(x);
  DUPE(y);
  DUPE(w);
  DUPE(h);
  DUPE(integer);
  a->a=this->a;
  a->tc.Set(&tc);
  a->tcB.Set(&tcB);
  a->tcC.Set(&tcC);
  a->tcD.Set(&tcD);
  tintA.fromCrayon(tintA);
  tintB.fromCrayon(tintB);
  tintC.fromCrayon(tintC);
  tintD.fromCrayon(tintD);
 })
 void toTriangles( TwoDeeTriangles *out );
MANY(Rotated2dQuad,Rotated2dQuadHandle,Rotated2dQuadHandles,"Rotated2dQuad",Rotated2dQuads,{})
 KEYSWORDSGroups("Rotated2dQuad",Rotated2dQuad)
 CLONES(Rotated2dQuad,Rotated2dQuads)
 void toTriangles( TwoDeeTriangles *out );
DONE(Rotated2dQuad);

ONE(TwoDeeTriangle,{ tintA.Pick(alabaster); tintB.Pick(alabaster); tintC.Pick(alabaster); })
 Cartesiand A,B,C; // x2,y2 are texCoords
 Zp<Cartesiand> Ap,Bp,Cp;
 Crayon tintA,tintB,tintC;
 KEYWORDS({
  SSUBWORD("A",A);  SSUBWORD("B",B);  SSUBWORD("C",C);
  TINTWORD("tintA",tintA);  TINTWORD("tintB",tintB);  TINTWORD("tintC",tintC);
 })
 WORDKEYS({
  SSUBKEY("A",A);  SSUBKEY("B",B);  SSUBKEY("C",C);
  TINTKEY("tintA",tintA);  TINTKEY("tintB",tintB);  TINTKEY("tintC",tintC);
 })
 BINARY_IN({
  B_IN_SSUB(A);  B_IN_SSUB(B);  B_IN_SSUB(C);
  B_IN_SSUB(tintA);  B_IN_SSUB(tintB);  B_IN_SSUB(tintC);
 })
 BINARY_OUT({
  B_OUT_SSUB(A);  B_OUT_SSUB(B);  B_OUT_SSUB(C);
  B_OUT_SSUB(tintA);  B_OUT_SSUB(tintB);  B_OUT_SSUB(tintC);
 })
 CLONE(TwoDeeTriangle,{
  a->A.Set(&A);  a->B.Set(&B);  a->C.Set(&C);
  DUPE(Ap);  DUPE(Bp);  DUPE(Cp);
  a->tintA.fromCrayon(tintA);  a->tintB.fromCrayon(tintB);  a->tintC.fromCrayon(tintC);
 })
MANY(TwoDeeTriangle,TwoDeeTriangleHandle,TwoDeeTriangleHandles,"2dtri",TwoDeeTriangles,{})
 void CopyInto( TwoDeeTriangles *in, double ox, double oy, double w, double h ) {
  EACH(in->first,TwoDeeTriangle,t1) {
   TwoDeeTriangle *t2=t1->Duplicate();
   t2->A.x*=w; t2->A.y*=h; t2->A.MoveBy(ox,oy); 
   t2->B.x*=w; t2->B.y*=h; t2->B.MoveBy(ox,oy); 
   t2->C.x*=w; t2->C.y*=h; t2->C.MoveBy(ox,oy); 
   Append(t2);
  }
 }
 KEYSWORDSGroups("2dtri",TwoDeeTriangle)
 BINARY_INOUT(TwoDeeTriangle)
 CLONES(TwoDeeTriangle,TwoDeeTriangles)
 void Add( double ax, double ay, double tax, double tay, double bx, double by, double tbx, double tby, double cx, double cy, double tcx, double tcy ) {
  TwoDeeTriangle *t=new TwoDeeTriangle;
  t->A.Set(ax,ay); t->A.x2=tax; t->A.y2=tay;
  t->B.Set(bx,by); t->B.x2=tbx; t->B.y2=tby;
  t->C.Set(cx,cy); t->C.x2=tcx; t->C.y2=tcy;
  Append(t);
 }
 void Add( double ax, double ay, double tax, double tay, double bx, double by, double tbx, double tby, double cx, double cy, double tcx, double tcy, double dx, double dy, double tdx, double tdy ) {
  TwoDeeTriangle *t=new TwoDeeTriangle;
  t->A.Set(ax,ay); t->A.x2=tax; t->A.y2=tay;
  t->B.Set(bx,by); t->B.x2=tbx; t->B.y2=tby;
  t->C.Set(dx,dy); t->C.x2=tdx; t->C.y2=tdy;
  Append(t);
  t=new TwoDeeTriangle;
  t->A.Set(bx,by); t->A.x2=tbx; t->A.y2=tby;
  t->B.Set(cx,cy); t->B.x2=tcx; t->B.y2=tcy;
  t->C.Set(dx,dy); t->C.x2=tdx; t->C.y2=tdy;
  Append(t);
 }
 void Add(
   Crayon tintA, double ax, double ay, double tax, double tay,
   Crayon tintB, double bx, double by, double tbx, double tby,
   Crayon tintC, double cx, double cy, double tcx, double tcy,
   Crayon tintD, double dx, double dy, double tdx, double tdy ) {
  TwoDeeTriangle *t=new TwoDeeTriangle;
  t->A.Set(ax,ay); t->A.x2=tax; t->A.y2=tay;
  t->B.Set(bx,by); t->B.x2=tbx; t->B.y2=tby;
  t->C.Set(dx,dy); t->C.x2=tdx; t->C.y2=tdy;
  t->tintA.fromCrayon(tintA);
  t->tintB.fromCrayon(tintB);
  t->tintC.fromCrayon(tintC);
  Append(t);
  t=new TwoDeeTriangle;
  t->A.Set(bx,by); t->A.x2=tbx; t->A.y2=tby;
  t->B.Set(cx,cy); t->B.x2=tcx; t->B.y2=tcy;
  t->C.Set(dx,dy); t->C.x2=tdx; t->C.y2=tdy;
  t->tintA.fromCrayon(tintB);
  t->tintB.fromCrayon(tintC);
  t->tintC.fromCrayon(tintD);
  Append(t);
 }
DONE(TwoDeeTriangle);

enum TwoDeeStrokeType {
  tds_Line=0,
  tds_Lines=1,
  tds_Curve=2,
  tds_FilledCircle=3,
  tds_Triangle=4,
  tds_Quad=5
};

ONE(TwoDeeStroke,{
 type=tds_Lines;
 addCircleAtJoint=true;
 thickness=0.05;
 precision=8;
 endPrecision=6;
 scale.Set(1.0,1.0);
 A.x2=0.0; A.y2=0.0;
 B.x2=1.0; B.y2=0.0;
 C.x2=1.0; C.y2=1.0;
 D.x2=0.0; D.y2=1.0;
})
 TwoDeeStrokeType type;
 Cartesiands lines; // connected lines where L1: x,y->x2,y2 L2: x2,y2->x3,y3
 Zbool roundEnds;
 Zint precision,endPrecision;
 Zbool startCap;   // endpoint of curve or line or lines is beginning of next stroke
 Zbool endCap;     // startpoint of curve or line or lines is end of previous stroke
 Cartesiand scale; // Scales (by expansion) output x/y values by an arbitrary setting.
 Cartesiand A,B,C,D; // Line, Quad or Triangle types, where x2,y2 is texcoord
 Circle circle;    // For Circle type, x and y, R
 Zbool texCoordsCenterToEdge,texCoordsFollowLines,addCircleAtJoint;
 Crayon tintA,tintB,tintC,tintD;
 Zdouble thickness;
 Zbool variableThickness;
 Doubles thicknesses;
 KEYWORDS({
  KEYWORD("name",{name=w;})
  ENUMWORD("type",type,TwoDeeStrokeType)
  SSUBWORD("lines",lines)
  TAGWORD("roundEnds",roundEnds)
  NUMWORD("precision",precision)
  NUMWORD("endPrecision",endPrecision)
  TAGWORD("startCap",startCap)
  TAGWORD("endCap",endCap)
  SSUBWORD("scale",scale)
  SSUBWORD("A",A)
  SSUBWORD("B",B)
  SSUBWORD("C",C)
  SSUBWORD("D",D)
  SSUBWORD("circle",circle)
  TAGWORD("centerToEdge",texCoordsCenterToEdge)
  TAGWORD("followLines",texCoordsFollowLines)
  TINTWORD("tintA",tintA)
  TINTWORD("tintB",tintB)
  TINTWORD("tintC",tintC)
  TINTWORD("tintD",tintD)
  NUMWORD("thickness",thickness)
  TAGWORD("variableThickness",variableThickness)
  SSUBWORD("thicknesses",thicknesses)
 })
 WORDKEYS({
  WORDKEY("name",name)
  ENUMKEY("type",type)
  SSUBKEY("lines",lines)
  TAGKEY("roundEnds",roundEnds)
  NUMBKEY("precision",precision)
  NUMBKEY("endPrecision",endPrecision)
  TAGKEY("startCap",startCap)
  TAGKEY("endCap",endCap)
  SSUBKEY("scale",scale)
  SSUBKEY("A",A)
  SSUBKEY("B",B)
  SSUBKEY("C",C)
  SSUBKEY("D",D)
  SSUBKEY("circle",circle)
  TAGKEY("centerToEdge",texCoordsCenterToEdge)
  TAGKEY("followLines",texCoordsFollowLines)
  TINTKEY("tintA",tintA)
  TINTKEY("tintB",tintB)
  TINTKEY("tintC",tintC)
  TINTKEY("tintD",tintD)
  NUMBKEY("thickness",thickness)
  TAGKEY("variableThickness",variableThickness)
  SSUBKEY("thicknesses",thicknesses)
 })
 BINARY_IN({
  B_IN_STRING(name)
  B_IN_ENUM(TwoDeeStrokeType,type)
  B_IN_SSUB(lines)
  B_IN_BOOL(roundEnds)
  B_IN_NUM(precision)
  B_IN_NUM(endPrecision)
  B_IN_BOOL(startCap)
  B_IN_BOOL(endCap)
  B_IN_SSUB(scale)
  B_IN_SSUB(A)
  B_IN_SSUB(B)
  B_IN_SSUB(C)
  B_IN_SSUB(D)
  B_IN_SSUB(circle)
  B_IN_BOOL(texCoordsCenterToEdge)
  B_IN_BOOL(texCoordsFollowLines)
  B_IN_TINT(tintA)
  B_IN_TINT(tintB)
  B_IN_TINT(tintC)
  B_IN_TINT(tintD)
  B_IN_NUM(thickness)
  B_IN_BOOL(variableThickness)
  B_IN_SSUB(thicknesses)
 })
 BINARY_OUT({
  B_OUT_STRING(name)
  B_OUT_ENUM(TwoDeeStrokeType,type)
  B_OUT_SSUB(lines)
  B_OUT_BOOL(roundEnds)
  B_OUT_NUM(precision)
  B_OUT_NUM(endPrecision)
  B_OUT_BOOL(startCap)
  B_OUT_BOOL(endCap)
  B_OUT_SSUB(scale)
  B_OUT_SSUB(A)
  B_OUT_SSUB(B)
  B_OUT_SSUB(C)
  B_OUT_SSUB(D)
  B_OUT_SSUB(circle)
  B_OUT_BOOL(texCoordsCenterToEdge)
  B_OUT_BOOL(texCoordsFollowLines)
  B_OUT_TINT(tintA)
  B_OUT_TINT(tintB)
  B_OUT_TINT(tintC)
  B_OUT_TINT(tintD)
  B_OUT_NUM(thickness)
  B_OUT_BOOL(variableThickness)
  B_OUT_SSUB(thicknesses)
 })
 CLONE(TwoDeeStroke,{
  DUPE(type)
  a->lines.Duplicate(&lines);
  DUPE(roundEnds)
  DUPE(precision)
  DUPE(endPrecision)
  DUPE(startCap)
  DUPE(endCap)
  a->scale.Set(&scale);
  a->A.Set(&A);
  a->B.Set(&B);
  a->C.Set(&C);
  a->D.Set(&D);
  a->circle.Set(&circle);
  DUPE(texCoordsCenterToEdge)
  DUPE(texCoordsFollowLines)
  a->tintA.fromCrayon(tintA);
  a->tintB.fromCrayon(tintB);
  a->tintC.fromCrayon(tintC);
  a->tintD.fromCrayon(tintD);
  DUPE(thickness)
  DUPE(variableThickness)
  a->thicknesses.Duplicate(&thicknesses);
 })
 int TrianglesCount() {
  int ends=(roundEnds?(startCap&&endCap?2:(startCap?1:(endCap?1:0))):0);
  switch ( type ) {   
   case tds_Line:  return 2+endPrecision*ends; break;
   case tds_Lines: return (lines.count*2) + endPrecision*ends; break;
   case tds_Curve: return (precision*2) + endPrecision*ends; break;
   case tds_FilledCircle: return precision; break;
   case tds_Triangle: return 1; break;
   case tds_Quad: return 2; break;
  }
 }
 void SetLine( double x, double y, double x2, double y2 ) {
  type=tds_Line;
  A.Set(x,y);
  B.Set(x2,y2);
 }
 void SetLines( Cartesiands *lines ) {
  type=tds_Lines;
  this->lines.Duplicate(lines);
  Cartesiand *one=(Cartesiand *)this->lines.first;
  Cartesiand *two=(Cartesiand *)this->lines.last;
  if ( one ) this->A.Set(one);
  if ( two ) this->B.Set(two);
 }
 void SetLines( Cartesiands *lines, Doubles *thicknesses ) {
  type=tds_Lines;
  variableThickness=true;
  this->lines.Duplicate(lines);
  Cartesiand *one=(Cartesiand *)this->lines.first;
  Cartesiand *two=(Cartesiand *)this->lines.last;
  if ( one ) this->A.Set(one);
  if ( two ) this->B.Set(two);
 }
 void SetCurve( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy ) {
  type=tds_Curve;
  this->A.Set(ax,ay);
  this->B.Set(bx,by);
  this->C.Set(cx,cy);
  this->D.Set(dx,dy);
  this->lines.Clear();
  curve2d.SetLOD(precision);
  Vertices *verts=curve2d.Bezier(ax,ay,bx,by,cx,cy,dx,dy);
  EACH(verts->first,Vertex,v) this->lines.Append(new Cartesiand((double)v->x,(double)v->y));
  delete verts;
  Cartesiand *one=(Cartesiand *)this->lines.first;
  Cartesiand *two=(Cartesiand *)this->lines.last;
  if ( one ) this->A.Set(one);
  if ( two ) this->B.Set(two);
 }
 void SetCircle( double ax, double ay, double R ) {
  type=tds_FilledCircle;
  circle.Set(ax,ay,R);
 }
 void SetTriangle( double ax, double ay, double bx, double by, double cx, double cy ) {
  type=tds_Triangle;
  A.Set(ax,ay);
  B.Set(bx,by);
  C.Set(cx,cy);
 }
 void SetQuad( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy ) {
  type=tds_Quad;
  A.Set(ax,ay);
  B.Set(bx,by);
  C.Set(cx,cy);
  D.Set(dx,dy);
 }
 void toCode() {
  OUTPUT("s=new TwoDeeStroke; c->strokes.Append(s);\n");
  OUTPUT("s->type=(TwoDeeStrokeType)%d;",(int)type);
  EACH(lines.first,Cartesiand,c) OUTPUT("s->lines.Append(new Cartesiand(%f,%f));\n",(double)c->x,(double)c->y);
  OUTPUT("s->roundEnds=%s; ",roundEnds?"true":"false");
  OUTPUT("s->precision=%d; ",(int)precision);
  OUTPUT("s->endPrecision=%d; ",(int)endPrecision);
  OUTPUT("s->startCap=%s; ",startCap?"true":"false");
  OUTPUT("s->endCap=%s;\n",endCap?"true":"false");
  //OUTPUT("s->scale.Set(%f,%f);\n",(double)scale.x,(double)scale.y);
  OUTPUT("s->A.Set(%f,%f); ",(double)A.x,(double)A.y);
  OUTPUT("s->B.Set(%f,%f); ",(double)B.x,(double)B.y);
  OUTPUT("s->C.Set(%f,%f); ",(double)C.x,(double)C.y);
  OUTPUT("s->D.Set(%f,%f);\n",(double)D.x,(double)D.y);
  OUTPUT("s->circle.Set(%f,%f,%f);\n",(double)circle.x,(double)circle.y,(double)circle.R);
  OUTPUT("s->texCoordsCenterToEdge=%s; ",texCoordsCenterToEdge?"true":"false");
  OUTPUT("s->texCoordsFollowLines=%s; ",texCoordsFollowLines?"true":"false");
  OUTPUT("s->addCircleAtJoint=%s; ",addCircleAtJoint?"true":"false");
  OUTPUT("s->variableThickness=%s;\n",variableThickness?"true":"false");
  //OUTPUT("s->tintA.Float(%f,%f,%f,%f);\n",(float)tintA.rf,(float)tintA.gf,(float)tintA.bf,(float)tintA.af);
  //OUTPUT("s->tintB.Float(%f,%f,%f,%f);\n",(float)tintB.rf,(float)tintB.gf,(float)tintB.bf,(float)tintB.af);
  //OUTPUT("s->tintC.Float(%f,%f,%f,%f);\n",(float)tintC.rf,(float)tintC.gf,(float)tintC.bf,(float)tintC.af);
  //OUTPUT("s->tintD.Float(%f,%f,%f,%f);\n",(float)tintD.rf,(float)tintD.gf,(float)tintD.bf,(float)tintD.af);
  OUTPUT("s->thickness=%f;\n",(double)thickness);
 }
MANY(TwoDeeStroke,TwoDeeStrokeHandle,TwoDeeStrokeHandles,"stroke",TwoDeeStrokes,{ tint.Pick(alabaster); stashed_id=-1; })
 Crayon tint;
 ///
 Zint stashed_id;
 TwoDeeTriangles triangles;
 ~TwoDeeStrokes();
 KEYSWORDSGroups("stroke",TwoDeeStroke);
 BINARY_INOUT(TwoDeeStroke);
 CLONES(TwoDeeStroke,TwoDeeStrokes);
 int TrianglesCount() { int total=0; FOREACH(TwoDeeStroke,tds) total+=tds->TrianglesCount(); return total; }
 void SetBasicTexcoord( double x, double y, Vertexd *out ) { out->Set(x/1.0,y/1.0); }
 double AddLineQuad( TwoDeeTriangles *out, double x, double y, double x2, double y2, double thickness=0.05, bool texCoordsFollowLines=false, double tcx=0.0, double tcx2=1.0 ) {
  Circle c(0.0,0.0,thickness/2.0);
  Line seg(x,y,x2,y2);
  double angleA=rad2deg(seg.LineAngle());
  double angleB=angleA+180.0;
  double topA=norm_deg(angleB+90.0)/360.0;
  double topB=norm_deg(angleA-90.0)/360.0;
  double bottomA=norm_deg(angleB-90.0)/360.0;  
  double bottomB=norm_deg(angleA+90.0)/360.0;  
  Vertexd A; c.Point(topA,&A);    A.Add(x,y);
  Vertexd B; c.Point(topB,&B);    B.Add(x2,y2);
  Vertexd C; c.Point(bottomB,&C); C.Add(x2,y2);
  Vertexd D; c.Point(bottomA,&D); D.Add(x,y);
  Vertexd ta,tb,tc,td;
  if ( texCoordsFollowLines ) {
  } else {
   SetBasicTexcoord(A.x,A.y,&ta);
   SetBasicTexcoord(B.x,B.y,&tb);
   SetBasicTexcoord(C.x,C.y,&tc);
   SetBasicTexcoord(D.x,D.y,&td);
  }
  out->Add(A.x,A.y,ta.x,ta.y,B.x,B.y,tb.x,tb.y,C.x,C.y,tc.x,tc.y,D.x,D.y,td.x,td.y);
  seg.Set(C.x,C.y,D.x,D.y);
  return angleA;
 }
 void AddCircle( TwoDeeTriangles *out, double x, double y, double R, int precision, bool texCoordsCenterToEdge=false ) {
  double delta=iratiod(1,precision);
  double time=0.0;
  Circle c(x,y,R);
  Vertexd B1,B2,tc1,tc2,tc3;
  for ( int i=0; i<precision; i++ ) {
   c.Point(time,&B1);
   c.Point(UMIN(1.0,(time+delta)),&B2);
   if ( texCoordsCenterToEdge ) {
    tc1.Set(time,time);
    tc2.Set(time+delta,time+delta);
    tc3.Set(0.5,0.5);
   } else {
    SetBasicTexcoord(B1.x,B1.y,&tc1);
    SetBasicTexcoord(B2.x,B2.y,&tc2);
    SetBasicTexcoord(c.x,c.y,&tc3);
   }
   out->Add(B2.x,B2.y,tc2.x,tc2.y,B1.x,B1.y,tc1.x,tc1.y,c.x,c.y,tc3.x,tc3.y);
   time+=delta;
  }
 }
 void AddSemicircle( TwoDeeTriangles *out, double x, double y, double a, double R, int precision, bool texCoordsCenterToEdge=false ) {
  double delta=iratiod(1,precision)/2.0;
  double time=norm_deg(a)/360.0;
  Circle c(x,y,R);
  Vertexd B1,B2,tc1,tc2,tc3;
  for ( int i=0; i<precision; i++ ) {
   c.Point(time,&B1);
   c.Point(UMIN(1.0,(time+delta)),&B2);
   if ( texCoordsCenterToEdge ) {
    tc1.Set(time,time);
    tc2.Set(time+delta,time+delta);
    tc3.Set(0.5,0.5);
   } else {
    SetBasicTexcoord(B1.x,B1.y,&tc1);
    SetBasicTexcoord(B2.x,B2.y,&tc2);
    SetBasicTexcoord(c.x,c.y,&tc3);
   }
   out->Add(B2.x,B2.y,tc2.x,tc2.y,B1.x,B1.y,tc1.x,tc1.y,c.x,c.y,tc3.x,tc3.y);
   time+=delta;
  }
 }
 void Get( TwoDeeTriangles *out, bool texCoordsFollowLines=false ) {
  FOREACH(TwoDeeStroke,tds) {
   if ( tds->type == tds_Triangle ) {
    out->Add(tds->A.x,tds->A.y,tds->A.x2,tds->A.y2,tds->B.x,tds->B.y,tds->B.x2,tds->B.y2,tds->C.x,tds->C.y,tds->C.x2,tds->C.y2);
   } else if ( tds->type == tds_Quad ) {
    out->Add(tds->A.x,tds->A.y,tds->A.x2,tds->A.y2,tds->B.x,tds->B.y,tds->B.x2,tds->B.y2,tds->C.x,tds->C.y,tds->C.x2,tds->C.y2,tds->D.x,tds->D.y,tds->D.x2,tds->D.y2);
   } else if ( tds->type == tds_FilledCircle ) {
    AddCircle(out,tds->A.x,tds->A.y,tds->thickness,tds->precision,tds->texCoordsCenterToEdge);
   } else {
    double angle=0.0;
    switch ( tds->type ) {   
     case tds_Line: {
      angle=AddLineQuad(out,tds->A.x,tds->A.y,tds->B.x,tds->B.y,tds->thickness,(tds->texCoordsFollowLines||texCoordsFollowLines) /*...*/ );
      if (!tds->addCircleAtJoint) {
       if ( tds->startCap ) AddSemicircle(out,tds->A.x,tds->A.y,angle+90.0,tds->thickness/2.0,tds->endPrecision);
       if ( tds->endCap )   AddSemicircle(out,tds->B.x,tds->B.y,angle+270.0,tds->thickness/2.0,tds->endPrecision);
      }
     } break;
     case tds_Lines: {
      Circle c(tds->thickness/2.0);
      EACH(tds->lines.first,Cartesiand,a) {
       if ( tds->addCircleAtJoint ) AddCircle(out,a->x,a->y,tds->thickness/2.0,tds->endPrecision.value,tds->texCoordsCenterToEdge);
       Cartesiand *b=(Cartesiand *) a->next;
       if ( b ) {
        angle=AddLineQuad(out,a->x,a->y,b->x,b->y,tds->thickness,(tds->texCoordsFollowLines || texCoordsFollowLines) /*...*/ );
       } else { // endCap?
        if (!tds->addCircleAtJoint && tds->endCap   ) AddSemicircle(out,a->x,a->y,angle+270.0,tds->thickness/2.0,tds->endPrecision);
       }
       if (!a->prev) { // startCap?
        if (!tds->addCircleAtJoint && tds->startCap ) AddSemicircle(out,a->x,a->y,angle+90.0,tds->thickness/2.0,tds->endPrecision);
       }
      }
     } break;
     case tds_Curve: {
      curve2d.SetLOD(tds->precision);
      Vertices *verts=curve2d.Bezier(tds->A.x,tds->A.y,tds->B.x,tds->B.y,tds->C.x,tds->C.y,tds->D.x,tds->D.y);
      if ( verts ) {
       EACH(verts->first,Vertex,a) {
        if ( tds->addCircleAtJoint ) AddCircle(out,(double)a->x,(double)a->y,tds->thickness/2.0,tds->endPrecision.value,tds->texCoordsCenterToEdge);
        Vertex *b=(Vertex *) a->next;
        if ( b ) {
         angle=AddLineQuad(out,(double)a->x,(double)a->y,(double)b->x,(double)b->y,tds->thickness,(tds->texCoordsFollowLines || texCoordsFollowLines) /*...*/ );
        } else { // endCap?
         if (!tds->addCircleAtJoint && tds->endCap   ) AddSemicircle(out,(double)a->x,(double)a->y,angle+270.0,tds->thickness/2.0,tds->endPrecision);
        }
        if (!a->prev) { // startCap?
         if (!tds->addCircleAtJoint && tds->startCap ) AddSemicircle(out,(double)a->x,(double)a->y,angle+90.0,tds->thickness/2.0,tds->endPrecision);
        }
       }
       delete verts;
      }
     } break;
     case tds_FilledCircle: case tds_Triangle: case tds_Quad: break;
    }
   }
  }
 }
 void toCode() {
  FOREACH(TwoDeeStroke,c) c->toCode();
 }
DONE(TwoDeeStroke);

ONE(TwoDeeStrokeCollection,{})
 Zint ord;
 TwoDeeStrokes strokes;
 KEYWORDS({ KEYWORD("name",{name=w;}); SSUBWORD("strokes",strokes); NUMWORD("ord",ord); })
 WORDKEYS({ WORDKEY("name",name);      SSUBKEY("strokes",strokes);  NUMBKEY("ord",ord); })
 BINARY_IN({  B_IN_STRING(name);  B_IN_NUM(ord);  B_IN_SSUB(strokes); })
 BINARY_OUT({ B_OUT_STRING(name); B_OUT_NUM(ord); B_OUT_SSUB(strokes); })
 CLONE(TwoDeeStrokeCollection,{ DUPE(name); DUPE(ord); a->strokes.Duplicate(&strokes); })
 void toCode() {
  OUTPUT("c=new TwoDeeStrokeCollection; uglyFontStrokes.Append(c); c->ord = %d; c->name=\"%s\";\n",ord.value,name.c_str());
  strokes.toCode();
 }
MANY(TwoDeeStrokeCollection,TwoDeeStrokeCollectionHandle,TwoDeeStrokeCollectionHandles,"collection",TwoDeeStrokeCollections,{})
 KEYSWORDSGroups("collection",TwoDeeStrokeCollection)
 BINARY_INOUT(TwoDeeStrokeCollection)
 CLONES(TwoDeeStrokeCollection,TwoDeeStrokeCollections)
 void toCode() {
  FOREACH(TwoDeeStrokeCollection,c) c->toCode();
 }
DONE(TwoDeeStrokeCollection);

ONE(TwoDeeElement,{ ebo=vbo=0; })
 Zint stash_id;
 GLuint ebo,vbo;
// Crayon tint;
 Zint elements;
 Zbool replace;
 ~TwoDeeElement() {
  glDeleteBuffers(1,&ebo);
  glDeleteBuffers(1,&vbo);
 }
MANY(TwoDeeElement,TwoDeeElementHandle,TwoDeeElementHandles,"TwoDeeElement",TwoDeeElementStash,{})
 Indexed<Zp<TwoDeeElement>> stashed;
 Integers replaceables;
 Zint
  rectangle, rectFlipX, rectFlipY, rectFlipXY,
  filledRect, filledRect_FlipX, filledRect_FlipY, filledRect_FlipXY,
  linesRect,linesGrid10x10,
  ca_TL,ca_TR,ca_BR,ca_BL,
  rr_Button, rr_Vertical, rr_Standard, rr_FlipX, rr_FlipY, rr_FlipXY, rr_Transition, 
  ellipsoid, ellipsoidFlipX, ellipsoidCenterEdge;
 Zp<TwoDeeElement> 
  rrs_rectangle, rrs_rectFlipX, rrs_rectFlipY, rrs_rectFlipXY,
  rrs_filledRect, rrs_filledRect_FlipX, rrs_filledRect_FlipY, rrs_filledRect_FlipXY,
  rrs_linesRect,rrs_linesGrid10x10,
  rrs_ca_TL,rrs_ca_TR,rrs_ca_BR,rrs_ca_BL,
  rrs_rr_Button, rrs_rr_Vertical, rrs_rr_Standard, rrs_rr_FlipX, rrs_rr_FlipY, rrs_rr_FlipXY, rrs_rr_Transition, 
  rrs_ellipsoid, rrs_ellipsoidFlipX, rrs_ellipsoidCenterEdge;
 TwoDeeElement *Get( int i ) {
  if ( i<0 || i >= (int) stashed.length ) {
   OUTPUT("TwoDeeElementStash warning: %d lookup was out of range 0-%d.\n",i,(int)(stashed.length-1));
   return null;
  }
  if ( !stashed[i].pointer ) {
   OUTPUT("TwoDeeElementStash warning: stashed pointer was empty at index %d.\n",i);
  }
  return stashed[i].pointer;
 }
 int Add( TwoDeeElement *tde ) {
  int replaceable=FirstReplaceable();
  if ( replaceable == -1 ) {
   Append(tde);
   stashed.Increase()->pointer=tde;
   tde->stash_id=(int)stashed.length-1;
   OUTPUT("TwoDeeElementStash stashed into %d, increasing the stash size.\n",(int)(stashed.length-1));
   return tde->stash_id;
  } else {
   if ( stashed[replaceable] ) {
    if ( stashed[replaceable]->replace ) OUTPUT("TwoDeeElementStash stashed %d elements into %d, replacing an existing item.\n",stashed[replaceable]->elements,replaceable);
    Remove(stashed[replaceable].pointer);
    delete stashed[replaceable].pointer;
    stashed[replaceable].pointer=tde;
    Append(tde);
    tde->stash_id=replaceable;
   } else OUTPUT("TwoDeeElementStash stashed into %d, replacing an empty item! (shouldn't ever happen)\n",replaceable);
   return replaceable;
  }
 }
 void Replace( int stash ) {
  if ( stash >=0 && stash < (int)stashed.length && stashed[stash] ) {
   if ( !stashed[stash]->replace ) {
    stashed[stash]->replace=true;
    replaceables.Add(stash);
   }
  } else {
   OUTPUT("TwoDeeElementStash warning: %d lookup was out of range 0-%d.\n",stash,(int)(stashed.length-1));
  }
 }
 int FirstReplaceable() {
  if ( replaceables.count > 0 ) {
   Integer *n=(Integer *)replaceables.first;
   if ( n ) {
    int value=n->i;
    replaceables.Remove(n);
    delete n;
    return value;
   }
   OUTPUT("Error! FirstReplaceable unexpectedly not on list\n");
   return -1;
  } else return -1;
 }
 void Init();
 int GenGrid( int cols, int rows, double thickness=0.05, Crayon tint=crayons.Pick(alabaster), bool texCoordsFollowLines=false, bool outerEdge=true, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=false, bool roundEnds=true, int endPrecision=6 );
 int GenRectangle( Crayon tint=crayons.Pick(alabaster), bool flipX=false, bool flipY=false );
 int GenQuad( double tcx, double tcy, double tcx2, double tcy2, Crayon tint=crayons.Pick(alabaster) );
 int GenQuad( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double tcx, double tcy, double tcx2, double tcy2, Crayon tint=crayons.Pick(alabaster) );
 int GenQuadRotated( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double tcx, double tcy, double tcx2, double tcy2, Crayon tint=crayons.Pick(alabaster) );
 int GenQuads( Cartesiands *quads, Cartesiands *texcoords, Crayon tint=crayons.Pick(alabaster) ) ;
 int GenQuads( Cartesiands *quads, Cartesiands *texcoords, Cartesiands *colors ) ;
 int GenCircle( int precision=64, Crayon tint=crayons.Pick(alabaster), bool flipX=false, bool flipY=false, bool texcoordsCenterEdge=false );
 int GenCircleArc( double startTime, double endTime, int precision=16, Crayon tint=crayons.Pick(alabaster), bool flipX=false, bool flipY=false, bool texcoordsCenterEdge=false );
 int GenCircleThickness( double startTime, double endTime, int precision=64, Crayon tint=crayons.Pick(alabaster), double thickness=0.05, bool roundEnds=true, bool texcoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 ) {
  Circle c(0.5,0.5,0.5);
  Cartesiands linespoints;
  double timeDelta=(endTime-startTime)/(double)precision;
  Vertexd out;
  for ( double t=startTime; t<=endTime; t+=timeDelta ) {
   c.Point(t,&out);
   linespoints.Append(new Cartesiand(out.x,out.y));
  }
  if ( startTime == 0.0 && endTime == 1.0 ) {
   endPrecision=0;
   roundEnds=false;
  }
  return GenThickness(&linespoints,tint,thickness,roundEnds,texcoordsFollowLines,flipX,flipY,fitTo1,texcoordsFollowLinesScale,endPrecision);
 }
 int GenThickness( Cartesiands *linespoints, Crayon tint=crayons.Pick(alabaster), double thickness=0.05, bool roundEnds=true, bool texcoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 );
 int GenThickness( CartesiandsList *linespoints, Crayon tint=crayons.Pick(alabaster), double thickness=0.05, bool roundEnds=true, bool texcoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 );
 int GenLines( Cartesiands *lines, Crayon tint=crayons.Pick(alabaster), double thickness=0.05, bool roundEnds=false, bool texcoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 );
 int GenLines( CartesiandsList *lines, Crayon tint=crayons.Pick(alabaster), double thickness=0.05, bool roundEnds=false, bool texcoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 );
 int GenVariableThickness( Cartesiands *linespoints, Crayon tint=crayons.Pick(alabaster), bool roundEnds=true, bool texcoordsFollowLines=false, bool teeth=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=1.0, int endPrecision=8 );
 int GenRoundRect( double cornerRadius=0.1, Crayon tint=crayons.Pick(alabaster), bool flipX=false, bool flipY=false, int precision=8 );
 int GenRoundRect( double xcornerRadius, double ycornerRadius, Crayon tint=crayons.Pick(alabaster), bool flipX=false, bool flipY=false, int precision=8 );
 int GenEmptyRect( double thickness=0.05, Crayon tint=crayons.Pick(alabaster), bool texCoordsFollowLines=false, bool flipX=false, bool flipY=false, bool fitTo1=true, double texcoordsFollowLinesScale=false, bool roundEnds=true, int endPrecision=6 );
 int GenFacets( STLFacets *in, Crayon tint=crayons.Pick(alabaster), bool fitTo1=true, bool flipX=false, bool flipY=false );
 int GenTriangles( TwoDeeTriangles *in );
DONE(TwoDeeElement);

ONE(ViewportSettings,{})
 glm::mat4 ortho;
 Zint w,h;
 Zdouble wd,hd,aspectW,aspectH,viewAspect,inverseAspect;
 void Set() { Set(display.w,display.h); }
 void Set( int W, int H ) {
  w=W;
  h=H;
  wd=(double)w;
  hd=(double)h;
  viewAspect=wd/hd;
  inverseAspect=hd/wd;
  ortho=glm::ortho(0.0f,(float)wd,(float)hd,0.0f,-100.0f,100.0f);
 }
MANY(ViewportSettings,ViewportSettingsHandle,ViewportSettingsHandles,"ViewportSettings",ViewportSettingses,{})
 void Push( ViewportSettings *vs ) { Append(vs); }
 ViewportSettings *Pop() {
  ViewportSettings *popped=(ViewportSettings *) last;
  if ( popped ) Remove(popped);
  return popped;
 }
DONE(ViewportSettings);

class Art2d : public ListItem {
public:
 Zp<GLSLShader> shader;
 TwoDeeElementStash stash;
 GLuint ebo;
 GLuint vTopLeft,    vTopLeftYFlipped,    vTopLeftXFlipped, vTopLeftXYFlipped,
     vbo_qn11n11, vbo_qn11n11YFlipped, vbo_qn11n11XFlipped, vbo_qn11n11XYFlipped,
       vCentered,   vCenteredYFlipped,   vCenteredXFlipped, vCenteredXYFlipped;
 void CreatePrimitives();
 Zp<GLImage> solidblack,solidwhite,rampL,rampR,checkers;
 Zint standardMouseCursorPt1,standardMouseCursorPt2;
 Art2d() : ListItem() { shader=&texShader; viewport.Instantiate(); Viewport(); }

 void Init() {
  stash.Init();
  InitSimpleSprites();
  CreatePrimitives();
  Viewport();
 }

 ViewportSettingses viewportStack;
 Zdis<ViewportSettings> viewport;
 void PushViewport() {
  viewportStack.Push(viewport);
  viewport.pointer=new ViewportSettings;
 }
 void PushViewport( FBO *f, bool _glViewport=false ) {
  viewportStack.Push(viewport);
  viewport.pointer=new ViewportSettings;
  Viewport(f,_glViewport);
 }
 void PushViewport( int w, int h, bool _glViewport=false ) {
  viewportStack.Push(viewport);
  viewport.pointer=new ViewportSettings;
  Viewport(w,h,_glViewport);
 }
 void PopViewport() {
  ViewportSettings *popped=viewportStack.Pop();
  if ( !popped ) { OUTPUT("Art2d::ViewportSettings stack could not Pop()\n"); return; }
  viewport.Recycle(popped);
 }
 void ResetViewport( bool _glViewport=false ) {
  viewportStack.Clear();
  viewport.Recycle(new ViewportSettings);
  Viewport(_glViewport);
 }
 void Viewport( bool _glViewport=false ) { Viewport(display.w,display.h,_glViewport); }
 void Viewport( Display *d, bool _glViewport=false ) { Viewport(d->w,d->h,_glViewport); }
 void Viewport( FBO *f, bool _glViewport=false ) { Viewport(f->w,f->h,_glViewport); }
 void Viewport( int w, int h, bool _glViewport=false ) {
  viewport->Set(w,h);
  if ( _glViewport ) {
   glViewport(0, 0, (GLsizei) display.w, (GLsizei) display.h);
   glDisable(GL_DEPTH_TEST);
  }
 }

 // Quads 0->1 x1->x2 0->1 y1->y2
 // Quads -1 to 1 x1/y1->x2/y2
 // Quads -0.5 to 0.5 1->2
 void InitSimpleSprites();
 
 void Pochoir( VBOStaticVC *vc, Crayon tint,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vc->ibo);
  glBindBuffer(GL_ARRAY_BUFFER,vc->vbo);
  v2Shader.UpdateUniforms(trans,tint);
  glDrawElements(vc->draw_method,vc->count,preferred_type_enum,BUFFER_OFFSET(0));
  v2Shader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
 }
 void Pochoir( VBOStaticVC *vc, Crayon tint, Blends blend, 
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  Blending(blend);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vc->ibo);
  glBindBuffer(GL_ARRAY_BUFFER,vc->vbo);
  v2Shader.UpdateUniforms(trans,tint);
  glDrawElements(vc->draw_method,vc->count,preferred_type_enum,BUFFER_OFFSET(0));
  v2Shader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
 }
 void Stretch( GLuint source, Crayon tint,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  shader->UpdateUniforms(trans,source,tint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  shader->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Stretch( GLuint source, Crayon tint, Blends blend,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  Blending(blend);
  Stretch(source,tint,x,y,w,h,flipX,flipY);
 }
 void Stretch( GLImage *source, Crayon tint,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  Stretch(source->texture,tint,x,y,w,h,flipX,flipY);
 }
 void Stretch( GLImage *source, Crayon tint, Blends blend,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  Blending(blend);
  Stretch(source->texture,tint,x,y,w,h,flipX,flipY);
 } 
 void Stretch( GLSLShader *source,
               Crayon tint,
               double x, double y, double w, double h,  
               bool flipX=false, bool flipY=false ) {
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  shader->Tint(&tint);
  shader->UpdateUniforms(trans);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  shader->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Stretch( GLSLShader *source,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false ) {
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vTopLeftYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  shader->UpdateUniforms(trans);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  shader->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void BestFit( GLImage *source, Crayon tint, Blends blend,
               double x, double y, double w, double h,
               bool flipX=false, bool flipY=false, double clampMaxScaleTo=0.0 ) {
  if ( !source || source->width == 0 || source->height == 0 || w==0 || h== 0 ) return;
  double scale = UMIN( w/(double)source->width, h/(double)source->height );
  if ( clampMaxScaleTo > 0.0 && scale > clampMaxScaleTo ) scale=clampMaxScaleTo;
  Cartesiand draw(0,0,scale*(double)source->width,scale*(double)source->height);
  draw.MoveBy(x+w/2.0-draw.w/2.0,y+h/2.0-draw.h/2.0);
  Blending(blend);
  Stretch(source->texture,tint,draw.x,draw.y,draw.w,draw.h,flipX,flipY);
 }
 void ElementLines( Crayon tint, Blends blend, int stashNumber, double x, double y, double w, double h ) { // for debugging
  TwoDeeElement *rrs=stash.Get(stashNumber);
  if ( !rrs ) {
   OUTPUT("art.Element says: Invalid stashNumber: %d\n",stashNumber);
   return;
  }
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  Blending(blend);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  v2Shader.UpdateUniforms(trans,tint);
  glDrawElements( GL_LINES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  v2Shader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 glm::mat4 PrepElement( int stashNumber, double x, double y, double w, double h ) {
  stashNumber=stashNumber%(int)stash.count;
  TwoDeeElement *rrs=stash.Get(stashNumber);
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  return trans;
 }
 void DrawElement( int stashNumber, double x, double y, double w, double h ) {
  TwoDeeElement *rrs=stash.Get(stashNumber);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Element( GLSLShader *in, int stashNumber, double x, double y, double w, double h ) {
  TwoDeeElement *rrs=stash.Get(stashNumber);
  if ( !rrs ) {
   OUTPUT("art.Element says: Invalid stashNumber: %d\n",stashNumber);
   return;
  }
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  in->UpdateUniforms(trans);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  in->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Element( Crayon tint, int stashNumber, double x, double y, double w, double h ) {
  TwoDeeElement *rrs=stash.Get(stashNumber);
  if ( !rrs ) {
   OUTPUT("art.Element says: Invalid stashNumber: %d\n",stashNumber);
   return;
  }
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  v2Shader.UpdateUniforms(trans,tint);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  v2Shader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Element( GLuint t, Crayon tint, int stashNumber, double x, double y, double w, double h ) {
  stashNumber=stashNumber%(int)stash.count;
  TwoDeeElement *rrs=stash.Get(stashNumber);
  if ( !rrs ) {
   OUTPUT("art.Element says: Invalid stashNumber: %d\n",stashNumber);
   return;
  }
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  texShader.UpdateUniforms(trans,t,tint);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  texShader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
 void Element( GLImage *t, Crayon tint, int stashNumber, double x, double y, double w, double h ) {
  Element(t->texture,tint,stashNumber,x,y,w,h);
 }
 void Element( GLImage *t, Crayon tint, Blends blend, int stashNumber, double x, double y, double w, double h ) {
  Blending(blend);
  Element(t->texture,tint,stashNumber,x,y,w,h);
 }
 void Element( GLImage *t, Blends blend, int stashNumber, double x, double y, double w, double h ) {
  Blending(blend);
  Element(t->texture,crayons.Pick(alabaster),stashNumber,x,y,w,h);
 }
 void Element( Crayon tint, Blends blend, int stashNumber, double x, double y, double w, double h ) {
  Blending(blend);
  Element(tint,stashNumber,x,y,w,h);
 }
 void Element( int stashNumber, double x, double y, double w, double h ) {
  Element(crayons.Pick(alabaster),stashNumber,x,y,w,h);
 }
 void Element2( Crayon outer, Crayon inner, int stashNumber, double x, double y, double w, double h, double border=5.0 ) {
  Element(outer,stashNumber,x,y,w,h);
  Element(inner,stashNumber,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outer, Crayon inner, Blends blend, int stashNumber, double x, double y, double w, double h, double border=5.0 ) {
  Element(outer,blend,stashNumber,x,y,w,h);
  Element(inner,blend,stashNumber,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outer, Crayon inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Element(outer,blend,stashNumber1,x,y,w,h);
  Element(inner,blend,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outer, GLImage *inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Element(outer,blend,stashNumber1,x,y,w,h);
  Element(inner,blend,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( GLImage *outer, Crayon inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Element(outer,blend,stashNumber1,x,y,w,h);
  Element(inner,blend,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( GLImage *outer, GLImage *inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Blending(blend);
  Element(outer,crayons.Pick(alabaster),stashNumber1,x,y,w,h);
  Element(inner,crayons.Pick(alabaster),stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outerTint, GLImage *outer, Crayon innerTint, GLImage *inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Blending(blend);
  Element(outer,outerTint,stashNumber1,x,y,w,h);
  Element(inner,innerTint,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outerTint, GLImage *outer, Crayon innerTint, Blends blend, int stashNumber, double x, double y, double w, double h, double border=5.0 ) {
  Blending(blend);
  Element(outer,outerTint,stashNumber,x,y,w,h);
  Element(innerTint,stashNumber,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outerTint, Crayon innerTint, GLImage *inner, Blends blend, int stashNumber, double x, double y, double w, double h, double border=5.0 ) {
  Element(outerTint,blend,stashNumber,x,y,w,h);
  Element(inner,innerTint,blend,stashNumber,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outerTint, GLImage *outer, Crayon innerTint, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Blending(blend);
  Element(outer,outerTint,stashNumber1,x,y,w,h);
  Element(innerTint,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 void Element2( Crayon outerTint, Crayon innerTint, GLImage *inner, Blends blend, int stashNumber1, int stashNumber2, double x, double y, double w, double h, double border=5.0 ) {
  Element(outerTint,blend,stashNumber1,x,y,w,h);
  Element(inner,innerTint,blend,stashNumber2,x+border,y+border,w-border*2,h-border*2);
 }
 // Pivots a rectangle around axis of rotation px,py at position x,y 
 // rotated by angle a, h2*2 by w2*2 are rectangular dimensions
 void Pivot( GLImage *source, Crayon tint, Blends blend,
             double x, double y, double px, double py,
             double h2, double w2, double a=0.0, 
             bool flipX=false, bool flipY=false ) {
  flipY=!flipY; // fixes Y inversion of texture
  GL_Assert("Entering art.Pivot");
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::rotate(trans, (float)glm::radians(a), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::translate(trans,glm::vec3(px,py,1.0));
  trans = glm::scale(trans,glm::vec3(w2*2.0,h2*2.0,1.0));
  trans = viewport->ortho * trans;
  Blending(blend);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vCenteredXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vCentered);
  glReportError( glGetError() );
  shader->UpdateUniforms(trans,source,tint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  shader->Disable();
  glReportError( glGetError() );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
 }
 // Pivots a rectangle around axis of rotation px,py at position x,y 
 // rotated by angle a, h2*2 by w2*2 are rectangular dimensions
 void Pivot( GLSLShader *source, 
             double x, double y, double h2, double w2, double a=0.0, double px=0.0, double py=0.0, 
             bool flipX=false, bool flipY=false ) {
  flipY=!flipY; // fixes Y inversion of texture
  GL_Assert("Entering art.Pivot(shader)");
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::rotate(trans, (float)glm::radians(a), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::translate(trans,glm::vec3(px,py,1.0));
  trans = glm::scale(trans,glm::vec3(w2*2.0,h2*2.0,1.0));
  trans = viewport->ortho * trans;
  shader->UpdateUniforms(trans);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vCenteredXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vCentered);
  glReportError( glGetError() );
  source->UpdateUniforms(trans);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  source->Disable();
  glReportError( glGetError() );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
 }
 // Pivots a rectangle around axis of rotation px,py at position x,y 
 // rotated by angle a, h2*2 by w2*2 are rectangular dimensions
 void Pivot( GLSLShader *source, 
             Crayon tint,
             double x, double y, double h2, double w2, double a=0.0, double px=0.0, double py=0.0, 
             bool flipX=false, bool flipY=false ) {
  flipY=!flipY; // fixes Y inversion of texture
  GL_Assert("Entering art.Pivot(shader)");
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::rotate(trans, (float)glm::radians(a), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::translate(trans,glm::vec3(px,py,1.0));
  trans = glm::scale(trans,glm::vec3(w2*2.0,h2*2.0,1.0));
  trans = viewport->ortho * trans;
  source->Tint(&tint);
  shader->UpdateUniforms(trans);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if ( flipX ) {
   if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredXYFlipped);
   else glBindBuffer(GL_ARRAY_BUFFER, vCenteredXFlipped);
  } else if ( flipY ) glBindBuffer(GL_ARRAY_BUFFER, vCenteredYFlipped);
  else glBindBuffer(GL_ARRAY_BUFFER, vCentered);
  glReportError( glGetError() );
  source->UpdateUniforms(trans);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  source->Disable();
  glReportError( glGetError() );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glReportError( glGetError() );
 }
 // Uses the 7-element draw method
 void BevelRect( Crayon tint, double cornerWidth, double x, double y, double w, double h ) {
  Element(tint,stash.ca_TL,x,y,cornerWidth*2,cornerWidth*2);      
  double wide=w-cornerWidth*2; if ( wide > 0.0 ) Element(tint,stash.filledRect,x+cornerWidth,y,wide,cornerWidth); // [1][2][3]
  Element(tint,stash.ca_TR,x+w-cornerWidth*2,y,cornerWidth*2,cornerWidth*2);                      // [   4   ]
  double top=h-cornerWidth*2; if ( top > 0.0 ) Element(tint,stash.filledRect,x,y+cornerWidth,w,top);              // [5][6][7]
  Element(tint,stash.ca_BL,x,y+h-cornerWidth*2,cornerWidth*2,cornerWidth*2);
  if ( wide > 0.0 ) Element(tint,stash.filledRect,x+cornerWidth,y+h-cornerWidth,w-cornerWidth*2,cornerWidth);
  Element(tint,stash.ca_BR,x+w-cornerWidth*2,y+h-cornerWidth*2,cornerWidth*2,cornerWidth*2);
 }
 void BevelRect( Crayon tint, Blends blend, double cornerWidth, double x, double y, double w, double h ) {
  Blending(blend);
  BevelRect(tint,cornerWidth,x,y,w,h);
 }
 void BevelRect2( Crayon outer, Blends a, Crayon inner, Blends b, double cornerWidth, double borderWidth, double x, double y, double w, double h ) {
  BevelRect(outer,a,cornerWidth,x,y,w,h);
  BevelRect(inner,b,cornerWidth,x+borderWidth,y+borderWidth,w-borderWidth*2,h-borderWidth*2);
 }
 void BevelRect2( Crayon outer, Crayon inner, double cornerWidth, double borderWidth, double x, double y, double w, double h ) {
  BevelRect(outer,cornerWidth,x,y,w,h);
  BevelRect(inner,cornerWidth,x+borderWidth,y+borderWidth,w-borderWidth*2,h-borderWidth*2);
 }
 void BevelRect2( Crayon outer, Crayon inner, Blends blend, double cornerWidth, double borderWidth, double x, double y, double w, double h ) {
  Blending(blend);
  BevelRect(outer,cornerWidth,x,y,w,h);
  BevelRect(inner,cornerWidth,x+borderWidth,y+borderWidth,w-borderWidth*2,h-borderWidth*2);
 }
 void Square( Crayon tint, Blends blend, double x, double y, double width ) {
  Blending(blend);
  Square(tint,x,y,width);
 }
 void Square( Crayon tint, double x, double y, double width ) {
  Element(tint,stash.filledRect,x-width/2.0,y-width/2.0,width,width);
 }
 void Rectangle( Crayon tint, Blends blend, double x, double y, double w, double h ) {
  Blending(blend);
  Rectangle(tint,x,y,w,h);
 }
 void Rectangle( Crayon tint, double x, double y, double w, double h ) {
  Element(tint,stash.filledRect,x,y,w,h);
 }

 void StretchXtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTC, double endTC ) {
  Blending(blend);
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  texTCShader.tc.SetCorners(startTC,0.0,endTC,1.0);
  texTCShader.UpdateUniforms(trans,source,tint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  texTCShader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }

 void StretchYtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTC, double endTC ) {
  Blending(blend);
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  texTCShader.tc.SetCorners(0.0,startTC,1.0,endTC);
  texTCShader.UpdateUniforms(trans,source,tint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  texTCShader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }

 void StretchXYtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTCx, double startTCy, double endTCx, double endTCy ) {
  Blending(blend);
  glReportError( glGetError() );
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  texTCShader.tc.SetCorners(startTCx,startTCy,endTCx,endTCy);
  texTCShader.UpdateUniforms(trans,source,tint);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  texTCShader.Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }

 void CreateCursor();
 void DrawStandardCursor();
};

extern Art2d art;

class TwoDeeDisposer {
public:
 Zint stashed_id;
 TwoDeeDisposer() { stashed_id=-1; }
 ~TwoDeeDisposer() {
  if ( stashed_id >= 0 && stashed_id < (int) art.stash.stashed.length && !art.stash.stashed[(size_t)(int)stashed_id]->replace )
   art.stash.Replace(stashed_id);
 }
 void Set( int i ) { stashed_id=i; }
};

class TwoDeeElementArray : public Indexed<Zp<TwoDeeElement>> {
public:
 Indexed<TwoDeeDisposer> disposal;
 void FromStrokes( TwoDeeStrokeCollections *collections ) {
  (*this).Size(256); // ASCII Table.
  disposal.Size(256);
  EACH(collections->first,TwoDeeStrokeCollection,c) {
   if ( c->strokes.stashed_id >= 0 ) art.stash.Replace(c->strokes.stashed_id);
   c->strokes.triangles.Clear();
   c->strokes.Get(&c->strokes.triangles);
   c->strokes.stashed_id.value=art.stash.GenTriangles(&c->strokes.triangles);
   disposal[(size_t)(int)c->ord.value].Set(c->strokes.stashed_id);
   if ( c->ord > 0 && c->ord < 256 ) (*this)[c->ord.value]=art.stash.stashed[(size_t)(int)c->ord.value];
   OUTPUT("Stashed `%s` ord %d into %d with %d triangles\n",c->name.c_str(),c->ord.value,c->strokes.stashed_id.value,c->strokes.triangles.count.value);
  }
 }
};

class Art2dTest : public GLWindow {
 Zp<GLImage> sprite1;
 Zint stashNumber;
 Zint stashedCurve,curveTest2,curveTest3;
 void OnLoad();
 void Render();
};

