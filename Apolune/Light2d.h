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
#include "Art.h"
#include "Art2d.h"

#include "Crayon.h"
#include "Cartesian.h"
#include "Vertex.h"
#include "Polygon.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "TextureLibrary.h"
#include "FBO.h"
#include "Shadows2DShader.h"

// Major splinterings; currently used by LightManaged2d to mildly optimize removal of lights
enum Light2dTypesAtomic {
 l2d_shaded=0,
 l2d_caster=1,
 l2d_static=2,
 l2d_shadowless=3,
 l2d_shaded16static=4
};

// Minor fragments, for identifying the light and used by LightDescripion2d programmatically
enum Light2dTypes {
 /* Spot2d                   */ spot_light_2d=0,
 /* TriangleSpot2d           */ triangle_spot_2d=1,
 /* Light2d                  */ point_light_2d=2,
 /* ShadedLight2d            */ shaded_light_2d=3,
 /* ShadowlessLight2d        */ shadowless_light_2d=4,
 /* ShadowlessShadedLight2d  */ shadowless_shaded_light_2d=5,
 /* BeamLight2d              */ light_beam_2d=6, // needs implemented, draws beam attenuation
 /* StaticSpot2d             */ static_spot_2d=7,
 /* StaticLight2d            */ static_point_2d=8,
 /* PulsingLight2d           */ pulsing_point_2d=9, // advanced pulse
 /* ShadowlessStaticLight2d  */ shadowless_static_point_2d=10,
 /* ShadowlessPulsingLight2d */ shadowless_pulsing_point_2d=11,
 /* PulsingSpot2d            */ pulsing_spot_2d=12,
 /* ShadowlessSpot2d         */ shadowless_spot_2d=13,
 /* ShadowlessPulsingSpot2d  */ shadowless_pulsing_spot_2d=14,
 /* ShadowlessStaticSpot2d   */ shadowless_static_spot_2d=15,
 /* GPUStaticLights2d        */ lightboard_static_2d=16,
                                gpulight_2d=17,
                                patch_gpulight_2d=18,
                                point_gpulight_2d=19,
                                pulsing_gpulight_2d=20,
};

/* Method:
 * 0) limit light drawing using the Zone system
 * 1) draw a light with an attenuation billboard (controlled by tweens)
 * 2) subtract shadows by drawing quads with the ambient color, or use shaders
 * 3) add to final fbo accumulator
 * 4) perform blur operations on final fbo if desired
 * 5) render over scene
 */

/* To determine wall relevance we use distance tests, could be enhanced with an octree */

#define MaximumShadowCast 100000.0

// A line that casts a shadow
struct Obstruction2d : public ListItem {
 Vertexd pt1,pt2;
 double d;
 bool reflects;
 Crayon filter;
 Obstruction2d() : ListItem() {
  d=0.0;
  filter.Float(0.0f,0.0f,0.0f,0.0f);
  reflects=false;
 }
 Obstruction2d(Vertexd p1,Vertexd p2) : ListItem() {
  d=0.0;
  pt1.Set(p1);
  pt2.Set(p2);
  filter.Float(0.0f,0.0f,0.0f,0.0f);
  reflects=false;
 }
 Obstruction2d(Vertexd p1,Vertexd p2,Crayon color) : ListItem() {
  d=0.0;
  pt1.Set(p1);
  pt2.Set(p2);
  filter.fromCrayon(color);
  reflects=false;
 }
 Obstruction2d(Vertexd p1,Vertexd p2,bool reflect) : ListItem() {
  d=0.0;
  pt1.Set(p1);
  pt2.Set(p2);
  filter.Float(0.0f,0.0f,0.0f,0.0f);
  reflects=reflect;
 }
 Obstruction2d(Vertexd p1,Vertexd p2,Crayon color,bool reflect) : ListItem() {
  d=0.0;
  pt1.Set(p1);
  pt2.Set(p2);
  filter.fromCrayon(color);
  reflects=reflect;
 }
 SKEYSTART
  SKEY("pt1",      { SPARAMType(pt1);       })
  SKEY("pt2",      { SPARAMType(pt2);       })
  SKEY("d",        { SPARAM(d,double,atof); })
  SKEY("reflects", { reflects=true;         })
  SKEY("filter",   { SPARAMColor(filter);   })
 SKEYEND("Obstruction2d");
 SOUTSTART
  SOUTT("pt1",pt1)
  SOUTT("pt2",pt2)
  SOUTB("reflects",reflects)
  if ( d!=0.0 ) SOUTFloat("d",d)
  SOUTT("filter",filter)
 SOUTEND
 double Distance(double x, double y) {
  double 
   A = x - pt1.x,
   B = y - pt1.y,
   C = pt2.x - pt1.x,
   D = pt2.y - pt1.y,
   dot = A * C + B * D,
   len_sq = C * C + D * D,
   param = dot / len_sq,
   xx,yy;
  if (param < 0) {
   xx = pt1.x;
   yy = pt1.y;
  } else if (param > 1) {
   xx = pt2.x;
   yy = pt2.y;
  } else {
   xx = pt1.x + param * C;
   yy = pt1.y + param * D;
  }
  return ddistance(x,y,xx,yy);
 }
 bool Relevant( Vertexd *position, double radius ) {
  return ( (d=Distance( position->x, position->y )) < radius );
 }
 void Debug() {
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue),(int)pt1.x,(int)pt1.y,(int)pt2.x,(int)pt2.y);
 }
 void RenderToFBO() {
  DrawLine(crayons.Pick(black),(int)pt1.x,(int)pt1.y,(int)pt2.x,(int)pt2.y);
 }
 void MoveBy( double dx, double dy ) {
  pt1.x+=dx;
  pt1.y+=dy;
  pt2.x+=dx;
  pt2.y+=dy;
 }
};

HANDLES(Obstruction2d,Obstruction2dHandle,Obstruction2dHandles,"obstruct");//,Obstruction2dHandlesHandle,Obstruction2dHandlesHandles);

