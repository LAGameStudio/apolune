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

#include <Windows.h>

#include "macros.h"
#include "GLImage.h"
#include "Proce55or.h"
#include "Art.h"
#include "Art2d.h"
#include "Strings.h"
#include "Trigger.h"
#include "Tweens.h"

#include "Presentation.h"

enum AngleControllerModes {
 ac_oscillate,
 ac_teeter,
 ac_linear,
 ac_spin,
 ac_fixed
};

class AngleController : public ListItem {
public:
 Zp<Tween> t;
 Zdouble angle;
 AngleControllerModes mode;
 Zdouble start,end,d;
 Zdouble spin,delta;
 Zint direction;
 Zdouble d1,d2,d3,d4,d5;
 void Set(AngleController *in) {
  t=in->t;
  angle=in->angle;
  mode=in->mode;
  start=in->start;
  end=in->end;
  d=in->d;
  spin=in->spin;
  delta=in->delta;
  direction=in->direction;
  d1=in->d1;
  d2=in->d2;
  d3=in->d3;
  d4=in->d4;
  d5=in->d5;
 }
 KEYWORDS({
  KEYWORD("t",{t=tweens.find(w);})
  else KEYWORD("ang",{angle=w;})
  else KEYWORD("mode",{mode=(AngleControllerModes)atoi(w);})
  else NUMWORD("s",start)
  else NUMWORD("e",end)
  else NUMWORD("d",d)
  else NUMWORD("spin",delta)
  else NUMWORD("dir",direction)
  else NUMWORD("d1",d1)
  else NUMWORD("d2",d2)
  else NUMWORD("d3",d3)
  else NUMWORD("d4",d4)
  else NUMWORD("d5",d5)
 })
 WORDKEYS({
  if ( t ) WORDKEY("t",t->name)
  NUMBKEY("ang",angle)
  ENUMKEY("mode",mode)
  NUMBKEY("s",start)
  NUMBKEY("e",end)
  NUMBKEY("d",d)
  NUMBKEY("spin",delta)
  NUMBKEY("dir",direction)
  NUMBKEY("d1",d1)
  NUMBKEY("d2",d2)
  NUMBKEY("d3",d3)
  NUMBKEY("d4",d4)
  NUMBKEY("d5",d5)
 })
 AngleController() : ListItem() {
  mode=ac_fixed;
 }
 double T( double time ) { return t ? t->tweend(time) : time; }
 void Set( double start, double end ) {
  this->start=start;
  this->end=end;
  d=start < end ? end - start : start - end;
  direction=start > end ? -1 : 1;
  d1= direction * (d+d/2);
  d2=-direction * d;
  d3= direction * (d/2+d/4);
  d4=-direction * (d/4+d/8);
  d5= direction * (d/8);
 }
 void Set( double spin ) {
  this->spin=spin;
  this->delta=spin*(double)direction;
 }
 double Linear( double time ) {
  return start>end?((start-end)*T(time)+end):((end-start)*T(time)+start);
 }
 double Update( double time ) {
  switch ( mode ) {
   case ac_fixed: angle=start; return start;
   case ac_oscillate:
     angle = time < 0.5 ? Linear(t?T(time*2.0):time*2.0)
           : Linear(t?T((time-0.5)*2.0):(time-0.5)*2.0);
    break;
   case ac_teeter: 
     if ( time <= 0.2 ) {
      double tt=time/0.2;
      angle=d1*T(tt);
     } else if ( time <= 0.4 ) {
      double tt=(time-0.2)/0.2;
      angle=(d1)+d2*T(tt);
     } else if ( time <= 0.6 ) {
      double tt=(time-0.4)/0.2;
      angle=(d1+d2)+d3*T(tt);
     } else if ( time <= 0.8 ) {
      double tt=(time-0.6)/0.2;
      angle=(d1+d2+d3)+d4*T(tt);
     } else {
      double tt=(time-0.8)/0.2;
      angle=(d1+d2+d3+d4)+d4*tt;
     }
    break;
   case ac_linear: angle=Linear(time);
    break;
   case ac_spin: angle+=delta;
    break;
  }
  return angle;
 }
};

#include "Curve2d.h"
enum PointControllerModes { 
 pc_orbit,
 pc_curve,
 pc_lines,
 pc_line,   
 pc_fixed,   
};

