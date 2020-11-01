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
/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once
#include "CommandLine.h"

#include "GLWindow.h"
#include "GameOptions.h"
#include "NewGamePanel.h"

#include "SpaceShaders.h"

#include "Game.h"

extern Zbool allowCheaters, spaceGameJunkie, hunterCheat, alienPickleCheat, spazCheat;

class GameMenu : public GLWindow {
public:
 ElectronCloudsProfile electron;
 Zdis<Presentation> p,p2;
 Zbool playingTitle;
 Strings options;
 Zp<FBOColor> fbo;
 Zp<GLImage> title;
 Zint minute;
 Zp<TwoDeeElement> rrs;
 Zdouble time;
 void OnLoad() {
  electron.time = 5.0;
  ShipImageSet temp; temp.Load("data/images/textures", true);
  input->hideCursor = false;
  title = library.Load("data/images/title.png");
  fast.DefaultState();
  Fullscreen();
  input->cursorFilm=vault.find("cursors","cursor");
  background.Pick(black);
  fbo=&fbo1080p2;
  fast.ResponsiveRatio(fbo1080p2.w,fbo1080p2.h);
  dJ.Play("track_operatic",0.25,true);
  options.Add("Play");
  options.Add("Exit");
  rrs = art.stash.rrs_filledRect_FlipY;
#if defined(DEBUG)
  allowCheaters = true;
#else
  allowCheaters = CmdLine.Option("-cheating") != null;
  if ( allowCheaters ) ACHIEVE("APOLUNE_18");
#endif
 }
 void Between() {
  SYSTEMTIME myTime;
  GetLocalTime(&myTime);
  minute = (int)myTime.wMinute;
  if (allowCheaters) {
   if (!spaceGameJunkie) {
    if (input->KeyDown(DX_S)
     && input->KeyDown(DX_G)
     && input->KeyDown(DX_J)) {
     dJ.FadeToSilence(0.1);
     audio.manager.CueAndPlay("data/sounds/spoken/SGJbryan.wav",1.0,2.0);
     spaceGameJunkie = true;
     if (spaceGameJunkie&&hunterCheat&&alienPickleCheat&&spazCheat) ACHIEVE(true, "APOLUNE_19");
    }
   }
   if (!hunterCheat) {
    if (input->KeyDown(DX_H)
     && input->KeyDown(DX_U)
     && input->KeyDown(DX_N)
     && input->KeyDown(DX_T)
     && input->KeyDown(DX_E)
     && input->KeyDown(DX_R)) {
     dJ.FadeToSilence(0.1);
     audio.manager.CueAndPlay("data/sounds/spoken/SGJhunter.wav", 1.0, 2.0);
     hunterCheat = true;
     if (spaceGameJunkie&&hunterCheat&&alienPickleCheat&&spazCheat) ACHIEVE(true, "APOLUNE_19");
    }
   }
   if (!alienPickleCheat) {
    if (input->KeyDown(DX_A)
     && input->KeyDown(DX_L)
     && input->KeyDown(DX_I)
     && input->KeyDown(DX_E)
     && input->KeyDown(DX_N)) {
     dJ.FadeToSilence(0.1);
     audio.manager.CueAndPlay("data/sounds/spoken/SGJjim.wav", 1.0, 2.0);
     alienPickleCheat = true;
     if (spaceGameJunkie&&hunterCheat&&alienPickleCheat&&spazCheat) ACHIEVE(true, "APOLUNE_19");
    }
   }
   if (!spazCheat) {
    if (input->KeyDown(DX_S)
     && input->KeyDown(DX_P)
     && input->KeyDown(DX_A)
     && input->KeyDown(DX_Z)) {
     dJ.FadeToSilence(0.1);
     audio.manager.CueAndPlay("data/sounds/spoken/SGJspazz.wav", 1.0, 2.0);
     spazCheat = true;
     if (spaceGameJunkie&&hunterCheat&&alienPickleCheat&&spazCheat) ACHIEVE(true, "APOLUNE_19");
    }
   }
  }
 }
 void RenderMenu() {
  fast.member = null;
  fast.ResponsiveRatio(display->w,display->h);
  fast.SaveState();
  fast.color.Alpha(0.75);
  fast.activated.Int(127,64,96,255);
  int selection=fast.menu(digicaps,&options, display->w/2-128, display->h-(48+8)*options.count-32, 256,48);
  fast.RecallState();
  if ( selection >= 0 || input->KeyHit(DX_ESCAPE) || input -> KeyHit(DX_BACKTICK) || input->KeyHit(DX_BACKSPACE) || input->KeyHit(DX_ENTER) || input->KeyHit(DX_LCTRL) || input->KeyHit(DX_RCTRL) ) {
   if (time > 1.0 && (input->KeyHit(DX_ENTER) || input->KeyHit(DX_LCTRL) || input->KeyHit(DX_RCTRL))) selection = 0;
   if (time > 1.0 && (input->KeyHit(DX_ESCAPE) || input->KeyHit(DX_BACKTICK) || input->KeyHit(DX_BACKSPACE))) selection = 1;
   switch (selection) {
   case 0: // New
    ACHIEVE("APOLUNE_1");
    if (spazCheat) ACHIEVE(true,"APOLUNE_22");
    windows.Add(new Game);
    destroyMe = true;
    break;
   case 1: // Exit
    exit(0);
    break;
   default:break;
   }
  }
  if (allowCheaters) {
   fast.ljust(digicaps, FMT("%c%c%c%c", 
    spaceGameJunkie ? 'B' : ' ',
    alienPickleCheat ? 'J' : ' ',
    hunterCheat ? 'H' : ' ',
    spazCheat ? 'S' : ' '
   ).c_str(), 16, 32, 0, 3, crayons.Pick(green255), none);
  }
 }
 void Render() {
  if (!visible) return;
  time += FRAMETIME;
  Blending(none);
  art.PushViewport(display->w, display->h);
  art.Rectangle(crayons.Pick(black), 0, 0, w, h);
  glm::mat4 trans;
  trans = glm::translate(trans, glm::vec3(w / 2 - w / 4, h / 2 - w / 4, 0.0));
  trans = glm::scale(trans, glm::vec3(w / 2, w / 2, 1.0));
  trans = art.viewport->ortho * trans;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);

  /*
  {
   glm::mat4 trans;
   trans = glm::translate(trans, glm::vec3(0, 0, 0.0));
   trans = glm::scale(trans, glm::vec3(w, h, 1.0));
   trans = art.viewport->ortho * trans;
   electroncloudsShader.Set(&electron);
   electroncloudsShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   electroncloudsShader.Disable();
  }
  */

  Blending(additive);
  {
   glm::mat4 trans;
   electron.time += FRAMETIME/10.0;
   electron.formuparam2 = sin(electron.time);
   trans = glm::translate(trans, glm::vec3(0,0, 0.0));
   trans = glm::scale(trans, glm::vec3(w,h, 1.0));
   trans = art.viewport->ortho * trans;
   electroncloudsShader.Set(&electron);
   electroncloudsShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   electroncloudsShader.Disable();
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  StretchiNoAlpha(title, crayons.Pick(alabaster), transparency, display->w2 - title->width / 2, display->h2 - title->height, title->width, title->height);
  RenderMenu();
  art.PopViewport();
 }
};