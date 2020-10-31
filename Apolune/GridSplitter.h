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
#include "Cartesian.h"
#include "ScreenPosition.h"
#include "UglyFont.h"
#include "Art.h"
#include "Trigger.h"

enum GridSplits {
 gs_Grid=-1,    // Signal used by Best()
 gs_Unknown=0,
 gs_Uno=1,
 gs_Bisect=2,
 gs_Trisect=3,
 gs_Quadrisect=4,
 gs_Quinquesect=5,
 gs_Sexisect=6,
 gs_Septisect=7,
 gs_Octisect=8,
 gs_Nonasect=9,
 gs_Decisect=10,
 gs_Duolayer=11,
 gs_Trilayer=12,
 gs_Quadrilayer=13,
 gs_Quintilayer=14,
 gs_Sexilayer=15,
 gs_Septilayer=16,
 gs_Octilayer=17,
 gs_Nonalayer=18,
 gs_Decilayer=19,
 gs_Trisect2=20,
 gs_BisectQuadrisect=21,
 gs_Col4Row3=22,
 gs_BisectGrid6=23,
 gs_QuadrisectL9=24,
 gs_NonaSeptisectTop=25,
 gs_3377=26,
 gs_RightBisect=27,
 gs_366=28,
 gs_U13Quadrisect=29,
 gs_U8Uno=30,
 gs_RightQuadrisect=31,
 gs_Col8Row3QuadrisectTop=32,
 gs_U13LeftBisect=33,
 gs_Col4QuadCol4=34,
 gs_Col4Quadrisect=35,
 gs_8448=36,
 gs_U13Bisect=37,
 gs_LeftBisect=38,
 gs_Col4UnoCol4=39,
 gs_LeftQuadrisect=40,
 gs_848=41,
 gs_Col4Col4BisectCol4Col4=42,
 gs_Col3Col3BisectCol3Col3Row7=43,
 gs_727=44,
 gs_7337=45,
 gs_488=46,
 gs_Col4Col4Quadrisect=47,
 gs_616=48,
 gs_636=49
};

#define MAX_GridSplits (gs_636)

string GridSplitsName( GridSplits in );

enum GridAspects {
 gs_VLine,
 gs_Column,      // Aspect W:H<=0.125 Inverse H:W>=8.0
 gs_Columnar,    // Aspect W:H<=0.25  Inverse H:W>=4.0
 gs_TallThin,    // Aspect W:H<=0.5   Inverse H:W>=2.0
 gs_NearSquare,  // Aspect W:H ~1.0   Inverse H:W ~1.0
 gs_WideLong,    // Aspect W:H<=24.0   Inverse H:W<=0.5
 gs_Rowar,       // Aspect W:H>=4.0   Inverse H:W>=0.25
 gs_Strip,       // Aspect W:H>=8.0   Inverse H:W<=0.125
 gs_HLine
};

