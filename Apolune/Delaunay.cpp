#include "Delaunay.h"

// swap triangles ids

void CDelaunay2D::SwapTriangles( int id, int T1, int T2 ) {
	//id had T1 now it has T2
	if ( id>=0 ) {
		if ( t[id].t1.value==T1 ) t[ id].t1=(int)T2; 
  else if( t[id].t2.value==T1 ) t[id].t2=(int)T2;
	 else t[id].t3=(int)T2;
	}
}

// line orientation in 2d

double CDelaunay2D::Orient2d(double *pa,double *pb,double* pc) {
 double
  acx = pa[0] - pc[0],
  bcx = pb[0] - pc[0],
  acy = pa[1] - pc[1],
  bcy = pb[1] - pc[1];
 return acx * bcy - acy * bcx;
}

// determine if a point is inside a circle
// formed by the 3 points which are 
//circumscribed inside the circle itself

double CDelaunay2D::IsInCircle(double *pa, double *pb, double *pc,double *pd) {
 double
 	adx = pa[0] - pd[0],
 	ady = pa[1] - pd[1],
 	bdx = pb[0] - pd[0],
 	bdy = pb[1] - pd[1],
 	cdx = pc[0] - pd[0],
 	cdy = pc[1] - pd[1],
 	abdet = adx * bdy - bdx * ady,
 	bcdet = bdx * cdy - cdx * bdy,
 	cadet = cdx * ady - adx * cdy,
 	alift = adx * adx + ady * ady,
 	blift = bdx * bdx + bdy * bdy,
 	clift = cdx * cdx + cdy * cdy;
	return alift * bcdet + blift * cadet + clift * abdet;
}

// normalize to the o-1 square a set of 2D points
// it is also used to build the Triangle map;

void CDelaunay2D::PreProcess(DELAUNAYVERTEX *p , int N) {
 int i;
 double nbbox[4];
	// find bounding box
	nbbox[0]= HUGE_VAL;
 nbbox[2]= HUGE_VAL;
 nbbox[1]=-HUGE_VAL;
 nbbox[3]=-HUGE_VAL;
	for ( i=0; i<N; i++ ) {
		if ( p[i].x<nbbox[0] ) nbbox[0]=p[i].x;
		if ( p[i].x>nbbox[1] ) nbbox[1]=p[i].x;
		if ( p[i].y<nbbox[2] ) nbbox[2]=p[i].y;
		if ( p[i].y>nbbox[3] ) nbbox[3]=p[i].y;
 }
	// store original bounding box
	Bbox[0]=nbbox[0];
	Bbox[2]=nbbox[2];
	Bbox[1]=nbbox[1];
	Bbox[3]=nbbox[3];
 //move to origin
 for( i=0; i<N; i++ )	{
  p[i].x-=nbbox[0];
  p[i].y-=nbbox[2];
 }
 //normalize to a 1 diagonal box
	//get maximum value
 ((nbbox[1]-nbbox[0])>(nbbox[3]-nbbox[1])) ? Maximum=nbbox[1]-nbbox[0]: Maximum=nbbox[3]-nbbox[1];
 for(i=0;i<N;i++)	{
  p[i].x/=Maximum;
  p[i].y/=Maximum;
 }
 //move to the origin the bounding box
	nbbox[1]=nbbox[1]-nbbox[0];
	nbbox[3]=nbbox[3]-nbbox[2];
	nbbox[0]=0;
	nbbox[2]=0;
	//normalize the bounding box too 
 nbbox[0]/=Maximum;
 nbbox[1]/=Maximum;
 nbbox[2]/=Maximum;
 nbbox[3]/=Maximum;
	//build the triangle map
 TMap.BuildTMap(nbbox, np, t.list, p, DENSITY);
}

//locate the enclosing triangle for each point.
// this function detecs duplicated points
int CDelaunay2D::LocatePoint(int* cT, int i) {
	// cT current triangle
	// i point to locate
	double o1=1, o2=1, o3=1;	//orientation
 int pe2=-1;			//point of the edge where we come from.
	//it is used to skip the orient call on the same edge where we come from
 //notice that since edges are sorted clockwise, we just need a point to recognize an edge
 //initialize to -1 for first iteration
 // once again, a dangerous loop
 int max_tries=1000000;
 while (--max_tries > 0) {
  int
   p1= t[*cT].p1,
   p2= t[*cT].p2,
   p3= t[*cT].p3;
  //check edge 1 (p2,p3)
  if(p1!=pe2) {
			o1=Orient2d(&p[p1].x.value, &p[p2].x.value, &p[i].x.value );
			if (o1<0)	{
			 pe2=p2;
			 *cT=t[*cT].t1;
			 if (*cT>=0) continue;   // go to next triangle
				else {	// Error in point location;
					return -1;
			 }
			}
  }
		//check edge 1 (p2,p3)
		if(p2!=pe2) {
			o2=Orient2d(&p[p2].x.value, &p[p3].x.value, &p[i].x.value);
			if (o2<0) {
				pe2=p3;
				*cT=t[*cT].t2;
			 if (*cT>=0) continue; // go to next triangle
			 else { // Error in point location
				 return -1;					
				}
			}
  }
  //check edge 1 (p3,p1)
  if(p3!=pe2) {
			o3=Orient2d(&p[p3].x.value, &p[p1].x.value, &p[i].x.value);
			if (o3<0)	{
				pe2=p1;
				*cT=t[*cT].t3;
				if (*cT>=0) continue; // go to next triangle
			 else { // Error in point location
			  return -1;
		  }
			}
  }
		//check for duplicate points can be optimized
  if (o1==0 && o2==0 || o1==0 && o3==0 || o2==0 && o3==0)	{
 		//	duplicate point found
   if ( removeDuplicates )	return -1;
		}
 	//we found it
  return 0; 
 }
	//we found it
 return 0; 
}

