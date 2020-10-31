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

#include "ZIndexed.h"
#include "ZeroTypes.h"

/*
 * Purchased from BinPress on 6/16/2014 as "Unlimited" license $24.99

 An example of usage is shown below:
// first , define some random points in 2d space
double Px[1024],Py[1024];
int N=10;
for ( int i=0; i<N; i++ ) {
    Px[i]=(float)(rand()%450)+350;
    Py[i]=(float)(rand()%400)+150;
}
CDelaunay2D    T;
// this function call takes caer of anything
T.Triangulate( Px,Py,N );    
// just to show how is easy to get low level access 
// to data structures an example using mfc is shown here:
    int i,p1,p2,p3;
    const DELAUNAYVERTEX* VertexPtr=T.GetVertexPtr();
    const DELAUNAYTRIANGLE* TrianglePtr=T.GetTrianglePtr();
    CPoint pts[3];
    for ( i=0; i<T.GetTriangleCount(); i ++  )    {
        p1=TrianglePtr[ i ].p1;
        p2=TrianglePtr[ i ].p2;
        p3=TrianglePtr[ i ].p3;
        // draw with a thick blue pen
       CPen penBlue(PS_SOLID, 1, RGB(0, 0, 55));
       CPen* pOldPen = pDC->SelectObject(&penBlue);
       // and a solid red brush
       CBrush brushRed(RGB(155, 155, 155));
       CBrush* pOldBrush = pDC->SelectObject(&brushRed);
       // Find the midpoints of the top, right, left, and bottom
       // of the client area. They will be the vertices of our polygon.
       pts[0].x = (int)VertexPtr[ p1 ].x;
       pts[0].y = (int)VertexPtr[ p1 ].y;
       pts[1].x = (int)VertexPtr[ p2 ].x;
       pts[1].y = (int)VertexPtr[ p2 ].y;
       pts[2].x = (int)VertexPtr[ p3 ].x;
       pts[2].y = (int)VertexPtr[ p3 ].y;
       // Calling Polygon() on that array will draw three lines
       // between the points, as well as an additional line to
       // close the shape--from the last point to the first point
       // we specified.
       pDC->Polygon(pts, 3);
       // Put back the old objects.
       pDC->SelectObject(pOldPen);
       pDC->SelectObject(pOldBrush);
    }
    for ( i=0; i<T.GetVertexCount(); i++ )
        Point(pDC,(int)VertexPtr[i].x-2,(int)VertexPtr[ i ].y-2,col );
// draws bounding box of triangulation
    Rectangle( pDC,(int)T.GetMinX(),(int)T.GetMinY(),(int)T.GetMaxX(),(int)T.GetMaxY(),&WhitePen );
// if you want to draw the adjacent triangle just use these lines of code
            j=0;   // we want adjacent traingles to triangle number 0
int p12;
p12=TrianglePtr[ j ].t1;
i ( p12!=-1)    DrawPoly( pDC , p12 ,col2 );
p12=TrianglePtr[ j ].t2;
if ( p12!=-1)    DrawPoly( pDC , p12 ,col1 );
p12=TrianglePtr[ j ].t3;
if ( p12!=-1)    DrawPoly( pDC , p12 ,col3 );
 */

/* Example use
CDelaunay2D T;
Indexed<double> Px;
Indexed<double> Py;
int i=0;
EACH(outer.first,FreetypeContour,c) {
 EACH(c->points.first,Vertex,v) {
  Px.Increase();
  Py.Increase();
  Px[i]=v->x;
  Py[i]=v->y;
  i++;
 }
}
EACH(inner.first,FreetypeContour,c) {
 EACH(c->points.first,Vertex,v) {
  Px.Increase();
  Py.Increase();
  Px[i]=v->x;
  Py[i]=v->y;
  i++;
 }
}
T.Triangulate(Px.list,Py.list,i);
const DELAUNAYVERTEX* VertexPtr=T.GetVertexPtr();
const DELAUNAYTRIANGLE* TrianglePtr=T.GetTrianglePtr();
for ( i=0; i<T.GetTriangleCount(); i ++  ) {
 mPolygon *p=new mPolygon;
 int p1=TrianglePtr[ i ].p1;
 int p2=TrianglePtr[ i ].p2;
 int p3=TrianglePtr[ i ].p3;
 Vertex *a = new Vertex( VertexPtr[ p1 ].x, VertexPtr[ p1 ].y, 0.0 );   p->addVertex(a);
 Vertex *b = new Vertex( VertexPtr[ p2 ].x, VertexPtr[ p2 ].y, 0.0 );   p->addVertex(b);
 Vertex *c = new Vertex( VertexPtr[ p3 ].x, VertexPtr[ p3 ].y, 0.0 );   p->addVertex(c);
 result4.Append(p);
} */

