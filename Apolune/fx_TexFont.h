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
#include <math.h>

#include "macros.h"
#include "moremath.h"

#include "GLWindow.h"
#include "GLImage.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55Controller.h"
#include "Proce55or.h"
#include "Art.h"
#include "Audio.h"
#include "TexFont.h"
#include "VisualAid.h"
#include "TextureLibrary.h"
#include "Interface.h"
#include "CompositeImage.h"
#include "DisplayList.h"

extern Interface input;

class fx_TexFont : public Proce55or {
public:
 Zpointer<TexFont> font;
 Zstring content;
 Zint fw,fh;
 Zint len;
 Zint maxChars;
 Zdouble deltaDuration, deltaIn, deltaOut, deltaDelay;
 Crayon color;
 Zbool UseRelativeGlyphWidths;
 Zint lineSpace;
 Zint lines;
 Blends blend;
 CompositeImages i;
 Zpointer<GLWindow> target;

 fx_TexFont() : Proce55or() {
  font=calibri;
  maxChars=1024;
  color.Pick(black);
  UseRelativeGlyphWidths=true;
  lines=1;
  lineSpace=1;
  blend=additive;
 }

 fx_TexFont( TexFont *f, GLWindow *pa, char *t, double duration, int x, int y, int ffw, int ffh, Crayon c ) : Proce55or() {
  font=f;
  maxChars=128;
  lineSpace=2;
  blend=additive;
  this->duration=duration;
  deltaOut=duration;
  deltaDuration=duration;
  this->x=x;
  this->y=y;
  fw=ffw;
  fh=ffh;
  UseRelativeGlyphWidths=true;
  color.fromCrayon(c);
  lines=1;
  target=pa;
  Text(t,pa);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx; y+=byy; x2+=byx; y2+=byy;
  EACH(i.first,CompositeImage,c) {
   c->position.MoveBy(byx,byy);
  }
 }

 // Needs updated to include support for "variable width" fonts
 void Text( const char *t, GLWindow *surface ) {
  this->parent=surface;
  content=string(t);
 }

 void Centered( GLWindow *pops, char *t, int height, int width, int linespace ) {
  parent=pops;
  int total=WidthTexFont(font,(fw=width),0,t);
  fh=height;
  lineSpace=linespace;
  Text(t,pops);  
  x+=(parent->w-total)/2;
 }

 void Center( const char *t, GLWindow *surface, int width, int height, int ffw, int ffh, int linespace ) {
  int htotal=WidthTexFont(font,(fw=width),0,t);
  int vtotal=linefeeds(t);
  y+=(height-(vtotal*linespace+vtotal*ffh))/2;
  x+=(parent->w-htotal)/2;
  Text(t,surface);
  fh=ffh;
  fw=ffw;
  lineSpace=linespace;
  OUTPUT("%d", (int)x);
 }
 
 void Attach( GLWindow *surface ) {
  WriteTexFont(font,color,blend,x,y,fw,fh,lineSpace,content.c_str());
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Draw(GLWindow *surface) {
  if ( !hidden && expired>=delay && expired-delay<duration ) {
   Attach(surface);
  }
 }

 void Render() {
  if ( !hidden ) Attach(parent);
 }
 
 void Render(GLWindow *surface) {
  if ( !hidden ) Attach(surface);
 }
};


class fx_TexFontFader : public Proce55or {
public:
 Zpointer<TexFont> font;
 Zstring content;
 Zint fw,fh;
 Zint len;
 Zint maxChars;
 Zdouble expired, deltaDuration, deltaIn, deltaOut, deltaDelay;
 Crayon color;
 Zbool UseRelativeGlyphWidths;
 Zint lineSpace;
 Zint lines;
 Blends blend;
 CompositeImages i;
 Zpointer<GLWindow> target;

 fx_TexFontFader() : Proce55or() {
  font=calibri;
  maxChars=1024;
  color.Pick(black);
  UseRelativeGlyphWidths=true;
  lines=1;
  lineSpace=1;
  blend=additive;
  hidden=false;
 }

 fx_TexFontFader( TexFont *f, GLWindow *pa, char *t, float duration, int x, int y, int ffw, int ffh, Crayon c ) : Proce55or() {
  font=f;
  len=0;
  maxChars=128;
  lineSpace=2;
  blend=additive;
  hidden=false;
  this->duration=duration;
  deltaIn=0.0;
  deltaOut=duration;
  deltaDuration=duration;
  this->x=x;
  this->y=y;
  fw=ffw;
  fh=ffh;
  UseRelativeGlyphWidths=true;
  color.fromCrayon(c);
  target=pa;
  lines=1;
  expired=0.0;
  Text(t,pa);
 }
 