// A bunch of lines that cast shadows
class Obstruction2ds : public LinkedList {
public:
 Vertexd centroid;
 Crayon *ambient;
 void MakeReflective() {
  FOREACH(Obstruction2d,o) o->reflects=true;
  CalculateCentroid();
 }
 void CalculateCentroid() {
  centroid.Set(0.0,0.0);
  FOREACH(Obstruction2d,o) {
   centroid.x+=o->pt1.x+o->pt2.x;
   centroid.y+=o->pt1.y+o->pt2.y;
  }
  centroid.x/=(double) count*2.0;
  centroid.y/=(double) count*2.0;
 }
 SORTING(Obstruction2d,SortDistance,{},{  if ( A->d < B->d ) result=-1; },{});
 Obstruction2d *Add(Vertexd pt1,Vertexd pt2) {
  Obstruction2d *wall=new Obstruction2d(pt1,pt2,*ambient);
  Append(wall);
  return wall;
 }
 Obstruction2d *Add(Vertexd pt1,Vertexd pt2,Crayon filter) {
  Obstruction2d *wall=new Obstruction2d(pt1,pt2,filter);
  Append(wall);
  return wall;
 }
 void Rectangle(double x, double y, double w, double h) {
  Obstruction2d *wall;
  wall=new Obstruction2d(Vertexd(x,y),Vertexd(x+w,y));     Append(wall); // ^-
  wall=new Obstruction2d(Vertexd(x+w,y),Vertexd(x+w,y+h)); Append(wall); // >|
  wall=new Obstruction2d(Vertexd(x,y+h),Vertexd(x+w,y+h)); Append(wall); // v_
  wall=new Obstruction2d(Vertexd(x,y),Vertexd(x,y+h));     Append(wall); // |<
 }
 Obstruction2dHandles *RectangleWithHandles(double x, double y, double w, double h) {
  Obstruction2dHandles *handles=new Obstruction2dHandles;
  Obstruction2d *wall;
  wall=new Obstruction2d(Vertexd(x,y),Vertexd(x+w,y));     Append(wall); handles->Add(wall); // ^-
  wall=new Obstruction2d(Vertexd(x+w,y),Vertexd(x+w,y+h)); Append(wall); handles->Add(wall); // >|
  wall=new Obstruction2d(Vertexd(x,y+h),Vertexd(x+w,y+h)); Append(wall); handles->Add(wall); // v_
  wall=new Obstruction2d(Vertexd(x,y),Vertexd(x,y+h));     Append(wall); handles->Add(wall); // |<
  return handles;
 }
 void Circle(double x, double y, double radius, int segments) {
  Obstruction2d *wall;
  if ( segments < 6 ) segments=6;
  else
  if ( segments % 2 == 1 ) segments++;
  double angle=0.0;
  double delta=TWO_PI / (double) segments;
  Vertexd last,start;
  last.Set(-1.0,-1.0);
  for ( int i=0; i<segments; i++ ) {
   if ( last.x==-1.0 && last.y==-1.0 ) {
    start.x=last.x=x+cos(angle)*radius;
    start.y=last.y=y+sin(angle)*radius;
    continue;
   }
   Vertexd point;
   point.x=x+cos(angle)*radius;
   point.y=y+sin(angle)*radius;
   wall=new Obstruction2d(Vertexd(last.x,last.y),Vertexd(point.x,point.y));
   Append(wall); 
   angle+=delta;
   last.Set(point);
  }
  wall=new Obstruction2d(Vertexd(last.x,last.y),Vertexd(start.x,start.y));
  Append(wall); 
 }
 Obstruction2dHandles *CircleWithHandles(double x, double y, double radius, int segments) {
  Obstruction2dHandles *handles=new Obstruction2dHandles;
  Obstruction2d *wall;
  if ( segments < 6 ) segments=6;
  else
  if ( segments % 2 == 1 ) segments++;
  double angle=0.0;
  double delta=TWO_PI / (double) segments;
  Vertexd last,start;
  last.Set(-1.0,-1.0);
  for ( int i=0; i<segments; i++ ) {
   if ( last.x==-1.0 && last.y==-1.0 ) {
    start.x=last.x=x+cos(angle)*radius;
    start.y=last.y=y+sin(angle)*radius;
    continue;
   }
   Vertexd point;
   point.x=x+cos(angle)*radius;
   point.y=y+sin(angle)*radius;
   wall=new Obstruction2d(Vertexd(last.x,last.y),Vertexd(point.x,point.y)); handles->Add(wall); Append(wall); 
   angle+=delta;
   last.Set(point);
  }
  wall=new Obstruction2d(Vertexd(last.x,last.y),Vertexd(start.x,start.y)); handles->Add(wall); Append(wall); 
  return handles;
 }
 void Poly( mPolygon *p, bool closed=true, bool dispose=true ) {
 }
 void PolyWithHandles( mPolygon *p, bool closed=true, bool dispose=true ) {
 }
 // Debug info
 void Debug() {
  FOREACH(Obstruction2d,o) o->Debug();
 }
 CLEARLISTRESET(Obstruction2d);
 ~Obstruction2ds() { Clear(); }
};

/*
 * Obstructions grouped around an 'object' metaphor,
 * a "closed convex hull" obstruction.
 */
class ObjectiveObstruction : public ListItem {
public:
 Vertexd centroid;
 Obstruction2ds obstructions;
 void CalculateCentroid() {
  centroid.Set(0.0,0.0);
  EACH(obstructions.first,Obstruction2d,o) {
   centroid.x+=o->pt1.x+o->pt2.x;
   centroid.y+=o->pt1.y+o->pt2.y;
  }
  centroid.x/=(double) obstructions.count*2.0;
  centroid.y/=(double) obstructions.count*2.0;
 }
 void MoveBy( double x, double y ) {
  EACH(obstructions.first,Obstruction2d,o) {
   o->MoveBy(x,y);
  }
  CalculateCentroid();
 }
};

class ObjectiveObstructions : public LinkedList {
public:
 Crayon *ambient;
 void Add(ObjectiveObstruction *o) {
  o->obstructions.ambient=ambient;
  Append(o);
 }
 ObjectiveObstruction *Add() {
  ObjectiveObstruction *o=new ObjectiveObstruction;
  o->obstructions.ambient=ambient;
  Append(o);
  return o;
 }
 void MoveBy( double x, double y ) {
  FOREACH(ObjectiveObstruction,o) o->MoveBy(x,y);
 }
 void Debug() {
  FOREACH(ObjectiveObstruction,o) o->obstructions.Debug();
 }
 CLEARLISTRESET(ObjectiveObstruction);
 ~ObjectiveObstructions() {
  Clear();
 }
};

