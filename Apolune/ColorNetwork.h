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
#include "ColorField.h"
#include "Trigger.h"

class ColorNode : public ListItem {
public:
 Zdouble x,y;
 Crayon color;
 double distance( double x, double y ) { return ddistance(x,y,this->x,this->y); }
};

class ColorNetwork : public LinkedList {
public:
 CLEARLISTRESET(ColorNode);
 ColorNetwork() : LinkedList() {
 }
 void Add( Crayon c, double x, double y ) {
  ColorNode *cn=new ColorNode;
  cn->x=x;
  cn->y=y;
  cn->color.fromCrayon(c);
  Append(cn);
 }
 Crayon Request( double x, double y ) {
  if ( count == 0 ) return crayons.Pick(black);
  if ( count == 1 ) return ((ColorNode *) first)->color;
  if ( count == 2 ) {
   ColorNode *a=(ColorNode *) first;
   ColorNode *b=(ColorNode *) last;
   double d=ddistance(a->x,a->y,b->x,b->y);
   double dAB=0.0;
   Line AB(a->x,a->y,b->x,b->y);
   Vertexd v;
   Zbool testA=AB.DistancePointLine(x,y,0.0f,&v,&dAB);
   if ( testA ) {
    double t=AB.Time(v.x,v.y);
    Crayon mixed;
    mixed.ColorInterp(a->color,b->color,(float)t);
    return mixed;
   } else {
    double dA=ddistance(x,y,a->x,a->y);
    double dB=ddistance(x,y,b->x,b->y);
    if ( dA > dB ) return b->color;
    else return a->color;
   }
  }
  ColorNode *a=null;
  Zdouble da;
  ColorNode *b=null;
  Zdouble db;
  ColorNode *c=null;
  Zdouble dc;
  FOREACH(ColorNode,p) {
   if ( a==null ) {
    a=p;
    da=p->distance(x,y);
   } else {
    double d=p->distance(x,y);
    if ( d < da ) {
     c=b;
     dc=db;
     b=a;
     db=da;
     a=p;
     da=d;
    } else {
     if ( b==null ) {
      b=p;
      db=d;
     } else {
      if ( d < db ) {
       c=b;
       dc=db;
       b=p;
       db=d;
      } else {
       if ( c==null ) {
        c=p;
        dc=d;
       } else {
        if ( d < dc ) {
         c=p;
         dc=d;
        }
       }
      }
     }
    }
   }
  }
  Line AB(a->x,a->y,b->x,b->y);
  Line BC(b->x,b->y,c->x,c->y);
  Line AC(a->x,a->y,c->x,c->y);
  Vertexd v;
  double Da=1000000.0,Db=1000000.0,Dc=1000000.0;
  Zbool
   testA=AB.DistancePointLine(x,y,0.0,&v,&Da),
   testB=BC.DistancePointLine(x,y,0.0,&v,&Db),
   testC=AC.DistancePointLine(x,y,0.0,&v,&Dc);
  double tA=AB.Time(v.x,v.y);
  double tB=BC.Time(v.x,v.y);
  double tC=AC.Time(v.x,v.y);
  Crayon cAB; cAB.ColorInterp(a->color,b->color,(float)tA);
  Crayon cBC; cBC.ColorInterp(b->color,c->color,(float)tB);
  Crayon cAC; cAC.ColorInterp(a->color,c->color,(float)tC);
  Crayon mixed;
  if ( testA && testB && testC ) {
   mixed.Float(
    (cAB.rf+cBC.rf+cAC.rf)/3.0f,
    (cAB.bf+cBC.bf+cAC.bf)/3.0f,
    (cAB.gf+cBC.gf+cAC.gf)/3.0f,
    (cAB.af+cBC.af+cAC.af)/3.0f
   );
  } else if ( testA && testB ) {
   mixed.fromCrayon(cAB);
  } else if ( testB && testC ) {
   mixed.fromCrayon(cBC);
  } else if ( testA && testC ) {
   mixed.fromCrayon(cAC);
  } else if ( testA ) {
   return a->color;
  } else if ( testB ) {
   return b->color;
  } else if ( testC ) {
   return c->color;
  }
  return mixed;
  /*
  if ( a && b && c ) {
  } else if ( a && b ) {
  } else if ( b && c ) {
  } else if ( a && c ) {
  } else if ( a ) {
  } else if ( b ) {
  } else if ( c ) {
  }
  */
 }
 ColorNode *nearest( double x, double y ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return ((ColorNode *)first);
  ColorNode *candidate=null;
  Zdouble dist;
  FOREACH(ColorNode,p) {
   if ( !candidate ) {
    dist=ddistance(x,y,p->x,p->y);
    candidate=p;
   } else {
    Zdouble td=ddistance(x,y,p->x,p->y);
    if ( td < dist ) {
     dist=td;
     candidate=p;
    }
   }
  }
  return candidate;
 }
};

class ColorWebNode : public ListItem {
public:
};

class ColorWeb : public LinkedList {
public:
 CLEARLISTRESET(ColorWebNode);
};

class ColorGear : public LinkedList {
public:
 Zdouble Delta;
 ColorGear() : LinkedList() {
  Delta=1.0/10000.0;
 }
 CLEARLISTRESET(ColorNode);
 void Add( Crayon c ) {
  ColorNode *cn=new ColorNode;
  cn->color.fromCrayon(c);
  Append(cn);
 }
 Crayon Request( double x, double y ) {
  if ( count == 0 ) return crayons.Pick(clear);
  if ( count == 1 ) return ((ColorNode *)first)->color;
  double f=1.0+sin(x*Delta)+1.0+sin(y*Delta);
  double d=(f)/4.0;
  double e=d*(double)count;
  int idx=(int)e;
  double excess=e-(double)idx;
  ColorNode *cn=(ColorNode *) Element(idx);
  ColorNode *cnn=null;
  if ( !cn->next ) cnn=(ColorNode *) first;
  else cnn=(ColorNode *) cn->next;
  Crayon mixed;
  mixed.ColorInterp(cn->color,cnn->color,(float)excess);
  return mixed;
 }
};