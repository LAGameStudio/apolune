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
#include <iostream>
#include <string>
using namespace std;

#include "macros.h"
#include "Proce55ors.h"
#include "Audio.h"

class Presentation ;

class Slide : public ListItem {
public:
 Zp<Presentation> parent;
 Zstring name;
 Zdisposable<Proce55ors> fx, fullscreen_fx;
 Zdouble duration, remaining;
 Zbool rewind,unpause;
 Zint seek;
 SoundSequencer sounds;

 Slide(float d);
 Slide(void);

 void BetweenFrames(void);
 void Reset(bool paused);
 void Redraw(void);
 void Render(void);
 void Drop( string name );

 virtual void OnComplete();
 virtual bool OnSlideComplete( );
 virtual void PostProcess( );
 virtual void PreProcess( );

 void      OnLoad(void)  { if ( fullscreen_fx ) fullscreen_fx->OnLoad();     if ( fx ) fx->OnLoad();     }
 void     Resized(int w, int h)   { if ( fullscreen_fx ) fullscreen_fx->Resized(w,h); if ( fx ) fx->Resized(w,h);  }
 void   MouseMove(void)  { if ( fullscreen_fx ) fullscreen_fx->MouseMove();  if ( fx ) fx->MouseMove();  }
 void   MouseOver(void)  { if ( fullscreen_fx ) fullscreen_fx->MouseOver();  if ( fx ) fx->MouseOver();  }
 void    LeftDown(void)  { if ( fullscreen_fx ) fullscreen_fx->LeftDown();   if ( fx ) fx->LeftDown();   }
 void   RightDown(void)  { if ( fullscreen_fx ) fullscreen_fx->RightDown();  if ( fx ) fx->RightDown();  }
 void      LeftUp(void)  { if ( fullscreen_fx ) fullscreen_fx->LeftUp();     if ( fx ) fx->LeftUp();     }
 void     RightUp(void)  { if ( fullscreen_fx ) fullscreen_fx->RightUp();    if ( fx ) fx->RightUp();    }
 void  MouseEnter(void)  { if ( fullscreen_fx ) fullscreen_fx->MouseEnter(); if ( fx ) fx->MouseEnter(); }
 void  MouseLeave(void)  { if ( fullscreen_fx ) fullscreen_fx->MouseLeave(); if ( fx ) fx->MouseLeave(); }
 void    MiddleUp(void)  { if ( fullscreen_fx ) fullscreen_fx->MiddleUp();   if ( fx ) fx->MiddleUp();   }
 void  MiddleDown(void)  { if ( fullscreen_fx ) fullscreen_fx->MiddleDown(); if ( fx ) fx->MiddleDown(); }
 void     WheelUp(void)  { if ( fullscreen_fx ) fullscreen_fx->WheelUp();    if ( fx ) fx->WheelUp();    }
 void   WheelDown(void)  { if ( fullscreen_fx ) fullscreen_fx->WheelDown();  if ( fx ) fx->WheelDown();  }
/*
 virtual void   LeftDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._LeftDouble(mx,my,mods);   if ( fx ) fx._LeftDouble(mx,my,mods);   return true; }
 virtual void  RightDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._RightDouble(mx,my,mods);  if ( fx ) fx._RightDouble(mx,my,mods);  return true; }
 virtual void MiddleDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._MiddleDouble(mx,my,mods); if ( fx ) fx._MiddleDouble(mx,my,mods); return true; }
 */
 virtual void   KeyUp( KeyList *keys ) { if ( fullscreen_fx ) fullscreen_fx->KeyUp(keys);   if ( fx ) fx->KeyUp(keys);   }
 virtual void KeyDown( KeyList *keys ) { if ( fullscreen_fx ) fullscreen_fx->KeyDown(keys); if ( fx ) fx->KeyDown(keys); }  

};

class Slides : public LinkedList {
public:
 Zp<Presentation> parent;
// Slides(void) : LinkedList() {}
 void Add(Slide *s) { Append(s); s->parent=parent; }
 Slide *find( const char *name ) {
  FOREACH(Slide,s) if ( !str_cmp(s->name.c_str(),name) ) return s;
  return null;
 }
 // Can't use CLEARLISTRESET(Slide); here because need granular debug
 void Clear() {
  Slide *n;
  for ( Slide *p=(Slide *) first; p; p=n ) {
    n=(Slide *)(p->next);
    delete p;
  }
  first=null;
  last=null;
  count=0;
 }
 ~Slides() {
  Clear();
 }
};