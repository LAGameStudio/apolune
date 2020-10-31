#define WIN32_LEAN_AND_MEAN

#include <cmath>
#include <ctime>
#include <limits>
#include <iomanip>
#include <utility>
#include "HRTimer.h"

Timers timers;
extern HWND game_window;

HRTimer::HRTimer(void)
{
 expires=0;
 expired=0;
 delayExpired=NULL;
 looping=false;
 destroyMe=false;
}

HRTimer::HRTimer( int delay_ms )
{
 expires=delay_ms;
 expired=0;
 delayExpired=NULL;
 looping=false;
 destroyMe=false;
}

HRTimer::~HRTimer(void)
{
}

void HRTimer::Start(void) {
 looping=true;
}

void HRTimer::Stop(void) 
{
 looping=false;
}

void HRTimer::Update(void) {
 if ( !looping ) return;
 if(expired>expires){ 
  expired=0;
  looping=this->DelayExpired();
  if ( delayExpired ) delayExpired->Virtual();
 }
}

Timers::Timers(void)
{
}

void Timers::Start(void)
{
 oldtime=timeGetTime();
 SetTimerResolution(FPS);//resolution=1000);
 handle[0] = CreateSemaphore(null, 0, 1, null);
}

Timers::~Timers(void)
{
}

void Timers::Stop(void)
{
 HRTimer *t,*n;
 ReleaseSemaphore(*handle, 1, null);
 for ( t=(HRTimer *)first; t; t=n ) { n=(HRTimer *)t->next; delete t; }
 first=last=null;
 count=0;
 timeEndPeriod(resolution);
 if(hiResTimer) timeKillEvent(hiResTimer);
}

HRTimer *Timers::Add( ListItem *L, int ms )
{
 HRTimer *t=new HRTimer;
 t->expires=ms;
 t->delayExpired=L;
 return t;
}

HRTimer *Timers::Add( ListItem *L, float s )
{
 HRTimer *t=new HRTimer;
 t->expired=(int) ( 1000.0f * s );
 t->delayExpired=L;
 return t;
}

void Timers::AddTimer( HRTimer *t )
{
 Prepend(t);
}

void Timers::Update(void) 
{
 HRTimer *t;
 DWORD now=timeGetTime();
 int diff=now-oldtime;
 for ( t=(HRTimer *)first; t; t=(HRTimer *) (t->next) ) t->Step(diff);
}

void Tick();

#include "AppFramework.h"

void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
 app.Tick();
 //PostMessage(game_window,WM_APP+1,0,0);
}

void Timers::SetTimerResolution(UINT hertz)
{
 if(hiResTimer) timeKillEvent(hiResTimer);
 if(hertz)
    hiResTimer = timeSetEvent(1000 / hertz, 0, (LPTIMECALLBACK) TimerProc, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
}  

void Timers::Clean(void) {
 //this->Lock();
 HRTimer *p,*n;
 for ( p=(HRTimer *) first; p; p=n ) {
  n=(HRTimer *) (p->next);
  if ( p->destroyMe ) {
   Remove(p);
   delete p;
  }
 }
 //this->Unlock();
}