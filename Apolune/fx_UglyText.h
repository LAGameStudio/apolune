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
#include "Vertex.h"
#include "Proce55or.h"
#include "UglyFont.h"
#include "Crayon.h"
#include "Art.h"

#include "GLImage.h"
#include "TextureLibrary.h"

enum UglyDrawStyle {
 ugly3d=0,
 uglyOutlined=1,
 uglyShadowed=2,
 uglyWire=3
};

UglyDrawStyle uglydraw_fromString(const char *s);

class fx_UglyText : public Proce55or {
public:
 Vertexd position;
 Zstring out;
 Zint fw,fh,lineSpace;
 Zbool centered;
 Blends blend,blendShading;
 Crayon tint,shade;
 UglyDrawStyle style;
 PROGRAMMABLE(fx_UglyText,DrawTextPtr,drawtext,DrawText);
 void Initialize() {  
  drawtext=&fx_UglyText::UglyWire;
  fw=8;
  fh=10;
  lineSpace=2;
  centered=true;
  tint.Pick(alabaster);
  shade.Pick(black);
  duration=10.0f;
  style=uglyWire;
 }
 fx_UglyText() : Proce55or() {
  Initialize();
 }
 fx_UglyText( string s ) : Proce55or() {
  Initialize();
  out=s;
 }
 fx_UglyText( int x, int y, string s ) : Proce55or() {
  Initialize();
  out=s;
  this->x=x;
  this->y=y;
 }
 fx_UglyText( int x, int y, string s, UglyDrawStyle mode ) : Proce55or() {
  Initialize();
  out=s;
  this->x=x;
  this->y=y;
  SetStyle(mode);
 }
 void Setup() {
  MoveTo(x,y);
 }
 void SetStyle( UglyDrawStyle uds ) {
  style=uds;
  switch ( style ) {
        case ugly3d:
          blend=additive;
          tint.Pick(blue);
          blendShading=additive;
          shade.Pick(red255);
          drawtext=&fx_UglyText::Ugly3d;
         break;
  case uglyOutlined: drawtext=&fx_UglyText::UglyOutlined; break;
  case uglyShadowed: drawtext=&fx_UglyText::UglyShadowed; break;
      case uglyWire: drawtext=&fx_UglyText::UglyWire; break;
  }
 }
 void Ugly3d() {
  Text(out,shade,blendShading,position.x+1,position.y-1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyOutlined() {
  Text(out,shade,blendShading,position.x-1,position.y-1,fw,fh,2.0,lineSpace,centered);
  Text(out,shade,blendShading,position.x+1,position.y+1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyShadowed() {
  Text(out,shade,blendShading,position.x-1,position.y+1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyWire() {
  Text(out.c_str(),tint,blend,(double)position.x,(double)position.y,(double)fw,(double)fh,2.0,(double)lineSpace,centered);
 }
 void Render() {
  Render(this->parent);
 }
 void Render(GLWindow *surface) {
  if ( !hidden ) DrawText();
 }
 void MoveTo( int xx, int yy ) {
  x=xx;
  y=yy;
  position.x=x;
  position.y=y;
 }
 void MoveBy( int xx, int yy ) {
  x+=xx;
  y+=yy;
  position.x=x;
  position.y=y;
 }
};

class fx_UglyTextBox : public Proce55or {
public:
 Zpointer<GLImage> backing;
 // bool moveUpWhenMultilines;
 Cartesian extents,backingExtents;
 Zbool focused,hovering,clicking,wasBackspace,acceptReturn,wasChanged;
 Crayon hover,focus,click,cursor;
 Zint cursorPosition,maxChars;
 Zfloat cursorBlink,keyboardDelay,lastBackspaceRelease,doubleBackDelay,backspacesInARow;
 Vertexd position;
 Zstring out;
 Zpointer<string> bound;
 Zint fw,fh,lineSpace,border;
 Zbool centered,password;
 char passwordChar;
 char passwordbuf[256];
 Zfloat enterDelay;
 Blends blend,blendShading, cursorBlend, hoverBlend, focusBlend;
 Crayon tint,shade;
 UglyDrawStyle style;
 Zint mx,my;
 char c,lastc;
 Zfloat backspaceDelay,eraseDuplicateDelay;
 PROGRAMMABLE(fx_UglyTextBox,DrawTextPtr,drawtext,DrawText);
 virtual void WhenComplete( bool EnterPressed ) { // Called when ENTER or unfocused
 }
 virtual void WhenFocused() {}
 void SetValue( string s ) {
  if ( bound ) *bound=s;
  out=s;
  int totalWidth=(int)((float)out.length*(fw+1));
  extents.x=(int) position.x-totalWidth/2;
  extents.y=(int) position.y-fh/2;
  extents.w=totalWidth;
  extents.h=fh;
  extents.x2=extents.x+extents.w;
  extents.y2=extents.y+fh;
  backingExtents.x=extents.x-border;
  backingExtents.y=extents.y-border;
  backingExtents.w=extents.w+border;
  backingExtents.h=extents.h+border*2;
  backingExtents.x2=backingExtents.x+backingExtents.w;
  backingExtents.y2=backingExtents.y+backingExtents.h;
 }
 void Initialize() {  
  drawtext=&fx_UglyTextBox::UglyWire;
  border=2;
  fw=8;
  fh=10;
  lineSpace=2;
  centered=true;
  blend=blendShading=none;
  hoverBlend=focusBlend=cursorBlend=additive;
  tint.Pick(alabaster);
  shade.Pick(black);
  out=string("");
  duration=10.0f;
  focused=false;
  hovering=false;
  hover.Pick(green);
  focus.Pick(chocolate);
  click.Pick(red255);
  cursor.Pick(gray);
  backing=library.find("data/images/elements/longbar.png");
  cursorBlink=1.0f;
  keyboardDelay=0.0f;
  lastc=c='\0';
  mx=my=-1;
  backspaceDelay=0.0f;
  eraseDuplicateDelay=0.0f;
  maxChars=60;
  doubleBackDelay=0.0f;
  lastBackspaceRelease=1000.0f;
  wasBackspace=false;
  backspacesInARow=0;
  password=false;
  passwordChar='X';
  acceptReturn=false;
  enterDelay=1.0f;
  bound=null;
  wasChanged=false;
  clicking=false;
  cursorPosition=0;
  for ( int i=0; i<256; i++ ) passwordbuf[i]='\0';
  SetStyle(uglyWire);
 }
 fx_UglyTextBox() {
  Constructor();
  Initialize();
 }
 fx_UglyTextBox( string s ) {
  Constructor();
  Initialize();
  SetValue(s);
 }
 fx_UglyTextBox( int x, int y, string s ) {
  Constructor();
  Initialize();
  this->x=x;
  this->y=y;
  SetValue(s);
 }
 fx_UglyTextBox( int x, int y, string s, UglyDrawStyle mode ) {
  Constructor();
  Initialize();
  this->x=x;
  this->y=y;
  SetStyle(mode);
  SetValue(s);
 }
 fx_UglyTextBox( int x, int y, string *s, UglyDrawStyle mode ) {
  Constructor();
  Initialize();
  this->x=x;
  this->y=y;
  SetStyle(mode);
  bound=s;
  SetValue(*s);
 }
 char *Password() {
  int len=out.length;
  for ( int i=0; i<len; i++ ) passwordbuf[i]=passwordChar;
  passwordbuf[out.length]='\0';
  return passwordbuf;
 }
 void Setup() {
  MoveTo(x,y);
 }
 void SetStyle( UglyDrawStyle uds ) {
  style=uds;
  switch ( style ) {
        case ugly3d:
          blend=additive;
          tint.Pick(blue);
          blendShading=additive;
          shade.Pick(red255);
          drawtext=&fx_UglyTextBox::Ugly3d;
         break;
  case uglyOutlined: drawtext=&fx_UglyTextBox::UglyOutlined; break;
  case uglyShadowed: drawtext=&fx_UglyTextBox::UglyShadowed; break;
      case uglyWire: drawtext=&fx_UglyTextBox::UglyWire; break;
  }
 }
 void Ugly3d() {
  Text(out,shade,blendShading,position.x+1,position.y-1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyOutlined() {
  Text(out,shade,blendShading,position.x-1,position.y-1,fw,fh,2.0,lineSpace,centered);
  Text(out,shade,blendShading,position.x+1,position.y+1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyShadowed() {
  Text(out,shade,blendShading,position.x-1,position.y+1,fw,fh,2.0,lineSpace,centered);
  Text(out,tint,blend,position.x,position.y,fw,fh,2.0,lineSpace,centered);
 }
 void UglyWire() {
  Text(out.c_str(),tint,blend,(double)position.x,(double)position.y,(double)fw,(double)fh,2.0,(double)lineSpace,centered);
 }
 void Clean() {
  if ( focused ) Unfocus();
 }
 void Hide() {
  hidden=true;
  if ( focused ) Unfocus();
 }
 void Unhide() {
  hidden=false;
 }
 void Focus();
 void Unfocus();
 void Render() {
  Render(this->parent);
 }
 void Render(GLWindow *surface) {
  if ( hidden ) return;
  if ( focused ) {
   Crayon color;
   color.fromCrayon(cursor);
   color.ScaleByAlphaf(cursorBlink);
   Blending(cursorBlend);
   int ofsx=extents.x+(fw+2)*cursorPosition;
   Area(surface,color,ofsx,backingExtents.y,ofsx+(fw+2),backingExtents.y+backingExtents.h);
   StretchiNoAlpha(surface,backing,focus,focusBlend,backingExtents.x,backingExtents.y,backingExtents.w,backingExtents.h);
  } else
  if ( hovering ) {
   StretchiNoAlpha(surface,backing,hover,hoverBlend,backingExtents.x,backingExtents.y,backingExtents.w,backingExtents.h);
  }
  DrawText();
 }
 void Between() {
  if ( hidden ) return;
  if ( enterDelay > 0.0f ) enterDelay-=FRAMETIME;
  bool shifting=input.KeyDown(DX_LSHIFT)||input.KeyDown(DX_RSHIFT);
  MousePosition(&mx,&my);
  hovering=WITHIN(mx,my,extents.x,extents.y,extents.x2,extents.y2);
  cursorBlink+=FRAMETIME;
  if ( cursorBlink > 1.0f ) cursorBlink=0.0f;
  if ( backspaceDelay > 0.0f ) backspaceDelay-=FRAMETIME;
  if ( eraseDuplicateDelay > 0.0f ) eraseDuplicateDelay-=FRAMETIME;
  if ( focused ) {
   if ( /*input.KeyDown(DX_NUMPADENTER) ||*/ focused && input.KeyDown(DX_RETURN) && enterDelay <= 0.0f ) {
    Unfocus();
    WhenComplete(true);
    enterDelay=1.0f;
   } else
   if ( (input.KeyDown(DX_BACKSPACE) || input.KeyDown(DX_DELETE))
    && backspaceDelay <= 0.0f
    && out.length > 0) {
    char newcontent[STRING_SIZE];
    int i=0;
    int len=(int) out.length;
    const char *p=out.c_str();
    for( ; i<(len-1); i++ ) { newcontent[i]=*p; p++; }
    newcontent[(len-1)]='\0';
    SetValue(string(newcontent));
    cursorPosition=(int)out.length-1;
    backspaceDelay=0.25f;
    wasChanged=true;
    wasBackspace=true;
   } else {
    wasBackspace=false;
    c=input.keyboard(false);
    if ( c != '\0' && (int) out.length < maxChars ) {
     if ( eraseDuplicateDelay <= 0.0f ) lastc='\0';
     if ( c != lastc && UPPER(c) != UPPER(lastc) ) {
      SetValue(out+c);
      wasChanged=true;
      cursorPosition=(int)out.length-1;
      lastc=c;
      eraseDuplicateDelay=0.3f;
     }
    }
   }
   if ( !input.KeyDown(DX_BACKSPACE) && !input.KeyDown(DX_DELETE) && wasBackspace ) {
    backspacesInARow++;
    if ( lastBackspaceRelease > 0.5f && lastBackspaceRelease < 1.0f && backspacesInARow >= 3 ) {
     SetValue(string(""));
     backspacesInARow=0;
    }
    lastBackspaceRelease=0.0f;
    wasBackspace=false;
   }
   lastBackspaceRelease+=FRAMETIME;
  }
 }
 void MoveTo( int xx, int yy ) {
  x=xx;
  y=yy;
  position.x=x;
  position.y=y;
  SetValue(out);
 }
 void MoveBy( int xx, int yy ) {
  x+=xx;
  y+=yy;
  position.x=x;
  position.y=y;
  SetValue(out);
 }
 void MouseDown() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=WITHIN(mx,my,backingExtents.x,backingExtents.y,backingExtents.x2,backingExtents.y2);
 }
 void MouseUp() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=WITHIN(mx,my,extents.x,extents.y,extents.x2,extents.y2);
  if ( hovering ) {
   if ( !focused ) {
    Focus();
   } else {
    if ( input.right ) {
     Unfocus();
    }
   }
  }
 }
 void LeftDown() { MouseDown(); }
 void RightDown(){ MouseDown(); }
 void LeftUp()   { MouseUp(); }
 void RightUp()  { MouseUp(); }
 void MouseMove() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=WITHIN(mx,my,extents.x,extents.y,extents.x2,extents.y2);
 }
};

extern fx_UglyTextBox *fx_UglyTextBoxFocused;
extern bool fx_UglyTextBox_is_focused;
void fx_UglyTextBoxInit();

void UnfocusAllUglyTextboxes();