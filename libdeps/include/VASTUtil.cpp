/*
 * VAST, a scalable peer-to-peer network for virtual environments
 * Copyright (C) 2009   Shun-Yun Hu     (syhu@ieee.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
    Implementation of VASTUtil

*/

#include "VASTUtil.h"

#include "ace/ACE.h"        // ACE_OS::gettimeofday () for TimeMonitor
#include "ace/OS.h"

namespace Vast
{  

//TimeMonitor gTimeMonitor;
TimeMonitor TimeMonitor::_instance;

TimeMonitor::TimeMonitor ()
{
    ACE::init ();
    _budget = 0;

    ACE_Time_Value time = ACE_OS::gettimeofday ();
    _start = (long long)time.sec () * 1000000 + time.usec ();
}


// set how much time is left (in millisecond)
void 
TimeMonitor::setBudget (int time_budget)
{
    if (time_budget > 1000)
    {
        printf ("TimeMonitor::setBudget () budget exceeds 1000 ms, may want to check if this is correct\n");
    }

    _budget = time_budget * 1000;
    ACE_Time_Value time = ACE_OS::gettimeofday();
    _start = (long long)time.sec () * 1000000 + time.usec ();
    _first = true;
}

// still time available?
int
TimeMonitor::available ()
{
    // unlimited budget returns (-1)
    if (_budget == 0)
        return (-1);
    // budget already used up, return immediately
    else if (_budget < 0)
        return 0;

    // fix movement at tick_persec times per second
    ACE_Time_Value time = ACE_OS::gettimeofday();
    long long elapsed = (long long)time.sec () * 1000000 + time.usec () - _start;
    
    //printf ("available (): %ld budget: %ld\n", (long)now, (long)_budget);
    int time_left = (int)(_budget - elapsed)/1000;

    if (time_left < 0)
        time_left = 0;

    // we make sure that a positive response is returned at least once per cycle
    if (_first == true)
    {
        _first = false;
      
        if (time_left == 0)
            time_left = 1;
    }

    return time_left;
}

// return a global instance of TimeMonitor
//TimeMonitor *
//TimeMonitor::instance ()
TimeMonitor&
TimeMonitor::getInstance ()
{
    //return &gTimeMonitor;
    return TimeMonitor::_instance;
}


} // namespace Vast

