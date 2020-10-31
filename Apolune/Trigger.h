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
#include "Vertexd.h"
#include "Polygon.h"
#include "Art.h"

class Box;
class CoplanarLinesResult;
class CoplanarLinesResults;
struct BoxCollisionResults;
class Line;
class Lines;

ONE(Circle,{})
 Zdisposable<Lines> lines;
 Zdouble x,y,z;
 Zdouble R;
 Zint precision;
 KEYWORDS({
  NUMWORD("x",x)
  NUMWORD("y",y)
  NUMWORD("z",z)
  NUMWORD("R",R)
  KEYWORD("precision",{ precision=w; Approximate(precision); })
 })
 WORDKEYS({
  NUMBKEY("x",x)
  NUMBKEY("y",y)
  NUMBKEY("z",z)
  NUMBKEY("R",R)
  NUMBKEY("precision",precision)
 })
 BINARY_IN({
  B_IN_NUM(x)
  B_IN_NUM(y)
  B_IN_NUM(z)
  B_IN_NUM(R)
  B_IN_NUM(precision) Approximate(precision);
 })
 BINARY_OUT({
  B_OUT_NUM(x)
  B_OUT_NUM(y)
  B_OUT_NUM(z)
  B_OUT_NUM(R)
  B_OUT_NUM(precision)
 })
 Circle( double R, int precision=16 ) : ListItem() {
  this->R=R;
  Approximate(precision);
 }
 Circle( double x, double y, double R, int precision=16 ) : ListItem() {
  this->x=x;
  this->y=y;
  this->R=R;
  Approximate(precision);
 }
 Circle( double x, double y, double z, double R, int precision=16 ) : ListItem() { // Or: Sphere
  this->x=x;
  this->y=y;
  this->z=z;
  this->R=R;
  Approximate(precision);
 }
 void Set( Circle *in ) { Set(in->x,in->y,in->R); Approximate(in->precision); }
 void Set( double R ) {
  this->R=R;
  Approximate(precision);
 }
 void Set( double x, double y, double R ) {
  this->x=x;
  this->y=y;
  this->R=R;
  Approximate(precision);
 }
 void Set( double x, double y ) {
  this->x=x;
  this->y=y;
  Approximate(precision);
 }
 void Approximate( int precision );
 // Source: Tim Voght via Paul Bourke
 bool Collides( Circle *c, Line *out );
 bool Collides( Box *b, CoplanarLinesResults *out, int precision );
 bool Collides( Vertices *v, bool connectLastToFirst, CoplanarLinesResults *out, int precision );
 bool Collides( Lines *lines, CoplanarLinesResults *out, int precision );
 bool within( double x, double y ) {
  return ( ddistance(x,y,this->x,this->y) <= R );
 }
 bool within( double x, double y, double z ) {
  return ( ddistance(x,y,z,this->x,this->y,this->z) <= R );
 }
 bool within( Circle *c );
 double overlap( Circle *c );
 void Point( double time, Vertexd *out ) {
  out->Set( x.value+cos(time*TWO_PI)*R.value, y.value+sin(time*TWO_PI)*R.value, z.value );
 }
 void RandomPointOn( Vertexd *out ) {
  double q = uniform() * TWO_PI;
  out->x = x+ (R) * cos(q);
  out->y = y+ (R) * sin(q);
 }
 void RandomPoint( Vertexd *out ) {
  double m = uniform();
  double q = uniform() * TWO_PI;
  out->Set(R*m,0);
  out->Rotate(q);
 }
 void RandomPoint( Vertexd *out, int seed ) {
  double m = (double)pseudo(seed);
  double q = (double)pseudo(seed) * TWO_PI;
  out->Set(R*m,0);
  out->Rotate(q);
 }
 void RandomPointOn( Vertexd *out, double degA, double degAwidth ) {
  double q = deg2rad(degA) + uniform() * deg2rad(degAwidth);
  out->x = x+ (R) * cos(q);
  out->y = y+ (R) * sin(q);
 }
 void RandomPoint( Vertexd *out, double degA, double degAwidth ) {
  double m = uniform();
  double q = deg2rad(degA) + uniform() * deg2rad(degAwidth);
  out->Set(R*m,0);
  out->Rotate(q);
 }
 void RandomPoint( Vertexd *out, double degA, double degAwidth, int seed ) {
  double m = (double)pseudo(seed+seed+seed);
  double q = deg2rad(degA) + (double)pseudo(seed) * deg2rad(degAwidth);
  out->Set(R*m,0);
  out->Rotate(q);
 }
 void RandomPointSpiral( Vertexd *out, double degA, double degAwidth, int seed ) {
  double m = (double)pseudo(seed);
  double q = deg2rad(degA) + (double)pseudo(seed) * deg2rad(degAwidth);
  out->x = x+ (R * m) * cos(q);
  out->y = y+ (R * m) * sin(q);
 }
 void Random3dPoint( Vertexd *out ) {
  double r = uniform()*R;
  double phi = uniform()*PI;
  double theta = uniform()*PI;
  out->x = x+ r * cos(theta) * sin(phi);
  out->y = y+ r * sin(theta) * sin(phi);
  out->z = z+ r * cos(phi);
 }

 void RandomPointOn( Vertexd *out, int seed ) {
  Random *set=randoms.GetSet(seed+12345);
  double q = (double) set->aunique(seed+5432) * TWO_PI;
  out->Set(R,0);
  out->Rotate(q);
 }
 void RandomPointOn( Vertexd *out, double degA, double degAwidth, int seed ) {
  Random *set=randoms.GetSet(seed+51342);
  double q = deg2rad(degA) + (double) set->aunique(seed+2345) * deg2rad(degAwidth);
  out->x = x+ (R) * cos(q);
  out->y = y+ (R) * sin(q);
 }
 void Random3dPoint( Vertexd *out, int seed ) {
  Random *set=randoms.GetSet(seed+54231);
  double r = (double) set->aunique(seed+5342)*R;
  double phi = (double) set->aunique(seed+2432)*PI;
  double theta = (double) set->aunique(seed+5253)*PI;
  out->x = x+ r * cos(theta) * sin(phi);
  out->y = y+ r * sin(theta) * sin(phi);
  out->z = z+ r * cos(phi);
 }
 void Debug2d( Crayon color, double x, double y ) {
  Blending(none);
  color.gl();
  DrawCircle(x+this->x,y+this->y,R,16);
 }
