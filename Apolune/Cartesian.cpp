
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Cartesian.h"

#include "Display.h"
#include "Vertex.h"
#include "Vertexd.h"
#include "Trigger.h"

CoplanarLinesResults intersection; Box overlap;
void Cartesiands::FitTo1( having threed ) {
 Box bounds;
 Bounds3d(&bounds);
 FOREACH(Cartesiand,c) c->Set(c->x/bounds.x,c->y/bounds.y,c->z/bounds.z,c->w/bounds.w,c->h/bounds.h,c->d/bounds.d);
}

Cartesian::Cartesian( float ffx, float ffy ) : ListItem()
{
 x=y=z=w=h=x2=y2=0;
 fx=fy=fz=fw=fh=0.0f;
 a=0.0f;
 Xd=Yd=0.0;
 fx=ffx;
 fy=ffy;
}

Cartesian::Cartesian( const Zfloat& ffx, const Zfloat& ffy ) : ListItem()
{
 x=y=z=w=h=x2=y2=0;
 fx=fy=fz=fw=fh=0.0f;
 a=0.0f;
 Xd=Yd=0.0;
 fx=ffx.value;
 fy=ffy.value;
}


Cartesian::Cartesian( int ix, int iy ) : ListItem()
{
 x=y=z=w=h=x2=y2=0;
 fx=fy=fz=fw=fh=0.0f;
 a=0.0f;
 Xd=Yd=0.0;
 x=ix;
 y=iy;
 fx=(float)ix;
 fy=(float)iy;
}

Cartesian::Cartesian( int ix, int iy, int iw, int ih ) : ListItem()
{
 x=y=z=w=h=x2=y2=0;
 fx=fy=fz=fw=fh=0.0f;
 a=0.0f;
 Xd=Yd=0.0;
 SetRect(ix,iy,iw,ih);
}

Cartesian::Cartesian( const Zint& ix, const Zint& iy ) : ListItem()
{
 x=y=z=w=h=x2=y2=0;
 fx=fy=fz=fw=fh=0.0f;
 a=0.0f;
 Xd=Yd=0.0;
 x=ix.value;
 y=iy.value;
 fx=(float)ix.value;
 fy=(float)iy.value;
}

void Cartesian::Multiply( Cartesiand *ratio ) {
 x  = (int) ( (double) x  * ratio->x );
 y  = (int) ( (double) y  * ratio->y );
 z  = (int) ( (double) z  * ratio->z );
 w  = (int) ( (double) w  * ratio->w );
 h  = (int) ( (double) h  * ratio->h );
 x2 = (int) ( (double) x2 * ratio->x2 );
 y2 = (int) ( (double) y2 * ratio->y2 );
 fx=(float)x;
 fy=(float)y;
 fz=(float)z;
 fw=(float)w;
 fh=(float)h;
}

/*
# The coordinates must be in decimal degrees so you’ll need to unproject any projected coordinates and then 
  re-project the result. This may introduce some distortion. I didn’t notice any in my application but I’d 
  suggest some more rigorous testing if you’ve got tight precision requirements.
# Rotation follows engineering standards (zero East, counter-clockwise)
# The base point is shifted to 0,0 and the same offset is applied to all other points in order to keep the math straightforward.
 */
Cartesian Cartesian::RotateXYi(Cartesian const &sourcePoint, float rotationAngleDegrees) {
 double r;
 double theta;
 double offsetX;
 double offsetY;
 double offsetTheta;
 double rotateX;
 double rotateY;
 double rotationRadians;

 //shift x and y relative to 0,0 origin
 offsetX = (sourcePoint.x + (this->x * -1));
 offsetY = (sourcePoint.y + (this->y * -1));
 //convert to radians. take absolute value (necessary for x coord only).
 offsetX = abs(offsetX * (PI / 180));
 offsetY = offsetY * (PI / 180);
 rotationRadians = rotationAngleDegrees * (PI / 180);
 //get distance from origin to source point
 r = sqrt((offsetX*offsetX) + (offsetY*offsetY));
 //get current angle of orientation
 theta = atan(offsetY / offsetX);
 // add rotation value to theta to get new angle of orientation
 offsetTheta = theta + rotationRadians;
 //calculate new x coord
 rotateX = r * cos(offsetTheta);
 //calculate new y coord
 rotateY = r * sin(offsetTheta);
 //convert new x and y back to decimal degrees
 rotateX = rotateX * (180 / PI);
 rotateY = rotateY * (180 / PI);
 //shift new x and y relative to base point
 rotateX = (rotateX + this->x);
 rotateY = (rotateY + this->y);
 //return new point
 Cartesian temp;
 temp.x=(int) rotateX;
 temp.y=(int) rotateY;
 temp.fx=(float) rotateX;
 temp.fy=(float) rotateY;
 return temp;
}

