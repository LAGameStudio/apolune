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
#include "ListItem.h"
#include "Art.h"
#include "Crayon.h"
#include "DisplayList.h"

#include "Emitter2d.h"

#define PARTICLECOLLECTION(c,t) class c : public LinkedList { public: c *Duplicate() { c *s=new c; EACH(first,t,e) { t *n=e->Duplicate(); s->Append(n); } return s; } void SetOffsets( Cartesiand *offset ) { EACH(first,t,e) e->offset=offset; } CLEARLISTRESET(t); ~c() { Clear(); } };
#define PARTICLECOLLECTIONi(c,t) class c : public LinkedList { public: c *Duplicate() { c *s=new c; EACH(first,t,e) { t *n=e->Duplicate(); s->Append(n); } return s; } void SetOffsets( Cartesian *offset ) { EACH(first,t,e) e->offset=offset; } CLEARLISTRESET(t); ~c() { Clear(); } };
#define CPUPARTICLESYSTEMLIST(ts,t,list) class ts : public LinkedList { public: CLEARLISTRESET(t); void Init() { FOREACH(t,ps) ps->Init(); } void Render( GLWindow *surface ) { FOREACH(t,g) g->Render(surface); } void Between() { FOREACH(t,ps) ps->Between(); } ~ts() { Clear(); } };
#define DYINGCPUPARTICLESYSTEMLIST(ts,t,list) \
 class ts : public LinkedList {\
  public: CLEARLISTRESET(t);\
  void Init() { FOREACH(t,ps) ps->Init(); }\
  void Render( GLWindow *surface ) { FOREACH(t,g) g->Render(surface); }\
  void Between() { t *n; for ( t *g=(t *)first; g; g=n ) { n=(t *) g->next; if ( g->list.count == 0 ) Delete(g); else g->Between(); } }\
  ~ts() { Clear(); }\
 };
#define PRECOMPUTEDPARTICLESYSTEMMOVIE(h,c,ts,t,ps,ps2) \
 Cartesiands offsets;\
 int current,frames;\
 class h : public ListItem { public: ts *e; ~h() { delete e; } };\
 class c : public LinkedList {\
 public:\
 void Add(ts *e) {\
  h *n=new h;\
  n->e=e;\
  Append(n);\
 }\
 CLEARLISTRESET(h);\
 ~c() { Clear(); }\
 }; c movie;\
 void Render(GLWindow *surface) {\
  if ( current>=frames ) return; h *e = (h *) movie.Element(current);\
  EACH(e->e->first,t,f) f->Render(surface);\
 }\
 void MoveTo( int x, int y ) { EACH(offsets.first,Cartesiand,e) { e->x=x; e->y=y; } }\
 void Between() { current++; }\
 void Rewind() { current=0; }\
 void Prerender() {\
  for ( int i=0; i<frames; i++ ) {\
   Cartesiand *offset = new Cartesiand;\
   offset->x=-1000; offset->y=-1000;\
   offsets.Append(offset);\
   ts *e=ps2.Duplicate();\
   e->SetOffsets(offset);\
   movie.Add(e);\
   ps.Between();\
  }\
 }\
 void Record( GLWindow *surface, DisplayLists *lists ) {\
  for ( int i=0; i<frames; i++ ) {\
   DisplayList *dl=new DisplayList;\
   ps.Between();\
   dl->StartRecording();\
   ps.Render(surface);\
   dl->StopRecording();\
   lists->Append(dl);\
  }\
 }
