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
#include "Display.h"
#include "Interface.h"

#include "TextureLibrary.h"
#include "Audio.h"
#include "Art.h"
#include "Crayon.h"
#include "Tweens.h"
#include "fx_Glow.h"
#include "fx_TexFont.h"
#include "Presentation.h"

#include "TitleMenu.h"
#include "Globals.h"

#define SHOW 1

class Introduction : public Presentation {
public:
 Introduction( Display *d, Interface *i, Audio *a ) { Present(d,i,a); }
 void RightDown() {
  if ( visible ) OnComplete();
 }
 bool OnComplete();
 void OnLoad() { 
  Slide *s;
  VisualAid *v;
//  SoundEvent *se;
//  fx_Glow *p;

  stayOnTop=true;
  cursor=false;
  background=crayons.Pick(black);

//  font = displaySystem.LoadFont("fonts/written/data-latin.ttf", 10, FontFlags { bold = true } );
  letterbox=true;
  y1=display->h/6;
  y2=display->h-y1;
  //debugmode=1;//2;

#if !defined(SHOW)
  Add( new Slide(0.0f) );

#else

  s=new Slide(3.1f);
  Add(s);

   // Startup sound
   audio->manager.CueAndPlay( "data/sounds/UI/alert.wav",1.0f,0.5f,false,true);

//  se=new SoundEvent(logo_sounds.random(),null,0,false);
//  se->delay=0.001f;
//  s->sounds.Add(se);

//  p=fx_MetaTunnel{ duration=30.0f, w=display->w, h=display->h, ofs_x=0, ofs_y=0, scale=10 };
//  p._next=s.fx; s.fx=p;  

  v = new VisualAid; // Cartoon: Lost Astronaut
  s->fx->Add(v,this);
  v->image = library.find( "data/images/logo.png" );
  if ( !v->image ) OUTPUT( "Image is null\n" );
  v->delay=0.0f;  v->in=1.0f; v->out=2.0f;  v->duration=3.0f;
  v->w=v->w2=display->w/2;
  v->h=v->h2=display->h/2;
  v->sx=display->w/4;
  v->sy=display->h/4;
  v->Opacity(0);
  //v->flicker_out=true;

#endif // SHOW

  Init(false);
 }


};


extern Zpointer<Introduction> introduction;