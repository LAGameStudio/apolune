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

#include "LinkedList.h"
#include "Art.h"
#include "Crayon.h"
#include "Cartesian.h"
#include "Proce55ors.h"

#include "MicroFont.h"
#include "Delay.h"
#include "Graphics.h"
#include "Trigger.h"


class VirtualDisplay : public ListItem {
};

class VirtualWindowStyle : public ListItem {
public:
 Zdouble textAspect;
  /// Style settings
 Zdouble unit,unit2x,unit4x,smallestSquareButton,attachmentCircleSize,attachmentCircleThickness,attachmentLineThickness;
 Cartesiand smallestText, smallText,mediumText,largeText,superText,hugeText,colossalText,gargantuanText,cyclopeanText;
 Cartesiand firstTextLineOffset;
 Crayon
  windowBacking,windowTitle,windowTitleText,windowEdge,
  buttonNormal,buttonHover,buttonSelected,buttonHoverSelected,buttonClicking,buttonDisabled,buttonText,buttonSelectedText,
  windowText,windowTextBold,windowTextDisabled,windowSelectedTextBackground,
  attachmentColor;
 Zp<TexFont> windowTitleFont;
 Blends windowTitleFontBlend;
 Zint windowTitleKern;
 VirtualWindowStyle() : ListItem() { }
 void Defaults() {
  // Default Size
  Relative(1.3,0.80);
  // Default Colors
  windowBacking.x_DarkSlateBlue();
  windowTitle.x_MidnightBlue();
  windowTitleText.x_LightGoldenrod();
  windowTitleFont=termFont;
  windowTitleFontBlend=transparency;
  windowTitleKern=-3;
  windowEdge.x_DarkSlateBlue();
  buttonNormal.x_Blue();
  buttonHover.x_RoyalBlue();
  buttonSelected.x_OrangeRed();
  buttonHoverSelected.x_Orange();
  buttonClicking.x_Tomato();
  buttonDisabled.x_DarkGray();
  windowText.x_Silver();
  windowTextBold.x_Silver(); windowTextBold.MultiplyClampRGB(1.5f);
  windowTextDisabled.x_SlateGray();
  buttonText.x_White();
  buttonSelectedText.x_LightCyan();
  windowSelectedTextBackground.x_DarkBlue();
  attachmentColor.x_Lime();
 }
 virtual void Relative( double unitSize, double textAspectRatio ) {
  textAspect=textAspectRatio;
  unit=unitSize;
  unit2x=unit*2.0;
  unit4x=unit*4.0;
  smallestSquareButton=18.0*unit;
  firstTextLineOffset.Set(unit2x,unit2x);
  smallestText.Set(unit*textAspect*10.0,unit*10.0);
  smallText.Set(unit*textAspect*14.0,unit*14.0);
  mediumText.Set(unit*textAspect*17.0,unit*17.0);
  largeText.Set(unit*textAspect*30.0,unit*30.0);
  superText.Set(unit*textAspect*48.0,unit*48.0);
  hugeText.Set(unit*textAspect*64.0,unit*64.0);
  colossalText.Set(unit*textAspect*96.0,unit*96.0);
  gargantuanText.Set(unit*textAspect*116.0,unit*116.0);
  cyclopeanText.Set(unit*textAspect*160.0,unit*160.0);
  attachmentCircleSize=smallestSquareButton/2.0;
  attachmentCircleThickness=unit*2.0;
  attachmentLineThickness=unit*5.0;
 }
};

extern VirtualWindowStyle virtualStyleDefault;

class VirtualGUI;