class Light2ds;
class Light2dBase : public ListItem {
public:
 Light2dTypesAtomic atomic_type;
 Light2dTypes type;
};
class Light2d : public Light2dBase {
public:
 bool lit,add;
 double angle,turnRate;
 GLImage *flare;     // Desired light flare billboard
 GLImage *source;    // Desired lightsource add
 Vertexd position;   // Position à l'écran
 double intensity;   // (alpha value)
 double radius;      // radius in pixels (w/2)
 Blends flareBlend,sourceBlend;
 double attenuation; // attenuation (prefers to be smaller than radius)
 Blends attenuationBlend,shadowBlend;
 Crayon color,actual;       // Couleur de la lumière
 Crayon *ambient;
 Polygons shadows;          // Quads describing light subtraction
 Polygons reflections;      // TriFans describing light addition (reflection off an object's surface)
 bool reflects;
 double expired,duration,delta;
 Obstruction2dHandles relevant;
 // Constructeur et destructeur
 Light2d() : Light2dBase() {
  attenuationBlend=additive;
  shadowBlend=multiply;
  ambient=null;
  atomic_type=l2d_caster;
  type=point_light_2d;
  turnRate=0.25;
  angle=0.0;
  intensity=0.0;
  radius=20.0;
  attenuation=0.1;
  color.Pick(alabaster);
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare8.png");
  add=false;
  lit=true;
  flareBlend=sourceBlend=additive;
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=false;
 }
 Light2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  add=true;
  attenuationBlend=additive;
  shadowBlend=multiply;
  ambient=null;
  atomic_type=l2d_caster;
  type=point_light_2d;
  turnRate=0.25;
  angle=0.0;
  this->intensity=intensity;
  this->radius=radius;
  this->attenuation=attenuation;
  this->color.fromCrayon(color);
  color.ScaleByAlphaf((float)intensity);
  color.Pick(alabaster);
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare8.png");
  lit=true;
  flareBlend=sourceBlend=additive;
  MoveTo(position.x,position.y);
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=false;
 }
 Light2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color, bool reflect) {
  add=true;
  attenuationBlend=additive;
  shadowBlend=multiply;
  ambient=null;
  atomic_type=l2d_caster;
  type=point_light_2d;
  turnRate=0.25;
  angle=0.0;
  this->intensity=intensity;
  this->radius=radius;
  this->attenuation=attenuation;
  this->color.fromCrayon(color);
  color.ScaleByAlphaf((float)intensity);
  color.Pick(alabaster);
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare8.png");
  lit=true;
  flareBlend=sourceBlend=additive;
  MoveTo(position.x,position.y);
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=reflect;
 }
 virtual Light2d *Duplicate() {
  Light2d *light=new Light2d;
  light->intensity=intensity;
  light->radius=radius;
  light->attenuation=attenuation;
  light->color.fromCrayon(color);
  light->color.ScaleByAlphaf((float)intensity);
  light->color.Pick(alabaster);
  light->flare =flare;
  light->source=source;
  light->lit=lit;
  light->flareBlend=flareBlend;
  light->sourceBlend=sourceBlend;
  light->MoveTo(position.x,position.y);
  light->expired=0.0;
  light->duration=(double)float_range(0.0f,3.0f);
  light->delta=FRAMETIME;
  light->reflects=reflects;
  light->angle=angle;
  light->turnRate=turnRate;
  light->atomic_type=atomic_type;
  light->type=type;
  return light;
 }
 virtual Light2d *Duplicate( int ox, int oy ) {
  Light2d *l=Duplicate();
  l->MoveTo(l->position.x+ox,l->position.y+oy);
  return l;
 }
 void SetColor( Crayon color, double intensity ) {
  this->intensity=intensity;
  color.fromCrayon(color);
  color.ScaleByAlphaf((float)intensity);
  actual.fromCrayon(color);
 }
 virtual void MoveTo(double x, double y) {
  position.x=x;
  position.y=y;
  extents.x=x-radius;
  extents.y=y-radius;
  extents.x2=x+radius;
  extents.y2=y+radius;
 }
 virtual void MoveBy(double x, double y ) {
  position.x+=x;
  position.y+=y;
  extents.x+=x;
  extents.y+=y;
  extents.x2+=x;
  extents.y2+=y;
 }
 virtual bool OnScreen() {
  return ( extents.x > -radius && extents.x2 < display.w+radius ) 
      && ( extents.y > -radius && extents.y2 < display.h+radius );
 }
 virtual void Customize() {}
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}
 // Calculates the shadow subtractions
 Cartesiand extents;
 // Draws quads on the light map that delete areas that are dark
 virtual void CastShadows(Obstruction2ds *walls)  {
  Cartesiand calculator;
  double langle;
  relevant.Clear();
  EACH(walls->first,Obstruction2d,wall) {
   // Determine relevance
   if ( wall->Relevant(&position,radius) ) {
    relevant.Add(wall);
    Vertex *M,*A,*B;//,*closest,*farthest; to determine point intensity
    mPolygon *p=new mPolygon;
    shadows.Append(p);
    p->data=(void *) walls;
    M=p->points.Add( (float) wall->pt1.x, (float) wall->pt1.y, 0.0f );
    //We manipulate the color here based on light proximity.
    
    M=p->points.Add( (float) wall->pt2.x, (float) wall->pt2.y, 0.0f );
    // Two invisible lines extend from position to edge end points.
    // The rise and run of these lines extends to the screen edge from the edge points.
    // With the line angle we can use cos/sin to project.
    calculator.x=position.x;
    calculator.y=position.y;
    calculator.x2=wall->pt2.x;
    calculator.y2=wall->pt2.y;
    langle=calculator.LineAngle();
    A=p->points.Add( (float) (wall->pt2.x + MaximumShadowCast * cos(langle)),
                   (float) (wall->pt2.y + MaximumShadowCast * sin(langle)), 0.0f );
    calculator.x=position.x;
    calculator.y=position.y;
    calculator.x2=wall->pt1.x;
    calculator.y2=wall->pt1.y;
    langle=calculator.LineAngle();
    B=p->points.Add( (float) (wall->pt1.x + MaximumShadowCast * cos(langle)),
                   (float) (wall->pt1.y + MaximumShadowCast * sin(langle)), 0.0f );
    // In rare cases (like vertical lines) the shadow is not cast far enough to fully
    // obscure the light, so we "adjust" this manually to avoid clipping glitches.
    // Note, this may cause distortion on projected shadows!
    // To do this, we find the midpoint of the far segment and move it away from the
    // light center by a factor of the radius.
    //calculator.x=(double) (A->x+B->x)/2.0;
    //calculator.y=(double) (A->y+B->y)/2.0;
    //double dist=ddistance(calculator.x,calculator.y,position.x,position.y);
    // Project a new midpoint by treating this as a midpoint, then adjust the values of A and B
    //if ( dist < radius ) {
     A->x*=10.0f;     // was 2.0f
     A->y*=10.0f;     // was 2.0f
     B->x*=10.0f;     // was 2.0f
     B->y*=10.0f;     // was 2.0f
    //}
   }
  }
 }
 // Casts reflections on Obstruction2ds collected in the ObjectiveObstruction object,
 // processed per-object, occurs after shadows are cast in case the relevant shadows have filters.
 virtual void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {
  if ( walls->count < 1 ) return;  // oops, nothing to do
  // Copy wall positions out as vertices in the new polygon, eliminating duplicates.
  mPolygon *p=new mPolygon;
  EACH( relevant.first, Obstruction2dHandle, wallHandle ) if ( wallHandle->p->reflects ) {
   Obstruction2d *wall=wallHandle->p;
   p->points.addIfUnique((float)wall->pt1.x,(float)wall->pt1.y);
   p->points.addIfUnique((float)wall->pt2.x,(float)wall->pt2.y);
  }
  // Sort by distance by setting the ->a value to the distance from the light position.
  EACH(p->points.first,Vertex,v) v->a=fdistance((float) position.x, (float) position.y, v->x,v->y);
  p->points.Sort();
  // Now that distance is from least to greatest, we use the first value as the 'near' or white value.
  Vertex *closest=(Vertex *) p->points.first;
  Vertex *farthest=(Vertex *) p->points.last;
  // if ( !closest || !farthest ) return;  // oops, nothing to do, taken care of above
  // Set each point's color to the white value of the percentage of brightness based on proximity
  // If the point falls within a shadow map, floor to 0
  EACH(p->points.first,Vertex,v) {
   v->rgba[0]=v->rgba[1]=v->rgba[2]=1.0f-((v->a-closest->a)/(farthest->a-closest->a));// add light attenuation here
   v->rgba[0]*=actual.rf;
   v->rgba[1]*=actual.gf;
   v->rgba[2]*=actual.bf;
   EACH( shadows->first, mPolygon, q ) 
    if ( (!q->data || q->data != (void *) walls) && q->inPoly(1,v->x,v->y) ) {
     v->rgba[0]=v->rgba[1]=v->rgba[2]=0.0f;
     break;
    }
   v->rgba[3]=1.0f;
  }
  // Now calculate the line angle and wind the polygon properly, using the previously calculated
  // objective centroid.
  Cartesiand calculator;
  EACH(p->points.first,Vertex,v) {
   calculator.x2=(double) v->x-centroid->x;
   calculator.y2=(double) v->y-centroid->y;
   v->a=(float) calculator.LineAngle();
  }
  p->points.Sort();
  // Add to our reflections list
  reflections.Append(p);
 }
 // Move or adjust lighting
 virtual void Between() {
  angle+=turnRate;
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
  shadows.Clear();
  reflections.Clear();
 }
 virtual void Debug() {
  int x=(int)(position.x-radius);
  int y=(int)(position.y-radius);
  int x2=x+(int)(radius*2);
  int y2=y+(int)(radius*2);
  Blending(none);
  DrawLine(color,x,y,x2,y2);
  DrawLine(color,x2,y,x,y2);
  x=(int)(position.x-attenuation);
  y=(int)(position.y-attenuation);
  x2=x+(int)(attenuation*2);
  y2=y+(int)(attenuation*2);
  DrawLine(color,x+(int) attenuation,y,x+(int)attenuation,y2);
  DrawLine(color,x,y+(int) attenuation,x2,y+(int) attenuation);
  EACH(shadows.first,mPolygon,p) {
   glBegin(GL_LINES);
   EACH( p->points.first, Vertex, v ) {
    glVertex2f(v->x,v->y);
   }
   glEnd();
  }
 }
 void SubtractShadowsAndAddReflections() {
  // Subtract (or project) shadows
  Blending(none);
  glColor4f(0.0f,0.0f,0.0f,0.0f);//ambient->floats);
  EACH(shadows.first,mPolygon,p) {
   if ( p->material ) {
    glEnable(GL_TEXTURE_2D);
    GLuint *tex=(GLuint *) p->material;
    glBindTexture(GL_TEXTURE_2D,*tex);
    glBegin(GL_QUADS);
    EACH( p->points.first, Vertex, v ) {
     glColor4fv(v->rgba);
     glTexCoord2f(v->s,v->t);
     glVertex2f(v->x,v->y);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
   } else {
    glBegin(GL_QUADS);
    EACH( p->points.first, Vertex, v ) {
     glVertex2f(v->x,v->y);
    }
    glEnd();
   }
  }
  // Add object reflections
  Blending(none);
  EACH(reflections.first,mPolygon,p) {
   glBegin(GL_POLYGON);
   EACH( p->points.first, Vertex, v ) {
    glColor4fv(v->rgba);
    glVertex2f(v->x,v->y);
   }
   glEnd();
  }
 }
 virtual void Render() {
  PivotedRotatedRectf(flare,actual,flareBlend,(float)position.x,(float)position.y,0.0f,0.0f,(float)radius,(float)radius,(float)angle);
//  QuaddNoAlpha(flare,actual,flareBlend,position.x-radius,position.y-radius,radius*2,radius*2);
  if ( source ) 
   PivotedRotatedRectf(source,actual,sourceBlend,(float)position.x,(float)position.y,0.0f,0.0f,(float)attenuation,(float)attenuation,360.0f-(float)angle);
   //QuaddNoAlpha(source,actual,sourceBlend,position.x-attenuation,position.y-attenuation,attenuation*2,attenuation*2);
  SubtractShadowsAndAddReflections();
 }
};