class GridRect : public ListItem {
public:
 Zstring name;
 ScreenPosition rectangle,square;
 GridRect() : ListItem() {}
 GridRect( const char *named, double x, double y, double w, double h ) : ListItem() {
  name=named;
  Set(x,y,w,h);
 }
 void DrawDebug() {
  rectangle.DrawDebug();
  Rectangle(crayons.Pick(green255),square.dx,square.dy,square.dx2,square.dy2);
  Text(FORMAT("%s\n%1.2f x %1.2f",name.c_str(),rectangle.dcx,rectangle.dcy).c_str(),
   crayons.Pick(green255),none,rectangle.dcx,rectangle.dcy,8.0,11.0,3.0,1.0,true
  );
 }
 void Set( double x, double y, double w, double h ) {
  rectangle.Set(x,y,w,h);
  if ( w > h ) square.Set(0.0,0.0,h,h);
  else square.Set(0.0,0.0,w,w);
  square.Center(rectangle.dcx,rectangle.dcy);
 }
 GridAspects Aspect() {
  double A=rectangle.Aspect();
  if ( A <= 0.0625 ) return gs_VLine;
  if ( A <= 0.125  ) return gs_Column;
  if ( A <= 0.25   ) return gs_Columnar;
  if ( A <= 0.75   ) return gs_TallThin;
  if ( A <= 1.5    ) return gs_NearSquare;
  if ( A <= 2.0    ) return gs_WideLong;
  if ( A <= 4.0    ) return gs_Rowar;
  if ( A <= 8.0    ) return gs_Strip;
  /* else         */ return gs_HLine;
 }
 double X() { return rectangle.dx; }
 double Y() { return rectangle.dy; }
 double W() { return rectangle.dw; }
 double H() { return rectangle.dh; }
 double X2() { return rectangle.dx2; }
 double Y2() { return rectangle.dy2; }
 double CX() { return rectangle.dcx; }
 double CY() { return rectangle.dcy; }
 double dx() { return rectangle.dx; }
 double dy() { return rectangle.dy; }
 double dw() { return rectangle.dw; }
 double dh() { return rectangle.dh; }
 double dx2() { return rectangle.dx2; }
 double dy2() { return rectangle.dy2; }
 double dcx() { return rectangle.dcx; }
 double dcy() { return rectangle.dcy; }
 // Makes a bounding rectangle of the combination.
 void Combine( GridRect *other ) {
  double x=rectangle.x < other->rectangle.x ? rectangle.x : other->rectangle.x;
  double y=rectangle.y < other->rectangle.y ? rectangle.y : other->rectangle.y;
  double x2=rectangle.x2 > other->rectangle.x2 ? rectangle.x2 : other->rectangle.x2;
  double y2=rectangle.y2 > other->rectangle.y2 ? rectangle.y2 : other->rectangle.y2;
  double w=x2-x;
  double h=y2-y;
  Set(x,y,w,h);
 }
 void MoveBy( double x, double y ) {
  rectangle.MoveBy(x,y);
  square.MoveBy(x,y);
 }
 void MoveTo( double x, double y ) {
  rectangle.MoveTo(x,y);
  Set(rectangle.dx,rectangle.dy,rectangle.dh,rectangle.dw);
 }
 void Center( double x, double y ) {
  rectangle.Center(x,y);
  square.Center(x,y);
 }
};

HANDLES(GridRect,GridRectHandle,GridRectHandles,"GridRect");

