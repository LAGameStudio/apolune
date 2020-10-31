#include "MicroFont.h"

// this doesn't seem to work, maybe switch to Text() or fast.text() call?

void Small( const char *s, double x, double y ) {
 Small(s,(int)x,(int)y);
}

void Small( const char *s, double x, double y, Crayon tint ) {
 Small(s,(int)x,(int)y,tint);
}

void Small( const char *s, double x, double y, Crayon tint, Blends blend ) {
 Small(s,(int)x,(int)y,tint,blend);
}

void SmallShadow( const char *s, double x, double y, Crayon tint, Blends blend ) {
 SmallShadow(s,(int)x,(int)y,tint,blend);
}


void Small( const char *s, int x, int y ) {
 WriteTexFont(microFont,crayons.Pick(alabaster),additive,x,y,7,16,4,s);
}
void Small( const char *s, int x, int y, Crayon tint ) {
 WriteTexFont(microFont,tint,additive,x,y,7,16,4,s);
}
void Small( const char *s, int x, int y, Crayon tint, Blends blend ) {
 WriteTexFont(microFont,tint,additive,x,y,7,16,4,s);
}
void SmallShadow( const char *s, int x, int y, Crayon tint, Blends blend ) {
 Crayon half(tint); half.Multiply(0.5f);
 WriteTexFont(microFont,half,blend,x,y+1,7,16,4,s);
 WriteTexFont(microFont,tint,blend,x+1,y,7,16,5,s);
}