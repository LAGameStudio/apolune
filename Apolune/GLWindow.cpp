
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "GLWindow.h"

#include "Graphics.h"
#include "UglyGlyphs.h"

GLWindowManager windows;

void GLWindowChildren::Visible( bool OnVisible ) {
 FOREACH(GLWindow,p) { p->visible=true; if ( OnVisible ) p->OnVisible(); }
}

void GLWindowChildren::Invisible( bool OnInvisible ) {
 FOREACH(GLWindow,p) { p->visible=false; if ( OnInvisible ) p->OnInvisible(); }
}

void GLWindowChildren::Only( GLWindow *visible, bool OnVisible, bool OnInvisible ) {
 FOREACH(GLWindow,p) { p->visible=false; if ( OnVisible ) p->OnInvisible(); }
 visible->visible=true;
 if ( OnVisible ) visible->OnVisible();
}

void GLWindowChildren::Clear(void) {
 EACHN(first,GLWindow,p,{
  Remove(p);
  delete p;
 });
 first=last=null;
 count=0;
}

void GLWindowChildren::Drop(GLWindow *child) {
 if (this->ListIteminList(child)) { Remove(child); delete child; } else {
  OUTPUT("GLWindowChildren child not in list!\n");
 }
}

void GLWindowChildren::RemoveDestroyed() {
 EACHN(first,GLWindow,child,{
  if ( child->destroyMe ) {
   Remove(child);
   delete child;
   OUTPUT("GLWindowChildren: Deleted a child window, %d remaining\n",count.value);
  }
 });
}


GLWindow::GLWindow(void)
{
 this->InitGLWindow();
}

GLWindow::GLWindow( Display *d, Interface *i )
{
 this->display = d;
 this->input = i;
 this->InitGLWindow();
}

GLWindow::~GLWindow(void)
{
 this->DeinitGLWindow();
}

void GLWindow::InitGLWindow(void)
{
 backgroundBlend=transparency;
 visible=true;
 cursor=true;
 clip=true;
 display=displayPointer;
 input=inputPointer;
}

extern UglyTextCache uglyCachedText;

void GLWindow::DeinitGLWindow(void)
{
 if ( !unloaded ) {
  OnUnload();
  unloaded=true;
 }
 EACHN(children.first,GLWindow,f,{
  DeleteChild(f); // calls _OnUnload in the child
 });
 children.first=children.last=null;
 children.count=0;
 uglyCachedText.ClearCache();
}

void GLWindow::MoveTo( int xx, int yy ) {
 SetPositionAndSize(xx,yy,this->w,this->h);
}

void GLWindow::MoveBy( int xx, int yy ) {
 SetPositionAndSize(this->x+xx,this->y+yy,this->w,this->h);
}

/* Window sizing and related calculations */

void GLWindow::SetPositionAndSize( int xx, int yy, int ww, int hh ) 
{
 x=xx; 
 y=yy; 
 w=ww; w2=w/2;
 h=hh; h2=h/2;
 x2=xx+ww; 
 y2=yy+hh; 
 Clip();
 //this->_OnResize( ww, hh );
}

void GLWindow::VCenter() {
 x=(int)vg.resolution.x/2-w/2;
 y=(int)vg.resolution.y/2-h/2;
 x2=x+w;
 y2=y+h;
 Clip();
}
void GLWindow::Clip(void) 
{
 clipx=x;
 clipy=displayPointer->h-(y2)-1;
 clipw=w;
 cliph=h;
}

void GLWindow::Anchor( float fromLeft, float fromRight, float fromTop, float fromBottom )
{
 float ww=display->wf;
 float hh=display->hf;
 x =(int) (ww * fromLeft);
 y =(int) (hh * fromTop);
 x2=display->w-(int) (ww * fromRight);
 y2=display->h-(int) (hh * fromBottom);
 w=x2-x; w2=w/2;
 h=y2-y; h2=h/2;
 Clip();
 //this->_OnResize( w, h );
}

void GLWindow::AnchorParent( float fromLeft, float fromRight, float fromTop, float fromBottom )
{
 float ww=(float) parent->w;
 float hh=(float) parent->h;
 x =(int) (ww * fromLeft);
 y =(int) (hh * fromTop);
 x2=display->w-(int) (ww * fromRight);
 y2=display->h-(int) (hh * fromBottom);
 w=x2-x; w2=w/2;
 h=y2-y; h2=h/2;
 Clip();
 //this->_OnResize( w, h );
}

