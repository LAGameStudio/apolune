// $Id: TSCtime.c,v 1.7 2003/09/25 17:07:23 kw217 Exp $
//
// Copyright (c) 2003, Keith Wansbrough
// All rights reserved.
// BSD license as found on  http://www.opensource.org/licenses/bsd-license.php

#include "TSCTime.h"
#include "string_functions.h"

#if defined(NEVER)

// TSC Cal
#include <stdio.h>
#include <math.h>
#include <tchar.h>

//#define TSC_DEBUG

#ifdef TSC_DEBUG
#include <conio.h>
#endif

// be sure to build in Release configuration, and make sure inlining is on.
#if defined(WIN32)
__forceinline ULONGLONG gettsc(void) {
	ULONG hi,lo;
	LARGE_INTEGER r;
	__asm {
		rdtsc
		mov hi,edx
		mov lo,eax
	}
	r.HighPart = hi;
	r.LowPart = lo;
	return r.QuadPart;
}
#endif

ULONGLONG basetsc = 0;                 // base timestamp in processor clocks
ULONGLONG basest  = 0;                 // base time in hecto-nanoseconds (100ns units, 10^-7 sec)
ULONGLONG tscfreq = 500000000uI64;     // timestamp clock frequency in ticks/second
ULONGLONG tscsd   = 500000000uI64;     // measured error (standard deviation) of above
ULONGLONG ugly_hack_offset = 0;        // correction to add to returned time
  // this accounts for the fact that we never get control exactly at the tick; it's always
  // a bit later than that.  The value is a guesstimate.
  // This is stored as a string value (for easy editing) in the registry.

  // ** Turns out that isn't necessary for our tests; but we leave the hook here just in case.


__forceinline ULONGLONG hectonanotime_of_tsc(ULONGLONG curtsc) {
    // compute seconds and hectonanoseconds separately to avoid overflow problems
	// deltaticks may be negative if we're measuring first and calibrating later
	BOOL neg = curtsc < basetsc;
	ULONGLONG deltaticks = neg ? basetsc - curtsc : curtsc - basetsc;
	ULONGLONG deltasecs = deltaticks / tscfreq;
	ULONGLONG deltafrac = (10000000 * (deltaticks % tscfreq)) / tscfreq;  /* in hectonanoseconds */
	ULONGLONG delta = deltasecs * 10000000 + deltafrac;
	return (neg ? basest - delta : basest + delta);
}

// store time of last recalibration
ULONGLONG lastrecal = 0;  // hectonanosecond time since epoch of last recalibration; 0 == never

// recalibrate -- may take up to 100msec or so.  Updates the four static globals above.
#define nreps 5  // number of ticks to examine; we take the average.
void recalibrate(void) {
	int i;

	ULONGLONG basetscs[nreps];  // basetsc for each round
	ULONGLONG basests[nreps];   // basest for each round

        char buf[100] = "0";

	// read calibration
	HKEY key;
	DWORD cbData;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\NetSem\\TTHEE",0,KEY_READ,&key);
	cbData = sizeof(tscfreq);
	RegQueryValueEx(key,"tscfreq",NULL,NULL,(BYTE *)&tscfreq,&cbData);
	cbData = sizeof(tscsd);
	RegQueryValueEx(key,"tscsd",NULL,NULL,(BYTE *)&tscsd,&cbData);
	// double error = double(tscsd) / double(tscfreq);
        cbData = sizeof(buf)-1;
	RegQueryValueEx(key,"ugly_hack_offset",NULL,NULL,(BYTE *)buf,&cbData);
        buf[cbData] = '\0';
        ugly_hack_offset = _atoi64(buf);

	// get base times
	for (i=0; i<nreps; i++) {
		FILETIME baseft;
		Sleep(5);  // to be sure we're on a tick boundary
		basetscs[i] = gettsc();  // get this first because it changes very fast
		GetSystemTimeAsFileTime(&baseft);  // no rush; we have 1/64 sec to read this before it changes
		basests[i] = ((ULONGLONG)baseft.dwHighDateTime)<<32 | ((ULONGLONG)baseft.dwLowDateTime);
	}

	basetsc = basetscs[nreps-1];
	/* Now: we want to know the correct basest for the latest basetsc.  Each measurement can be
	   extrapolated to give us a basest for the latest basetsc.  But if there was a delay between the
	   tick and our process being scheduled in a particular round, this will show up as the basetsc
	   being high, a.k.a., the basest being low.  Thus, we take the highest computed basest. */
	basest = basests[nreps-1];
	for (i=0; i<nreps; i++) {
		ULONGLONG basestx = basests[i] + (10000000 * (basetscs[nreps-1] - basetscs[i])) / tscfreq;
			// don't worry about overflow; at 500MHz (glia) it won't overflow until just over an hour
		if (basestx > basest) {
			basest = basestx;
		}
	}

        basest -= ugly_hack_offset;   // apply the hack offset.

	lastrecal = basest;  // we just recalibrated.
}

// get current time, without recalibration
__forceinline ULONGLONG gethectonanotime_norecal(void) {
	ULONGLONG curtsc;

	// get the timestamp right up front
	curtsc = gettsc();
	return hectonanotime_of_tsc(curtsc);
}

