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
#include "Indexed.h"
#include "ZeroTypes.h"
#include "LinkedList.h"
#include "Vertex.h"
#include "Polygon.h"

class TVector : public ListItem {
public:
 double point[3];
 TVector() : ListItem() { point[0]=0.0; point[1]=0.0; point[2]=0.0; }
 TVector( Vertex *v ) : ListItem() { Set(v); }
 void Set(Vertex *v) { Set((double)v->x,(double)v->y,(double)v->z); }
 TVector( Vertexd *v ) : ListItem() { Set(v); }
 void Set(Vertexd *v) { Set(v->x,v->y,v->z); }
 void Set( double x, double y, double z) {
  point[0]=x;
  point[1]=y;
  point[2]=z;
 }
 TVector& operator= ( const TVector& t ) {
  point[0]=t.point[0];
  point[1]=t.point[1];
  point[2]=t.point[2];
  return *this;
 }
};

bool operator==(const TVector &a, const TVector &b);

class TVectors : public LinkedList {
public:
 TVectors() : LinkedList() {}
 TVectors( Vertices *in ) : LinkedList() {
  EACH(in->first,Vertex,v) {
   Append(new TVector(v));
  }
 }
  /*
    From P Bourke's C prototype - 
    qsort(p,nv,sizeof(XYZ),XYZCompare);		
    int XYZCompare(void *v1,void *v2) {
     XYZ *p1,*p2;  p1 = v1; p2 = v2;
     if (p1->x < p2->x) return(-1);
     else if (p1->x > p2->x) return(1);
     else return(0);
    }
  */
 SORTING(TVector,Sort,{},{ if (A->point[0] < B->point[0]) result=-1; },{});
 CLEARLISTRESET(TVector);
};

class TEdge : public ListItem {
public:
 TVector p1,p2;
 TEdge( TVector p1, TVector p2 ) : ListItem() { Set(p1,p2); }
 void Set( TVector p1, TVector p2 ) {
  this->p1=p1;
  this->p2=p2;
 }
 Zbool cull;
};

class TEdges : public LinkedList {
public:
 TEdges() : LinkedList() {}
 CLEARLISTRESET(TEdge);
 void Add( const TVector &p1, const TVector &p2 ) {
  Append(new TEdge(p1,p2));
 }
};

class TTriangle : public ListItem {
public:
 TVector p1,p2,p3;
 TTriangle() : ListItem() {}
 TTriangle( TVector p1, TVector p2, TVector p3 ) : ListItem() { Set(p1,p2,p3); }
 void Set( TVector p1, TVector p2, TVector p3 ) {
  this->p1=p1;
  this->p2=p2;
  this->p3=p3;
 }
 bool Shares(TTriangle other) {
  return p1 == other.p1 || p1 == other.p2 || p1 == other.p3
      || p2 == other.p1 || p2 == other.p2 || p2 == other.p3
      || p3 == other.p1 || p3 == other.p2 || p3 == other.p3;
 }
 mPolygon *toPolygon() {
  mPolygon *p=new mPolygon;
  p->draw_method=GL_TRIANGLES; // Counter-clockwise winding
  p->points.Add(p1.point[0],p1.point[1],p1.point[2]);
  p->points.Add(p3.point[0],p3.point[1],p3.point[2]);
  p->points.Add(p2.point[0],p2.point[1],p2.point[2]);
  return p;
 }
 TTriangle& operator= ( const TTriangle& t ) { p1=t.p1; p2=t.p2; p3=t.p3; return *this; }
};

bool operator== ( const TTriangle &a, const TTriangle &b );