HANDLES(Light2d,Light2dHandle,Light2dHandles,"light");

/* Major derivative class of Light2d, modifies the way shadows are cast and reflections are made. */
class Spot2d : public Light2d {
public:
 double emission;
 Spot2d() : Light2d() { atomic_type=l2d_caster; type=spot_light_2d; angle = 0.0; spread = 0.0; emission=0.0; }
 Spot2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color, double a, double spread )
       :Light2d(position, intensity, radius, attenuation, color), spread(spread), emission(0.0) {
  type=spot_light_2d;
  angle=a;
  turnRate=0.0; 
  actual.fromCrayon(color);
 }
 virtual ~Spot2d() {}
 virtual void Customize() {
 }
 void SetColor( Crayon c ) {
  color.fromCrayon(c);
  color.ScaleByAlphaf((float) intensity);
  actual.fromCrayon(c);
 }
 void PointAt( double x, double y ) {
  Cartesiand calculator;
  calculator.x=x;
  calculator.y=display.h-y;
  calculator.x2=position.x;
  calculator.y2=position.y;
  angle=90.0-RAD2DEG(calculator.LineAngle());
 }
 virtual void Render() {
  RotatedIsotrapf(flare,actual,flareBlend,(float)position.x,(float)position.y,(float)radius,(float)emission,(float)spread,(float)angle);
  if ( source ) QuaddNoAlpha(source,actual,sourceBlend,position.x-attenuation,position.y-attenuation,attenuation*2,attenuation*2);
  SubtractShadowsAndAddReflections();
 }
 virtual void Between() {
  angle+=turnRate;
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
  shadows.Clear();
  reflections.Clear();
 } 
 // Casts reflections on Obstruction2ds collected in the ObjectiveObstruction object,
 // processed per-object, occurs after shadows are cast in case the relevant shadows have filters.
 Cartesiand calculator;
 virtual void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {
  if ( walls->count < 1 ) return;  // oops, nothing to do
  // Copy wall positions out as vertices in the new polygon, eliminating duplicates.
  mPolygon *p=new mPolygon;
  EACH( relevant.first, Obstruction2dHandle, wallHandle ) if ( wallHandle->p->reflects ) {
   Obstruction2d *wall=wallHandle->p;
   p->points.addIfUnique((float)wall->pt1.x,(float)wall->pt1.y);
   p->points.addIfUnique((float)wall->pt2.x,(float)wall->pt2.y);
  }
  // Sort by distance by setting the ->a value to the distance from the light position.
  EACH(p->points.first,Vertex,v) v->a=fdistance((float) position.x, (float) position.y, v->x,v->y);
  p->points.Sort();
  // Now that distance is from least to greatest, we use the first value as the 'near' or white value.
  Vertex *closest=(Vertex *) p->points.first;
  Vertex *farthest=(Vertex *) p->points.last;
  // Generate a "spot range triangle" that describes (approximately) a spotlight's range and spread
  // imagine an isosceles triangle:
  //    x,y
  //    /\
  //   /  \ a,c=10    R= a/2sin(A) where R=radius, thus 
  // A+----+C
  //  b=spread*2
  mPolygon triangle;
  double radius9th=radius*0.9;
  float theta=rad2deg((float)atan2(spread,radius));
  float diff=deg2rad((float)(angle-theta+90.0));
  triangle.points.Add((float) position.x, (float) position.y, 0.0f );
  triangle.points.Add((float) (position.x + radius9th*cos(diff)), (float) (position.y + radius9th*sin(diff)) );
  diff=deg2rad((float)(angle+theta+90.0));
  triangle.points.Add((float) (position.x + radius9th*cos(diff)), (float) (position.y + radius9th*sin(diff)) );
  // if ( !closest || !farthest ) return;  // oops, nothing to do, taken care of above
  // Set each point's color to the white value of the percentage of brightness based on proximity
  // If the point falls within a shadow map, or outside the spot range, floor to 0
  EACH(p->points.first,Vertex,v) {
   if ( !triangle.pinpoly2d(v->x,v->y) ) {
     v->rgba[0]=v->rgba[1]=v->rgba[2]=0.0f;
     continue;
   }
   v->rgba[0]=v->rgba[1]=v->rgba[2]=1.0f-((v->a-closest->a)/(farthest->a-closest->a));// add light attenuation here
   v->rgba[0]*=actual.rf;
   v->rgba[1]*=actual.gf;
   v->rgba[2]*=actual.bf;
   EACH( shadows->first, mPolygon, q )
    if ( (!q->data || q->data != (void *) walls) && q->pinpoly2d(v->x,v->y) ) {
     v->rgba[0]=v->rgba[1]=v->rgba[2]=0.0f;
     break;
    }
   v->rgba[3]=1.0f;
  }
  // Now calculate the line angle and wind the polygon properly, using the previously calculated
  // objective centroid.
  EACH(p->points.first,Vertex,v) {
   calculator.x2=(double) v->x-centroid->x;
   calculator.y2=(double) v->y-centroid->y;
   v->a=(float) calculator.LineAngle();
  }
  p->points.Sort();
  // Add to our reflections list
  reflections.Append(p);
 }
 void Set( double a, double s ) {
  angle=a;
  spread=s;
 }
 double spread;
 virtual Light2d *Duplicate() {
  Spot2d *light=new Spot2d;
  light->intensity=intensity;
  light->radius=radius;
  light->attenuation=attenuation;
  light->color.fromCrayon(color);
  light->color.ScaleByAlphaf((float)intensity);
  light->color.Pick(alabaster);
  light->flare =flare;
  light->source=source;
  light->lit=lit;
  light->flareBlend=flareBlend;
  light->sourceBlend=sourceBlend;
  light->MoveTo(position.x,position.y);
  light->expired=0.0;
  light->duration=(double)float_range(0.0f,3.0f);
  light->delta=FRAMETIME;
  light->angle=angle;
  light->spread=spread;
  light->emission=emission;
  light->turnRate=turnRate;
  light->reflects=reflects;
  return (Light2d *) light;
 }
};

