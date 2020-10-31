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

#include "GLWindow.h"
#include "GLImage.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Art.h"
#include "Audio.h"

#include "Proce55Controller.h"
#include "Proce55or.h"
#include "fx_TexFont.h"

#include "TextureLibrary.h"
#include "Load.h"

#include "moremath.h"

/********************
 * fx_IntegerSelect *
 ********************/

class fx_IntegerSelect : public Proce55or {
public:
 Zpointer<int> binding;
 Zpointer<GLImage> left,right;
 Zpointer<fx_TexFont> tf;
 Crayon color;
 Zpointer<TexFont> font;
 Zint fw,fh, Lx,Ry;
 Zbool hoverU,hoverD;
 Zint min,max;

 fx_IntegerSelect() : Proce55or() {
  binding=null;
  left=right=null;
  tf=null;
  color.Pick(alabaster);
  font=null;
  fw=fh=Lx=Ry=0;
  hoverU=hoverD=null;
  min=max=0;
 }

 void Clean() { if ( tf ) delete tf; }

 void Update() {
  delete tf;
  FORMAT( buf, 128, "%10d", (int)(*binding) );
  tf = new fx_TexFont;
  tf->color=color;
  tf->font=font;
  tf->fw=fw;
  tf->fh=fh;
  tf->x=0;
  tf->y=0;
  tf->Text(buf,parent);
  tf->Reset(false);
  Lx=fw*10; Ry=h/2;
  w=fw*10+fw;
 }
 
 void Setup() {
  left=library.find("data/images/elements/uparrow.png");
  right=library.find("data/images/elements/downarrow.png");
 }

 void Draw(GLWindow *surface) {
  Crayon tint=crayons.Pick( hoverU ? hotPink : aliceBlue );
  Stretchi(surface,left,tint,1.0f,additive,Lx,0,fw,Ry);
  tint=crayons.Pick( hoverD ? hotPink : aliceBlue );
  Stretchi(surface,right,tint,1.0f,additive,Lx,0,fw,Ry);
  tf->Draw(surface);
 }

 void MouseMove(int mx, int my, Modifiers mods) {
  hoverU=WITHIN(mx,my,Lx,0,w,Ry);
  hoverD=WITHIN(mx,my,Lx,Ry,w,h);
 }

 void LeftDown(int mx, int my, Modifiers mods) {
  hoverU=WITHIN(mx,my,Lx,0,w,Ry);
  hoverD=WITHIN(mx,my,Lx,Ry,w,h);
  if ( hoverU ) (*binding)++;
  if ( hoverD ) (*binding)--;
  if ( *binding < min ) *binding = max;
  if ( *binding > max ) *binding = min;
 }
};

/******************
 * fx_FloatSelect *
 ******************/

class fx_FloatSelect : public Proce55or {
public:
 Zpointer<float> binding;
 Zfloat delta;
 Zpointer<GLImage> left, right;
 Zpointer<fx_TexFont> tf;
 Crayon color;
 Zpointer<TexFont> font;
 Zint fw,fh, Lx,Ry;
 Zbool hoverU,hoverD;
 Zfloat min,max;

 fx_FloatSelect() : Proce55or() {
  color.Pick(alabaster);
  fw=fh=Lx=Ry;
  hoverU=hoverD=false;
  min=max=0.0f;
 }

 void Clean() { }

 void Update() {
  tf.Delete();
  FORMAT( buf,128, "%1.2f", (int)(*binding) );
  tf = new fx_TexFont;
  tf->color=color;
  tf->font=font;
  tf->fw=fw;
  tf->fh=fh;
  tf->x=0;
  tf->y=0;
  tf->Text(buf,parent);
  tf->Reset(false);
  Lx=fw*10; Ry=h/2;
  w=fw*10+fw;
 }
 
 void Setup() {
  left=library.find("data/images/elements/uparrow.png");
  right=library.find("data/images/elements/downarrow.png");
 }

 void Draw(GLWindow *surface) {
  Crayon tint;
  tint=crayons.Pick( hoverU ? hotPink : aliceBlue );
  Stretchi(surface,left,tint,1.0f,additive,Lx,0,fw,Ry);
  tint=crayons.Pick( hoverD ? hotPink : aliceBlue );
  Stretchi(surface,right,tint,1.0f,additive,Lx,0,fw,Ry);
  tf->Draw(surface);
 }

 void MouseMove(int mx, int my, Modifiers mods) {
  hoverU=WITHIN(mx,my,Lx,0,w,Ry);
  hoverD=WITHIN(mx,my,Lx,Ry,w,h);
 }

 void LeftDown(int mx, int my, Modifiers mods) {
  hoverU=WITHIN(mx,my,Lx,0,w,Ry);
  hoverD=WITHIN(mx,my,Lx,Ry,w,h);
  if ( hoverU ) (*binding)++;
  if ( hoverD ) (*binding)--;
  if ( *binding < min ) *binding = max;
  if ( *binding > max ) *binding = min;
 }
};
