/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "Indexed.h"
#include "ZeroTypes.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLSetup.h"
#include "Interface.h"
#include "HRTimer.h"
#include "Display.h"
#include "Cartesian.h"
#include "FBO.h"

//#define GRANULAR_DEBUG 1 // Activates granular debugging console output on period key (.)

extern Display display;

class GLWindowManager;

#define CHILDREN(func) { GLWindow *n; for ( GLWindow *c=(GLWindow *) children.first; c; c=(GLWindow *) c=n ) { n=(GLWindow *) c->next; if ( c->visible ) c->func(); } }
#define CHILDRENW(func) { GLWindow *n; for ( GLWindow *c=(GLWindow *) children.first; c; c=(GLWindow *) c=n ) { n=(GLWindow *) c->next; if ( c->visible && WITHIN( input->mxi,input->myi, c->x,c->y,c->x2,c->y2 ) ) c->func(); if ( manager->stopDrill ) break; } }

class GLWindow;

class GLWindowChildren : public LinkedList {
public:
 void Visible( bool OnVisible=true );
 void Invisible( bool OnInvisible=true );
 void Only( GLWindow *visible, bool OnVisible=true, bool OnInvisible=true );
 void Clear();
 void Drop(GLWindow *child);
 void RemoveDestroyed();
};

class GLWindow : public ListItem {
public:
 Zstring name,label,classDescription;
 Zint code;
 FBOColorDepth buffer;
 Zpointer<GLWindowManager> manager;
 Zpointer<GLWindow> parent, exits_to;
 GLWindowChildren children;
 Zpointer<Display> display;
 Zpointer<Interface> input;
 Zint x,y;
 Zint w,h,w2,h2;
 Zint x2,y2;
 Zint clipx,clipy,clipw,cliph;
 Crayon background;
 Blends backgroundBlend;
 Zbool visible,focused,stayOnTop;
 Zbool cursor,clip;
 Zbool unloaded,destroyMe;
 Zbool post;
 
 GLWindow(void);
	GLWindow( Display *d, Interface *i );
	~GLWindow(void);

 void InitGLWindow(void);
 void DeinitGLWindow(void);

 void AddChild( GLWindow *child );
 void AddChild( GLWindow *child, bool onload );
 void AppendChild( GLWindow *child );
 void AppendChild( GLWindow *child, bool onload );
 void RemoveChild( GLWindow *child );
 void DeleteChild( GLWindow *child );
 void DropChildren();

 void Name( const char *s ) { name=string(s); }
 void Name( string s ) { name=s; }
 void Label( const char *s ) { name=string(s); }
 void Label( string s ) { name=s; }

 virtual bool mouseWithin() {
  return WITHIN(input->mxi,input->myi,this->x,this->y,this->x2,this->y2);
 }

 void MoveTo(int xx, int yy);
 void MoveBy(int xx, int yy);

 void SetPositionAndSize( int xx, int yy, int ww, int hh );
 void Anchor( float fromLeft, float fromRight, float fromTop, float fromBottom );
 void AnchorParent( float fromLeft, float fromRight, float fromTop, float fromBottom );
 void Fullscreen() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
 }
 void VAnchor( float fromLeft, float fromRight, float fromTop, float fromBottom );
 void VFullscreen() {
  VAnchor(0.0f,0.0f,0.0f,0.0f);
 }
 void VSizeAndCenter( int ww, int hh ) {
  SetPositionAndSize( 0,0,ww,hh );
  VCenter();
 }
 void VCenter();
 void Center() {
  x=display->w/2-w/2;
  y=display->h/2-h/2;
  x2=x+w;
  y2=y+h;
  Clip();
 }
 void SizeAndCenter( int ww, int hh ) {
  SetPositionAndSize( 0,0,ww,hh );
  Center();
 }
 void Clip(void);

 void MousePosition( int *mx, int *my ) {
  *mx=input->mxi-this->x;
  *my=input->myi-this->y;
 }

 void MousePosition( Zint *mx, Zint *my ) {
  mx->value=input->mxi-this->x;
  my->value=input->myi-this->y;
 }

 void Resized( int ww, int hh );
 virtual void OnResize( int nw, int nh );

 virtual void BetweenFrames(void);
 virtual void Between(void);
 virtual void Render(void);        // "parent" window rendering
 virtual void RenderAfter(void);   // called after children are rendered
 virtual void Finish(void);        // called when buffer is active after children are rendered and after RenderAfter

 virtual void Init();
 virtual void OnLoad();
 virtual void Setup();
 virtual void Customize();

 virtual void LoadGraphics() {}
 virtual void OnUnload();
 virtual void MouseMove();
 virtual void MouseOver();
 virtual void LeftDown();
 virtual void RightDown();
 virtual void LeftUp();
 virtual void RightUp();
 virtual void MouseEnter();
 virtual void MouseLeave();
 virtual void Unfocused();
 virtual void Focused();
 virtual void MiddleUp();
 virtual void MiddleDown();
 virtual void WheelUp();
 virtual void WheelDown();
 virtual void LeftDouble();
 virtual void RightDouble();
 virtual void MiddleDouble();
 virtual void OnVisible() {}
 virtual void OnInvisible() {}
 virtual void KeyUp( KeyList *keys );
 virtual void KeyDown( KeyList *keys );

 void _OnLoad()       {
  Init();
  OnLoad();
  Setup();
  Customize();
  //if ( parent ) CHILDREN(_OnLoad); /*Called when added*/
 }
 void _LoadGraphics() { LoadGraphics(); unloaded=false; CHILDREN(_LoadGraphics); }
 void _OnUnload()     { if ( !unloaded ) OnUnload(); unloaded=true; CHILDREN(_OnUnload); }
 void _MouseMove()    ;
 void _MouseOver()    ;
 void _LeftDown()     ;
 void _RightDown()    ;
 void _LeftUp()       ;
 void _RightUp()      ;
 void _MouseEnter()   ;
 void _MouseLeave()   ;
 void _Unfocused()    ;
 void _Focused()      ;
 void _MiddleUp()     ;
 void _MiddleDown()   ;
 void _WheelUp()      ;
 void _WheelDown()    ;
 void _LeftDouble()   ;
 void _RightDouble()  ;
 void _MiddleDouble() ;
 void _KeyUp( KeyList *keys )     { KeyUp(keys);   }
 void _KeyDown( KeyList *keys )   { KeyDown(keys); }
 void _Resized( int ww, int hh )  {
  Resized(ww,hh);
 }
 void _OnResize( int nw, int nh ) {
  OnResize(nw,nh);
  GLWindow *n;
  for ( GLWindow *c=(GLWindow *) children.first; c; c=n ) {
   n=(GLWindow *) c->next;
   c->_OnResize(nw,nh);
  }
 }
 void _BetweenFrames(void) {
  BetweenFrames();
  CHILDREN(_BetweenFrames);
 }
 void _Between(void)       {
  Between();
  if ( !destroyMe ) CHILDREN(_Between);
 }
 void _Render(void)        {
  Render();
  // CHILDREN(_Render); // Exists elsewhere.
 }
 void WindowName( const char *s ) { Name(s); }
 void WindowName( string s ) { Name(s); }
};