class PointController2d : public ListItem {
public:
 Zp<Tween> t;
 Zint precision;
 Cartesian start,B,C,end;
 Zdouble d;
 Vertex curved;
 AngleController orbit;
 Zdisposable<Vertices> vertices;
 TweenHandles tween;
 PointControllerModes mode;
 KEYWORDS({
  KEYWORD("t",{t=tweens.find(w);})
  else KEYWORD("pre",{precision=w;})
  else KEYWORD("s",{start.fromString(w);})
  else KEYWORD("b",{B.fromString(w);})
  else KEYWORD("c",{C.fromString(w);})
  else KEYWORD("e",{end.fromString(w);})
  else NUMWORD("d",d)
  else KEYWORD("curve",{curved.fromString(w);})
  else KEYWORD("o",{orbit.fromString(w);})
  else KEYWORD("verts",{vertices.Recycle(); vertices->fromString(w);})
  else KEYWORD("tween",{ })
  else KEYWORD("mode",{mode=(PointControllerModes)atoi(w);})
 })
 WORDKEYS({
  if ( t ) WORDKEY("t",t->name);
  NUMBKEY("pre",precision);
  SSUBKEY("s",start);
  SSUBKEY("b",B);
  SSUBKEY("c",C);
  SSUBKEY("e",end);
  NUMBKEY("d",d);
  SSUBKEY("curve",curved);
  SSUBKEY("o",orbit);
  if ( vertices ) SUBKEY("verts",vertices);
  ENUMKEY("mode",mode)
 })
 void Set(PointController2d *in) {
  t=in->t;
  precision=in->precision;
  start.Set(&in->start);
  B.Set(&in->B);
  C.Set(&in->C);
  end.Set(&in->end);
  d=in->d;
  curved.Set(&in->curved);
  orbit.Set(&in->orbit);
  if ( in->vertices ) {
   vertices.Recycle();
   vertices->CopyAppend(in->vertices);
  }
  tween.Clear(); EACH(in->tween.first,TweenHandle,th) tween.Add(th->p);
  mode=in->mode;
 }
 PointController2d() : ListItem() {
  mode=pc_fixed;
  precision=16;
 }
 void Start( int x, int y ) {
  start.x=x;
  start.y=y;
  d=ddistance((double)x,(double)y,(double)end.x,(double)end.y);
 }
 void End( int x, int y ) {
  end.x=x;
  end.y=y;
  d=ddistance((double)x,(double)y,(double)end.x,(double)end.y);
 }
 void Path( int x, int y, int x2, int y2 ) {
  start.x=x;
  start.y=y;
  end.x=x2;
  end.y=y2;
  d=ddistance((double)x,(double)y,(double)end.x,(double)end.y);
 }
 void Curve( double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy ) {
  SetCurve(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy);
  Path((int)Ax,(int)Ay,(int)Dx,(int)Dy);
 }
 void SetCurve( double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy ) {
  vertices.Recycle(
   curve2d.Hermite(Ax,Ay,0.0,Bx,By,0.0,Cx,Cy,0.0,Dx,Dy,0.0)
  );
 }
 void Curved( double time ) {
  if ( vertices ) {
   vertices->GetPoint(&curved,time);
  } else {
   SetCurve((double)start.x,(double)start.y,(double)start.x,(double)start.y,(double)end.x,(double)end.y,(double)end.x,(double)end.y);
   vertices->GetPoint(&curved,time);
  }
 }
 void Waypoint( int x, int y ) {
  vertices->Add((float)x,(float)y,0.0f);
  tween.Add(tweens.find("One"));
 }
 void Waypoint( int x, int y, Tween *t ) {
  vertices->Add((float)x,(float)y,0.0f);
  tween.Add(t);
 }
 void Get( Cartesian *out, double time ) {
  switch ( mode ) {
   case pc_orbit:
     out->x = start.x+(int)((d)*cos(orbit.angle));
     out->y = start.y+(int)((d)*sin(orbit.angle));
    break;
   case pc_curve:
     Curved(time);
     out->x=(int)curved.x;
     out->y=(int)curved.y;
    break;
   case pc_lines:
    {
     Vertex result;
     vertices->GetPoint(&result,time,&tween);
     out->x=(int)result.x;
     out->y=(int)result.y;
    }
    break;
   case pc_line:
     out->x=start.x+(int)(time*(end.x-start.x));
     out->y=start.y+(int)(time*(end.y-start.y));
    break;
   case pc_fixed:
     out->x=start.x;
     out->y=start.y;
    break;
  }
 }
};

class SizeController2d : public ListItem {
public:
 TweenHandles tween;
 Vertices sizes;
 void Set(SizeController2d *in) {
  tween.Clear(); EACH(in->tween.first,TweenHandle,th) tween.Add(th->p);
  sizes.CopyAppend(&in->sizes);
 }
 KEYWORDS({
  KEYWORD("tween",{ })
  else KEYWORD("sizes",{ sizes.fromString(w); })
 })
 WORDKEYS({
//  WORDKEY("tween")
  SSUBKEY("sizes",sizes)
 })
 void Add( int w, int h ) {
  sizes.Add((float)w,(float)h,0.0f);
  tween.Add(tweens.find("One"));
 }
 void Add( int w, int h, Tween *t ) {
  sizes.Add((float)w,(float)h,0.0f);
  tween.Add(t?t:tweens.find("One"));
 }
 void Get( Cartesian *out, double time ) {
  if ( sizes.count == 0 ) return;
  if ( sizes.count == 1 ) {
   Tween *t=((TweenHandle *)tween.Element(0))->p;
   Vertex *A=(Vertex *)sizes.Element(0);
   Vertex B;
   Line line((double)A->x,(double)A->y,(double)B.x,(double)B.y);
   Vertexd result;
   line.Point(time,&result);
   out->x=(int)result.x;
   out->y=(int)result.y;
   return;
  }
  if ( sizes.count == 2 ) {
   Vertex *A=(Vertex *)sizes.Element(0);
   Vertex *B=(Vertex *)sizes.Element(1);
   Line line((double)A->x,(double)A->y,(double)B->x,(double)B->y);
   Vertexd result;
   line.Point(time,&result);
   out->x=(int)result.x;
   out->y=(int)result.y;
   return;
  }
  int tweenNumber=UMAX(0,(int)(abs(time)*(double)tween.count-2));
  Tween *t=((TweenHandle *)tween.Element(tweenNumber))->p;
  Vertex *A=(Vertex *)sizes.Element(tweenNumber);
  Vertex *B=(Vertex *)sizes.Element(tweenNumber+1);
  Line line((double)A->x,(double)A->y,(double)B->x,(double)B->y);
  Vertexd result;
  line.Point(time,&result);
  out->x=(int)result.x;
  out->y=(int)result.y;
 }
};