MANY(Circle,CircleHandle,CircleHandles,"circle",Circles,{})
 KEYSWORDSGroups("circle",Circle)
 BINARY_INOUT(Circle)
 Circle *Add( double R, int precision ) {
  Circle *result;
  result=new Circle(R,precision);
  Append(result);
  return result;
 }
 Circle *Add( double R ) {
  Circle *result;
  result=new Circle(R);
  Append(result);
  return result;
 }
 Circle *Add( double x, double y, double R ) {
  Circle *result;
  result=new Circle(x,y,R);
  Append(result);
  return result;
 }
 Circle *within( double x, double y ) {
  FOREACH(Circle,c) if ( c->within(x,y) ) return c;
  return null;
 }
 Circle *within( double x, double y, double z ) {
  FOREACH(Circle,c) if ( c->within(x,y,z) ) return c;
  return null;
 }
DONE(Circle);


// Actually specifically a "ray segment"
struct Slope {
 Zdouble ratio;
 Zbool vertical; // undefined value to slope formula
 Slope& operator = ( double d ) { ratio=d; return *this; }
};
class Line : public ListItem {
public:
 Zdouble x,y,z;
 Zdouble x2,y2,z2;
 Slope slope,slopeYZ;
 Zdouble rise,run,runZ;
 Zdouble angleRad,angleDeg,length;
 // These two values are used for the 'edge sorting' and DistancePointLine calculations when Line
 // is found in a list.
 Zdouble d;
 Zbool onSegment;
 Vertexd intersection; // Point of intersection for the point-to-line test (point falls on the line)
 Line() : ListItem() {}
 Line( Line *line ) : ListItem() { Set(line); }
 Line( Cartesian *c ) : ListItem() { Set((double)c->x,(double)c->y,(double)c->x2,(double)c->y2); }
 Line( Cartesiand *d ) : ListItem() { Set(d->x,d->y,d->z,d->x2,d->y2,d->z2); }
 Line( double x, double y, double x2, double y2 ) : ListItem() { Set(x,y,x2,y2); }
 Line( double x, double y, double z, double x2, double y2, double z2 ) : ListItem() { Set(x,y,z,x2,y2,z2); }
 Line( ScreenPosition *p ) : ListItem() { Set(p->dx,p->dy,p->dx2,p->dy2); }
 double minX() { return ( x < x2 ? x : x2 ); }
 double minY() { return ( y < y2 ? y : y2 ); }
 double minZ() { return ( z < z2 ? z : z2 ); }
 double maxX() { return ( x > x2 ? x : x2 ); }
 double maxY() { return ( y > y2 ? y : y2 ); }
 double maxZ() { return ( z > z2 ? z : z2 ); }
 bool Equals( Line *line ) { return ( x == line->x && y == line->y && x2 == line->x2 && y2 == line->y2 ); }
 void Set( Line *line ) {
  x=line->x;
  y=line->y;
  z=line->z;
  x2=line->x2;
  y2=line->y2;
  z2=line->z2;
  slope.ratio=line->slope.ratio;
  slope.vertical=line->slope.vertical;
  slopeYZ.ratio=line->slope.ratio;
  slopeYZ.vertical=line->slope.vertical;
  angleRad=line->angleRad;
  angleDeg=line->angleDeg;
  d=line->d;
  onSegment=line->onSegment;
  intersection.Set(&line->intersection);
  length=line->length;
  rise=line->rise;
  run=line->run;
  runZ=line->runZ;
 }
 void Set( double x, double y, double x2, double y2 ) {
  this->x=x;
  this->y=y;
  this->x2=x2;
  this->y2=y2;
  CalculateSlope();
 }
 void Set( double x, double y, double z, double x2, double y2, double z2 ) {
  this->x=x;
  this->y=y;
  this->x=x;
  this->x2=x2;
  this->y2=y2;
  this->z2=z2;
  CalculateSlope();
 }
 void Reverse() {
  Set(x2,y2,x,y);
 }
 void CalculateSlope() {
  double x2mx=x2-x;
  double y2my=y2-y;
  double z2mz=z2-z;
  if ( x2mx == 0.0 ) slope.vertical=true; else slope=(y2my)/(x2mx);
  if ( z2mz == 0.0 ) slope.vertical=true; else slopeYZ=(y2my)/(z2mz);
  angleRad=LineAngle();
  angleDeg=rad2deg(angleRad);
  length=ddistance(x,y,x2,y2);
  rise=x2-x;
  run=y2-y;
  runZ=z2-z;
 } 
 bool Collides( Circle *c, Vertexd *out ) {  
  if ( c->R == 0.0 ) {
   out->x=out->y=out->z=0.0;
   return false;
  }
  double a,b,C, bb4ac;
  Vertexd dp( x2 - x, y2 - y, z2 - z );
  a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
  b = 2 * (dp.x * (x - c->x) + dp.y * (y - c->y) + dp.z * (z - c->z));
  C = c->x * c->x + c->y * c->y + c->z * c->z;
  C += x * x + y * y + z * z;
  C -= 2 * (c->x * x + c->y * y + c->z * z);
  C -= c->R * c->R;
  bb4ac = b * b - 4 * a * C;
  if (abs(a) < EPSILON || bb4ac < 0) {
   out->x=0;
   out->y=0;
   return false;
  }
  out->x = (-b + sqrt(bb4ac)) / (2 * a);
  out->y = (-b - sqrt(bb4ac)) / (2 * a);
  return true;
 }
 bool Collides( Circle *c, Line *out, int precision );
 bool Collides( Vertices *points, CoplanarLinesResults *out, bool connectLastToFirst );
 bool Collides( Lines *lines, CoplanarLinesResults *out );
 // Source: Paul Bourke
 // Incorrectly returns the midpoint of the test line (this)
 // as the collision point when test against line has length of 0,
 // so we use Paeth's PntOnLine function to guestimate collision.
 // Because PntOnLine is integer-based, so normalized values will
 // all collide at their integer equivalents 0,1 (so we scale by
 // an arbitrary value of 100)
 bool Collides( Line *lineB, Vertexd *out ) { 
  if ( lineB->x==lineB->x2 && lineB->y==lineB->y2 ) {
   int res=PntOnLine(
    (int)(x*100.0),(int)(y*100.0),
    (int)(x2*100.0),(int)(y2*100.0),
    (int)(lineB->x*100.0),(int)(lineB->y*100.0)
   );
   out->x=lineB->x;
   out->y=lineB->y;
   return (res!=2);
  } else
  if ( x==x2 && y==y2 ) {
   int res=PntOnLine(
    (int)(lineB->x*100.0),(int)(lineB->y*100.0),
    (int)(lineB->x2*100.0),(int)(lineB->y2*100.0),
    (int)(x*100.0),(int)(y*100.0)
   );
   out->x=x;
   out->y=y;
   return (res!=2);
  }
  double mua,mub;
  double denom,numera,numerb;
  denom  = (lineB->y2-lineB->y) * (x2-x) - (lineB->x2-lineB->x) * (y2-y);
  numera = (lineB->x2-lineB->x) * (y-lineB->y) - (lineB->y2-lineB->y) * (x-lineB->x);
  numerb = (x2-x) * (y-lineB->y) - (y2-y) * (x-lineB->x);
  /* Are the line coincident? */
  if (abs(numera) < EPSILON && abs(numerb) < EPSILON && abs(denom) < EPSILON) {
   out->x = (x + x2) / 2.0;
   out->y = (y + y2) / 2.0;
   return true;
  }
  /* Are the line parallel */
  if (abs(denom) < EPSILON) {
   out->x = 0.0;
   out->y = 0.0;
   return false;
  }
  /* Is the intersection along the the segments */
  mua = numera / denom;
  mub = numerb / denom;
  if (mua < 0.0 || mua > 1.0 || mub < 0.0 || mub > 1.0) {
   out->x = 0.0;
   out->y = 0.0;
   return false;
  }
  out->x = x + mua * (x2 - x);
  out->y = y + mua * (y2 - y);
  return(TRUE);
 }
 bool Collides( Box *b, BoxCollisionResults *out );
 double LineMagnitude() {
  Cartesiand vector;
  vector.x = x2 - x;
  vector.y = y2 - y;
  return sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
 }
 double Magnitude( Cartesiand *Point2 ) {
  Cartesiand vector;
  vector.x = Point2->x - x;
  vector.y = Point2->y - y;
  vector.z = Point2->z - z;
  return sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
 }
 // Compute the line angle in radians
 double LineAngle() { return atan2( (y2-y), (x2-x) ); }
/*
 * given a line through P:(px,py) Q:(qx,qy) and T:(tx,ty)
 * return 0 if T is not on the line through      <--P--Q-->
 *        1 if T is on the open ray ending at P: <--P
 *        2 if T is on the closed interior along:   P--Q
 *        3 if T is on the open ray beginning at Q:    Q-->
 */
 int Point( double tx, double ty ) {
  return ( PntOnLine( (int)(x*100.0),(int)(y*100.0),(int)(x2*100.0),(int)(y2*100.0), (int)(tx*100.0), (int)(ty*100.0) ) );
 }
 // Source: Paul Bourke
 // Returns true when point is near enough to the line segment to be perpendicular
 // Returns false if the perpendicular would not hit the segment
 bool DistancePointLine( double px, double py, double pz, Vertexd *intersection, double *Distance ) {
    double LineMag=LineMagnitude();
    double U = ( ( ( px - x ) * ( x2 - x ) ) +
                 ( ( py - y ) * ( y2 - y ) ) +
                 ( ( pz - z ) * ( z2 - z ) ) ) /
               ( LineMag * LineMag ); 
    if( U < 0.0 || U > 1.0 ) return false;   // closest point does not fall within the line segment 
    intersection->x = x + U * ( x2 - x );
    intersection->y = y + U * ( y2 - y );
    intersection->z = z + U * ( z2 - z );
    Line distance(px,py,pz,intersection->x,intersection->y,intersection->z);
    *Distance = distance.LineMagnitude();
    return true;
 }
 void Point( double time, Vertexd *out ) {
  out->x=x+time*(x2-x);
  out->y=y+time*(y2-y);
  out->z=z+time*(z2-z);
 }
 // For a point on the line, we can return the time of the point on the line.
 // This function gives erroreous values when the point is not precisely on the line.
 double Time( double x, double y ) {
  return ddistance(x,y,this->x,this->y)/length;
 }
 // For a point on a line, we can return the time of the point on the line.
 // This function gives erroreous values when the point is not precisely on the line.
 double Time( double x, double y, double z ) {
  return ddistance(x,y,z,this->x,this->y,this->z)/length;
 }
 // Calculates a perpendicular point at (time) along line (dist) far from line.
 void Point( double dist, double time, Vertexd *out, double deltaAngle=90.0 ) {
  Vertexd pointOnLine;
  Point(time,&pointOnLine);
  double vector=LineAngle()+deg2rad(deltaAngle);
  out->x=pointOnLine.x+dist*cos(vector);
  out->y=pointOnLine.y+dist*sin(vector);
 }
 void Translate( double x, double y ) {
  this->x+=x;
  this->y+=y;
  this->x2+=x;
  this->y2+=y;
 }
 void Debug2d( Crayon color, double x, double y ) {
  Blending(none);
  DrawLine(color,x+this->x,y+this->y,x+x2,y+y2);
 }
 bool within( Box *b );
 bool within( Circle *c );
 bool within3d( Box *b );
 bool within3d( Circle *c );
 void MoveBy( double x, double y ) {
  this->x+=x;
  this->y+=y;
  x2+=x;
  y2+=y;
 }
};

