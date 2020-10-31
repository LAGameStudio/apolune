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

#include "Audio.h"
#include "Interface.h"
#include "Display.h"
#include "GLImage.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "TexFont.h"
#include "LoadingSequence.h"
#include "Tweens.h"
#include "Chemistry.h"

//void LoadOrCreateTestCharacter();

// #define NOLOAD_UNIVERSE 1
// #define NOLOAD_SOUNDS 1
// #define NOLOAD_TEXTURES 1
// #define NOLOAD_TITLES 1
// #define NOLOAD_FLAGS 1
// #define NOLOAD_FONTS 1
// #define NOLOAD_RADIO 1

void DrawLoading( GLImage *loading, HDC *game_dc, HWND *game_window, int w, int h );
void Load( GLWindowManager *windows, Audio *audio, Display *display, Interface *input );

// Locally-used prototypes
void LoadFonts();
void LoadAudio();
void LoadTextures();
void LoadMaterials();
void LoadPrimitives();
void LoadFilms();
void LoadImageModelsAndPointFilms();
void LoadSmilies();
void LoadRadio();
void InventStuff();
void CreateHUD();
void LoadPlayerProfileData();
void LoadScripts();

void LoadWindows();

class MainLoadingSequenceUnthreaded : public LoadingSequence {
 Zpointer<GLImage> splash;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black); // make the screen go black
  splash=library.find("data/images/splash.png");
 }
 void Render() {
  QuadiNoAlpha(splash,crayons.Pick(alabaster),none,0,0,display->w,display->h);
 }
 void Load();
};

class MainLoadingSequence;
class MainSequenceLoadingThread : public LoadingSequenceThread {
public:
 Zpointer<MainLoadingSequence> sequence;
 MainSequenceLoadingThread( MainLoadingSequence *mls );
 void Advance();
 bool Unlocked();
};

class MainLoadingSequence : public ThreadedLoadingSequence {
public:
 Zpointer<MainSequenceLoadingThread> thread;
 Zstring output;
 Zpointer<Tween> pulse;
 Zpointer<GLImage> swab,back,target,blip;
 Cartesian rect,rect2;
 Cartesians blips;
 Crayon drawing,tint,bgTint,targetTint;
 Zint counter,wasCounter;
 Zdouble d;
 Zfloat fadePerc;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black); // make the screen go black
  swab=library.find("data/images/elements/horizRamp.png");
  back=library.find("data/images/nav/radar.png");
  blip=library.find("data/images/flares/neoflare17.png");
  target=library.find("data/images/icons/earth.png");
  rect.SetRect(display->w/2-display->h/4+10,display->h/2-display->h/4+10,display->h/2-20,display->h/2-20);
  rect2.SetRect(display->w/2-display->h/8,display->h/2-display->h/8,display->h/4,display->h/4);
  bgTint.Int(0,255,0,255);
  targetTint.Int(127,255,127,255);
  wasCounter=counter=0;
  d=0.0f;
  fadePerc=1.0f;
  pulse=tweens.find("Humanized In Out");
  output=string("Software booting.\n");
#pragma warning( push ) 
#pragma warning( disable : 4101 )
  NEW(thread, MainSequenceLoadingThread(this));
#pragma warning( pop )
  pthreads.New(thread);
 }
// void OnUnload() {
//  pthreads.Remove(thread);
//  delete thread;
// }
 void AdvanceCounter();
 void Between();
 void Render();
};