ONE(VirtualWindow,{
  wasCreated=true;
  noTitle=false;
  noMinimize=true;
  noMaximize=true;
  noClose=true;
  noResize=true;
  style=&virtualStyleDefault;
  minimized.y=style->smallestText.y+style->firstTextLineOffset.y*4.0;
  vdisplay.Set(1.0,1.0);
  maximumSize.Set(10000,10000);
  background.Pick(clear);
  attachmentRadius=-1.0;
  Construct();
 })
 Zp<VirtualWindow> attachedTo;
 Zp<Proce55or> proce55or;
 Zp<ListItem> associated,editing;  // For tracking editors that wrap classes
 Cartesian target;
 Crayon background;
 Zp<VirtualGUI> holder;
 Zp<ListItem> encapsulated; // Not quite a parent.
 Zstring title;
 Zint priority;
 Zp<VirtualWindowStyle> style;
 Cartesiand position,wasPosition,minimumSize,maximumSize;
 Cartesiand edge,titlebar,t_title,b_resize,b_close,b_maximize,b_minimize,b_center,b_returnTo;
 Cartesiand interior; // Contains the extents of the interior drawable region
 Zbool inside; // If the mouse is inside the interior and not interacting with the window itself
 Cartesiand vdisplay,mouse,mouseRatio,oldMouse,oldMouseRatio,mouseScreen,oldMouseScreen; // inside mouse (pixels) and inside mouse ratio, and mouse relative screen
 Zbool isMinimized,wasMaximized,alwaysOnTop;
 Zbool minimizeWhenClosed,doNotDeleteWhenClosed,doNotReturnToAttachee;
 Cartesiand maximizedMemory,moved;
 Zint titleTextWidth,maximizeClicks;
 // Options:
 Zbool noTitle,noTitleBar,noBacking,noEdge,noBackground,noCenter,noMinimize,noMaximize,noClose,noResize,noMove,
       resizeVerticalOnly,resizeHorizontalOnly,noClipTitle,manipulated;
 Cartesiand minimized; // Size when minimized
 // States
 Zbool wasCreated;
 Zbool dragging,wasDragging;
 Zbool within,withinTitlebar,withinResize,withinClose,withinMaximize,withinMinimize,withinCenter,withinReturnTo;
 Zbool clicking,wasClicking,released,leftClicking,rightClicking,leftReleased,rightReleased;
 // Signals
 Zbool closing,minimizing,maximizing,moving,resizing;
 Zbool wasMoving,wasResizing;
 Zbool persistOffscreen; // Turn this on to indicate we want this window to update even when off screen.
 Delay stayInFront; // Triggered when a window on the navigator is clicked, giving users a chance to move the window if it smaller than a window it is behind.
 Zbool focused; // is this the window that is "on top"?
 // Implemented in VirtualGUI
 Zbool dockLeft,dockRight,dockUp,dockDown; // Determines which way a window can dock.
 Zdouble attachmentRadius;
 VirtualWindow( int x, int y, int w, int h ) : ListItem() {
  position.SetRect(x,y,w,h);
  Construct();
 }
 VirtualWindow( VirtualWindowStyle *style ) : ListItem() {
  this->style=style;
  Construct();
 }
 VirtualWindow( VirtualWindowStyle *style, int x, int y, int w, int h ) : ListItem() {
  this->style=style;
  position.SetRect(x,y,w,h);
  Construct();
 }
 void Title( const char *s ) {
  title=s;
  titleTextWidth=WidthTexFont(style->windowTitleFont,(int)style->smallText.x,(int)style->windowTitleKern,title.c_str());
  minimized.x=(double)titleTextWidth+style->firstTextLineOffset.x*4.0;
 }
 void Style( VirtualWindowStyle *s ) {
  style=s;
  Title(title.c_str());
  Calculate();
  minimized.y=style->smallestText.y+style->firstTextLineOffset.y*4.0;
 }
 void Construct() {
  stayInFront.Set(10.0f);
  Calculate();
 }
 bool OnScreen() { return !OffScreen(); }
 bool OnScreenPartially() {
  if ( OffScreen() ) return false;
  Cartesiand screen(0,0,vdisplay.w,vdisplay.h);
  bool TL=screen.within(position.x,position.y);
  bool TR=screen.within(position.x,position.y2);
  bool BR=screen.within(position.x2,position.y2);
  bool BL=screen.within(position.x,position.y2);
  if ( TL || TR || BR || BL ) return true;
  return screen.Intersects(&position);
 }
 bool OffScreenPartially() {
  Cartesiand screen(0,0,vdisplay.w,vdisplay.h);
  bool TL=screen.within(position.x,position.y);
  bool TR=screen.within(position.x,position.y2);
  bool BR=screen.within(position.x2,position.y2);
  bool BL=screen.within(position.x,position.y2);
  if ( TL && TR && BR && BL ) return false;
  return true;
 }
 bool OffScreen() {
  return position.x2 <= 0 || position.y2 <= 0 || position.x >= vdisplay.w || position.y >= vdisplay.h;
 }
 void MoveTo( double x, double y ) {
  if ( proce55or ) proce55or->MoveTo((int)x,(int)y);
  position.SetRect(x,y,position.w,position.h);
  Calculate();
 }
 void MoveBy( double x, double y ) {
  if ( proce55or ) proce55or->MoveBy((int)x,(int)y);
  position.SetRect(position.x+x,position.y+y,position.w,position.h);
  Calculate();
 }
 void MoveBy( double x, double y, having wasMovingSignal ) {
  if ( proce55or ) proce55or->MoveBy((int)x,(int)y);
  position.SetRect(position.x+x,position.y+y,position.w,position.h);
  Calculate();
  wasMoving=true;
 }
 void Resize( double w, double h ) {
  if ( proce55or ) proce55or->SetExtents((int)proce55or->x,(int)proce55or->y,(int)w,(int)h);
  position.SetRect(position.x,position.y,w,h);
  Calculate();
 }
 void SizeAndPosition( double x, double y, double w, double h ) {
  if ( proce55or ) proce55or->SetExtents((int)proce55or->x,(int)proce55or->y,(int)w,(int)h);
  position.SetRect(x,y,w,h);
  Calculate();
 }
 void SizeAndNear( double sw, double sh, VirtualWindow *vw );
 virtual void OnCalculate() {}
 void Calculate() {
  edge.SetRect(position.x,position.y,position.w,position.h);
  b_resize.SetRect(
   position.x2-style->smallestSquareButton,
   position.y2-style->smallestSquareButton,
   style->smallestSquareButton, style->smallestSquareButton
  );
  double buttonAndMargin=style->unit2x+style->smallestSquareButton;
  double sx=position.x+position.w-buttonAndMargin;
  double sy=position.y+style->unit2x;
  if ( !noClose ) {
   b_close.SetRect(sx,sy,style->smallestSquareButton,style->smallestSquareButton);
   sx-=buttonAndMargin;
  }
  if ( !noMaximize ) {
   b_maximize.SetRect(sx,sy,style->smallestSquareButton,style->smallestSquareButton);
   sx-=buttonAndMargin;
  }
  if ( !noMinimize ) {
   b_minimize.SetRect(sx,sy,style->smallestSquareButton,style->smallestSquareButton);
   sx-=buttonAndMargin;
  }
  if ( !noCenter ) {
   b_center.SetRect(sx,sy,style->smallestSquareButton,style->smallestSquareButton);
   sx-=buttonAndMargin;
  }
  if ( attachedTo ) {
   b_returnTo.SetRect(sx,sy,style->smallestSquareButton,style->smallestSquareButton);
   sx-=buttonAndMargin;
  }
  if ( noTitle ) titlebar.SetRect(
   position.x+style->unit2x,
   position.y+style->unit2x,
   style->smallestSquareButton,style->smallestSquareButton
  ); else {
   int buttonCount=0;
   if ( !noCenter ) buttonCount++;
   if ( !noMinimize ) buttonCount++;
   if ( !noMaximize ) buttonCount++;
   if ( !noClose ) buttonCount++;
   if ( attachedTo ) buttonCount++;
   double width=position.w-buttonAndMargin*(double)buttonCount-style->unit4x;
   if ( width < style->smallestSquareButton ) width=style->smallestSquareButton;
   titlebar.SetRect(
    position.x+style->unit2x,
    position.y+style->unit2x,
    width,
    style->smallestText.y+style->firstTextLineOffset.y*4.0
   );
  }
  interior.SetRect(titlebar.x,titlebar.y2,position.w-style->unit2x,position.h-(titlebar.y2-position.y));
  OnCalculate();
 }
 void Associate( ListItem * I ) { this->associated=I; }
 void Edits(ListItem *I) { this->editing=I; }
 void Editing(ListItem *I) { this->editing=I; }
 void Attach( VirtualWindow *w, int target_center_relative_x, int target_center_relative_y ) {
  attachedTo=w;
  target.Set(target_center_relative_x,target_center_relative_y);
  Calculate();
 }
 void Attach( VirtualWindow *w, having positionScreenfromTopLeft, int sx, int sy ) {
  attachedTo=w;
  target.Set( sx,sy ); //0,0 );// (-(int)(w->interior.w/2.0)), (-(int)(w->interior.h/2.0)) );
 }
 void Unminimize();
 virtual void Between();
 virtual void RenderBack();
 virtual void RenderInside() { /*...*/ }
 virtual void RenderFront() {
  if ( !noResize ) DrawSmallestButton("-",b_resize.x+3,b_resize.y+3,withinResize);
 }
 virtual void Render() {
  Between();
  RenderBack();
  scissors.Push((int)interior.x,(int)interior.y,(int)interior.w,(int)interior.h);
  RenderInside();
  scissors.Pop();
  RenderFront();
 }