bool operator == ( Line& a, Line& b );

class CoplanarLinesResult : public ListItem {
public:
 Zdouble x,y;
 Line segment;
 Zdouble d;
 Line onEdge;
 Zdouble edgeDistance;
 void Reset() {
  x=y=d=edgeDistance=0.0;
  segment.Set(0.0,0.0,0.0,0.0,0.0,0.0);
  onEdge.Set(0.0,0.0,0.0,0.0,0.0,0.0);
 }
 void Distance( double sx, double sy ) {
  d=ddistance(sx,sy,x,y);
 }
};

class CoplanarLinesResults : public LinkedList {
public:
 Zp<Line> onEdge;
 Zdouble sx,sy;
 void Min( Vertexd *min ) {
  if ( count == 0 ) return;
  CoplanarLinesResult *one=(CoplanarLinesResult *)first;
  min->Set(one->x,one->y);
  FOREACH(CoplanarLinesResult,c) {
   if ( c->x < min->x ) min->x=c->x;
   if ( c->y < min->y ) min->y=c->y;
  }
 }
 void Max( Vertexd *max ) {
  if ( count == 0 ) return;
  CoplanarLinesResult *one=(CoplanarLinesResult *)first;
  max->Set(one->x,one->y);
  FOREACH(CoplanarLinesResult,c) {
   if ( c->x > max->x ) max->x=c->x;
   if ( c->y > max->y ) max->y=c->y;
  }
 }
 void Add( Line *line, Vertexd *collision ) {
  CoplanarLinesResult *result=new CoplanarLinesResult;
  result->x=collision->x;
  result->y=collision->y;
  result->segment.Set(line->x,line->y,line->z,line->x2,line->y2,line->z2);
  Append(result);
 }
 void Debug( Crayon color, const char *prefix, double x, double y ) {
  int i=0;
  FOREACH(CoplanarLinesResult,L) {
   i++;
   string output=FORMAT(buf,124,"%s%d",prefix,i);
   DrawNamedPoint(color,output.c_str(),x+L->x,y+L->y);
  }
 }
 SORTING(CoplanarLinesResult,SortDistance,{
  FOREACH(CoplanarLinesResult,L) L->Distance(sx,sy);
 },{  if ( A->d < B->d ) result=-1; },{});
 CLEARLISTRESET(CoplanarLinesResult);
 ~CoplanarLinesResults() { Clear(); }
};

