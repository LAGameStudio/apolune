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
#include <math.h>
#include "Vertex.h"

enum Curve2dTypes {
 c2d_Lines=0,
 c2d_Bezier=1,
 c2d_Hermite=2,
 c2d_2Bspline=3,
 c2d_clamped2Bspline=4,
 c2d_Nurbs=5
};

class Curve2d {
public:
 Curve2d() { LOD=16; }
 void SetLOD( int precision ) { LOD=precision >=4 ? precision: 4; }
 Vertices *Bezier( 
  double Ax, double Ay, 
  double Bx, double By, 
  double Cx, double Cy, 
  double Dx, double Dy
  ) {
  Vertices *result=new Vertices;
  for ( int i=0; i<LOD; i++ ) {
   double t = iratiod(i,(LOD-1)),
    it = 1.0-t,
    b0 = t*t*t,
    b1 = 3.0*t*t*it,
    b2 = 3.0*t*it*it,
    b3 =  it*it*it,
    x = b0*Ax +  b1*Bx +  b2*Cx +  b3*Dx,
    y = b0*Ay +  b1*By +  b2*Cy +  b3*Dy;
   result->Prepend(new Vertex((float)x,(float)y,0.0f));
  }
  return result;
 }
 Vertices *Bezier( 
  double Ax, double Ay, double Az, 
  double Bx, double By, double Bz,
  double Cx, double Cy, double Cz,
  double Dx, double Dy, double Dz
  ) {
  Vertices *result=new Vertices;
  for ( int i=0; i<LOD; i++ ) {
   double t = iratiod(i,(LOD-1)),
    it = 1.0-t,
    b0 = t*t*t,
    b1 = 3.0*t*t*it,
    b2 = 3.0*t*it*it,
    b3 =  it*it*it,
    x = b0*Ax +  b1*Bx +  b2*Cx +  b3*Dx,
    y = b0*Ay +  b1*By +  b2*Cy +  b3*Dy,
    z = b0*Az +  b1*Bz +  b2*Cz +  b3*Dz ;
   result->Prepend(new Vertex((float)x,(float)y,(float)z));
  }
  return result;
 }
 // 2 end points and 2 tangent vectors describing the shape of the curve
 Vertices *Hermite(
  double Ax, double Ay,
  double Bx, double By,
  double Ctvx, double Ctvy,
  double Dtvx, double Dtvy ) {
  Vertices *result=new Vertices;
  for ( int i=0; i<LOD; i++ ) {
   double t = iratiod(i,(LOD-1)),
    b0 =  2.0*t*t*t - 3.0*t*t + 1.0,
    b1 = -2.0*t*t*t + 3.0*t*t,
    b2 = t*t*t - 2.0*t*t + t,
    b3 = t*t*t - t*t,
    x = b0*Ax + b1*Bx + b2*Ctvx + b3*Dtvx,
    y = b0*Ay + b1*By + b2*Ctvy + b3*Dtvy;
   result->Append(new Vertex((float)x,(float)y,0.0f));
  }
  return result;
 }
 // 2 end points and 2 tangent vectors describing the shape of the curve
 Vertices *Hermite(
  double Ax, double Ay, double Az,
  double Bx, double By, double Bz,
  double Ctvx, double Ctvy, double Ctvz,
  double Dtvx, double Dtvy, double Dtvz ) {
  Vertices *result=new Vertices;
  for ( int i=0; i<LOD; i++ ) {
   double t = iratiod(i,(LOD-1)),
    b0 =  2.0*t*t*t - 3.0*t*t + 1.0,
    b1 = -2.0*t*t*t + 3.0*t*t,
    b2 = t*t*t - 2.0*t*t + t,
    b3 = t*t*t - t*t,
    x = b0*Ax + b1*Bx + b2*Ctvx + b3*Dtvx,
    y = b0*Ay + b1*By + b2*Ctvy + b3*Dtvy,
    z = b0*Az + b1*Bz + b2*Ctvz + b3*Dtvz;
   result->Append(new Vertex((float)x,(float)y,(float)z));
  }
  return result;
 }

/*
/// a strcture to hold a curve point
struct Point {

	/// the position
	float x,y,z;

	/// ctor
	Point() : x(0),y(0),z(0){}

	/// ctor
	Point(const float a,const float b,const float c)
		:x(a),y(b),z(c){}

	/// copy ctor
	Point(const Point& p) : x(p.x),y(p.y),z(p.z) {}
};

/// a dynamic array of all points in the curve
std::vector<Point> Points;
//------------------------------------------------------------	IncreaseLod()
// When we increase the LOD we will have to re-create the points
// array inserting the new intermediate points into it.
//
//	Basically the subdivision works like this. each line,
//
//            A  *------------*  B
//
//	will be split into 2 new points, Q and R.
//
//                   Q    R
//            A  *---|----|---*  B
//
//	Q and R are given by the equations :
//
// 		Q = 3/4*A + 1/4*B
// 		R = 3/4*B + 1/4*A
//
void IncreaseLod() {
	std::vector<Point> NewPoints;

	// keep the first point
	NewPoints.push_back(Points[0]);
	for(unsigned int i=0;i<(Points.size()-1);++i) {
	
		// get 2 original points
		const Point& p0 = Points[i];
		const Point& p1 = Points[i+1];
		Point Q;
		Point R;

		// average the 2 original points to create 2 new points. For each
		// CV, another 2 verts are created.
		Q.x = 0.75f*p0.x + 0.25f*p1.x;
		Q.y = 0.75f*p0.y + 0.25f*p1.y;
		Q.z = 0.75f*p0.z + 0.25f*p1.z;

		R.x = 0.25f*p0.x + 0.75f*p1.x;
		R.y = 0.25f*p0.y + 0.75f*p1.y;
		R.z = 0.25f*p0.z + 0.75f*p1.z;

		NewPoints.push_back(Q);
		NewPoints.push_back(R);
	}
	// keep the last point
	NewPoints.push_back(Points[Points.size()-1]);

	// update the points array
	Points = NewPoints;
}
//------------------------------------------------------------	DecreaseLod()
// When we decrease the LOD, we can rather niftily get back
// to exactly what we had before. Since the original subdivision
// simply required a basic ratio of both points, we can simply
// reverse the ratio's to get the previous point...
//
void DecreaseLod() {

	// make sure we dont loose any points!!
	if (Points.size()<=4) 
		return;

	std::vector<Point> NewPoints;

	// keep the first point
	NewPoints.push_back(Points[0]);

	// step over every 2 points
	for(unsigned int i=1;i<(Points.size()-1);i+=2) {

		// get last point found in reduced array
		const Point& pLast = NewPoints[NewPoints.size()-1];

		// get 2 original point
		const Point& p0 = Points[i];
		Point Q;

		// calculate the original point
		Q.x = p0.x - 0.75f*pLast.x;
		Q.y = p0.y - 0.75f*pLast.y;
		Q.z = p0.z - 0.75f*pLast.z;

		Q.x *= 4.0f;
		Q.y *= 4.0f;
		Q.z *= 4.0f;

		// add to new curve
		NewPoints.push_back(Q);
	}

	// copy over points
	Points = NewPoints;
}
 
// The Chaikin curve is one of the simplest subdivison
// curves around. It uses a very simple ratio method
// to determine the new points on the curve. It can be
// shown mathmatically that this curve is the same as
// a quadratic b-spline.
 Vertices *Chaikin( Vertices *hulls, int LOD ) {
 }
*/