//Adds the point i inside the triangle idt
void CDelaunay2D::AddPoint(int idt, int i) {
	//the third one
 t[ct+2].p1=t[idt].p3; t[ct+2].p2=t[idt].p1; t[ct+2].p3=i;
 t[ct+2].t1=t[idt].t3; t[ct+2].t2=idt;       t[ct+2].t3=(int)ct+1;
	//the second one
 t[ct+1].p1=t[idt].p2; t[ct+1].p2=t[idt].p3; t[ct+1].p3=i;
 t[ct+1].t1=t[idt].t2; t[ct+1].t2=(int)ct+2; t[ct+1].t3=idt;
 SwapTriangles(t[idt].t2, idt, ct+1);	//T2 had idt now it has ct+1
 SwapTriangles(t[idt].t3, idt, ct+2); //T3 had idt now it has ct+2
	//the first one replace idt; (//change the first at last)
	//t[idt].p1=p1;t[idt].p2=p2;
	t[idt].p3=i;
	//t[idt].t1=T1;
 t[idt].t2=(int)ct+1;
	t[idt].t3=(int)ct+2;
	// flip the edges if needed
	Flip(idt);
 Flip(ct+1);
 Flip(ct+2);
	//Add to map
	TMap.AddTriangle(idt);
	TMap.AddTriangle(ct+1);
	TMap.AddTriangle(ct+2);
	ct+=2;	//two more triangles
}

//check for flips the edge between triangle T1 & T2
// the code is optized in order to have the flip always on the first edge of T1
void CDelaunay2D::Flip(int T1) {
	//we may end here 
	// if no triangle
 if (t[T1].t1<0) return;
	//Assigning local variables (it can be avoided but it is much more readeable and just alittle slower)
 int
 	p1=t[T1].p1,
 	p2=t[T1].p2,
 	p3=t[T1].p3;
 int	T2=t[T1].t1;

	//T3 T4 moved after the incircle
  
	//find the position of p2 to get the position of p4 T5 T6
	//now we only need p4, this comparison can be repetead after the incircle (slightly optimized)
 int p4=0,T3=0,T4=0,T5=0,T6=0;
	if (t[T2].p1.value==p2) {
		p4=t[T2].p3;
		T5=t[T2].t2;
  T6=t[T2].t3;
	}	else if (t[T2].p2.value==p2)	{
		p4=t[T2].p1; 
		T5=t[T2].t3;
		T6=t[T2].t1;
	}	else	{
		p4=t[T2].p2; 
		T5=t[T2].t1;
		T6=t[T2].t2;
	}

	// incircle test
	double r=IsInCircle( &p[p1].x.value, &p[p2].x.value, &p[p3].x.value, &p[p4].x.value);

 //points is inside a flip is requested
	if (r>0) {
		T3=t[T1].t2;T4=t[T1].t3;
		//flip!
		//t[T1].p1=p1;
		t[T1].p2=p4;
		//t[T1].p3=p3;
		t[T1].t1=T5;
		t[T1].t2=T2;
		//t[T1].t3=T4;
		t[T2].p1=p4; 
		t[T2].p2=p2;
		t[T2].p3=p3;
		t[T2].t1=T6;
		t[T2].t2=T3;
		t[T2].t3=T1;

		//we also need to update something into T4,T5
		SwapTriangles(T3, T1, T2);	//T3 had T1 now it has T2
		SwapTriangles(T5, T2, T1);	// T5 had T2 now it has T1

		//we also need to check p1-p4 & p4 p2
		Flip(T1);	//recursive call
		Flip(T2);	//recursive call
	}
}

