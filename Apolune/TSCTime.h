/* TSCtime.h : header file for high-precision time library
   $Id: TSCtime.h,v 1.4 2003/09/25 17:07:23 kw217 Exp $
  
   Copyright (c) 2003, Keith Wansbrough
   All rights reserved.
  
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
  
       * Redistributions of source code must retain the above
         copyright notice, this list of conditions and the following
         disclaimer.
       * Redistributions in binary form must reproduce the above
         copyright notice, this list of conditions and the following
         disclaimer in the documentation and/or other materials provided
         with the distribution.
       * Neither the name of the University of Cambridge nor the names of its
         contributors may be used to endorse or promote products derived
         from this software without specific prior written permission.
  
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.
  
  
   (for information only: this is the BSD license as found on
    http://www.opensource.org/licenses/bsd-license.php) 

    --- README CONTENTS ---

TSCcal
~~~~~~

This function calibrates the IA32 timestamp counter
register accessed by the RDTSC instruction.  It uses
GetSystemTimeAsFileTime as a time reference, assuming that it is
correct if invoked as soon as possible after a Sleep() (i.e., a
scheduler tick, 1/64sec on WinXP).  The calibration data is written to
the registry, at HKEY_LOCAL_MACHINE\SOFTWARE\NetSem\TTHEE, as follows:

  tscfreq        REG_QWORD  TSC frequency in ticks per second
  tscsd          REG_QWORD  measurement error (standard deviation) in above
  tcslastupdate  REG_SZ     ISO date of last update

TSCcal can accept two arguments:  tsccalibrate(n,m)

where n is the number of measurements to make, and m is the duration
(in milliseconds) of each measurement.  An OK choice would be 10
10000, which takes 100 seconds; a good one would be 10 100000, which
takes 1000 seconds.  This latter is the default.

If the DEBUG symbol is turned on, various output appears at stdio
describing what's going on, and at the end the application waits for a
character on the console (NOT stdio!) before it exits.

Ideally, this would be a Windows Service, but I couldn't be bothered
making it one, since that's a bit complicated.

--KW 8-), 2003-09-19.


TSCtime.obj/TSCtime.h is a module that can be linked into your application, 
providing gethectonanotime_first(), gethectonanotime_last(), 
and gethectonanotime_norecal(), along with a few other useful bits and pieces. 

The functions return an accurate time (in hectonanoseconds since the Windows epoch, 
just like GetSystemTimeAsFileTime()); they differ in when recalibration is done. 
gethectonanotime_first() returns the time when it was called, but may delay before 
returning. gethectonanotime_last() returns the time when it returns, but may delay before
that. 

In both cases, the delay (of up to around 100ms) is recalibration being performed,
and this is done every 100 seconds. gethectonanotime_norecal() always returns quickly,
and never recalibrates. They rely on the registry values written by TSCcal.exe, and also
on an additional value under the same key, ugly_hack_offset, which is a string value parsed
as a signed decimal int64 offset (in hectonanoseconds) to be applied to all times returned 
by TSCtime; this defaults to zero, but may be useful in some circumstances. 

*/

#pragma once

#include <windows.h>

#if defined(NEVER)

// NB: epoch for hectonanotime is January 1, 1601 UTC,
//     so subtract 11644473600 seconds to get Unix time.
//     (369 years, 89 of which are leap years;
//      = 134774 days)

#define UNIX_EPOCH 116444736000000000ui64

class TSCTime
{
public:
 TSCTime(void);
 ~TSCTime(void);
};

// Function prototypes
void recalibrate(void);
void tsccalibrate( ULONGLONG trials, ULONGLONG msec );

#endif