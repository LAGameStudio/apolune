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

class Application {
public:
 virtual void Entrance() { } // Called after static initialization
 virtual void Startup();     // Called when traditionally we have played the startup sound.
 virtual void PreInit() {}   // Called before AppFramework.Init();
 virtual void Init() {}      // Called at time of Init();
 virtual void PostInit() {}  // Called after Init() and AppFramework.Init()
 virtual void PreLoad() {}   // Called before windows.Load() in pre-Loop phase
 virtual void Load() {}      // Called right after Windows.Load() in pre-Loop phase
 virtual void PostLoad() {}  // Called right after Load()
 virtual void PreLock() {}   // Called during AppFramework.Loop() at start of Loop cycle
 virtual void PreLoop() {}   // Called during AppFramework.Loop() at start of Loop cycle
 virtual void Loop() {}      // Called during AppFramework.Loop() before windows.Between
 virtual void PostLoop() {}  // Called during AppFramework.Loop() at end of Loop cycle
 virtual void PostLock() {}  // Called during AppFramework.Loop() at start of Loop cycle
 virtual void AfterLoop() {} // Called when the application exits the main loop on way to final exit.
 virtual void Exit() {}      // Called during called_on_exit() before AppFramework.Quit()
};

#include "ZeroTypes.h"
extern Zp<Application> application;

