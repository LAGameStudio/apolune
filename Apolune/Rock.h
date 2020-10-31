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
#include "moremath.h"
#include "Primitive3d.h"
#include "VBO.h"
#include "Randoms.h"
#include "FBO.h"
#include "CubeMappedSphere.h"

/*
  Creating a sphere from a profiled cube mapped sphere.
  Deform the sphere with seeded noise.
*/
class Rock3d : public Primitive3d {
public:
 PrecomputedCubeMappedSphereProfile *sphere;
 Random *set;
 Indexed<Zfloat> pointsX; // points copied from the profile and modified by the deformer
 Indexed<Zfloat> pointsY;
 Indexed<Zfloat> pointsZ;
 float roughness;
 int seed;
 bool holes;
 Indexed<VBOStaticVNT> vboVNT;
 Rock3d() : Primitive3d() {
  Name("");
  this->sphere=(PrecomputedCubeMappedSphereProfile *) spheres.first;
  this->seed=0;
  this->holes=false;
  this->roughness=1.0f;
  set=null;
  vboVNT.Size(6);
 }
 Rock3d( PrecomputedCubeMappedSphereProfile *sphere, int seed, bool holes, float roughness ) : Primitive3d() {
  Name("");
  this->sphere=sphere;
  this->seed=seed;
  this->holes=holes;
  this->roughness=roughness;
  pointsX.Size(sphere->arraySize);
  pointsY.Size(sphere->arraySize);
  pointsZ.Size(sphere->arraySize);
  set=randoms.GetSet(seed);
  vboVNT.Size(6);
  VertexBufferAndImmediate();
 }
 void BinaryWrite( BinaryFile *file ) {
  file->write(&sphere->resolution);
  file->write(&seed);
  file->write(&roughness);
  int temp=holes?1:0; file->write(&temp);
  model.BinaryWrite(file);
 }
 void BinaryRead( BinaryFile *file ) {
  int resolution; file->read(&resolution); sphere=spheres.find(resolution);
  file->read(&seed);
  file->read(&roughness);
  int temp; file->read(&temp); holes=temp==1;
  model.BinaryRead(file);
 }
 void VertexBufferAndImmediate() {
  if ( holes ) DeformWithHoles();
  else DeformWithoutHoles();
 }

 Vertex n;