class CrayonController : public ListItem {
public:
 Crayons colors;
 TweenHandles tween;
 void Add( Crayon c ) {
  colors.Hold(c);
  tween.Add(tweens.find("One"));
 }
 void Add( Crayon c, Tween *t ) {
  colors.Hold(c);
  tween.Add(t?t:tweens.find("One"));
 }
 Crayon Get( double time ) {
  if ( colors.count == 0 ) return crayons.Pick(clear);
  if ( colors.count == 1 ) return ((CrayonHandle *)colors.first)->color;
  if ( colors.count == 2 ) {
   Crayon A=((CrayonHandle *)colors.first)->color;
   Crayon B=((CrayonHandle *)colors.last)->color;
   Crayon actual;
   Tween *t=((TweenHandle *)tween.Element(0))->p;
   actual.ColorInterp(A,B,t->tweend(time));
   return actual;
  }
  int crayonNumber=(int)(time*(double)colors.count-2);
  Crayon actual;
  Crayon A=((CrayonHandle *)colors.Element(crayonNumber))->color;
  Crayon B=((CrayonHandle *)colors.Element(crayonNumber+1))->color;
  Tween *t=((TweenHandle *)tween.Element(crayonNumber))->p;
  actual.ColorInterp(A,B,t->tweend(time));
  return actual;
 }
};

// Maximal and generally useful.  Uses PointController
class Visual : public Proce55or {
public:
 Zp<Presentation> p;
 Zp<GLImage> image;
 Zdouble in, out, delayplusin,delayplusout,delayplusoutpH,delayplusdurationmH; // delta points
 PointController2d location;
 AngleController rotation;
 SizeController2d size;
 Cartesian position,scale,pivot;
 CrayonController color;
 Blends blend;
 Zdouble time;
 Visual() : Proce55or() {
  blend=additive;
 }
 virtual void Setup() {
  p=(Presentation *)parent.pointer;
  location.Get(&position,0.0);
  if ( image && size.sizes.count == 0 ) size.Add(image->width,image->height);
  size.Get(&scale,0.0);
  x=position.x;
  y=position.y;
  delayplusin=delay+in;
  delayplusout=delay+out;
  delayplusoutpH=delay+out+H;
  delayplusdurationmH=(delay+duration)-H;
  time=0.0;
 }
 virtual void Render( GLWindow *surace ) { Render(); }
 virtual void Draw( GLWindow *surface ) { Render(); }
 virtual void Between() {
  if ( p ) {
   if ( !hidden ) {
    double H2=p->expired-(double)delay;
    time=(H2/(double)duration);
    if ( time <= 1.0 ) {
     rotation.Update(time);
     size.Get(&scale,time);
     location.Get(&position,time);
     x=position.x;
     y=position.y;
     x2=position.x+scale.x;
     y2=position.y+scale.y;
    }
   } else {
    if ( p->expired>delay && p->expired<delayplusdurationmH ) { hidden=false; return; }
   }
  }
 }
 virtual void Render() {
  if ( !hidden ) {
   if ( image && rotation.mode != ac_fixed ) {
    PivotedRotatedRectf(
     image,color.Get(time),blend,
     (float)position.x,(float)position.y,
     pivot.fx,pivot.fy,
     iratiof(scale.x,2),iratiof(scale.y,2),
     (float)rotation.angle
    );
   } else {
    if ( image ) StretchiNoAlpha(
     image,color.Get(time),blend,
     position.x,position.y,
     scale.x,scale.y
    );
   }
  }
 }
};


// Simplified and minimal.
class Aid : public Proce55or {
public:
 Zpointer<GLImage> image;
 Crayon tint;
 Blends blend;
 Zdouble expired;
 Zbool xreversed;
 Aid() : Proce55or() {
  tint.Pick(alabaster);
 }
 Aid( int x, int y, GLImage *image ) : Proce55or() {
  this->x=x;
  this->y=y;
  this->blend=transparency;
  this->image=image;
  tint.Pick(alabaster);
  this->w=image->width;
  this->h=image->height;
 }
 virtual void Render( GLWindow *surace ) { Render(); }
 virtual void Draw( GLWindow *surface ) { Render(); }
 virtual void Between() {}
 virtual void Render() {
  if ( !hidden ) {
   if ( xreversed ) XReversibleStretchiNoAlpha(parent,image,tint,blend,x,y,w,h,true);
   else QuadiNoAlpha(image,tint,blend,x,y,w,h);
  }
 }
};

