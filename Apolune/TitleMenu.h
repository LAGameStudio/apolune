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

#include "GLWindow.h"
#include "GLImage.h"
#include "Display.h"
#include "Interface.h"
#include "Audio.h"

#include "Crayon.h"
#include "TextureLibrary.h"
#include "Art.h"
#include "Tweens.h"
#include "TexFont.h"
#include "Presentation.h"

#include "Proce55or.h"
#include "fx_MixedBag.h"
#include "fx_Glow.h"
#include "fx_Buttons.h"
#include "VisualAid.h"


//import "texteffects"
//import "credits"

#define SHOW 1

#define NUM_PARTICLES 80//100//80//400//600
#define NUM_STARS 100//200

class LoginButton : public fx_CoolButton {
 void Customize() {
  sound_hover=string( "data/sounds/UI/bleep.wav" );
  sound_click=string( "data/sounds/UI/bloop.wav" );
  controller.deltaf*=2;
 }
 void WhenClicked();
};

class OptionsButton : public fx_CoolButton {
 void Customize() {
  sound_hover=string( "data/sounds/UI/bleep.wav" );
  sound_click=string( "data/sounds/UI/bloop.wav" );
  controller.deltaf*=2;
 }
 void WhenClicked();
};

class ExitButton : public fx_CoolButton {
 void Customize() {
  sound_hover=string( "data/sounds/UI/bleep.wav" );
  sound_click=string( "data/sounds/UI/bloop.wav" );
  controller.deltaf*=2;
 }
 void WhenClicked();
};

class MainMenuActual : public Presentation {
public:
 MainMenuActual( Display *d, Interface *i, Audio *a ) : Presentation(d,i,a) { }

 void OnLoad();

 bool OnComplete();

};

class MainMenu : public GLWindow {
public:
 Zpointer<MainMenuActual> menu; 
 Zbool stayClosed;

 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  Repopulate();
 }

 void Repopulate() {
  stayClosed=false;
  DropChildren();
  AddChild(menu=new MainMenuActual(display,input,&audio));
  menu->visible=false;
 }

 void MouseMove() {
  if ( !visible ) return;
  if ( stayClosed ) return;
  cursor=true;
  menu->visible=true;
  if ( menu->paused ) menu->Rewind(false);
  manager->send=menu;
 }

 void RightDown() {
  if ( !visible ) { return; }
  if ( menu && !stayClosed ) menu->Seek(3);
 }

 void LeftDown() {
  if ( !visible ) return;
  if ( menu && menu->current != null ) return;
  menu->Rewind(false);
 }
};

extern Zpointer<MainMenu> mainmenu;