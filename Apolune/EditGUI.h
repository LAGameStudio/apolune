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

#include "Proce55or.h"
#include "VirtualWindow.h"
#include "FileDialog.h"
#include "TextureLibrary.h"
#include "Film.h"

class Campaign;

ONE(TempFilmReferencer,{})
 Zp<Zp<Film>> bound;
MANY(TempFilmReferencer,TempFilmReferencerHandle,TempFilmReferencerHandles,"",TempFilmReferencers,{})
 TempFilmReferencer *Find( Zp<Film> *f ) { 
  FOREACH(TempFilmReferencer,tfr) if ( tfr->bound == f ) return tfr;
  return null;
 }
DONE(TempFilmReferencer);

class ImageCartesiansEditor;
class SoundEffect;
class SoundEffectEditor;
class Mulligan;
class MulliganEditor;

ONE(FastImageDialog,{
 fd.background.x_DarkSlateBlue();
 fd.bound=&s.value;
 w=64;
 h=64;
 bh=16;
 xToNull=true;
})
 Zint w,h,bh;
 Zp<Zp<GLImage>> bound;
 FileDialog fd;
 Zstring s,label;
 Zbool xToNull;
 void Bind( Zp<GLImage> *in ) { bound=in; }
 void Set( const char *path="data/images/", const char *description="Choose an image for this animation element.", const char *included="" ) {
  fd.description=description;
  if ( strlen(included) > 0 ) fd.includedLocations.Add(included);
  fd.path=path;
 }
 void Render( int x, int y ) {
  if ( bound ) {
   fast.text(label.c_str(),x+w/2,y-8,true);
   (bound.pointer->pointer) = fast.imagePicker(&fd,bound.pointer->pointer,x,y,w,h,bh,xToNull);
  }
 }
MANY(FastImageDialog,FastImageDialogHandle,FastImageDialogHandles,"",FastImageDialogs,{})
 FastImageDialog *Add( Zp<GLImage> *binding, const char *path="data/images/", const char *description="Choose the first image for this animation element.", const char *label="", const char *included="" ) {
  FastImageDialog *a=new FastImageDialog;
  Append(a);
  a->Bind(binding);
  a->Set(path,description,included);
  a->label=label;
  return a;
 }
DONE(FastImageDialog);

ONE(FastFilmDialog,{
 label="FILM Vaults";
 w=384;
 h=256;
})
 Zp<Zp<Film>> bound;
 Zstring fi,va,label;
 Zdouble instant;
 Zint scrolled,w,h;
 void Set( Film *f ) {
  if ( !f ) { fi=""; va=""; return; }
  FilmLibrary *lib=f->GetLibrary();
  if ( !lib ) { fi=""; va=""; return; }
  va=lib->title;
  fi=f->title;
 }
 void Bind( Zp<Film> *in ) {
  bound=in;
  if ( bound ) Set(bound.pointer->pointer);
 }
 void Render( int x, int y ) {
  if ( bound ) (bound.pointer->pointer)=fast.FilmPicker(&va.value,&fi.value,&instant.value,&scrolled.value,x,y,(int)w,(int)h,label.c_str());
 }
MANY(FastFilmDialog,FastFilmDialogHandle,FastFilmDialogHandles,"",FastFilmDialogs,{})
 FastFilmDialog *Add( Zp<Film> *binding, int w=384, int h=256, const char *label="FILM Vaults" ) {
  FastFilmDialog *a=new FastFilmDialog;
  Append(a);
  a->Bind(binding);
  a->w=w;
  a->h=h;
  a->label=label;
  return a;
 }
DONE(FastFilmDialog);

