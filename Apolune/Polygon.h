/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "Crayon.h"
#include "Vertex.h"

enum ViewModes {
 LEFT=0,
 TOP=1,
 RIGHT=2
};

enum PolygonFlags {
};

struct mPolyBox {
public:
 Zdisposable<Vertex> least,greatest;
};

// A "convex planar N-gon"
class mPolygon : public ListItem {
public:
 Zstring mat_hc;
 Zint id; // Links to material
 void *material,*data; // Evidence that C++ is doomed.   Not saved in binaries
 Vertices points;
 GLenum draw_method;  // TRIANGLE, QUAD, TRI_STRIP...; defaults to POLYGON
 Zfloat area;
 Zuint flags;
 Zint group; //-1=none
 Zdisposable<Vertex> bary;
 Zdisposable<Vertex> rotation;
 Zdisposable<Vertex> normal;
 Zdisposable<Vertex> centroid;
 Zdisposable<Vertex> tangent;
 Zdisposable<Vertex> bitangent;
 Zdisposable<mPolyBox> box;

 mPolygon(void) {
  data=material=null;
  draw_method=GL_POLYGON;
  group=-1;
 }
  
 ~mPolygon(void) {
 }

 void BinaryWrite( BinaryFile *file ) {
  file->write(mat_hc);
  file->write(&id);
  int _draw_method=(int)draw_method; file->write(&_draw_method);
  file->write(&area);
  file->write(&group);
  if ( bary )     { file->scrawl(1); bary->BinaryWrite(file); }
  else { file->scrawl(0); }
  if ( rotation ) { file->scrawl(1); rotation->BinaryWrite(file); }
  else { file->scrawl(0); }
  if ( normal )   { file->scrawl(1); normal->BinaryWrite(file); }
  else { file->scrawl(0); }
  if ( centroid ) { file->scrawl(1); centroid->BinaryWrite(file); }
  else { file->scrawl(0); }
  if ( tangent )  { file->scrawl(1); tangent->BinaryWrite(file); }
  else { file->scrawl(0); }
  points.BinaryWrite(file);
 }

 void BinaryRead( BinaryFile *file ) {
  file->read(&mat_hc);
  file->read(&id);
  int _draw_method; file->read(&_draw_method); draw_method=(GLenum) _draw_method;
  file->read(&area);
  file->read(&group);
  int value;
  file->read(&value); if ( value==1 ) { Vertex *b=new Vertex; b->BinaryRead(file); bary=b;     }
  file->read(&value); if ( value==1 ) { Vertex *b=new Vertex; b->BinaryRead(file); rotation=b; }
  file->read(&value); if ( value==1 ) { Vertex *b=new Vertex; b->BinaryRead(file); normal=b;   }
  file->read(&value); if ( value==1 ) { Vertex *b=new Vertex; b->BinaryRead(file); centroid=b; }
  file->read(&value); if ( value==1 ) { Vertex *b=new Vertex; b->BinaryRead(file); tangent=b;  }
  points.BinaryRead(file);
 }

 string toString() {
  string r;
  r=( mat_hc.length > 0 ? string("material=\"")+mat_hc+string("\", "): string("") );
  if ( id != 0 ) {
   FORMAT(buf,512, "id=%d, ",(int) id );
   r+=string(buf);
  } else
  if ( (int) draw_method != 0 ) {
   FORMAT(buf,512, "draw=%d, ", (int) draw_method );
   r+=string(buf);
  } else
  if ( area != 0.0f ) {
   FORMAT(buf,512, "area=%f, ", (float) area );
   r+=string(buf);
  } else
  if ( group != 0 ) {
   FORMAT(buf,512, "group=%d, ", (int) group );
   r+=string(buf);
  }
  r+=string("\nPoints {\n" )+points.toString()+string("}\n");
  return r;
 }
 void fromString( const char *s ) {
  string buf;
  const char *p=s;
  while ( *p!='\0' ) {
   p=string_argument(p,&buf);
   if ( buf==string("points") ) {
    p=string_argument(p,&buf);
    points.fromString((char *)buf.c_str());
   } else
   if ( buf==string("material") ) { // stores the string for later binding
    p=string_argument(p,&buf);
    mat_hc=buf;
   } else
   if ( buf==string("area") ) {
    p=string_argument(p,&buf); area=(float) atof((char *)buf.c_str());
   } else
   if ( buf==string("id") ) {
    p=string_argument(p,&buf); id=atoi((char *)buf.c_str());
   } else
   if ( buf==string("group") ) {
    p=string_argument(p,&buf); group=atoi((char *)buf.c_str());
   } else
   if ( buf==string("draw") ) {
    p=string_argument(p,&buf); draw_method=(GLenum) atoi((char *)buf.c_str());
   }
  }
 }