 void DeformWithHoles() {
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
  // copy / deformation / scale step
  for (int i=0;i<sphere->arraySize;i++){
   if (i==(sphere->arraySize-1)){
    pointsX[i]=pointsX[i-1];
   	pointsY[i]=pointsY[i-1];
   	pointsZ[i]=pointsZ[i-1];
    continue;
   }
   pointsX[i]=sphere->pointsX[i]*sphere->r+sphere->pointsX[i]*sphere->r*roughness*set->unique(i,i,i);
   pointsY[i]=sphere->pointsY[i]*sphere->r+sphere->pointsY[i]*sphere->r*roughness*set->unique(i,i+1,i);
   pointsZ[i]=sphere->pointsZ[i]*sphere->r+sphere->pointsZ[i]*sphere->r*roughness*set->unique(i,i,i+1);
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
    for (int i=0; i<=sphere->resolution;i++){//per y      // 
     if (i==sphere->resolution) {v1++; v2++; break;}      // v1+1    __  v2+1 
	    count++;                                             //      |\   | 
   	 //two triangles together per column segment          // |    | \ 2|
     mPolygon *t1,*t2;                                    //      |1 \ |
     Vertex *n1,*n2;                                      // v1   |__ \| v2 (v2-1)
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
      default: OUTPUT("CubeMappedSphere::VertexBufferAndImmediate() encountered %d, an invalid model group id.\n", (int) k); break;
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

  for ( int i=0; i<6; i++ ) {
   vboVNT[i].fromModel(&model,i);
   vboVNT[i].Create();
  }
 }

 void DeformWithoutHoles() {
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
  // copy / deformation / scale step
  // to eliminate holes, we have to identify like verts and modify them at the same time
  Indexed<Zbool> modified;
  modified.Size(sphere->arraySize); // temporary array to make sure we don't double mod a vert
  for ( int i=0; i<sphere->arraySize; i++ ) modified[i]=false;
  for (int i=0;i<sphere->arraySize;i++){
   if (i==(sphere->arraySize-1)){
    pointsX[i]=pointsX[i-1];
   	pointsY[i]=pointsY[i-1];
   	pointsZ[i]=pointsZ[i-1];
    continue;
   }
   if ( modified[i] ) continue;
   float proposedX=sphere->pointsX[i]*sphere->r+sphere->pointsX[i]*sphere->r*roughness*set->unique(i,i,i);
   float proposedY=sphere->pointsY[i]*sphere->r+sphere->pointsY[i]*sphere->r*roughness*set->unique(i,i+1,i);
   float proposedZ=sphere->pointsZ[i]*sphere->r+sphere->pointsZ[i]*sphere->r*roughness*set->unique(i,i,i+1);
   int testX=(int)(sphere->pointsX[i]*1000.0f); // Use an integer test on the 1000ths place
   int testY=(int)(sphere->pointsY[i]*1000.0f); // to determine if the values are similar
   int testZ=(int)(sphere->pointsZ[i]*1000.0f); // enough to be essentially the same
   for ( int j=0; j<sphere->arraySize; j++) if ( j != i ) {
    if ( (int)(sphere->pointsX[j]*1000.0f)==testX
      && (int)(sphere->pointsY[j]*1000.0f)==testY
      && (int)(sphere->pointsZ[j]*1000.0f)==testZ ) {
       pointsX[j]=proposedX;
       pointsY[j]=proposedY;
       pointsZ[j]=proposedZ;
       modified[j]=true;
      }
   }
   pointsX[i]=proposedX;
   pointsY[i]=proposedY;
   pointsZ[i]=proposedZ;
   modified[i]=true;
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
    for (int i=0; i<=sphere->resolution;i++){//per y      // 
     if (i==sphere->resolution) {v1++; v2++; break;}      // v1+1    __  v2+1 
	    count++;                                             //      |\   | 
	    //two triangles together per column segment          // |    | \ 2|
     mPolygon *t1,*t2;                                    //      |1 \ |
     Vertex *n1,*n2;                                      // v1   |__ \| v2 (v2-1)
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
      default: OUTPUT("CubeMappedSphere::VertexBufferAndImmediate() encountered %d, an invalid model group id.\n", (int) k); break;
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

  for ( int i=0; i<6; i++ ) {
   vboVNT[i].fromModel(&model,i);
   vboVNT[i].Create();
  }
 }
};

class Rock3dCollection : public LinkedList {
public:
 int variationSmall, variationBig;
 float roughnessDelta;
 Rock3dCollection() : LinkedList() {
  variationSmall=10;
  variationBig=5;
  roughnessDelta=0.1f;
 }
 Rock3dCollection( const char *filename ) : LinkedList() {
  variationSmall=10;
  variationBig=5;
  roughnessDelta=0.1f;
  //..//
 }
 void ReadAppend( BinaryFile *open ) {
  Rock3d *rock=new Rock3d();
  rock->BinaryRead(open);
  Append(rock);
 }
 void Generate( int variationSmall, int variationBig, float lowestRoughness, float roughnessDelta, float maxRoughness ) {
  this->variationSmall=variationSmall;
  this->variationBig=variationBig;
  this->roughnessDelta=roughnessDelta;
  EACH(spheres.first,PrecomputedCubeMappedSphereProfile,s) {
   for ( float f=0.3f; f<maxRoughness; f+=roughnessDelta ) {
    for ( int i=0; i<(s->resolution<4?variationSmall:variationBig); i++ ) {
     Append(new Rock3d(s,1000+i,true,f));
     Append(new Rock3d(s,2001+i,false,f));
    }
   }
  }
  OUTPUT("Rock3dCollection:Generate created %d rocks of varying sizes, roughness and source geometry.\n", (int) count );
 }
 Rock3d *Find( int sphereResolutionL, int sphereResolutionH, int seed, bool holes, float roughness ) {
  Rock3d *best_match=(Rock3d *) first;
  float diff=1.0f;
  // Normalize
  int i=0;
  FOREACH(Rock3d,rock) if ( rock->holes == holes && rock->sphere->resolution >= sphereResolutionL && rock->sphere->resolution <= sphereResolutionH ) {
   float d=abs(rock->roughness-roughness);
   if ( d <= diff ) {
    diff=d;
    best_match=rock;
    i++;
    if ( randoms.Unique(seed+i,rock->seed) < (rock->sphere->resolution < 4 ? 1.0f/(float)variationSmall : 1.0f/(float)variationBig) ) return rock;
   }
  }
  return best_match;
 }
 CLEARLISTRESET(Rock3d);
 ~Rock3dCollection() { Clear(); }
};

extern Rock3dCollection rocks, rocksHiRes;

void InitAsteroids();

/*
#include "LookAtPerspective.h"
#include "UglyFont.h"

class Rock3dTest : public GLWindow {
public: 
 PrecomputedCubeMappedSphereProfile *active;
 LookAtPerspective lap;
 ImmediateLighting lights;
 CubeMappedSphere3d core;
 Rock3d *rock;
 int resolution;
 GLImage *texture;
 int seed;
 GLImage *F1,*L1,*B1,*R1,*U1,*D1; // outer
 GLImage *F2,*L2,*B2,*R2,*U2,*D2; // inner
 void OnLoad() {
  SetPositionAndSize((int)(uniform()*(double)display->w2),(int)(uniform()*(double)display->h2),display->w2,display->h2);
  background.Pick(clear);
  resolution=10;
  seed=0;
  rock=new Rock3d(
   active=(PrecomputedCubeMappedSphereProfile *) profiles.last,
   seed,
   seed%2==1,
   0.25f
  );
  rock->VertexBufferAndImmediate();
  core.r=0.5f;
  core.r2=0.25f;
  core.SetRes(7);
  core.VertexBufferAndImmediate();
  U1=library.find("data/images/planets/surfaces/jewel/jewel_top3.png");
  D1=library.find("data/images/planets/surfaces/jewel/jewel_bottom4.png");
  L1=library.find("data/images/planets/surfaces/jewel/jewel_left2.png");
  R1=library.find("data/images/planets/surfaces/jewel/jewel_right1.png");
  F1=library.find("data/images/planets/surfaces/jewel/jewel_front5.png");
  B1=library.find("data/images/planets/surfaces/jewel/jewel_back6.png");
  // interior
  U2=library.find("data/images/planets/surfaces/zach/zach_top3.png");
  D2=library.find("data/images/planets/surfaces/zach/zach_bottom4.png");
  L2=library.find("data/images/planets/surfaces/zach/zach_left2.png");
  R2=library.find("data/images/planets/surfaces/zach/zach_right1.png");
  F2=library.find("data/images/planets/surfaces/zach/zach_front5.png");
  B2=library.find("data/images/planets/surfaces/zach/zach_back6.png");
  scale.x=1.0;
  scale.y=1.0;
  scale.z=1.0;
 }
 void WheelUp() {
  resolution++;
  active=(PrecomputedCubeMappedSphereProfile *) active->next;
  if ( !active ) active=(PrecomputedCubeMappedSphereProfile *) profiles.first;
 }
 void WheelDown() {
  resolution--;
  active=(PrecomputedCubeMappedSphereProfile *) active->prev;
  if ( !active ) active=(PrecomputedCubeMappedSphereProfile *) profiles.last;
 }
 void LeftUp() {
  delete rock;
  seed++;
  rock=new Rock3d(
   active,
   seed,
   seed%2==1,
   0.25f
  );
  rock->VertexBufferAndImmediate();
  scale.x=0.25+rock->set->unique(seed)*2.0;
  scale.y=0.25+rock->set->unique(seed+1)*2.0;
  scale.z=0.25+rock->set->unique(seed+2)*2.0;
 }
 void RightUp() {
  delete rock;
  seed--;
  rock=new Rock3d(active,seed,seed%2==1,0.25f);//(seed,1.0f,1.0f,1.0f,16);
  rock->VertexBufferAndImmediate();
  scale.x=0.25+rock->set->unique(seed)*2.0;
  scale.y=0.25+rock->set->unique(seed+1)*2.0;
  scale.z=0.25+rock->set->unique(seed+2)*2.0;
 }
 Cartesiand rotation,position,scale;
 void Between() {
  rotation.x+=0.05;
  rotation.y+=0.1;
 }
 void Render() {
  Crayon drawing;
  drawing.Pick(alabaster);
  glColor4fv(drawing.floats);
  Blending(none);
  char buf[128];
  FORMAT(buf,128,"Triangles: %d Resolution: %d Holes: %s",rock->model.polygons.count,active->resolution,rock->holes?"yes":"no");
  Text(buf,(double)x+20.0,(double)y+20.0,10.0,12.0,false);

  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();
   glPush();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   lap.Apply();
   glTranslated(0.0,0.0,-1.3);
   glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_NONE); 
   glRotatef((GLfloat) rotation.x, 0.0f, 1.0f, 0.0f);
   glRotatef((GLfloat) rotation.y, 1.0f, 0.0f, 0.0f);
   glScaled(scale.x,scale.y,scale.z);
   glColor4f(1.0f,1.0f,1.0f,1.0f);
   glEnable(GL_TEXTURE_2D);
   if ( seed%2==1 )
   EACH(sphere.model.polygons.first, mPolygon, p){
    i++;
    switch( face=p->group ) {
     case 1: glBindTexture(GL_TEXTURE_2D,F2->texture); break;
     case 2: glBindTexture(GL_TEXTURE_2D,L2->texture); break;
     case 3: glBindTexture(GL_TEXTURE_2D,B2->texture); break;
     case 4: glBindTexture(GL_TEXTURE_2D,R2->texture); break;
     case 5: glBindTexture(GL_TEXTURE_2D,D2->texture); break;
     case 6: glBindTexture(GL_TEXTURE_2D,U2->texture); break;
     default: OUTPUT("%d OUT OF RANGE.", face); break;
    }
    glBegin((GLenum) p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first, Vertex, v){
     j++;
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x, v->y, v->z);
    }
    glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,0);
   glPop();
   Blending(none);
   Rectangle(this,crayons.Pick(green),x,y,x2,y2);
 }
};

class OuterSpace : public GLWindow {
public:
 LookAtPerspective lap;
 ImmediateLighting lights;
 CubeMappedSphere3d sphere;
 Rock3d *rock;
 GLImage *texture;
 int seed;
 GLImage *F,*L,*B,*R,*U,*D;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  seed=0;
  rock=new Rock3d(seed,2.0f);//(seed,1.0f,1.0f,1.0f,16);
  rock->r=5.0f;
  rock->VertexBufferAndImmediate();
  sphere.r=5.0f;
  sphere.VertexBufferAndImmediate();
  texture=library.Load("data/images/planets/details/addcomb.png");
  U=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_top3.png");
  D=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_bottom4.png");
  L=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_left2.png");
  R=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_right1.png");
  F=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_front5.png");
  B=library.find("data/images/skyboxes/2048/greenbrown/greenbrown_back6.png");
 }
 void LeftDown() {
  delete rock;
  seed++;
  rock=new Rock3d(seed,2.0f);//(seed,1.0f,1.0f,1.0f,16);
  rock->VertexBufferAndImmediate();
 }
 Cartesiand rotation,position;
 void Between() {
  rotation.x+=0.5;
  rotation.y+=1.0;
 }
 void Render() {
  Crayon drawing;
  drawing.Pick(alabaster);
  glColor4fv(drawing.floats);
  Blending(none);
  char buf[128];
  FORMAT(buf,128,"Triangles: %d",rock->model.polygons.count);
  Text(buf,20.0,20.0,10.0,12.0,false);

   glPush();
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   int face=0;
   int i=0;
   int j=0;
   lap.Apply();
   glTranslated(0.0,0.0,-1.3);
   glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glPolygonMode(GL_FRONT, GL_FILL); glPolygonMode(GL_BACK, GL_NONE); 
   glRotatef((GLfloat) rotation.x, 0.0f, 1.0f, 0.0f);
   glRotatef((GLfloat) rotation.y, 1.0f, 0.0f, 0.0f);
   glColor4f(1.0f,1.0f,1.0f,1.0f);
   glEnable(GL_TEXTURE_2D);
   EACH(rock->model.polygons.first, mPolygon, p){
    i++;
    switch( face=p->group ) {
     case 1: glBindTexture(GL_TEXTURE_2D,F->texture); break;
     case 2: glBindTexture(GL_TEXTURE_2D,L->texture); break;
     case 3: glBindTexture(GL_TEXTURE_2D,B->texture); break;
     case 4: glBindTexture(GL_TEXTURE_2D,R->texture); break;
     case 5: glBindTexture(GL_TEXTURE_2D,D->texture); break;
     case 6: glBindTexture(GL_TEXTURE_2D,U->texture); break;
     default: OUTPUT("%d OUT OF RANGE.", face); break;
    }
    glBegin((GLenum) p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first, Vertex, v){
     j++;
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x, v->y, v->z);
    }
    glEnd();
   }
   EACH(sphere.model.polygons.first, mPolygon, p){
    i++;
    switch( face=p->group ) {
     case 1: glBindTexture(GL_TEXTURE_2D,F->texture); break;
     case 2: glBindTexture(GL_TEXTURE_2D,L->texture); break;
     case 3: glBindTexture(GL_TEXTURE_2D,B->texture); break;
     case 4: glBindTexture(GL_TEXTURE_2D,R->texture); break;
     case 5: glBindTexture(GL_TEXTURE_2D,D->texture); break;
     case 6: glBindTexture(GL_TEXTURE_2D,U->texture); break;
     default: OUTPUT("%d OUT OF RANGE.", face); break;
    }
    glBegin((GLenum) p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first, Vertex, v){
     j++;
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x, v->y, v->z);
    }
    glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,0);
   glPop();
 }
};
*/