#pragma once
#include "LinkedList.h"
#include "Crayon.h"
#include "ComplexWave.h"
#include "Art.h"

SINGLE(ColorController,{});
 Zstring name;
 BlendProgram blendProgram;
 Zp<Crayon> color;
 Crayon tint;
 Zp<Blends> blend;
 Zp<float> alpha;
 Zint loopsRemaining,initialLoops;
 Zdouble an,expired,duration;
 Zbool useProgram,modulateAlpha,separateChannels,paused;
 ComplexWaveProgram waves,r,g,b,a;
 void Rewind() {
  loopsRemaining=initialLoops;
  expired=0.0;
  blendProgram.Rewind();
  waves.Rewind();
  r.Rewind();
  g.Rewind();
  b.Rewind();
  a.Rewind();
 }
 void Between() {
  if ( loopsRemaining == 0 ) return;
  expired+=FRAMETIMEd;
  if ( expired >= duration ) {
   expired=0.0;
   if ( loopsRemaining > 0 ) loopsRemaining-=1;
   if ( loopsRemaining == 0 ) return;
  }
  waves.Between();
  if ( separateChannels ) {
   r.Between();
   g.Between();
   b.Between();
   a.Between();
  }
  if ( blend && useProgram ) blendProgram.Between();
  an=expired/duration;
 }
 void Apply() {
  if ( alpha ) {
   (*alpha)=modulateAlpha ? (tint.af * (float)waves.Compute(an)) : tint.af;
  } 
  if ( color ) {
   if ( separateChannels ) {
    (*color).Float(
     tint.rf*(float)r.Compute(an),
     tint.gf*(float)g.Compute(an),
     tint.bf*(float)b.Compute(an),
     (!alpha ? (*color).af : (modulateAlpha ? (tint.af *(float)a.Compute(an)) : tint.af))
    );
   } else {
    (*color).Float(
     tint.rf*(float)waves.Compute(an),
     tint.gf*(float)waves.Compute(an),
     tint.bf*(float)waves.Compute(an),
     (!alpha ? (*color).af : (modulateAlpha ? (tint.af *(float)waves.Compute(an)) : tint.af))
    );
   }
  }
  if ( blend && useProgram && blendProgram.current ) {
   (*blend) = blendProgram.current->blend;
  }
 }
PLURAL(ColorController,ColorControllers,{});
ENDSET(ColorController);