 mPolygon *Copy(void);
 mPolygon *Copy( int mangle, float dx, float dy );
 void Divide( Vertex *v );
 // Removes provided vertex from list
 void remVertex( Vertex *v );
 // Appends provided Vertex to list
 void addVertex( Vertex *v );
 void addVertex( Vertex *v, Crayon c );
 void Color( Crayon c );
 void ColorMultiply( Crayon c );
 // Does not dispose or append Vertexd provided
 Vertex *addVertex( Vertexd *v );
 Vertex *addVertex( Vertexd *v, Crayon c );
 Vertex *barycenter(void);
 void GrahamScan(void);
 bool inPoly( int xz_or_zy_or_xy, float xt, float yt );
 bool inPoly2d( float xt, float yt ) { // Bourke's
  int counter = 0;
  float xinters;
  Vertex *p1;
  p1 = (Vertex *) points.first;
  EACH(points.first,Vertex,v) {
    if (yt > UMIN(p1->y,v->y)) {
      if (yt <= UMAX(p1->y,v->y)) {
		  if(xt > UMIN(p1->x,v->y)){ //changed by shweta
        if (xt <= UMAX(p1->x,v->x)) {
          if (p1->y != v->y) {
            xinters = (yt-p1->y)*(v->x-p1->x)/(v->y-p1->y)+p1->x;
            if (xt == v->x || xt <= xinters) counter++;
          }
        }
		  }
      }
    }
    p1 = v;
  }
  if (counter % 2 == 0) return(false); else return(true);
 }
 bool pinpoly2d(float x, float y) { // Randolph Franklin's
  Vertex *j= (Vertex *) points.last;
  int c = 0;
  EACH(points.first,Vertex,i) {
   if ((((i->y <= y) && (y < j->y)) ||
        ((j->y <= y) && (y < i->y))) &&
       (x < (j->x - i->x) * (y - i->y) / (j->y - i->y) + i->x))
     c = !c;
   j=i;
  }
  return c==1;
 }
 //  Globals which should be set before calling this function:
 //  float  tx, ty       =  point to be tested
 //  The function will return YES if the point x,y is inside the polygon, or
 //  NO if it is not.  If the point is exactly on the edge of the polygon,
 //  then the function may return YES or NO.
 //  Note that division by zero is avoided because the division is protected
 //  by the "if" clause which surrounds it.
 // from http://alienryderflex.com/polygon/ most efficient example
 bool pointInPolygon( float tx, float ty ) {
  Vertex *j=(Vertex *) (points.last);
  bool oddNodes=false;
  EACH(points.first,Vertex,i) {
   if ((i->y < ty && j->y>=ty || j->y < ty && i->y>=ty) &&  (i->x<=tx || j->x<=tx)) 
   oddNodes^=(i->x+(ty-i->y)/(j->y-i->y)*(j->x-i->x)<tx);
   j=i; 
  }
  return oddNodes; 
 }
 // http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html (Attribution Requirement)
 bool inPoly( float px, float py, float pz ) {
  if ( !points.first ) return false;
  float minX = ((Vertex *) points.first)->x;
  float maxX = ((Vertex *) points.first)->x;
  float minY = ((Vertex *) points.first)->y;
  float maxY = ((Vertex *) points.first)->y;
  EACH(points.first,Vertex,q) {
   minX = UMIN( q->x, minX );
   maxX = UMAX( q->x, maxX );
   minY = UMIN( q->y, minY );
   maxY = UMAX( q->y, maxY );
  }
  if ( px < minX || px > maxX || py < minY || py > maxY ) return false;
  bool inside = false;
  Vertex *p=(Vertex *) points.last;
  EACH(points.first,Vertex,q) {
   if ( ( q->y > py ) != ( p->y > py )
     && px < ( p->x - q->x ) * ( py - q->y ) / ( p->y - q->y ) + q->x )
    inside = !inside;
   p=q;
  }
  return inside;
 } 
/* ======= Crossings algorithm ============================================ */

/* Shoot a test ray along +X axis.  The strategy, from MacMartin, is to
 * compare vertex Y values to the testing point's Y and quickly discard
 * edges which are entirely to one side of the test ray.
 *
 * Input 2D polygon _pgon_ with _numverts_ number of vertices and test point
 * _point_, returns 1 if inside, 0 if outside.	WINDING and CONVEX can be
 * defined for this test.
 */
#define CT_CONVEX
 int CrossingsTest( float tx, float ty ) {
#ifdef	CT_WINDING
register int	crossings ;
#endif
register int	j, yflag0, yflag1, inside_flag, xflag0 ;
#ifdef	CT_CONVEX
register int	line_flag ;
#endif
    Vertex *vt0=(Vertex *) points.first;
    Vertex *vt1=(Vertex *) points.last;
    yflag0 = ( vt0->y >= ty ) ;

#ifdef	CT_WINDING
    crossings = 0 ;
#else
    inside_flag = 0 ;
#endif
#ifdef	CT_CONVEX
    line_flag = 0 ;
#endif
    for ( j = points.count+1 ; --j ; ) {
	yflag1 = ( vt1->y >= ty ) ;
	/* check if endpoints straddle (are on opposite sides) of X axis
	 * (i.e. the Y's differ); if so, +X ray could intersect this edge.
	 */
	if ( yflag0 != yflag1 ) {
	    xflag0 = ( vt0->x >= tx ) ;
	    /* check if endpoints are on same side of the Y axis (i.e. X's
	     * are the same); if so, it's easy to test if edge hits or misses.
	     */
	    if ( xflag0 == ( vt1->x >= tx ) ) {

		/* if edge's X values both right of the point, must hit */
#ifdef	CT_WINDING
		if ( xflag0 ) crossings += ( yflag0 ? -1 : 1 ) ;
#else
		if ( xflag0 ) inside_flag = !inside_flag ;
#endif
	    } else {
		/* compute intersection of pgon segment with +X ray, note
		 * if >= point's X; if so, the ray hits it.
		 */
		if ( vt1->x-(vt1->y-ty)*(vt0->x-vt1->x)/(vt0->y-vt1->y) >= tx ) {
#ifdef	CT_WINDING
		    crossings += ( yflag0 ? -1 : 1 ) ;
#else
		    inside_flag = !inside_flag ;
#endif
		}
	    }
#ifdef	CT_CONVEX
	    /* if this is second edge hit, then done testing */
	    if ( line_flag ) goto Exit ;

	    /* note that one edge has been hit by the ray's line */
	    line_flag = TRUE ;
#endif
	}

	/* move to next pair of vertices, retaining info as possible */
	yflag0 = yflag1 ;
    vt0=vt1;
    if ( !vt1->next ) vt1=(Vertex *) vt1->prev;
    else
    vt1=(Vertex *) vt1->next;
    }
#ifdef	CT_CONVEX
    Exit: ;
#endif
#ifdef	CT_WINDING
    /* test if crossings is not zero */
    inside_flag = (crossings != 0) ;
#endif

    return( inside_flag ) ;
 }
 // Very not efficient, see above.
 /*bool InsidePolygon(float tx, float ty) { // Philippe Reverdy's
  if ( points.count < 3 ) return false;
  double angle=0.0f;
  Vertex p1,p2;
  EACH(points.first,Vertex,v) {
  Vertex *v_n=(Vertex *) v->next;
   if ( !v_n ) v_n=(Vertex *)points.first;
   p1.x=v->x-tx;
   p1.y=v->y-ty;
   p2.x=v_n->x-tx;
   p2.y=v_n->y-ty;
   angle+= Angle2D((double)p1.x,(double)p1.y,(double)p2.x,(double)p2.y);
  }
  if (abs(angle) < PI) return false;
  return true;
 }*/
 /*
    Return the angle between two vectors on a plane
    The angle is from vector 1 to vector 2, positive anticlockwise
    The result is between -pi -> pi
    Used by the above InsidePolygon test
 */
 /*double Angle2D(double x1, double y1, double x2, double y2)
 {
   double dtheta,theta1,theta2;

   theta1 = atan2(y1,x1);
   theta2 = atan2(y2,x2);
   dtheta = theta2 - theta1;
   while (dtheta > PI)
      dtheta -= TWO_PI;
   while (dtheta < -PI)
      dtheta += TWO_PI;

   return(dtheta);
 }*/
 // Möller–Trumbore ray-triangle intersection algorithm
 // Source: http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
 int RayTriangle( Vertex *Ro, Vertex *Rd, Vertex *out ) {
  Vertex *Ta=(Vertex *)(points.first);
  Vertex *Tb=(Vertex *)(points.first->next);
  Vertex *Tc=(Vertex *)(points.first->next->next);
  Vertex e1,e2;  // Edge 1, Edge 2
  Vertex D,K,a,b,c;
  a = Ta;
  b = Tb;
  c = Tc;
  float x,y,z,magni;
  D.Set(Rd); // added by Shweta because direction vector required not destination point.
  D.Subtract(Ro);

  x = D.x;
  y = D.y;
  z = D.z;

  magni = sqrt(x*x + y*y +z*z);

  D.x = D.x/magni;// normalizing D vector
  D.y = D.y/magni;
  D.z = D.z/magni;

  e1.Set(Tb); e1.Subtract(Ta);
  e2.Set(Tc); e2.Subtract(Ta);
  Vertex P,Q,T;
  float u = 0;
  float v = 0;
  float w = 0;
  float t = 0;
  float xx = 0;
  float yy = 0;
  float zz = 0;
  P.StoreCrossProduct(&D,&e2);
  float det = e1.DotProduct(&P);
  if ( det > -EPSILON && det < EPSILON ) return 0; // No intersection
  float inv_det = 1.f/det;
  T.Set(Ro);
  T.Subtract(Ta);
  
  u = T.DotProduct(&P) *inv_det;// Shweta changed e1 to T
  if ( u < 0.f || u > 1.f ) return 0; // No intersection
  Q.StoreCrossProduct(&T,&e1);

  v = D.DotProduct(&Q) * inv_det;
  if ( v < 0.f || u+v > 1.f ) return 0; // No intersection
  t = e2.DotProduct(&Q) * inv_det;
  w = 1-u-v;
  xx = a.x*w +b.x*u +c.x*v;//to calculate x, y, z
  yy = a.y*w +b.y*u +c.y*v;
  zz = a.z*w +b.z*u +c.z*v;
  out->Set(xx,yy,zz); 
  return 1; // Intersection
 }
 bool vertexInPoly( Vertex *v );
 bool vertexInPolyOrColocated( Vertex *v );
  void boundingBox( mPolygon *p, float *w, float *h );
 void setEqual( mPolygon *p );
 void ScaleX( float factor );
 void ScaleY( float factor );
 void ScaleZ( float factor );
 void UnscaleX( float factor );
 void UnscaleY( float factor );
 void UnscaleZ( float factor );
 void NewellsNormal() {
  normal.Recycle( new Vertex(0.0f,0.0f,0.0f) );
  int i;
  Vertex *v=(Vertex *) (points.first);
  for ( i=0; i<points.count; i++ ) {
   Vertex *n=(Vertex *) v->next;
   if ( !n ) break;
   normal->Add( (v->y-n->y)*(v->z+n->z),
                (v->z-n->z)*(v->x+n->x),
                (v->x-n->x)*(v->y+n->y) );
   v=n;
  }
  normal->normalize();
 }
 void TriNormal() {
  if ( points.count < 3 ) return;
  Vertex *V2,*V1,*V3;
  V1=(Vertex *) points.first;
  V2=(Vertex *) V1->next;
  V3=(Vertex *) V2->next;
  Vertex a,b;
  a.Set(V1);
  a.Subtract(V2);
  b.Set(V3);
  b.Subtract(V1);
  normal.Recycle(a.cross(&b));
 }
 void RenderPolyNormal() {
  glBegin( draw_method );
  glNormal3f(this->normal->x,this->normal->y,this->normal->z);
  EACH(points.first,Vertex,v) {
   glVertex3f(v->x,v->y,v->z);
   glTexCoord2f(v->s,v->t);
  }
  glEnd();
 }
// PolygonList *tesselate(void);
 void RenderImmediate2D() {
  if ( material ) {
   glBegin( draw_method );
   EACH(points.first,Vertex,v) {
    glColor4f(v->rgba[0],v->rgba[1],v->rgba[2],v->rgba[3]);
    glVertex2f(v->x,v->y);
    glTexCoord2f(v->r,v->q);
   }
   glEnd();
   return;
  }
  glBegin( draw_method );
  EACH(points.first,Vertex,v) {
   glColor4f(v->rgba[0],v->rgba[1],v->rgba[2],v->rgba[3]);
   glVertex2f(v->x,v->y);
  }
  glEnd();
 }
 void RenderImmediate3D() {
  if ( material ) {
   glBegin( draw_method );
   EACH(points.first,Vertex,v) {
    glColor4f(v->rgba[0],v->rgba[1],v->rgba[2],v->rgba[3]);
    glVertex3f(v->x,v->y,v->z);
    glTexCoord2f(v->r,v->q);
   }
   glEnd();
   return;
  }
  glBegin( draw_method );
  EACH(points.first,Vertex,v) {
   glColor4f(v->rgba[0],v->rgba[1],v->rgba[2],v->rgba[3]);
   glVertex3f(v->x,v->y,v->z);
  }
  glEnd();
 }
 void Barycenter() {
  static Vertex *v;
  bary.Recycle();
  EACH(points.first,Vertex,v) {
   bary->x+=v->x;
   bary->y+=v->y;
   bary->z+=v->z;
   bary->w+=v->w;
  }
  bary->x/=(float) points.count;
  bary->y/=(float) points.count;
  bary->z/=(float) points.count;
  bary->w/=(float) points.count;
 }
 void CalcTangentBasis() {
  if ( points.count == 3 ) {
   Vertex *a=(Vertex *) points.first;
   Vertex *b=(Vertex *) a->next;
   Vertex *c=(Vertex *) b->next;
   a->TangentBasis( b, c );
   b->TangentBasis( a, c );
   c->TangentBasis( a, b );
  } else if ( points.count % 3 == 0 && points.count >= 3 ) {
   int i=0;
   EACH(points.first,Vertex,a) {
    if ( i%3 == 0 ) {
     Vertex *b=(Vertex *) a->next;
     Vertex *c=(Vertex *) b->next;
     a->TangentBasis( b, c );
     b->TangentBasis( a, c );
     c->TangentBasis( a, b );
    }
    i++;
   }
  } else OUTPUT("Polygon::CalcTangentBasis() could not calculate because no method has been implemented for this criteria.\n");
 }
};

