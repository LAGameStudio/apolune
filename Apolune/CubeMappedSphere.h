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

#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "Cartesian.h"
#include "Display.h"

#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "VBO.h"
#include "Chemistry.h"
#include "Primitive3d.h"
#include "Sphere.h"

#include "Indexed.h"

#if !defined(FRONT_ID)
#define FRONT_ID  0
#define RIGHT_ID  1
#define BACK_ID   2
#define LEFT_ID   3
#define TOP_ID    4
#define BOTTOM_ID 5
#endif

// Woody March-Steinman Sep 2011
class CubeMappedSphere3d : public Primitive3d {
public:
	/*Creating a sphere from cube vertices.
	http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
	 [x1]  [x sqrt(1-(y^2/2)-(z^2/2)+((y^2*z^2)/3)]
	 [y1]= [y sqrt(1-(z^2/2)-(x^2/2)+((z^2*x^2)/3)]
	 [z1]  [z sqrt(1-(x^2/2)-(y^2/2)+((x^2*y^2)/3)]

	  /|-------/|
	 /_|______/ |
	 | |      | |  order of face generation: front, left, back, top, right, bottom
	 | |      | |
	 | |      | |
	 | /------|-/
	 |/_______|/

	 ____________
	 |/|/|/|/|/|/| triangle generation: res iterations on outer loop per side, res iterations on inner loop per side.
	 ____________
	 |/|/|/|/|/|/| repeats 6 times, one per face.
	 ____________           (v2+1-1) 
	 |/|/|/|/|/|/|   v2 ------ v2+1
	 ____________       |2  /|
	 |/|/|/|/|/|/|      |  / |
	 ____________       | /1 |
	 |/|/|/|/|/|/|   v1 |/  _| v1+1
                   
	*/
 float r,r2;
 int res;
 float unit;
 float unit2;
 int arraySize;
 float deltaunit;
 Indexed<Zfloat> pointsX;
 Indexed<Zfloat> pointsY;
 Indexed<Zfloat> pointsZ;
 int F,L,B,R,D,U;
 Indexed<VBOStaticVNT> vboVNT;
 CubeMappedSphere3d() : Primitive3d() {
  res=10; // not all numbers, works: 10, 64
  arraySize=(int) (res+1)*(res+1)*6+1;
  pointsX.Size(arraySize);
  pointsY.Size(arraySize);
  pointsZ.Size(arraySize);
  vboVNT.Size(6);
  unit=2.0f;
  unit2=1.0f;
  deltaunit=unit/res;
  r=0.5f;
  r2=r/2.0f;
  F=L=B=R=D=U=0;
 }

 void SetRes( int r ) {
  res=r;
  arraySize=(int) (res+1)*(res+1)*6+1;
  pointsX.Size(arraySize);
  pointsY.Size(arraySize);
  pointsZ.Size(arraySize);
  deltaunit=unit/res;
 }

 Vertex n;