class VisualAid : public Proce55or
{
public:
 Zpointer<GLImage> image,mask,solarize,filter;
 Zint sx,sy,dx,dy,width,height,w2,h2;
 Zdouble in, out, delayplusin,delayplusout,delayplusoutpH,delayplusdurationmH; // delta points
 Blends blend;
 Crayon tint_start;
 Crayon tint_mid;
 Crayon tint_end;
 Crayon actual;
 Zint start, mid, end; // delta targetted changes in opaciy
 Zbool visible,flicker_out,perpetuate;
 Zint repeats,repeated;
 Zpointer<Presentation> p;

 // "Private"
 Zint opacity;
 Zdouble H;

 VisualAid() : Proce55or() {
  Opacity(0);
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_mid.Float( 1.0f, 1.0f, 1.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  visible=true;
  repeats=1;
  blend=additive;
  H=1.0f/FPS;
 }

 void Setup() {
  p=(Presentation *)parent.pointer;
  x=sx;
  y=sy;
  width=w;
  height=h;
  delayplusin=delay+in;
  delayplusout=delay+out;
  delayplusoutpH=delay+out+H;
  delayplusdurationmH=(delay+duration)-H;
 }

 void Between() {
  if ( !visible ) return;
  // we're on: let's update our position and size
  double H1=(double)p->expired-delayplusout;
  double H2=(double)p->expired-delay;
  double time=H2/duration;
  if ( p->expired>delayplusdurationmH ) { visible=false; return; }
  x=iround( (double)sx+(double)dx*time );
  y=iround( (double)sy+(double)dy*time );
  width=iround((double)w+(double)(w2-w)*time);
  height=iround((double)h+(double)(h2-h)*time);
  if ( p->expired < delayplusin ) { // if we're before the fade-in duration marker,
   double H3=H2/in;
   opacity=ilerp(start,mid,H3);
   actual.ColorInterp( tint_start, tint_mid, H3 );    
  } else
  if ( p->expired < delayplusout && p->expired < delayplusoutpH ) { // or we're after the fade-in and before the holding limit; also, limit #calls? I wonder about this part but it may help
   opacity=mid; 
   actual.fromCrayon(tint_mid);
  } else { // or if we're past the hold and fading out...
   double H3=H1/(duration-out);
   opacity=ilerp(mid,end,H3);
   actual.ColorInterp( tint_mid, tint_end, H3 );
  }  
 }

 void Reset(bool paused) {
  opacity=start;
  x=sx;
  y=sy;
  width=w;
  height=h;
  visible=true;
  repeated=0;
  actual.fromCrayon(tint_start);
 }

 void Opacity( byte val ) { start=mid=end=opacity=val; }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_mid.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b, Crayon c ) {
  tint_start.fromCrayon(a);
  tint_mid.fromCrayon(b);
  tint_end.fromCrayon(c);
 }

 virtual void Draw(GLWindow *surface) {
   if ( visible ) {
    if ( image ) StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
   }
 }
 
 // draw surface to surface
 virtual void Redraw() {
  Draw(parent);
 } 

};

class VisualAids : public LinkedList {
public:
 CLEARLISTRESET(VisualAid);
};


class VisualAidAdditive : public VisualAid {
public:
 virtual void Draw(GLWindow *surface) {
   if ( visible ) {
     if ( image ) Additivei(surface,image,actual,x,y,width,height);
   }
 }
};


class VisualAid2 : public Proce55or
{
public:
 Zpointer<GLImage> image,mask,solarize,filter;
 Zint sx,sy,dx,dy,width,height,w2,h2;
 Zdouble in, out, delayplusin,delayplusout,delayplusoutpH,delayplusdurationmH; // delta points
 Blends blend;
 Crayon tint_start;
 Crayon tint_mid;
 Crayon tint_end;
 Crayon actual;
 Zint start, mid, end; // delta targetted changes in opaciy
 Zbool visible,flicker_out,perpetuate;
 Zint repeats,repeated;

 // "Private"
 Zint opacity;
 Zdouble H;

 VisualAid2() : Proce55or() {
  Opacity(255);
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_mid.Float( 1.0f, 1.0f, 1.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  visible=true;
  repeats=1;
  blend=additive;
  H=FRAMETIME;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  width=w;
  height=h;
  delayplusin=delay+in;
  delayplusout=delay+out;
  delayplusoutpH=delay+out+H;
  delayplusdurationmH=(delay+duration)-H;
 }

 void Between() {
  if ( !visible ) return;
  expired+=FRAMETIME;
  // we're on: let's update our position and size
  double H1=expired-delayplusout;
  double H2=expired-delay;
  double time=H2/duration;
  if ( expired>delayplusdurationmH ) { visible=false; return; }
  x=iround( (double)sx+(double)dx*time );
  y=iround( (double)sy+(double)dy*time );
  width=iround((double)w+(double)(w2-w)*time);
  height=iround((double)h+(double)(h2-h)*time);
  x2=x+width;
  y2=y+height;
  if ( expired < delayplusin ) { // if we're before the fade-in duration marker,
   double H3=H2/in;
   opacity=ilerp(start,mid,H3);
   actual.ColorInterp( tint_start, tint_mid, H3 );    
  } else
  if ( expired < delayplusout && expired < delayplusoutpH ) { // or we're after the fade-in and before the holding limit; also, limit #calls? I wonder about this part but it may help
   opacity=mid; 
   actual.fromCrayon(tint_mid);
  } else { // or if we're past the hold and fading out...
   double H3=H1/(duration-out);
   opacity=ilerp(mid,end,H3);
   actual.ColorInterp( tint_mid, tint_end, H3 );
  }  
 }

 void Reset(bool paused) {
  opacity=start;
  x=sx;
  y=sy;
  width=w;
  height=h;
  visible=true;
  repeated=0;
  actual.fromCrayon(tint_start);
  expired=0.0f;
 }

 void Opacity( byte val ) { start=mid=end=opacity=val; }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_mid.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b, Crayon c ) {
  tint_start.fromCrayon(a);
  tint_mid.fromCrayon(b);
  tint_end.fromCrayon(c);
 }

