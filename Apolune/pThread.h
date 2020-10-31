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

#if defined(USE_PHTREADS)
#include <pthread.h>
#endif

#include <cmath>
#include <ctime>
#include <limits>
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>
#include <Windows.h>
#include "ListItem.h"
#include "LinkedList.h"

#define USE_WINTHREADS 1

const char *TranslateErrorCode( DWORD ERR );

#if defined(USE_WINTHREADS)
extern HANDLE appMutex;
#else
extern pthread_mutex_t appMutex;
#endif

#if defined(USE_WINTHREADS)
DWORD WINAPI ThreadLoop( LPVOID );
#else
static void *ThreadLoop( void *p );
#endif

class pThread : public ListItem {
public:
 Zbool created,done,deleteMe;
#if defined(USE_WINTHREADS)
 HANDLE thread;
 DWORD ID;
 Zbool started;
#else
 bool locking;
 bool detached;
 pthread_t thread;
 pthread_attr_t attr;
 pthread_mutex_t mutex;
#endif
 Zint iret;
 Zbool paused;
 pThread(void);
 pThread(bool lock);
 ~pThread(void);
 void Pause();
 void Unpause();
 void Create(void);
 void Join(void);
 virtual void Main() {}
 void Lock(void);
 void Unlock(void);
 virtual void OnDestroy() {} // often is not called .. why?
 virtual void OnComplete() {} // only called when called in Main() in derived
 virtual void OnKill() {}
};

class pThreads : public LinkedList {
public:
 int locked;
 pThreads(void);
 ~pThreads(void);
 void Loop(void); // called in the main application loop
 void Join(void);
 void Clean(void);
 void Lock(void);
 void Unlock(void);
 void New( pThread *p );
 void Kill( pThread *p );
};

extern pThreads pthreads;
