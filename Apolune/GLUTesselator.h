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
#include "GLSetup.h"
#include "Vertex.h"
#include "Polygon.h"

/* Implements GLU Tesselation Object
 * GLU 1.3 specification: http://opengl3.org/documentation/specs/glu/glu1_3.pdf
 * Only implements Tesselation object, no quadric support (yet) or NURBS (yet)
 */

/*
 * Consider: http://www.cs.cmu.edu/~quake/triangle.delaunay.html
 */



  // Use gluTess to tesselate
  //FreetypeContour *initial = (FreetypeContour *) outer.first;
  //GLUTesselator gluTess;
  //if ( initial->outline.flags & FT_OUTLINE_EVEN_ODD_FILL ) {
  // gluTess.SetWinding(glu_tess_odd);
  //} else {
  // gluTess.SetWinding(glu_tess_nonzero);
  //}
  //VerticesHandles outers,inners;
  //// Execute outer contour
  //EACH(outer.first,FreetypeContour,c) {
  // outers.Add(&c->points);
  //}
  //gluTess.Tesselate(&outers,&inners,&result1);

typedef GLvoid (CALLBACK *GLUTesselatorFunction) ();

enum GLUTesselatorWinding {
 glu_tess_odd=GLU_TESS_WINDING_ODD,
 glu_tess_nonzero=GLU_TESS_WINDING_NONZERO,
 glu_tess_positive=GLU_TESS_WINDING_POSITIVE,
 glu_tess_negative=GLU_TESS_WINDING_NEGATIVE,
 glu_tess_abs_geq_2=GLU_TESS_WINDING_ABS_GEQ_TWO
};

enum GLUTesselatorContourType {
 glu_tess_cw=GLU_CW,
 glu_tess_ccw=GLU_CCW,
 glu_tess_exterior=GLU_EXTERIOR,
 glu_tess_interior=GLU_INTERIOR,
 glu_tess_unknown=GLU_UNKNOWN
};

const double Freetype_FRONT_FACING = 1.0;
const double Freetype_BACK_FACING = -1.0;

