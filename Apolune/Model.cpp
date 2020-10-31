
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"

#include "Art.h"
#include "Crayon.h"
#include "Material.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"

ImmediateModel::ImmediateModel( ImmediateModel *a ) {
 Duplicate(a);
}

ImmediateModel::ImmediateModel(void)
{
}

ImmediateModel::~ImmediateModel(void)
{
}

//extern char* layer_modes   [MAX_MODES];
//extern char* pattern_modes [MAX_PATTERN];
//extern char* random_modes  [MAX_RANDOM];
//extern char* blend_modes   [MAX_BLEND];
//extern char* kernel_modes  [G_MAX_KERNEL];
//extern char* stamp_modes   [STAMP_MAX];

/*
#define G_SMOOTHING   0
#define G_SHARPENING  1
#define G_EMBOSS      2
#define G_FIND_EDGES  3
#define G_MOTION_BLUR 4
#define G_MAX_KERNEL  5
 */

// Idea: global pre-execution function BugBuster; list of functions, ability to select visually to report
// #2: add progress bar to layer view, thread execute_procedure()

//class ImmediateModelStorage : WebPage { ImmediateModel t; };

// Calculates a cost figure for the ImmediateModel based on material selections
float ImmediateModel::cost(void) {
 return 1.0f;/*
 float total,w,h;
 EACH(polygons.first,mPolygon,p) {
  p->boundingBox(p,&w,&h); 
  if ( p->material ) total+=p->material->Value() * (w*h); 
  else total+=100.0f; 
 }
 return total;*/
}

void ImmediateModel::Duplicate( ImmediateModel *m ) {
 EACH(m->polygons.first,mPolygon,p) polygons.Append(p->Copy()); 
 name = m->name;
//  n->id=id; 
 description = m->description;
 volume = m->volume;
 weight = m->weight;
 density = m->density;
 springyness = m->springyness;
 groups = m->groups;
 boxMagnitude.Set(&m->boxMagnitude);
 leasts.Set(&m->leasts);
 greatests.Set(&m->greatests);
// n->thumbnail = Bitmap {};
// n->thumbnail->Copy( thumbnail );//
 if ( m->bary ) bary->Duplicate(m->bary);// = bary->Copy();
 if ( m->axis ) axis->Duplicate(m->axis);// = axis->Copy();
}

void ImmediateModel::Duplicate( ImmediateModel *m, int mangle, float dx, float dy ) {
 EACH(m->polygons.first,mPolygon,p) polygons.Append(p->Copy(mangle,dx,dy)); 
 name = m->name;
//  n->id=id; 
 description = m->description;
 volume = m->volume;
 weight = m->weight;
 density = m->density;
 springyness = m->springyness;
 groups = m->groups;
// n->thumbnail = Bitmap {};
// n->thumbnail->Copy( thumbnail );//
 if ( m->bary ) bary->Duplicate(m->bary);// = bary->Copy();
 if ( m->axis ) axis->Duplicate(m->axis);// = axis->Copy();
}

ImmediateModel *ImmediateModel::Copy(void) {
 ImmediateModel *n=new ImmediateModel;
 EACH(polygons.first,mPolygon,p) n->polygons.Append(p->Copy()); 
 n->name = name;
//  n->id=id; 
 n->description = description;
 n->volume = volume;
 n->weight = weight;
 n->density = density;
 n->springyness = springyness;
 n->groups = groups;
// n->thumbnail = Bitmap {};
// n->thumbnail->Copy( thumbnail );//
 if ( bary ) n->bary->Duplicate(bary);// = bary->Copy();
 if ( axis ) n->axis->Duplicate(axis);// = axis->Copy();
 return n;
}

ImmediateModel *ImmediateModel::CopyGroup( int g ) {
 ImmediateModel *n=new ImmediateModel;
 EACH(polygons.first,mPolygon,p) if ( p->group == g ) n->polygons.Append(p->Copy()); 
 n->name = name;
// n->id=id;
 n->description = description;
 n->volume = volume;
 n->weight = weight;
 n->density = density;
 n->springyness = springyness;
 n->groups = groups;
// n->thumbnail = Bitmap {};
// n->thumbnail->Copy( thumbnail );//
 if ( bary ) n->bary->Duplicate(bary.pointer);
 if ( axis ) n->axis->Duplicate(axis.pointer);
 return n;
}

PolygonList *ImmediateModel::findGroup( int g ) {
 PolygonList *list=new PolygonList;
 EACH(polygons.first,mPolygon,p) if ( p->group == g ) list->Push(p); 
 return list;
}