class TTriangles : public LinkedList {
public:
 CLEARLISTRESET(TTriangles);
 void Add( const TTriangle& t ) {
  TTriangle *p=new TTriangle();
  *p=t;
  Append(p);
 }
 void Add( const TVector &a, const TVector &b, const TVector &c ) {
  TTriangle *t=new TTriangle();
  t->p1=a;
  t->p2=b;
  t->p3=c;
  Append(t);
 }
 bool contains( const TTriangle& a ) {
  FOREACH(TTriangle,b) if ( a == *b ) return true;
  return false;
 }
 Polygons *toPolygons() {
  Polygons *polygons=new Polygons;
  toPolygons(polygons);
  return polygons;
 }
 void toPolygons( Polygons *polygons ) {
  FOREACH(TTriangle,t) polygons->Append(t->toPolygon());
 }
};

HANDLES(TTriangle,TTriangleHandle,TTriangleHandles,"TTriangleHandle");

class Triangulate {
public:
  /*
    Return TRUE if a point (xp,yp) is inside the circumcircle made up
    of the points (x1,y1), (x2,y2), (x3,y3)
    The circumcircle centre is returned in (xc,yc) and the radius r
    NOTE: A point on the edge is inside the circumcircle
  */
  bool circumCircle(TVector p, TTriangle t, TVector *circle) {
   double m1,m2,mx1,mx2,my1,my2;
   double dx,dy,rsqr,drsqr;

   if ( abs(t.p1.point[1]-t.p2.point[1]) < EPSILON
     && abs(t.p2.point[1]-t.p3.point[1]) < EPSILON ) {
    OUTPUT("Triangulate: circumCircle: Points are coincident.\n");
    return false;
   }
   if ( abs(t.p2.point[1]-t.p1.point[1]) < EPSILON ) {
    m2 = - (t.p3.point[0]-t.p2.point[0]) / (t.p3.point[1]-t.p2.point[1]);
    mx2 = (t.p2.point[0] + t.p3.point[0]) / 2.0f;
    my2 = (t.p2.point[1] + t.p3.point[1]) / 2.0f;
    circle->point[0] = (t.p2.point[0] + t.p1.point[0]) / 2.0f;
    circle->point[1] = m2 * (circle->point[0] - mx2) + my2;
   } else if ( abs(t.p3.point[1]-t.p2.point[1]) < EPSILON ) {
    m1 = - (t.p2.point[0]-t.p1.point[0]) / (t.p2.point[1]-t.p1.point[1]);
    mx1 = (t.p1.point[0] + t.p2.point[0]) / 2.0f;
    my1 = (t.p1.point[1] + t.p2.point[1]) / 2.0f;
    circle->point[0] = (t.p3.point[0] + t.p2.point[0]) / 2.0f;
    circle->point[1] = m1 * (circle->point[0] - mx1) + my1;	
   } else {
    m1 = - (t.p2.point[0]-t.p1.point[0]) / (t.p2.point[1]-t.p1.point[1]);
    m2 = - (t.p3.point[0]-t.p2.point[0]) / (t.p3.point[1]-t.p2.point[1]);
    mx1 = (t.p1.point[0] + t.p2.point[0]) / 2.0f;
    mx2 = (t.p2.point[0] + t.p3.point[0]) / 2.0f;
    my1 = (t.p1.point[1] + t.p2.point[1]) / 2.0f;
    my2 = (t.p2.point[1] + t.p3.point[1]) / 2.0f;
    circle->point[0] = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
    circle->point[1] = m1 * (circle->point[0] - mx1) + my1;
   }
	
    dx = t.p2.point[0] - circle->point[0];
    dy = t.p2.point[1] - circle->point[1];
    rsqr = dx*dx + dy*dy;
    circle->point[2] = sqrt(rsqr);
		
    dx = p.point[0] - circle->point[0];
    dy = p.point[1] - circle->point[1];
    drsqr = dx*dx + dy*dy;
	
    return drsqr <= rsqr;
  }