// interval between automatic recalibrations
ULONGLONG recalinterval = 1000000000uI64;  // recal every 100sec.

// get time of call, possibly recalibrating before returning
__forceinline ULONGLONG gethectonanotime_first(void) {
	ULONGLONG curtsc = gettsc();
	ULONGLONG now = hectonanotime_of_tsc(curtsc);
	if ((lastrecal == 0) || (now - lastrecal) > recalinterval) {
		recalibrate();
		now = hectonanotime_of_tsc(curtsc);
	}
	return now;
}

// get time of return, possibly recalibrating before returning
__forceinline ULONGLONG gethectonanotime_last(void) {
	(void)gethectonanotime_first();
	return gethectonanotime_norecal();
}

void tsccalibrate( int trials, DWORD msec ) {
	ULONGLONG basetsc,curtsc;  // in processor clocks
	FILETIME baseft,curft;     // in hecto-nanoseconds (100ns units, 10^-7 sec)
	ULONGLONG basest, curst;
	int numtrials = 10;
	DWORD trialmsec = 10000; // 100000?

	numtrials = trials;
	trialmsec = msec;
#ifdef TSC_DEBUG
	{
		printf("WARNING: Usage: %s numtrials trialmsec\n",argv[0]);
	}
#endif

	double *rate = new double[numtrials];
	ULONGLONG *dur = new ULONGLONG[numtrials];

#ifdef TSC_DEBUG
	LARGE_INTEGER ld;
	QueryPerformanceFrequency(&ld);
	OUTPUT("For comparison, QueryPerformanceFrequency says %I64u\n",ld.QuadPart);
#endif

	for (int i=0; i<numtrials; i++) {  // let's do it a few times for good measure

	// get base times
	Sleep(5);  // to be sure we're on a tick boundary
    basetsc = gettsc();  // get this first because it changes very fast
    GetSystemTimeAsFileTime(&baseft);  // no rush; we have 1/64 sec to read this before it changes

	// get later times
	Sleep(trialmsec);
    curtsc = gettsc();
    GetSystemTimeAsFileTime(&curft);

    // coerce filetimes
	basest = ULONGLONG(baseft.dwHighDateTime)<<32 | ULONGLONG(baseft.dwLowDateTime);
    curst = ULONGLONG(curft.dwHighDateTime)<<32 | ULONGLONG(curft.dwLowDateTime);

    // do calibration
	// NB: dwLowDateTime wraps every 429.5 seconds, so don't just use that!
	ULONGLONG deltsc = curtsc - basetsc;
	ULONGLONG delst = curst - basest;
	double tscrate = double(deltsc) / (double(delst) / 1e7);

#ifdef TSC_DEBUG
	OUTPUT("Tick delta %I64u, hectonanosecond delta %I64u.\nSo ticks/sec = %0.4f\n",deltsc,delst,tscrate);
#endif

	rate[i] = tscrate;
	dur[i] = delst;
	}

	ULONGLONG sumdur = 0;
	for (int i=0; i<numtrials; i++) {
		sumdur += dur[i];
	}
	double avdur = double(sumdur) / numtrials / 1e7;

	double minrate, maxrate;
	minrate = maxrate = rate[0];
	for (int i=1; i<numtrials; i++) {
		if (rate[i]<minrate) { minrate = rate[i]; }
		if (rate[i]>maxrate) { maxrate = rate[i]; }
	}
	double sum = 0;
	for (int i=0; i<numtrials; i++) { sum += rate[i]; }
	double av = sum / numtrials;
	double sumsqdiff = 0;
	for (int i=0; i<numtrials; i++) { double diff = rate[i] - av; sumsqdiff += diff*diff; }
	double sd = sqrt(sumsqdiff/numtrials);

#ifdef TSC_DEBUG
	OUTPUT("Mean rate  %14.4f ticks/sec\n      min -%14.4f\n      max +%14.4f\n      s.d. %14.4f\n",av,av-minrate,maxrate-av,sd);

	OUTPUT("Scheduling noise (TSC measurement precision):\n  min -%14.4f ticks\n  max +%14.4f\n  s.d. %14.4f\n",
			avdur * (av-minrate), avdur * (maxrate-av), avdur * sd);
#endif

	ULONGLONG tcsfreq = ULONGLONG(floor(av+0.5));
	ULONGLONG tcssd = ULONGLONG(floor(sd+0.5));
	SYSTEMTIME syst;
	GetSystemTime(&syst);
	char buf[100];
	FORMAT(buf,100,"%04d-%02d-%02dT%02d:%02d:%02dZ",
  (int) syst.wYear, (int) syst.wMonth, (int) syst.wDay, (int) syst.wHour, (int) syst.wMinute, (int) syst.wSecond);

	HKEY key;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\NetSem\\TTHEE",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&key,NULL);
 RegSetValueEx(key,"tscfreq",0,REG_QWORD,(const BYTE *)&tcsfreq,8);
	RegSetValueEx(key,"tscsd",0,REG_QWORD,(const BYTE *)&tcssd,8);
	RegSetValueEx(key,"tcslastupdate",0,REG_SZ,(const BYTE *)buf,DWORD(strlen(buf))+1);
}

#endif