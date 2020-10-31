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
 ***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once

#include <Windows.h>

#include "macros.h"
#include "moremath.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Crayon.h"

#include "fx_Buttons.h"
#include "Presentation.h"
#include "TextureLibrary.h"

#include "HUDOverlay.h"

#include "GameOptions.h"

extern Zbool builderignoreclick;
extern bool HUD_GUI_LOCKED;

class GUILockButton : public fx_CoolButtonToggle {
public:
 virtual void Customize() {
  w=32;
  h=32;
  DoNotShowBackingNormal=true;
  normal   = crayons.Pick(chocolate);
  t_normal = crayons.Pick(alabaster);
  f_normal = crayons.Pick(chocolate);
  click    = crayons.Pick(red255);
  t_click  = crayons.Pick(alabaster);
  f_click  = crayons.Pick(red255);
  hover    = crayons.Pick(green);
  t_hover  = crayons.Pick(alabaster);
  f_hover  = crayons.Pick(green);
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  glow_c  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
  buttontext = HUD_GUI_LOCKED ? library.find( "data/images/elements/locked.png" ) : library.find( "data/images/elements/unlocked.png");
  delay=0.0f;
  duration=20.0f;
  sound_hover=string( "data/sounds/UI/bonk.wav" );
  HUD_GUI_LOCKED ? sound_click=string( "data/sounds/UI/unlocked.wav" ) : sound_click=string("data/sounds/UI/locked.wav" );
 }
 HUDOverlayManager *manager;
 void WhenClicked() {
  HUD_GUI_LOCKED = !HUD_GUI_LOCKED;
  buttontext = HUD_GUI_LOCKED ? library.find( "data/images/elements/locked.png" ) : library.find( "data/images/elements/unlocked.png");
  HUD_GUI_LOCKED ? sound_click=string("data/sounds/UI/unlocked.wav") : sound_click=string("data/sounds/UI/locked.wav" );
 }
};

class OptionsToggle : public fx_CoolButton {
public:
 virtual void Customize() {
  w=16;
  h=16;
  DoNotShowBackingNormal=true;
  normal   = crayons.Pick(chocolate);
  t_normal = crayons.Pick(alabaster);
  f_normal = crayons.Pick(chocolate);
  click    = crayons.Pick(red255);
  t_click  = crayons.Pick(alabaster);
  f_click  = crayons.Pick(red255);
  hover    = crayons.Pick(green);
  t_hover  = crayons.Pick(alabaster);
  f_hover  = crayons.Pick(green);
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  glow_c  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_h  = library.find("data/images/elements/buttonglow_1024_4.png");
  glow_n  = library.find("data/images/elements/buttonglow_1024_0.png");
  buttontext = library.find( "data/images/flags/barretr_Gear.png");
  delay=0.0f;
  duration=20.0f;
  sound_hover=string( "data/sounds/UI/bonk.wav" );
  sound_click=string( "data/sounds/UI/locked.wav" );
 }
 void WhenClicked();
};


class HUD : public HUDOverlayManager {
public:
 Zpointer<HUDObject> ho;
 Zpointer<GUILockButton> glb;
 Zpointer<OptionsToggle> ot;
 Zbool keypressed,uppressed;

 HUD() : HUDOverlayManager() {}
 void Repopulate();

 void BetweenFrames();

 // Map interface to appropriate children
 void MouseMove();
 void LeftDown();
 void RightDown();
 void LeftUp();
 void RightUp();
};

extern Zpointer<HUD> hud;