void GLWindow::VAnchor( float fromLeft, float fromRight, float fromTop, float fromBottom )
{
 float ww=(float) vg.resolution.x;
 float hh=(float) vg.resolution.y;
 x =(int) (ww * fromLeft);
 y =(int) (hh * fromTop);
 x2=display->w-(int) (ww * fromRight);
 y2=display->h-(int) (hh * fromBottom);
 w=x2-x; w2=w/2;
 h=y2-y; h2=h/2;
 Clip();
 //this->_OnResize( w, h );
}

void GLWindow::Resized( int ww, int hh ) 
{
// GLWindow *win;
 w=ww; w2=w/2;
 h=hh; h2=h/2;
 x2=x+ww;
 y2=y+hh;
 Clip();
 //this->_OnResize( ww, hh );
}

void GLWindow::OnResize( int nw, int nh ) 
{
}

/* Window sizing and related calculations */

void GLWindow::AddChild( GLWindow *child ) 
{
 children.Prepend(child);
 child->parent=this;
 child->display=display;
 child->manager=manager;
 child->input=input;
 child->_OnLoad();
}


void GLWindow::AddChild( GLWindow *child, bool onload ) 
{
 children.Prepend(child);
 child->parent=this;
 child->display=display;
 child->manager=manager;
 child->input=input;
 if ( onload ) child->_OnLoad();
}


void GLWindow::AppendChild( GLWindow *child ) 
{
 children.Append(child);
 child->parent=this;
 child->display=display;
 child->manager=manager;
 child->input=input;
 child->_OnLoad();
}


void GLWindow::AppendChild( GLWindow *child, bool onload ) 
{
 children.Append(child);
 child->parent=this;
 child->display=display;
 child->manager=manager;
 child->input=input;
 if ( onload ) child->_OnLoad();
}

void GLWindow::RemoveChild( GLWindow *child ) 
{
 child->_OnUnload();
 children.Remove(child);
 child->parent=null;
}

void GLWindow::DeleteChild( GLWindow *child ) 
{
 child->_OnUnload();
 children.Remove(child);
 child->parent=null;
 delete child;
}

void GLWindow::DropChildren() {
 EACHN(children.first,GLWindow,child,{
  child->_OnUnload();
  children.Remove(child);
  child->parent=null;
  delete child;
 });
}


// Interface and various programmable triggered functions

void GLWindow::Init() {}
void GLWindow::OnLoad() {}
void GLWindow::Setup() {}
void GLWindow::Customize() {}
void GLWindow::OnUnload() {}
void GLWindow::MouseMove() {}
void GLWindow::MouseOver() {}
void GLWindow::LeftDown() {}
void GLWindow::RightDown() {}
void GLWindow::LeftUp() {}
void GLWindow::RightUp() {}
void GLWindow::MouseEnter() {}
void GLWindow::MouseLeave() {}
void GLWindow::Focused() {}
void GLWindow::Unfocused() {}
void GLWindow::MiddleUp() {}
void GLWindow::MiddleDown() {}
void GLWindow::WheelUp() {}
void GLWindow::WheelDown() {}
void GLWindow::LeftDouble() {}
void GLWindow::RightDouble() {}
void GLWindow::MiddleDouble() {}
void GLWindow::KeyUp( KeyList *keys ) {}
void GLWindow::KeyDown( KeyList *keys ) {}
void GLWindow::BetweenFrames() {}
void GLWindow::Between(void){}
void GLWindow::Render(void){}
void GLWindow::RenderAfter(void) {} // Called after children are rendered

