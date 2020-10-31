#pragma once
#pragma once
#include "Globals.h"
#include "Crayon.h"
#include "GLWindow.h"
#include "TexFont.h"
#include "UglyFont.h"
#include "Audio.h"
#include "Proce55or.h"
#include "FastGUI.h"

extern Zp<TexFont> alarmClockNumbers;
extern Zp<TexFont> silverFont;

extern Audio audio;

class fx_MoneyCounter : public Proce55or {
public:
 Zp<GLImage> dollarsign;
 Zdouble testValue;
 Zp<double> value;
 Zdouble speed;
 Zdouble displaying_value,scaled_value,target_value,delta, last_polled_value;
 Zint dollars,cents;
 char buf[128], mag[16];
 Crayon color,overlayTint,bgTint,invertOverlayTint;
 Zdouble cantBuyBleep,boughtBleep;
 fx_MoneyCounter() : Proce55or() {
  testValue=5.0f;
  TrackValue(&testValue.value);
  SetExtents(display.w2,8,256+24,32);
  dollarsign=library.find("data/images/fonts/metal/silver_dollarsign.png");
  color.Float(0.3f,1.0f,0.3f,1.0f);
  overlayTint.Int(64,64,0,255);
  invertOverlayTint.Pick(red255);
  speed=3.0; // in seconds, lower=faster
  bgTint.Float(0.0,0.5,0.0,0.5);
 }
 bool Stopped() {
  return last_polled_value == (*value) && displaying_value == last_polled_value;
 }
 void CantBuy() {
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/UI/alert.wav",1.0f,1.0f);
  cantBuyBleep=1.0;
 }
 void TrackValue( double *tracking ) {
  value=tracking;
  displaying_value=last_polled_value=*tracking;
  delta=1.0;
 }
 void MovedTo( double nx, double ny ) {
  SetExtents((int)nx,(int)ny,w,h);
 }
 void PollAndUpdateTarget() {
  target_value=*value;
  if ( target_value != displaying_value ) {
   boughtBleep=1.0;
   if ( target_value > displaying_value ) {
    double diff=target_value-displaying_value;
    if ( abs(diff) < 1.0 ) {
     delta=1.0;
     displaying_value=target_value;
     audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/chaching.wav",0.75f+(float)(0.15-uniform()*0.3),1.0f,false,true);
    }
    else {
     if ( diff > (double)FPS*speed*delta ) delta=diff/((double)FPS*speed);
     if ( delta < FRAMETIME ) delta=FRAMETIME;
     displaying_value+=delta;
     if ( displaying_value > target_value ) {
      delta=1.0;
      displaying_value=target_value;
      audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/chaching.wav",0.75f+(float)(0.15-uniform()*0.3),1.0f,false,true);
     }
    }
   } else {
    double diff=(displaying_value-target_value);
    if ( abs(diff) < 1.0 ) {
     delta=1.0;
     displaying_value=target_value;
     audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/chaching.wav",0.75f+(float)(0.15-uniform()*0.3),1.0f,false,true);
    }
    else {
     if ( diff > (double)FPS*speed*delta ) delta=diff/((double)FPS*speed);
     if ( delta < FRAMETIME ) delta=FRAMETIME;
     displaying_value-=delta;
     if ( displaying_value < target_value ) {
      delta=1.0;
      displaying_value=target_value;
      audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/chaching.wav",0.75f+(float)(0.15-uniform()*0.3),1.0f,false,true);
     }
    }
   }
  }
  if ( last_polled_value != target_value || (displaying_value!=target_value && uniform()>0.001) )
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/coindrop.wav",(float)(1.0+(0.5-uniform())),1.0f,false,true);
  last_polled_value=*value;
 }
 void Between() {
  PollAndUpdateTarget();
  scaled_value=displaying_value;
  if ( scaled_value > 1000000000000000000000000000000000.0 ) {
   FORMAT(mag,16,"Decillion");
   scaled_value=displaying_value/1000000000000000000000000000000000.0;
  } else if ( scaled_value > 1000000000000000000000000000000.0 ) {
   FORMAT(mag,16,"Nonillion");
   scaled_value=displaying_value/1000000000000000000000000000000.0;
  } else if ( scaled_value > 1000000000000000000000000000.0 ) {
   FORMAT(mag,16,"Octillion");
   scaled_value=displaying_value/1000000000000000000000000000.0;
  } else if ( scaled_value > 1000000000000000000000000.0 ) {
   FORMAT(mag,16,"Septillion");
   scaled_value=displaying_value/1000000000000000000000000.0;
  } else if ( scaled_value > 1000000000000000000000.0 ) {
   FORMAT(mag,16,"Sextillion");
   scaled_value=displaying_value/1000000000000000000000.0;
  } else if ( scaled_value > 1000000000000000000.0 ) {
   FORMAT(mag,16,"Quintillion");
   scaled_value=displaying_value/1000000000000000000.0;
  } else if ( scaled_value > 1000000000000000.0 ) {
   FORMAT(mag,16,"Quadrillion");
   scaled_value=displaying_value/1000000000000000.0;
  } else if ( scaled_value > 1000000000000.0 ) {
   FORMAT(mag,16,"Trillion");
   scaled_value=displaying_value/1000000000000.0;
  } else if ( scaled_value > 1000000000.0 ) {
   FORMAT(mag,16,"Billion");
   scaled_value=displaying_value/1000000000.0;
  } else if ( scaled_value > 1000000.0 ) {
   FORMAT(mag,16,"Million");
   scaled_value=displaying_value/1000000.0;
  } else FORMAT(mag,16,"");
  dollars=(int)(scaled_value);
  cents=(int)( (scaled_value - (double)dollars) * 100.0);
#if defined(DEBUG)
  if ( input.KeyDown(DX_BACKTICK) ) DebugKeys();
#endif
 }
 void Render() {
  Blending(additive);
  fast.rrect(bgTint,bgTint,bgTint,x,y,w,h);
  if ( boughtBleep > 0.0 ) {
   Crayon t=overlayTint;
   t.ScaleByAlphaf((float)boughtBleep);
   Blending(additive);
   fast.rrect(t,t,t,x,y,w,h);   
   Blending(none);
   boughtBleep-=FRAMETIME;
  }
  if ( cantBuyBleep > 0.0 ) {
   Crayon t=invertOverlayTint;
   t.ScaleByAlphaf((float)cantBuyBleep);
   Blending(additive);
   fast.rrect(t,t,t,x,y,w,h);   
   Blending(none);
   cantBuyBleep-=FRAMETIME;
  }
  FORMAT(buf,128,"%d", (int) dollars);
  int len=strlen(buf);
  int nfw=(int) ((double)w/2.0/6.0);
  int nfh=(int) (h-4);
  int tfw=WidthTexFont(alarmClockNumbers,nfw,0,buf);
  WriteTexFont(alarmClockNumbers,color,additive,x+w-tfw-nfw*2-4,y+2,nfw,nfh,0,buf);
  WriteTexFont(alarmClockNumbers,color,additive,x+w-tfw-nfw*2-4,y+2,nfw,nfh,0,buf);
  FORMAT(buf,128,"%s%d",cents<10?"0":"", (int) cents);
  WriteTexFont(alarmClockNumbers,color,additive,x+w-nfw*2-4+2,y+2,nfw/2,nfh/2,0,buf);
  QuadiNoAlpha(dollarsign,color,additive,x,y+2,32,30);
  glColor3d(0.0,1.0,0.0);
  Text("pace",x+32-3,y+8,5,7,false);
  Text(" Bucks",x+32-3,y+8+10,5,7,false);
  Text(mag,x+w-36-24,y+31,5,7,false);
//  Text("pace Bucks",x+w/2+w/4+32,36,5,7,true); 
 }
 void DebugKeys() {  
  if ( input.KeyDown(DX_SPACE) ) testValue+=0.25;
  if ( input.KeyUp(DX_RETURN) ) testValue+=100.0;
  if ( input.KeyUp(DX_APOSTROPHE) ) testValue-=100.0;
  if ( input.KeyDown(DX_BACKSPACE) ) testValue-=100000.0;
  if ( input.KeyDown(DX_EQUALS) ) testValue+=1000000.0;
  if ( input.KeyDown(DX_MINUS) ) testValue=0.0;
  if ( input.KeyDown(DX_BACKSLASH) ) testValue+=1000000000.0;
  if ( input.KeyDown(DX_SLASH) ) testValue+=1000000000000.0;
  if ( input.KeyDown(DX_PERIOD) ) testValue+=1000000000000000.0f;
  if ( input.KeyDown(DX_COMMA) ) testValue+=1000000000000000000.0;
  if ( input.KeyDown(DX_M) ) testValue+=1000000000000000000000.0;
  if ( input.KeyDown(DX_N) ) testValue+=1000000000000000000000000.0;
  if ( input.KeyDown(DX_B) ) testValue+=1000000000000000000000000000.0;
  if ( input.KeyDown(DX_V) ) testValue+=1000000000000000000000000000000.0;
  if ( input.KeyDown(DX_C) ) testValue+=1000000000000000000000000000000000.0;
  if ( input.KeyDown(DX_1) ) speed+=1.0;
  if ( input.KeyDown(DX_2) ) speed-=1.0;
  if ( input.KeyDown(DX_3) ) speed=1.0;
 }
};

