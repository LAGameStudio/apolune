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
#include "LinkedList.h"
#include "Proce55or.h"
#include "GLWindow.h"
#include "Audio.h"
#include "Slide.h"

C_ONE(Presentation,GLWindow,{
 this->Constructor();
})
private:
 void Constructor(void);
 void InitDisplay(void);
public:
 Zp<FBOColor> fbo;
 Zpointer<Audio> audio;
 Zpointer<Presentation> next_presentation;
 Zdisposable<Slides> slides;
 Zpointer<Slide> current;
 Zdouble duration,remaining,expired; // was moved to GLWindow, then back. keep it here. this is due BetweenFrames default in Proce55or class
 Zbool paused,loop,doNotPauseWhenNotVisible,freezeLast;
 Zbyte paused_fader;
 Zint px,py,px2,py2,ppx,ppy,ppx2,ppy2;
 Zbool letterbox;
 Zint y1;//,y2; //defined in parent
 Zbool active;
 Zint debugmode;
 Zdouble timescale;
 Zint lx,ly; // for storing the last mousex, mousey
 Zint MovedX,MovedY;
 Zint looped; // how many times it has looped, when looped=1, you should recreate the presi.

 Presentation(Display *d);
 Presentation(int x,int y,int w,int h);
 Presentation(Display *d,int x,int y,int w,int h);
 Presentation(Display *d, Interface *i, Audio *a);
 ~Presentation(void);

 void Present(void);
 void Present(Display *d);
 void Present(int x,int y,int w,int h);
 void Present(Display *d,int x,int y,int w,int h);
 void Present(Display *d, Interface *i, Audio *a);
 void Deinit();

 void Init( bool start_paused );
 void Add( Slide *s ) {
  s->sounds.audio=this->audio;
  slides->Add(s);
 }

 virtual void BetweenFrames(void);

 void Render() { OnRedraw(); }
 void OnRedraw(void);
 void Pause(void);
 void LetterBox(void);
 void Rewind( bool start_paused );
 void Seek( int n );
 void Rollback( float delta );
 void Move( int movebyx, int movebyy, Slide *slideornullforall, bool updateSounds );
 
 virtual void RenderPaused() {
  if ( current ) current->Redraw();
  if ( letterbox ) {
   Area(this,background,0,0,w,y1);
   Area(this,background,0,y2,w,h);
  }
  if ( paused_fader < 255 ) paused_fader++;
  Crayon b;
  b.floats[0]=b.floats[1]=b.floats[2]=1.0f;
  b.floats[3]=((float) paused_fader/255.0f);
  Area(this,b,px, py, px2, py2 );
  Area(this,b,ppx, ppy, ppx2, ppy2 );
 }

 virtual bool OnComplete(void);
 virtual void PreProcessing(void) {}
 virtual void PostProcessing(void) {}
         void LoadGraphics(void)       { if ( current ) current->OnLoad();                 active=false; }
 virtual void Resized(int w, int h)    { if ( current ) current->Resized(w,h);             active=true;  }
 virtual void KeyDown( KeyList *keys ) { if ( current && visible ) current->KeyDown(keys); active=true;  }
 virtual void KeyUp( KeyList *keys )   { if ( current && visible ) current->KeyUp(keys);   active=true;  }
 virtual void MouseMove(void)          { if ( current && visible ) current->MouseMove();   active=true;  } /**/
 virtual void LeftDown(void)           { if ( current && visible ) current->LeftDown();    active=true;  }
 virtual void RightDown(void)          { if ( current && visible ) current->RightDown();   active=true;  }
 virtual void LeftUp(void)             { if ( current && visible ) current->LeftUp();      active=false; }
 virtual void RightUp(void)            { if ( current && visible ) current->RightUp();     active=false; }
 virtual void WheelUp(void)            { if ( current && visible ) current->WheelUp();     active=true;  }
 virtual void WheelDown(void)          { if ( current && visible ) current->WheelDown();   active=true;  }
 virtual void MiddleUp(void)           { if ( current && visible ) current->MiddleUp();    active=true;  }
 virtual void MiddleDown(void)         { if ( current && visible ) current->MiddleDown();  active=true;  }
/*
 virtual void LeftDouble()   { if ( current && visible ) current.LeftDouble();   active=true;  return true; }
 virtual void RightDouble()  { if ( current && visible ) current.RightDouble();  active=true;  return true; }
 virtual void MiddleDouble() { if ( current && visible ) current.MiddleDouble(); active=true;  return true; }
 */
 virtual void MouseEnter()            { if ( current && visible ) current->MouseEnter();   active=false; }
 virtual void MouseLeave()            { if ( current && visible ) current->MouseLeave();   active=false; }
// virtual void MouseCaptureLost()                        { if ( current && visible ) current->MouseLost();            active=false; }

MANY(Presentation,PresentationHandle,PresentationHandles,"Presentation",Presentations,{})
DONE(Presentation);

class PresentationList : public PresentationHandles {};