 void VertexBufferAndImmediate() {
  if ( pList ) pList->Clear();
  else
  pList = new PolygonList;
  vbo16.Clear();
  model.polygons.Clear();

  mPolygon *p, *first;
  Vertex *w;
  int group=model.groups+1;
  model.groups++;

  int v1=0,v2=0;
  int pos=0;
  float x=0.0f,y=0.0f,z=0.0f;


  //generate points for unit sphere
  //front face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pointsZ[pos]= 1 * sqrt( 1.0f - SQ(x)/2.0f - SQ(y)/2.0f + (SQ(x)*SQ(y))/3.0f );
    pos++;
   }
  }
  F=pos;
  //left face
  for ( z=unit2; z>=-unit2; z-=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= 1 * sqrt( 1.0f - SQ(y)/2.0f - SQ(z)/2.0f + (SQ(y)*SQ(z))/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pos++;
   }
  }
  L=pos;
   //back face
  for ( x=unit2; x>=-unit2; x-=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pointsZ[pos]= -1 * sqrt( 1.0f - SQ(x)/2.0f - SQ(y)/2.0f + (SQ(x)*SQ(y))/3.0f );
    pos++;
   }
  }
  B=pos;

 //right face
  for ( z=-unit2; z<=unit2; z+=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= -1 * sqrt( 1.0f- SQ(y)/2.0f - SQ(z)/2.0f + (SQ(y)*SQ(z))/3.0f );
    pointsY[pos]= y * sqrt( 0.5f- SQ(z)/2.0f + SQ(z)/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pos++;
   }
  }
  R=pos;
   //bottom face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( z=-unit2; z<=unit2; z+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsY[pos]= 1 * sqrt( 1.0f - SQ(z)/2.0f - SQ(x)/2.0f + (SQ(z)*SQ(x))/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pos++;
   }
  }
  D=pos;
  //top face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( z=-unit2; z<=unit2; z+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsY[pos]= -1 * sqrt( 1.0f - SQ(z)/2.0f - SQ(x)/2.0f + (SQ(z)*SQ(x))/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pos++;
   }
  }
  U=pos;
  v1=0;
  v2=res+1;
  for (int i=0;i<arraySize;i++){
   if (i==(arraySize-1)){
    pointsX[i]=pointsX[i-1];
   	pointsY[i]=pointsY[i-1];
   	pointsZ[i]=pointsZ[i-1];
    continue;
   }
   pointsX[i]=pointsX[i]*r;
   pointsY[i]=pointsY[i]*r;
   pointsZ[i]=pointsZ[i]*r;
  }
  //polygon generation
  int count=0;
  float rD=iratiof(1,res);
  for (int k=0; k<6; k++){
   float tcx=0.0f;
   for (int j=0; j<=res; j++){//per x
    float tcy=1.0f;
   	int temp=0;    
    for (int i=0; i<=res;i++){//per y                     // 
     if (i==res) {v1++; v2++; break;}                     // v1+1    __  v2+1 
	    count++;                                             //      |\   | 
	    //two triangles together per column segment          // |    | \ 2|
     mPolygon *t1,*t2;                                    //      |1 \ |
     Vertex *n1,*n2;                                      // v1   |__ \| v2 (v2-1)
   	 if ((k==4||k==3)&&j==res) {v1++;v2++; continue;}  
     if ( k==5&&j==res) {v1++;v2++; break;}   
     if ( k == 4 ) { // bottom
      float xU=tcx,xV=tcx+rD;
      float yU=1.0f-tcy,yV=1.0f-(tcy-rD); // Y is inverted on bottom face
   	  t1=p=new mPolygon; // tri1
      model.addPolygon(p);
      p->group=BOTTOM_ID;
      pList->Push(p);
      p->draw_method = GL_TRIANGLES;
	     if (j==0&&i==0&&k==0) first=p; v1++;
 	    w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1] );
      w->q=xU;
      w->r=yV;
      p->addVertex(w);
      v1--;
      w=new Vertex( pointsX[v2], pointsY[v2], pointsZ[v2] );
      w->q=xV;
      w->r=yU;
      p->addVertex(w);
      v2++; 
      w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1] );
      w->q=xU;
      w->r=yU;
      p->addVertex(w);
      v1++;
      p->TriNormal();
      n1=p->normal;
      t2=p=new mPolygon; // tri2
      model.addPolygon(p);
      p->group=BOTTOM_ID;
      pList->Push(p);
      p->draw_method = GL_TRIANGLES;
   	  w=new Vertex( pointsX[v2], pointsY[v2], pointsZ[v2] );
      w->q=xV;
      w->r=yV;
      p->addVertex(w);
	     w=new Vertex( pointsX[v2-1], pointsY[v2-1], pointsZ[v2-1] );
      w->q=xV;
      w->r=yU;
      p->addVertex(w);
	     w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1] );
      w->q=xU;
      w->r=yV;
      p->addVertex(w);
      p->TriNormal();
      n2=p->normal;
     } else { // other faces
      float xU=tcx,xV=tcx+rD;
      float yU=tcy,yV=tcy-rD;
      int pgroup=k;
      switch ( pgroup ) {
      case 0: pgroup=FRONT_ID; break;
      case 1: pgroup=LEFT_ID; break;
      case 2: pgroup=BACK_ID; break;
      case 3: pgroup=RIGHT_ID; break;
//      case 4: pgroup=BOTTOM_ID; break;
      case 5: pgroup=TOP_ID; break;
      default: OUTPUT("CubeMappedSphere::VertexBufferAndImmediate() encountered %d, an invalid model group id.\n",k); break;
      }
 	    t1=p=new mPolygon; // tri1
      model.addPolygon(p);
      p->group=pgroup;
      pList->Push(p);
      p->draw_method = GL_TRIANGLES;
	     if (j==0&&i==0&&k==0) first=p;
      w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1]);
      w->q=xU;
      w->r=yU;
      p->addVertex(w);
      v1++;
      w=new Vertex( pointsX[v2], pointsY[v2], pointsZ[v2]);
      w->q=xV;
      w->r=yU;
      p->addVertex(w);
      v2++; 
 	    w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1]);
      w->q=xU;
      w->r=yV;
      p->addVertex(w);
      p->TriNormal();
      n1=p->normal;
      t2=p=new mPolygon; // tri2
      model.addPolygon(p);
      p->group=pgroup;
      pList->Push(p);
      p->draw_method = GL_TRIANGLES;
   	  w=new Vertex( pointsX[v1], pointsY[v1], pointsZ[v1] );
      w->q=xU;
      w->r=yV;
      p->addVertex(w);
	     w=new Vertex( pointsX[v2-1], pointsY[v2-1], pointsZ[v2-1]);
      w->q=xV;
      w->r=yU;
      p->addVertex(w);
	     w=new Vertex(pointsX[v2], pointsY[v2], pointsZ[v2]);
      w->q=xV;
      w->r=yV;
      p->addVertex(w);
      p->TriNormal();
      n2=p->normal;
     }
     tcy-=rD;
    }//end i
    tcx+=rD;
   }//end j
  }//end k

  // Creates seperate VBOs for F,L,B,U,D,R
  vboVNT[FRONT_ID].fromModel(&model,FRONT_ID);
  vboVNT[RIGHT_ID].fromModel(&model,RIGHT_ID);
  vboVNT[BACK_ID].fromModel(&model,BACK_ID);
  vboVNT[LEFT_ID].fromModel(&model,LEFT_ID);
  vboVNT[TOP_ID].fromModel(&model,TOP_ID);
  vboVNT[BOTTOM_ID].fromModel(&model,BOTTOM_ID);
  for ( int i=0; i<6; i++ ) vboVNT[i].Create();
 }
};