 Vertices *CubicBSpline( 
  double Ax, double Ay, double Az, 
  double Bx, double By, double Bz,
  double Cx, double Cy, double Cz,
  double Dx, double Dy, double Dz
  ) {
  Vertices *result=new Vertices;
  for ( int i=0; i<LOD; i++ ) {
   double t = iratiod(i,(LOD-1)),
    it = 1.0-t,
    b0 = it*it*it/6.0,
    b1 = (3.0*t*t*t - 6.0*t*t +4.0)/6.0,
    b2 = (-3.0*t*t*t +3.0*t*t + 3.0*t + 1)/6.0,
    b3 = t*t*t/6.0f,
    x = b0*Ax +  b1*Bx +  b2*Cx +  b3*Dx,
    y = b0*Ay +  b1*By +  b2*Cy +  b3*Dy,
    z = b0*Az +  b1*Bz +  b2*Cz +  b3*Dz ;
   Vertex *v=new Vertex((float)x,(float)y,(float)z);
   result->Append(v);
  }
  return result;
 }
 Vertices *ClampedCubicBSpline( Vertices *CV ) {
  if ( !CV || CV->count == 0 ) return null;
  Vertices *result=new Vertices;
  for ( int start_cv=-3,j=0; j<result->count; ++j,++start_cv ) {
   for ( int i=0; i<LOD; i++ ) {
    double t = iratiod(i,(LOD-1)),
     it = 1.0-t,
     b0 = it*it*it/6.0,
     b1 = (3.0*t*t*t - 6.0*t*t +4.0)/6.0,
     b2 = (-3.0*t*t*t +3.0*t*t + 3.0*t + 1)/6.0,
     b3 = t*t*t/6.0f;
    Vertex *A=ClampedCubicBSplineGetPoint(CV,start_cv+0),
           *B=ClampedCubicBSplineGetPoint(CV,start_cv+1),
           *C=ClampedCubicBSplineGetPoint(CV,start_cv+2),
           *D=ClampedCubicBSplineGetPoint(CV,start_cv+3);
    double
     x = b0 * A->x + b1 * B->x + b2 * C->x + b3 * D->x,
     y = b0 * A->y + b1 * B->y + b2 * C->y + b3 * D->y,
     z = b0 * A->z + b1 * B->z + b2 * C->z + b3 * D->z;
    result->Append(new Vertex((float)x,(float)y,(float)z));
   }
  }
  Vertex *end=new Vertex((Vertex *) CV->last);
  result->Append(end);
  return result;
 }
 Vertices *Nurbs( Vertices *CV, float *Knots, int knots ) {
  if ( !CV || CV->count == 0 || knots < 1 || !Knots ) return null;
  Vertices *result=new Vertices;
  for ( int i=0; i!=LOD; ++i ) {
   float t  = Knots[knots-1] * i / (float)(LOD-1);
   if(i==LOD-1)	t-=0.001f;
   float Outpoint[3]={0,0,0};
   GetOutpoint(CV,t,Outpoint,3,Knots,knots);
   result->Add(Outpoint[0],Outpoint[1],Outpoint[2]);
  }
  return result;
 }
 // Rob Bateman
 private:
 int LOD; // Set to 16 possibly for some specific reason
 Vertex *ClampedCubicBSplineGetPoint( Vertices *cv, int i ) {
  if ( !cv ) return null;
  if ( i < 0 ) return (Vertex *) cv->first;
  if ( i < cv->count ) return (Vertex *) cv->Element(i);
  return (Vertex *) cv->last;
 }
 // For Nurbs
 float CoxDeBoor(float u,int i,int k, float *Knots, int knots) {
  if(knots==1) {
   if( Knots[i] <= u && u <= Knots[i+1] ) return 1.0f;
   return 0.0f;
  }
  float Den1 = Knots[i+knots-1] - Knots[i];
  float Den2 = Knots[i+knots] - Knots[i+1];
  float Eq1=0,Eq2=0;
  if(Den1>0) Eq1 = ((u-Knots[i]) / Den1) * CoxDeBoor(u,i,k,Knots,knots-1);
  if(Den2>0) Eq2 = (Knots[i+knots]-u) / Den2 * CoxDeBoor(u,i+1,k,Knots,knots-1);
  return Eq1+Eq2;
 }
 void GetOutpoint(Vertices *CV, float t,float *OutPoint,int OutPoints, float *Knots, int knots ) {
  // sum the effect of all CV's on the curve at this point to 
  // get the evaluated curve point
  int i=0;
  EACH(CV->first,Vertex,v) {
   // calculate the effect of this point on the curve
   float Val = CoxDeBoor(t,i,4,Knots,knots);
   if (Val>0.001f) {
    // sum effect of CV on this part of the curve
    OutPoint[0] += Val * v->x;
    OutPoint[1] += Val * v->y;
    OutPoint[2] += Val * v->z;
   }
   i++;
  }
 }
};