private:
 void DrawSmallestButton( const char *c, double x, double y, bool withinButton ) {
  Cartesiand border(x,y,style->smallestSquareButton.value-3,style->smallestSquareButton.value-3);
  double size=(style->smallestSquareButton-style->unit4x)-3;
  Cartesiand inner(x+style->unit2x+size/2,y+style->unit2x+size/2);
  if ( withinButton ) {
   if ( clicking ) {
    Area(style->buttonClicking,border.x,border.y,border.x2,border.y2);
    style->buttonSelectedText.gl();
//    glLineWidth(2.0);
    Text(c,inner.x,inner.y,size,size,true);
//    glLineWidth(1.0);
    Rectangle(style->windowEdge,(int)border.x,(int)border.y,(int)border.x2,(int)border.y2);
   } else {
    Area(style->buttonHover,(int)border.x,(int)border.y,(int)border.x2,(int)border.y2);
    style->buttonText.gl();
//    glLineWidth(2.0);
    Text(c,inner.x,inner.y,size,size,true);
//    glLineWidth(1.0);
    Rectangle(style->windowEdge,(int)border.x,(int)border.y,(int)border.x2,(int)border.y2);
   }
  } else {
   Area(style->buttonNormal,(int)border.x,(int)border.y,(int)border.x2,(int)border.y2);
   style->buttonText.gl();
//   glLineWidth(2.0);
   Text(c,inner.x,inner.y,size,size,true);
//   glLineWidth(1.0);
   Rectangle(style->windowEdge,(int)border.x,(int)border.y,(int)border.x2,(int)border.y2);
  }
 }
 void Dockable( bool left=true, bool right=true, bool up=true, bool down=true ) {
  dockLeft=left;
  dockRight=right;
  dockUp=up;
  dockDown=down;
 }
MANY(VirtualWindow,VirtualWindowHandle,VirtualWindowHandles,"VirtualWindow",VirtualWindows,{})
DONE(VirtualWindow);

class VirtualWindowSettings : public ListItem {
public:
 Zstring window;
 Cartesiand position;
 VirtualWindowSettings() : ListItem() {}
 VirtualWindowSettings( const char *s ) : ListItem() { fromString(s); }
 VirtualWindowSettings( VirtualWindow *v ) : ListItem() {
  window=v->title;
  position.Set(&v->position);
  if ( window.length < 1 ) window="1";
 }
 string toString() {
  return string("win {")+window+string("} pos {")+position.toString()+string("}");
 }
 void fromString(const char *p) {
  Zstring words(p);
  const char *w=words.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"win") ) {
    w=words.Next();
    window=w;
   } else if ( !str_cmp(w,"pos") ) {
    w=words.Next();
    position.fromString(w);
   }
   w=words.Next();
  }
  if ( window.length < 1 ) window="1";
 }
};

class VirtualWindowSettingsHeap : public LinkedList {
public:
 CLEARLISTRESET(VirtualWindowSettings);
 VirtualWindowSettingsHeap() : LinkedList() {
  Load();
 }
 VirtualWindowSettings *Find( const char *match ) { // all windows should have a name of at least one character!
  if ( strlen(match) == 0 )
  FOREACH(VirtualWindowSettings,vws) if ( !str_cmp(vws->window.c_str(),"1") ) return vws;
  else
  FOREACH(VirtualWindowSettings,vws) if ( !str_cmp(vws->window.c_str(),match) ) return vws;
  return null;
 }
 void Update( VirtualWindow *v ) {
  VirtualWindowSettings *existing=Find(v->title.c_str());
  if ( existing ) {
   existing->position.Set(&v->position);
  } else {
   Append(new VirtualWindowSettings(v));
  }
 }
 string toString() {
  Zstring out;
  FOREACH(VirtualWindowSettings,vws) out+=string("gui {")+vws->toString()+string("}\n");
  return out.value;
 }
 void fromString( const char * s ) {
  Zstring words(s);
  const char *w=words.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"gui") ) {
    w=words.Next();
    Append(new VirtualWindowSettings(w));
   }
   w=words.Next();
  }
 }
 void Save() { string_as_file(toString().c_str(),"data/logs/virtual.windows.txt"); }
 void Load() { if ( file_exists("data/logs/virtual.windows.txt") ) fromString(file_as_string("data/logs/virtual.windows.txt").c_str()); }
};

extern VirtualWindowSettingsHeap virtualWindowSettingsHeap;

ONE(VirtualWindowSortItem,{})
 Zp<VirtualWindowHandle> window;
 double area() { return window->p->position.w * window->p->position.h; }
MANY(VirtualWindowSortItem,VirtualWindowSortItemHandle,VirtualWindowSortItemHandles,"",VirtualWindowSortItems,{})
 void Process( VirtualWindowHandles *out ) {
  Snarf(out);
  SortSize();
  Spit(out);
 }
 SORTING(VirtualWindowSortItem,SortSize,{},{ if ( A->area() < B->area() ) result=-1;},{});
private:
 void Snarf( VirtualWindowHandles *windows ) {
  EACHN(windows->first,VirtualWindowHandle,vwh,{
   windows->Remove(vwh);
   VirtualWindowSortItem *item=new VirtualWindowSortItem;
   item->window=vwh;
   Append(item);
  });
 }
 void Spit( VirtualWindowHandles *out ) {
  FOREACHN(VirtualWindowSortItem,s,{
   Remove(s);
   out->Append(s->window);
   delete s;
  });
 }