extern CubeMappedSphere3d cubesphere3d;

// Holds precomputed spheres for later render / deformation
class PrecomputedCubeMappedSphereProfile : public ListItem {
public:
 float r,r2;
 int resolution;
 float unit;
 float unit2;
 int arraySize;
 float deltaunit;
 Indexed<Zfloat> pointsX;
 Indexed<Zfloat> pointsY;
 Indexed<Zfloat> pointsZ;
 int F,L,B,R,D,U; // face group start offsets
 PrecomputedCubeMappedSphereProfile( int resolution, float r ) {
  this->r=r;
  this->r2=r/2.0f;
  unit=2.0f;
  unit2=1.0f;
  this->SetRes(resolution);
  Generate();
 }
 void SetRes( int segments ) {
  resolution=segments; // not all numbers, works: 10, 64
  arraySize=(int) (resolution+1)*(resolution+1)*6+1;
  pointsX.Size(arraySize);
  pointsY.Size(arraySize);
  pointsZ.Size(arraySize);
  deltaunit=unit/(float)resolution;
 }
 void Generate() {
  int pos=0;
  float x=0.0f,y=0.0f,z=0.0f;

  //generate points for unit sphere
  //front face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pointsZ[pos]= 1.0f * sqrt( 1.0f - SQ(x)/2.0f - SQ(y)/2.0f + (SQ(x)*SQ(y))/3.0f );
    pos++;
   }
  }
  F=pos;
  //left face
  for ( z=unit2; z>=-unit2; z-=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= 1.0f * sqrt( 1.0f - SQ(y)/2.0f - SQ(z)/2.0f + (SQ(y)*SQ(z))/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pos++;
   }
  }
  L=pos;
   //back face
  for ( x=unit2; x>=-unit2; x-=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pointsY[pos]= y * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pointsZ[pos]= -1.0f * sqrt( 1.0f - SQ(x)/2.0f - SQ(y)/2.0f + (SQ(x)*SQ(y))/3.0f );
    pos++;
   }
  }
  B=pos;

 //right face
  for ( z=-unit2; z<=unit2; z+=deltaunit ) {
   for ( y=-unit2; y<=unit2; y+=deltaunit ) {
    pointsX[pos]= -1.0f * sqrt( 1.0f- SQ(y)/2.0f - SQ(z)/2.0f + (SQ(y)*SQ(z))/3.0f );
    pointsY[pos]= y * sqrt( 0.5f- SQ(z)/2.0f + SQ(z)/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(y)/2.0f + SQ(y)/3.0f );
    pos++;
   }
  }
  R=pos;
   //bottom face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( z=-unit2; z<=unit2; z+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsY[pos]= 1.0f * sqrt( 1.0f - SQ(z)/2.0f - SQ(x)/2.0f + (SQ(z)*SQ(x))/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pos++;
   }
  }
  D=pos;
  //top face
  for ( x=-unit2; x<=unit2; x+=deltaunit ) {
   for ( z=-unit2; z<=unit2; z+=deltaunit ) {
    pointsX[pos]= x * sqrt( 0.5f - SQ(z)/2.0f + SQ(z)/3.0f );
    pointsY[pos]= -1.0f * sqrt( 1.0f - SQ(z)/2.0f - SQ(x)/2.0f + (SQ(z)*SQ(x))/3.0f );
    pointsZ[pos]= z * sqrt( 0.5f - SQ(x)/2.0f + SQ(x)/3.0f );
    pos++;
   }
  }
  U=pos;
 }
};