void ImmediateModel::addPolygon( mPolygon *p ) { 
 polygons.Append(p); 
}

mPolygon *ImmediateModel::find_polygon_by_vertex( Vertex *v ) {
 if ( !v ) return null;
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,vv) if ( vv==v ) return p;
 return null;
}

// Find the "least" point on a certain view plane
Vertex *ImmediateModel::leastPoint(void) {
 mPolygon *p=(mPolygon *) polygons.first;
 Vertex *v;
 Vertex *r=new Vertex; // result;
 if ( p && p->points.first ) {
  v=(Vertex*) p->points.first;
  r->x=v->x;
  r->y=v->y;
  r->z=v->z;
 }
 for ( ; p; p=(mPolygon *)p->next ) //if ( polygons->p && polygons->p->points )
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) (v->next) ) {
   if ( v->x < r->x ) r->x=v->x;
   if ( v->y < r->y ) r->y=v->y;
   if ( v->z < r->z ) r->z=v->z;
  }
 return r;
}

// Find the "greatest" point on a certain view plane
Vertex *ImmediateModel::greatestPoint() {
 mPolygon *p=(mPolygon *) polygons.first;
 Vertex *v;
 Vertex *r=new Vertex; // result;
 if ( p && p->points.first ) { 
  v=(Vertex *)p->points.first; 
  r->x=v->x; 
  r->y=v->y; 
  r->z=v->z;
 }
 for ( ; p; p=(mPolygon *) p->next ) //if ( polygons->p && polygons->p->points )
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) (v->next) ) {
   if ( v->x > r->x ) r->x=v->x; 
   if ( v->y > r->y ) r->y=v->y;
   if ( v->z > r->z ) r->z=v->z;
  }
 return r;
}

void ImmediateModel::moveBy( float x, float y, float z ) {
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x+=x;
  v->y+=y;
  v->z+=z;
 }
}

void ImmediateModel::MoveBy( Vertexd *d ) {
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x+=(float)d->x;
  v->y+=(float)d->y;
  v->z+=(float)d->z;
 }
}

void ImmediateModel::Rotate( float x, float y, float z ) {
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) v->Rotate(x,y,z);
}

void ImmediateModel::Rotate( Vertexd *rotation ) {
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) v->Rotate((float)rotation->x,(float)rotation->y,(float)rotation->z);
}