/* GPU Lights */

class ShadedLight2d : public Light2d {
public:
 Cartesiand cullLimit;
 ShadedLight2d() : Light2d() {
  atomic_type=l2d_shaded;
  type=shaded_light_2d;
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 ShadedLight2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  type=shaded_light_2d;
  this->intensity=intensity;
  this->radius=radius;
  this->attenuation=attenuation;
  this->color.fromCrayon(color);
  this->color.ScaleByAlphaf((float)intensity);
  actual.fromCrayon(color);
  flare=source=null;
  lit=true;
  flareBlend=sourceBlend=additive;
  MoveTo(position.x,position.y);
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=false;
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 bool OnScreen() {
  return ( position.x > cullLimit.x && position.x < cullLimit.x2 ) 
      && ( position.y > cullLimit.y && position.y < cullLimit.y2 );
 }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 virtual void Render() {
  shadows2dShader.position[0]=(float) this->position.x;
  shadows2dShader.position[1]=(float) this->position.y;
  shadows2dShader.radius=(float) radius;
  shadows2dShader.color.fromCrayon(this->actual);
  art.Stretch(&shadows2dShader,0,0,display.w,display.h);
 }
 void MoveTo(double x, double y) {
  position.x=(x)/display.wd;
  position.y=(display.hd-y)/display.hd;
 }
 void MoveBy(double x, double y ) {
  position.x+=(x)/display.wd;
  position.y+=(-y)/display.hd;
 }
 virtual void Between() {
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
 }
 virtual void Debug() {
  int x=(int)(position.x*display.wd)-10;
  int y=(int)(position.y*display.hd)-10;
  int x2=x+20;
  int y2=y+20;
  Blending(none);
  DrawLine(color,x,y,x2,y2);
  DrawLine(color,x2,y,x,y2);
 }
};

class ShadedShadowlessLight2d : public ShadedLight2d {
public:
 Cartesiand cullLimit;
 ShadedShadowlessLight2d() : ShadedLight2d() {
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 ShadedShadowlessLight2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  atomic_type=l2d_shaded;
  type=shadowless_shaded_light_2d;
  this->intensity=intensity;
  this->radius=radius;
  this->attenuation=attenuation;
  this->color.fromCrayon(color);
  this->color.ScaleByAlphaf((float)intensity);
  actual.fromCrayon(color);
  flare=source=null;
  lit=true;
  flareBlend=sourceBlend=additive;
  MoveTo(position.x,position.y);
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=false;
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 void Render() {
  light2dShader.position[0]=(float) this->position.x;
  light2dShader.position[1]=(float) this->position.y;
  light2dShader.color.fromCrayon(this->actual);
  light2dShader.radius=(float) radius;
  art.Stretch(&light2dShader,0,0,display.w,display.h);
 }
 void Between() {
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
 }
};

/* Derived from Light2d, an interim minor child
 * Light2d offers pulsing, but PulsingLight2d offers more control at a cost of cycles.
 * It is also programmatic, use the Numbers class to control speed of light shifts.
 */
enum PulsingLight2dModes {
 pl_pingpong=0,
 pl_rotate=1,
 pl_randomize=2
};

class PulsingLight2d : public Light2d {
public:
 PulsingLight2dModes mode;
 double min,max;
 bool once;
 Crayons colors;
 Crayon at,approaching;
 Numbers durations;
 PulsingLight2d() : Light2d() {
  once=false;
  atomic_type=l2d_caster;
  type=pulsing_point_2d;
  mode=pl_rotate;
  min=0.0;
  max=1.0;
 }
 bool Done() {
  return (once && expired>duration);
 }
 virtual void Between() {
  angle+=turnRate;
  double completeness=(expired/duration);
  expired+=FRAMETIME;
  if ( expired > duration ) {
   if ( once ) return;
   expired=0.0;
   switch ( mode ) {
    case pl_pingpong:
     break;
    case pl_rotate:
     break;
    case pl_randomize:
      approaching=colors.random();
     break;
   }
  }
 }
};

/* Simpler variations */

/*
 * Static (does not pulse or change color) version of the standard light
 */
class StaticLight2d : public Light2d {
public: 
 StaticLight2d() : Light2d() {
  atomic_type=l2d_static;
  type=static_point_2d;
 }
 void Between() {
  shadows.Clear();
  reflections.Clear();
 }
};

class ShadowlessStaticLight2d : public Light2d {
public: 
 ShadowlessStaticLight2d() : Light2d() { atomic_type=l2d_shadowless; type=shadowless_static_point_2d; }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 void Between() {}
};

class ShadowlessPulsingLight2d : public PulsingLight2d {
public:
 ShadowlessPulsingLight2d() : PulsingLight2d() { atomic_type=l2d_shadowless; type=shadowless_pulsing_point_2d; }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 void Between() {
 }
};

class StaticSpot2d : public Spot2d {
public:
 StaticSpot2d() : Spot2d() { atomic_type=l2d_shadowless; type=static_spot_2d; } 
 virtual void Between() {
  shadows.Clear();
  reflections.Clear();
 } 
};

class ShadowlessStaticSpot2d : public Spot2d {
public:
 ShadowlessStaticSpot2d() : Spot2d() { atomic_type=l2d_static; type=shadowless_static_spot_2d; }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 virtual void Between() {
 }
};

class PulsingSpot2d : public Spot2d {
public:
 PulsingSpot2d() : Spot2d() { atomic_type=l2d_caster; type=pulsing_spot_2d; } 
 void Between() {
  shadows.Clear();
  reflections.Clear();
 } 
};

class ShadowlessPulsingSpot2d : public Spot2d {
public:
 ShadowlessPulsingSpot2d() : Spot2d() { atomic_type=l2d_shadowless; type=shadowless_pulsing_spot_2d; }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 void Between() {
 } 
};

class ShadowlessSpot2d : public Spot2d {
public:
 ShadowlessSpot2d() : Spot2d() { atomic_type=l2d_shadowless; type=shadowless_spot_2d; } 
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 void Between() {
  angle+=turnRate;
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
 } 
};

class TriangleSpot2d : public Spot2d {
public:
 TriangleSpot2d() : Spot2d() { atomic_type=l2d_caster; type=triangle_spot_2d; }
 void Customize() {
  flare=library.Load("data/images/elements/white.png");
 }
 virtual void Render() {
  RotatedTrif(flare,color,flareBlend,(float)position.x,(float)position.y,(float)radius,(float)spread,(float)angle);
  if ( source ) QuaddNoAlpha(source,color,sourceBlend,position.x-attenuation,position.y-attenuation,attenuation*2,attenuation*2);
  SubtractShadowsAndAddReflections();
 }
 virtual void Between() {
  angle+=1.0;
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
  shadows.Clear();
  reflections.Clear();
 }
};

class ShadowlessLight2d : public Light2d {
public:
 Cartesiand cullLimit;
 ShadowlessLight2d() : Light2d() {
  atomic_type=l2d_shadowless;
  type=shadowless_light_2d;
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare8.png");
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 ShadowlessLight2d(Vertexd position, double intensity, double radius, double attenuation, Crayon color) {
  atomic_type=l2d_shadowless;
  type=shadowless_light_2d;
  this->intensity=intensity;
  this->radius=radius;
  this->attenuation=attenuation;
  this->color.fromCrayon(color);
  color.ScaleByAlphaf((float)intensity);
  color.Pick(alabaster);
  flare=library.Load("data/images/flares/neoflare1.png");
  source=library.Load("data/images/flares/neoflare8.png");
  lit=true;
  flareBlend=sourceBlend=additive;
  MoveTo(position.x,position.y);
  expired=0.0;
  duration=(double)float_range(0.0f,3.0f);
  delta=FRAMETIME;
  reflects=false;
  cullLimit.x=iratiod(-display.w/2,display.w);
  cullLimit.y=iratiod(-display.h/2,display.h);
  cullLimit.x2=iratiod(display.w+display.w/2,display.w);
  cullLimit.y2=iratiod(display.h+display.h/2,display.h);
 }
 bool OnScreen() {
  return ( position.x > cullLimit.x && position.x < cullLimit.x2 ) 
      && ( position.y > cullLimit.y && position.y < cullLimit.y2 );
 }
 void CastShadows(Obstruction2ds *walls)  {}
 void CreateReflections( Polygons *shadows, Obstruction2ds *walls, Vertexd *centroid ) {}
 virtual void Between() {
  if ( duration > 0.0 ) {
   double pulse=expired/duration;
   if ( expired>=duration ) delta=-delta;
   else
   if ( expired<=0.0 ) delta=abs(delta);
   expired+=delta;
   actual.fromCrayon(color);
   actual.ScaleByAlphaf((float)(pulse*intensity));
  }
 }
};


/*
/* GPU Light, Radius is 1-100, % of resulting display size (tex-coords are used to calculate position).
 * 3 lights per pass, with an RGB color (which determines intensity) and position.
 */
class ShadedLightBoard2d : public ListItem {
public:
 GPUMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 int active;
 ShadedLightBoard2d() { active=0; }
 void Render() {
  lights2dShader.bulbs=bulbs;
  art.Stretch(&lights2dShader,0.0,0.0,display.wd,display.hd);
 }
 virtual void Between() {}
 void MoveBy( float x, float y ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i].position[0]+=(x/display.wf);
   bulbs[i].position[1]+=(-y/display.hf);
  }
 }
};

class ShadedLightBoard2ds : public LinkedList {
public:
 int assigned;
 ShadedLightBoard2ds() : LinkedList() {
  assigned=0;
 }
 GPUMultiLight2d *Add( float radius, Crayon color, float x, float y ) {
  ShadedLightBoard2d *L;
  int board_requested=(assigned)/GPUMULTILIGHTLIMIT;
  if ( (assigned)%GPUMULTILIGHTLIMIT==0 || count==0 ) Append(L=new ShadedLightBoard2d);
  else L=(ShadedLightBoard2d *) Element(board_requested);
  int index=assigned-(GPUMULTILIGHTLIMIT*board_requested);
  L->bulbs[index].color.fromCrayon(color);
  L->bulbs[index].position[0]=(x/display.wf);
  L->bulbs[index].position[1]=1.0f-(y/display.hf);
  L->bulbs[index].radius=radius;
  L->active++;
  assigned++;
  return &L->bulbs[index];
 }
 void Reset() {
  Clear();
  assigned=0;
 }
 void Deactivate( GPUMultiLight2d *light ) {
  FOREACH(ShadedLightBoard2d,L) {
   for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) if ( light == &(L->bulbs[i]) ) {
    light->color.Pick(black);
    L->active--;
    assigned--;
    if ( L->active<=0 ) {
     Remove(L);
     delete L;
    }
    return;
   }
  }
 }
 void Render() { FOREACH(ShadedLightBoard2d,L) L->Render(); }
 void Between() { FOREACH(ShadedLightBoard2d,L) L->Between(); }
 void MoveBy(float x, float y) { FOREACH(ShadedLightBoard2d,L) L->MoveBy(x,y); }
 CLEARLISTRESET(ShadedLightBoard2d);
 ~ShadedLightBoard2ds() { Clear(); }
};


