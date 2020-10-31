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
#include "GLWindow.h"
#include "GLImage.h"
#include "Strings.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Crayon.h"
#include "Vertex.h"
#include "Polygon.h"
#include "DisplayList.h"
#include "Trigger.h"
#include "GLTransform.h"
#include "VBOTriangles.h"

class VBOStaticVCLines;
class VBOStaticVNT;

class STLFile;

#define CYCLE_ON_OFF      0
#define CYCLE_FLICKER     1
#define CYCLE_BLINK       2
#define CYCLE_PULSE       3
#define CYCLE_FADE_IN_OUT 4                 
#define CYCLE_FADE_OUT    5
#define CYCLE_FADE_2XIN   6
#define CYCLE_ROTATE      7
#define CYCLE_ROTATE2     8
#define CYCLE_FLASH       9
#define CYCLE_1XSPOT     10
#define CYCLE_2XSPOT     11

#define TRAIL_BLUR        0  // leaves a blur of past states
#define TRAIL_PARTICLES   1  // leaves a trail of particles
#define TRAIL_STICKS      2  // leaves a trail of sticks
#define TRAIL_CIRCLES     3  // leaves a trail of circles
#define TRAIL_SQUARES     4  // leaves a trail of pulsing squares

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

//class MODELStorage : WebPage { MODEL t; };
class ImmediateModel : public ListItem {
public:
 ImmediateModel( ImmediateModel *a );
 ImmediateModel(void);
 ~ImmediateModel(void);
 Polygons polygons;
// POLYGON tesselated; // A list of triangles that has been calculated from *polygons
 Zint id;
 Zstring name;
 Zstring description;
 Zfloat volume, weight, density, springyness;
 Zint groups;
 //GLImage *thumbnail;
 Zdisposable<Vertex> bary,least,greatest;
 Zdisposable<Vertex> axis;  // may double as barycenter, or used for other things
 Zdisposable<Vertex> OAB; // Object-axis Aligned Box
 Vertex greatests,leasts,boxMagnitude;

 void Render();
 void RenderPolyNormal();

 void Stretch( double x, double y, double z ) {
  Unitize();
  Scale( (float)x, (float)y, (float)z );
 }

