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
#include <cmath>
#include <ctime>
#include <limits>
#include <iomanip>
#include <iostream>
#include <utility>
#include <windows.h> 
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class CTimer
{
public:
	float dt, td, td2;
	__int64       frequency;									// Timer Frequency
	float         resolution;									// Timer Resolution
	unsigned long mm_timer_start;								// Multimedia Timer Start Value
	unsigned long mm_timer_elapsed;								// Multimedia Timer Elapsed Time
	bool		  performance_timer;							// Using The Performance Timer?
	__int64       performance_timer_start;						// Performance Timer Start Value
	__int64       performance_timer_elapsed;					// Performance Timer Elapsed Time

	CTimer();
	void Update();
	float GetTime() const;
	void Initialize();
};

extern CTimer timer;