/* GPU Light, No Radius Parameter
 * 3 lights per pass, but no radius is provided so the hard coded value is used.
 * The hard coded value is set to a patch of color approximately 5-10% as wide as the screen.
 */
class ShadedNoRadiusLightBoard2d : public ListItem {
public:
 GPUNoRadiusMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 int active;
 ShadedNoRadiusLightBoard2d() { active=0; }
 void Render() {
  lightsnoradius2dShader.bulbs=bulbs;
  art.Stretch(&lightsnoradius2dShader,0.0,0.0,display.wd,display.hd);
 }
 virtual void Between() {}
 void MoveBy( float x, float y ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i].position[0]+=(x/display.wf);
   bulbs[i].position[1]+=(-y/display.hf);
  }
 }
};

class ShadedNoRadiusLightBoard2ds : public LinkedList {
public:
 int assigned;
 ShadedNoRadiusLightBoard2ds() : LinkedList() {
  assigned=0;
 }
 GPUNoRadiusMultiLight2d *Add( Crayon color, float x, float y ) {
  ShadedNoRadiusLightBoard2d *L;
  int board_requested=(assigned)/GPUMULTILIGHTLIMIT;
  if ( (assigned)%GPUMULTILIGHTLIMIT==0 || count==0 ) Append(L=new ShadedNoRadiusLightBoard2d);
  else L=(ShadedNoRadiusLightBoard2d *) Element(board_requested);
  int index=assigned-(GPUMULTILIGHTLIMIT*board_requested);
  L->bulbs[index].color.fromCrayon(color);
  L->bulbs[index].position[0]=(x/display.wf);
  L->bulbs[index].position[1]=1.0f-(y/display.hf);
  L->active++;
  assigned++;
  return &L->bulbs[index];
 }
 void Reset() {
  Clear();
  assigned=0;
 }
 void Deactivate( GPUNoRadiusMultiLight2d *light ) {
  FOREACH(ShadedNoRadiusLightBoard2d,L) {
   for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) if ( light == &(L->bulbs[i]) ) {
    light->color.Pick(black);
    L->active--;
    assigned--;
    if ( L->active<=0 ) {
     Remove(L);
     delete L;
    }
    return;
   }
  }
 }
 void Render() { FOREACH(ShadedNoRadiusLightBoard2d,L) L->Render(); }
 void Between() { FOREACH(ShadedNoRadiusLightBoard2d,L) L->Between(); }
 void MoveBy(float x, float y) { FOREACH(ShadedNoRadiusLightBoard2d,L) L->MoveBy(x,y); }
 CLEARLISTRESET(ShadedNoRadiusLightBoard2d);
 ~ShadedNoRadiusLightBoard2ds() { Clear(); }
};