bool CDelaunay2D::Triangulate( double *px,double *py,int inputnp) {
 Clear();
	ct=0;
 //copy to class data structure
	np=(unsigned int) inputnp;
	//maximum number of triangles
	const unsigned int maxt=(np+3)*2; 
  //allocate memory

	//+3 for the service triangulation
 p.Size((size_t)(np+3));
 t.Size((size_t)maxt);

	for (unsigned int i=0; i<np; i++ )	{
		p[i].x=px[i];
		p[i].y=py[i];
 }
	
	for (unsigned int i=0; i<maxt; i++ )	{
		t[i].p1=-1;
		t[i].p2=-1;
		t[i].p3=-1;
		t[i].t1=-1;
		t[i].t2=-1;
		t[i].t3=-1;
 }
    
	//copy the input double points
	//normalize points
 PreProcess(p.list, np);
   
 //add the service triangulation (notice this requires an extra space inside
	p[np  ].x= 0.5f;
	p[np  ].y=10.0f;
 p[np+1].x=-9.5f;
	p[np+1].y=-5.0f;
 p[np+2].x=10.5f;
	p[np+2].y=-5.0f;
 t[0].p1=(int)np.value;
	t[0].p2=(int)np.value+1;
	t[0].p3=(int)np.value+2;
		
	//add first triagnle to map  avoids infinite loop
	TMap.AddTriangle(0);

	//current enclosing triagnle
	int cT=0;
 int flag=0;	
	for (unsigned int i=0;i<np;i++)	{
		TMap.GetTriangle(&p[i], &cT);		//finds the enlcosing simplex
		flag=LocatePoint(&cT, i);    		//duplicate point skip!
		if (flag<0) continue;
		//adds the poit i inside the triangle idt
		AddPoint(cT, i);
	}
		
	// now store all triangles in the 
	ZIndexed<bool> isgood;	// indexed triangle structure
	isgood.Size((size_t)ct+1);

	//indicates whether a triangle is good or not
	nt=0;
 for (unsigned int i=0;i<=ct.value;i++ ) {
  if( t[i].p1<(int)np && t[i].p2<(int)np && t[i].p3<(int)np ) { 
			isgood[i]=true;
			nt++;
		} else	isgood[i]=false;
 }

 Triangle.Size((size_t)nt);
	int c=0;
	for (unsigned int i=0; i<=ct.value;i++)	{
	 if(isgood[i]) {
   Triangle[c].p1=t[i].p1;
   Triangle[c].p2=t[i].p2;
   Triangle[c].p3=t[i].p3;
   Triangle[c].t1=-1;
   Triangle[c].t2=-1;
   Triangle[c].t3=-1;
	  c++;
	 } 
	}
 	
 // undo trasnformation
	// to restore original points
 for(unsigned int i=0;i<np;i++) {
  p[i].x*=Maximum;
  p[i].y*=Maximum;
 } 
 for(unsigned int i=0; i<np; i++ ) {
  p[i].x+=Bbox[0];
  p[i].y+=Bbox[2];
 }
 
	// compute triangle adjacency
	return ComputeAdjacency();
}

int CDelaunay2D::FindEdgeOf( int j, int p1, int p2 ) {
	int p12=-1;
	Indices=0;

	// fill trianglearray with triangle indices indexed by the first 
	// index of current triangle , p1
	for (unsigned int i=0; i<TriangleHash[p1].length; i++ ) {
	 if ( i!=j ) { 
	  TriangleCounter[i]++;
	  TriangleIndex[ Indices++ ]=(int)i;
  }
 }
	// fill trianglearray with triangle indices indexed by the 
	// second  index of current triangle , p2, if the triangle index 
	// is already present, the ret value will be false,that means 
	// the triangle is already inserted andits sharing the edge 
	// p1 and p2 , so mark it as edge neibourgh for the current triangle
 for (unsigned int i=0; i<TriangleHash[ p2 ].length; i++ ) if ( i!=(int)j ) {
		TriangleCounter[i]++; 
		if ( TriangleCounter[i].value>1 ) { p12=(int)i; break; }
		TriangleIndex[ Indices++ ]=(int)i;
 }
	for ( unsigned int i=0; i<Indices; i++ ) TriangleCounter[ TriangleIndex[i] ]=0;
	return p12;
}

bool CDelaunay2D::ComputeAdjacency() {
	// create an hashmap for fast vertex indexing
 TriangleHash.Size((size_t)np);	
	// create an array wher to store triangles
	// found adjacent to a each triangle edge
	// create the triangle hasmap
	for (unsigned int i=0; i<nt; i++ )	{
  unsigned int
 		p1=Triangle[i].p1,
 		p2=Triangle[i].p2,
 		p3=Triangle[i].p3;
		(*(TriangleHash[p1].Increase()))=(int)i;
		(*(TriangleHash[p2].Increase()))=(int)i;
		(*(TriangleHash[p3].Increase()))=(int)i;
	}

	// alloctae memory for triangle adjacency
 TriangleIndex.Size((size_t)nt);
 TriangleCounter.Size((size_t)nt);

	// compute triangle adjacency for each triangle
	for (unsigned int j=0; j<nt; j++ )	{	
		Triangle[j].t1=FindEdgeOf( j,Triangle[ j ].p1,Triangle[ j ].p2 );
		Triangle[j].t2=FindEdgeOf( j,Triangle[ j ].p2,Triangle[ j ].p3 );
		Triangle[j].t3=FindEdgeOf( j,Triangle[ j ].p1,Triangle[ j ].p3 );
 }
	return true;
}