void ImmediateModel::CopyScaleRotateMove( ImmediateModel *in, Vertexd *scale, Vertexd *rotate, Vertexd *move ) {
 if ( !in ) {
  OUTPUT("ImmediateModel:CopyScaleRotateMove() model to copy was null\n");
  return;
 }
 EACH(in->polygons.first,mPolygon,p) {
  mPolygon *q=new mPolygon;
  EACH(p->points.first,Vertex,v) {
   Vertex *w=v->Copy();
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  if ( p->normal ) q->normal=p->normal->Copy();
  else q->TriNormal();
  polygons.Append(q);
 }
}

#include "STLFile.h"

void ImmediateModel::CopyScaleRotateMove( STLFile *in, Vertexd *scale, Vertexd *rotate, Vertexd *move ) {
 if ( !in ) {
  OUTPUT("ImmediateModel:CopyScaleRotateMove() model to copy was null\n");
  return;
 }
 EACH(in->facets.first,STLFacet,p) {
  mPolygon *q=new mPolygon;
  {
   Vertex *w=new Vertex(p->a.x,p->a.y,p->a.z);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  {
   Vertex *w=new Vertex(p->b.x,p->b.y,p->b.z);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  {
   Vertex *w=new Vertex(p->c.x,p->c.y,p->c.z);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  q->normal=new Vertex(p->trinormal.x,p->trinormal.y,p->trinormal.z);
  polygons.Append(q);
 }
}

void ImmediateModel::CopyAsUnitScaleRotateMove( STLFile *in, Vertexd *scale, Vertexd *rotate, Vertexd *move ) {
 if ( !in ) {
  OUTPUT("ImmediateModel:CopyScaleRotateMove() model to copy was null\n");
  return;
 }
 in->facets.CalculateExtents();
 EACH(in->facets.first,STLFacet,p) {
  mPolygon *q=new mPolygon;
  {
   Vertex *w=new Vertex(p->a.x,p->a.y,p->a.z);
   w->Divide(&in->facets.extents);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  {
   Vertex *w=new Vertex(p->b.x,p->b.y,p->b.z);
   w->Divide(&in->facets.extents);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  {
   Vertex *w=new Vertex(p->c.x,p->c.y,p->c.z);
   w->Divide(&in->facets.extents);
   w->Scale((float) scale->x,(float) scale->y, (float) scale->z);
   w->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
   w->MoveBy((float)move->x,(float)move->y,(float)move->z);
   q->points.Append(w);
  }
  q->normal=new Vertex(p->trinormal.x,p->trinormal.y,p->trinormal.z);
  polygons.Append(q);
 }
}

void ImmediateModel::limitTo( float x, float y, float z ) {
 least=leastPoint();
 moveBy(-least->x,-least->y,-least->z);
 greatest=greatestPoint();
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  if ( greatest->x > 0 ) v->x=v->x/greatest->x*x;
  if ( greatest->y > 0 ) v->y=v->y/greatest->y*y;
  if ( greatest->z > 0 ) v->z=v->z/greatest->z*z;
 }
}

inline void ImmediateModel::normalizeTo1() { limitTo( 1.0f, 1.0f, 1.0f ); }

// The averaged center of N points over P polygons
Vertex *ImmediateModel::barycenter(void) {
 Vertex *r=new Vertex; // result
 int rolling_total=0;
 
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  r->x+= v->x;
  r->y+= v->y;
  r->z+= v->z;
  rolling_total++;
 }
 r->x/=rolling_total;
 r->y/=rolling_total;
 r->z/=rolling_total;
 return r;
}

void ImmediateModel::align_to_barycenter( bool recalcbounds, bool calcbounds ) {
 if ( calcbounds ) BoxBounds();
 Vertex *bc=barycenter();
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x-=bc->x;
  v->y-=bc->y;
  v->z-=bc->z;
 }
 delete bc;  
 if ( recalcbounds ) BoxBounds();
}

void ImmediateModel::align_to_barycenterXY( bool recalcbounds, bool calcbounds ) {
 if ( calcbounds ) BoxBounds();
 Vertex *bc=barycenter();
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x-=bc->x;
  v->y-=bc->y;
 }
 delete bc;  
 if ( recalcbounds ) BoxBounds();
}

void ImmediateModel::align_to_origin( bool recalcbounds, bool calcbounds ) {
 if ( calcbounds ) BoxBounds();
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x-=leasts.x;
  v->y-=leasts.y;
  v->z-=leasts.z;
 }
 if ( recalcbounds ) BoxBounds();
}

void ImmediateModel::align_to_originXY( bool recalcbounds, bool calcbounds ) {
 if ( calcbounds ) BoxBounds();
 EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
  v->x-=leasts.x;
  v->y-=leasts.y;
 }
 if ( recalcbounds ) BoxBounds();
}

// Draw the polygon outline for all polygons, find nearest vertex to highlight when alt is pressed
void ImmediateModel::RenderTopDown( GLWindow *win, int x, int y, int w, int h, Crayon colorLine, bool vertexes ) {
 Vertex *v_last,*v_first;
 int h2=y+h/10,w2=x+w/10;
 int vfirstx,vfirstz;
 int vlastx,vlastz;
 Vertex *least,*greatest; 
 float zoomX,zoomZ,ofsX=0.0f,ofsZ=0.0f;
 Crayon pen;

 least=leastPoint();
 greatest=greatestPoint();
 zoomX=abs(least->x-greatest->x)/(w-w/5);
 zoomZ=abs(least->z-greatest->z)/(h-h/5); 
 if ( least->x < 0 ) ofsX=abs(least->x);
 if ( least->z < 0 ) ofsZ=abs(least->z);
 delete least;
 delete greatest;
 
 EACH(polygons.first,mPolygon,p) {
  v_last=null;
  if ( (v_first=(Vertex *)p->points.first) ) {
   vfirstx=(int) (w2+(ofsX+v_first->x)/zoomX);
   vfirstz=(int) (h2+(ofsZ+v_first->z)/zoomZ);
  }
  EACH(p->points.first,Vertex,v) {
   int vx=(int) (w2+(v->x+ofsX)/zoomX);
   int vz=(int) (h2+(v->z+ofsZ)/zoomZ);
   if ( v_last == null && vertexes ) {
    pen.fromFloat(v->rgba);
    Area( win, pen, vx-1,vz-1, vx+1,vz+1 );//3,3 );
   } else {
    DrawLine(win,colorLine,vx,vz,vlastx,vlastz);
    if ( v_first != v && v->next == null ) DrawLine(win,colorLine,vx,vz,vfirstx,vfirstz);
    if ( vertexes ) {
     pen.fromFloat(v->rgba);
     Area( win, pen, vx-1,vz-1, vx+1,vz+1 );//3,3 );
    }
   }
   v_last=v;
   vlastx=vx;
   vlastz=vz;
  }
 } // end for          
}