class MoveableWindowManager;
class MoveableEditorWindow : public Proce55or {
public:
 VirtualWindow v;
 Zint i;
 Zbool hide;
 Zbool clip;
 Zp<MoveableWindowManager> mwm;
 TempFilmReferencers tfrs;
 MoveableEditorWindow() : Proce55or() { Defaults(); }
 MoveableEditorWindow( MoveableWindowManager *mwm );
 virtual ListItem *Editing() { return null; }
 void Defaults() { 
  v.proce55or=this;
  Name("My Window!!");
  v.SizeAndPosition(x,y,w,h);
  clip=true;
  v.encapsulated=this;
 }
 void Name(string s) { Name(s.c_str()); }
 void Name(const char *s) {
  name=s;
  v.Title(s);
 }
 void WindowExtents( int X, int Y, int W, int H ) {
  v.SizeAndPosition(X,Y,W,H);
  SetExtents(
   (int)v.interior.x,
   (int)v.interior.y,
   (int)v.interior.w,
   (int)v.interior.h
  );
 }
 FastImageDialogs fast_image;
 FastFilmDialogs fast_film;
 void ClearFastDialogs() { fast_image.Clear(); fast_film.Clear(); }
 FastImageDialog *BindImage( Zp<GLImage> *i, const char *path="data/images/", const char *label="", const char *description="Choose an image for this animation element.", const char *included="" ) { return fast_image.Add(i,path,description,label,included); }
 FastFilmDialog *BindFilm( Zp<Film> *f, int w=384, int h=256, const char *label="FILM Vaults" ) { return fast_film.Add(f,w,h,label); }
 virtual void OnSetup() {
 }
 virtual void OnMessage( const char *text ) {}
 virtual void OnMessage( int i ) {}
 virtual void Repopulate() {}
 void Setup() {
  v.Title(name.c_str());
  v.noResize=false;
  v.noClose=false;
  v.noMaximize=false;
  v.noMinimize=false;
  OnSetup();
  SetExtents(
   (int)v.interior.x,
   (int)v.interior.y,
   (int)v.interior.w,
   (int)v.interior.h
  );
  /*
  VirtualWindowSettings *vws=virtualWindowSettingsHeap.Find(name.c_str());
  if ( vws ) { // Load previously saved window data.
   v.SizeAndPosition(vws->position.x,vws->position.y,vws->position.w,vws->position.h);
   this->SetExtents(
    (int)v.position.x,
    (int)v.position.y,
    (int)v.position.w,
    (int)v.position.h
   );
  }
  */
 }
 virtual void RenderWindow() {}
 virtual void Render() {
  if ( hide ) return;
  v.Between();
  VirtualWindow *old=fast.member;
  fast.member=&v;
  if ( v.OnScreenPartially() || v.persistOffscreen ) {
   if ( !v.isMinimized ) v.RenderBack();
   if ( clip ) scissors.Push((int)v.interior.x,(int)v.interior.y,(int)v.interior.w,(int)v.interior.h);
   if ( !v.isMinimized ) RenderWindow();
   if ( clip ) scissors.Pop();
   if ( !v.isMinimized ) v.RenderFront();
  }
  fast.member=old;
 }
 virtual void Between() {
 }
 void BetweenFrames() {
  if ( hide ) return;
  Between();
  this->SetExtents(
   (int)v.interior.x,
   (int)v.interior.y,
   (int)v.interior.w,
   (int)v.interior.h
  );
  /*
   virtualWindowSettingsHeap.Update(&v);
   if ( v.released ) virtualWindowSettingsHeap.Save();
  */
 }
};