Cartesian Cartesian::RotateXYf(Cartesian const &sourcePoint, float rotationAngleDegrees ) {
 double r;
 double theta;
 double offsetX;
 double offsetY;
 double offsetTheta;
 double rotateX;
 double rotateY;
 double rotationRadians;

 //shift x and y relative to 0,0 origin
 offsetX = (sourcePoint.fx + (this->fx * -1));
 offsetY = (sourcePoint.fy + (this->fy * -1));
 //convert to radians. take absolute value (necessary for x coord only).
 offsetX = abs(offsetX * (PI / 180));
 offsetY = offsetY * (PI / 180);
 rotationRadians = rotationAngleDegrees * (PI / 180);
 //get distance from origin to source point
 r = sqrt((offsetX*offsetX) + (offsetY*offsetY));
 //get current angle of orientation
 theta = atan(offsetY / offsetX);
 // add rotation value to theta to get new angle of orientation
 offsetTheta = theta + rotationRadians;
 //calculate new x coord
 rotateX = r * cos(offsetTheta);
 //calculate new y coord
 rotateY = r * sin(offsetTheta);
 //convert new x and y back to decimal degrees
 rotateX = rotateX * (180 / PI);
 rotateY = rotateY * (180 / PI);
 //shift new x and y relative to base point
 rotateX = (rotateX + this->fx);
 rotateY = (rotateY + this->fy);
 //return new point
 Cartesian temp;
 temp.fx=(float) rotateX;
 temp.fy=(float) rotateY;
 return temp;
}

Cartesian Cartesian::RotateZYf(Cartesian const &sourcePoint, float rotationAngleDegrees ) {
 double r;
 double theta;
 double offsetZ;
 double offsetY;
 double offsetTheta;
 double rotateZ;
 double rotateY;
 double rotationRadians;

 //shift x and y relative to 0,0 origin
 offsetZ = (sourcePoint.fz + (this->fz * -1));
 offsetY = (sourcePoint.fy + (this->fy * -1));
 //convert to radians. take absolute value (necessary for x coord only).
 offsetZ = abs(offsetZ * (PI / 180));
 offsetY = offsetY * (PI / 180);
 rotationRadians = rotationAngleDegrees * (PI / 180);
 //get distance from origin to source point
 r = sqrt((offsetZ*offsetZ) + (offsetY*offsetY));
 //get current angle of orientation
 theta = atan(offsetY / offsetZ);
 // add rotation value to theta to get new angle of orientation
 offsetTheta = theta + rotationRadians;
 //calculate new x coord
 rotateZ = r * cos(offsetTheta);
 //calculate new y coord
 rotateY = r * sin(offsetTheta);
 //convert new x and y back to decimal degrees
 rotateZ = rotateZ * (180 / PI);
 rotateY = rotateY * (180 / PI);
 //shift new x and y relative to base point
 rotateZ = (rotateZ + this->fz);
 rotateY = (rotateY + this->fy);
 //return new point
 Cartesian temp;
 temp.fx=sourcePoint.fx;
 temp.fz=(float) rotateZ;
 temp.fy=(float) rotateY;
 return temp;
}

Cartesian Cartesian::RotateXYZYf(Cartesian const &sourcePoint, float rotationAngleDegreesXY, float rotationAngleDegreesZY )
{
 Cartesian result = this->RotateXYf(sourcePoint,rotationAngleDegreesXY);
 Cartesian returning = result.RotateXYf(sourcePoint,rotationAngleDegreesZY);
// delete result;
 return returning;
}

float Cartesian::Distancei() {
 return isqrt( ((this->x - x2) * (this->x - x2)) + ((this->y - y2) * (this->y - y2)) );
}

float Cartesian::Distancef( float xx, float yy ) {
 return sqrtf( ((this->fx - xx) * (this->fx - xx)) + ((this->fy - yy) * (this->fy - yy)) ) ;
}

float Cartesian::Distancei( int xx, int yy ) {
 return isqrt( ((this->x - xx) * (this->x - xx)) + ((this->y - yy) * (this->y - yy)) ) ;
}

float Cartesian::Distance( float xx, float yy ) {
 return sqrtf( ((this->fx - xx) * (this->fx - xx)) + ((this->fy - yy) * (this->fy - yy)) ) ;
}