class GLUTesselator {
public:
 GLUTesselatorWinding winding;
 GLenum error;
 Zstring message;
 Vertexd normal;
 GLenum type;
 Zint size;
 Zpointer<GLUtesselator> tess;
 Indexed<double> doubles,doubles2;
 Zbool edge,boundary_only;
 Zdouble toleranceHint;
 Zbool initialized;
 GLUTesselatorFunction TESS_BEGIN_DATA;
 GLUTesselatorFunction TESS_EDGE_FLAG_DATA;
 GLUTesselatorFunction TESS_VERTEX_DATA;
 GLUTesselatorFunction TESS_COMBINE_DATA;
 GLUTesselatorFunction TESS_ERROR_DATA;
 GLUTesselatorFunction TESS_END_DATA;
 Zpointer<VerticesHandles> inners,outers;
 Zpointer<Vertices> out;
 Zpointer<Vertex> last;
 GLenum firstContour;
 GLUTesselator() {
  firstContour=GLU_EXTERIOR | GL_CW;
  type=GL_TRIANGLE_STRIP;
  error=GL_NO_ERROR;
  winding=glu_tess_nonzero;
  normal.Set(0.0,0.0,1.0);
  InitGLUTesselatorFunctions();
 }
 void InitGLUTesselatorFunctions();
 // Tesselates a list of contours assembled into a single Vertices (by default)
 void Tesselate( VerticesHandles *outers, VerticesHandles *inners, Vertices *out ) {
  this->outers=outers;
  this->inners=inners;
  doubles.Size( (size_t) (inners->count*3) );
  doubles2.Size( (size_t) (inners->count*3) );
  this->out=out;
  Init();
  if ( tess ) {
   Loop();
   Deinit();
  }
 }
/* Redbook Chapter 11: "Calls to gluTessBeginPolygon() and gluTessEndPolygon() surround
the definition of one or more contours. When gluTessEndPolygon() is called, the
tessellation algorithm is implemented, and the tessellated polygons are generated
and rendered. */
 void BeginPolygon() { gluTessBeginPolygon(tess,this); }
 void EndPolygon() { gluTessEndPolygon(tess); }
/* Redbook Chapter 11: "Begins and ends the specification of a closed contour, which is
a portion of a polygon. A closed contour consists of zero or more calls to gluTessVertex(),
which defines the vertices. The last vertex of each contour is automatically linked to the
first. In practice, a minimum of three vertices is needed for a meaningful contour." */
 void BeginContour() { gluTessBeginContour(tess); }
 void EndContour() { gluTessEndContour(tess); }
 void TessVertex( Vertex *v, unsigned int startIndex ) {
  GLdouble c[3];  c[0]=v->x;  c[1]=v->y;  c[2]=v->z;
  gluTessVertex(tess,c,&doubles2.list[startIndex]);
 }
 virtual void Loop() {
  BeginPolygon();
   unsigned int i=0;
   EACH(outers->first,VerticesHandle,vh) {
    Vertices *points=vh->p;
    if ( points ) {
     if ( points->count > 1 ) {
      Vertex *f=(Vertex *) points->first;
      if ( outers->first != vh ) gluNextContour(tess, GLU_EXTERIOR );
      BeginContour(); //
      EACH(points->first,Vertex,v) {
       TessVertex(v,i); i++;
      }
      EndContour(); ///
     }
    }
   }
   EACH(inners->first,VerticesHandle,vh) {
    Vertices *points=vh->p;
    if ( points ) {
     if ( points->count > 1 ) {
      Vertex *f=(Vertex *) points->first;
      gluNextContour(tess, GLU_INTERIOR );
      BeginContour(); //
      EACH(points->first,Vertex,v) {
       TessVertex(v,i); i++;
      }
      EndContour(); ///
     }
    }
   }
  EndPolygon();
 }
 void Init() {
  tess = gluNewTess();
  if ( tess ) {
   BindToFunctions();
   SetWinding(winding);
   SetToleranceHint(toleranceHint.value);
   SetBoundaryOnly(boundary_only.value);
   SetNormal(&normal);
  } else {
   OUTPUT("GLUTesselator: gluNewTess() returned NULL\n");
  }
 }
 void SetNormal( float x, float y, float z ) {
  normal.Set((double)x,(double)y,(double)z);
 }
 void SetNormal( double x, double y, double z ) {
  normal.Set(x,y,z);
  gluTessNormal( tess, normal.x, normal.y, normal.z );
 }
 void SetNormal( Vertexd *normal ) {
  this->normal.Set(normal);
  gluTessNormal( tess, normal->x, normal->y, normal->z );
 }
 void Deinit() {
  gluDeleteTess(tess);
 }
 void BindToFunctions() {
  gluTessCallback(tess, GLU_TESS_ERROR_DATA,     TESS_ERROR_DATA);
  gluTessCallback(tess, GLU_TESS_COMBINE_DATA,   TESS_COMBINE_DATA);
  gluTessCallback(tess, GLU_TESS_BEGIN_DATA,     TESS_BEGIN_DATA);
  gluTessCallback(tess, GLU_TESS_VERTEX_DATA,    TESS_VERTEX_DATA);
  gluTessCallback(tess, GLU_TESS_EDGE_FLAG_DATA, TESS_EDGE_FLAG_DATA);
  gluTessCallback(tess, GLU_TESS_END_DATA,       TESS_END_DATA);
 }
 void SetWinding(GLUTesselatorWinding winding_rule) {
  winding=winding_rule;
  if ( tess ) gluTessProperty(tess, GLU_TESS_WINDING_RULE, (GLenum) winding); 
 }
 void SetToleranceHint(double T) {
  toleranceHint=T;
  if ( tess ) gluTessProperty(tess,GLU_TESS_TOLERANCE,(GLdouble) T);
 }
 void SetBoundaryOnly(bool on) {
  boundary_only=on;
  if ( tess ) gluTessProperty(tess,GLU_TESS_BOUNDARY_ONLY,(GLdouble)(on?1.0:0.0));
 }
// void _Begin(GLenum t) { type=t; }
 virtual void _BeginData( GLenum t ) { type=t; }
 virtual void _EdgeFlagData( bool f ) { edge=f; }
 virtual void _VertexData( double *v ) { this->out->Add( v[0], v[1], v[2], edge ); }
 virtual void _CombineData( const double *v, GLdouble *d[4], const GLfloat *w, void **o ) {
  /* Example from http://www.flipcode.com/archives/Polygon_Tessellation_In_OpenGL.shtml
     was used partially here, but unfortunately that archive is corrupt!  Some of
     the code has been overwritten by some process which injected the word "indent"
     .. so, we only use part of it and filled in the gaps with 
     http://www.songho.ca/opengl/gl_tessellation.html 
   */
  GLdouble *p = (GLdouble *) malloc(6 * sizeof(GLdouble));
  // New vertex info
  p[0] = (GLdouble) v[0];
  p[1] = (GLdouble) v[1];
  p[2] = (GLdouble) v[2];
  p[3] = w[0] * d[0][3] + w[1] * d[1][3] + w[2] * d[2][3] + w[3] * d[3][3];
  p[4] = w[0] * d[0][4] + w[1] * d[1][4] + w[2] * d[2][4] + w[3] * d[3][4];
  p[5] = w[0] * d[0][5] + w[1] * d[1][5] + w[2] * d[2][5] + w[3] * d[3][5];
  *o = p;
  Vertex *n=out->Add( v[0], v[1], v[2] );
  n->data.pointer=(void *)p;
 }
 virtual void _EndData() {
  // Delete data from vertices since we are no longer needing it.
  EACH(out->first,Vertex,v) {
   if ( v->data ) {
    free((GLdouble *)(v->data.pointer));
    v->data=null;
   }
  }
 }
 virtual void _ErrorData(GLenum err) {
  if ((error = err) != GL_NO_ERROR) {
   const GLubyte *errString;
   errString = gluErrorString(error);
   OUTPUT("GLUTesselator Error: %s\n", (const char *)errString);
   message=string((const char *)errString);
  }
 }
};