#define PRECOMPUTEDPARTICLESYSTEMMOVIEi(h,c,ts,t,ps,ps2) \
 Cartesians offsets;\
 int current,frames;\
 class h : public ListItem { public: ts *e; ~h() { delete e; } };\
 class c : public LinkedList {\
 public:\
 void Add(ts *e) {\
  h *n=new h;\
  n->e=e;\
  Append(n);\
 }\
 CLEARLISTRESET(h);\
 ~c() { Clear(); }\
 }; c movie;\
 void Render(GLWindow *surface) {\
  if ( current>=frames ) return; h *e = (h *) movie.Element(current);\
  EACH(e->e->first,t,f) f->Render(surface);\
 }\
 void MoveTo( int x, int y ) { EACH(offsets.first,Cartesian,e) { e->x=x; e->y=y; } }\
 void Between() { current++; }\
 void Rewind() { current=0; }\
 void Prerender() {\
  for ( int i=0; i<frames; i++ ) {\
   Cartesian *offset = new Cartesian;\
   offset->x=-1000; offset->y=-1000;\
   offsets.Append(offset);\
   ts *e=ps2.Duplicate();\
   e->SetOffsets(offset);\
   movie.Add(e);\
   ps.Between();\
  }\
 }\
 void Record( GLWindow *surface, DisplayLists *lists ) {\
  for ( int i=0; i<frames; i++ ) {\
   DisplayList *dl=new DisplayList;\
   ps.Between();\
   dl->StartRecording();\
   ps.Render(surface);\
   dl->StopRecording();\
   lists->Append(dl);\
  }\
 }
#define PRECOMPUTEDPARTICLESYSTEMMOVIEf(h,c) \
 Cartesians offsets;\
 float duration,expired;\
 int current,frames; c movie;\
 class h : public ListItem { public: ts *e; ~h() { delete e; } };\
 class c : public LinkedList {\
 public:\
 void Add(ts *e) { h *n=new h; n->e=e; Append(n); } CLEARLISTRESET(h); ~c() { Clear(); } };\
 void Render(GLWindow *surface) {\
  if ( current>=frames ) return;\
  h *e = (h *) movie.Element(current);\
  EACH(e->e->first,t,f) f->Render(surface);\
 }\
 void MoveTo( int x, int y ) { EACH(movie.first,h,e) EACH(e->e->first,t,f) f->MoveTo(x,y); }\
 void Rewind() { expired=0.0f; }\
 void Between() { current=(int)(expired/duration*(float) frames); expired+=1.0/FPS; }\
 void Prerender() {\
  for ( int i=0; i<frames; i++ ) {\
   Cartesian *offset = new Cartesian;\
   offset->x=-1000; offset->y=-1000;\
   offsets.Append(offset);\
   ts *e=ps2.Duplicate();\
   e->SetOffsets(offset);\
   movie.Add(e);\
   ps.Between();\
  }\
 }\
 void Record( GLWindow *surface, DisplayLists *lists ) {\
  for ( int i=0; i<frames; i++ ) {\
   DisplayList *dl=new DisplayList;\
   ps.Between();\
   dl->StartRecording();\
   ps.Render(surface);\
   dl->StopRecording();\
   lists->Append(dl);\
  }\
 }
#define PRECOMPUTEDPARTICLESYSTEMCOLLECTION(c,t) class c :\
 public LinkedList { public:\
 t *findOldest() {\
  t *candidate=(t *) first; FOREACH(t,p) if ( p->current > candidate->current ) candidate=p;\
  return candidate;\
 }\
 void PlayAt( int x, int y ) { t *candidate=findOldest(); candidate->Rewind(); candidate->MoveTo(x,y); }\
 void Between() { FOREACH(t,p) p->Between(); }\
 void Render(GLWindow *surface) { FOREACH(t,p) p->Render(surface); }\
 CLEARLISTRESET(t); ~c() { Clear(); } };

/*
 * Specialized class optimized for 2d Particle systems. (float)
 */

class Particlef2d : public ListItem
{
public:
 bool dead;
 virtual void Init(void) {}
 virtual void Between(void) {}
 virtual void Render( GLWindow *surface ) {}
 Particlef2d(void) : ListItem() { dead=false; }
};

/*
 * Specialized class optimized for 2d Particle systems. (int)
 */

class Particlei2d : public ListItem
{
public:
 bool dead;
 virtual void Init() { dead=false; }
 virtual void Between(void) {}
 virtual void Render( GLWindow *surface ) {}
 Particlei2d(void) : ListItem() { dead=false; }
};