float Cartesian::Distance( int xx, int yy ) {
 return isqrt( ((this->x - xx) * (this->x - xx)) + ((this->y - yy) * (this->y - yy)) ) ;
}

bool Cartesian::Intersects( Vertex *offset, Cartesian *a ) {
 Box b(a);
 Box c( (double)(offset->x+x), (double)(offset->y+y), (double)(offset->x+x2), (double)(offset->y+y2) );
 return b.Collides(&c,&intersection,&overlap);
}


bool Cartesian::TranslateIntersects( Vertex *offset1, Vertex *offset2, Cartesian *a ) {
 Box b( (double)(x+offset1->x), (double)(y+offset1->y), (double)(x2+offset1->x), (double)(y2+offset1->y) );
 Box c( (double)(a->x+(int) offset2->x), (double)(a->y+(int) offset2->y), (double)(a->x2+(int) offset2->x), (double)(a->y2+(int) offset2->y) );
 return b.Collides(&c,&intersection,&overlap);
}

#include "Trigger.h"
void Cartesian::Lerp2d( Cartesian *a, Cartesian *b, double time ) {
 Line TL(a->x,a->y,b->x,b->y);
 Line HW(a->w,a->h,b->w,b->h);
 Vertexd pointA,pointB;
 TL.Point(time,&pointA);
 HW.Point(time,&pointB);
 SetRect(pointA.x,pointA.y,pointB.x,pointB.y);
}
void Cartesiand::Lerp2d( Cartesiand *a, Cartesiand *b, double time ) {
 Line TL(a->x,a->y,b->x,b->y);
 Line HW(a->w,a->h,b->w,b->h);
 Vertexd pointA,pointB;
 TL.Point(time,&pointA);
 HW.Point(time,&pointB);
 SetRect(pointA.x,pointA.y,pointB.x,pointB.y);
}


bool Cartesian::OnScreen() {
 return ( x > -w && x2 < display.w+w ) && ( y > -h && y2 < display.h+h );
}

bool Cartesians::Intersects( Cartesiand *a, Cartesiand *ofs ) {
 FOREACH(Cartesian,c) {
  Cartesiand d(c->x,c->y);
  d.MoveBy(ofs->x,ofs->y);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Cartesiand *a ) {
 FOREACH(Cartesian,c) if ( c->Intersects(a) ) return true;
 return false;
}

bool Cartesians::Intersects( Cartesiand *a, int ox, int oy ) {
 FOREACH(Cartesian,c) {
  Cartesian d(c);
  d.MoveBy(ox,oy);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Cartesiand *a, double ox, double oy ) {
 FOREACH(Cartesian,c) {
  Cartesiand d(c->x,c->y,c->w,c->h);
  d.MoveBy(ox,oy);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Vertex *offset, Cartesians *many ) {
 FOREACH(Cartesian,c) EACH(many->first,Cartesian,d) 
  if ( c->Intersects(offset,d) ) return true;
 return true;
}

bool Cartesians::TranslateIntersects( Vertex *offset1, Vertex *offset2, Cartesians *many ) {
 FOREACH(Cartesian,c) EACH(many->first,Cartesian,d) 
  if ( c->TranslateIntersects(offset1,offset2,d) ) return true;
 return true;
}

void Cartesians::GetLinePoints( int x, int y, int x2, int y2, int accuracy ) { 
 for ( int i=0; i<accuracy; i++ ) {
  float percent=iratiof(i,accuracy);
  Cartesian *point=new Cartesian;
  point->Set(
   x + percent * (x2 - x),
   y + percent * (y2 - y)
  );
  Append(point);
 }
}

void Cartesiand::Set( Vertexd *in ) {
 Set(in->x,in->y,in->z);
}

// For the scripting language
#include "Variadics.h"
void Cartesian::fromString( const char *s, VariableStore *variables ) {
 Zstring words(s);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( is_integer(w) ) {
   SetInterpi(w);
   interp_value+=1;
  } else if ( is_decimal(w) ) {
   SetInterpf(w);
   interp_value+=1;
  } else if ( !str_cmp(w,"x") ) {
   w=words.Next();
   this->x=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y") ) {
   w=words.Next();
   this->y=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"z") ) {
   w=words.Next();
   this->z=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"w") ) {
   w=words.Next();
   this->w=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"h") ) {
   w=words.Next();
   this->h=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"x2") ) {
   w=words.Next();
   this->x2=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y2") ) {
   w=words.Next();
   this->y2=variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"fx") ) {
   w=words.Next();
   this->fx=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"fy") ) {
   w=words.Next();
   this->fy=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"fz") ) {
   w=words.Next();
   this->fz=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"fw") ) {
   w=words.Next();
   this->fw=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"fh") ) {
   w=words.Next();
   this->fh=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"a") ) {
   w=words.Next();
   this->a=(float)variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"corners") || !str_cmp(w,"line") ) {
   this->Corners(this->x,this->y,this->x2,this->y2);
  }
  w=words.Next();
 }
 interp_value=0;
}


