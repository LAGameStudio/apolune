#pragma once

#include "Proce55or.h"
#include "Delay.h"
#include "Art2d.h"
#include "TexFont.h"
#include "Atlas.h"
#include "Audio.h"

class fx_Teletype : public Proce55or {
public:
 Zstring showingText;
 Zint showingCharacters;
 Delay textDelay,afterDelay;
 Zint lineWrap,fontW,fontH,lineSpace,kern;
 Zp<TexFont> font;
 Crayon ink;
 Zbool off,clickToAccelerate,centered,scrollUp,stayOnDone,finished;
 Cartesian bounds;
 fx_Teletype() : Proce55or() {
  delay=10.0;
  textDelay.Reset();
  textDelay.Set(0.035);
  afterDelay.Reset();
  afterDelay.Set(2.0);
  lineWrap=70;
  ink.Pick(alabaster);
  Font();
 }
 void Font( TexFont *f=termFont, int fontW=32, int fontH=40, int lineSpace=0, int kern=-6 ) {
  this->font=f;
  this->fontW=fontW;
  this->fontH=fontH;
  this->lineSpace=lineSpace;
  this->kern=kern;
 }
 void WriteText( const char * s, bool centered=false ) {
  showingText=s;
  showingText=TexFontColorCodesWordWrap(showingText.c_str(),lineWrap);
  textDelay.Reset();
  afterDelay.Reset();
  showingCharacters=0;
  off=false;
  this->centered=centered;
  TexFontColorCodesBoxBounds(&bounds,font,lineSpace,kern,fontW,fontH,s);
  finished=false;
 }
 void Render() {
  if ( off ) return;
  if ( delay > 0.0 ) { delay-=FRAMETIME; return; }
  if ( !stayOnDone && afterDelay && showingText.length <= showingCharacters ) {
   afterDelay.Between();
  }
  if ( afterDelay ) {
   textDelay.Between();
   if ( !textDelay || ( clickToAccelerate && input.left ) ) {
    textDelay.Reset();
    if ( showingText.length > showingCharacters ) {
     showingCharacters++;
     char c=showingText.At((unsigned int)showingCharacters);
     char cm1=showingText.At((unsigned int)showingCharacters-1);
     if ( c == '^' ) showingCharacters++;
     else if ( cm1 != ' ' && (showingCharacters == 0 || cm1 != '^') )
     audio.manager.CueAndPlay("data/sounds/Legacy/sfx_teletype.wav",1.0,1.0);
    } else finished=true;
   }
   Crayon tint;
   tint=ink;
   tint.ScaleByAlphaf(afterDelay.Ratio());
   string active=showingText.value.substr(0,(size_t)showingCharacters.value);
   WriteTexFontColorCodes(font,tint,1.0,font->blend,
    x+(centered?(-bounds.x):0),y+(scrollUp?(-bounds.h+fontH+lineSpace):0),
    fontW,fontH,lineSpace,active.c_str(),kern
   );
  } else off=true;
 }
};