 void Zeroed() {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x-=leasts.x;
    v->y-=leasts.y;
    v->z-=leasts.z;
   }
  }
  BoxBounds();
 }
 // Equivalent of ZeroCentered(true,true,true)
 void Centered() { ZeroCentered(true,true,true); }
 // Will center at origin along one or multiple axis
 void ZeroCentered( bool x=false, bool y=false, bool z=true) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=(v->x-leasts.x) + ( x ? -boxMagnitude.x/2.0f : 0.0f);
    v->y=(v->y-leasts.y) + ( y ? -boxMagnitude.y/2.0f : 0.0f);
    v->z=(v->z-leasts.z) + ( z ? -boxMagnitude.z/2.0f : 0.0f);
   }
  }
  BoxBounds();
 }

 // Sets least to 0 and fits within 1.0 (v-least)/box
 void Unitize() {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=boxMagnitude.x!=0.0f?(v->x-leasts.x)/boxMagnitude.x:0.0f;
    v->y=boxMagnitude.y!=0.0f?(v->y-leasts.y)/boxMagnitude.y:0.0f;
    v->z=boxMagnitude.z!=0.0f?(v->z-leasts.z)/boxMagnitude.z:0.0f;
    if ( v->normal ) v->Normal();
   }
   if ( p->normal ) p->TriNormal();
  }
  BoxBounds();
 }
 // Centered version of Unitize()
 void Unit1Centered() {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=(boxMagnitude.x!=0.0f?(v->x-leasts.x)/boxMagnitude.x:0.0f)-0.5f;
    v->y=(boxMagnitude.y!=0.0f?(v->y-leasts.y)/boxMagnitude.y:0.0f)-0.5f;
    v->z=(boxMagnitude.z!=0.0f?(v->z-leasts.z)/boxMagnitude.z:0.0f)-0.5f;
    if ( v->normal ) v->Normal();
   }
   if ( p->normal ) p->TriNormal();
  }
  BoxBounds();
 }

 void Scale( Vertexd *scale ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x*=(float)scale->x;
    v->y*=(float)scale->y;
    v->z*=(float)scale->z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }

 void Scale( float x, float y, float z ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x*=x;
    v->y*=y;
    v->z*=z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }

 void Fit( float x, float y ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=((v->x-leasts.x)/(greatests.x-leasts.x))*x+leasts.x;
    v->y=((v->y-leasts.y)/(greatests.y-leasts.y))*y+leasts.y;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void FitYZ( float y, float z ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->y=((v->y-leasts.y)/(greatests.y-leasts.y))*y+leasts.y;
    v->z=((v->z-leasts.z)/(greatests.z-leasts.z))*z+leasts.z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void FitXZ( float x, float z ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=((v->x-leasts.x)/(greatests.x-leasts.x))*x+leasts.x;
    v->z=((v->z-leasts.z)/(greatests.z-leasts.z))*z+leasts.z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void Fit( float x, float y, float z ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=((v->x-leasts.x)/(greatests.x-leasts.x))*x+leasts.x;
    v->y=((v->y-leasts.y)/(greatests.y-leasts.y))*y+leasts.y;
    v->z=((v->z-leasts.z)/(greatests.z-leasts.z))*z+leasts.z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void FitX( float x ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->x=((v->x-leasts.x)/(greatests.x-leasts.x))*x+leasts.x;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void FitY( float y ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->y=((v->y-leasts.y)/(greatests.y-leasts.y))*y+leasts.y;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }
 void Fit( float z ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->z=((v->z-leasts.z)/(greatests.z-leasts.z))*z+leasts.z;
    if ( v->normal ) v->normalize();
   }
   if ( p->normal ) p->TriNormal();
  }
 }

 void BoxBounds() {
  polygons.Leasts(&leasts);
  polygons.Greatests(&greatests);
  boxMagnitude.Set(
   abs((float)(greatests.x-leasts.x)),
   abs((float)(greatests.y-leasts.y)),
   abs((float)(greatests.z-leasts.z))
  );
 }

 void Concat( ImmediateModel *m, int g ) {
  m->polygons.SetAllGroup(g);
  polygons.Concat(&m->polygons);
 }

 void CopyAsUnitCube( ImmediateModel *in ) {
  EACH(in->polygons.first,mPolygon,i) {
   mPolygon *p=i->Copy();
   p->Divide(&in->boxMagnitude);
   this->polygons.Append(p);
  }
 }

 void CopyInto( ImmediateModel *in ) {
  EACH(in->polygons.first,mPolygon,i) {
   mPolygon *p=i->Copy();
   this->polygons.Append(p);
  }
 }
 
 void Rotate( float x, float y, float z );
 void Rotate( Vertexd *scale );

 void CopyScaleRotateMove( ImmediateModel *in, Vertexd *move, Vertexd *scale, Vertexd *rotate );
 void CopyScaleRotateMove( STLFile *in, Vertexd *scale, Vertexd *rotate, Vertexd *move );
 void CopyAsUnitScaleRotateMove( STLFile *in, Vertexd *scale, Vertexd *rotate, Vertexd *move );
 void ScaleRotateMove( Vertexd *scale, Vertexd *rotate, Vertexd *move ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->Scale( (float)scale->x, (float)scale->y, (float)scale->z);
    v->Rotate((float)rotate->x,(float)rotate->y,(float)rotate->z);
    v->MoveBy((float)move->x,  (float)move->y,  (float)move->z);
   }
  }
 }
 
 // Sets the color of the vertexes to their XYZ position in 3d space based on box boundary
 void ColorMapVertices() {
  polygons.ColorMapVertices();
 }

 void ColorTransitiveMapVertices() {
  polygons.ColorTransitiveMapVertices();
 }

 void ColorTransitiveMapVertices( Crayon A, Crayon B, Crayon C, Crayon D ) {
  polygons.ColorTransitiveMapVertices(A,B,C,D);
 }

 void SetAlpha( double a ) {
  EACH(polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    v->Alpha((float)a);
   }
  }
 }

 
 VBOTriangles triangles; // used to connect ImmediateModel vertices with a VBO (use ModernVBO class to avoid using this)
 DisplayList dl_top, dl_right, dl_scaled, dlist;
 
 void BinaryWrite( BinaryFile *file ) {
  file->scrawl(id.value);
  file->scrawl(name.value);
  file->scrawl(description.value);
  file->scrawl(volume.value);
  file->scrawl(weight.value);
  file->scrawl(density.value);
  file->scrawl(springyness.value);
  file->scrawl(groups.value);
  polygons.BinaryWrite(file);
  triangles.BinaryWrite(file);
 }

 void BinaryRead( BinaryFile *file ) {
  file->read(&id);
  file->read(&name);
  file->read(&description);
  file->read(&volume);
  file->read(&weight);
  file->read(&density);
  file->read(&springyness);
  file->read(&groups);
  polygons.BinaryRead(file);
  triangles.BinaryRead(file);
 }
 
 // Utility (Texcoord and Vertex color modifiers)

 // Generates a "stupid TC" for each point in the model
 void StoopMap() {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   v->q=v->x;
   v->r=v->z;
  }
 }

 // Generates a "stupid TC" for each point in the model
 void PootsMap() {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   v->q=v->y;
   v->r=v->z;
  }
 }

 // Generates a "Cylindrical Map" for each point in the model
 void CylindricalMap() {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   v->q=sinf(v->x);
   v->r=v->z;
  }
 }

 // Generates a "Normal UV Map" for each point in the model
 void NormalUVMap() {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   v->q=v->x;
   v->r=v->z;
  }
 }

 // Generates a "Spherical Map" for each point in the model
 void SphereMap() {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   v->q=(asinf( (float) (v->x/v->z) ) / TWO_PIf) + 0.5f;
   v->r=(asinf( (float) v->y ) / PIf) + 0.5f;
  }
 }

 // Generates a "Cube Map" for each point in the model
 void CubeMap() {
  Vertex n;
  Vertex smallest,largest;
  polygons.Leasts(&smallest);
  polygons.Greatests(&largest);
  Box extents(smallest.x,smallest.y,smallest.z,largest.x,largest.y,largest.z);
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) {
   n.Set(v); n.normalize();
   Vertex an(&n);
   an.x=abs(an.x);
   an.y=abs(an.y);
   an.z=abs(an.z);
   v->q=an.x;
   v->r=an.y;
   if ( an.x > an.y && an.x > an.z ) {
    if ( n.z > 0 ) {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->y + extents.y) / (extents.h));
    } else {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->y + extents.y) / (extents.h));
    }
   } else if (an.y > an.x && an.y > an.z) { 
    if (n.y > 0) {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->z + extents.z) / (extents.d));
    } else {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->z + extents.z) / (extents.d));
    }
   } else {
    if (n.z > 0) {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->y + extents.y) / (extents.h));
    } else {
     v->q= (GLfloat) abs((v->x + extents.x) / (extents.w));
     v->r= (GLfloat) abs((v->y + extents.y) / (extents.h));
    }
   }   
  }
 }

 // Adds random grimyness
 void Grime( double factor, int density ) {
  EACH(polygons.first,mPolygon,m) EACH(m->points.first,Vertex,v) if ( number_range(0,density) == 1 ) {
   float oldalpha=v->rgba[3];
   Crayon grime; grime.fromFloat(v->rgba); grime.ScaleByAlphaf((float) (uniform()*factor));
   grime.Alpha(oldalpha);
   v->Color(grime);
  }
 }

 // 2D and 'Immediate' Render Modes
 void Render2dTopScaled( float f ) { Render2dXZ(f); } // legacy
 void Render2dXZ( float f ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glVertex2f(v->x*f,v->z*f);
    }
   glEnd();
  }
 }

 void Render2dSideScaled( float f ) { Render2dZY(f); } // legacy
 void Render2dZY( float f ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glVertex2f(v->z*f,v->y*f);
    }
   glEnd();
  }
 }

 void Render2dXY( float f ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glVertex2f(v->x*f,v->y*f);
    }
   glEnd();
  }
 }

 void Render3dScaled( float f ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x*f,v->y*f,v->z*f);
    }
   glEnd();
  }
 }

 void Render2dVT( int ox, int oy, int sx, int sy ) {
  Render2dVT( (float) ox, (float) oy, (float) sx, (float) sy );
 } 
 void Render2dVT( double ox, double oy, double sx, double sy ) {
  Render2dVT( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVT( float ox, float oy, float sx, float sy ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glTexCoord2f(v->q,v->r);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 void Render2dVNT( int ox, int oy, int sx, int sy ) {
  Render2dVNT( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVNT( double ox, double oy, double sx, double sy ) {
  Render2dVNT( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVNT( float ox, float oy, float sx, float sy ) {
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glTexCoord2f(v->q,v->r);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 
 void Render2dCM( Crayon c, int ox, int oy, int sx, int sy ) {
  Render2dCM( c, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dCM( Crayon c, double ox, double oy, double sx, double sy ) {
  Render2dCM( c, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dCM( Crayon c, float ox, float oy, float sx, float sy ) {
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor3d( c.rf*v->rgba[0], c.gf*v->rgba[1], c.bf*v->rgba[2] );
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 void Render2dC( int ox, int oy, int sx, int sy ) {
  Render2dC( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dC( double ox, double oy, double sx, double sy ) {
  Render2dC( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dC( float ox, float oy, float sx, float sy ) {
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }
  
 void Render2dC( Crayon c, int ox, int oy, int sx, int sy ) {
  Render2dC( c, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dC( Crayon c, double ox, double oy, double sx, double sy ) {
  Render2dC( c, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dC(Crayon c, float ox, float oy, float sx, float sy ) {
  c.gl();
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }
  
 void Render2dVC( int ox, int oy, int sx, int sy ) {
  Render2dVC( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC( double ox, double oy, double sx, double sy ) {
  Render2dVC( (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC( float ox, float oy, float sx, float sy ) {
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 void Render2dVC(Crayon multiply, int ox, int oy, int sx, int sy ) {
  Render2dVC(multiply, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC(Crayon multiply, double ox, double oy, double sx, double sy ) {
  Render2dVC( multiply, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC( Crayon multiply, float ox, float oy, float sx, float sy ) {
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     Crayon multiplied(v->rgba[0],v->rgba[1],v->rgba[2],v->rgba[3]);
     multiplied.MultiplyClampRGB(multiply.rf,multiply.gf,multiply.bf);
     glColor4fv(multiplied.floats);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 void Render2dVC( int ox, int oy, int oz, int sx, int sy, int sz ) {
  Render2dVC( (float) ox, (float) oy, (float) oz, (float) sx, (float) sy, (float) sz );
 }
 void Render2dVC( double ox, double oy, double oz, double sx, double sy, double sz ) {
  Render2dVC( (float) ox, (float) oy, (float) oz, (float) sx, (float) sy, (float) sz );
 }
 void Render2dVC( float ox, float oy, float oz, float sx, float sy, float sz ) {
  glDisable(GL_TEXTURE_2D);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
 }

 void Render2dVNC( GLImage *tex, int ox, int oy, int oz, int sx, int sy, int sz ) {
  Render2dVNC(tex,(float)ox,(float)oy,(float)oz,(float)sx,(float)sy,(float)sz );
 }
 void Render2dVNC( GLImage *tex, double ox, double oy, double oz, double sx, double sy, double sz ) {
  Render2dVNC(tex,(float)ox,(float)oy,(float)oz,(float)sx,(float)sy,(float)sz );
 }
 void Render2dVNC( GLImage *tex, float ox, float oy, float oz, float sx, float sy, float sz ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glNormal3f(p->normal->x,p->normal->y,p->normal->z);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }
 
 void Render2dVTC( GLImage *tex, int ox, int oy, int sx, int sy ) {
  Render2dVTC(tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVTC( GLImage *tex, double ox, double oy, double sx, double sy ) {
  Render2dVTC(tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVTC( GLImage *tex, float ox, float oy, float sx, float sy ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }

 void Render2dVNTC( GLImage *tex, int ox, int oy, int sx, int sy ) {
  Render2dVNTC(tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVNTC( GLImage *tex, double ox, double oy, double sx, double sy ) {
  Render2dVNTC(tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVNTC( GLImage *tex, float ox, float oy, float sx, float sy ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    EACH(p->points.first,Vertex,v) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->q,v->r);
     glNormal3f(p->normal->x,p->normal->y,p->normal->z);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }

 void Render2d( GLImage *tex, Crayon c, float ox, float oy, float oz, float sx, float sy, float sz ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  c.gl();
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glTexCoord2f(v->q,v->r);
     glVertex2f(ox+v->x*sx,oy+v->y*sy);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }

 void Render( GLImage *tex ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glTexCoord2f(v->q,v->r);
     glColor4fv(v->rgba);
     glVertex3f(v->x,v->y,v->z);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }

 void Render( GLImage *tex, Crayon c ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex->texture);
  c.gl();
  EACH(polygons.first,mPolygon,p) {
   glBegin(p->draw_method);
    glNormal3f(p->normal->x,p->normal->y,p->normal->z);
    EACH(p->points.first,Vertex,v) {
     glTexCoord2f(v->q,v->r);
     glVertex3f(v->x,v->y,v->z);
    }
   glEnd();
  }
  glDisable(GL_TEXTURE_2D);
 }


 // 2D and 'Immediate' Render Modes
 void Render2dTopScaled( GLTransform *xform, float f ) { Render2dXZ(xform,f); } // legacy
 void Render2dXZ( GLTransform *xform, float f ) {
  xform->PushAll();
  Render2dXZ(f);
  xform->PopAll();
 }

 void Render2dSideScaled( GLTransform *xform, float f ) { Render2dZY(xform,f); } // legacy
 void Render2dZY( GLTransform *xform, float f ) {
  xform->PushAll();
  Render2dZY(f);
  xform->PopAll();
 }

 void Render2dXY( GLTransform *xform, float f ) {
  xform->PushAll();
  Render2dXY(f);
  xform->PopAll();
 }

 void Render3dScaled( GLTransform *xform, float f ) {
  xform->PushAll();
  Render3dScaled(f);
  xform->PopAll();
 }

 void Render2dVT( GLTransform *xform, int ox, int oy, int sx, int sy ) {
  Render2dVT( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 } 
 void Render2dVT( GLTransform *xform, double ox, double oy, double sx, double sy ) {
  Render2dVT( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVT( GLTransform *xform, float ox, float oy, float sx, float sy ) {
  xform->PushAll();
  Render2dVT(ox,oy,sx,sy);
  xform->PopAll();
 }

 void Render2dVNT( GLTransform *xform, int ox, int oy, int sx, int sy ) {
  Render2dVNT( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVNT( GLTransform *xform, double ox, double oy, double sx, double sy ) {
  Render2dVNT( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVNT( GLTransform *xform, float ox, float oy, float sx, float sy ) {
  xform->PushAll();
  Render2dVNT(ox,oy,sx,sy);
  xform->PopAll();
 }
  
 void Render2dVC( GLTransform *xform, int ox, int oy, int sx, int sy ) {
  Render2dVC( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC( GLTransform *xform, double ox, double oy, double sx, double sy ) {
  Render2dVC( xform, (float) ox, (float) oy, (float) sx, (float) sy );
 }
 void Render2dVC( GLTransform *xform, float ox, float oy, float sx, float sy ) {
  xform->PushAll();
  Render2dVC(ox,oy,sx,sy);
  xform->PopAll();
 }

 void Render2dVC( GLTransform *xform, int ox, int oy, int oz, int sx, int sy, int sz ) {
  Render2dVC( xform, (float) ox, (float) oy, (float) oz, (float) sx, (float) sy, (float) sz );
 }
 void Render2dVC( GLTransform *xform, double ox, double oy, double oz, double sx, double sy, double sz ) {
  Render2dVC( xform, (float) ox, (float) oy, (float) oz, (float) sx, (float) sy, (float) sz );
 }
 void Render2dVC( GLTransform *xform, float ox, float oy, float oz, float sx, float sy, float sz ) {
  xform->PushAll();
  Render2dVC(ox,oy,oz,sx,sy,sz);
  xform->PopAll();
 }

 void Render2dVNC( GLTransform *xform, GLImage *tex, int ox, int oy, int oz, int sx, int sy, int sz ) {
  Render2dVNC(xform,tex,(float)ox,(float)oy,(float)oz,(float)sx,(float)sy,(float)sz );
 }
 void Render2dVNC( GLTransform *xform, GLImage *tex, double ox, double oy, double oz, double sx, double sy, double sz ) {
  Render2dVNC(xform,tex,(float)ox,(float)oy,(float)oz,(float)sx,(float)sy,(float)sz );
 }
 void Render2dVNC( GLTransform *xform, GLImage *tex, float ox, float oy, float oz, float sx, float sy, float sz ) {
  xform->PushAll();
  Render2dVNC(tex,ox,oy,oz,sx,sy,sz);
  xform->PopAll();
 }
 
 void Render2dVTC( GLTransform *xform, GLImage *tex, int ox, int oy, int sx, int sy ) {
  Render2dVTC(xform,tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVTC( GLTransform *xform, GLImage *tex, double ox, double oy, double sx, double sy ) {
  Render2dVTC(xform,tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVTC( GLTransform *xform, GLImage *tex, float ox, float oy, float sx, float sy ) {
  xform->PushAll();
  Render2dVTC(tex,ox,oy,sx,sy);
  xform->PopAll();
 }

 void Render2dVNTC( GLTransform *xform, GLImage *tex, int ox, int oy, int sx, int sy ) {
  Render2dVNTC(xform,tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVNTC( GLTransform *xform, GLImage *tex, double ox, double oy, double sx, double sy ) {
  Render2dVNTC(xform,tex,(float)ox,(float)oy,(float)sx,(float)sy);
 }
 void Render2dVNTC( GLTransform *xform, GLImage *tex, float ox, float oy, float sx, float sy ) {
  xform->PushAll();
  Render2dVNTC(tex,ox,oy,sx,sy);
  xform->PopAll();
 }

 void Render2d( GLTransform *xform, GLImage *tex, Crayon c, float ox, float oy, float oz, float sx, float sy, float sz ) {
  xform->PushAll();
  Render2d(xform,tex,c,ox,oy,oz,sx,sy,sz);
  xform->PopAll();
 }

 void Render( GLTransform *xform, GLImage *tex ) {
  xform->PushAll();
  Render(tex);
  xform->PopAll();
 }

 void Render( GLTransform *xform, GLImage *tex, Crayon c ) {
  xform->PushAll();
  Render(tex,c);
  xform->PopAll();
 }

 Zdisposable<VBOStaticVNT> vboVNT;
 // Display lists (deprecated features)
 
 void CompileLists( float f ) {
  if ( dl_top.uploaded ) dl_top.Delete();
  if ( dl_right.uploaded ) dl_right.Delete();
  if ( dl_scaled.uploaded ) dl_scaled.Delete();
  if ( dlist.uploaded ) dlist.Delete();
  if ( polygons.count < 1 ) return;

  dl_top.StartRecording();
  Render2dTopScaled(f);
  dl_top.StopRecording();

  dl_right.StartRecording();
  Render2dSideScaled(f);
  dl_right.StopRecording();

  dl_scaled.StartRecording();
  Render3dScaled(f);
  dl_scaled.StopRecording();

  dlist.StartRecording();
  Render();
  dlist.StopRecording();
 }

 // Returns the determined offset of a vertex from point 0, not very efficient
 Vertex *vertexElement( int i ) {
  int j=0;
  EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
    j++;
    if ( j==i ) return v;
  }
  return null;
 }

 // Calculates TriNormals for all poly
 void calculateNormals() {
  EACH(polygons.first,mPolygon,p) p->TriNormal();
 }

 string toString() {
  string r=string("");
  if ( name.length > 0 ) r=string("name=\"")+name+string("\"\n");
  if ( description.length > 0 ) r=string("//description//\n")+description+string("//end//\n");
  if ( volume != 0.0f ) {
   FORMAT(buf,512, "volume=%f, ", (float) volume );
   r+=string(buf);
  } else
  if ( weight != 0.0f ) {
   FORMAT(buf,512, "weight=%f, ", (float) weight );
   r+=string(buf);
  } else
  if ( density != 0.0f ) {
   FORMAT(buf,512, "density=%f, ", (float) density );
   r+=string(buf);
  } else
  if ( springyness != 0.0f ) {
   FORMAT(buf,512, "springyness=%f, ", (float) springyness );
   r+=string(buf);
  } else
  if ( groups != 0 ) {
   FORMAT(buf,512, "groups=%d, ", (int) groups );
   r+=string(buf);
  }
  if ( polygons.count > 0 ) r+=string("\nPolygons {\n")+polygons.toString()+string("\n}\n");
  if ( triangles.count > 0 ) r+=string("Triangles {\n")+triangles.toString()+string("\n}\n");
  return r;
 }

 void fromString( const char *s ) {
  string buf;
  const char *p=s;
  while ( *p != '\0' ) {
   p=string_argument(p,&buf);
   if ( buf==string("volume") ) {
    p=string_argument(p,&buf); volume=(float) atof((char *)buf.c_str());
   } else
   if ( buf==string("weight") ) {
    p=string_argument(p,&buf); weight=(float) atof((char *)buf.c_str());
   } else
   if ( buf==string("density") ) {
    p=string_argument(p,&buf); density=(float) atof((char *)buf.c_str());
   } else
   if ( buf==string("springyness") ) {
    p=string_argument(p,&buf); springyness=(float) atof((char *)buf.c_str());
   } else
   if ( buf==string("groups") ) {
    p=string_argument(p,&buf); groups=atoi((char *)buf.c_str());
   } else
   if ( buf==string("polygons") ) {
    p=string_argument(p,&buf);
    polygons.fromString((char *)buf.c_str());
   } else
   if ( buf==string("triangles") ) {
    p=string_argument(p,&buf);
    triangles.fromString((char *)buf.c_str());
   } else
   if ( buf==string("name") ) {
    p=string_argument(p,&name);
   } else
   if ( buf==string("//description//") ) {
    p=gather_to(p,"//end//",&description);
   }
  }
 }
 float cost(void);
 void Duplicate( ImmediateModel *m );
 void Duplicate( ImmediateModel *m, int mangle, float dx, float dy );
 ImmediateModel *Copy(void);
 ImmediateModel *CopyGroup( int g );
 PolygonList *findGroup( int g );
 void addPolygon( mPolygon *p );
 mPolygon *find_polygon_by_vertex( Vertex *v );
 Vertex *leastPoint(void);
 Vertex *greatestPoint(void);
 void MoveBy( Vertexd *d );
 void moveBy( float x, float y, float z );
 void limitTo( float x, float y, float z );
 inline void normalizeTo1(void);
 Vertex *barycenter(void);
 void align_to_barycenter( bool recalcbounds=true, bool calcbounds=false );
 void align_to_barycenterXY( bool recalcbounds=true, bool calcbounds=false );
 void align_to_origin( bool recalcbounds=true, bool calcbounds=false );
 void align_to_originXY( bool recalcbounds=true, bool calcbounds=false );
 /* Take an arbitrary 2D projection (throw out one of the coordinates),
  * and hash the AABBs (Axis Aligned Bounding Boxes) of the triangles
  * to a 2D array. Use the 2D array and the 2D projection of the point
  * you are testing to get a small set of triangles, which you do a 
  * 3D ray/triangle intersection test on http://geomalgorithms.com/a06-_intersect-2.html
  * (see Intersections of Rays, Segments, Planes and Triangles in 3D)
  * and count the number of triangles the ray intersection where the
  * z-coordinate (the coordinate thrown out) is greater than the
  * z-coordinate of the point. An even number of intersections means
  * it is outside the mesh. An odd number of intersections means it
  * is inside the mesh. This method is not only fast, but very easy to
  * implement.
  */
 bool PointInside( double x, double y, double z ) {
  unsigned int intersections=0;
  Indexed<AABBf> aaboxes;
  aaboxes.Size(polygons.count);
  unsigned int i=0;
  Vertex a,b,intersection;
  EACH(polygons.first,mPolygon,p) {
   p->points.AxisAlignedBoundingBoxXY(&aaboxes[i]);
   if ( aaboxes[i].Within( (float) x, (float) y) ) {
    a.Set((float)x,(float)y,(float)z);
    b.Set((float)x,(float)y,(float)z+1000000.0f);
    int result=p->RayTriangle( &a, &b, &intersection );
    if ( result == 1 ) if ( intersection.z > (float)z ) intersections+=1;
   }
   i++;
  }
  if ( intersections % 2 == 1 ) return true;
  else return false;
 }
 void RenderTopDown( GLWindow *win, int x, int y, int w, int h, Crayon colorLine, bool vertexes );
 void RenderSide( GLWindow *win, int x, int y, int w, int h, Crayon colorLine, bool vertexes );
 Vertex *findVertexWithIndex( int i ) {
  EACH(polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) if ( v->indices.find(i) != null ) return v;
  return null;
 }
 void RoughVolume() {
  // Calculate a simple volume based on the greatest point.
  Vertex *H=greatestPoint();
  volume=H->x*H->y*H->z;
  delete H;
 }
 void LoadOBJ( const char *filename );
 void CalcTangentBasis() {
  EACH(polygons.first,mPolygon,p) p->CalcTangentBasis();
 }
 void Clear() {
  polygons.Clear();
 }
 void PushLine(
   Vertex *a, Crayon *ca,
   Vertex *b, Crayon *cb ) {
  mPolygon *line=new mPolygon;
  line->draw_method=GL_LINES;
  Vertex normalVector;
  Vertex *A,*B;
  A=line->points.Add(a->x,a->y,a->z); A->Color(*ca);
  B=line->points.Add(b->x,b->y,b->z); B->Color(*cb);
  polygons.Append(line);
 }
 void PushLine(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb ) {
  mPolygon *line=new mPolygon;
  line->draw_method=GL_LINES;
  Vertex normalVector;
  Vertex *A,*B;
  A=line->points.Add(a->x,a->y,a->z); A->Color(*ca);
  B=line->points.Add(b->x,b->y,b->z); B->Color(*cb);
  polygons.Append(line);
 }
 void PushTri(
   Vertex *a, Crayon *ca,
   Vertex *b, Crayon *cb,
   Vertex *c, Crayon *cc ) {
  mPolygon *tri=new mPolygon;
  tri->draw_method=GL_TRIANGLES;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri->points.Add(b->x,b->y,b->z); B->Color(*cb); B->q=1.0; B->r=0.0;
  C=tri->points.Add(c->x,c->y,c->z); C->Color(*cc); C->q=1.0; C->r=0.0;
  A=tri->points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri->Barycenter();
  tri->TriNormal();
  tri->normal->normalize();
  tri->bary->Color(1.0,0.0,0.0,1.0);
  polygons.Append(tri);
 }
 void PushQuad(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   Vertexd *d, Crayon *cd ) {
  mPolygon *tri=new mPolygon;
  tri->draw_method=GL_TRIANGLES;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri->points.Add(b->x,b->y,b->z); B->Color(*cb); B->q=1.0; B->r=0.0;
  C=tri->points.Add(c->x,c->y,c->z); C->Color(*cc); C->q=1.0; C->r=0.0;
  A=tri->points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri->Barycenter();
  tri->TriNormal();
  tri->normal->normalize();
  tri->bary->Color(1.0,0.0,0.0,1.0);
//  vbo.PushTriangle(&tri);
  polygons.Append(tri);
  tri=new mPolygon;
  tri->draw_method=GL_TRIANGLES;
  tri->points.Clear();
  B=tri->points.Add(c->x,c->y,c->z); B->Color(*cc); B->q=1.0; B->r=0.0;
  C=tri->points.Add(d->x,d->y,d->z); C->Color(*cd); C->q=1.0; C->r=0.0;
  A=tri->points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri->Barycenter();
  tri->TriNormal();
  tri->normal->normalize();
  tri->bary->Color(1.0,0.0,0.0,1.0);
//  vbo.PushTriangle(&tri);
  polygons.Append(tri);
 }

 void Quad( Crayon c, int x, int y, int w, int h ) {

  Vertex *v;
  // AB   BAC BCD
  // CD
  mPolygon *tri1=new mPolygon; 
  addPolygon(tri1);
  tri1->draw_method=GL_TRIANGLES;
  v=new Vertex; v->Set(x+w,y);   v->TexCoord(0,1); v->Color(c); tri1->addVertex(v); //B
  v=new Vertex; v->Set(x,y);     v->TexCoord(0,0); v->Color(c); tri1->addVertex(v); //A
  v=new Vertex; v->Set(x,y+h);   v->TexCoord(1,0); v->Color(c); tri1->addVertex(v); //C
  tri1->TriNormal();

  mPolygon *tri2=new mPolygon;
  addPolygon(tri2);
  tri2->draw_method=GL_TRIANGLES;
  v=new Vertex; v->Set(x+w,y);   v->TexCoord(0,1); v->Color(c); tri2->addVertex(v); //B
  v=new Vertex; v->Set(x,y+h);   v->TexCoord(1,0); v->Color(c); tri2->addVertex(v); //C
  v=new Vertex; v->Set(x+w,y+h); v->TexCoord(1,1); v->Color(c); tri2->addVertex(v); //D
  tri2->TriNormal();
 }
};

class ImmediateModels : public LinkedList {
public:
 CLEARLISTRESET(ImmediateModel);
};

HANDLED(ImmediateModel,ImmediateModelHandle,ImmediateModelHandles,ImmediateModelHandlesHandle,ImmediateModelHandlesHandles);