 virtual void MoveBy( int byx, int byy ) {
  x+=byx; y+=byy; x2+=byx; y2+=byy;
  EACH(i.first,CompositeImage,c) {
   c->position.MoveBy(byx,byy);
  }
 }

 // Needs updated to include support for "variable width" fonts
 void Text( const char *t, GLWindow *surface ) {
  this->parent=surface;
  content=t;
 }

 void Centered( GLWindow *pops, string s, int height, int width, int linespace ) {
  Centered(pops,s.c_str(),height,width,linespace);
 }
 void Centered( GLWindow *pops, const char *t, int height, int width, int linespace ) {
  parent=pops;
  int total=WidthTexFont(font,(fw=width),0,t);
  fh=height;
  lineSpace=linespace;
  Text(t,pops);  
  x+=(parent->w-total)/2;
 }

 void Center( const char *t, GLWindow *surface, int width, int height, int ffw, int ffh, int linespace ) {
  int htotal=WidthTexFont(font,(fw=width),0,t);
  int vtotal=linefeeds(t);
  y+=(height-(vtotal*linespace+vtotal*ffh))/2;
  x+=(parent->w-htotal)/2;
  Text(t,surface);
  fh=ffh;
  fw=ffw;
  lineSpace=linespace;
  // OUTPUT("%d", (int)x);
 }
 
 void Attach( GLWindow *surface ) {
  if ( expired < delay ) return;
  double instant;
  if ( expired-delay < deltaIn ) {
   instant=(expired-delay)/deltaIn;
  } else if ( (expired-delay) < deltaOut ) {
   instant=1.0;
  } else {
   instant=1.0-(expired-delay-deltaOut)/(deltaDuration-deltaOut);
  }
  Crayon drawing;
  drawing.fromCrayon(color);
  drawing.ScaleByAlphaf((float)instant);
  WriteTexFont(font,drawing,blend,x,y,fw,fh,lineSpace,content);
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Draw(GLWindow *surface) {
  if ( !hidden && expired>=delay && expired-delay<duration ) {
   Attach(surface);
  }
 }

 void Render() {
  if ( !hidden ) Attach(parent);
 }
 
 void Render(GLWindow *surface) {
  if ( !hidden ) Attach(surface);
 }
};

class fx_TexFontBox : public fx_TexFont {
public:
 Zint characters,lines;
 Zpointer<char> binding;
 Zpointer<GLImage> backing;
 Crayon color;
 Zbool focused,sticky;
 Zint cursor;
 fx_TexFontBox() : fx_TexFont() {}
 virtual void OnEnterPressed() { }
 virtual void WhenChanged() { }
 void Init() {
  characters=1024;
  lines=1;
  color.Pick(alabaster);
 }
 void DisplayText(GLWindow *surface) {
  int width=WidthTexFont(font,fw,0,content.c_str());
  int lines=linefeeds(content.c_str());
  WriteTexFont(surface,font,color,blend,x,y,fw,fh,lineSpace,content.c_str());
  if ( focused ) {
   Crayon blink;
   blink.fromCrayon(color);
   blink.ScaleByAlphaf((float) (cursor%255)/255.0f);
   BlendArea(surface,blink,additive,x+width,y,x+width+fw*2,y+fh);
  }
 }
 void Between() { 
  controller.Updatef();
  cursor++;
 }
 void Draw(GLWindow *surface) {
   if ( backing ) {
   }
   DisplayText(surface);
 }
 void KeyDown(KeyList *keys) {  
  char ch=keys->FirstCharInList();
//  printf ( "char %d = %c", (int) ch, (char) ch );
  len=(int) content.length;
  if ( !focused ) return;
//  if ( (!focused && !sticky) ) return;
  if ( keys->InList(DX_RETURN) || keys->InList(DX_TAB) ) { OnEnterPressed(); return; }
  if ((keys->InList(DX_LSHIFT) || keys->InList(DX_RSHIFT)) && keys->InList(DX_TAB) ){
 
  }
  if ( (keys->InList(DX_BACKSPACE) || keys->InList(DX_DELETE) ) && len > 0 ) {
   content--;
  } else 
  if ( ch && (int) ch <= 127 && len < characters ) content+=ch;
  if ( binding ) {
   char *P;
   const char *Q;
   P=binding;
   Q=content.c_str();
   while ( *Q != '\0' ) { *P=*Q; P++; Q++; }
   if (*Q=='\0') *P='\0';
  }
  WhenChanged();
  Text(content,target);
 }

 void MouseMove() {
  if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) { 
//   if ( !this.Broadcast(0,"") ) return; 
    SetFocused(false);
  } else MouseLeave();
 }