/* GPU Light, No Color or Radius Parameter
 * 3 lights per pass, but no radius is provided so the hard coded value is used.
 * The hard coded value is set to a patch of vec3(1.0,1.0,1.0) approximately 1-5% as wide as the screen.
 */
class ShadedNoColorLightBoard2d : public ListItem {
public:
 GPUNoColorMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 int active;
 ShadedNoColorLightBoard2d() { active=0; }
 void Render() {
  lightsnocolor2dShader.bulbs=bulbs;
  art.Stretch(&lightsnocolor2dShader,0.0,0.0,display.wd,display.hd);
 }
 virtual void Between() {}
 void MoveBy( float x, float y ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i].position[0]+=(x/display.wf);
   bulbs[i].position[1]+=(-y/display.hf);
  }
 }
};

class ShadedNoColorLightBoard2ds : public LinkedList {
public:
 int assigned;
 ShadedNoColorLightBoard2ds() : LinkedList() {
  assigned=0;
 }
 GPUNoColorMultiLight2d *Add( float x, float y ) {
  ShadedNoColorLightBoard2d *L;
  int board_requested=(assigned)/GPUMULTILIGHTLIMIT;
  if ( (assigned)%GPUMULTILIGHTLIMIT==0 || count==0 ) Append(L=new ShadedNoColorLightBoard2d);
  else L=(ShadedNoColorLightBoard2d *) Element(board_requested);
  int index=assigned-(GPUMULTILIGHTLIMIT*board_requested);
  L->bulbs[index].position[0]=(x/display.wf);
  L->bulbs[index].position[1]=1.0f-(y/display.hf);
  L->active++;
  assigned++;
  return &L->bulbs[index];
 }
 void Reset() {
  Clear();
  assigned=0;
 }
 void Deactivate( GPUNoColorMultiLight2d *light ) {
  FOREACH(ShadedNoColorLightBoard2d,L) {
   for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) if ( light == &(L->bulbs[i]) ) {
    light->position[0]=light->position[1]=0.0f; // turns it off
    L->active--;
    assigned--;
    if ( L->active<=0 ) {
     Remove(L);
     delete L;
    }
    return;
   }
  }
 }
 void Render() { FOREACH(ShadedNoColorLightBoard2d,L) L->Render(); }
 void Between() { FOREACH(ShadedNoColorLightBoard2d,L) L->Between(); }
 void MoveBy(float x, float y) { FOREACH(ShadedNoColorLightBoard2d,L) L->MoveBy(x,y); }
 CLEARLISTRESET(ShadedNoColorLightBoard2d);
 ~ShadedNoColorLightBoard2ds() { Clear(); }
};


/* Time-based Pulsing GPU Light, with all parameters including cycle.
 * 3 lights per pass, but no radius is provided so the hard coded value is used.
 * The hard coded value is set to a patch of vec3(1.0,1.0,1.0) approximately 1-5% as wide as the screen.
 */
class ShadedPulsingLightBoard2d : public ListItem {
public:
 GPUPulsingMultiLight2d bulbs[GPUMULTILIGHTLIMIT];
 int active;
 ShadedPulsingLightBoard2d() { active=0; }
 void Render() {
  lightspulsing2dShader.bulbs=bulbs;
  art.Stretch(&lightspulsing2dShader,0.0,0.0,display.wd,display.hd);
 }
 virtual void Between() {}
 void MoveBy( float x, float y ) {
  for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) {
   bulbs[i].position[0]+=(x/display.wf);
   bulbs[i].position[1]+=(-y/display.hf);
  }
 }
};