extern Curve2d curve2d;

ONE(CurveDefinition,{
 type=c2d_Bezier;
 lines=1;
})
 Zint lines;
 Curve2dTypes type;
 Zdouble Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz, Dx, Dy, Dz;
 Vertices cv;
 Zbool threeD;
 Indexed<float> knots;
 CLONE(CurveDefinition,{
  DUPE(lines);
  DUPE(type);
  DUPE(Ax);
  DUPE(Ay);
  DUPE(Az);
  DUPE(Bx);
  DUPE(By);
  DUPE(Bz);
  DUPE(Cx);
  DUPE(Cy);
  DUPE(Cz);
  DUPE(Dx);
  DUPE(Dy);
  DUPE(Dz);
  a->cv.CopyAppend(&cv);
  DUPE(threeD);
  a->knots.Size(knots.length); for ( int i=0; i<(int)knots.length; i++ ) a->knots[i]=knots[i];
 })
 // Compute based on type and settings.
 bool UsesABCD() { return ( type == c2d_Lines || type == c2d_Bezier || type == c2d_Hermite ); }
 bool UsesVertices() { return !UsesABCD(); }
 Vertices *Compute() {
  switch ( type ) {
   case c2d_Lines:
    {
     Vertices *out=new Vertices;
     out->Append(new Vertex(Ax,Ay,Az));
     out->Append(new Vertex(Bx,By,Bz));
     if ( lines >= 2 ) out->Append(new Vertex(Cx,Cy,Cz));
     if ( lines >= 3 ) out->Append(new Vertex(Dx,Dy,Dz));
     return out;
    }
    break;
   case c2d_Bezier:
    {
     if ( threeD ) {
      return curve2d.Bezier(Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz,Dx,Dy,Dz);
     } else {
      return curve2d.Bezier(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy);
     }
    }
    break;
   case c2d_Hermite:
    {
     if ( threeD ) {
      return curve2d.Hermite(Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz,Dx,Dy,Dz);
     } else {
      return curve2d.Hermite(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy);
     }
    }
    break; 
   case c2d_2Bspline:
    {
     return curve2d.CubicBSpline(Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz,Dx,Dy,Dz);
    }
    break;
   case c2d_clamped2Bspline:
    {
     return curve2d.ClampedCubicBSpline(&cv);
    }
    break;
   case c2d_Nurbs:
    {
     return curve2d.Nurbs( &cv, knots.list, knots.length );
    }
    break;
   default: return null;
  }
 }
 KEYWORDS({
  KEYWORD("type",{type=(Curve2dTypes)atoi(w);})
  else KEYWORD("ax",{Ax=atof(w);})
  else KEYWORD("ay",{Ay=atof(w);})
  else KEYWORD("az",{Az=atof(w);})
  else KEYWORD("bx",{Bx=atof(w);})
  else KEYWORD("by",{By=atof(w);})
  else KEYWORD("bz",{Bz=atof(w);})
  else KEYWORD("cx",{Cx=atof(w);})
  else KEYWORD("cy",{Cy=atof(w);})
  else KEYWORD("cz",{Cz=atof(w);})
  else KEYWORD("dx",{Dx=atof(w);})
  else KEYWORD("dy",{Dy=atof(w);})
  else KEYWORD("dz",{Dz=atof(w);})
  else KEYWORD("cv",{cv.fromString(w);})
 })
 WORDKEYS({
  ENUMKEY("type",type)
  NUMBKEY("ax",Ax)
  NUMBKEY("ay",Ay)
  NUMBKEY("az",Az)
  NUMBKEY("bx",Bx)
  NUMBKEY("by",By)
  NUMBKEY("bz",Bz)
  NUMBKEY("cx",Cx)
  NUMBKEY("cy",Cy)
  NUMBKEY("cz",Cz)
  NUMBKEY("dx",Dx)
  NUMBKEY("dy",Dy)
  NUMBKEY("dz",Dz)
  SSUBKEY("cv",cv)
 })