 void Render(GLWindow *surface) { Draw(surface); }
 void Render() { Draw(parent); }
 virtual void Draw(GLWindow *surface) {
   if ( visible ) {
    if ( image ) StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
   }
 } 
 
 virtual void MoveBy( int byx, int byy ) { x+=byx; y+=byy; x2+=byx; y2+=byy; sx+=byx; sy+=byy; }

 virtual void Redraw(GLWindow *surface) { Draw(surface); } 
 virtual void Redraw() { Draw(parent); } 

};


// Not really implemented, just a copy of above (please fix)
class VisualRotated : public Proce55or
{
public:
 Zpointer<GLImage> image,mask,solarize,filter;
 Zint sx,sy,dx,dy,width,height,w2,h2;
 Zdouble in, out, delayplusin,delayplusout,delayplusoutpH,delayplusdurationmH; // delta points
 Blends blend;
 Crayon tint_start;
 Crayon tint_mid;
 Crayon tint_end;
 Crayon actual;
 Zint start, mid, end; // delta targetted changes in opaciy
 Zbool visible,flicker_out,perpetuate;
 Zint repeats,repeated;
 Zdouble start_angle,angle,spin;
 Cartesian pivot;

 // "Private"
 Zint opacity;
 Zdouble H;

 VisualRotated() : Proce55or() {
  Opacity(255);
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_mid.Float( 1.0f, 1.0f, 1.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  visible=true;
  repeats=1;
  blend=additive;
  H=FRAMETIME;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  width=w;
  w2=w/2;
  height=h;
  h2=h/2;
  delayplusin=delay+in;
  delayplusout=delay+out;
  delayplusoutpH=delay+out+H;
  delayplusdurationmH=(delay+duration)-H;
  expired=0.0f;
 }

 void Between() {
  if ( !visible ) return;
  angle+=spin;
  expired+=FRAMETIME;
  // we're on: let's update our position and size
  double H1=expired-delayplusout;
  double H2=expired-delay;
  double time=H2/duration;
  if ( expired>delayplusdurationmH ) { visible=false; return; }
  x=iround( (double)sx+(double)dx*time );
  y=iround( (double)sy+(double)dy*time );
  width =iround( (double) w+(double) (w2-w)*time);
  height=iround( (double) h+(double) (h2-h)*time);
  x2=x+width;
  y2=y+height;
  if ( expired < delayplusin ) { // if we're before the fade-in duration marker,
   double H3=H2/in;
   opacity=ilerp(start,mid,H3);
   actual.ColorInterp( tint_start, tint_mid, H3 );    
  } else
  if ( expired < delayplusout && expired < delayplusoutpH ) { // or we're after the fade-in and before the holding limit; also, limit #calls? I wonder about this part but it may help
   opacity=mid; 
   actual.fromCrayon(tint_mid);
  } else { // or if we're past the hold and fading out...
   double H3=H1/(duration-out);
   opacity=ilerp(mid,end,H3);
   actual.ColorInterp( tint_mid, tint_end, H3 );
  }  
 }

 void Reset(bool paused) {
  opacity=start;
  x=sx;
  y=sy;
  width=w;
  height=h;
  visible=true;
  repeated=0;
  actual.fromCrayon(tint_start);
  expired=0.0f;
  angle=start_angle;
 }

 void Opacity( byte val ) { start=mid=end=opacity=val; }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_mid.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b, Crayon c ) {
  tint_start.fromCrayon(a);
  tint_mid.fromCrayon(b);
  tint_end.fromCrayon(c);
 }

 virtual void MoveBy( int byx, int byy ) { x+=byx; y+=byy; x2+=byx; y2+=byy; sx+=byx; sy+=byy; }
 virtual void Draw(GLWindow *surface) {
  if ( visible ) {
   if (image) PivotedRotatedRectf(image,actual,blend,(float)x,(float)y,pivot.fx,pivot.fy,(float)w2,(float)h2,(float)angle);
  }
 }
 virtual void Redraw(GLWindow *surface) { Draw(surface); }
 virtual void Redraw() { Draw(parent); }
 virtual void Render() { Draw(parent); }
};