void ImmediateModel::RenderSide( GLWindow *win, int x, int y, int w, int h, Crayon colorLine, bool vertexes ) {
 Vertex *v_last,*v_first;
 int h2=y+h/10,w2=x+w/10;
 int vfirsty,vfirstz;
 int vlasty=0,vlastz=0;
 Vertex *least,*greatest; 
 float zoomZ,zoomY, ofsZ,ofsY;
 Crayon pen;

 least=leastPoint();
 greatest=greatestPoint();
 zoomZ=abs(least->z-greatest->z)/(w-w/5);
 zoomY=abs(least->y-greatest->y)/(h-h/5); 
 if ( least->z < 0 ) ofsZ=abs(least->z);
 if ( least->y < 0 ) ofsY=abs(least->y);
 delete least,greatest;
 EACH(polygons.first,mPolygon,p) {
  v_last=null;
  if ( (v_first=(Vertex *) p->points.first) ) {
   vfirsty=(int) (h2+(ofsY+v_first->y)/zoomY);
   vfirstz=(int) (w2+(ofsZ+v_first->z)/zoomZ); // h2 became w2
  } 
  EACH(p->points.first,Vertex,v) {
   int vy=(int) (h2+(ofsY+v->y)/zoomY); 
   int vz=(int) (w2+(ofsZ+v->z)/zoomZ); // h2 became w2
   if ( v_last == null && vertexes ) {
    pen.fromFloat(v->rgba);
    Area( win,pen, vz-1,vy-1, vz+1,vy+1 );//3,3 );
   } else {
    DrawLine(win,colorLine,vz,vy,vlastz,vlasty);
    if ( v_first != v && v->next == null ) DrawLine(win,colorLine,vz,vy,vfirstz,vfirsty);
    if ( vertexes ) {
     pen.fromFloat(v->rgba);
     Area( win, pen, vz-1,vy-1, vz+1,vy+1 );//3,3 );
    }
   }
   v_last=v; 
   vlasty=vy; 
   vlastz=vz;
  }
 } // end for   
}

#include <glm/glm.hpp>

