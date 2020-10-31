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

#include "VBO.h"
#include "Model.h"
#include "Tweens.h"

extern Tweens tweens;

/*
 * Inputs: Two models, in the same homogenous vertex format as provided by the Image2Models class,
 * and a tween selection (default is a linear transition), a timescale, optional color pairing parameters.
 * Outputs: A series of VBOs that interpolate the points moving between the positions.
 *          If both models do not have 1-1 ratio of points, points converge when A>B, or split B>A (needs implemented)
 *          You can override converge/diverge with fade by selecting ipfc_fade
 *          Not yet implemented: Only 1-1 ratios are supported currently
 */
enum InterpolatedPointFilmClipModes {
 ipfc_converge=0,
 ipfc_diverge=1,
 ipfc_move=2,
 ipfc_fade=3
};

class InterpolatedPointFilmClip : public ListItem {
public:
 ImmediateModel *A, *B;
 mPolygon *a,*b;
 Zpointer<Tween> tween;
 float deltaTime;
 int points;
 InterpolatedPointFilmClipModes operation;
 InterpolatedPointFilmClip() : ListItem() {
  tween=tweens.find("Linear");
  A=B=null;
  deltaTime=FRAMETIME;
 }
 InterpolatedPointFilmClip( ImmediateModel *a, ImmediateModel *b ) : ListItem() {
  tween=tweens.find("Linear");
  deltaTime=FRAMETIME;
  SetModels(a,b);
 }
 InterpolatedPointFilmClip( Tween *t, ImmediateModel *a, ImmediateModel *b ) : ListItem() {
  tween=t;
  deltaTime=FRAMETIME;
  SetModels(a,b);
 }
 InterpolatedPointFilmClip( const char *tweenName, ImmediateModel *a, ImmediateModel *b ) : ListItem() {
  tween=tweens.find(tweenName);
  deltaTime=FRAMETIME;
  SetModels(a,b);
 } 
 void SetModels( ImmediateModel *aa, ImmediateModel *bb ) {
  A=aa;
  B=bb;
  a=(mPolygon *)A->polygons.first;
  b=(mPolygon *)B->polygons.first;
  if ( a->points.count > b->points.count ) {
   operation=ipfc_converge;
   points=a->points.count;
  } else if ( a->points.count < b->points.count ) {
   operation=ipfc_diverge;
   points=b->points.count;
  } else {
   operation=ipfc_move;
   points=a->points.count;
  }
 }
 VBOStaticVCs *Calculate( float timescale ) {
  VBOStaticVCs *vbos=new VBOStaticVCs;
  ImmediateModel working;
  mPolygon *vertexes=new mPolygon;
  working.addPolygon(vertexes);
  if ( operation == ipfc_converge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else if ( operation == ipfc_diverge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
    vertexes->points.Clear();
    Vertex *v2=(Vertex *) b->points.first;
    EACH(a->points.first,Vertex,v) {
     float f=tween->tweenf(t/timescale);
     Vertex *w=new Vertex;
     w->x=v->x+f*(v2->x-v->x);
     w->y=v->y+f*(v2->y-v->y);
     w->z=v->z+f*(v2->z-v->z);
     w->rgba[0]=v->rgba[0]+f*(v2->rgba[0]-v->rgba[0]);
     w->rgba[1]=v->rgba[1]+f*(v2->rgba[1]-v->rgba[1]);
     w->rgba[2]=v->rgba[2]+f*(v2->rgba[2]-v->rgba[2]);
     w->rgba[3]=v->rgba[3]+f*(v2->rgba[3]-v->rgba[3]);
     vertexes->points.Append(w);
     v2=(Vertex *)v2->next;
    }
    VBOStaticVC *vc=new VBOStaticVC;
    vc->fromModel(&working);
    vc->Create();
    vbos->Append(vc);
   }
  }
  delete vertexes;
  return vbos;
 }
 VBOStaticVCs *Calculate( float timescale, bool MatchByColor ) {
  VBOStaticVCs *vbos=new VBOStaticVCs;
  ImmediateModel working;
  mPolygon *vertexes=new mPolygon;
  working.addPolygon(vertexes);
  if ( operation == ipfc_converge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else if ( operation == ipfc_diverge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
    vertexes->points.Clear();
    EACH(a->points.first,Vertex,v) {
     Vertex *v2=b->points.findByColor(v->rgba);
     float f=tween->tweenf(t/timescale);
     Vertex *w=new Vertex;
     w->x=v->x+f*(v2->x-v->x);
     w->y=v->y+f*(v2->y-v->y);
     w->z=v->z+f*(v2->z-v->z);
     w->rgba[0]=v->rgba[0]+f*(v2->rgba[0]-v->rgba[0]);
     w->rgba[1]=v->rgba[1]+f*(v2->rgba[1]-v->rgba[1]);
     w->rgba[2]=v->rgba[2]+f*(v2->rgba[2]-v->rgba[2]);
     w->rgba[3]=v->rgba[3]+f*(v2->rgba[3]-v->rgba[3]);
     vertexes->points.Append(w);
    }
    VBOStaticVC *vc=new VBOStaticVC;
    vc->fromModel(&working);
    vc->Create();
    vbos->Append(vc);
   }
  }
  delete vertexes;
  return vbos;
 }
 VBOStaticVCs *Calculate( float timescale, float colorNearness ) {
  VBOStaticVCs *vbos=new VBOStaticVCs;
  ImmediateModel working;
  mPolygon *vertexes=new mPolygon;
  working.addPolygon(vertexes);
  if ( operation == ipfc_converge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else if ( operation == ipfc_diverge ) {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
   }
  } else {
   for ( float t=0.0f; t<timescale; t+=deltaTime ) {
    vertexes->points.Clear();
    EACH(a->points.first,Vertex,v) {
     Vertex *v2=b->points.findByColor(v->rgba,colorNearness);
     float f=tween->tweenf(t/timescale);
     Vertex *w=new Vertex;
     w->x=v->x+f*(v2->x-v->x);
     w->y=v->y+f*(v2->y-v->y);
     w->z=v->z+f*(v2->z-v->z);
     w->rgba[0]=v->rgba[0]+f*(v2->rgba[0]-v->rgba[0]);
     w->rgba[1]=v->rgba[1]+f*(v2->rgba[1]-v->rgba[1]);
     w->rgba[2]=v->rgba[2]+f*(v2->rgba[2]-v->rgba[2]);
     w->rgba[3]=v->rgba[3]+f*(v2->rgba[3]-v->rgba[3]);
     vertexes->points.Append(w);
    }
    VBOStaticVC *vc=new VBOStaticVC;
    vc->fromModel(&working);
    vc->Create();
    vbos->Append(vc);
   }
  }
  delete vertexes;
  return vbos;
 }
};

class InterpolatedPointFilmClips : public LinkedList {
public:
 CLEARLISTRESET(InterpolatedPointFilmClip);
};

extern InterpolatedPointFilmClips ipflibrary;