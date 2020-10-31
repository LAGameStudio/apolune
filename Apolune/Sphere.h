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
#include "OldVBO.h"
#include "Chemistry.h"
#include "Primitive3d.h"

#define SINCOS_PRECISION 0.5
#define SINCOS_LENGTH  ( (int) (360.0 / SINCOS_PRECISION) )
#define DEG_TO_RAD     ( PI / 180 )

extern Indexed<Zfloat> sinLUT;
extern Indexed<Zfloat> cosLUT;

void initsincosLUT();

enum SphereTextureOptions {
 PositionalSphereMapping,
 SphericalMappingY,
 SphericalMappingZ,
 CubeMapNormal,
 NaiveSphereMapping,
 GnomonicProjection,
 OmnitectMapping,
 AasgaardProjection,
 CylinderMapping
};

class Geosphere3d : public Primitive3d {
public:
 int res;
 float r,r2;
 SphereTextureOptions textureCoordOptions;

 Geosphere3d() : Primitive3d() {
  res=64;
  r=0.5f;
  r2=r/2.0f;
  textureCoordOptions=SphericalMappingY;
 }

 Vertex n;
 void texCoord( Vertex *v ) {
  switch ( textureCoordOptions ) {
  case PositionalSphereMapping:
    n.Set(v); n.normalize();
    v->q=asinf( n.x ) / PIf + 0.5f;
    v->r=asinf( n.y ) / PIf + 0.5f;
   break;
  case SphericalMappingY:
    v->q=(asinf( (float) (v->x/v->z) ) / TWO_PIf) + 0.5f;
    v->r=(asinf( (float) v->y ) / PIf) + 0.5f;
   break;
  case SphericalMappingZ:
    v->q=(asinf( (float) (v->x/v->y) ) / TWO_PIf) + 0.5f;
    v->r=(asinf( (float) v->z) / PIf) + 0.5f;
   break;
  case CubeMapNormal:
    n.Set(v); n.normalize();
    v->q=n.x;
    v->r=n.y;
   break;
  case NaiveSphereMapping:
    v->r = acosf(v->z/r) / PIf;
   if (v->y >= 0)
    v->q = acosf(v->x/(r * sinf(PIf*(v->r)))) / TWO_PIf;
   else
    v->q = (PIf + acosf(v->x/(r * sinf(PIf*(v->r))))) / TWO_PIf;
   break;
  case GnomonicProjection:
   break;
  case OmnitectMapping:
   break;
  }
 }

