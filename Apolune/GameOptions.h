/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
/***********************************************************************************************
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

#include "SpecialPaths.h"
#include "DJ.h"
#include "FormUI.h"
#include "PixelDoubler.h"
#include "ControlOptions.h"

extern Zp<GLWindow> activeEngineWindow;

class GameMenu;

class GameOptions {
public:
 GameOptions() {
  maxParticles=2000;
  musicVolume=0.5;
  overdubVolume=1.0;
  soundsVolume=1.0;
  maxPlayers=0;
 }
 void SetGlobalPointer();
 void ResetToDefaults() {
  maxParticles=2000;
  musicVolume=0.5;
  overdubVolume=1.0;
  soundsVolume=1.0;
  maxPlayers=0;
  p2xShading=true;
  vibrate=true;
  friendsOnly=true;
 }

 // CPU limitations
 Zint maxParticles;      // Limits mulligans to a certain maximum limit globally.

 // GPU limitations
 Zbool asciiShading,p2xShading;  // Tells the shader engine to use ASCII shading.
 Zbool hiresShaders;  // High resolution shading.
 Zbool noShadersOnPanels;

 // Audio options
 Zdouble musicVolume,overdubVolume,soundsVolume;
 Zdouble muted_musicVolume,muted_overdubVolume,muted_soundsVolume;
 Zbool mute;

 // GUI Options

 // Gameplay Options
 Zbool autoaim,vibrate,scrollDownOnlyText;
 
 // Network Settings
 Zbool friendsOnly; // Allow only friends to join your game
 Zbool autojoin;    // Allow players to join your game without approval
 Zint  maxPlayers;  // Limit the number of players who can join

 PixelDoublerTypes pixelDoublerMode;
 
 WORDKEYS({
  NUMBKEY("particles",maxParticles)
  TAGKEY("ascii",asciiShading)
  TAGKEY("p2x",p2xShading)
  TAGKEY("mute",mute)
  NUMBKEY("music",musicVolume)
  NUMBKEY("overdub",overdubVolume)
  NUMBKEY("sound",soundsVolume)
  NUMBKEY("m_music",muted_musicVolume)
  NUMBKEY("m_overdub",muted_overdubVolume)
  NUMBKEY("m_sound",muted_soundsVolume)
  TAGKEY("autoaim",autoaim)
  TAGKEY("friendsOnly",friendsOnly)
  TAGKEY("autojoin",autojoin)
  TAGKEY("vibrate",vibrate)
  NUMBKEY("maxPlayers",maxPlayers)
  ENUMKEY("pixelDoubler",pixelDoublerMode)
  TAGKEY("noShadersOnPanels",noShadersOnPanels)
 })
 KEYWORDS({
  NUMWORD("particles",maxParticles)
  else TAGWORD("ascii",asciiShading)
  else TAGWORD("p2x",p2xShading)
  else TAGWORD("mute",mute)
  else NUMWORD("music",musicVolume)
  else NUMWORD("overdub",overdubVolume)
  else NUMWORD("sound",soundsVolume)
  else NUMWORD("m_music",muted_musicVolume)
  else NUMWORD("m_overdub",muted_overdubVolume)
  else NUMWORD("m_sound",muted_soundsVolume)
  else TAGWORD("autoaim",autoaim)
  else TAGWORD("friendsOnly",friendsOnly)
  else TAGWORD("autojoin",autojoin)
  else TAGWORD("vibrate",vibrate)
  else NUMWORD("maxPlayers",maxPlayers)
  else ENUMWORD("pixelDoubler",pixelDoublerMode,PixelDoublerTypes)
  else TAGWORD("noShadersOnPanels",noShadersOnPanels)
 })
 void Load() {
  string file=specialPaths.historic("options.txt");
  if ( !file_exists(file.c_str()) ) ResetToDefaults();
  else fromString(file_as_string(file.c_str()).c_str());
  ApplyVolume();
 }
 void Save() {
  string_as_file(toString().c_str(),specialPaths.historic("options.txt").c_str());
 }
 void ApplyVolume() {
  dJ.SetMusicVolume(musicVolume);
  dJ.SetOverdubVolume(overdubVolume);
//  dJ.SetSamplerVolume(soundsVolume);
 }
 void MuteToggle() {
  if ( mute ) {
   muted_musicVolume=musicVolume;
   muted_overdubVolume=overdubVolume;
   muted_soundsVolume=soundsVolume;
   musicVolume=0.0;
   overdubVolume=0.0;
   soundsVolume=0.0;
  } else {
   musicVolume=muted_musicVolume;
   overdubVolume=muted_overdubVolume;
   soundsVolume=muted_soundsVolume;
  }
  ApplyVolume();
 }
};

extern GameOptions options;
extern Zp<GameOptions> gameOptions;

class GameOptionsEditor : public MoveableEditorWindow {
public:
 Strings modes;
 void OnSetup() {
  clip=false;
  v.background.Pick(black);
  v.noMove=false;
  v.noMinimize=true;
  v.noMaximize=true;
  v.noResize=true;
  v.noTitle=false;
  v.noTitleBar=false;
  v.noBacking=false;
  v.alwaysOnTop=false;
  v.noClose=true;
  v.noCenter=false;
  v.noClipTitle=true;
  v.minimumSize.Set(768,600);
  v.maximumSize.Set(768,600);
  v.SizeAndPosition(1920/2-384,1080/2-300,768,600);
  Name("Options");
  PixelDoubler::DeployTypes(&modes);
 }
 void Between() {
 }
 void RenderWindow() {
  Cartesian pen;
  pen.Set(x+16,y+64);
  bool was_mute=options.mute;
  options.mute=fast.toggle("Mute",options.mute,pen.x+w-160,pen.y-32,24,24);
  if ( was_mute != options.mute ) options.MuteToggle();
  if ( options.mute ) {
   Crayon wasHighlight;
   wasHighlight.fromCrayon(fast.highlight);
   fast.highlight.Pick(gray);
   fast.hslider("Music Volume",options.musicVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
   pen.y+=64+16;
   fast.hslider("Voices (Overdub Volume)",options.overdubVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
   pen.y+=64+16;
   fast.hslider("Sound FX Volume",options.soundsVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
   pen.y+=64;
   fast.highlight.fromCrayon(wasHighlight);
  } else {
   double was_musicVolume=options.musicVolume;
   options.musicVolume=fast.hslider("Music Volume",options.musicVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
   if( was_musicVolume != options.musicVolume ) dJ.SetMusicVolume(options.musicVolume);
   pen.y+=64+16;
   double was_overdubVolume=options.overdubVolume;
   options.overdubVolume=fast.hslider("Voices (Overdub Volume)",options.overdubVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
   if( was_overdubVolume != options.overdubVolume ) dJ.SetOverdubVolume(options.overdubVolume);
   pen.y+=64+16;
   double was_soundsVolume=options.soundsVolume;
   options.soundsVolume=fast.hslider("Sound FX Volume",options.soundsVolume,0.0,1.0,pen.x,pen.y,w-32,32,true);
  // if( was_soundsVolume != options.soundsVolume ) dJ.SetSamplerVolume(options.soundsVolume);
   pen.y+=64;
  }
  pen.y+=64;
  options.maxParticles=fast.hslider("CPU Particle Limit",options.maxParticles,200,4000,pen.x,pen.y,w/2,32,true);
  options.pixelDoublerMode=(PixelDoublerTypes)fast.enumerated(&modes,"Pixel Doubling",(int)options.pixelDoublerMode,x+w-192,pen.y-16-64+8,48,24);
  options.noShadersOnPanels=fast.toggle("No Panel Shader FX",options.noShadersOnPanels,x+16,pen.y-16-64+8,16,16);
  options.scrollDownOnlyText=fast.toggle("Text Scroll Down",options.scrollDownOnlyText,x+256+16,pen.y-16-64+8,16,16);
  options.asciiShading=fast.toggle("ASCII","PIXEL","Roguelike",options.asciiShading,x+w-192,pen.y-16,24,48);
  options.p2xShading=fast.toggle("2X",options.p2xShading,pen.x+w-128,pen.y+32,16,16);
  pen.y+=64;
  options.maxPlayers=fast.integer("MAX NET PLAYERS",options.maxPlayers,0,4,1,pen.x,pen.y+16,48,24,3,false);
  options.friendsOnly=fast.toggle("Only friends can join your game",options.friendsOnly,pen.x+160 ,pen.y,24,24);
  pen.y+=32;
  options.autojoin=fast.toggle("Allow players to automatically join",options.autojoin,pen.x+160,pen.y,24,24);
  pen.y+=32;
  if ( fast.button("Setup or Change Controls",pen.x,pen.y,768-32,32) ) {
   VirtualWindowHandle *existing=v.holder->Editing(&controls);
   if ( existing ) {
    v.holder->SlideTo(existing->p);
   } else {
    ControlOptionsEditor *coe=new ControlOptionsEditor;
    v.holder->Add(&coe->v);
    coe->Setup();
    v.holder->SlideTo(&coe->v);
   }
  }
  pen.y+=32+8;
  options.vibrate=fast.toggle("Vibrate",options.vibrate,pen.x,pen.y,24,24);
  //if ( fast.button("Save Game and Exit to Menu",pen.x,pen.y,600-32,32) ) {
  //}
 }
};

class GameOptionsPanel : public MoveableWindowManager {
public:
 Zp<GLWindow> back_to;
 Delay saveStateInterval;
 Zp<GameOptionsEditor> oe;
 Zbool back_to_is_game_menu;
 Zstring statefile;
 Zbool alreadyReleased;
 GameOptionsPanel() : MoveableWindowManager() {
  autoarrange=false;
  vgui.drawWindowControls=false;
  statefile=specialPaths.historic("options.panel.windows.txt");
 }
 void LoadEditors() {
  oe=new GameOptionsEditor;
  Add(oe);
  back_to_is_game_menu = back_to->is("GameMenu") ? true : false;
  if ( file_exists(statefile.c_str()) ) vgui.LoadState(statefile.c_str());
  else vgui.SaveState(statefile.c_str());
 }
 void RenderBackground() {
  fast.ResponsiveRatio(fbo->w,fbo->h);
  fast.ljust(digicaps,"Game Options",16,32,32,32,2,3,crayons.Pick(aliceBlue),additive);
 }
 void ReturnToGameMenu();
 void ResumeGame();
 void RenderAfterWindow() {
  if ( back_to ) {
   if ( !back_to_is_game_menu && fast.button("Quit to Menu",fbo->w-192-32-8,fbo->h-64-8,192,32,false) ) {
    ReturnToGameMenu();
    return;
   }
   if ( ( alreadyReleased && input->KeyUp(DX_ESCAPE) ) || controls.Detected("Bk") || fast.button("<- RESUME GAME",fbo->w-192-8,8,192,32,false) ) {
    ResumeGame();
    return;
   }
   if ( input->KeyUp(DX_ESCAPE) ) alreadyReleased=true;
  }
 }
};