// See GLWindowStack below this class as a way to extricate the windows list to create modals.

class GLWindowManager : public LinkedList {
public:
 Zbool stopDrill;
 Zpointer<Display> display;
 Zpointer<Interface> input;
 Zpointer<GLWindow> send;  // Redirects all input to a single window
 Zbool solo;       // Exclusively to send
 Zpointer<GLWindow> focused;

 GLWindowManager(void);
 GLWindowManager( Display *d );
 // Dont use CLEARLISTRESET for debugging and other reasons
 void Clear() {
  GLWindow *n=null;
  for ( GLWindow *p=(GLWindow *) first; p; p=n ) {
   n=(GLWindow *)(p->next);
   delete p;
  }
  first=null;
  last=null;
  count=0;
  send=null;
  focused=null;
  solo=false;
  stopDrill=false;
 }
 void Load(void);
 GLWindow *Named( const char *s ) {
  FOREACH(GLWindow,g) if ( !str_cmp(s,g->name.c_str()) ) return g;
  return null;
 }
 GLWindow *Find( const char *s ) {
  Zstring key(s);
  FOREACH(GLWindow,g) if ( g->name == key ) return g;
  return null;
 }
 GLWindow *findTop( bool stayOnTop );
 GLWindow *Add( GLWindow *win, bool thread_lock=true );
 void BringToFront( GLWindow *win );
 void SendToBack( GLWindow *win );
 void RenderWindow( GLWindow *win, bool *DC );
 void RenderAfterWindow( GLWindow *win );
 void Render(void);
 void ProcessInput(void);
 void Between(void);
 void BetweenFrames(void);
 void Resized(void);
 void DestroyAll();
 void Visible() { EACH(first,GLWindow,g) g->visible=true; }
};

extern GLWindowManager windows;

class GLWindowManagerTimer : public HRTimer
{
public:
 GLWindowManager *_windows;
 GLWindowManagerTimer();
 virtual bool DelayExpired(void) {
//  windows->BetweenFrames();
  return true;
 }
};

class gl2D
{
public:
 gl2D() {
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
  gluOrtho2D(0, viewport[2], 0, viewport[3]);
  glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
 }
 ~gl2D() {
  glMatrixMode(GL_PROJECTION); glPopMatrix();
  glMatrixMode(GL_MODELVIEW); glPopMatrix();
 }
};

HANDLED(GLWindow,GLWindowHandle,GLWindowHandles,GLWindowHandlesHandle,GLWindowHandlesHandles);

// This class lets you temporarily freeze a set of windows by holding them.
// Contents of this class are disposed at delete time, so if it is a local,
// make sure not to create a "fatal reference loop" by destroying the local
// while depending on values that are contained in or related to windows in
// this stack.
//
// The stack accumulates (using LinkedList's Concat feature, so multiple pushes
// result in a combined set of windows).
//
// Mainly use this class to do "focused modals" .. but note the windows behind the
// modal will not be drawn, do not receive input, and don't have their frame-related
// methods called.
//
// You can use Find and Named to locate windows and move them back off using Revive()
class GLWindowStack : public LinkedList {
public:
 void Clear() { // CLEARLISTRESET
  GLWindow *n=null;
  for ( GLWindow *p=(GLWindow *) first; p; p=n ) {
   n=(GLWindow *)(p->next);
   delete p;
  }
  count=0;
  first=null;
  last=null;
 }
 void Push(); // Accumulative
 void Pop(bool replace=false); // Puts everything back to regular windows manager using Concat, or by deleting active windows first.
 void Revive( GLWindow *w );
 GLWindow *Named( const char *s ) {
  FOREACH(GLWindow,g) if ( !str_cmp(s,g->name.c_str()) ) return g;
  return null;
 }
 GLWindow *Find( const char *s ) {
  Zstring key(s);
  FOREACH(GLWindow,g) if ( g->name == key ) return g;
  return null;
 }
};