class PrecomputedCubeMappedSphereProfiles : public LinkedList {
public:
 void Generate() {
  Clear();
  Append(new PrecomputedCubeMappedSphereProfile(2,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(3,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(4,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(5,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(7,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(8,0.5f));
  Append(new PrecomputedCubeMappedSphereProfile(9,0.5f));
//  Append(new PrecomputedCubeMappedSphereProfile(10,0.5f)); // high poly count, useful only after VBOs work
//  Append(new PrecomputedCubeMappedSphereProfile(11,0.5f));
//  Append(new PrecomputedCubeMappedSphereProfile(13,0.5f));
//  Append(new PrecomputedCubeMappedSphereProfile(14,0.5f));
//  Append(new PrecomputedCubeMappedSphereProfile(15,0.5f));
//  Append(new PrecomputedCubeMappedSphereProfile(16,0.5f));
 }
 PrecomputedCubeMappedSphereProfile *find( int resolution ) {
  FOREACH(PrecomputedCubeMappedSphereProfile,sphere) if ( sphere->resolution==resolution ) return sphere;
  return null;
 }
 CLEARLISTRESET(PrecomputedCubeMappedSphereProfile);
};

extern PrecomputedCubeMappedSphereProfiles spheres,spheresHiRes;

// Builds a sphere model / vbo from a profile.
class ProfiledCubeMappedSphere3d : public Primitive3d {
public:
 PrecomputedCubeMappedSphereProfile *sphere;
 float r;
 Indexed<Zfloat> pointsX; // points copied from the profile, scaled by the radius
 Indexed<Zfloat> pointsY;
 Indexed<Zfloat> pointsZ;
 Indexed<VBOStaticVNT> vboVNT;
 ProfiledCubeMappedSphere3d( float radius, PrecomputedCubeMappedSphereProfile *sphere ) : Primitive3d() {
  this->sphere=sphere;
  r=radius;
  vboVNT.Size(6);
  VertexBufferAndImmediate();
 }
 void VertexBufferAndImmediate() {
  pointsX.Size(sphere->arraySize);
  pointsY.Size(sphere->arraySize);
  pointsZ.Size(sphere->arraySize);
  if ( pList ) pList->Clear();
  else
  pList = new PolygonList;
  vbo16.Clear();
  model.polygons.Clear();

  Vertex *w;
  int group=model.groups+1;
  model.groups++;
  
  int v1=0;
  int v2=sphere->resolution+1;
  // copy / scale step
  for (int i=0;i<sphere->arraySize;i++){
   if (i==(sphere->arraySize-1)){
    pointsX[i]=pointsX[i-1];
   	pointsY[i]=pointsY[i-1];
   	pointsZ[i]=pointsZ[i-1];
    continue;
   }
   pointsX[i]=sphere->pointsX[i]*r;
   pointsY[i]=sphere->pointsY[i]*r;
   pointsZ[i]=sphere->pointsZ[i]*r;
  }
  //polygon generation
  int count=0;
  float rD=iratiof(1,sphere->resolution);
  for (int k=0; k<6; k++){
   mPolygon *p;
   float tcx=0.0f;
   for (int j=0; j<=sphere->resolution; j++){//per x
    float tcy=1.0f;
	   int temp=0;                                         
    for (int i=0; i<=sphere->resolution;i++){//per y     // 
    if (i==sphere->resolution) {v1++; v2++; break;}      // v1+1    __  v2+1 
	   count++;                                             //      |\   | 
	 //two triangles together per column segment            // |    | \ 2|
     mPolygon *t1,*t2;                                   //      |1 \ |
     Vertex *n1,*n2;                                     // v1   |__ \| v2 (v2-1)
	    if ((k==4||k==3)&&j==sphere->resolution) {v1++;v2++; continue;}  
     if ( k==5&&j==sphere->resolution) {v1++;v2++; break;}
     if ( k == 4 ) { // bottom
      float xU=tcx,xV=tcx+rD;
      float yU=1.0f-tcy,yV=1.0f-(tcy-rD); // Y is inverted on bottom face
 	    t1=p=new mPolygon; model.addPolygon(p); p->group=BOTTOM_ID; pList->Push(p);// tri1
      p->draw_method = GL_TRIANGLES;
      v1++;
 	    w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yV; p->addVertex(w); v1--;
      w=new Vertex; w->x= (pointsX[v2]);   w->y= (pointsY[v2]);   w->z= (pointsZ[v2]);   w->q=xV; w->r=yU; p->addVertex(w); v2++; 
      w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yU; p->addVertex(w); v1++;
      p->TriNormal(); n1=p->normal;
      t2=p=new mPolygon; model.addPolygon(p); p->group=BOTTOM_ID; pList->Push(p);// tri2
      p->draw_method = GL_TRIANGLES;
	     w=new Vertex; w->x= (pointsX[v2]);   w->y= (pointsY[v2]);   w->z= (pointsZ[v2]);   w->q=xV; w->r=yV; p->addVertex(w);
	     w=new Vertex; w->x= (pointsX[v2-1]); w->y= (pointsY[v2-1]); w->z= (pointsZ[v2-1]); w->q=xV; w->r=yU; p->addVertex(w);
	     w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yV; p->addVertex(w);
      p->TriNormal(); n2=p->normal;
     } else { // other faces
      float xU=tcx,xV=tcx+rD;
      float yU=tcy,yV=tcy-rD;
      int pgroup=k;
      switch ( pgroup ) {
      case 0: pgroup=FRONT_ID; break;
      case 1: pgroup=LEFT_ID; break;
      case 2: pgroup=BACK_ID; break;
      case 3: pgroup=RIGHT_ID; break;
//      case 4: pgroup=BOTTOM_ID; break;
      case 5: pgroup=TOP_ID; break;
      default: OUTPUT("CubeMappedSphere::VertexBufferAndImmediate() encountered %d, an invalid model group id.\n",k); break;
      }
   	  t1=p=new mPolygon; model.addPolygon(p); p->group=pgroup; pList->Push(p);// tri1
      p->draw_method = GL_TRIANGLES;
      w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yU; p->addVertex(w); v1++;
      w=new Vertex; w->x= (pointsX[v2]);   w->y= (pointsY[v2]);   w->z= (pointsZ[v2]);   w->q=xV; w->r=yU; p->addVertex(w); v2++; 
   	  w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yV; p->addVertex(w);
      p->TriNormal(); n1=p->normal;
      t2=p=new mPolygon; model.addPolygon(p); p->group=pgroup; pList->Push(p);// tri2
      p->draw_method = GL_TRIANGLES;
	     w=new Vertex; w->x= (pointsX[v1]);   w->y= (pointsY[v1]);   w->z= (pointsZ[v1]);   w->q=xU; w->r=yV; p->addVertex(w);
	     w=new Vertex; w->x= (pointsX[v2-1]); w->y= (pointsY[v2-1]); w->z= (pointsZ[v2-1]); w->q=xV; w->r=yU; p->addVertex(w);
	     w=new Vertex; w->x= (pointsX[v2]);   w->y= (pointsY[v2]);   w->z= (pointsZ[v2]);   w->q=xV; w->r=yV; p->addVertex(w);
      p->TriNormal(); n2=p->normal;
     }
     tcy-=rD;
    }//end i
    tcx+=rD;
   }//end j
  }//end k
  
  // Creates seperate VBOs for F,L,B,U,D,R
  vboVNT[FRONT_ID].fromModel(&model,FRONT_ID);
  vboVNT[RIGHT_ID].fromModel(&model,RIGHT_ID);
  vboVNT[BACK_ID].fromModel(&model,BACK_ID);
  vboVNT[LEFT_ID].fromModel(&model,LEFT_ID);
  vboVNT[TOP_ID].fromModel(&model,TOP_ID);
  vboVNT[BOTTOM_ID].fromModel(&model,BOTTOM_ID);
  for ( int i=0; i<6; i++ ) vboVNT[i].Create();
 }
};

class PrecomputedCubeMappedSphere3ds : public Primitives3d {
public:
 ProfiledCubeMappedSphere3d *find( PrecomputedCubeMappedSphereProfile *profile ) {
  FOREACH(ProfiledCubeMappedSphere3d,sphere) if ( sphere->sphere==profile ) return sphere;
  OUTPUT("PrecomputedcubeMappedSphere3ds::find() could not find that profile.\n");
  return null;
 }
};

extern PrecomputedCubeMappedSphere3ds sphereModels,sphereModelsHiRes;

void PrecomputeSpheres();