DONE(VirtualWindowSortItem);

#include "FastGUI.h"
class MoveableEditorWindow;
class GridRects;
ONE(VirtualGUI,{
 slideSpeed.Set(0.5);
 buttonCascade=library.Load("data/images/elements/vgui_cascade.png");
 button2Rows=library.Load("data/images/elements/vgui_2rows.png");
 buttonRow=library.Load("data/images/elements/vgui_row.png");
 buttonNav=library.Load("data/images/elements/vgui_nav.png");
 dontMoveMinimized=true;
})
 Zp<GLWindow> glWindow; // The GLWindow the vgui is running in
 Zp<Proce55ors> proce55ors; // The Proce55ors (active) of that window
 Zp<GLImage> buttonCascade,button2Rows,buttonRow,buttonNav;
 Zbool windowsLeft,windowsRight,windowsUp,windowsDown; // Indicates when windows are beyond the screen.
 Zbool showCascadeButton;
 VirtualWindowHandles windows;
 Delay slideSpeed;
 Cartesiand vdisplay;
 Cartesiand position,slidingTo,slidingBy;
 Zint slidesteps;
 Zbool sliding,wasSliding,arranging,wasArranged;
 Zbool dontMoveMinimized,restoreMinimized,drawWindowControls;
 Zp<VirtualWindow> focused,slidingToWindow;
 Zp<VirtualWindowHandle> focusedHandle;
 bool WindowCanBePlacedHere( double wx, double wy, double ww, double wh ) {
  Box newWindowRect(wx,wy,wx+ww,wy+wh);
  bool found=false;
  CoplanarLinesResults results;
  Box overlap;
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->isMinimized ) {
   Box rect2(vwh->p->position.x,vwh->p->position.y,vwh->p->position.x2,vwh->p->position.y2);
   bool collides=rect2.Collides(&newWindowRect,&results,&overlap);
   if ( collides ) { found=true; break; }
  }
  return !found;
 }
 int Attached( VirtualWindow *p ) {
  int t=0;
  EACH(windows.first,VirtualWindowHandle,vwh) if ( vwh->p->attachedTo == p ) t++;
  return t;
 }
 void Add( VirtualWindow * v ) {
  v->holder=this;
  v->vdisplay.Set(&vdisplay);
  if ( !windows.find(v) ) {
   windows.Add(v);
  } else {
   OUTPUT("VirtualGUI:Add(v) already found v in the list.\n");
  }
 }
 void Add( MoveableEditorWindow * w );
 void Remove( MoveableEditorWindow * w, bool keep=false );
 void VirtualDisplaySize( double w, double h ) { vdisplay.SetRect(w/display.wd,h/display.hd,w,h); } // Sets the mouse detection ratio.
 void VirtualDisplaySize( double w, double h, having updateAll ) { vdisplay.SetRect(w/display.wd,h/display.hd,w,h);  EACH(windows.first,VirtualWindowHandle,vwh) vwh->p->vdisplay.Set(&vdisplay); }
 void VirtualDisplay1080p() { VirtualDisplaySize(1920.0,1080.0); }
 void VirtualDisplay1080p( having updateAll ) { VirtualDisplaySize(1920.0,1080.0,true); }
 void Remove( VirtualWindow * w ) { VirtualWindowHandle *h=windows.find(w,true); if ( h ) { windows.Remove(h); delete h; } }
 int Order( VirtualWindow * w ) { int i=0; EACH(windows.first,VirtualWindowHandle,vwh) { if ( vwh->p == w ) return i; i++; } return -1; }
 void Reorder( Proce55ors *in_out );
 void Arrange( GridRects *in, int margin=16 );
 Zint pressedArrange;
 void SortSmallestToLargest() {
  VirtualWindowSortItems sorter;
  sorter.Process(&windows);
 }
 void MoveAlwaysOnTopToTop() {
  Cartesiand mouseScreen((int)((double)input.mx*vdisplay.x),(int)((double)input.my*vdisplay.y));
  VirtualWindowHandle *priorLast=(VirtualWindowHandle *)windows.last;
  EACHN(windows.first,VirtualWindowHandle,vwh,{
   if ( vwh->p->alwaysOnTop ) {
    windows.Remove(vwh);
    windows.Append(vwh);
   }
  /* Can't do this here reliably, have to wait for next frame.
   if ( vwh->p->position.within(mouseScreen.x,mouseScreen.y) ) {
    focused=vwh->p;
    focusedHandle=vwh; // Activate the last one
   }
   */
   if ( vwh == priorLast ) break;
  });
 }
 void MoveStayInFrontToTop() {
  Cartesiand mouseScreen((int)((double)input.mx*vdisplay.x),(int)((double)input.my*vdisplay.y));
  // Windows with temporary stacking override because they were clicked in the Navigator
  VirtualWindowHandle *priorLast=(VirtualWindowHandle *)windows.last;
  EACHN(windows.first,VirtualWindowHandle,vwh,{
   vwh->p->stayInFront.Between();
   if ( vwh->p->stayInFront ) {
    windows.Remove(vwh);
    windows.Append(vwh);
    if ( vwh->p->position.within(mouseScreen.x,mouseScreen.y) ) {     
     focused=vwh->p;
     focusedHandle=vwh;
    }
   }
   if ( vwh == priorLast ) break;
  });
 }
 void ArrangeRow() {
  if ( pressedArrange % 3 != 0 ) windows.ReverseOrder();
  else SortSmallestToLargest();
  MoveAlwaysOnTopToTop();
  double sx=0.0;
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->noMove  ) {
   if ( dontMoveMinimized && vwh->p->isMinimized ) continue;
   if ( vwh->p->position.h < vdisplay.h-64.0 ) vwh->p->MoveTo(sx,(vdisplay.h-vwh->p->position.h)/2.0);
   else vwh->p->MoveTo(sx,16.0);
   sx+=vwh->p->position.w+16.0;
   if ( restoreMinimized ) vwh->p->isMinimized=false;
  }
  arranging=true;
  CalculateAll();
 }
 void Arrange2Rows( double xGap=16.0, double yGap=16.0 ) {
  if ( pressedArrange % 3 != 0 ) windows.ReverseOrder();
  else SortSmallestToLargest();
  MoveAlwaysOnTopToTop();
  //double totalWidth=0.0;
  //EACH(windows.first,VirtualWindowHandle,vwh) {
  // totalWidth+=vwh->p->position.w+16.0;
  //}
  double halfTotalWidth=vdisplay.w-90.0; //totalWidth/2.0;
  Cartesian walking;
  double sx=32.0;
  double sy=16.0;
  double largestH=0.0;
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->noMove ) {
   if ( dontMoveMinimized && vwh->p->isMinimized ) continue;
   vwh->p->MoveTo(sx,sy);
   if ( vwh->p->position.h > largestH ) largestH=vwh->p->position.h;
   sx+=vwh->p->position.w+xGap;
   if ( vwh->next != null
    && ((((VirtualWindowHandle *) vwh->next)->p->position.w+sx) > halfTotalWidth) ) {
    sx=32.0;
    sy+=largestH+yGap;
    largestH=0.0;
   }
   if ( restoreMinimized ) vwh->p->isMinimized=false;
  }
  arranging=true;
  CalculateAll();
 }
 void Cascade() {
  // Sort by size smallest to largest
  if ( pressedArrange % 3 != 0 ) windows.ReverseOrder();
  else SortSmallestToLargest();
  MoveAlwaysOnTopToTop();
  // Cascade
  Cartesiand walking(32.0,32.0);
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->noMove ) {
   if ( dontMoveMinimized && vwh->p->isMinimized ) continue;
   vwh->p->MoveTo(walking.x,walking.y);
   walking.x+=128.0;
   walking.y+=128.0;
   if ( restoreMinimized ) vwh->p->isMinimized=false;
   focused=vwh->p;
   focusedHandle=vwh; // Temporarily activate the last one
  }
  arranging=true;
  CalculateAll();
 }
 void CalculateAll() {
  EACH(windows.first,VirtualWindowHandle,vwh) vwh->p->Calculate();
 }
 VirtualWindowHandle *Editing( ListItem *I ) { EACH(windows.first,VirtualWindowHandle,vwh) if ( vwh->p->editing == I ) return vwh; return null; }
 VirtualWindowHandle *byName( const char *name ) {  EACH(windows.first,VirtualWindowHandle,vwh) if ( name == vwh->p->name ) return vwh; return null; }
 VirtualWindowHandle *byTitle( const char *name ) {  EACH(windows.first,VirtualWindowHandle,vwh) if ( name == vwh->p->title ) return vwh; return null; }
 VirtualWindowHandle *byAssociation( ListItem *I ) { EACH(windows.first,VirtualWindowHandle,vwh) if ( vwh->p->associated==I ) return vwh; return null; }
 int countByType( ListItem *I ) {
  int total=0;
  EACH(windows.first,VirtualWindowHandle,vwh)
   if ( vwh->p->associated == I ) total++;
  return total;
 }
 int countByWindowType( Proce55or *p ) {
  int total=0;
  EACH(windows.first,VirtualWindowHandle,vwh)
   if ( vwh->p->proce55or && !str_cmp(vwh->p->proce55or->identity().c_str(),p->identity().c_str()) ) total++;
  return total;
 }
 bool SlideToIfAssociated( ListItem *I ) {
  VirtualWindowHandle *vwh=byAssociation(I);
  if ( !vwh ) return false;
  SlideTo(vwh->p);
  return true;
 }
 void LoadState( const char *fn ) {
  if ( file_exists(fn) ) {
   Zstring file=file_as_string(fn);
   Strings kv;
   const char *w=file.Next();
   while ( *w != '\0' ) {
    Zstring k=w;
    w=file.Next();
    kv.addKeyValue(k.c_str(),w);
   }
   EACH(kv.first,KeyValuePair,k) {
    VirtualWindowHandle *vwh=byTitle(k->s.c_str());
    if ( vwh ) {
     Cartesiand c; c.fromString(k->value.c_str());
     vwh->p->SizeAndPosition(c.x,c.y,c.w,c.h);
     vwh->p->isMinimized=(c.d > 0.0);
    }
   }
  }
 }
 void SaveState( const char *fn ) {
  Zstring out;
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !(vwh->p->noMove && vwh->p->noResize) && !vwh->p->attachedTo ) {
   vwh->p->position.d=(vwh->p->isMinimized?1.0:0.0);
   out+=string(" {")+vwh->p->title+string("} ");
   out+=string(" {")+vwh->p->position.toString()+string("} ");
  }
  string_as_file(out.c_str(),fn);
 }
 void SlideTo( VirtualWindow *target ) {
  slidingToWindow=target;
  Cartesian moveTo(32,32);
  if ( target->position.w < vdisplay.w ) moveTo.x=(int)(vdisplay.w - target->position.w)/2;
  if ( target->position.h < vdisplay.h ) moveTo.y=(int)(vdisplay.h - target->position.h)/2;
  SlideTo((int)(position.x-target->position.x)+moveTo.x,(int)(position.y-target->position.y)+moveTo.y);
  target->Unminimize();
 }
 void Unminimize( VirtualWindow *v ) {
  v->isMinimized=false;
  v->minimizing=false;
  v->withinMinimize=false;
  if ( v->minimizeWhenClosed ) v->closing=false;
 }
 void SlideTo( int x, int y ) {
  sliding=true;
  slidingToWindow=null;
  slidingTo.Set(x,y);
  slidesteps=(int)(slideSpeed.reset * (float) FPS);
  slidingBy.Set(
   ((slidingTo.x-position.x)/(double)slidesteps),
   ((slidingTo.y-position.y)/(double)slidesteps)
  );
 }
 void SlideBy( int dx, int dy ) {
  SlideTo((int)slidingTo.x+dx,(int)slidingTo.y+dy);
 }
 void MoveBy( int dx, int dy ) {
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->noMove ) vwh->p->MoveBy(dx,dy,true);
  position.MoveBy(dx,dy);
 }
 void MoveBy( double dx, double dy ) {
  EACH(windows.first,VirtualWindowHandle,vwh) if ( !vwh->p->noMove ) vwh->p->MoveBy(dx,dy,true);
  position.MoveBy(dx,dy);
 }
 void SlideUpdate() {
  if ( slidesteps == 0 ) {
   MoveBy(slidingTo.x-position.x,slidingTo.y-position.y);
   slidingTo.Set(position.x,position.y);
   sliding=false;
   wasSliding=true;
   slidingToWindow=null;
  } else if ( slidesteps > 1 ) {
   MoveBy(slidingBy.x,slidingBy.y);
  }
  slidesteps--;
 }
 void DestroyAttached( VirtualWindow *w ) {
  if ( !proce55ors ) return;
  EACHN(windows.first,VirtualWindowHandle,vwh,{
   if ( vwh->p->attachedTo == w ) {
    if ( vwh->p->proce55or ) {
     proce55ors->Remove(vwh->p->proce55or);
     if ( !vwh->p->doNotDeleteWhenClosed ) delete vwh->p->proce55or;
    }
    windows.Remove(vwh);
    delete vwh;
   }
  });
 }
 void DestroyAttached( VirtualWindow *w, VirtualWindow *except ) {
  if ( !proce55ors ) return;
  EACHN(windows.first,VirtualWindowHandle,vwh,{
   if ( vwh->p->attachedTo == w && vwh->p != except ) {
    if ( vwh->p->proce55or ) {
     proce55ors->Remove(vwh->p->proce55or);
     if ( !vwh->p->doNotDeleteWhenClosed ) delete vwh->p->proce55or;
    }
    windows.Remove(vwh);
    delete vwh;
   }
  });
 }
 void PanToFirstNonminimizedMinimizableWindow() {
  EACH(windows.first,VirtualWindowHandle,vwh) {
   if ( !vwh->p->isMinimized && !vwh->p->noMinimize ) {
    VirtualWindow *win=vwh->p;
    Cartesian moveTo(32,32);
    SlideTo(win);
    return;
   }
  }
 }
 Cartesiand outerBounds;
 void HandleNavWindow();
 Zint minimizedWindowCount,notMinimizedNotMinimizableWindows;
 Zbool ignoreNextDelayedMoveIndication;
 void Update() {
  bool closedWindow=false;
  if ( arranging ) { wasArranged=true; arranging=false; }
  else if ( wasArranged ) wasArranged=false;
  if ( sliding ) SlideUpdate();
  else if ( wasSliding ) wasSliding=false;
  position.SetRect(position.x,position.y,vdisplay.w,vdisplay.h);
  windowsLeft=false;
  windowsRight=false;
  windowsUp=false;
  windowsDown=false;
  VirtualWindow *manipulating=null;
  focused=null;
  focusedHandle=null;
  Cartesiand mouseScreen((int)((double)input.mxd*vdisplay.w),(int)((double)input.myd*vdisplay.h));
  EACHN(windows.first,VirtualWindowHandle,vwh,{
   if ( vwh->p->minimizing ) EACH(windows.first,VirtualWindowHandle,attached)
    if ( attached->p->attachedTo == vwh->p ) attached->p->isMinimized=true; /* Note: minimizes even though noMinimize may be set */
   if ( vwh->p->closing ) {
    closedWindow=true;
    if ( vwh->p->minimizeWhenClosed ) {
     vwh->p->closing=false;
     vwh->p->isMinimized=true;
     vwh->p->minimized.x=(double)vwh->p->titleTextWidth+vwh->p->style->firstTextLineOffset.x*4.0;
     if ( vwh->p->attachedTo ) SlideTo(vwh->p->attachedTo);
    } else {
     VirtualWindow *destination=null;
     if ( vwh->p->attachedTo && !vwh->p->doNotReturnToAttachee ) destination=vwh->p->attachedTo;
     if ( focused == vwh->p ) {
      focused=null;
      focusedHandle=null;
     }
     while ( n && n->p->attachedTo == vwh->p ) n=(VirtualWindowHandle *) n->next;
     DestroyAttached(vwh->p);
     if ( proce55ors && vwh->p->proce55or ) {
      proce55ors->Remove(vwh->p->proce55or);
      if ( !vwh->p->doNotDeleteWhenClosed ) delete vwh->p->proce55or;
     }
     windows.Remove(vwh);
     delete vwh;
     if ( destination ) {
      SlideTo(destination);
      closedWindow=false;
     }
     continue;
    }
   }
   VirtualWindow *win=vwh->p;
   if ( !win->isMinimized ) {
    if ( win->position.x < 0 ) windowsLeft=true;
    if ( win->position.y < 0 ) windowsUp=true;
    if ( win->position.x2 > (double) vg.w ) windowsRight=true;
    if ( win->position.y2 > (double) vg.h ) windowsDown=true;
   }
   win->focused=!win->isMinimized && (win->moving || win->resizing || win->dragging || win->position.within(mouseScreen.x,mouseScreen.y)); // Set this true on any applicable
   if ( win->focused ) {
    focused=win;
    focusedHandle=vwh;
   }
  });
  outerBounds.SetRect(vdisplay.w/2.0,vdisplay.h/2.0,0.0,0.0);
  minimizedWindowCount=0;
  notMinimizedNotMinimizableWindows=0;
  EACH(windows.first,VirtualWindowHandle,vwh) { // Remove focus from all but top window.  Do some calculations.
   VirtualWindow *win=vwh->p;
   if ( !win->isMinimized ) {
    if ( win->edge.x < outerBounds.x )   outerBounds.x=win->edge.x;
    if ( win->edge.y < outerBounds.y )   outerBounds.y=win->edge.y;
    if ( win->edge.x2 > outerBounds.x2 ) outerBounds.x2=win->edge.x2;
    if ( win->edge.y2 > outerBounds.y2 ) outerBounds.y2=win->edge.y2;
   }
   if ( win->isMinimized || (win->attachedTo && win->attachedTo->isMinimized) ) minimizedWindowCount++;
   if ( !win->isMinimized  && win->noMinimize ) notMinimizedNotMinimizableWindows++;
   if ( win->focused && win != focused ) win->focused=false;
  }
  outerBounds.w=outerBounds.x2-outerBounds.x;
  outerBounds.h=outerBounds.y2-outerBounds.y;
  if ( focused ) { // Bring to front.
   windows.Remove(focusedHandle);
   windows.Append(focusedHandle);
   if ( sliding && !focused->noMove && focused->moving ) focused->MoveBy((int)-slidingBy.x,(int)-slidingBy.y);
  }
  bool canSlideBy=(FRAME%(int)(FPS*slideSpeed.reset)==0);
  if ( windowsLeft )  {
   if ( (input.mxi < 16 && canSlideBy) || input.KeyUp(DX_NUMPAD_LEFT) || input.KeyUp(DX_NUMPAD_1) || input.KeyUp(DX_NUMPAD_7) ) {
    SlideBy(180, 0 );
   }
  } 
  if ( windowsRight ) {
   if ( (input.mxi > display.w-16 && canSlideBy) || input.KeyUp(DX_NUMPAD_RIGHT) || input.KeyUp(DX_NUMPAD_3) || input.KeyUp(DX_NUMPAD_9) ) {
    SlideBy(-180, 0 );
   }
  } else if ( input.mxi > display.w-16 ) {
   if ( !ignoreNextDelayedMoveIndication && FRAME%(3*FPS)==0 ) {
    SlideBy(-90, 0 );
    ignoreNextDelayedMoveIndication=true;
   }
  } else ignoreNextDelayedMoveIndication=false;
  if ( windowsUp ) {
   if ( ( (input.myi < 16 && canSlideBy) ||  input.KeyUp(DX_NUMPAD_UP) || input.KeyUp(DX_NUMPAD_7) || input.KeyUp(DX_NUMPAD_9) ) ) {
    SlideBy(0,180);
   }
  }
  if ( windowsDown ) {
   if ( (input.myi > display.h-16 && canSlideBy) || input.KeyUp(DX_NUMPAD_DOWN) || input.KeyUp(DX_NUMPAD_1) || input.KeyUp(DX_NUMPAD_3) ) {
    SlideBy(0,-180);
   }
  } else if ( input.myi > display.h-16 ) {
   if ( !ignoreNextDelayedMoveIndication && FRAME%(3*FPS)==0 ) {
    SlideBy(0,-90);
    ignoreNextDelayedMoveIndication=true;
   }
  } else ignoreNextDelayedMoveIndication=false;
  // If only one window is un-minimized, and we haven't done this yet, move it to the center.  This is
  // generally for the first time a GUI is loaded.  We need to subtract the number of minimized windows,
  // from the total window count minus the number of top-level no-minimize windows, and determine if
  // only one is not minimized.
  if ( ( !pannedToOnlyNonminimizedWindow || closedWindow )
   && minimizedWindowCount == windows.count-notMinimizedNotMinimizableWindows-1 ) {
   PanToFirstNonminimizedMinimizableWindow();
   pannedToOnlyNonminimizedWindow=true;
  }
  if ( slidingToWindow && (slidingToWindow->position.x != slidingTo.x || slidingToWindow->position.y != slidingTo.y ))
   SlideTo(slidingToWindow);
 }
 void RenderOverlay() {
  if ( windowsLeft ) {
   Stretch(fast.leftArrow,fast.color,additive,8,8,32,32 );
  } 
  if ( windowsRight ) {
   Stretch(fast.rightArrow,fast.color,additive,(int)vdisplay.w-32-8,(int)vdisplay.h-32-8,32,32 );
  }
  if ( windowsUp ) {
   Stretch(fast.upArrow,fast.color,additive,(int)vdisplay.w-32-8,8,32,32 );
  }
  if ( windowsDown ) {
   Stretch(fast.downArrow,fast.color,additive,8,(int)vdisplay.h-32-8,32,32 );
  }
 }
 // Minimized windows appear in the list at the bottom of the screen.
 // We also draw the window organization buttons on this layer.
 Zbool pannedToOnlyNonminimizedWindow;
 void RenderMinimized() {
  if ( windows.count == 0 ) return;
  VirtualWindowHandle *f=(VirtualWindowHandle *)windows.first;
  Cartesiand mouseScreen((int)((double)input.mxd*vdisplay.w),(int)((double)input.myd*vdisplay.h));
  if ( drawWindowControls ) {
   {
    Cartesiand rect(vdisplay.w-64.0-24.0,64.0,64.0,64.0);
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Area((hovering?f->p->style->buttonHover:f->p->style->buttonNormal),(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Stretch(buttonRow,(hovering?crayons.Pick(alabaster):crayons.Pick(gray)),none,(int)rect.x,(int)rect.y,(int)rect.w,(int)rect.h);
    if ( hovering && input.leftReleased() ) {
     ArrangeRow();
     pressedArrange+=1;
    }
   }
   {
    Cartesiand rect(vdisplay.w-64.0-24.0,64.0+16.0+64.0,64.0,64.0);
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Area((hovering?f->p->style->buttonHover:f->p->style->buttonNormal),(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Stretch(button2Rows,(hovering?crayons.Pick(alabaster):crayons.Pick(gray)),none,(int)rect.x,(int)rect.y,(int)rect.w,(int)rect.h);
    if ( hovering && input.leftReleased() ) {
     Arrange2Rows();
     pressedArrange+=1;
    }
   }
   if ( showCascadeButton ) {
    Cartesiand rect(vdisplay.w-64.0-24.0,64.0+16.0+64.0+16.0+64.0,64.0,64.0);
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Area((hovering?f->p->style->buttonHover:f->p->style->buttonNormal),(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Stretch(buttonCascade,(hovering?crayons.Pick(alabaster):crayons.Pick(gray)),none,(int)rect.x,(int)rect.y,(int)rect.w,(int)rect.h);
    if ( hovering && input.leftReleased() ) {
     Cascade();
     pressedArrange+=1;
    }
   }
   if ( !( outerBounds.w < vdisplay.w && outerBounds.h < vdisplay.h ) ) {
    Cartesiand rect(vdisplay.w-64.0-24.0,128.0+32.0+64.0+16.0+64.0,64.0,64.0);
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Area((hovering?f->p->style->buttonHover:f->p->style->buttonNormal),(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Stretch(buttonNav,(hovering?crayons.Pick(alabaster):crayons.Pick(gray)),none,(int)rect.x,(int)rect.y,(int)rect.w,(int)rect.h);
    if ( hovering && input.leftReleased() ) {
     HandleNavWindow();
    }
   }
  }
  double sx=48.0;
  double sy=vdisplay.h-24.0-f->p->minimized.y-4.0;
  double ty=vdisplay.h-128.0;
  Zp<VirtualWindowHandle> movingHandle;
  EACH(windows.first,VirtualWindowHandle,vwh) {
   if ( vwh->p->moving || vwh->p->wasMoving ) movingHandle=vwh;
   if ( vwh->p->maximizing ) {
    VirtualWindow *win=vwh->p;
    focused=win;
    focusedHandle=vwh;
    windows.Remove(vwh);
    windows.Append(vwh);
    if ( win->OffScreenPartially() ) {
     Cartesian moveTo(32,32);
     if ( win->position.w < vdisplay.w ) moveTo.x=(int)(vdisplay.w - win->position.w)/2;
     if ( win->position.h < vdisplay.h ) moveTo.y=(int)(vdisplay.h - win->position.h)/2;
     SlideTo((int)(position.x-win->position.x)+moveTo.x,(int)(position.y-win->position.y)+moveTo.y);
    }
   } else if ( vwh->p->isMinimized && !vwh->p->attachedTo ) {
    VirtualWindow *win=vwh->p;
    if ( sx+win->minimized.x+4.0 > vdisplay.w-32.0 ) {
     sx=48.0;
     sy-=32.0+4.0;
    }
    Cartesiand rect;
    rect.SetCorners(sx,sy,(sx+win->minimized.x),(sy+win->minimized.y));
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Area(hovering?win->style->windowEdge:win->style->buttonDisabled,(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Area(clicking?win->style->buttonClicking:(hovering?win->style->buttonHover:win->style->windowTitle),&rect);
    if ( hovering && input.leftReleased() ) {
     win->isMinimized=false;
     EACH(windows.first,VirtualWindowHandle,attached) if ( attached->p->attachedTo == vwh->p ) attached->p->isMinimized=false;
     focused=win;
     focusedHandle=vwh;
     windows.Remove(vwh);
     windows.Append(vwh);
     if ( win->OffScreenPartially() ) {
      Cartesian moveTo(32,32);
      if ( win->position.w < vdisplay.w ) moveTo.x=(int)(vdisplay.w - win->position.w)/2;
      if ( win->position.h < vdisplay.h ) moveTo.y=(int)(vdisplay.h - win->position.h)/2;
      SlideTo((int)(position.x-win->position.x)+moveTo.x,(int)(position.y-win->position.y)+moveTo.y);
     }
     return;
    }
    WriteTexFont(win->style->windowTitleFont,win->style->windowTitleText,win->style->windowTitleFontBlend,
     (int)(sx+win->style->firstTextLineOffset.x),
     (int)(sy+win->style->firstTextLineOffset.y), //+style->smallText.y),
     (int)win->style->smallText.x,(int)win->style->smallText.y,
     4,win->title.c_str(),win->style->windowTitleKern
    );
    sx+=win->minimized.x+8.0;
   } else if ( vwh->p->OffScreenPartially() ) { // Not minimized, and partially off the screen.
    VirtualWindow *win=vwh->p;
    double tx=vdisplay.w-8.0-win->titleTextWidth-win->style->firstTextLineOffset.x*4.0;
    Cartesiand rect;
    rect.SetCorners(tx,ty,(tx+win->minimized.x),(ty+win->minimized.y));
    bool hovering=!focused && rect.within(mouseScreen.x,mouseScreen.y);
    bool clicking=hovering && input.left;
    Crayon border; border=hovering?win->style->windowEdge:win->style->buttonDisabled;
    Crayon face; face=clicking?win->style->buttonClicking:(hovering?win->style->buttonHover:win->style->windowTitle);
    Crayon text; text=win->style->windowTitleText;
    if ( !hovering ) {
     border.ScaleByAlphaf(0.70f);
     face.ScaleByAlphaf(0.70f);
     text.ScaleByAlphaf(0.70f);
    }
    Area(border,(int)rect.x-1,(int)rect.y-1,(int)rect.x2+1,(int)rect.y2+1);
    Area(face,&rect);
    if ( hovering && input.leftReleased() ) {
     focused=win;
     focusedHandle=vwh;
     windows.Remove(vwh);
     windows.Append(vwh);
     if ( win->OffScreenPartially() ) {
      Cartesian moveTo(32,32);
      if ( win->position.w < vdisplay.w ) moveTo.x=(int)(vdisplay.w - win->position.w)/2;
      if ( win->position.h < vdisplay.h ) moveTo.y=(int)(vdisplay.h - win->position.h)/2;
      SlideTo((int)(position.x-win->position.x)+moveTo.x,(int)(position.y-win->position.y)+moveTo.y);
     }
     return;
    }
    WriteTexFont(win->style->windowTitleFont,text,win->style->windowTitleFontBlend,
     (int)(tx+win->style->firstTextLineOffset.x),
     (int)(ty+win->style->firstTextLineOffset.y), //+style->smallText.y),
     (int)win->style->smallText.x,(int)win->style->smallText.y,
     4,win->title.c_str(),win->style->windowTitleKern
    );
    ty-=win->minimized.y+3;
   }
  }
  MoveAlwaysOnTopToTop();
  if ( movingHandle ) {
   focused=movingHandle->p;
   focusedHandle=movingHandle;
  } else {
   MoveStayInFrontToTop();
  }
 }
 void Render() {
  Update();
  RenderMinimized();
  Reorder(proce55ors);
  proce55ors->Render();
  RenderOverlay();
 }
 void RenderPart1() {
  Update();
  RenderMinimized();
 }
 void RenderPart2() {
  Reorder(proce55ors);
  proce55ors->Render();
  RenderOverlay();
 }

MANY(VirtualGUI,VirtualGUIHandle,VirtualGUIHandles,"VirtualGUI",VirtualGUIs,{})
DONE(VirtualGUI);