struct BoxCollisionResults {
public:
 Zint collisions;
 Zbool collided,startsInside,endsInside,within;
 Zbool top,left,right,bottom;
 Zdouble x,y,x2,y2;
 Zp<Line> onEdge;
 CoplanarLinesResults results;
 void Reset() {
  collisions=0;
  collided=startsInside=endsInside=false;
  top=left=right=bottom=false;
  x=y=x2=y2=0.0;
  onEdge=null;
  within=false;
  results.Clear();
 }
};

// A rectangular prism where xyz must be top left near and x2y2z2 must be far right bottom
class Box : public ListItem {
public:
 Zdisposable<Lines> lines;
 Zp<Line> top,bottom,left,right;
 Zdouble x,y,z,x2,y2,z2;
 Zdouble h,w,d;
 Zdouble area,volume;
 Vertexd translation,rotation,centroid;
 Box() : ListItem() {}
 Box( Box *b ) : ListItem() {
  Set(b);
  translation.Set(&b->translation);
  rotation.Set(&b->rotation);
 }
 Box( Line *L ) : ListItem() { Set(L); }
 Box( double x, double y, double x2, double y2 ) : ListItem() { Set(x,y,x2,y2); }
 Box( double x, double y, double z, double x2, double y2, double z2 ) : ListItem() { Set(x,y,z,x2,y2,z2); }
 Box( Cartesian *c ) : ListItem() { Set((double)c->x,(double)c->y,(double)c->x2,(double)c->y2); }
 Box( Cartesiand *d ) : ListItem() { Set(d->x,d->y,d->z,d->x2,d->y2,d->z2); }
 Box( ScreenPosition *p ) : ListItem() { Set(p->dx,p->dy,p->dx2,p->dy2); }
 double minX() { return ( x < x2 ? x : x2 ); }
 double minY() { return ( y < y2 ? y : y2 ); }
 double minZ() { return ( z < z2 ? z : z2 ); }
 double maxX() { return ( x > x2 ? x : x2 ); }
 double maxY() { return ( y > y2 ? y : y2 ); }
 double maxZ() { return ( z > z2 ? z : z2 ); }
 void Set( Line *L ) { Set(L->x,L->y,L->z,L->x2,L->y2,L->z2); }
 void Set( Box *b ) { Set(b->x,b->y,b->z,b->x2,b->y2,b->z2); }
 void Set( double lx, double ly ) {
  this->x=lx;
  this->y=ly;
  CalculateDimensions();
 }
 void Set( double fx, double fy, bool right ) {
  this->x2=fx;
  this->y2=fy;
  CalculateDimensions();
 }
 void Set( double lx, double ly, double lz ) {
  this->x=lx;
  this->y=ly;
  this->z=lz;
  CalculateDimensions();
 }
 void Set( double fx, double fy, double fz, bool right ) {
  this->x2=fx;
  this->y2=fy;
  this->z2=fz;
  CalculateDimensions();
 }
 void Set( double x, double y, double z, double x2, double y2, double z2 ) {
  this->x=UMIN(x,x2);
  this->y=UMIN(y,y2);
  this->z=UMIN(z,z2);
  this->x2=UMAX(x,x2);
  this->y2=UMAX(y,y2);
  this->z2=UMAX(z,z2);
  CalculateDimensions();
 }
 void Set( double x, double y, double x2, double y2 ) {
  this->x=UMIN(x,x2);
  this->y=UMIN(y,y2);
  this->x2=UMAX(x,x2);
  this->y2=UMAX(y,y2);
  CalculateDimensions();
 }
 bool Collides( Box *b, CoplanarLinesResults *out, Box *over );
 bool Collides3d( Box *b, CoplanarLinesResults *out, Box *over );
 bool Collides( Lines *in, CoplanarLinesResults *out );
 bool within(double x, double y) { return WITHIN(x,y,this->x,this->y,x2,y2); }
 bool within(double x, double y, double z) { return WITHIN3D(x,y,z,this->x,this->y,this->z,x2,y2,z2); }
 bool within( Box *b ) {
  return b->within(x,y) && b->within(x,y2) && b->within(x2,y2) && b->within(x2,y);
 }
 bool overlaps( Box *b ) {
  return ! ( x > b->x2 || b->x > x2 || y > b->y2 || b->y > y2);
 }
 bool within3d( Box *b ) {
  return
   b->within(x,y,z) && b->within(x,y2,z) && b->within(x2,y2,z) && b->within(x2,y,z) &&
   b->within(x,y,z2) && b->within(x,y2,z2) && b->within(x2,y2,z2) && b->within(x2,y,z2);
 }
 bool overlaps3d( Box *b ) {
  return ( 
   ((x <= b->x && b->x <= x2) || (b->x <= x && x <= b->x2)) &&
   ((y <= b->y && b->y <= y2) || (b->y <= y && y <= b->y2)) &&
   ((z <= b->z && b->z <= z2) || (b->z <= z && z <= b->z2)) 
  );
 }
 void CalculateDimensions();
 void Translate( double x, double y ) {
  this->x+=x;
  this->y+=y;
  this->x2+=x;
  this->y2+=y;
  CalculateDimensions();
 }
 void Translate( double x, double y, double z ) {
  this->x+=x;
  this->y+=y;
  this->z+=z;
  this->x2+=x;
  this->y2+=y;
  this->z2+=z;
  CalculateDimensions();
 }
 void Debug( Crayon color, double x, double y ) {
  Blending(none);
  Rectangle(color,x+this->x,y+this->y,x+x2,y+y2);
 }
 void Debug( Crayon color ) {
  color.gl();
  glBegin(GL_QUADS);
  glEnd();
 }
};