/////////////////////////////////////////////////
// vertex type

typedef struct DELAUNAYVERTEXTYPE { Zdouble x,y; } DELAUNAYVERTEX;

////////////////////////////////////////////
// indexed triangle

typedef struct DELAUNAYTRIANGLETYPE
{
   Zint p1;	//points index
   Zint p2;
   Zint p3;
   Zint t1;	//neigh triangles
   Zint t2;
   Zint t3;
} DELAUNAYTRIANGLE; 

///////////////////////////////////////
// trangle mapping class

class CDelaunayTriangleMap {
  	////////////////////////////////////////////
   // private data
   ZIndexed<unsigned int>	Map;
   Zint			nx;
   Zint			ny;
   Zdouble	step;
   Zdouble	mx;
   Zdouble	my;
   Zp<DELAUNAYTRIANGLE>	t;
   Zp<DELAUNAYVERTEX>	  p;
 public:
 	void AddTriangle(int idt)	{
 		double ccx,ccy;
 		int id=0,idx=0,idy=0,
 		 p1=t[idt].p1,
 		 p2=t[idt].p2,
 		 p3=t[idt].p3;
		//get centroid
		ccx=( p[p1].x+p[p2].x+p[p3].x )/3.0f;
		ccy=( p[p1].y+p[p2].y+p[p3].y )/3.0f;
		// add first point
		idx=(int)((ccx-mx)/step);
		idy=(int)((ccy-my)/step);
		if (idx>nx-1)idx=nx-1;
  else if (idx<0)idx=0;
		if (idy>ny-1)idy=ny-1;
		else if (idy<0)idy=0;
		id=ny.value*idx+idy;
		Map[id]=idt;
	}
	void BuildTMap(double* bbox,int np,DELAUNAYTRIANGLE* tin, DELAUNAYVERTEX* pin,float Density)
	{
	   //save the pointer to triangles and points

	   p=pin;
	   t=tin; 

	   //build the triangles map
    
	   double dx=bbox[1]-bbox[0];  
	   double dy=bbox[3]-bbox[2];
   
	   mx=bbox[0];
	   my=bbox[2];
   
	   step=sqrt( dx*dy/double(np)*Density );
  
	   //+1 prevent to gooutside the map

	   nx=(int)(dx/step+1);
	   ny=(int)(dy/step+1);

	   int size=nx*ny;

    Map.Size((size_t)size);
	   for ( int i=0; i<size; i++ ) Map[ i ]=-1;
	}        
	