 void ResetFoci() {
  Proce55or *p;
  for ( p=head_of_list; p; p=p->next_in_list ) { 
   fx_TexFontBox *tb=(fx_TexFontBox *) p; 
   if ( tb->sticky || tb->focused ) tb->SetUnfocused();
  }
 }

 void SetFocused( bool stick ) {
  if ( stick ) ResetFoci();
  focused=true;
  sticky=stick;
  OnFocus();
 }

 void SetUnfocused( ) {
  focused=sticky=false;
  OnFocusLost();
 }

 void LeftDown() {
  if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) {
   ResetFoci(); 
   SetFocused(true);
  } //else { focused=false; OnFocusLost(); }
 }
};


class fx_TexFontBoxString : public fx_TexFont {
public:
 Zint characters,lines;
 Zpointer<string> binding;
 Zpointer<GLImage> backing;
 Crayon color;
 Zbool focused,sticky;
 Zint cursor;
 fx_TexFontBoxString() : fx_TexFont() {}
 virtual void OnEnterPressed() { }
 virtual void WhenChanged() { }
 void Init() {
  characters=1024;
  lines=1;
  color.Pick(alabaster);
 }
 void DisplayText(GLWindow *surface) {
  int width=WidthTexFont(font,fw,0,content);
  int lines=linefeeds(content);
  WriteTexFont(surface,font,color,blend,x,y,fw,fh,lineSpace,content);
  if ( focused ) {
   Crayon blink;
   blink.fromCrayon(color);
   blink.ScaleByAlphaf((float) (cursor%255)/255.0f);
   BlendArea(surface,blink,additive,x+width,y,x+width+fw*2,y+fh);
  }
 }
 void Between() { 
  controller.Updatef();
  cursor++;
 }
 void Draw(GLWindow *surface) {
   if ( backing ) {
   }
   DisplayText(surface);
 }
 void KeyDown(KeyList *keys) {  
  char ch=keys->FirstCharInList();
//  printf ( "char %d = %c", (int) ch, (char) ch );
  len=(int) content.length;
  if ( !focused ) return;
//  if ( (!focused && !sticky) ) return;
  if ( keys->InList(DX_RETURN) || keys->InList(DX_TAB) ) { OnEnterPressed(); return; }
  if ((keys->InList(DX_LSHIFT) || keys->InList(DX_RSHIFT)) && keys->InList(DX_TAB) ){
 
  }
  if ( (keys->InList(DX_BACKSPACE) || keys->InList(DX_DELETE) ) && len > 0 ) { 
   content--;
  } else 
  if ( ch && (int) ch <= 127 && len < characters ) content+=ch;
  if ( binding ) {
   (*binding)=content.value;
  }
  WhenChanged();
  Text(content,target);
 }

 void MouseMove() {
  if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) { 
//   if ( !this.Broadcast(0,"") ) return; 
    SetFocused(false);
  } else MouseLeave();
 }

 void ResetFoci() {
  Proce55or *p;
  for ( p=head_of_list; p; p=p->next_in_list ) { 
   fx_TexFontBox *tb=(fx_TexFontBox *) p; 
   if ( tb->sticky || tb->focused ) tb->SetUnfocused();
  }
 }

 void SetFocused( bool stick ) {
  if ( stick ) ResetFoci();
  focused=true;
  sticky=stick;
  OnFocus();
 }

 void SetUnfocused( ) {
  focused=sticky=false;
  OnFocusLost();
 }

 void LeftDown() {
  if ( WITHIN(input.mxi,input.myi,x,y,x2,y2) ) {
   ResetFoci(); 
   SetFocused(true);
  } //else { focused=false; OnFocusLost(); }
 }
};

/*******************
 * fx_NumberToggle *
 *******************/

class fx_TexNumberToggle : public fx_TexFont {
public:
 Zpointer<int> binding;
 Zint numbers;
 Zint number;
 Indexed<Zint> values;

 fx_TexNumberToggle() : fx_TexFont() {}

 void LeftDown(int mx, int my, Modifiers mods) {
  if ( !WITHIN(mx,my,x,y,x+w,y+h) ) return;
  number++; 
  if ( number >= numbers ) number=0;
  *binding=values[number];
  FORMAT( buf,1024, "%d", (int)(*binding) );
  Text(buf,target);
 }

 void RightDown(int mx, int my, Modifiers mods) {
  if ( !WITHIN(mx,my,x,y,x+w,y+h) ) return;
  number--;
  if ( number < 0 ) number=numbers;
  *binding=values[number];
  FORMAT( buf,1024, "%d", (int)(*binding) );
  Text(buf,target);
 }
 
 ~fx_TexNumberToggle() {}
};
