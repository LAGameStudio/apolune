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
#include "LoadingSequence.h"
#include "TextureLibrary.h"
#include "Tweens.h"

class HyperspaceLoadingSequence;
class HyperspaceLoadingThread : public LoadingSequenceThread {
public:
 HyperspaceLoadingSequence *sequence;
 HyperspaceLoadingThread( HyperspaceLoadingSequence *hls );
 bool Unlocked();
};

class HyperspaceLoadingSequence : public ThreadedLoadingSequence {
public:
 float dx,dy,dw,dh;
 FBOColorDepth fcd;
 float overlay,a_delay,b_delay;
 Crayon A,B;
 Tween *tween;
 HyperspaceLoadingSequence( ) : ThreadedLoadingSequence() {
  dx=float_range(0.0f,display->wf/4.0f);
  dy=float_range(0.0f,display->hf/4.0f);
  dw=float_range(display->wf/4.0f,display->wf/2.0f);
  dh=float_range(display->hf/4.0f,display->hf/2.0f);
 }
 void OnLoad();
 void OnUnload();
 void Between();
 void Render();
};

#include "Art.h"

class HyperspaceLoadingSequenceUnthreaded : public LoadingSequence {
public:
 GLImage *splash;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
  splash=library.find("data/images/splash.png");
 }
 void Render() {
  QuadiNoAlpha(splash,crayons.Pick(alabaster),none,0,0,display->w,display->h);
 }
 void Load();
};