class Lines : public LinkedList {
public:
 Vertexd point; // Used to determine edge-related statistics, set this point then call Sort() to find closest line
 Vertexd centroid;
 Box bounds;
 Zdouble length;
 Lines() : LinkedList() {}
 Lines( Lines *lines ) : LinkedList() {
  Add(lines);
  point.Set(&lines->point);
  centroid.Set(&lines->centroid);
  bounds.Set(&lines->bounds);
 }
 Lines( Vertices *lines, bool connectFirstToLast ) : LinkedList() {
  Add(lines,connectFirstToLast);
 }
 void CalculateCentroidAndBounds() {
  if ( count == 0 ) {
   centroid.Set(0.0,0.0,0.0);
   return;
  }
  Vertexd tally;
  Zint total;
  length=0.0;
  FOREACH(Line,b) {
   if ( b->x < bounds.x ) bounds.x=b->x;
   if ( b->y < bounds.y ) bounds.y=b->y;
   total++;
   tally.Add(b->x,b->y);
   total++;
   tally.Add(b->x2,b->y2);
   if ( b->x2 > bounds.x2 ) bounds.x2=b->x2;
   if ( b->y2 > bounds.y2 ) bounds.y2=b->y2;
   length+=b->length;
  }
  bounds.CalculateDimensions();
  double t=(double)total;
  centroid.Set(tally.x/total,tally.y/total);
 }
 Line *Add( double x, double y, double x2, double y2 ) {
  Line *result=new Line(x,y,x2,y2);
  Append(result);
  return result;
 }
 void Add( Box *b );
 void Add( Circle *c, int precision ) {
  for ( int i=0; i<precision; i++ ) {
   double p1=iratiod(i,precision);
   double p2=iratiod(i+1,precision);
   Line *segment=new Line(
    c->x+(c->R*cos(TWO_PI*p1)), c->y+(c->R*sin(TWO_PI*p1)),
    c->x+(c->R*cos(TWO_PI*p2)), c->y+(c->R*sin(TWO_PI*p2))
   );
   Append(segment);
  }
 }
 void Add( Vertices *v, bool connectLastToFirst ) {
  if ( v->count <= 1 ) return;
  EACH(v->first,Vertex,p) if ( p->next ) {
   Vertex *q=(Vertex *)p->next;
   Append(new Line(p->x,p->y,p->z,q->x,q->y,q->z));
  }
  if ( connectLastToFirst ) {
   Vertex *a=(Vertex *) v->last;
   Vertex *b=(Vertex *) v->first;
   Append(new Line(a->x,a->y,a->z,b->x,b->y,b->z));
  }
 }
 void Add( Lines *lines ) {
  EACH(lines->first,Line,line) {
   Append(new Line(line));
  }
 }
 void TallyLength() {
  length=0.0;
  FOREACH(Line,b) length+=b->length;
 }
 bool GetPoint( Vertexd *out, double time ) {
  double len=0.0;
  double stopLength=time*length;
  FOREACH(Line,b) {
   if ( len+b->length > stopLength ) {
    double localLength=stopLength-len;
    double localTime=localLength/b->length;
    b->Point(localTime,out);
    return true;
   } else len+=b->length;
  }
  return false;
 }
 bool Collides( Line *line, CoplanarLinesResults *out );
 bool Collides( Line *line, CoplanarLinesResults *out, bool reverseSort );
 bool Collides( Lines *lines, CoplanarLinesResults *out );
 // Returns the first encountered (not the least) point-to-line test within Tolerance
 Line *OnEdge( double x, double y, double z, double Tolerance, Vertexd *intersection ) {
  Vertexd tpoint(x,y,z);
  FOREACH(Line,line) {
   double D;
   bool onSegment=line->DistancePointLine(tpoint.x,tpoint.y,tpoint.z,intersection,&D);
   if ( D <= Tolerance ) return line;
  }
  return null;
 }
 // Returns all point-to-line tests within Tolerance
 // (simply sets and executes the sort), changing the order of the line list
 void PointOnEdgeSort( double x, double y, double z ) {
  point.Set(x,y,z);
  SortPointOnEdge();
 }
 // Set the value of point to the desired point for edge detection,
 // results will be sorted by their point-to-line distance
 SORTING(Line,SortPointOnEdge,{
  Vertexd intersection;
  FOREACH(Line,line) line->onSegment=line->DistancePointLine(point.x,point.y,point.z,&line->intersection,&line->d.value);
 },{ if ( A->d < B->d ) result=-1; },{})
 CLEARLISTRESET(Line);
 ~Lines() { Clear(); }
};

