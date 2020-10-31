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

#include "FastGUI.h"
#include "Proce55or.h"
#include "Scripting.h"
#include "ScriptableProce55or.h"

class fx_FastGUIButtonScriptable : public ScriptableProce55or {
public:
 Consulate pressed;
 Zp<GLImage> backing;
 Zp<TexFont> font;
 Zstring label;
 Zbool selected;
 Cartesian fontSize;
 Cartesian extents;
 Blends blend;
 fx_FastGUIButtonScriptable( Consulate *es ) : ScriptableProce55or( es ) {
  pressed.variables=es->variables;
  backing=library.Load("data/images/elements/white.png");
  font=defaultTexFont;
  fontSize.Set(8,8);
  blend=transparency;
 }
 void ButtonPressed() {
  pressed.Execute();
 }
 void Render() {
  if ( fast.button(
    backing,font,label.c_str(),
    x,y,w,h,
    fontSize.x,fontSize.y,
    selected.value,blend
   ) ) {
   ButtonPressed();
  }
 }
 void ScriptModify( const char *in ) {
  Zstring subwords(in);
  const char *p=subwords.Next();
  while ( *p != '\0' ) {
   if ( !str_cmp(p,"font") ) {
    p=subwords.Next();
    string f=consulate->variables->EvaluateString(p);
    font=texFontByName(f.c_str());
   } else if ( !str_cmp(p,"label") || !str_cmp(p,"text") || !str_cmp(p,"content") ) {
    p=subwords.Next();
    label=consulate->variables->EvaluateString(p);
   } else if ( !str_cmp(p,"group") ) {
    p=subwords.Next();
    grouping=consulate->variables->EvaluateInteger(p);
   } else if ( !str_cmp(p,"font.size") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    fontSize.SetRect(xx,yy,(ww == 0 ? fontSize.w : ww),(hh == 0 ? fontSize.h : hh));
   } else if ( !str_cmp(p,"font.size:cartesian") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCartesian(p,&fontSize);
   } else if ( !str_cmp(p,"cartesian") || !str_cmp(p,"extents:cartesian") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCartesian(p,&extents);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"size") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    SetExtents(x,y,ww,hh);
   } else if ( !str_cmp(p,"position") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    SetExtents(xx,yy,w,h);
   } else if ( !str_cmp(p,"extents") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    SetExtents(xx,yy,ww,hh);
   } else if ( !str_cmp(p,"blend") ) {
    p=subwords.Next();
    blend=consulate->variables->EvaluateBlend(p);
   } else Proce55or_ScriptModify(subwords.self(),&p);
   p=subwords.Next();
  }
 }
};

class fx_FastGUIIconScriptable : public ScriptableProce55or {
public:
 Consulate pressed;
 Zp<GLImage> icon,backing;
 Zbool selected;
 Cartesian extents;
 Blends blend;
 Crayon color_back,color_icon,color_disabled;
 fx_FastGUIIconScriptable( Consulate *es ) : ScriptableProce55or( es ) {
  pressed.variables=es->variables;
  icon=library.Load("data/images/elements/white.png");
  backing=library.Load("data/images/elements/white.png");
  blend=transparency;
  extents.SetRect(0,0,64,64);
  color_back.Pick(cyan);
  color_icon.Pick(alabaster);
  color_disabled.Pick(gray);
 }
 void ButtonPressed() {
  pressed.Execute();
 }
 void Render() {
  if ( fast.button(icon,
    backing,
    x,y,w,h,
    color_back,color_icon,color_disabled,
    selected.value,blend
   ) ) {
   ButtonPressed();
  }
 }
 void ScriptModify( const char *in ) {
  Zstring subwords(in);
  const char *p=subwords.Next();
  while ( *p != '\0' ) {
   if ( !str_cmp(p,"label") || !str_cmp(p,"icon") ) {
    p=subwords.Next();
    consulate->variables->EvaluateImage(p,&icon.pointer);
   } else if ( !str_cmp(p,"cartesian") || !str_cmp(p,"extents:cartesian") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCartesian(p,&extents);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"size") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    extents.SetRect(extents.x,extents.y,ww,hh);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"position") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    extents.SetRect(xx,yy,extents.w,extents.h);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"extents") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    extents.SetRect(xx,yy,ww,hh);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"tint") || !str_cmp(p,"crayon") || !str_cmp(p,"color") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCrayon(p,&color_back);
   } else if ( !str_cmp(p,"icon:tint") || !str_cmp(p,"icon:crayon") || !str_cmp(p,"icon:color") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCrayon(p,&color_icon);
   } else if ( !str_cmp(p,"disabled:tint") || !str_cmp(p,"disabled:crayon") || !str_cmp(p,"disabled:color") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCrayon(p,&color_disabled);
   } else if ( !str_cmp(p,"blend") ) {
    p=subwords.Next();
    blend=consulate->variables->EvaluateBlend(p);
   } else Proce55or_ScriptModify(subwords.self(),&p);
   p=subwords.Next();
  }
 }
};