class ShadedPulsingLightBoard2ds : public LinkedList {
public:
 int assigned;
 ShadedPulsingLightBoard2ds() : LinkedList() {
  assigned=0;
 }
 GPUPulsingMultiLight2d *Add( float cycle, float radius, Crayon color, float x, float y ) {
  ShadedPulsingLightBoard2d *L;
  int board_requested=(assigned)/GPUMULTILIGHTLIMIT;
  if ( (assigned)%GPUMULTILIGHTLIMIT==0 || count==0 ) Append(L=new ShadedPulsingLightBoard2d);
  else L=(ShadedPulsingLightBoard2d *) Element(board_requested);
  int index=assigned-(GPUMULTILIGHTLIMIT*board_requested);
  L->bulbs[index].position[0]=(x/display.wf);
  L->bulbs[index].position[1]=1.0f-(y/display.hf);
  L->bulbs[index].cycle=cycle;
  L->bulbs[index].radius=radius;
  L->bulbs[index].color.fromCrayon(color);
  L->active++;
  assigned++;
  return &L->bulbs[index];
 }
 void Reset() {
  Clear();
  assigned=0;
 }
 void Deactivate( GPUPulsingMultiLight2d *light ) {
  FOREACH(ShadedPulsingLightBoard2d,L) {
   for ( int i=0; i<GPUMULTILIGHTLIMIT; i++ ) if ( light == &(L->bulbs[i]) ) {
    light->color.Pick(black);
    L->active--;
    assigned--;
    if ( L->active<=0 ) {
     Remove(L);
     delete L;
    }
    return;
   }
  }
 }
 void Render() { FOREACH(ShadedPulsingLightBoard2d,L) L->Render(); }
 void Between() { FOREACH(ShadedPulsingLightBoard2d,L) L->Between(); }
 void MoveBy(float x, float y) { FOREACH(ShadedPulsingLightBoard2d,L) L->MoveBy(x,y); }
 CLEARLISTRESET(ShadedPulsingLightBoard2d);
 ~ShadedPulsingLightBoard2ds() { Clear(); }
};

/*
 * Special subclasses wrapped around the simoid-based lighting method.
 * Inefficiently (memory) based on Light2d class so it can be handled by default manager-related Light2dHandle lists properly.
 * These are described by these types:
 *  gpulight_2d=17,
 *  patch_gpulight_2d=18,
 *  point_gpulight_2d=19,
 *  pulsing_gpulight_2d=20,
 */
class Light2dController : public Light2d {
public:
 Light2dController() : Light2d() {}
 virtual void Between() {}
 virtual void MoveBy( int x, int y ) {}
 virtual void MoveBy( double x, double y ) {}
};

/*
 * Genericized controller classes passed back from LightManager2d's "Add" methods.
 * To implement specifics, childs must be made both of LightManager2d and of these for a full integration.
 * Purpose: modify values with CPU, remove light from lightboard when deleted.
 */
class GPUMultiLight2dController : public Light2dController {
public:
 ShadedLightBoard2ds *board;
 GPUMultiLight2d *light;
 GPUMultiLight2dController( ShadedLightBoard2ds *b, GPUMultiLight2d *ptr ) 
  : Light2dController() { board=b; light=ptr; type=gpulight_2d; }
 ~GPUMultiLight2dController() { board->Deactivate(light); }
 virtual void Between() {}
 virtual void MoveBy( int x, int y ) {
  light->position[0]+=iratiof(x,display.w);
  light->position[1]+=iratiof(y,display.h);
 }
 virtual void MoveBy( double x, double y ) {
  light->position[0]+=(float)(x/display.wd);
  light->position[1]+=(float)(-y/display.hd);
 }
};

class GPUNoRadiusMultiLight2dController : public Light2dController {
public:
 ShadedNoRadiusLightBoard2ds *board;
 GPUNoRadiusMultiLight2d *light;
 GPUNoRadiusMultiLight2dController( ShadedNoRadiusLightBoard2ds *b, GPUNoRadiusMultiLight2d *ptr ) 
  : Light2dController() { board=b; light=ptr; type=patch_gpulight_2d; }
 ~GPUNoRadiusMultiLight2dController() { board->Deactivate(light); }
 virtual void Between() {}
 virtual void MoveBy( int x, int y ) {
  light->position[0]+=iratiof(x,display.w);
  light->position[1]+=iratiof(-y,display.h);
 }
 virtual void MoveBy( double x, double y ) {
  light->position[0]+=(float)(x/display.wd);
  light->position[1]+=(float)(-y/display.hd);
 }
};

class GPUNoColorMultiLight2dController : public Light2dController {
public:
 ShadedNoColorLightBoard2ds *board;
 GPUNoColorMultiLight2d *light;
 GPUNoColorMultiLight2dController( ShadedNoColorLightBoard2ds *b, GPUNoColorMultiLight2d *ptr ) 
  : Light2dController() { board=b; light=ptr; type=point_gpulight_2d; }
 ~GPUNoColorMultiLight2dController() { board->Deactivate(light); }
 virtual void Between() {}
 virtual void MoveBy( int x, int y ) {
  light->position[0]+=iratiof(x,display.w);
  light->position[1]+=iratiof(y,display.h);
 }
 virtual void MoveBy( double x, double y ) {
  light->position[0]+=(float)(x/display.wd);
  light->position[1]+=(float)(-y/display.hd);
 }
};

class GPUPulsingMultiLight2dController : public Light2dController {
public:
 ShadedPulsingLightBoard2ds *board;
 GPUPulsingMultiLight2d *light;
 GPUPulsingMultiLight2dController( ShadedPulsingLightBoard2ds *b, GPUPulsingMultiLight2d *ptr ) 
  : Light2dController() { board=b; light=ptr; type=pulsing_gpulight_2d; }
 ~GPUPulsingMultiLight2dController() { board->Deactivate(light); }
 virtual void Between() {}
 virtual void MoveBy( int x, int y ) {
  light->position[0]+=iratiof(x,display.w);
  light->position[1]+=iratiof(y,display.h);
 }
 virtual void MoveBy( double x, double y ) {
  light->position[0]+=(float)(x/display.wd);
  light->position[1]+=(float)(-y/display.hd);
 }
};

class Light2ds : public LinkedList {
public:
 Cartesian screen;
 Crayon *ambient;
 Light2ds() : LinkedList() {
  ambient=null;
  screen.SetRect(0,0,display.w,display.h);
 }
 void MoveBy( double x, double y ) {
  FOREACH(Light2d,o) o->MoveBy(x,y);
 }
 void CastShadows(Obstruction2ds *walls) {
  FOREACH(Light2d,o) {
   o->CastShadows(walls);
   if ( o->reflects ) o->CreateReflections(&o->shadows,walls,&walls->centroid);
  }
 }
 void CastShadows(ObjectiveObstructions *objects) {
  FOREACH(Light2d,L) {
   EACH( objects->first, ObjectiveObstruction, o ) {
    L->CastShadows(&o->obstructions);
    if ( L->reflects ) L->CreateReflections(&L->shadows,&o->obstructions,&o->centroid);
   }
  }
 }
 void Between() { FOREACH(Light2d,light) light->Between(); }
 // Debug info
 void Debug() {
  FOREACH(Light2d,o) o->Debug();
 }
 CLEARLISTRESET(Light2d);
};