// Lets you determine extents of an aid and control its movement on the screen.
//
//   sA ------ sB      dA ----- dB
//      |    |    ==>     |   |
//   sC ------ sD      dC ----- dD
//
// Perpetual.
class VisualSkewed : public Proce55or {
public:
 Zp<GLImage> image,mask,solarize,filter;
 Cartesian
  sA,sB,sC,sD,
  pA,pB,pC,pD,
  dA,dB,dC,dD;
 Zdouble in, out, delayplusin,delayplusout,delayplusoutpH,delayplusdurationmH; // delta points
 Blends blend;
 Crayon tint_start;
 Crayon tint_mid;
 Crayon tint_end;
 Crayon actual;
 Zint start, mid, end; // delta targetted changes in opaciy
 Zbool flicker_out,perpetuate;
 Zint repeats,repeated;

 // "Private"
 Zint opacity;
 Zdouble H;

 VisualSkewed() : Proce55or() {
  Opacity(255);
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_mid.Float( 1.0f, 1.0f, 1.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  H=1.0f/FPS;
 }
 
 void Setup() {
  pA=sA;
  pB=sB;
  pC=sC;
  pD=sD;
  delayplusin=delay+in;
  delayplusout=delay+out;
  delayplusoutpH=delay+out+H;
  delayplusdurationmH=(delay+duration)-H;
  expired=0.0f;
  RecalculatePosition();
 }

 void RecalculatePosition() {
  int smallestX=pA.x;
  if ( smallestX > pB.x ) smallestX = pB.x;
  if ( smallestX > pC.x ) smallestX = pC.x;
  if ( smallestX > pD.x ) smallestX = pD.x;
  int smallestY=pA.y;
  if ( smallestY > pB.y ) smallestY = pB.y;
  if ( smallestY > pC.y ) smallestY = pC.y;
  if ( smallestY > pD.y ) smallestY = pD.y;
  int biggestX=pA.x;
  if ( biggestX < pB.x ) biggestX = pB.x;
  if ( biggestX < pC.x ) biggestX = pC.x;
  if ( biggestX < pD.x ) biggestX = pD.x;
  int biggestY=pA.y;
  if ( biggestY < pB.y ) biggestY = pB.y;
  if ( biggestY < pC.y ) biggestY = pC.y;
  if ( biggestY < pD.y ) biggestY = pD.y;
  w=biggestX-smallestX;
  h=biggestY-smallestY;
  x=smallestX;
  y=smallestY;
  x2=x+w;
  y2=y+h;
 }

 void Between() {
  if ( hidden ) return;
  expired+=FRAMETIME;
  if ( expired>delayplusdurationmH ) return;
  // we're on: let's update our position and size
  double H1=expired-delayplusout;
  double H2=expired-delay;
  double time=H2/duration;
  pA.x=iround( (double)sA.x+(double)dA.x*time); pA.y=iround( (double)sA.y+(double)dA.y*time );
  pB.x=iround( (double)sB.x+(double)dB.x*time); pB.y=iround( (double)sB.y+(double)dB.y*time );
  pC.x=iround( (double)sC.x+(double)dC.x*time); pC.y=iround( (double)sC.y+(double)dC.y*time );
  pD.x=iround( (double)sD.x+(double)dD.x*time); pD.y=iround( (double)sD.y+(double)dD.y*time );
  RecalculatePosition();
  if ( expired < delayplusin ) { // if we're before the fade-in duration marker,
   double H3=H2/in;
   opacity=ilerp(start,mid,H3);
   actual.ColorInterp( tint_start, tint_mid, H3 );    
  } else
  if ( expired < delayplusout && expired < delayplusoutpH ) { // or we're after the fade-in and before the holding limit; also, limit #calls? I wonder about this part but it may help
   opacity=mid; 
   actual.fromCrayon(tint_mid);
  } else { // or if we're past the hold and fading out...
   double H3=H1/(duration-out);
   opacity=ilerp(mid,end,H3);
   actual.ColorInterp( tint_mid, tint_end, H3 );
  }  
 }

 void Reset(bool paused) {
  pA=sA;
  pB=sB;
  pC=sC;
  pD=sD;
  RecalculatePosition();
  opacity=start;
  repeated=0;
  actual.fromCrayon(tint_start);
  expired=0.0f;
 }

 void Opacity( byte val ) { start=mid=end=opacity=val; }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_mid.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b, Crayon c ) {
  tint_start.fromCrayon(a);
  tint_mid.fromCrayon(b);
  tint_end.fromCrayon(c);
 }

 virtual void Draw(GLWindow *surface) {
   if ( !hidden ) {
    Swabi(surface,image,actual,1.0f,blend,pA.x,pA.y,pB.x,pB.y,pC.x,pC.y,pD.x,pD.y);
   }
 } 
 
 virtual void MoveBy( int byx, int byy ) { 
  sA.x+=byx; dA.x+=byx;
  sA.y+=byy; dA.y+=byy;
  sB.x+=byx; dB.x+=byx;
  sB.y+=byy; dB.y+=byy;
  sC.x+=byx; dC.x+=byx;
  sC.y+=byy; dC.y+=byy;
  sD.x+=byx; dD.x+=byx;
  sD.y+=byy; dD.y+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
   if ( !hidden ) {
    Swabi(surface,image,actual,1.0f,blend,pA.x,pA.y,pB.x,pB.y,pC.x,pC.y,pD.x,pD.y);
   }
 } 

 // draw surface to surface
 virtual void Redraw() {
  Draw(parent);
 } 

};