MANY(CurveDefinition,CurveDefinitionHandle,CurveDefinitionHandles,"Curve",CurveDefinitions,{})
 CLONES(CurveDefinition,CurveDefinitions)
 KEYSWORDSGroups("Curve",CurveDefinition)
 // Simply concatenate each computed curve (creating lines between endpoints)
 void Compute( Vertices *out ) {
  FOREACH(CurveDefinition,c) {
   Vertices *in=c->Compute();
   out->Concat(in);
   delete in;
  }
 }
 // Compute an end-to-end approximation in a non-normalized coordinate space
 void Compute( Vertices *out, having attachedEndpoints ) {
  Vertex *last_vertex=NULL;
  FOREACH(CurveDefinition,c) {
   Vertices *in=c->Compute();
   if ( !in ) continue;
   if ( last_vertex && c->next ) {
    Vertex *first_vertex=(Vertex *)in->first;
    if ( first_vertex ) {
     in->MoveBy(-first_vertex->x,-first_vertex->y,-first_vertex->z);
     in->MoveBy(last_vertex->x,last_vertex->y,last_vertex->z);
     in->Remove(first_vertex);
     delete first_vertex;
    }
   }
   last_vertex=(Vertex *) in->last;
   out->Concat(in);
   delete in;
  }
 }
 static void DeployTypes( Strings *out ) {
  out->Add("Lines");
  out->Add("Bezier");
  out->Add("Hermite");
  out->Add("Cubic B-Spline");
  out->Add("Clamped B-Spline");
  out->Add("Nurbs");
 }
DONE(CurveDefinition);