void GLWindow::_MouseMove() {
 CHILDRENW(_MouseMove);
 if ( manager && !manager->stopDrill ) MouseMove();
}
void GLWindow::_MouseOver()    { CHILDRENW(_MouseOver);    if ( manager && !manager->stopDrill ) MouseOver();    }
void GLWindow::_LeftDown()     { CHILDRENW(_LeftDown);     if ( manager && !manager->stopDrill ) LeftDown();     }
void GLWindow::_RightDown()    { CHILDRENW(_RightDown);    if ( manager && !manager->stopDrill ) RightDown();    }
void GLWindow::_LeftUp()       { CHILDRENW(_LeftUp);       if ( manager && !manager->stopDrill ) LeftUp();       }
void GLWindow::_RightUp()      { CHILDRENW(_RightUp);      if ( manager && !manager->stopDrill ) RightUp();      }
void GLWindow::_MouseEnter()   { CHILDRENW(_MouseEnter);   if ( manager && !manager->stopDrill ) MouseEnter();   }
void GLWindow::_MouseLeave()   { CHILDREN(_MouseLeave);    MouseLeave();   }
void GLWindow::_Unfocused()    { CHILDREN(_Unfocused);     Unfocused();    }
void GLWindow::_Focused()      { CHILDREN(_Focused);       Focused();      }
void GLWindow::_MiddleUp()     { CHILDRENW(_MiddleUp);     if ( manager && !manager->stopDrill ) MiddleUp();     }
void GLWindow::_MiddleDown()   { CHILDRENW(_MiddleDown);   if ( manager && !manager->stopDrill ) MiddleDown();   }
void GLWindow::_WheelUp()      { CHILDRENW(_WheelUp);      if ( manager && !manager->stopDrill ) WheelUp();      }
void GLWindow::_WheelDown()    { CHILDRENW(_WheelDown);    if ( manager && !manager->stopDrill ) WheelDown();    }
void GLWindow::_LeftDouble()   { CHILDRENW(_LeftDouble);   if ( manager && !manager->stopDrill ) LeftDouble();   }
void GLWindow::_RightDouble()  { CHILDRENW(_RightDouble);  if ( manager && !manager->stopDrill ) RightDouble();  }
void GLWindow::_MiddleDouble() { CHILDRENW(_MiddleDouble); if ( manager && !manager->stopDrill ) MiddleDouble(); }

// GLWindowManager class

#pragma warning( push )
#pragma warning( disable : 4101 )
GLWindowManager::GLWindowManager(void) {
 GLWindowManagerTimer *t; NEW(t,GLWindowManagerTimer);
 if (!t) return;
 timers.AddTimer( t );
 t->_windows=this;
 t->Start();
}

GLWindowManager::GLWindowManager( Display *d ) {
 display=d;
 GLWindowManagerTimer *t; NEW(t,GLWindowManagerTimer);
 if (!t) return;
 t->expires=17;
 timers.AddTimer( t );
 t->_windows=this;
 t->Start();
}
#pragma warning( pop )

GLWindow *GLWindowManager::findTop( bool stayOnTop ) {
 if ( stayOnTop ) {
  for ( GLWindow *w=(GLWindow *) last; w; w=(GLWindow *) (w->prev) )
   if ( w->stayOnTop!=stayOnTop ) return w;
  return null;
 } else return (GLWindow *) last;
}

#include "pThread.h"

GLWindow *GLWindowManager::Add( GLWindow *win, bool thread_lock ) {
 OUTPUT("GLWindowManager adding new window: %s\n",win->identity().c_str());
 if ( thread_lock ) pthreads.Lock();
 GLWindow *w;
 win->manager=this;
 win->display=display;
 win->input=input;
 win->_OnLoad();
 if ( !win->stayOnTop ) {
  w=findTop(true);
  if ( !w ) {
   Append(win);
   return win;
  }
  // Insert before w
  InsertAfter(win,w);
  return win;
 } else Append(win);
 if ( thread_lock ) pthreads.Unlock();
 return win;
}

void GLWindowManager::BringToFront( GLWindow *win ) {
 GLWindow *w;
 for ( w=(GLWindow *) first; w; w=(GLWindow *) (w->next) ) if ( w==win ) break;
 if ( !w ) {
  OUTPUT( "GLWindowManager::BringToFront called on window that is not in the attached window list.\n" );
  return;
 } else Remove(w);
 if ( win->stayOnTop ) { Append(win); return; }
 for ( w=(GLWindow *) last; w; w=(GLWindow *) (w->prev) ) if ( !w->stayOnTop ) break;
 if ( !w ) {
  Append(win);
  return;
 }
 // Insert before w
 InsertAfter(win,w);
}

void GLWindowManager::SendToBack( GLWindow *win ) {
 GLWindow *w;
 Remove(win);
 if ( win->stayOnTop ) {
  for ( w=(GLWindow *) first; w; w=(GLWindow *) (w->next) ) if ( w->stayOnTop ) break;
  if ( !w ) {
   Prepend(win);
   return;
  }
  InsertBefore(win,w);
 } else
 Prepend(win);
}