	//Returns a triangle id previously allocated in the map
	// this function can be optimimed in the circular saarch phase
	void GetTriangle(DELAUNAYVERTEXTYPE* sp,int* st)	{
	 int id,
 	 idx=(int)((sp->x-mx)/step),
	  idy=(int)((sp->y-my)/step);
  
	 if (idx>nx-1) idx=nx-1;
		else if (idx<0)idx=0;
  
		if (idy>ny-1) idy=ny-1;
	 else if (idy<0)idy=0;
  
		id=ny*idx+idy;

		//check the first box    
		if (Map[id]>=0) {
			*st=Map[id];  
			return;
  }

		 //loop around until a triangle is found -- danger: infinite loop possibilities... :(
		int c,ic1,ic2,jc1,jc2,ii,jj;
		c=1;
		ic1=0;
		ic2=0;
		jc1=0;
		jc2=0;

  int max_tries=1000000;
		while (max_tries-- > 0) {   
			//xdown
			if ( idx-c>-1) {
 	  ic1=-c;
		  ii=-c;
				for (jj=jc1;jj<=jc2;jj++)	{ 
					id=ny*(idx+ii)+idy+jj;
					if (Map[id]>=0) {
						*st=Map[id];  
						return;
     }
				}
	  }
	  //xup
			if ( idx+c<nx ) { 
			 ic2=c;
			 ii=c;
			 for (jj=jc1;jj<=jc2;jj++) {
					id=ny*(idx+ii)+idy+jj;
					if (Map[id]>=0) {
						*st=Map[id];  
						return;
     }
			 }
	  }
			//yup
			if  (idy+c<ny) {
 		 jc2=c;
			 jj=c;
			 for (ii=ic1;ii<=ic2;ii++) {
					id=ny*(idx+ii)+idy+jj;
					if (Map[id]>=0) {
						*st=Map[id];  
						return;
     }
			 }
	  }
			//ydown
			if  (idy-c>-1) {
			 jc1=-c;
			 jj=-c;
		  for (ii=ic1;ii<=ic2;ii++)	{ 
					id=ny*(idx+ii)+idy+jj;
					if (Map[id]>=0) {
						*st=Map[id];  
						return;
     }
		  }
	  }
			c++;
  }
	}
}; 

///////////////////////////////////////

class CDelaunay2D {
 Zfloat DENSITY;                          //mean density of the map referred to points, original value was 5
 Zbool removeDuplicates;
 CDelaunayTriangleMap TMap;				          	// triangulation map
	Zdouble	Bbox[4];				                     // bounding box
 Zuint			np;				                          // number of points
	Zuint			ct;				                          // counter of triangles (final numbers of triangle=ct+1);
 Zuint			nt;				                          // number of effective triangles
	Zdouble	Maximum;				                     // maximum bounding box diagonal transformation factor
 ZIndexed<DELAUNAYVERTEX>		   p;					     // transformed points
 ZIndexed<DELAUNAYTRIANGLE>		 t;		  		    // triangles
	ZIndexed<DELAUNAYTRIANGLE>		 Triangle;			// final triangles
	   
 // functions
	void   PreProcess(DELAUNAYVERTEX* p,int N);
 int    LocatePoint(int* cT,int i);
 void   AddPoint(int idt,int i);
 void   Flip(int T1);
	double Orient2d(double *pa,double *pb,double* pc);
	double IsInCircle(double *pa, double *pb, double *pc,double *pd);
	void   SwapTriangles( int id, int T1, int T2 );
	bool   ComputeAdjacency();
	int	   FindEdgeOf( int id, int a, int b );

 public:
        
		bool Triangulate( double *px,double *py,int inputnp);
	 const double GetMinX(){ return Bbox[0]; } 
	 const double GetMinY(){ return Bbox[2]; } 
	 const double GetMaxX(){ return Bbox[1]; } 
	 const double GetMaxY(){ return Bbox[3]; } 
		const int GetVertexCount() { return np; } 
		const int GetTriangleCount() { return nt; }
		const DELAUNAYVERTEX* GetVertexPtr() { return p.list; }
		const DELAUNAYTRIANGLE* GetTrianglePtr() { return Triangle.list; }
     
		ZIndexed<ZIndexed<Zint>> TriangleHash;
 	ZIndexed<Zint>			        TriangleCounter;
		ZIndexed<Zint>		         TriangleIndex;
		Zint			                   Indices;

		//constructor , destructor
        
	 CDelaunay2D() {
   DENSITY=5.0f;
   removeDuplicates=true;
  }

  void Clear() {
   p.Size(0);
   t.Size(0);
   Triangle.Size(0);
   TriangleHash.Size(0);
   TriangleCounter.Size(0);
   TriangleIndex.Size(0);
   Indices=0;
  }
};