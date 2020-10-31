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
#include "GLWindow.h"

/*
 * A "loading sequence" is a blocking operation that causes
 * the screen to freeze while files are loaded or information
 * is calculated, but provides visual feedback.
 *
 * You can set a delay before starting a loading sequence
 * to allow sound to be faded out, etc.
 *
 * Each "frame" of a loading sequence can show the user an
 * update to the screen which occurs after each load operation.
 *
 * The term "load" should be interpreted pretty loosely since
 * it can include generation of whatever data from whatever
 * source.
 */

class LoadingSequence : public GLWindow {
public:
 int state;
 float delay;
 LoadingSequence() : GLWindow() {
  state=0;
  delay=1.0f;
  cursor=false;
  WindowName("LoadingSequence");
 }
 virtual void Load() {}
 virtual void Loading(int pass) {
  switch ( pass ) {
  case 0: break; // default screen before delay expires
  case 1: Load(); break;
  default: this->destroyMe=true; break;
  }
 }
 void Between() {
  if ( delay > 0.0f ) {
   delay-=FRAMETIME;
   return;
  }
  state++;
  Loading(state);
 }
 virtual void Render() {
 }
};

#include "pThread.h"

/*
 * A deferred loading sequence, with the caveat that textures
 * cannot be loaded without locking.  (Does not use shared GL contexts)
 * Textures and other OGL operations can be performed in the "Locked" virtual,
 * or by manually locking / unlocking.
 */

class LoadingSequenceThread : public pThread {
public:
 Zuint iteration;
 LoadingSequenceThread() : pThread() {}
 virtual bool Locked() { return true; }
 virtual bool Unlocked() { return true; }
 virtual void OnComplete() {}
 void Main();
};

class ThreadedLoadingSequence : public GLWindow {
public:
 void AddThread( LoadingSequenceThread *thread ) {
  pthreads.New(thread);
 }
 virtual void Advance() {}
};