class Polygons : public LinkedList {
public:
 CLEARLISTRESET(mPolygon);
 Zpointer<mPolygon> resulting;
 Vertex barycenter;
 void PushTri( Vertex &a, Vertex &b, Vertex &c ) {
  mPolygon *p=new mPolygon;
  p->draw_method=GL_TRIANGLES;
  Vertex n;
  n.TriNormal(&a,&b,&c);
  Vertex *v;
  v=new Vertex(&a);
  v->normal.Recycle(new Vertex(&n));
  p->addVertex(v);
  v=new Vertex(&b);
  v->normal.Recycle(new Vertex(&n));
  p->addVertex(v);
  v=new Vertex(&c);
  v->normal.Recycle(new Vertex(&n));
  p->addVertex(v);
  Append(p);
 }
 void PushQuad( Vertex &a, Vertex &b, Vertex &c, Vertex &d ) {
  PushTri(a,b,c);
  PushTri(a,c,d);
 // PushTri(c,b,a);
//  PushTri(d,b,a);
 }
 void BinaryWriteData( BinaryFile *file ) {
  barycenter.BinaryWrite(file);
 }
 void BinaryReadData( BinaryFile *file ) {
  barycenter.BinaryRead(file);
 }
 void ReadAppend( BinaryFile *file ) {
  mPolygon *p=new mPolygon;
  p->BinaryRead(file);
  Append(p);
 }
 mPolygon *pointInPolysTest( int viewing, float xt, float yt );
 string toString() {
  string r;
  EACH(first,mPolygon,p) r+=string("{\n ")+p->toString()+string("\n},\n");
  return r;
 }
 void ColorMapVertices() {
  Vertex leasts;
  Leasts(&leasts);
  Vertex greatests;
  Greatests(&greatests);
  Vertex size( absf(greatests.x-leasts.x), absf(greatests.y-leasts.y), absf(greatests.z-leasts.z) );
  FOREACH(mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->Color(
     (v->x+leasts.x)/size.x,
     (v->y+leasts.y)/size.y,
     (v->z+leasts.z)/size.z,
     1.0f
    );
   }
  }
 }
 void ColorTransitiveMapVertices() {
  Vertex leasts;
  Leasts(&leasts);
  Vertex greatests;
  Greatests(&greatests);
  Vertex size( absf(greatests.x-leasts.x), absf(greatests.y-leasts.y), absf(greatests.z-leasts.z) );
  Crayon crayon;
  FOREACH(mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    crayon.ColorCombine4(
     crayons.Pick(blue255),
     crayons.Pick(skyBlue),
     crayons.Pick(cyan),
     crayons.Pick(green255),
     (v->x+leasts.x)/size.x,
     (v->y+leasts.y)/size.y,
     (v->z+leasts.z)/size.z,
     1.0f
    );
    v->Color(crayon);
   }
  }
 }
 void ColorTransitiveMapVertices( Crayon A, Crayon B, Crayon C, Crayon D ) {
  Vertex leasts;
  Leasts(&leasts);
  Vertex greatests;
  Greatests(&greatests);
  Vertex size( absf(greatests.x-leasts.x), absf(greatests.y-leasts.y), absf(greatests.z-leasts.z) );
  Crayon crayon;
  FOREACH(mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    crayon.ColorCombine4(
     A,B,C,D,
     size.x == 0 ? 0 : (v->x+leasts.x)/size.x,
     size.y == 0 ? 0 : (v->y+leasts.y)/size.y,
     size.z == 0 ? 0 : (v->z+leasts.z)/size.z,
     1.0f
    );
    v->Color(crayon);
   }
  }
 }
 void fromString( const char *s ) {
  string buf;
  const char *p=s;
  while ( *p != '\0' ) {
   p=string_argument(p,&buf);
   if ( buf.length() > 0 ) {
    mPolygon *mp=new mPolygon;
    mp->fromString((char *) buf.c_str());
    Append(mp);
   }
  }
 }
 // Assigns to v the least combined point in the list;
 void Leasts( Vertex *v ) {
  v->x=0.0f;
  v->y=0.0f;
  v->z=0.0f;
  if ( first ) {
   mPolygon *po=(mPolygon *) first;
   if ( po->points.first ) {
    Vertex *w=(Vertex *) (po->points.first);
    v->x=w->x;
    v->y=w->y;
    v->z=w->z;
   }
   FOREACH(mPolygon,p) {
    EACH(p->points.first,Vertex,n) {
     if ( n->x < v->x ) v->x=n->x;
     if ( n->y < v->y ) v->y=n->y;
     if ( n->z < v->z ) v->z=n->z;
    }
   }
  }
 }
 // Assigns to v the greatest combined point in the list;
 void Greatests( Vertex *v ) {
  v->x=0.0f;
  v->y=0.0f;
  v->z=0.0f;
  FOREACH(mPolygon,p) {
   EACH(p->points.first,Vertex,n) {
    if ( n->x > v->x ) v->x=n->x;
    if ( n->y > v->y ) v->y=n->y;
    if ( n->z > v->z ) v->z=n->z;
   }
  }
 }
 void Divide( float x, float y, float z ) { FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) { v->Divide(x,y,z); } }
 void Divide( Vertexd *d ) { FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) v->Divide(d); }
 void ScaleX( float factor );
 void ScaleY( float factor );
 void ScaleZ( float factor );
 void UnscaleX( float factor );
 void UnscaleY( float factor );
 void UnscaleZ( float factor );
 void Barycenter() {
  static mPolygon *p;
  barycenter.Set(0.0f,0.0f,0.0f);
  for ( p=(mPolygon *) first; p; p=(mPolygon *) p->next ) {
   p->Barycenter();
   barycenter.x+=p->bary->x;
   barycenter.y+=p->bary->y;
   barycenter.z+=p->bary->z;
   barycenter.w+=p->bary->w;
  }
  float c=(float) count;
  barycenter.x/=c;
  barycenter.y/=c;
  barycenter.z/=c;
  barycenter.w/=c;   
 }
 void RenderXY() {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4fv(v->rgba);
    glVertex2f(v->x,v->y);
    glTexCoord2f(v->r,v->a);
   }
   glEnd();
  }
 }
 void RenderXYScaled( float sx=1.0f, float sy=1.0f ) {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4fv(v->rgba);
    glVertex2f(v->x*sx,v->y*sy);
    glTexCoord2f(v->r,v->a);
   }
   glEnd();
  }
 }
 void RenderXYOffsetScaled( float ox=0.0f, float oy=0.0f, float sx=1.0f, float sy=1.0f ) {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4fv(v->rgba);
    glVertex2f(ox+v->x*sx,oy+v->y*sy);
    glTexCoord2f(v->r,v->a);
   }
   glEnd();
  }
 }
 void RenderXYOffsetScaledYInverted( float ox=0.0f, float oy=0.0f, float sx=1.0f, float sy=1.0f ) {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4fv(v->rgba);
    glVertex2f(ox+v->x*sx,oy+(-v->y)*sy);
    glTexCoord2f(v->r,v->a);
   }
   glEnd();
  }
 }
 void Render2dZY() {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4fv(v->rgba);
    glVertex2f(v->z,v->y);
    glTexCoord2f(v->r,v->a);
   }
   glEnd();
  }
 }
 // Materials are supported but must be applied before
 // Normals must be calculated before
 void Render() {
  FOREACH(mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glVertex3f(v->x,v->y,v->z);
     glTexCoord2f(v->r,v->a);
    }
   glEnd();
  }
 }
 // Used by Image2Placements to find like colored vertices.
 mPolygon *findFirstByColor( float *query ) {
  FOREACH(mPolygon,p) {
    EACH(p->points.first,Vertex,v) {
     if ( v->rgba[0] == query[0] 
       && v->rgba[1] == query[1]
       && v->rgba[2] == query[2]
       && v->rgba[3] == query[3] ) return p;
    }
  }
  return null;
 }
 mPolygon *findFirstBySize( int size ) {
  FOREACH(mPolygon,p) if ( p->points.count == size ) return p;
  return null;
 }
 mPolygon *findFirstLeastComplex() { // Finds the polygon with the least number of vertices
  mPolygon *candidate=null;
  FOREACH(mPolygon,p) {
   if ( !candidate ) {
    candidate=p;
    continue;
   }
   if ( candidate->points.count > p->points.count ) candidate=p;
  }
  return candidate;
 }
 mPolygon *findFirstMostComplex() { // Finds the polygon with the least number of vertices
  mPolygon *candidate=null;
  FOREACH(mPolygon,p) {
   if ( !candidate ) {
    candidate=p;
    continue;
   }
   if ( candidate->points.count < p->points.count ) candidate=p;
  }
  return candidate;
 }
 Vertex *closestToZero() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) {
   if ( !candidate ) { candidate=v; resulting=p; }
   else {
    if ( abs(v->x) < abs(candidate->x)
      && abs(v->y) < abs(candidate->y)
      && abs(v->z) < abs(candidate->z) ) {
     candidate=v;
     resulting=p;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) { candidate=v; resulting=p; }
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
     resulting=p;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t, float dfar ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) { candidate=v; resulting=p; }
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
     resulting=p;
    }
   }
  }
  return candidate;
 }
 mPolygon *Any() {
  if ( count==0 ) return null;
  return (mPolygon *) Element(upto(count*1234)%count);
 }
 mPolygon *Any( int seed ) {
  if ( count == 0 ) return null;
  Random *set=randoms.GetSet(seed+666);
  return (mPolygon *) Element((int)((float)count*2.0f*set->unique(seed,seed+1,seed+2))%count);
 }
 Vertex *Any( int seed, VertexHandles *excluding ) {
  mPolygon *p=Any(seed);
  Vertex *v=(Vertex *) p->points.Any(seed);
  int tries=0;
  while ( excluding->Contains(v) && tries < 30 ) {
   tries++;
   p=Any(seed+tries);
   v=(Vertex *) p->points.Any(seed+tries);
  }
  return v;
 }
 Vertex *greatestX() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->x > candidate->x ) candidate=v;
  return candidate;
 }
 Vertex *greatestY() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->y > candidate->y ) candidate=v;
  return candidate;
 }
 Vertex *greatestZ() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->z > candidate->z ) candidate=v;
  return candidate;
 }
 Vertex *leastX() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->x < candidate->x ) candidate=v;
  return candidate;
 }
 Vertex *leastY() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->y < candidate->y ) candidate=v;
  return candidate;
 }
 Vertex *leastZ() {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v)
   if ( !candidate ) candidate=v; else if ( v->z < candidate->z ) candidate=v;
  return candidate;
 }
 Vertex *greatestX( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->x > candidate->x ) candidate=v;
  }
  return candidate;
 }
 Vertex *greatestY( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->y > candidate->y ) candidate=v;
  }
  return candidate;
 }
 Vertex *greatestZ( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->z > candidate->z ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastX( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->x < candidate->x ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastY( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->y < candidate->y ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastZ( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(mPolygon,p) EACH(p->points.first,Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->z < candidate->z ) candidate=v;
  }
  return candidate;
 }
 void CopyAppend( Polygons *in, int g=-1 ) {
  EACH(in->first,mPolygon,p) {
   mPolygon *q=p->Copy();
   Append(q);
   q->group=g;
  }
 }
 void SetNotGroup( int notGroup, int g ) {
  FOREACH(mPolygon,p) if ( p->group != notGroup ) p->group = g;
 }
 void SetAllGroup( int g ) {
  FOREACH(mPolygon,p) p->group = g;
 }
 void MoveGroup( int g, float dx, float dy, float dz ) {
  FOREACH(mPolygon,p) if ( p->group == g ) p->points.MoveBy(dx,dy,dz);
 }
 void DeleteInGroup( int g ) {
  mPolygon *n;
  for ( mPolygon *p=(mPolygon *)first; p; p=n ) {
   n=(mPolygon *) p->next;
   if ( p->group == g ) {
    Remove(p);
    delete p; 
   }
  }
 }
 void FlipNormalsGroup( int g ) {
  FOREACH(mPolygon,p) if ( p->group == g ) {
   p->points.ReverseOrder();
   p->TriNormal();
  }
 }
 void MarkEdgePoints() {
  FOREACH(mPolygon,p) {
   FOREACH(mPolygon,q) if ( p != q ) {
   }
  }
 }
 void Color(Crayon c) {
  FOREACH(mPolygon,q) q->Color(c);
 }
 void ColorMultiply(Crayon c) {
  FOREACH(mPolygon,q) q->ColorMultiply(c);
 }
};

class PolygonHandle : public ListItem {
public:
 mPolygon *p;
 PolygonHandle(void) { p=null; }
 PolygonHandle( mPolygon *poly ) { p=poly; }
};

class PolygonList : public LinkedList {
public:
 CLEARLISTRESET(PolygonHandle);
 PolygonList(void) {}
 ~PolygonList(void) { Clear(); }
 bool inList( mPolygon *p );
 void addIfUnique( mPolygon *p );
 void freeList();
 void freeListandPolygons();
 PolygonList *copy();
 PolygonList *Duplicate();
 void setEqual( PolygonList *qL );
 void move( float x, float y, float z );
 void ScaleX( float factor );
 void ScaleY( float factor );
 void ScaleZ( float factor );
 void UnscaleX( float factor );
 void UnscaleY( float factor );
 void UnscaleZ( float factor );
 VertexList *toVertexList();
 PolygonList *findGroup( mPolygon *p );
 void Push( mPolygon *p );
 PolygonHandle *find( mPolygon *p );
 mPolygon *pointInPolysTest( int viewing, float xt, float yt );
 mPolygon *vertexInList(Vertex *v);
 Vertex *barycenter();
 Vertex *leastPoint();
 void rotateXY( float angleDeg );
 void rotateYZ( float angleDeg );
};