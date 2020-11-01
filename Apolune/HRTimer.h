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

#include <Windows.h>
#include <MMSystem.h>

#include "ListItem.h"
#include "LinkedList.h"

class HRTimer : public ListItem {
public:
 int expired,expires;
 bool looping;
 bool destroyMe;
 ListItem *delayExpired;

 HRTimer(void);
 HRTimer(int delay_ms);
 ~HRTimer(void);

 void Start(void);
 void Stop(void);
 void Update();
 void Step( int diff ) { 
  expired += diff;
  Update();
 }
 virtual bool DelayExpired(void) { return false; }
};

class Timers : public LinkedList {
public:
 HANDLE handle[1];
 MMRESULT hiResTimer;
 DWORD oldtime;
 int resolution;
 Timers(void);
 ~Timers(void);
 HRTimer *Add( ListItem *L, int ms );
 HRTimer *Add( ListItem *L, float s );
 void AddTimer( HRTimer *t );
 void Start(void);
 void Stop(void);
 void Update(void);
 void Wait()
 {
 // MsgWaitForMultipleObjects(1, (const HANDLE *) (&handle), FALSE, (UINT)(1000 / 18.2) /*INFINITE*/, QS_TIMER);
 }
 void SetTimerResolution(UINT hertz);
 void Clean();
};

extern Timers timers;