class GridRects : public LinkedList {
public:
 GridRect outer;
 GridRectHandles selected;
 GridRect *Add( const char *pre, const char *named, double x, double y, double w, double h ) {
  return Add(Prefix(pre,named).c_str(),x,y,w,h);
 }
 GridRect *Add( const char *named, double x, double y, double w, double h ) {
  GridRect *gr=new GridRect(named,x,y,w,h);
  Append(gr);
  return gr;
 }
 void MoveSelectedBy( double x, double y ) { EACH(selected.first,GridRectHandle,rh) rh->p->MoveBy(x,y); }
 void MoveSelectedTo( double x, double y ) { EACH(selected.first,GridRectHandle,rh) rh->p->MoveTo(x,y); }
 GridRect *MergeSelected() {
  if ( !selected.first ) return null;
  GridRectHandle *start=(GridRectHandle *) selected.first;
  selected.Remove(selected.first);
  EACHN(selected.first,GridRectHandle,rh,{
   start->p->Combine(rh->p);
   Remove(rh->p);
   delete rh->p;
   selected.Remove(rh);
   delete rh;
  });
  GridRect *result=start->p;
  delete start;  // Leave rectangle behind.
  Deselect();
  return result;
 }
 void Deselect() { selected.Clear(); }
 void Deselect( GridRect *rect ) {
  EACHN(selected.first,GridRectHandle,rh,{
   if ( rh->p == rect ) {
    selected.Remove(rh);
    delete rh;
   }
  });
 }
 GridRect *PopFirstSelected() {
  if ( !selected.first ) return null;
  GridRectHandle *rh=((GridRectHandle*)selected.first);
  GridRect *r=rh->p;
  rh=null;
  Deselect(r);
  Remove(r);
  return r;
 }
 void Select() {
  FOREACH(GridRect,r) Select(r);
 }
 void Select( GridRect *rect ) {
  EACH(selected.first,GridRectHandle,rh) if ( rh->p == rect ) return;
  selected.Add(rect);
 }
 void Select( double x, double y ) {
  EACH(selected.first,GridRectHandle,rh) if ( rh->p->rectangle.within(x,y) ) Select(rh->p);
 }
 void Select( double x, double y, double x2, double y2 ) {
  Box a(x,y,x2,y2);
  EACH(selected.first,GridRectHandle,rh) {
   GridRect *r=rh->p;
   Box b(r->rectangle.dx,r->rectangle.dy,r->rectangle.dx2,r->rectangle.dy2);
   Box over;
   CoplanarLinesResults coplanar;
   if ( a.Collides(&b,&coplanar,&over) ) Select(r);
  }
 }
 void Select( double x, double y, double x2, double y2, having inclusivity ) {
  ScreenPosition box; box.Set(x,y,x2,y2);
  EACH(selected.first,GridRectHandle,rh) if ( rh->p->rectangle.within(&box) ) Select(rh->p);
 }
 void Select( const char *substring ) { FOREACH(GridRect,r) if ( r->name.inside(substring) )Select(r); }
 void Rows( const char *named, Integers *n, double x, double y, double w, double h ) {
  int rows=n->count;
  GridRect outer("",x,y,w,h);
  Split(&outer,rows,1);
  Select();
  Integer *len=(Integer *) n->first;
  int i=1;
  EACHN(selected.first,GridRectHandle,grh,{
   selected.Remove(grh);
   Split(grh->p,this,1,len->i,FORMAT("%s-r%d",named,i).c_str());
   delete grh;
   len=(Integer *)len->next;
   i++;
  });
 }
 void Row( const char *named, int cols, double x, double y, double cw, double ch ) {
  GridRect r(named,x,y,cw*(double)cols,ch);
  for ( int i=0; i<cols; i++ ) Split(&r,this,1,cols,named);
 }
 void RowSplit( const char *named, int cols, double x, double y, double w, double h ) {
  GridRect r(named,x,y,w,h);
  for ( int i=0; i<cols; i++ ) Split(&r,this,1,cols,named);
 }
 void Columns( const char *named, Integers *n, double x, double y, double w, double h ) { Cols(named,n,x,y,w,h); }
 void Cols( const char *named, Integers *n, double x, double y, double w, double h ) {
  int cols=n->count;
  GridRect outer("",x,y,w,h);
  Split(&outer,1,cols);
  Select();
  Integer *len=(Integer *) n->first;
  int j=1;
  EACHN(selected.first,GridRectHandle,grh,{
   selected.Remove(grh);
   Split(grh->p,this,1,len->i,FORMAT("%s-r%d",named,j).c_str());
   delete grh;
   len=(Integer *)len->next;
   j++;
  });
 }
 void Column( const char *named, int rows, double x, double y, double cw, double ch ) {
  GridRect r(named,x,y,cw,ch*(double)rows);
  for ( int i=0; i<rows; i++ ) Split(&r,this,rows,1,named);
 }
 void ColumnSplit( const char *named, int rows, double x, double y, double w, double h ) {
  GridRect r(named,x,y,w,h);
  for ( int i=0; i<rows; i++ ) Split(&r,this,rows,1,named);
 }
 void Col( const char *named, int rows, double x, double y, double cw, double ch ) {
  Column(named,rows,x,y,cw,ch);
 }
 void Split( GridRect *rect, int rows, int columns, const char *prefix ) {
  if ( ListIteminList(rect) ) { // Replace this here.
   Remove(rect);
   Split( rect,this,rows,columns,prefix);
   delete rect;
  } else Split(rect,this,rows,columns,prefix);
 }
 void Split( GridRect *rect, int rows, int columns, having columns_instead_of_rows, const char *prefix ) {
  if ( ListIteminList(rect) ) { // Replace this here.
   Remove(rect);
   Split( rect,this,rows,columns,columns_instead_of_rows,prefix);
   delete rect;
  } else Split(rect,this,rows,columns,columns_instead_of_rows,prefix);
 }
 void Split( GridRect *rect, int rows, int columns ) {
  Split(rect,this,rows,columns,rect->name);
 }
 bool Split( int rows, int columns ) { return Split("",rows,columns); }
 bool Split( const char *named, int rows, int columns ) {
  GridRect *rect=find(named);
  if ( rect ) {
   Split(rect,this,rows,columns,rect->name);
   return true;
  }
  return false;
 }
 bool SplitGroup( const char *beginsWith, int rows, int columns ) {
  GridRectHandles matched;
  matches(beginsWith,&matched);
  if ( matched.count == 0 ) return false;
  EACHN(matched.first,GridRectHandle,rh,{
   Remove(rh->p);
   Split(rh->p,rows,columns);
   delete rh->p;
  });
  return true;
 }
 // Actually perform the split.
 void Split( GridRect *outer, GridRects *out, int rows, int columns, const char *prefix ) {
  this->outer.Set(outer->rectangle.dx,outer->rectangle.dy,outer->rectangle.dw,outer->rectangle.dh);
  if ( rows <= 0 || columns <= 0 ) return;
  if ( rows == 1 && columns == 1 ) {
   out->Add(prefix,outer->rectangle.dx,outer->rectangle.dy,outer->rectangle.dw,outer->rectangle.dh);
  } else if ( rows == 1 ) {
   double dw=outer->rectangle.dw/(double)columns;
   double dh=outer->rectangle.dh;
   double dx=outer->rectangle.dx;
   for ( int j=0; j<columns; j++ ) {
    out->Add(FORMAT("%s-col-%d",prefix,j+1).c_str(),dx,outer->rectangle.dy,dw,dh);
    dx+=dw;
   }
  } else if ( columns == 1 ) {
   double dw=outer->rectangle.dw;
   double dh=outer->rectangle.dh/(double)rows;
   double dy=outer->rectangle.dy;
   for ( int i=0; i<rows; i++ ) {
    out->Add(FORMAT("%s-row-%d",prefix,i+1).c_str(),outer->rectangle.dx,dy,dw,dh);
    dy+=dh;
   }
  } else {
   double dw=outer->rectangle.dw/(double)columns;
   double dh=outer->rectangle.dh/(double)rows;
   double dy=outer->rectangle.dy;
   for ( int i=0; i<rows; i++ ) {
    double dx=outer->rectangle.dx;
    for ( int j=0; j<columns; j++ ) {
     out->Add(FORMAT("%s-%dx%d",prefix,i+1,j+1).c_str(),dx,dy,dw,dh); // name-RowxCol
     dx+=dw;
    }
    dy+=dh;
   }
  }
 }
 // Actually perform the split.
 void Split( GridRect *outer, GridRects *out, int rows, int columns, having columns_instead_of_rows, const char *prefix ) {
  this->outer.Set(outer->rectangle.dx,outer->rectangle.dy,outer->rectangle.dw,outer->rectangle.dh);
  if ( rows <= 0 || columns <= 0 ) return;
  if ( rows == 1 && columns == 1 ) {
   out->Add(prefix,outer->rectangle.dx,outer->rectangle.dy,outer->rectangle.dw,outer->rectangle.dh);
  } else if ( rows == 1 ) {
   double dw=outer->rectangle.dw/(double)columns;
   double dh=outer->rectangle.dh;
   double dx=outer->rectangle.dx;
   for ( int j=0; j<columns; j++ ) {
    out->Add(FORMAT("%s-col-%d",prefix,j+1).c_str(),dx,outer->rectangle.dy,dw,dh);
    dx+=dw;
   }
  } else if ( columns == 1 ) {
   double dw=outer->rectangle.dw;
   double dh=outer->rectangle.dh/(double)rows;
   double dy=outer->rectangle.dy;
   for ( int i=0; i<rows; i++ ) {
    out->Add(FORMAT("%s-row-%d",prefix,i+1).c_str(),outer->rectangle.dx,dy,dw,dh);
    dy+=dh;
   }
  } else {
   double dw=outer->rectangle.dw/(double)columns;
   double dh=outer->rectangle.dh/(double)rows;
   double dx=outer->rectangle.dx;
   for ( int j=0; j<columns; j++ ) {
    double dy=outer->rectangle.dy;
    for ( int i=0; i<rows; i++ ) {
     out->Add(FORMAT("%s-%dx%d",prefix,i+1,j+1).c_str(),dx,dy,dw,dh); // name-RowxCol
     dy+=dh;
    }
    dx+=dw;
   }
  }
 }
 GridRect *find( const char *named ) {
  FOREACH(GridRect,r) if ( r->name == named ) return r;
  return null;
 }
 void matches( const char *beginsWith ) { FOREACH(GridRect,r) if ( r->name.begins(beginsWith) ) Select(r); }
 void matches( const char *beginsWith, GridRectHandles *out ) { FOREACH(GridRect,r) if ( r->name.begins(beginsWith) ) out->Add(r); }
 void keyword( const char *key, GridRectHandles *out ) { FOREACH(GridRect,r) if ( r->name.inside(key) ) out->Add(r); }
 void DrawDebug() { FOREACH(GridRect,r) r->DrawDebug(); }
 void Prefix( const char *p ) { if ( strlen(p) == 0 ) return; FOREACH(GridRect,r) r->name=string(p)+r->name; }
 void PrefixReplace( const char *search, const char *replace ) {
  FOREACH(GridRect,r) r->name(search,replace);
 }
 void FlipH() {
  FOREACH(GridRect,r) {
   r->Set(
    (1.0-(r->rectangle.dx/outer.rectangle.dx)) * r->rectangle.dw,
    r->rectangle.dy, r->rectangle.dw, r->rectangle.dh
   );
  }
 }
 void FlipV() {
  FOREACH(GridRect,r) {
   r->Set(
    r->rectangle.dx,
    (1.0-(r->rectangle.dy/outer.rectangle.dy)) * r->rectangle.dh,
    r->rectangle.dw, r->rectangle.dh
   );
  }
 }
 string Prefix( string s, const char *n ) { return Prefix(s.c_str(),n); }
 string Prefix( string s, string n ) { return Prefix(s.c_str(),n.c_str()); }
 string Prefix( const char *s, const char *n ) {
  if (strlen(s) > 0) return string(s)+string("-")+string(n);
  return string(n);
 }
 CLEARLISTRESET(GridRect);
};

