#include "HUD.h"
#include "HyperspaceLoadingSequence.h"

HyperspaceLoadingThread::HyperspaceLoadingThread( HyperspaceLoadingSequence *hls )
 : LoadingSequenceThread() { sequence=hls; }

bool HyperspaceLoadingThread::Unlocked() {
 // do stuff
 sequence->destroyMe=true; 
 return true;
}

void HyperspaceLoadingSequence::OnLoad() {
 Anchor(0.0f,0.0f,0.0f,0.0f);
 A.Brights(randoms.Upto(10000));
 background.fromCrayon(A);
 B.Any();
 a_delay=2.0f;
 b_delay=0.0f;
 windows.Remove(hud);
 pthreads.New(new HyperspaceLoadingThread(this));
}

void HyperspaceLoadingSequence::OnUnload() {
 hud->visible=true;
 windows.Append(hud);
}

void HyperspaceLoadingSequence::Between() {
 if ( overlay > 0.0f ) overlay-=FRAMETIME;
 else if ( a_delay > 0.0f ) a_delay-=FRAMETIME;
 else b_delay+=FRAMETIME;
 dy+=0.1f;
 dx+=0.1f;
}

void HyperspaceLoadingSequence::Render() {
 Text("HYPERSPACE TRANSIT PLEASE WAIT",24,20,12,14,false);
}

void HyperspaceLoadingSequenceUnthreaded::Load() {
 // do stuff
 windows.Remove(hud);
// players.Save(); // thread-safe
 hud->visible=true;
 windows.Append(hud);
}