inline void GLWindowManager::RenderWindow(GLWindow *win, bool *DC) {
 
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::win->RenderWindow()" );
 glReportError( glGetError() );
#endif

 if ( win->buffer.created ) {
  win->buffer.Bind();
 } else {
  // Limit drawing
  scissors.Push(win->clipx,win->clipy,win->clipw,win->cliph);
 }

 // Draw window background
 glEnable(GL_BLEND);
 Blending(win->backgroundBlend);
 glColor4fv(win->background.floats);
 glRecti(win->x,win->y,win->x2+1,win->y2+1); // equivalent to Area(win,win->background,0,0,win->w,win->h+1);
 glDisable(GL_BLEND);

 glPushMatrix();
 glPushAttrib(GL_ALL_ATTRIB_BITS);

#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::win->RenderWindow()" );
 glReportError( glGetError() );
#endif
 win->_Render();
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::win->RenderWindow() _Render()" );
 glReportError( glGetError() );
#endif
 if ( !win->cursor && WITHIN( input->mxi,input->myi, win->x,win->y,win->x2,win->y2 ) )
  *DC=false;
#if defined(GL_DEBUG)
 GL_Assert( "[exiting GLWindowManager::win->RenderWindow()]" );
 glReportError( glGetError() );
#endif

 glPopMatrix();
 glPopAttrib();
 
 if ( !win->buffer.created ) {
  scissors.Pop();
 }

 GL_Report( "GLWindowManager::Render() (Post RenderAfterWindow pre-loop)" );

#if defined(GRANULAR_DEBUG)
 int millis;
 DWORD renderTime;
 int window=0;
#endif
  EACH(win->children.first,GLWindow,child) {
   if ( child->visible ) {
#if defined(GRANULAR_DEBUG)
  window++;
  renderTime=timeGetTime();
#endif
    RenderWindow(child,DC);
    /* if ( child->post ) */ RenderAfterWindow(child);
#if defined(GRANULAR_DEBUG)
  renderTime=timeGetTime()-renderTime;
  millis=(int)renderTime;
  string me=win->identity();
  if ( input->KeyDown(DX_PERIOD) ) OUTPUT("Rendered child %d:%s in %d ms\n", window, me.c_str(), millis ); 
#endif
  }
 }

 if ( win->buffer.created ) {
  win->buffer.Unbind();
  win->Finish();
 } else {
 }

 GL_Report( "GLWindowManager::Render() (Post RenderAfterWindow post-loop)" );

}

inline void GLWindow::Finish() {
    buffer.RenderUpsideDown(x,y,x2,y2);
}

// Render After Children
inline void GLWindowManager::RenderAfterWindow(GLWindow *win) {
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::win->RenderAfterWindow() Pre" );
 glReportError( glGetError() );
#endif

 if ( win->buffer.created ) {
  win->buffer.BindWithoutClearing();
 } else {
  // Limit drawing
  scissors.Push(win->clipx,win->clipy,win->clipw,win->cliph);
 }

  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::win->RenderAfterWindow()" );
 glReportError( glGetError() );
#endif

  win->RenderAfter();

#if defined(GL_DEBUG)
 GL_Assert( "[exiting GLWindowManager::win->RenderAfterWindow() RenderAfter()]" );
 glReportError( glGetError() );
#endif

  glPopMatrix();
  glPopAttrib();
  
 if ( !win->buffer.created ) {
  scissors.Pop();
 } else {
  win->buffer.Unbind();
  win->Finish();
 }

#if defined(GL_DEBUG)
 GL_Assert( "[exiting GLWindowManager::win->RenderAfterWindow() Post]" );
 glReportError( glGetError() );
#endif
}

