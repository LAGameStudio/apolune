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
#include "Trigger.h"
#include "GLWindow.h"
#include "UglyFont.h"

class CollisionTests : public GLWindow {
public:
 Vertexd point;
 Vertices points;
 Box box,box2;
 Circle circle,circle2;
 Line line,line2;
 BoxCollisionResults bcr;
 Line segment;
 Vertexd lineline;
 Lines lines;
 Box boxbox;
 CoplanarLinesResults results;
 Zbool one,two,three,four,five,six;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 void Render() {
  string output=string("");
  DrawLine(crayons.Pick(blue),-display->w2,display->w2,-display->h2,display->h2);
  glPush(false);
  glTranslated(display->w2d,display->h2d,0.0);
  Crayon crayon;
  crayon.Pick(red255);
  Rectangle(crayon,box.x,box.y,box.x2,box.y2);
  Rectangle(crayon,box2.x,box2.y,box2.x2,box2.y2);
  crayon.gl();
  DrawCircle(circle.x,circle.y,circle.R,16);
  DrawCircle(circle2.x,circle2.y,circle2.R,16);
  DrawLine(crayon,line2.x,line2.y,line2.x2,line2.y2);
  DrawLine2Color(crayons.Pick(green255),crayons.Pick(red255),line.x,line.y,line.x2,line.y2);
  points.Debug(0.0,0.0);
  if ( line.Collides(&box,&bcr) ) {
   DrawNamedPoint(crayons.Pick(yellow),"bcr1",bcr.x,bcr.y);
   if ( bcr.collisions > 1 ) DrawNamedPoint(crayons.Pick(orange),"bcr2",bcr.x2,bcr.y2);
  }
  if ( line.Collides(&circle,&segment,16) ) {
   DrawLine2Color(crayons.Pick(alabaster),crayons.Pick(gray),segment.x,segment.y,segment.x2,segment.y2);
   DrawNamedPoint(crayons.Pick(orange),"Lc1",segment.x,segment.y);
   DrawNamedPoint(crayons.Pick(orange),"Lc2",segment.x2,segment.y2);
  }
  if ( line.Collides(&line2,&lineline) ) {
   DrawNamedPoint(crayons.Pick(alabaster),"LL",lineline.x,lineline.y);
  }
  if ( circle.Collides(&circle2,&segment) ) {
   DrawLine2Color(crayons.Pick(yellow),crayons.Pick(orange),segment.x,segment.y,segment.x2,segment.y2);
   DrawNamedPoint(crayons.Pick(green255),"Cc1",segment.x,segment.y);
   DrawNamedPoint(crayons.Pick(green255),"Cc2",segment.x2,segment.y2);
  }
  if ( line.Collides(&points,&results,true) ) {
   string name=string("");
   int i=0;
   EACH(results.first,CoplanarLinesResult,r) {
    i++;
    name=FORMAT(buf,123,"Lp%d",i);
    DrawNamedPoint(crayons.Pick(skyBlue),name.c_str(),r->x,r->y);
   }
  }
  if ( circle.Collides(&box,&results,32) ) {
   results.Debug(crayons.Pick(hotPink),"cb",0.0,0.0);
  }
  if ( circle.Collides(&box2,&results,32) ) {
   results.Debug(crayons.Pick(chartreuse),"cbII-",0.0,0.0);
  }
  if ( circle.within(&circle2) ) {
   output+=string("Circle is within circle2\n");
  }
  double overlap=circle.overlap(&circle2);
  if ( overlap>=0.0 ) {
   output+=FORMAT(buf,1024,"Circle overlaps circle2 by %1.2f\n",overlap);
  }
  if ( circle.Collides(&points,true,&results,32) ) {
   results.Debug(crayons.Pick(oliveDrab),"cp",0.0,0.0);
  }
  if ( box.Collides(&box2,&results,&boxbox) ) {
   Blending(additive);
   Area(crayons.jami(0,32,64,128,255),boxbox.x,boxbox.y,boxbox.x2,boxbox.y2);
   Blending(none);
   results.Debug(crayons.Pick(black),"bb",-1.0,1.0);
   results.Debug(crayons.Pick(macNcheese),"bb",0.0,0.0);
   output+=string("Box1 is colliding with Box2\n");
  }
  lines.Clear();
  lines.Add(&points,true);
  if ( box2.Collides(&lines,&results) ) {
   results.Debug(crayons.Pick(vermilion),"bp",0.0,0.0);
  }
  DrawNamedPoint(crayons.Pick(purple),"A",line.x,line.y);
  DrawNamedPoint(crayons.Pick(purple),"B",line.x2,line.y2);
  DrawNamedPoint(crayons.Pick(purple),"A",line2.x,line2.y);
  DrawNamedPoint(crayons.Pick(purple),"B",line2.x2,line2.y2);
  DrawNamedPoint(crayons.Pick(purple),"A",box.x,box.y);
  DrawNamedPoint(crayons.Pick(purple),"B",box.x2,box.y2);
  DrawNamedPoint(crayons.Pick(purple),"A",box2.x,box2.y);
  DrawNamedPoint(crayons.Pick(purple),"B",box2.x2,box2.y2);
  string label=FORMAT(buf,123,"C, R=%1.2f",circle.R);
  DrawNamedPoint(crayons.Pick(purple),label.c_str(),circle.x,circle.y);
  DrawNamedPoint(crayons.Pick(hotPink),"Point",point.x,point.y);
  if ( circle.within(point.x,point.y) ) {
   output+="Point is in Circle\n";
  }
  if ( circle2.within(point.x,point.y) ) {
   output+="Point is in Circle2\n";
  }
  if ( box.within(point.x,point.y) ) {
   output+="Point is in Box\n";
  }
  if ( points.within(point.x,point.y) ) {
   output+="Point is in poly\n";
  }
  double d=0.0;
  Vertexd intersection;
  bool onSegment=line.DistancePointLine(point.x,point.y,point.z,&intersection,&d);
  if ( onSegment ) {
   DrawLine2Color(crayons.Pick(blue),crayons.Pick(cyan),point.x,point.y,intersection.x,intersection.y);
   DrawNamedPoint(crayons.Pick(hotPink),"Do",intersection.x,intersection.y);
   output+=FORMAT(buf,256,"Point is %1.2f from point-line\n", d);
  } else {
   output+=string("Point does not fall perpendicular\n");
  }
  if ( line.Point(point.x,point.y) == 2 ) output+=string("Point is on test segment\n");
  Text(output,display->w2d-256.0,display->h2d-256.0,7,10,4.0,1.0);
  glPop();
 }
 Zdouble mx,my;
 void Between() {
  mx=(double)input->mxi-display->w2d;
  my=(double)input->myi-display->h2d;
  if ( input->KeyDown(DX_SPACE) ) {
   line.Set(0,0,0,0);
   line2.Set(0,0,0,0);
   box.Set(0,0,0,0,0,0);
  }
  if ( input->KeyDown(DX_RETURN) ) {
   points.Clear();
  }
  if ( input->KeyDown(DX_X) ) destroyMe=true;
 }
 void LeftDown() {
  if ( input->KeyDown(DX_LALT) ) {
   points.Add((float)mx,(float)my);
  } else if ( input->KeyDown(DX_LCONTROL) ) {
   point.x=mx;
   point.y=my;
  } else if ( input->KeyDown(DX_LSHIFT) ) {
   if ( !four ) {
    line2.x=mx;
    line2.y=my;
    four=true;
   } else {
    line2.x2=mx;
    line2.y2=my;
    four=false;
   }
   line.CalculateSlope();
  } else {
   if ( !one ) {
    line.x=mx;
    line.y=my;
    one=true;
   } else {
    line.x2=mx;
    line.y2=my;
    one=false;
   }
   line.CalculateSlope();
  }
 }
 void RightDown() {
  if ( input->KeyDown(DX_RSHIFT) ) {
   if ( !six ) {
    box2.Set(mx,my,box2.maxX(),box2.maxY());
    six=true;
   } else {
    box2.Set(box2.minX(),box2.minY(),mx,my);
    six=false;
   }
  } else {
   if ( !two ) {
    box.Set(mx,my,box.maxX(),box.maxY());
    two=true;
   } else {
    box.Set(box.minX(),box.minY(),mx,my);
    two=false;
   }
  }
 }
 void MiddleDown() {
  if ( input->KeyDown(DX_LSHIFT) ) {
   if ( !five ) {
    circle2.Set(mx,my);
    five=true;
   } else {
    circle2.Set(ddistance(mx,my,circle.x,circle.y));
    five=false;
   }
  } else {
   if ( !three ) {
    circle.Set(mx,my);
    three=true;
   } else {
    circle.Set(ddistance(mx,my,circle.x,circle.y));
    three=false;
   }
  }
 }
};

