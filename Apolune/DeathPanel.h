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
#pragma once

#include "Campaign.h"
#include "Player.h"

#include "Art2d.h"
#include "fx_Teletype.h"

class DeathPanel : public GLWindow {
public:
 Zp<PresentationProfile> presiProfi;
 Zdis<Presentation> presi;
 Zstring reason,sound;
 fx_Teletype message1,message2;
 Cartesian messageAreaBox,loadGameBtn,quitMenuBtn;
 Zbool playOnce;
 Zp<FBOColor> fboPresi;
 DeathPanel( const char *reason="", const char *sound="", const char *presi="" ) : GLWindow() {
  this->reason=reason;
  if ( file_exists(sound) ) this->sound=sound;
  this->presiProfi=presentationProfiles.firstNamed(presi);
  if ( this->presiProfi ) this->presi.Recycle(presiProfi->Create());
  quitMenuBtn.SetRect(16,display->h-16-64,512,64);
  loadGameBtn.SetRect(display->w-16-512,display->h-16-64,512,64);
  fboPresi=&fbo1080p2;
  message1.delay=0.0;
  message1.ink.Pick(rgbF00);
  message1.font=bytes;
  message1.kern=8;
  message1.fontW=48;
  message1.fontH=64;
  message1.centered=true;
  message1.stayOnDone=true;
  message1.MoveTo(display->w/2,display->h/2-64);
  message1.WriteText("YOU HAVE PERISHED",true);
  message2.delay=3.0;
  message2.ink.Pick(rgbF44);
  message2.MoveTo(display->w/2,display->h/2);
  message2.WriteText(reason,true);
  message2.stayOnDone=true;
  messageAreaBox.SetRect(
   display->w/2-message2.bounds.w/2-32,
   display->h/2-message2.bounds.h/2-80,
   message2.bounds.w+64,
   message2.bounds.h+96
  );
  input->hideCursor=false;
 }
 void OnLoad() {
  Fullscreen();
  background.Pick(black);
  dJ.FadeToSilence(0.1);
  dJ.CutTo("track_death",0.1,false);
  if ( sound.length > 0 ) {
   audio.manager.CueAndPlay(sound.c_str(),1.0,1.0);
  }
 }
 void Between() {
  if ( destroyMe || !visible ) return;
  BetweenPresi();
 }
 void Render() {
  if ( destroyMe || !visible ) return;
  fast.ResponsiveRatio(w,h);
  RenderPresi();
  fast.bestrect(crayons.Pick(rgb0006),messageAreaBox.x-16,messageAreaBox.y-16,messageAreaBox.w+32,messageAreaBox.h+32);
  message1.Render();
  message2.Render();
  if ( fast.button(digicaps,"QUIT TO MENU",quitMenuBtn.x,quitMenuBtn.y,quitMenuBtn.w,quitMenuBtn.h) ) {
   QuitMenu();
   return;
  }
  if ( fast.button(digicaps,"LAST CHECKPOINT",loadGameBtn.x,loadGameBtn.y,loadGameBtn.w,loadGameBtn.h) ) {
   LastCheckpoint();
   return;
  }
 }
 void QuitMenu();
 void LastCheckpoint();
 void BetweenPresi() {
  if ( !presi ) return;
  presi->Between();
  presi->BetweenFrames();
  if ( presi->looped > 0 ) {
   presi.Deinstantiate();
   if ( !playOnce ) {
    presi.Recycle(presiProfi->Create());
    presi->loop=presiProfi->looping;
    presi->freezeLast=presiProfi->freezeLast;
    presi->Init(false);
    presi->Between();
    presi->BetweenFrames();
   }
  }
 }
 void RenderPresi() {
  if ( !presi ) return;
  art.PushViewport(fboPresi);
  fboPresi->background.fromCrayon(presiProfi->background);
  fboPresi->Bind();
  presi->Render();
  fboPresi->Unbind();
  art.PopViewport();
  fboPresi->blend=presiProfi->blend;
  fboPresi->tint.Pick(alabaster);
  fboPresi->RenderUpsideDown(0,0,w.value,h.value);
 }
};

