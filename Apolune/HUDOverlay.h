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

#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "Presentation.h"
#include "Proce55Viewer2d.h"

extern Audio audio;
extern Interface input;

class HUDObject : public ListItem {
public:
 GLWindow *p;
 HUDObject() : ListItem() { p=null; }
};

class HUDObjects : public LinkedList {
public:
 CLEARLISTRESET(HUDObjects);
};

class HUDOverlayManager : public Proce55Viewer2d {
public:
 HUDObjects objects;
 virtual void Repopulate() {}
 virtual void PreProcessing() {}
// virtual void PostProcessing() {
//  WriteTexFont(this,microFont,crayons.Pick(red255),additive,5,5,7,16,2,"Drawing");
// }
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);
  this->duration=10.0f;
  DropAndDestroyAll();
  Repopulate();
  Init();
  EACH(children.first,GLWindow,g) g->OnLoad();
 }
 void Drop( const char *name ) {
  HUDObject *h=FindObject(name);
  if ( !h ) {
   fx.Drop(string(name));
   return;
  }
  h->p->destroyMe=true;
  objects.Remove(h);
  delete h;
 }
 void Drop( GLWindow *p ) {
  HUDObject *h=FindObject(p);
  if ( !h ) return;
  h->p->destroyMe=true;
  objects.Remove(h);
  delete h;
 }
 void Drop( HUDObject *h ) {
  if ( !h ) return;
  h->p->destroyMe=true;
  objects.Remove(h);
  delete h;
 }
 void DropAndDestroyAll() {
  HUDObject *n;
  for ( HUDObject *h=(HUDObject *) (objects.first); h; h=n ) {
   n=(HUDObject *) (h->next);
   h->p->destroyMe=true;
   objects.Remove(h);
   delete h;
  }
  fx.Clear();
 }
 void Gauge( const char *name, Proce55or *a, int xx, int yy ) {
  a->duration=10.0f;
  a->delay=0.0f;
  a->name=string(name);
  a->x=xx;
  a->y=yy;
  a->x2=a->x+a->w;
  a->y2=a->y+a->h;
  Add(a);
 }
 void Gauge( const char *name, GLWindow *g ) {
  HUDObject *h=new HUDObject;
  h->p=g;
  g->Name(name);
  objects.Append(h);
  AddChild(g,false);
 }
 void Visible( const char *name, bool b ) {
  HUDObject *f=FindObject( name );
  if ( f ) f->p->visible=b;
  else {
   Proce55or *p=FindGauge( name );
   if ( p ) p->hidden=!b;
  }
 }
 Proce55or *FindGauge( const char *name ) {
  return fx.find(name);
 }
 HUDObject *FindObject( const char *name ) {
  for ( HUDObject *h=(HUDObject *) objects.first; h; h=(HUDObject *)(h->next) ) {
   if ( !str_cmp( name, h->p->name.c_str() ) ) return h;
  }
  return null;
 }
 HUDObject *FindObject( GLWindow *p ) {
  for ( HUDObject *h=(HUDObject *) objects.first; h; h=(HUDObject *)(h->next) ) {
   if ( h->p==p ) return h;
  }
  return null;
 }
};