class WobblingVisualAid2 : public Proce55or {
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod;
 Zfloat wanderSpeed, wanderSpeedLimit, wanderAccel;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;

 // "Private"
 Zint opacity;

 WobblingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeed=wanderSpeedLimit=1.0f;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
 }

 void Between() {
  // we're on: let's update our position and size
  expired+=FRAMETIME;
  wanderSpeed+=wanderAccel;
  if ( wanderSpeed > wanderSpeedLimit ) {
   wanderAccel=-wanderAccel;
   wanderSpeed=wanderSpeedLimit;
  } else if ( wanderSpeed < 0.0f ) {
   wanderAccel=-wanderAccel;
   wanderSpeed=wanderSpeedLimit;
  }
  actual.ColorInterp( tint_start, tint_end, wanderSpeed/wanderSpeedLimit );
  int xDirection=(int)((float)(1.0-(2.0*uniform()))*wanderSpeed);
  int yDirection=(int)((float)(1.0-(2.0*uniform()))*wanderSpeed);
  int movedX=x+xDirection;
  int movedY=y+yDirection;
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  actual.fromCrayon(tint_start);
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,w,h);
 }

};


class WanderingVisualAid2 : public Proce55or {
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod;
 Zfloat wanderSpeed, wanderSpeedMinimum, wanderSpeedLimit, wanderAccel;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;

 // "Private"
 Zint opacity;

 WanderingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeedMinimum=wanderSpeed=wanderSpeedLimit=1.0f;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
 }

 void Between() {
  // we're on: let's update our position and size
  wanderSpeed+=wanderAccel;
  if ( wanderSpeed > wanderSpeedLimit ) {
   wanderAccel=-wanderAccel;
   wanderSpeed=wanderSpeedLimit;
  } else if ( wanderSpeed < wanderSpeedMinimum) {
   wanderAccel=-wanderAccel;
   wanderSpeed=wanderSpeedMinimum;
  }
  actual.ColorInterp( tint_start, tint_end, wanderSpeed/wanderSpeedLimit );
  int xDirection=(int)((float)(1.0-(2.0*uniform()))*wanderSpeed);
  int yDirection=(int)((float)(1.0-(2.0*uniform()))*wanderSpeed);
  int movedX=x+xDirection;
  int movedY=y+yDirection;
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  actual.fromCrayon(tint_start);
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  if ( image ) StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  if ( image ) StretchiNoAlpha(surface,image,actual,blend,x,y,w,h);
 }

};


class DriftingVisualAid2 : public Proce55or {
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod,xDirection,yDirection;
 Zfloat wanderSpeed,tinting,delta;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;

 // "Private"
 Zint opacity;

 DriftingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeed=1.0f;
  delta=1.0f/ (float) FPS;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
  xDirection=(int) (RandomSign()*wanderSpeed);
  yDirection=(int) (RandomSign()*wanderSpeed);
 }

 float RandomSign() {
  if ( number_range(0,50) % 2 == 1 ) return -1.0f;
  else return 1.0f;
 }

 void Between() {
  tinting+=delta;
  if ( tinting > 1.0f ) {
   tinting=1.0f;
   delta=-delta;
  } else if ( tinting < 0.0f ) {
   tinting=0.0f;
   delta=-delta;
  }
  actual.ColorInterp( tint_start, tint_end, tinting );
  int movedX=x+(xDirection);
  int movedY=y+(yDirection);
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  tinting=0.0f;
  delta=1.0f/ (float) FPS;
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,w,h);
 }

};


class DriftingStretchingVisualAid2 : public Proce55or {
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod,xDirection,yDirection;
 Zfloat wanderSpeed,tinting,delta;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;
 Zfloat angle;

 // "Private"
 Zint opacity;

 DriftingStretchingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeed=1.0f;
  delta=1.0f/ (float) FPS;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
  xDirection=(int) (RandomSign()*wanderSpeed);
  yDirection=(int) (RandomSign()*wanderSpeed);
 }

 float RandomSign() {
  if ( number_range(0,50) % 2 == 1 ) return -1.0f;
  else return 1.0f;
 }

 void Between() {
  tinting+=delta;
  if ( tinting > 1.0f ) {
   tinting=1.0f;
   delta=-delta;
  } else if ( tinting < 0.0f ) {
   tinting=0.0f;
   delta=-delta;
  }
  width+=(int) delta;
  if ( width > w2*3 ) width=w2;
  actual.ColorInterp( tint_start, tint_end, tinting );
  int movedX=x+(xDirection);
  int movedY=y+(yDirection);
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  tinting=0.0f;
  delta=1.0f/ (float) FPS;
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,w,h);
 }

};


class DriftingRotatingVisualAid2 : public Proce55or
{
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod,xDirection,yDirection;
 Zfloat wanderSpeed,tinting,delta;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;
 Zfloat angle,deltaAngle;

 // "Private"
 Zint opacity;

 DriftingRotatingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeed=1.0f;
  delta=1.0f/FPS;
  deltaAngle=0.5f;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w=width;
  h=height;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
  xDirection=(int) (RandomSign()*wanderSpeed);
  yDirection=(int) (RandomSign()*wanderSpeed);
 }

 float RandomSign() {
  if ( number_range(0,50) % 2 == 1 ) return -1.0f;
  else return 1.0f;
 }

 void Between() {
  tinting+=delta;
  if ( tinting > 1.0f ) {
   tinting=1.0f;
   delta=-delta;
  } else if ( tinting < 0.0f ) {
   tinting=0.0f;
   delta=-delta;
  }
  angle+=deltaAngle;
//  if ( angle < -360.0f || angle > 360.0f ) deltaAngle=-deltaAngle;
  actual.ColorInterp( tint_start, tint_end, tinting );
  int movedX=x+(xDirection);
  int movedY=y+(yDirection);
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  h2=h/2;
  w2=w/2;
  tinting=0.0f;
  delta=1.0f/ (float) FPS;
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  if ( image ) art.Pivot(image,actual,blend,(double) x, (double) y,0.0,0.0,(double) h2,(double) w2,angle);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  Draw(surface);
 }

};


class DriftingStretchingRotatingVisualAid2 : public Proce55or {
public:
 Zp<Cartesian> wrapLimit;
 Zp<GLImage> image;
 Zint sx,sy,width,height,w2,h2,widthMod,heightMod,xDirection,yDirection;
 Zfloat wanderSpeed,tinting,delta;
 Blends blend;
 Crayon tint_start,tint_end;
 Crayon actual;

 // "Private"
 Zint opacity;

 DriftingStretchingRotatingVisualAid2() : Proce55or() {
  tint_start.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  tint_end.Float( 0.0f, 0.0f, 0.0f, 1.0f );
  blend=additive;
  wanderSpeed=1.0f;
  delta=1.0f/ (float) FPS;
 }
 
 void Setup() {
  x=sx;
  y=sy;
  w2=w/2;
  h2=h/2;
  x2=x+w;
  y2=y+h;
  xDirection=(int) (RandomSign()*wanderSpeed);
  yDirection=(int) (RandomSign()*wanderSpeed);
 }

 void Between() {
  tinting+=delta;
  if ( tinting > 1.0f ) {
   tinting=1.0f;
   delta=-delta;
  } else if ( tinting < 0.0f ) {
   tinting=0.0f;
   delta=-delta;
  }
  actual.ColorInterp( tint_start, tint_end, tinting );
  int movedX=x+(xDirection);
  int movedY=y+(yDirection);
  if ( wrapLimit ) {
   if ( movedX < wrapLimit->x  ) MoveBy(wrapLimit->w,0);
   if ( movedX > wrapLimit->x2 ) MoveBy(-wrapLimit->w,0);
   if ( movedY < wrapLimit->y  ) MoveBy(0,wrapLimit->h);
   if ( movedY > wrapLimit->y2 ) MoveBy(0,-wrapLimit->h);
  }
  MoveBy(xDirection,yDirection);
  w=width;
  h=height;
 }

 void Reset(bool paused) {
  x=sx;
  y=sy;
  x2=x+w;
  y2=y+h;
  w=width;
  h=height;
  tinting=0.0f;
  delta=1.0f/ (float) FPS;
 }

 void Tint( Crayon c ) {
  tint_start.fromCrayon(c);
  tint_end.fromCrayon(c);
 }

 void Tints( Crayon a, Crayon b ) {
  tint_start.fromCrayon(a);
  tint_end.fromCrayon(b);
 }

 virtual void Draw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,width,height);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  sx+=byx; 
  sy+=byy;
 }

 virtual void Redraw(GLWindow *surface) {
  StretchiNoAlpha(surface,image,actual,blend,x,y,w,h);
 }

};


// Legacy, not supported in iOS and who cares about it
#if defined(NEVER)
#include "DisplayList.h"

class MultiVisualAidDL : public DisplayList {
public:
 Cartesian *position;
 void Instructions() {
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);  glVertex2f(position->fx,position->fy);
  glTexCoord2f(1.0f, 1.0f);  glVertex2f(position->fx+position->fw,position->fy);
  glTexCoord2f(1.0f, 0.0f);  glVertex2f(position->fx+position->fw,position->fy+position->fh);
  glTexCoord2f(0.0f, 0.0f);  glVertex2f(position->fx,position->fy+position->fh);
  glEnd();
 }
};

class MultiVisualAid : public Proce55or {
public:
 GLImage *image;
 Cartesians positions;
 DisplayLists lists;
 Blends blend;
 Crayon tint;
 void Add( float x, float y, float w, float h ) {
  Cartesian *c=new Cartesian;
  c->SetRectf(x,y,w,h);
  positions.Append(c);
 }
 void Setup() {
  lists.Clean();
  lists.Clear();
  EACH(positions.first,Cartesian,c) {
   MultiVisualAidDL *dl=new MultiVisualAidDL;
   dl->position=c;
   lists.Append(dl);
  }
  lists.UploadAsOne();
 }
 void Clean() {
  lists.CleanAndClear();
  positions.Clear();
 }
 void Draw(GLWindow *surface) {
  if ( hidden ) return;
  glReportError( glGetError() );
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  Blending(blend);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, image->texture);
  glColor4fv(tint.floats); 
  lists.Call(); 
  glDisable(GL_TEXTURE_2D);
  glPopAttrib();
 }
};
#endif