class Boxes : public LinkedList {
public:
 Vertexd centroid;
 Box bounds;
 void CalculateCentroidAndBounds() {
  if ( count == 0 ) {
   centroid.Set(0.0,0.0,0.0);
   return;
  }
  Vertexd tally;
  Zint total;
  FOREACH(Box,b) {
   if ( b->x < bounds.x ) bounds.x=b->x;
   if ( b->y < bounds.y ) bounds.y=b->y;
   if ( b->z < bounds.z ) bounds.z=b->z;
   total++;
   tally.Add(b->x,b->y,b->z);
   total++;
   tally.Add(b->x2,b->y2,b->z2);
   if ( b->x2 > bounds.x2 ) bounds.x2=b->x2;
   if ( b->y2 > bounds.y2 ) bounds.y2=b->y2;
   if ( b->z2 > bounds.z2 ) bounds.z2=b->z2;
  }
  bounds.CalculateDimensions();
  double t=(double)total;
  centroid.Set(tally.x/total,tally.y/total,tally.z/total);
 }
 Box *Add( double x, double y, double x2, double y2 ) {
  Append(new Box(x,y,x2,y2));
  return (Box *) last;
 }
 Box *Add( double x, double y, double z, double x2, double y2, double z2 ) {
  Append(new Box(x,y,z,x2,y2,z2));
  return (Box *) last;
 }
 Box *within(double x, double y ) {
  FOREACH(Box,b) if ( b->within(x,y) ) return b;
  return null;
 }
 Box *within(double x, double y, double z) {
  FOREACH(Box,b) if ( b->within(x,y,z) ) return b;
  return null;
 }
 void RotateRelative( Vertexd *axis ) {
  //.....//
 }
 CLEARLISTRESET(Box);
 ~Boxes() { Clear(); }
};