// Specialty class used by VirtualGUI to create a navigator widget.
class VirtualWindowNavigator : public MoveableEditorWindow {
public:
 VirtualWindowNavigator() : MoveableEditorWindow() {}
 Crayon navBorder,navWindow,navWindowHalf,navIndicator,navFocused,navHovered;
 void OnSetup() {
  navBorder.Pick(magenta);
  navBorder.MultiplyClampRGB(0.25f);
  if ( v.style ) navWindow=v.style->windowBacking; else navWindow.Pick(gray);
  navWindowHalf.fromCrayon(navWindow);
  navWindowHalf.MultiplyClampRGB(0.5f);
  navFocused.x_LightCoral();
  navHovered.x_LightSteelBlue();
  navIndicator.Pick(magenta);
  navIndicator.MultiplyClampRGB(0.7f);
  clip=false;
  v.background.Pick(indigo);
  v.background.MultiplyClampRGB(1.5f);
  v.noMove=true;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=true;
  v.noTitleBar=true;
  v.noBacking=false;
  v.alwaysOnTop=true;
  v.noClose=true;
  v.noCenter=true;
  v.noClipTitle=true;
  v.minimumSize.Set(256,192);
  v.maximumSize.Set(256,192);
  if ( v.holder )
   WindowExtents((int)v.holder->vdisplay.w-32-64-256,128+32+64+16+64-(int)v.titlebar.h-(v.style?(int)v.style->unit2x:0),256,192);
  Name("Window Navigator");
 }
 void Between() {
 }
 Zp<VirtualWindow> clickingWindow,wasClickingWindow;
 void RenderWindow() {
  fast.member=&v;
  Blending(none);
  Rectangle(navWindow,&v.interior);
  if ( ( v.holder->outerBounds.w < v.holder->vdisplay.w && v.holder->outerBounds.h < v.holder->vdisplay.h ) || input.rightReleased() ) { v.closing=true; return; }
  if ( v.holder ) {
   Cartesiand mouseScreen((int)((double)input.mxd*v.holder->vdisplay.w),(int)((double)input.myd*v.holder->vdisplay.h));
   double boundsAspect=v.holder->outerBounds.w/v.holder->outerBounds.h;
   double boundsInverseAspect=v.holder->outerBounds.h/v.holder->outerBounds.w;
   Cartesiand outerBoundsOnScreen;
   if ( boundsAspect > 1.0 ) {
    outerBoundsOnScreen.SetRect(0,0,v.interior.w,v.interior.h*boundsInverseAspect);
    outerBoundsOnScreen.MoveBy(0.0,(v.interior.h-outerBoundsOnScreen.h)/2.0);
   } else {
    outerBoundsOnScreen.SetRect(0,0,v.interior.w*boundsAspect,v.interior.h);
    outerBoundsOnScreen.MoveBy((v.interior.w-outerBoundsOnScreen.w)/2.0,0.0);
   }
   Cartesiand boundsScale(
    outerBoundsOnScreen.w/v.holder->outerBounds.w,
    outerBoundsOnScreen.h/v.holder->outerBounds.h
   );
   outerBoundsOnScreen.MoveBy(v.interior.x,v.interior.y);
   Rectangle(navBorder,&outerBoundsOnScreen);
   Blending(additive);
   VirtualWindow *selecting=null;
   EACH(v.holder->windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->isMinimized && !vwh->p->noMove ) {
    VirtualWindow *win=vwh->p;
    Cartesiand rect;
    rect.SetRect(
     outerBoundsOnScreen.x+((win->edge.x-v.holder->outerBounds.x)/v.holder->outerBounds.w)*outerBoundsOnScreen.w,
     outerBoundsOnScreen.y+((win->edge.y-v.holder->outerBounds.y)/v.holder->outerBounds.h)*outerBoundsOnScreen.h,
     boundsScale.x*win->edge.w,
     boundsScale.y*win->edge.h
    );
    if ( win->attachedTo ) {
     Cartesiand attachment(
      outerBoundsOnScreen.x+((win->attachedTo->edge.x-v.holder->outerBounds.x)/v.holder->outerBounds.w)*outerBoundsOnScreen.w+boundsScale.x*win->attachedTo->edge.w/2.0,
      outerBoundsOnScreen.y+((win->attachedTo->edge.y-v.holder->outerBounds.y)/v.holder->outerBounds.h)*outerBoundsOnScreen.h+boundsScale.y*win->attachedTo->edge.h/2.0
     );
     DrawLine2Color(navWindowHalf,navWindow,rect.x+rect.w/2.0,rect.y+rect.h/2.0,attachment.x,attachment.y);
     Rectangle(navWindow,attachment.x-1,attachment.y-1,attachment.x+1,attachment.y+1);
    }
    if ( rect.within(mouseScreen.x,mouseScreen.y) ) {
     Area(navHovered,&rect);
     if ( input.leftReleased() ) {
      if ( selecting ) {
       if ( selecting->edge.within(win->edge.x,win->edge.y)
         && selecting->edge.within(win->edge.x2,win->edge.y2) ) selecting=win;
       else if ( win->edge.w*win->edge.h < selecting->edge.w*selecting->edge.h ) selecting=win;
      } else {
       selecting=win;
      }
     } else if ( input.left ) { // Moving the window on the nav
      wasClickingWindow=clickingWindow;
      clickingWindow=win;
     }
    } else if ( win == v.holder->focused ) Rectangle(navFocused,&rect);
    else Rectangle(navWindow,&rect);
   }
   if ( selecting ) {
    v.holder->SlideTo(selecting);
    selecting->stayInFront.Reset();
   }
   Blending(none);
   Cartesiand indicatorRect;
   indicatorRect.SetRect(
    (-v.holder->outerBounds.x/v.holder->outerBounds.w)*outerBoundsOnScreen.w,
    (-v.holder->outerBounds.y/v.holder->outerBounds.h)*outerBoundsOnScreen.h,
    boundsScale.x*v.holder->vdisplay.w,
    boundsScale.y*v.holder->vdisplay.h
   );
   Cartesiand mouseOnIndicator;
   mouseOnIndicator.Set(
    indicatorRect.x+(input.mxd*indicatorRect.w),
    indicatorRect.y+(input.myd*indicatorRect.h)
   );
   indicatorRect.MoveBy(
    outerBoundsOnScreen.x,
    outerBoundsOnScreen.y
   );
   if ( indicatorRect.x < v.interior.x ) indicatorRect.SetCorners(v.interior.x,indicatorRect.y,indicatorRect.x2,indicatorRect.y2);
   if ( indicatorRect.y < v.interior.y ) indicatorRect.SetCorners(indicatorRect.x,v.interior.y,indicatorRect.x2,indicatorRect.y2);
   if ( indicatorRect.x2 > v.interior.x2 ) indicatorRect.x2=v.interior.x2;
   if ( indicatorRect.y2 > v.interior.y2 ) indicatorRect.y2=v.interior.y2;
   indicatorRect.SetCorners(indicatorRect.x,indicatorRect.y,indicatorRect.x2,indicatorRect.y2);
   Rectangle(navIndicator,indicatorRect.x,indicatorRect.y,indicatorRect.x2,indicatorRect.y2);
   mouseOnIndicator.MoveBy(
    outerBoundsOnScreen.x,
    outerBoundsOnScreen.y
   );
   if ( outerBoundsOnScreen.within(mouseOnIndicator.x,mouseOnIndicator.y) ) {
    DrawCross(crayons.Pick(green255),(int)mouseOnIndicator.x,(int)mouseOnIndicator.y,1);
   }
  }
  Blending(none);
 }
};