bool Cartesians::Intersects( Cartesian *a, double dx, double dy ) {
 FOREACH(Cartesian,c) {
  Cartesiand d(c->x,c->y,c->w,c->h);
  d.MoveBy(dx,dy);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Cartesian *a, double dx, double dy, Cartesian *excepting ) {
 FOREACH(Cartesian,c) {
  if ( c == excepting ) continue;
  Cartesiand d(c->x,c->y,c->w,c->h);
  d.MoveBy(dx,dy);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Cartesiand *a, double dx, double dy, Cartesian *excepting ) {
 FOREACH(Cartesian,c) {
  if ( c == excepting ) continue;
  Cartesiand d(c->x,c->y,c->w,c->h);
  d.MoveBy(dx,dy);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesians::Intersects( Cartesian *a, Cartesiand *ofs ) {
 FOREACH(Cartesian,c) {
  Cartesian d(c);
  d.MoveBy((int)ofs->x,(int)ofs->y);
  if ( d.Intersects(a) ) return true;
 }
 return false;
}

bool Cartesian::Intersects( Cartesian *a ) {
 Box b(a);
 Box c(x,y,x2,y2);
 return b.Collides(&c,&intersection,&overlap);
}

bool Cartesian::Intersects( Cartesiand *a ) {
 Box b(a);
 Box c(x,y,x2,y2);
 return b.Collides(&c,&intersection,&overlap);
}

bool Cartesiand::Intersects( Cartesiand *a ) {
 Box b(a);
 Box c(x,y,x2,y2);
 return b.Collides(&c,&intersection,&overlap);
}

bool Cartesiand::Intersects( Cartesian *a ) {
 Box b(a);
 Box c(x,y,x2,y2);
 return b.Collides(&c,&intersection,&overlap);
}

#include "Trigger.h"


 // These two functions work together to test paths in a rectangle world.
 // Obstacles are rectangles, path is a line we intend to travel.
 bool Cartesiands::WillCollide( Cartesiands *obstacles, Line *path ) {
  EACH(obstacles->first,Cartesiand,cd) {
   Box box(cd->x,cd->y,cd->x2,cd->y2);
   BoxCollisionResults collision;
   path->Collides(&box,&collision);
   if ( collision.collided ) return true;
  }
  return false;
 }

 // These two functions work together to test paths in a rectangle world.       ___b
 // Obstacles are rectangles, path is a line we intend to travel and           /  /|
 // footprint is the rectangle that will pass along it, where the rectangle   +-+a/   would be line A-B,     A=TL+-+TR=B
 // center is the point which will move along the path line.                  +-+/     but as a rectangle.   D=BL+-+BR=C
 bool Cartesiands::WillCollide( Cartesiands *obstacles, Cartesiand *footprint, Line *path ) { //          L   R
  bool movingUp=path->y > path->y2; // Testing to see which corners to pick             up |DAB|ABC|
  bool movingLeft=path->x < path->x2; // Up Left means lower left, upper right.         dn |ADC|BCD|
  double diffX=path->x2-path->x;
  double diffY=path->y2-path->y;
  double w2=footprint->w/2;
  double h2=footprint->h/2;
  Line a,b,c;
  if ( movingUp ) {
   if ( movingLeft ) {
    a.Set(path->x-w2,path->y+h2,path->x-w2+diffX,path->y+h2+diffY);
    b.Set(path->x-w2,path->y-h2,path->x-w2+diffX,path->y-h2+diffY);
    c.Set(path->x+w2,path->y-h2,path->x+w2+diffX,path->y-h2+diffY);
   } else {
    b.Set(path->x-w2,path->y-h2,path->x-w2+diffX,path->y-h2+diffY);
    c.Set(path->x+w2,path->y-h2,path->x+w2+diffX,path->y-h2+diffY);
    a.Set(path->x+w2,path->y+h2,path->x+w2+diffX,path->y+h2+diffY);
   }
  } else {
   if ( movingLeft ) {
    a.Set(path->x+w2,path->y+h2,path->x+w2+diffX,path->y+h2+diffY);
    b.Set(path->x-w2,path->y-h2,path->x-w2+diffX,path->y-h2+diffY);
    c.Set(path->x-w2,path->y+h2,path->x-w2+diffX,path->y+h2+diffY);
   } else {
    a.Set(path->x+w2,path->y+h2,path->x+w2+diffX,path->y+h2+diffY);
    b.Set(path->x-w2,path->y+h2,path->x-w2+diffX,path->y+h2+diffY);
    c.Set(path->x+w2,path->y-h2,path->x+w2+diffX,path->y-h2+diffY);
   }
  }
  EACH(obstacles->first,Cartesiand,cd) {
   Box box(cd->x,cd->y,cd->x2,cd->y2);
   BoxCollisionResults collision;
   collision.Reset();
   a.Collides(&box,&collision);
   if ( collision.collided ) return true;
   collision.Reset();
   b.Collides(&box,&collision);
   if ( collision.collided ) return true;
   collision.Reset();
   c.Collides(&box,&collision);
   if ( collision.collided ) return true;
  }
  return false;
 }

 Cartesiand *Cartesiands::NearestPoint( Cartesiand *out, double x, double y, double farValue ) {
  bool found=false;
  Vertexd intersection;
  double candidateD=farValue;
  Cartesiand *candidate=null;
  FOREACH(Cartesiand,c) {
   if ( c->next ) {
    Cartesiand * b=(Cartesiand *)c->next;
    Line line(c->x,c->y,b->x,b->y);
    double d=0.0;
    bool result=line.DistancePointLine(x,y,0.0,&intersection,&d);
    if ( result ) {
     out->Set(intersection.x,intersection.y);
     candidate=b;
     candidateD=d;
    }
   }
  }
  if ( candidate ) return candidate;
  return NearestPoint(x,y,farValue);
 }
 
 Cartesiand *Cartesiands::NearestPoint( Cartesiand *out, double *distOut, double x, double y, double farValue ) {
  bool found=false;
  Vertexd intersection;
  double candidateD=farValue;
  Cartesiand *candidate=null;
  FOREACH(Cartesiand,c) {
   if ( c->next ) {
    Cartesiand * b=(Cartesiand *)c->next;
    Line line(c->x,c->y,b->x,b->y);
    double d=0.0;
    bool result=line.DistancePointLine(x,y,0.0,&intersection,&d);
    if ( result ) {
     out->Set(intersection.x,intersection.y);
     candidate=b;
     candidateD=d;
    }
   }
  }
  if ( candidate ) {
   (*distOut)=candidateD;
   return candidate;
  }
  (*distOut)=-1.0;
  return NearestPoint(x,y,farValue);
 }

 Cartesiand *Cartesiands::NearestPoint( having shortestBias, Cartesiand *out, double x, double y, double farValue ) {
  bool found=false;
  Vertexd intersection;
  double candidateD=farValue;
  Cartesiand *candidate=null;
  FOREACH(Cartesiand,c) {
   if ( c->next ) {
    Cartesiand * b=(Cartesiand *)c->next;
    Line line(c->x,c->y,b->x,b->y);
    double d=0.0;
    bool result=line.DistancePointLine(x,y,0.0,&intersection,&d);
    if ( result && d < candidateD ) {
     out->Set(intersection.x,intersection.y);
     candidate=b;
     candidateD=d;
    }
   }
  }
  if ( candidate ) return candidate;
  return NearestPoint(x,y,farValue);
 }

void Cartesiands::fromVertices( Vertices * in ) {
 EACH(in->first,Vertex,v) {
  Cartesiand *c=new Cartesiand;
  c->Set((double)v->x,(double)v->y,(double)v->z);
  Append(c);
 }
}

void Cartesiands::Bounds3d( Box * out ) {
  if ( count == 0 ) { OUTPUT("Cartesiand::Bounds on empty list\n"); return; }
  Cartesiand *p=(Cartesiand *) first;
  double Lx=p->x;
  double Ly=p->y;
  double Lz=p->z;
  double Gx=p->x;
  double Gy=p->y;
  double Gz=p->z;
  p=(Cartesiand *) p->next;
  while ( p ) {
   if ( p->x < Lx ) Lx=p->x;
   if ( p->y < Ly ) Ly=p->y;
   if ( p->z < Lz ) Lz=p->z;
   if ( p->x > Gx ) Gx=p->x;
   if ( p->y > Gy ) Gy=p->y;
   if ( p->z > Gz ) Gz=p->z;
   p=(Cartesiand *) p->next;
  }
  out->Set(Lx,Ly,Lz,Gx,Gy,Gz);
}