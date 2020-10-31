#include "WebAPI.h"

#if defined(USE_CURL)
int failures=0;
char web_buffer[65536];
bool WaitingForState=true;
States CLIENT=initial;
States WAS=precursor;

void AuthRequest::Result()
 {
    OUTPUT( "Unable to authenticate.\n" );
    failures++;
    CLIENT=bad_auth;
    WaitingForState=false;
 }

// Called when you log in, etc
void ChangeStates() {
 OUTPUT( "ChangeStates() called with %d as state!\n", (int) CLIENT );
 switch ( CLIENT ) {
  case initial:
   break;
  default: OUTPUT( "ChangeStates() reports invalid state!\n" ); break;
 }
 WAS=CLIENT;
}
#endif