class MoveableWindowManager : public GLWindow {
public:
 Zp<FBOColorDepth> fbo;
 Zp<GLWindow> back_to;
 ScreenPosition virtual_size;
 VirtualGUI vgui;
 Proce55ors editors;
 Delay saveStateInterval;
 Zstring statefile;
 Zbool autoarrange;
 MoveableWindowManager() : GLWindow() {
  SetVirtualSize();
  autoarrange=true;
 }
 MoveableWindowManager( Proce55ors *p ) : GLWindow() {
  SetVirtualSize();
  autoarrange=true;
 }
 virtual void LoadEditors() {}
 virtual void RenderBackground() {}
 void Add( MoveableEditorWindow *mew, bool slideToThisWindow=true ) {   
  vgui.Add(mew);
  mew->Setup();
  if ( slideToThisWindow ) vgui.SlideTo(&mew->v);
 }
 virtual void SetVirtualSize() {
  fbo=&fbo1080p;
  virtual_size.Set(0,0,fbo->w,fbo->h);
  vgui.VirtualDisplay1080p();
  vgui.proce55ors=&editors;
  vgui.glWindow=this;
  virtualStyleDefault.Relative(1.0,0.8);
 }
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  saveStateInterval.Set(60.0);
  LoadEditors();
  if ( autoarrange ) vgui.Arrange2Rows();
  if ( statefile.length > 0 ) {
   if ( file_exists(statefile.c_str()) ) vgui.LoadState(statefile.c_str());
   else vgui.SaveState(statefile.c_str());
  }
 }
 void Repopulate() {
  editors.Clear();
 }
 void Between() {
  if ( visible ) {
   saveStateInterval.Between();
   if ( !saveStateInterval ) {
    saveStateInterval.Reset();
    vgui.SaveState(statefile);
   }
   editors.Between();
  }
 }
 virtual void RenderOverlay() {}
 void Render() {
  if ( visible ) {
   fast.vwindows=&vgui;
   fast.vdisplay.SetRect(virtual_size.x,virtual_size.y,virtual_size.w,virtual_size.h);
   fbo->background.Pick(indigo);
   art.PushViewport();
   art.Viewport(fbo);
   fbo->Bind();
   vgui.RenderPart1();
   RenderBackground();
   RenderBeforeWindow();
   vgui.RenderPart2();
   RenderOverlay();
   RenderAfterWindow();
   fbo->Unbind();
   art.PopViewport();
   fbo->blend=none;  
   fbo->RenderUpsideDown(0,0,display->w,display->h);
  }
 }
 virtual void RenderBeforeWindow() {}
 virtual void RenderAfterWindow() {}
};

// String editor example
C_ONE(ZstringEditor,MoveableEditorWindow,{})
 Zp<ListItem> ed;
 Zp<Zstring> zstr;
 Zstring label;
 Cartesian window;
 Zint lines,lineSpace,border,kern;
 Zp<TexFont> font;
 ZstringEditor( ListItem *p, Zstring *q ) : MoveableEditorWindow() {
  ed=p;
  zstr=q;
  window.SetRect(512,512,512,96);
  lines=1;
  lineSpace=2;
  border=4;
  kern=-4;
  font=glowTermFont;
 }
 void OnSetup() {
  clip=false;
  v.alwaysOnTop=true;
  v.SizeAndPosition(window.x,window.y,window.w,window.h);
  v.minimumSize.Set(window.w,window.h);
  v.maximumSize.Set(window.w,window.h);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noClose=false;
  v.noClipTitle=true;
  v.background.x_Black();
  v.Editing(ed);
  Name("STRING EDITOR");
  Repopulate();
 }
 void Between() {}
 void RenderWindow() {
  if ( v.closing || v.moving || v.wasMoving || v.holder->sliding ) return;
  zstr->value = fast.textbox(font,transparency,termFont,additive,label.c_str(),zstr->value,
   x+8,y+8,UMIN(80,w/12-2),true,lines,lineSpace,border,false,kern);
  zstr->rewind();
 }
MANY(ZstringEditor,ZstringEditorHandle,ZstringEditorHandles,"ZstringEditor",ZstringEditors,{})
DONE(ZstringEditor);