class fx_FastGUITextScriptable : public ScriptableProce55or {
public:
 Zp<TexFont> font;
 Zstring label;
 Zbool selected;
 Cartesian fontSize;
 Cartesian extents;
 Crayon tint;
 Blends blend;
 Zp<Consulate> consulate;
 fx_FastGUITextScriptable( Consulate *es ) : ScriptableProce55or( es ) {
  font=defaultTexFont;
  fontSize.Set(8,8);
  label="window::text";
  blend=transparency;
 }
 void Render() {
  fast.text(font,label.c_str(),x.value,y.value,fontSize.x,fontSize.y,fontSize.w,fontSize.h,tint,blend);
 }
 void ScriptModify( const char *in ) {
  Zstring subwords(in);
  const char *p=subwords.Next();
  while ( *p != '\0' ) {
   if ( !str_cmp(p,"name") ) {
    p=subwords.Next();
    name=consulate->variables->EvaluateString(p);
   } else if ( !str_cmp(p,"font") ) {
    p=subwords.Next();
    string f=consulate->variables->EvaluateString(p);
    font=texFontByName(f.c_str());
   } else if ( !str_cmp(p,"font.size") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    fontSize.SetRect(xx,yy,(ww == 0 ? fontSize.w : ww),(hh == 0 ? fontSize.h : hh));
   } else if ( !str_cmp(p,"font.size:cartesian") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCartesian(p,&fontSize);
   } else if ( !str_cmp(p,"label") || !str_cmp(p,"text") || !str_cmp(p,"content") ) {
    p=subwords.Next();
    label=consulate->variables->EvaluateString(p);
   } else if ( !str_cmp(p,"group") ) {
    p=subwords.Next();
    grouping=consulate->variables->EvaluateInteger(p);
   } else if ( !str_cmp(p,"cartesian") || !str_cmp(p,"extents:cartesian") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCartesian(p,&extents);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"size") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    extents.SetRect(extents.x,extents.y,ww,hh);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"position") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    extents.SetRect(xx,yy,extents.w,extents.h);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"extents") ) {
    p=subwords.Next();
    Zstring subwords(p);
    const char *d=subwords.Next();
    int xx=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int yy=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int ww=consulate->variables->EvaluateInteger(d);
    d=subwords.Next();
    int hh=consulate->variables->EvaluateInteger(d);
    extents.SetRect(xx,yy,ww,hh);
    SetExtents(extents.x,extents.y,extents.w,extents.h);
   } else if ( !str_cmp(p,"tint") || !str_cmp(p,"crayon") || !str_cmp(p,"color") ) {
    p=subwords.Next();
    consulate->variables->EvaluateCrayon(p,&tint);
   } else if ( !str_cmp(p,"blend") ) {
    p=subwords.Next();
    blend=consulate->variables->EvaluateBlend(p);
   } else if ( !str_cmp(p,"hide") ) {
    hidden=true;
   } else if ( !str_cmp(p,"show") ) {
    hidden=false;
   } else Proce55or_ScriptModify(subwords.self(),&p);
   p=subwords.Next();
  }
 }
};