void GLWindowManager::Render(void)
{
#if defined(GRANULAR_DEBUG)
 ZIndexed<DWORD> times=new DWORD[count];
#endif
 bool DrawCursor=true;
 GLWindow *win;
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::Render() Pre" );
 GLint err = glGetError();
 if ( err != GL_NO_ERROR ) glReportError(err);
#endif
 glPush();

 glViewport(0,0,display->w,display->h);
 glOrtho(0,display->w,display->h,0,0.0,1.0);
 // Clear the screen
 scissors.Push(Zeroi,Zeroi,display->w,display->h);
 glClearColor(0.9f,0.4f,0.1f,0.0f);
 glClear(GL_COLOR_BUFFER_BIT);
 scissors.Pop();
 glDisable(GL_DEPTH_TEST);
#if defined(GL_DEBUG)
 GL_Assert( "GLWindowManager::Render()" );
 err = glGetError();
 if ( err != GL_NO_ERROR ) glReportError(err);
#endif
 
#if defined(GRANULAR_DEBUG)
 int millis;
 DWORD renderTime;
 int window=0;
#endif
 for ( win=(GLWindow *)first; win; win=(GLWindow *)(win->next) ) if ( win->visible && !win->destroyMe ) {
#if defined(GRANULAR_DEBUG)
  window++;
  renderTime=timeGetTime();
#endif
  RenderWindow(win,&DrawCursor);
  /*if ( win->post ) */ RenderAfterWindow(win);
#if defined(GRANULAR_DEBUG)
  renderTime=timeGetTime()-renderTime;
  string me=win->identity();
  millis=(int)renderTime;
  if ( input->KeyDown(DX_PERIOD) )
   OUTPUT("Rendered window%s %d:%s in %d ms\n", win->children.count>0?"+children":"", window, me.c_str(), millis ); 
#endif
 }
 GL_Report( "GLWindowManager::Render() (Post RenderAfterWindow loop)" );

 if ( DrawCursor ) {
  if ( !input->hasTouch || input->KeyDown(LCtrl) ) input->DrawGLCursor();
 }
 GL_Report( "GLWindowManager::Render(After GLCursor)" );

 glPop();

 // "Between frames"
 //for ( win=(GLWindow *)first; win; win=(GLWindow *)(win->next) ) 
 // win->_Between();
}

void GLWindowManager::Load(void) {
 GLWindow *w;
 for ( w=(GLWindow *) first; w; w=(GLWindow *) w->next ) {
  //w->OnLoad();
  w->LoadGraphics();
 }
}

void GLWindowManager::ProcessInput(void)
{
 stopDrill=false;
 int keyChange=memcmp(input->prev_key_data,input->key_data,sizeof(UCHAR[256]));
 bool mouseButton=(input->left!=input->wasLeft)||(input->right!=input->wasRight)||(input->middle!=input->wasMiddle);
 bool mouseMove=(input->x!=input->oldx) || (input->y!=input->oldy);
 bool mouseWheel=(input->z!=input->oldz);
 bool foundNewFocus=false;

 // Unfocus the focused window as we are seeking a new focus
 if ( focused && focused->visible == false ) {
  focused->_MouseLeave();
  focused->_Unfocused();
  focused->focused=false;
  focused=null;
 } 

 // If the mouse moved, determine "losing focus" and "gaining focus"
 if ( mouseMove ) {
  if ( send ) send->_MouseMove();
  if ( !solo ) {
   GLWindow *win;
   bool Stop=false;
   for ( win=(GLWindow *)last; win; win=(GLWindow *) (win->prev) ) {
    if ( win->visible && WITHIN(input->mxi,input->myi,win->x,win->y,win->x2,win->y2) ) {
     if ( send!=win && focused == win ) {
       win->_MouseMove();
       Stop=true;
     } else {
      if ( focused ) { // Switch focus to this window from the focused window
       focused->_MouseLeave();
       focused->_Unfocused();
       focused->focused=false;
       focused=win;
       win->_MouseEnter();
       win->_Focused();
       win->focused=true;
       foundNewFocus=true;
       Stop=true;
      } else { // Simply focus this window as it is the only one
       focused=win;
       win->_MouseEnter();
       win->_Focused();
       win->focused=true;
       Stop=true;
      }
     }
    }
    if ( Stop ) break;
   } // for each window backwards 
  } // !solo
 } // !mousemove

 // If a button was released, trigger appropriate stuff
 if ( !input->left && input->wasLeft ) {
  if ( send ) send->_LeftUp();
  if ( !solo && focused ) focused->_LeftUp();   
 }

 if ( !input->right && input->wasRight ) {
  if ( send ) send->_RightUp();
  if ( !solo && focused ) focused->_RightUp();
 }

 if ( !input->middle && input->wasMiddle ) {
  if ( send ) send->_MiddleUp();
  if ( !solo && focused ) focused->_MiddleUp();
 }

 // If a button was pressed, trigger appropriate stuff
 if ( input->left && !input->wasLeft ) {
  if ( send ) send->_LeftDown();
  if ( !solo && focused ) focused->_LeftDown();
 }

 if ( input->right && !input->wasRight ) {
  if ( send ) send->_RightDown();
  if ( !solo && focused ) focused->_RightDown();
 }

 if ( input->middle && !input->wasMiddle ) {
  if ( send ) send->_MiddleDown();
  if ( !solo && focused ) focused->_MiddleDown();
 }

 if ( input->wheelUp ) {
  if ( send ) send->_WheelUp();
  if ( !solo && focused ) focused->_WheelUp();
 }

 if ( input->wheelDown ) {
  if ( send ) send->_WheelDown();
  if ( !solo && focused ) focused->_WheelDown();
 }

 // Check to see if the state of the keyboard changed, use send and default
 if ( keyChange != 0 ) {
#pragma warning( push )
#pragma warning( disable : 4101 )
  KeyList *up; NEW(up, KeyList); if (!up) return;
  KeyList *down; NEW(down, KeyList); if (!down) { delete up; return; }
#pragma warning( pop )
  int i;
  for ( i=0; i<256; i++ ) {
   if ( (input->prev_key_data[i] & 0x80) && !(input->key_data[i] & 0x80) ) up->Push(i);
   else
   if ( !(input->prev_key_data[i] & 0x80) && (input->key_data[i] & 0x80) ) down->Push(i);
  }
  if ( send ) {
   if ( up->count > 0 ) send->_KeyUp( up );
   if ( down->count > 0 ) send->_KeyDown( down );
  }
  if ( !solo && focused && focused != send ) {
   if ( up->count > 0 ) focused->_KeyUp( up );
   if ( down->count > 0 ) focused->_KeyDown( down );
  }
  delete up;
  delete down;
 }
 
}