  /*
    Triangulation subroutine
    Takes as input vertices (convert to TVectors) in ArrayList pxyz
    Returned is a list of triangular faces in the ArrayList triangles 
    These triangles are arranged in a consistent clockwise order.
  */
  void Compute( Vertices *in, Polygons *out ) {
    TVectors pxyz(in);

    // sort vertex array in increasing x values
    pxyz.Sort();
    		
    /*
      Find the maximum and minimum vertex bounds.
      This is to allow calculation of the bounding triangle
    */
    TVector *p=(TVector *) pxyz.Element(0);
    double xmin = p->point[0];
    double ymin = p->point[1];
    double xmax = xmin;
    double ymax = ymin;
    
    EACH(pxyz.first,TVector,t) {
     if (t->point[0] < xmin) xmin = t->point[0];
     if (t->point[0] > xmax) xmax = t->point[0];
     if (t->point[1] < ymin) ymin = t->point[1];
     if (t->point[1] > ymax) ymax = t->point[1];
    }
    
    double dx = xmax - xmin;
    double dy = ymax - ymin;
    double dmax = (dx > dy) ? dx : dy;
    double xmid = (xmax + xmin) / 2.0f;
    double ymid = (ymax + ymin) / 2.0f;
	
    TTriangles triangles; // for the Triangles
    TTriangleHandles complete;  // for complete Triangles

    /*
      Set up the supertriangle
      This is a triangle which encompasses all the sample points.
      The supertriangle coordinates are added to the end of the
      vertex list. The supertriangle is the first triangle in
      the triangle list.
    */
    TTriangle superTriangle;
    superTriangle.p1.Set( xmid - 2.0f * dmax, ymid - dmax, 0.0f );
    superTriangle.p2.Set( xmid, ymid + 2.0f * dmax, 0.0f );
    superTriangle.p3.Set( xmid + 2.0f * dmax, ymid - dmax, 0.0f );
    triangles.Add(superTriangle);
    
    /*
      Include each point one at a time into the existing mesh
    */
    TEdges edges;
    EACH(pxyz.first,TVector,v) {
     edges.Clear();
      
      /*
        Set up the edge buffer.
        If the point (xp,yp) lies inside the circumcircle then the
        three edges of that triangle are added to the edge buffer
        and that triangle is removed.
      */
      TVector circle;
      TTriangle *n=null;
      for ( TTriangle *t=(TTriangle *) triangles.last; t; t=n ) {
       n=(TTriangle *) t->next;
       if (complete.ListIteminList(t)) continue;         
       bool inside = circumCircle( *v, *t, &circle );       
       if (circle.point[0] + circle.point[2] < v->point[0] ) complete.Add(t);
       if (inside) {
        edges.Add(t->p1, t->p2);
        edges.Add(t->p2, t->p3);
        edges.Add(t->p3, t->p1);
        complete.Delete(t);
        triangles.Remove(t);
       }
      }

      /*
        Tag multiple edges
        Note: if all triangles are specified anticlockwise then all
        interior edges are opposite pointing in direction.
      */
      EACH(edges.first,TEdge,e1) if ( e1->next && !e1->cull ) {
       EACH(e1->next,TEdge,e2) {
        if ( e1->p1 == e2->p2 && e1->p2 == e2->p1) {
         e1->cull=true; 
         e2->cull=true;
        }
        /* Shouldn't need the following, see note above */
        if (e1->p1 == e2->p1 && e1->p2 == e2->p2) {
         e1->cull=true; 
         e2->cull=true;
        }
       }
      }
      
      /*
        Form new triangles for the current point
        Skipping over any tagged edges.
        All edges are arranged in clockwise order.
      */
      for (unsigned int j=0; j < edges.count; j++) {
       TEdge *e = (TEdge *) edges.Element(j);
       if (e->cull) continue;
       triangles.Add(e->p1, e->p2, *v);
      }
      
    }
      
   /*
     Remove triangles with supertriangle vertices
   */
   EACHN(triangles.first,TTriangle,tri, {
    if (tri->Shares(superTriangle)) triangles.Remove(tri);
   });
   triangles.toPolygons(out);
  }
	
};

extern Triangulate triangulate;