void ImmediateModel::LoadOBJ( const char *filename ) {
	Indexed<unsigned int> vertexIndices, uvIndices, normalIndices;
	Indexed<glm::vec3> temp_vertices;
	Indexed<glm::vec2> temp_uvs;
	Indexed<glm::vec3> temp_normals;
	OUTPUT("Loading OBJ file %s...\n", filename);
 Strings file;
 file.Load(filename);
 Zint line;
 EACH(file.first,String,s) {
  line++;
  const char *p=s->s.c_str();
  if ( *p == '#' ) continue;
  if ( *p == 'm' ) {
  } else
  if ( *p == 'u' ) {
  } else
  if ( *p == 's' ) {
  } else
  if ( *p == 'f' ) {
   Strings words;
   words.SplitPush(p,' ');
   if ( words.count == 4) {
    String *word=(String *)words.first;
    String *one=(String *)word->next;
    String *two=(String *)one->next;
    String *three=(String *)two->next;
    Strings set1,set2,set3;
    set1.SplitPush(one->s.c_str(),'/');
    set2.SplitPush(two->s.c_str(),'/');
    set3.SplitPush(three->s.c_str(),'/');

    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    vertexIndex[0]=vertexIndex[1]=vertexIndex[2]=uvIndex[0]=uvIndex[1]=uvIndex[2]=normalIndex[0]=normalIndex[1]=normalIndex[2]=0;

    one=(String *) set1.first;
    two=(String *) one->next;
    three=(String *) two->next;

    vertexIndex[0]=atoi(one->s.c_str());
    uvIndex[0]    =atoi(two->s.c_str());
    normalIndex[0]=atoi(three->s.c_str());

    one=(String *) set2.first;
    two=(String *) one->next;
    three=(String *) two->next;

    vertexIndex[1]=atoi(one->s.c_str());
    uvIndex[1]    =atoi(two->s.c_str());
    normalIndex[1]=atoi(three->s.c_str());

    one=(String *) set3.first;
    two=(String *) one->next;
    three=(String *) two->next;

    vertexIndex[2]=atoi(one->s.c_str());
    uvIndex[2]    =atoi(two->s.c_str());
    normalIndex[2]=atoi(three->s.c_str());

    unsigned int *a;
 			a=vertexIndices.Increase(); *a=(vertexIndex[0]);
 			a=vertexIndices.Increase(); *a=(vertexIndex[1]);
 			a=vertexIndices.Increase(); *a=(vertexIndex[2]);
 			a=uvIndices    .Increase(); *a=(uvIndex[0]);
 			a=uvIndices    .Increase(); *a=(uvIndex[1]);
 			a=uvIndices    .Increase(); *a=(uvIndex[2]);
 			a=normalIndices.Increase(); *a=(normalIndex[0]);
 			a=normalIndices.Increase(); *a=(normalIndex[1]);
 			a=normalIndices.Increase(); *a=(normalIndex[2]);
   }
  } else
  if ( *p == 'v' ) {
   Strings words;
   words.SplitPush(p,' ');
   String *word=(String *) words.first;
   if ( words.count == 4 && !str_cmp(word->c_str(),"v") ) {
    String *one=(String *)word->next;
    String *two=(String *)one->next;
    String *three=(String *)two->next;
 			glm::vec3 *v=temp_vertices.Increase();
    v->x=(float)atof(one->s.c_str());
    v->y=(float)atof(two->s.c_str());
    v->z=(float)atof(three->s.c_str());
   } else if ( words.count == 3 && !str_cmp(word->c_str(),"vt") ) {
    String *one=(String *)word->next;
    String *two=(String *)one->next;
    glm::vec2 *tc=temp_uvs.Increase();
    tc->x=(float)atof(one->s.c_str());
    tc->y=(float)atof(two->s.c_str());
   } else if ( words.count == 4 && !str_cmp(word->c_str(),"vn") ) {
    String *one=(String *)word->next;
    String *two=(String *)one->next;
    String *three=(String *)two->next;
    glm::vec3 *normal=temp_normals.Increase();
    normal->x=(float)atof(one->s.c_str());
    normal->y=(float)atof(two->s.c_str());
    normal->z=(float)atof(three->s.c_str());
   }
//    else OUTPUT("Error parsing file on line %d:\n> %s\n", line, p);
  }
 }
 OUTPUT("%d vertices, %d normals, %d texcoords loaded for %d (%d/%d) indices\n",
  (int) temp_vertices.length,
  (int) temp_uvs.length,
  (int) temp_normals.length,
  (int) vertexIndices.length,
  (int) uvIndices.length,
  (int) normalIndices.length
 ); 

 // Since OBJs will end up as VBOs, we make a design choice here to implement a single "N-gon",
 // though this could be segmented into many 3-point Polygons of draw_method GL_TRIANGLE.
 polygons.Clear();
 mPolygon *p=new mPolygon;
 polygons.Append(p);

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.length; i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Put the attributes into the ImmediateModel.
  Vertex *v=new Vertex;
		// Get the attributes thanks to the index
		if ( vertexIndex > 0 ) {
   glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
   v->x=vertex.x;
   v->y=vertex.y;
   v->z=vertex.z;
  }
		if ( uvIndex > 0 ) {
   glm::vec2 uv = temp_uvs[ uvIndex-1 ];
   v->q=uv.x;
   v->r=uv.y;
  }
		if ( normalIndex > 0 ) {
   glm::vec3 normal = temp_normals[ normalIndex-1 ];
   v->normal.Instantiate();
   v->normal->x=normal.x;
   v->normal->y=normal.y;
   v->normal->z=normal.z;
  }		
  p->points.Append(v);
	}
}


 void ImmediateModel::Render() {
  if ( !vboVNT ) {
   vboVNT.Instantiate();
   vboVNT->fromModel(this);
   vboVNT->Create();
  }
  vboVNT->Render();
  /*
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    //glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     //glColor4fv(v->rgba); // removing per-vertex color
     glNormal3f(v->normal->x,v->normal->y,v->normal->z);
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x,v->y,v->z);
    }
   glEnd();
  }
  */
 }

 void ImmediateModel::RenderPolyNormal() {
  if ( !vboVNT ) {
   vboVNT.Instantiate();
   vboVNT->fromModel(this);
   vboVNT->Create();
  }
  vboVNT->Render();
  //EACH(polygons.first,mPolygon,p) {
  // glBegin(p->draw_method);
  //  //glNormal3f(p->normal->x,p->normal->y,p->normal->z);
  //  EACH(p->points.first,Vertex,v) {
  //   //glColor4fv(v->rgba); // removing per-vertex color
  //   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
  //   glTexCoord2f(v->q,v->r);
  //   glVertex3f(v->x,v->y,v->z);
  //  }
  // glEnd();
  //}
 }