 void VertexBufferAndImmediate() {
  initsincosLUT();
  mPolygon *p, *first;
  Vertex *w;
  model.groups++;
  int group=(model.groups);
  int vertCount;
  int i,j;
  Indexed<Zfloat> sphereX, sphereY, sphereZ;
  Indexed<Zfloat> cx, cz, tu, tv;
  float delta;
  float angle_step = (SINCOS_LENGTH*0.5f)/res;
  float angle = angle_step;
  int v1,v11,v2,voff,currVert;
  delta = (float)SINCOS_LENGTH/res;
  cx.Size(res);
  cz.Size(res);
  tu.Size(res);
  tv.Size(res);
  
  // Tex coordinate calculations
  float iu=(float) (1.0/(double) res);
  float iv=(float) (1.0/(double) res);

  for (i=0; i<res; i++ ) {
   tu[i]=i*iu;
   tv[i]=i*iv;
  }
  
  // Calc unit circle in XZ plane
  for (i = 0; i < res; i++) {
    cx[i] = -cosLUT[(int) (i*delta) % SINCOS_LENGTH];
    cz[i] =  sinLUT[(int) (i*delta) % SINCOS_LENGTH];
  }
  
  // Computing vertexlist vertexlist starts at south pole
  vertCount = res * (res-1) + 2;
  currVert = 0;
  
  // Re-init arrays to store vertices
  sphereX.Size(vertCount);
  sphereY.Size(vertCount);
  sphereZ.Size(vertCount);
  
  // Step along Y axis
  for (i = 1; i < res; i++) {
    float curradius =  sinLUT[(int) angle % SINCOS_LENGTH];
    float currY     = -cosLUT[(int) angle % SINCOS_LENGTH];
    for (j = 0; j < res; j++) {
      sphereX[currVert] = cx[j] * curradius;
      sphereY[currVert] = currY;
      sphereZ[currVert++] = cz[j] * curradius;
    }
    angle += angle_step;
  }

  // Southern cap
  //r = (float) ( (r + 240 ) * 0.33f );
  first= p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method = GL_TRIANGLE_STRIP;
  for (i = 0; i < res; i++) {
   w=new Vertex; w->x= sphereX[i]*r; w->y= sphereY[i]*r; w->z= sphereZ[i]*r; texCoord(w); p->addVertex(w);
   w=new Vertex; w->x=  0;           w->y= -r;           w->z= 0;            texCoord(w); p->addVertex(w);
  }
  w=new Vertex; w->x=  0;           w->y= -r;           w->z= 0;             texCoord(w); p->addVertex(w);
  w=new Vertex; w->x= sphereX[0]*r; w->y= sphereY[0]*r; w->z= sphereZ[0]*r;  texCoord(w); p->addVertex(w);
  
  // Middle rings
  voff = 0;
  for(i = 2; i < res; i++) {
    v1=v11=voff;
    voff += res;
    v2=voff;
    p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
    p->draw_method = GL_TRIANGLE_STRIP;
    for (j = 0; j < res; j++) {
     w=new Vertex; w->x= sphereX[v1]*r; w->y= sphereY[v1]*r; w->z= sphereZ[v1++]*r; texCoord(w); p->addVertex(w);
     w=new Vertex; w->x= sphereX[v2]*r; w->y= sphereY[v2]*r; w->z= sphereZ[v2++]*r; texCoord(w); p->addVertex(w);
    }
  
    // Close each ring
    v1=v11;
    v2=voff;
    w=new Vertex; w->x= sphereX[v1]*r; w->y= sphereY[v1]*r; w->z= sphereZ[v1]*r; texCoord(w); p->addVertex(w);
    w=new Vertex; w->x= sphereX[v2]*r; w->y= sphereY[v2]*r; w->z= sphereZ[v2]*r; texCoord(w); p->addVertex(w);
  }
//  u=0;
  
  // Add the northern cap
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method = GL_TRIANGLE_STRIP;
  for (i = 0; i < res; i++) {
    v2 = voff + i;
     w=new Vertex; w->x= sphereX[v2]*r; w->y= sphereY[v2]*r; w->z= sphereZ[v2]*r; texCoord(w); p->addVertex(w);
     w=new Vertex; w->x= 0;             w->y= r;             w->z= 0;             texCoord(w); p->addVertex(w);
  }
  w=new Vertex; w->x= sphereX[voff]*r; w->y= sphereY[voff]*r; w->z= sphereZ[voff]*r; texCoord(w); p->addVertex(w);

  toVBO16();
 }

 void toVBO16() {
  Vertex t;
  Vertex *w;

  // Convert to VBO
  int j=0;
  mPolygon *p;
  for ( p=(mPolygon *) model.polygons.first; p; p=(mPolygon *) p->next ) { j++;
   Vertex *a,*b,*c,*d;
   Vertex *a1, *c1;
   int i=0;
   a=b=c=d=null;
   a1=(Vertex *) (p->points.first);
   c1=(Vertex *) (a1->next->next);
   for ( w=(Vertex *) p->points.first; w; w=(Vertex *) w->next ) {
    if ( a ) {
     if ( b ) {
      if ( c ) {} else {
       c=w;
       if ( d ) {
        vbo16.PushTriangle(a,c,d);
        model.triangles.Add(i,i+1,i+2);
       }
       vbo16.PushTriangle(a,b,c);
       model.triangles.Add(i,i+1,i+2);
       d=c;
       a=b;
       b=c;
       c=null;
       i+=3;
      }
     } else {
      b=w;
     }
    } else {
     a=w;
    }
   }
   vbo16.PushTriangle(a1,c1,d); model.triangles.Add(i,i+1,i+2);
   t.Set(a1); texCoord(&t); vbo16.vertices[i].texCoord[0]=t.q;   vbo16.vertices[i].texCoord[1]=t.r;    
   t.Set(c1); texCoord(&t); vbo16.vertices[i+1].texCoord[0]=t.q; vbo16.vertices[i+1].texCoord[1]=t.r;    
   t.Set(d);  texCoord(&t); vbo16.vertices[i+2].texCoord[0]=t.q; vbo16.vertices[i+2].texCoord[1]=t.r;    
  }
 }
};

extern Geosphere3d geosphere3d;