void GLWindowManager::Between(void) {
 GLWindow *w;
 for ( w=(GLWindow *) first; w; w=(GLWindow *) (w->next) ) if ( !w->destroyMe ) w->_Between();
}

void GLWindowManager::BetweenFrames(void) {
 EACHN(first,GLWindow,w,{
  if ( !w->destroyMe ) {
   EACHN(w->children.first,GLWindow,c,{
    if ( c->destroyMe ) {
     if ( focused == c ) focused=null;
     if ( send == c ) send=null;
     w->RemoveChild(c);
    	c->OnUnload();
    	c->unloaded=true;
     delete c;
     OUTPUT("GLWindowManager::BetweenFrames deleted a child of %s, %d remaining children\n",
      w->identity().c_str(),
      w->children.count.value
     );
    }
    w->_BetweenFrames();
   });
  } else {
   if ( focused == w ) focused=null;
   if ( send == w ) send=null;
   if ( w->parent != null ) {
    OUTPUT("GLWindowManager::BetweenFrames destroying window %s, child of %s, %d remaining siblings\n",
     w->parent->identity().c_str(),
     w->parent->children.count.value-1
    );
    w->parent->RemoveChild(w);
   }
   else {
    Remove(w);
    OUTPUT("GLWindowManager::BetweenFrames destroying window %s, %d remaining windows\n",
     w->identity().c_str(),
     count.value-1
    );
    w->OnUnload();
    w->unloaded=true;
    delete w;
   }
  }
 });
}

GLWindowManagerTimer::GLWindowManagerTimer() {
 expires=(int) ((1.0f/FPS)*1000.0f);
 expired=0;
 delayExpired=NULL;
 looping=true;
 this->_windows=&windows;
}

void GLWindowManager::DestroyAll() {
 FOREACH(GLWindow,w) w->destroyMe=true;
}

void GLWindowManager::Resized(void) {
 FOREACH(GLWindow,w) w->OnResize(display->w,display->h);
}

void GLWindowStack::Push() {
 Concat(&windows);
}
void GLWindowStack::Pop( bool replace ) {
 if ( replace ) windows.DestroyAll();
 windows.Concat(this);
}

void GLWindowStack::Revive( GLWindow *w ) {
 Remove(w);
 windows.Append(w);
}