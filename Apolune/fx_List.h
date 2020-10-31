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
#include "ZeroTypes.h"
#include "fx_FireAndForget.h"

template <class L, class I>
class fx_List : public FAFProce55or {
public:
 Zpointer<L> list;
 Zpointer<I> selected;
 Zpointer<I> hovering;
 Zpointer<I> start;
 Zbool wraps;
 Zbool disposeMe;
 Zint itemH,displaying,yheight,xwidth;
 Cartesian margin;
 Blends bgBlend,backingBlend,itemBorderBlend,borderBlend;
 Crayon bg,border,itemBorder,backingTint;
 Zpointer<GLImage> backing;
 Zfloat hoveringDelay;
 Zfloat selectDelay,clickDelay,nextDelay;
 Zint countFiltered;
 Zint yStartOffset;
 fx_List( int x, int y, int w, int h, int ItemHeight, int Margin ) : FAFProce55or() {
  SetExtents(x,y,w,h);
  forItems(ItemHeight,Margin);
  bg.Pick(clear);
  backingBlend=borderBlend=bgBlend=transparency;
  backingTint.Pick(alabaster);
  itemBorderBlend=additive;
  itemBorder.Float(0.2f,0.2f,0.2f,1.0f);
  border.Pick(clear);
 }
 fx_List( int x, int y, int w, int h, int ItemHeight, int MarginH, int MarginV ) : FAFProce55or() {
  SetExtents(x,y,w,h);
  forItems(ItemHeight,MarginH,MarginV);
  bg.Pick(clear);
  backingBlend=borderBlend=bgBlend=transparency;
  backingTint.Pick(alabaster);
  itemBorderBlend=additive;
  itemBorder.Float(0.2f,0.2f,0.2f,1.0f);
  border.Pick(clear);
 }
 virtual void SetList( L *list ) {
  hovering=null;
  selected=null;
  start=null;
  if ( !list ) {
   this->list=null;
   this->start=null;
   return;
  }
  this->list=list;
  this->start=(I *) list->first;
  countFiltered=0;
  EACH(list->first,I,i) if ( Filter(i) ) countFiltered++;
  OnSetList();
 }
 virtual void OnSetList() {}
 bool Done() {
  return ( disposeMe );
 }
 void forItems( int H, int m ) {
  itemH=H;
  margin.Set(m,m);
  displaying=h/itemH+1;
  xwidth=w-margin.x*2;
  yheight=itemH+margin.y*2;
 }
 void forItems( int H, int mh, int mv ) {
  itemH=H;
  margin.Set(mh,mv);
  displaying=h/itemH+1;
  xwidth=w-margin.x*2;
  yheight=itemH+margin.y*2;
 }
 Cartesian rect;
 void Render() {
  if ( hidden ) return;
  rect.SetRect(x,y,w,h);
  if ( !rect.within(input.mxi,input.myi) ) hovering=null;
  Blending(bgBlend);
  Area(bg,x,y,x2,y2);
  Blending(borderBlend);
  Rectangle(border,&rect);
  if ( backing ) QuadiNoAlpha(backing,backingTint,backingBlend,x,y,w,h);
  if ( !list || !start || list->count == 0 || countFiltered == 0 ) {
   RenderEmptyList();
   return;
  }
  I *item=start;
  while ( item && !Filter(item) ) item=(I *)  item->next;
  start=item;
  if ( !start ) {
   RenderEmptyList();
   return;
  }
  bool stop=false;
  bool hasWrapped=false;
  for ( int i=0; i<displaying && !stop; i++ ) {
   if ( !item ) break;
   int itemy=yStartOffset+yheight*i+margin.y*i*2+margin.y; rect.SetRect(x+margin.x,y+itemy,(w-margin.x*2),itemH); // Setup the region we are drawing
   bool hovered=WITHIN(input.mxi,input.myi,rect.x,rect.y,rect.x2,rect.y2); // Determine if the mouse is hovering
   if ( hovered ) {
    if ( hovering != item && hoveringDelay <= 0.0f ) {
     hovering=item;
     OnHover();
     hoveringDelay=0.1f;
    }
    if ( input.left && item != selected && selectDelay <= 0.0f ) {
     selected=item;
     OnSelect();
     selectDelay=0.1f;
    } 
    if ( input.left && !input.wasLeft && clickDelay <= 0.0f ) {
     OnClick();
     clickDelay=0.1f;
    }
   } else if ( hovering == item ) hovering=null;
   scissors.Push(rect.x,rect.y,rect.w+1,rect.h);
   RenderItem(item,&rect,(item==selected),(item==hovering));
   Blending(itemBorderBlend);
   Rectangle(itemBorder,&rect);
   scissors.Pop();
   item=(I *) item->next;
   while ( item && !Filter(item) ) item=(I *)  item->next;
   if ( !item ) {
    hasWrapped=true;
    if ( !wraps ) stop=true;
    else {
     item=(I *) list->first;
     if ( item == start || item->isAfter(start) ) { stop=true; }
     while ( item && !Filter(item) ) {
      if ( item == start ) { stop=true; break; }
      item=(I *)  item->next;
     }
     if ( item == start || item->isAfter(start) ) { stop=true; }
    }
   }
   if ( hasWrapped && item == start && !wraps ) break;
  }
 }
 void Render( GLWindow *window ) { Render(); }
 void Draw( GLWindow *window ) { Render(); }
 virtual void RenderItem( I *item, Cartesian *extents, bool isSelected, bool isHovering ) {}
 virtual void RenderEmptyList() {}
 int fromEnd() {
  if ( !list || !start ) return 0;
  int items=0;
  EACH(list->first,I,p) if ( Filter(p) ) items++;
  return items;
 }
 void Between() {
  if ( hidden ) return;
  if ( nextDelay >= 0.0f ) nextDelay-=FRAMETIME;
  if ( selectDelay >= 0.0f ) selectDelay-=FRAMETIME;
  if ( clickDelay >= 0.0f ) clickDelay-=FRAMETIME;
  if ( hoveringDelay >= 0.0f ) hoveringDelay-=FRAMETIME;
  if ( !list || list->count == 0 ) return;
  if ( !start ) {
   if ( list ) start=(I *) list->first;
   else return;
  }
  if ( input.right ) {
   Unselect();
  }
 }
 void WheelDown() { if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) ClickedNext(); }
 void WheelUp()   { if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) ClickedPrev(); }
 void ClickedNext() {
  if ( !list || nextDelay > 0.0f ) return;
  if ( list->count <= displaying ) { start=(I *)list->first; return; }
  Unselect();
  if  ( fromEnd() >= displaying ) {
   if ( start ) start=(I *) start->next;
   while ( start && !Filter(start) ) start=(I *)  start->next;
   if ( !start ) start=(I *) list->first;
   while ( start && !Filter(start) ) start=(I *)  start->next;
  }
  nextDelay=0.1f;
 }
 void ClickedPrev() {
  if ( !list || nextDelay > 0.0f ) return;
  if ( list->count <= displaying ) { start=(I *)list->first; return; }
  if ( start ) start=(I *) start->prev;
  Unselect();
  if ( !start ) {
   start=(I *) list->last;
   if ( list->count > displaying ) {
    for ( int i=0; i<displaying; i++ ) {
     if ( !start ) {
      start=(I *) list->first;
     }
     if ( !start ) return;
     start=(I *) start->prev;
     while ( start && !Filter(start) ) start=(I *)  start->prev;
    }
   }
  }
  nextDelay=0.1f;
 }
 virtual void OnSelect() { }
 virtual void OnUnselect() { }
 virtual void OnHover() { }
 virtual void OnClick() { }
 virtual bool Filter( I *item ) { return true; }
 void Unselect() {
  hovering=selected=null;
  OnUnselect();
 }
};

