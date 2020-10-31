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

 void LoadFilms();

class LoadingSequenceThreadTester : public LoadingSequenceThread {
public:
 int count;
 LoadingSequenceThreadTester() : LoadingSequenceThread() { count=0; }
 bool Unlocked() {
  // load a bunch of textures
  library.Load("data/images/materials/asteroid.png");
  count++;
  library.Load("data/images/materials/asteroid_n.png");
  count++;
  library.Load("data/images/materials/asteroid_s.png");
  count++;
  LoadFilms();
  return true;
 }
};

#include "Art.h"
#include "UglyFont.h"

class ThreadedLoadingTest : public ThreadedLoadingSequence {
public:
 Zp<LoadingSequenceThreadTester> thread;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  pthreads.New(thread=new LoadingSequenceThreadTester);
 }
 void Render() {
  Crayon drawing;
  if ( thread->done ) drawing.Any();
  else drawing.Pick(red255);
  Area(drawing,20,20,128,128);
  char buf[24];
  pthreads.Lock();
  FORMAT(buf,24,"Loaded: %d", (int) thread->count);
  pthreads.Unlock();
  Text(buf,256,256,20,20,true);
 }
};

