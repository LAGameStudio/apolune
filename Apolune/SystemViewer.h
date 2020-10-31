/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once
#include "moremath.h"
#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "fx_MixedBag.h"
#include "fx_TexFont.h"
#include "fx_Buttons.h"
#include "fx_UglyText.h"

#include "Presentation.h"

#include "Universe.h"
#include "SystemViewer.h"

extern GLWindowManager windows;

class QuadrantViewer;

/* Moved 
class CoursePlotButton : public fx_CoolButton {
public:
 SolarSystem *system;
 void Customize() {
  DoNotShowBackingNormal=true;
  normal   = crayons.Pick(chocolate);
  t_normal = crayons.Pick(alabaster); 
  f_normal = crayons.Pick(chocolate);
  click    = crayons.Pick(red255);
  t_click  = crayons.Pick(alabaster);
  f_click  = crayons.Pick(red255);
  hover    = crayons.Pick(green);
  t_hover  = crayons.Pick(alabaster);
  f_hover  = crayons.Pick(green);
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  glow_c  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
  delay=0.0f;
  duration=600.0f;
  sprintf_s( sound_hover, "data/sounds/UI/bonk.wav" );
  sprintf_s( sound_click, "data/sounds/UI/bleep.wav" );
  this->w=32;
  this->h=32;
  this->buttontext=library.find("data/images/icons/dagobert83_apply.png");
 }
 void WhenClicked();
};
*/

class SystemViewer : public Presentation {
public:
 SolarSystem *s;
 QuadrantViewer *quadrantViewer;
 int lx,ly,diffx,diffy;
 bool dragging;
 fx_UglyTextBox *title;
 fx_TexFont *caption;

 void OnLoad() {
  dragging=false;
 }
 void RightDown();
 void LeftDown() {
//  cx=x; cy=y;
//  dragging=true;  
 }
 void MouseMove();
 void MouseLeave() {
  //quadrantViewer.sunScreen.visible=false;
  dragging=false;
 }
 void LeftUp() {
//  dragging=false;
//  diffx=diffy=0;
 }
};

class SystemViewerHandle : public ListItem {
public:
 SystemViewer *a;
};

class SystemViewers : public LinkedList {
public:
 void Add( SystemViewer *s ) {
  SystemViewerHandle *a=new SystemViewerHandle;
  a->a=s;
  Append(a);
 }
 void Drop( SystemViewer *s ) {
  SystemViewerHandle *a,*n;
  for ( a=(SystemViewerHandle *) first; a; a= n ) {
   n=(SystemViewerHandle *) a->next;
   if ( a->a == s ) {
    Remove(a);
    a->a->destroyMe=true;
    delete a;
    return;
   }
  }
 }
 bool inList( SolarSystem *d ) {
  SystemViewerHandle *a;
  for ( a=(SystemViewerHandle *) first; a; a= (SystemViewerHandle *) a->next )
   if ( a->a->s == d ) return true;
  return false;
 }
 CLEARLISTRESET(SystemViewerHandle);
 ~SystemViewers() {
  Clear();
 }
};