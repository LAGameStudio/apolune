#include "LoadingSequence.h"

#include "AppFramework.h"

void LoadingSequenceThread::Main() {
  while (!done) {
#if !defined(USE_GLFW)
   wglMakeCurrent(display.hdc, app.upload_rc);
#endif
   done |= Unlocked();
#if !defined(USE_GLFW)
   wglMakeCurrent(display.hdc, app.game_rc);
#endif
   pthreads.Lock();
   done |= Locked();
   pthreads.Unlock();
   iteration++;
  }
  done = true;
  pthreads.Lock();
  OnComplete();
  pthreads.Unlock();
 }