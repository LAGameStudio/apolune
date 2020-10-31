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

#include "Proce55ors.h"

class Proce55orGroup : public ListItem {
public:
 Proce55ors fx;
 string name;
 Proce55orGroup() : ListItem() { name=string(""); }
 Proce55orGroup( string n ) : ListItem() { name=n; }

 // Needs a parent window provided
 virtual void Add( Proce55or *p, GLWindow *pp ) { fx.Add(p,pp); }
};

class Proce55orGroups : public LinkedList {
public:
 CLEARLISTRESET(Proce55orGroup);
 ~Proce55orGroups() { Clear(); }
 
 // Finds first, not all
 Proce55or *find( const char *name ) {
  Proce55or *found=null;
  EACH(first,Proce55orGroup,p) {
   found=p->fx.find(name);
   if ( found ) return found;
  }
  return null;
 }
 
 // Finds first, not all
 Proce55or *pointTest( int x, int y ) {
  EACH(first,Proce55orGroup,p) {
   Proce55or *q=p->fx.pointTest(x,y);
   if ( q ) return q;
  }
  return null;
 }

 // Finds first, not all
 Proce55or *boxTest( int x, int y, int x2, int y2 ) {
  EACH(first,Proce55orGroup,p) {
   Proce55or *q=p->fx.boxTest(x,y,x2,y2);
   if ( q ) return q;
  }
 }

 void MoveBy( float x, float y ) {
  EACH(first,Proce55orGroup,p) p->fx.MoveBy(x,y);
 }
 void MoveBy( int x, int y ) {
  EACH(first,Proce55orGroup,p) p->fx.MoveBy(x,y);
 }

 void Go() {
  EACH(first,Proce55orGroup,p) p->fx.Go();
 }

 void Setup(void)           { EACH(first,Proce55orGroup,p) p->fx.Setup();      }
 void Reset( bool paused )  { EACH(first,Proce55orGroup,p) p->fx.Reset(paused);    }
 void Between(void)         { EACH(first,Proce55orGroup,p) p->fx.Between();    } // subroutine calls Between() when appropriate
 void Between( int x, int y, int x2, int y2 ) {
  EACH(first,Proce55orGroup,p) p->fx.Between( x, y, x2, y2 );
 }
 void Between( GLWindow *t ) {
  EACH(first,Proce55orGroup,p) p->fx.Between();
 }
 void Redraw(void)          { EACH(first,Proce55orGroup,p) p->fx.Redraw();     }
 void Redraw( GLWindow *t, int x, int y, int x2, int y2 ) {
  EACH(first,Proce55orGroup,p) p->fx.Redraw(t,x,y,x2,y2);
 }
 void Redraw( GLWindow *t ) {
  EACH(first,Proce55orGroup,p) p->fx.Render(t);
 }
 void Render(void)          { EACH(first,Proce55orGroup,p) p->fx.Render();     }
 void Render( GLWindow *t, int x, int y, int x2, int y2 ) {
  EACH(first,Proce55orGroup,p) p->fx.Render(t,x,y,x2,y2);
 }
 void Render( GLWindow *t ) {
  EACH(first,Proce55orGroup,p) p->fx.Render(t);
 }

 void OnLoad(void)          { EACH(first,Proce55orGroup,p) p->fx.OnLoad();     }
 void Resized(int w, int h) { EACH(first,Proce55orGroup,p) p->fx.Resized(w,h); }
 void OnUnload(void)        { EACH(first,Proce55orGroup,p) p->fx.OnUnload();   }
 void MouseMove(void)       { EACH(first,Proce55orGroup,p) p->fx.MouseMove();  }
 void MouseOver(void)       { EACH(first,Proce55orGroup,p) p->fx.MouseOver();  }
 void LeftDown(void)        { EACH(first,Proce55orGroup,p) p->fx.LeftDown();   }
 void RightDown(void)       { EACH(first,Proce55orGroup,p) p->fx.RightDown();  }
 void LeftUp(void)          { EACH(first,Proce55orGroup,p) p->fx.LeftUp();     }
 void RightUp(void)         { EACH(first,Proce55orGroup,p) p->fx.RightUp();    }
 void MouseEnter(void)      { EACH(first,Proce55orGroup,p) p->fx.MouseEnter(); }
 void MouseLeave(void)      { EACH(first,Proce55orGroup,p) p->fx.MouseLeave(); }
 void MiddleUp(void)        { EACH(first,Proce55orGroup,p) p->fx.MiddleUp();   }
 void MiddleDown(void)      { EACH(first,Proce55orGroup,p) p->fx.MiddleDown(); }
 void WheelUp(void)         { EACH(first,Proce55orGroup,p) p->fx.WheelUp();    }
 void WheelDown(void)       { EACH(first,Proce55orGroup,p) p->fx.WheelDown();  }

 void KeyUp( KeyList *keys )   { EACH(first,Proce55orGroup,p) p->fx.KeyUp(keys);   }
 void KeyDown( KeyList *keys ) { EACH(first,Proce55orGroup,p) p->fx.KeyDown(keys); }
 
 void Drop( string name ) {
  Proce55or *p, *pnext;
  EACH(first,Proce55orGroup,q) {
   for ( p=(Proce55or *)q->fx.first; p; p=pnext ) {
    pnext=(Proce55or *) p->next;
    if ( p->name == name ) {
     Remove( p );
     delete p;
    }
   }
  }
 }

 void Debug() {
  EACH(first,Proce55orGroup,p) p->fx.Debug();
 }
};