class GridSplitter : public ListItem {
public:
 GridRect outer;
 GridRects sectors;
 GridRects remainders; // Areas left over on certain types of grids that don't fill the entire rectangle.
 GridSplitter() : ListItem() {}
 GridSplitter( GridSplits preset, double x, double y, double w, double h ) {
  outer.Set(x,y,w,h);
  Generate(preset,GridSplitsName(preset).c_str());
 }
 GridSplitter( const char *prefix, GridSplits preset, double x, double y, double w, double h ) {
  outer.Set(x,y,w,h);
  Generate(preset,prefix);
 } 
 GridSplitter( double x, double y, double w, double h, int rows, int columns, const char *prefix="" ) {
  outer.Set(x,y,w,h);
  sectors.Split(&outer,rows,columns,prefix);
 }
 GridSplitter( double x, double y, double w, double h, int rows, int columns, having columns_instead_of_rows, const char *prefix="" ) {
  outer.Set(x,y,w,h);
  sectors.Split(&outer,rows,columns,columns_instead_of_rows,prefix);
 }
 void DrawDebug() {
  remainders.DrawDebug();
  sectors.DrawDebug();
 }
 void Generate( GridSplits preset, const char *prefix="" );
 GridSplitter( const char *prefix, int total, double x, double y, double w, double h ) : ListItem() {
  outer.Set(x,y,w,h);
  Best(total);
 }
 GridSplitter( int majors, int minors ) : ListItem() { Best(majors,minors); }
 GridSplitter( int majors, int minors, int ancillary ) : ListItem() { Best(majors,minors,ancillary); }
 GridSplits Best( int total ) {
  GridAspects aspect=outer.Aspect();
  return gs_Quadrisect;
  //switch ( aspect ) {
  //}
 }
 GridSplits Best( int majors, int minors ) {
  GridAspects aspect=outer.Aspect();
  return gs_Quadrisect;
  //switch ( aspect ) {
  //}
 }
 GridSplits Best( int majors, int minors, int ancillary ) {
  GridAspects aspect=outer.Aspect();
  return gs_Quadrisect;
  //switch ( aspect ) {
  //}
 }
 // Merges without collision checking
 void Merge( GridSplitter *in ) {
  sectors.Concat(&in->sectors);
  remainders.Concat(&in->remainders);
  outer.Combine(&in->outer);
 }
 string Prefix( string s, const char *n ) { return Prefix(s.c_str(),n); }
 string Prefix( string s, string n ) { return Prefix(s.c_str(),n.c_str()); }
 string Prefix( const char *s, const char *n ) {
  if (strlen(s) > 0) return string(s)+string("-")+string(n);
  return string(n);
 }
};