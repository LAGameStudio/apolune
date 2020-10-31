#pragma once

/* waveout.h
 * --------- 
 * Author: E. Pronk
 * Descr.: Handles all sound related tasks
 */

#ifndef LMTZ_WAVEOUT_H
#define LMTZ_WAVEOUT_H

#pragma comment(lib, "winmm")

#include <windows.h>
#include <MMReg.h>
#include <ks.h>
#include <ksmedia.h>

#include "exception.h"

GENERATE_EXCEPTION(WaveOutException, Exception);

// Abstract provider class, to make the waveout unaware of the implementation of Video
// However, the NextAudioBuffer is not general enough to justify this at the moment.
class AudioProvider
{
    public:
    virtual int NextAudioBuffer(void** buffer, int* length, int elapsed) = 0;
};

class WaveOut
{
    public:
    WaveOut(AudioProvider* provider, int sampleRate, int nrChannels, int bitsPerSample);
    ~WaveOut();

    // Start audio output
    void Start();

    private:
    AudioProvider* _provider;
    HWAVEOUT _waveout;
    bool _started;
    int _buffered;
    static void CALLBACK Callback(HWAVEOUT waveout, UINT msg, DWORD_PTR userData, DWORD_PTR p1, DWORD_PTR p2);
    static DWORD WINAPI WaveOutCacheProc(LPVOID param);
};

#endif