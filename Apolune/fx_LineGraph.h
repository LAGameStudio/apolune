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

#include "Proce55or.h"
#include "Vertex.h"
#include "Trigger.h"

/*
 * A "LineGraph" is a tool that lets you add and remove points from a linear region.
 * Points are returned normalized (in 1.0 unit space)
 *
 * To add a division to the linegraph, the user clicks near the line between two established points.
 * The points are "cleared" to a default value of 2 points.
 */

class fx_LineGraph : public Proce55or {
public:
 Vertices points;
 Zpointer<Vertex> closest, less, greater, nearest, selecting;
 Vertexd probe;
 ScreenPosition screen,probeActual;
 Zdouble lineDist,minimumDistance;
 Zfloat dist,nearestDist;
 Zfloat ydist;
 Zint mx,my;
 Zbool shift,ctrl;
 Zbool active,/*horiz,*/result;
 fx_LineGraph() : Proce55or() {
  ClearPoints();
  SetExtents(0,0,384,384);
 }
 void ClearPoints() {
  points.Clear();
  points.Add(0.5f,0.0f,0.0f);
  points.Add(0.5f,1.0f,0.0f);
 }
 void OnSetExtents() {
  screen.Set(
   (float)(x+(parent?parent->x:0)),
   (float)(y+(parent?parent->y:0))
  );
  minimumDistance=5.0/(double)h;
 }
 void Between() {
  shift=input.KeyDown(LShift) || input.KeyDown(RShift);
  ctrl=input.KeyDown(LCtrl) || input.KeyDown(RCtrl);
  mx=(int)(input.mxi-x-(parent?parent->x:0));
  my=(int)(input.myi-y-(parent?parent->y:0));
  active=WITHIN(mx,my,0,0,w,h);
  probe.Set(iratiod(mx,w),iratiod(my,h));
  probeActual.Set(screen.x+probe.x*(double)w,screen.y+probe.y*(double)h);
  if ( active ) {
   closest=points.closestY((float)probe.y,&dist.value);
   nearest=points.closest((float)probe.x,(float)probe.y,0.0f,&nearestDist.value);
   dist=0.0f;
   if ( input.rightReleased() && selecting ) {
    points.Delete(selecting);
    selecting=null;
   }
   if ( input.left && shift ) { // Free Move
    if ( nearest ) {
     nearest->x=(float)probe.x;
     nearest->y=(float)probe.y;     
    }
   }
   if ( input.leftReleased() ) { // Mouse released
    if ( selecting && !shift) { // Move X only
     if ( !ctrl ) selecting->x=(float)probe.x;
     else { // Allow both to move (this has limited utility)
      selecting->x=(float)probe.x;
      selecting->y=(float)probe.y;
     }
     // Sort by Y ascending
//     points.Sort();
    } else if ( closest ) { // Insert new point
     Vertex *v=new Vertex((float)probe.x,(float)probe.y,0.0f);
     if ( (float) probe.y < closest->y
      && (closest.pointer == points.last
       || closest.pointer != points.first) )
      points.InsertBefore(v,closest);
     else points.InsertAfter(v,closest);
    }
   }
  }
  if ( active ) {
   if ( input.KeyDown(Backspace) || input.KeyDown(Delete) ) {
    ClearPoints();
   }
  }
 }
 void Render() {
  Area(active?crayons.jami(0,16,0,32,255):crayons.Pick(black),
   (int)screen.x,(int)screen.y,(int)(screen.x+w),(int)(screen.y+h)
  );
  Rectangle(crayons.Pick(gray),(int)screen.x,(int)screen.y,(int)(screen.x+w),(int)(screen.y+h));
  if ( active ) {
   if ( less ) {
    DrawLine2Color(crayons.Pick(red255),crayons.jami(0,0,0,0,0),
     (int)probeActual.x,(int)probeActual.y,
     (int)(screen.x+less->x*(double)w),
     (int)(screen.y+less->y*(double)h)
    );
   } 
   if ( greater ) {
    DrawLine2Color(crayons.Pick(red255),crayons.jami(0,0,0,0,0),
     (int)probeActual.x,(int)probeActual.y,
     (int)(screen.x+greater->x*(double)w),
     (int)(screen.y+greater->y*(double)h)
    );
   }
  }
  less=null;
  greater=null;
  if ( active ) {
   DrawLine(crayons.jami(0,96,128,96,64),probeActual.x,screen.y,probeActual.x,screen.y+h);
   DrawLine(crayons.jami(0,96,128,96,64),screen.x,probeActual.y,screen.x+w,probeActual.y);
  }
  EACH(points.first,Vertex,v) {
   if ( v->y <= probe.y ) less=v;
   if ( !greater ) {
    if ( v->y > probe.y ) {
     if ( !greater ) greater=v;
     else if ( greater->y < v->y ) greater=v;
    }
   }
  }
  selecting=null;
  EACH(points.first,Vertex,v) {
   if ( active && abs(probe.y-v->y) < minimumDistance.value ) {
    selecting = v;
    Cartesian screenCoord(screen.x+v->x*w,screen.y+v->y*h);
    Area(crayons.Pick(macNcheese),screenCoord.x-2,screenCoord.y-2,screenCoord.x+2,screenCoord.y+2);
    DrawCircle(screenCoord.x,screenCoord.y,5,8);
   }
   if ( v->next ) {
    Vertex *n=(Vertex *)v->next;
    Vertexd a,b;
    a.Set(v); a.Multiply((double)w,(double)h,0.0); a.Add((double)screen.x,(double)screen.y,0.0);
    b.Set(n); b.Multiply((double)w,(double)h,0.0); b.Add((double)screen.x,(double)screen.y,0.0);
    DrawLine(
     crayons.Pick(darkBlue),
     a.x+16.0,a.y,b.x+16.0,b.y
    );
    DrawLine2Color(
     v==less   ?crayons.Pick(green255):crayons.Pick(red255),
     n==greater?crayons.Pick(green255):crayons.Pick(magenta),
     a.x,a.y,b.x,b.y
    );
    if ( active && v==less && n==greater ) {
     Vertexd linePoint;
     double linetime=(probe.y-less->y)/(greater->y-less->y);
     Line line;
     line.Set(less->x,less->y,0.0,greater->x,greater->y,0.0);
     line.Point(linetime,&linePoint);
     linePoint.Multiply((double)w,(double)h,0.0);
     linePoint.Add((double)screen.x,(double)screen.y,0.0);
     Area(crayons.Pick(alabaster),linePoint.x-2,linePoint.y-2,linePoint.x+2,linePoint.y+2);
    }
    Rectangle(crayons.Pick(red255),a.x-2,a.y-2,a.x+2,a.y+2);
    Rectangle(crayons.Pick(magenta),b.x-2,b.y-2,b.x+2,b.y+2);
   }
  }
  if ( active ) {
   crayons.gl(macNcheese);
   if ( shift ) { if ( nearest ) Text("Free Insert",input.mxi+16,input.myi,7,10,false,0); }
   else if ( selecting ) {
    if ( ctrl ) Text("Free Move",input.mxi+16,input.myi,7,10,false,0);
    else if ( shift ) Text("Free Move",input.mxi+16,input.myi,7,10,false,0);    
    else Text("Move",input.mxi+16,input.myi,7,10,false,0);
   }// else Text("Insert",input.mxi+16,input.myi,7,10,false,0);
  }
 }
};

