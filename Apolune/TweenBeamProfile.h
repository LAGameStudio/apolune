/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "GLWindow.h"
#include "TweenBeam.h"
#include "Tweens.h"
#include "FBO.h"
#include "UglyFont.h"
#include "TextureLibrary.h"
#include "Art2d.h"

class TweenBeamTest : public GLWindow {
public:
 int n,m,m0,m1,m2,m3,m4;
 float overlap;
 float ts0,ts1,ts2,ts3,ts4;
 float t0,t1,t2,t3,t4; // tween index (0-tweens.count)
 float a,b,c,d,e; // tween offset (0-1024)
 GLImage *pTex0,*pTex1,*pTex2,*pTex3,*pTex4;
 Crayon c0,c1,c2,c3,c4;
 float keyDelay;
 bool iterativeABCDE;
 GLint posAttrib,texAttrib;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Float(0.2f,0.2f,0.2f,1.0f);
  iterativeABCDE=false;
  ts0=ts1=ts2=ts3=ts4=1.0f;
  m=m0=m1=m2=m3=m4=0;
  t0=0.0f;
  t1=1.0f/(float)tweens.count;
  t2=2.0f/(float)tweens.count;
  t3=3.0f/(float)tweens.count;
  t4=4.0f/(float)tweens.count;
  a=756.0f;
  b=0.25f;
  c=0.5f;
  d=1.0f;
  e=1.25f;
  c0.Pick(red255);
  c1.Pick(blue);
  c2.Pick(green);
  c3.Pick(orange);
  c4.Pick(violet);
  overlap=30.0f;//0.1f;
  pTex0=library.Load("data/images/flares/neoflare1.png",tllto_Tiling);
  pTex1=library.Load("data/images/flares/neoflare2.png",tllto_Tiling);
  pTex2=library.Load("data/images/flares/neoflare3.png",tllto_Tiling);
  pTex3=library.Load("data/images/flares/neoflare4.png",tllto_Tiling);
  pTex4=library.Load("data/images/flares/neoflare5.png",tllto_Tiling);
  keyDelay=0.0f;
 }
 void Between() {
  if ( input->KeyDown(DX_LEFT) ) {
   t0+=1.0f/(float)tweens.count;
   t1+=1.0f/(float)tweens.count;
   t2+=1.0f/(float)tweens.count;
   t3+=1.0f/(float)tweens.count;
   t4+=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   t0-=1.0f/(float)tweens.count;
   t1-=1.0f/(float)tweens.count;
   t2-=1.0f/(float)tweens.count;
   t3-=1.0f/(float)tweens.count;
   t4-=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_UP) ) {
   overlap+=1.0f;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   overlap-=1.0f;
  }
  if ( keyDelay > 0.0f ) keyDelay-=FRAMETIME;
  if ( keyDelay <= 0.0f ) {
   if ( input->KeyDown(DX_A) ) {
    tweenBeam.iterations++;
    keyDelay=1.0f;
    OUTPUT("Iterations is %d", (int) tweenBeam.iterations);
   } else
   if ( input->KeyDown(DX_Z) ) {
    if ( tweenBeam.iterations > 1 ) tweenBeam.iterations--;
    keyDelay=1.0f;
    OUTPUT("Iterations is %d", (int) tweenBeam.iterations);
   } //else
  }
  if ( input->KeyDown(DX_0) ) {
   c0.Pick(black);
  }
  if ( input->KeyDown(DX_1) ) {
   c1.Pick(black);
  }
  if ( input->KeyDown(DX_2) ) {
   c2.Pick(black);
  }
  if ( input->KeyDown(DX_3) ) {
   c3.Pick(black);
  }
  if ( input->KeyDown(DX_4) ) {
   c4.Pick(black);
  }
  if ( input->KeyDown(DX_5) ) {
   c0.Any();
  }
  if ( input->KeyDown(DX_6) ) {
   c1.Any();
  }
  if ( input->KeyDown(DX_7) ) {
   c2.Any();
  }
  if ( input->KeyDown(DX_8) ) {
   c3.Any();
  }
  if ( input->KeyDown(DX_9) ) {
   c4.Any();
  }
  if ( input->KeyDown(DX_Q) ) {
   t0+=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_W) ) {
   t1+=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_E) ) {
   t2+=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_R) ) {
   t3+=1.0f/(float)tweens.count;
  }
  if ( input->KeyDown(DX_T) ) {
   t4+=1.0f/(float)tweens.count;
  }

   if ( input->KeyDown(DX_F) ) {
    ts0+=FRAMETIME;
   } else
   if ( input->KeyDown(DX_G) ) {
    ts1+=FRAMETIME;
   } else
   if ( input->KeyDown(DX_H) ) {
    ts2+=FRAMETIME;
   } else
   if ( input->KeyDown(DX_J) ) {
    ts3+=FRAMETIME;
   } else
   if ( input->KeyDown(DX_K) ) {
    ts4+=FRAMETIME;
   } else
   if ( input->KeyDown(DX_C) ) {
    ts0-=FRAMETIME;
   } else
   if ( input->KeyDown(DX_V) ) {
    ts1-=FRAMETIME;
   } else
   if ( input->KeyDown(DX_B) ) {
    ts2-=FRAMETIME;
   } else
   if ( input->KeyDown(DX_N) ) {
    ts3-=FRAMETIME;
   } else
   if ( input->KeyDown(DX_M) ) {
    ts4-=FRAMETIME;
   }
   if ( input->KeyDown(DX_Y) ) {
    if ( m0 == 0 ) m0=1;
    else m0=0;
   } else
   if ( input->KeyDown(DX_U) ) {
    if ( m1 == 0 ) m1=1;
    else m1=0;
   } else
   if ( input->KeyDown(DX_I) ) {
    if ( m2 == 0 ) m2=1;
    else m2=0;
   } else
   if ( input->KeyDown(DX_O) ) {
    if ( m3 == 0 ) m3=1;
    else m3=0;
   } else
   if ( input->KeyDown(DX_P) ) {
    if ( m4 == 0 ) m4=1;
    else m4=0;
   } else
   if ( input->KeyDown(DX_SPACE) ) {
    if ( n == 0 ) n=1;
    else if ( n == 1 ) n=2;
    else n=0;
   }else
   if ( input->KeyDown(DX_RETURN) ) {
    if ( m == 0 ) m=1;
    else if ( m == 1 ) m=2;
    else m=0;
   }

   if ( input->KeyDown(DX_RBRACKET) && keyDelay < 0.0f ) {
    iterativeABCDE=!iterativeABCDE;
    keyDelay=1.0f;
   }
   if ( input->KeyDown(DX_LBRACKET) ) {
    ts0=ts1=ts2=ts3=ts4=1.0f;
   }
   if ( iterativeABCDE ) {
    a+=FRAMETIME;
    b+=FRAMETIME;
    c+=FRAMETIME;
    d+=FRAMETIME;
    e+=FRAMETIME;
   }
 }
 void Render() {
  tweenBeam.time+=FRAMETIME;
  tweenBeam.overlap=overlap;
  tweenBeam.n=n;
  tweenBeam.m=m;
  tweenBeam.m0=m0;
  tweenBeam.m1=m1;
  tweenBeam.m2=m2;
  tweenBeam.m3=m3;
  tweenBeam.m4=m4;
  tweenBeam.t0=t0;
  tweenBeam.t1=t1;
  tweenBeam.t2=t2;
  tweenBeam.t3=t3;
  tweenBeam.t4=t4;
  tweenBeam.ts0=ts0;
  tweenBeam.ts1=ts1;
  tweenBeam.ts2=ts2;
  tweenBeam.ts3=ts3;
  tweenBeam.ts4=ts4;
  tweenBeam.a=a;
  tweenBeam.b=b;
  tweenBeam.c=c;
  tweenBeam.d=d;
  tweenBeam.e=e;
  tweenBeam.c0.fromCrayon(c0);
  tweenBeam.c1.fromCrayon(c1);
  tweenBeam.c2.fromCrayon(c2);
  tweenBeam.c3.fromCrayon(c3);
  tweenBeam.c4.fromCrayon(c4);
  tweenBeam.pTex0=&pTex0->texture;
  tweenBeam.pTex1=&pTex1->texture;
  tweenBeam.pTex2=&pTex2->texture;
  tweenBeam.pTex3=&pTex3->texture;
  tweenBeam.pTex4=&pTex4->texture;
  art.Stretch(&tweenBeam,64,64,1024,1024);
 }
};



class TweenBeamProfile : public ListItem {
public:
 Zint it,n,m,m0,m1,m2,m3,m4; // it is an iteration count, typically between 0-10 up to 64 works, n=0,1,2 (add,sub,addavg) m=0,1 (avg) m0to4=0,1 (add/sub)
 Zfloat overlap; // a number, default 30.0f, which controls one dimension of the beam
 Zfloat ts0,ts1,ts2,ts3,ts4; // time scale
 Zfloat t0,t1,t2,t3,t4; // tween index (0-tweens.count)
 Zfloat a,b,c,d,e; // tween offset (0-1024)
 Zp<GLImage> pTex0,pTex1,pTex2,pTex3,pTex4; // flare textures to use
 Crayon c0,c1,c2,c3,c4; // colors to use
 Zfloat widthHint; // a number like 2-64f which effects width of drawn beam
 Zfloat lo_temperature,lo_nions,lo_infrared,lo_rads; // use BeamTestFiring and ResetReadings 
 Zfloat hi_temperature,hi_nions,hi_infrared,hi_rads; // use BeamTestFiring and ResetReadings
 TweenBeamProfile() : ListItem() {
  it=10;
  ts0=1.0f;
  ts1=1.0f;
  ts2=1.0f;
  ts3=1.0f;
  ts4=1.0f;
  t1=1.0f/(float)tweens.count;
  t2=2.0f/(float)tweens.count;
  t3=3.0f/(float)tweens.count;
  t4=4.0f/(float)tweens.count;
  a=0.0f;
  b=0.25f;
  c=0.5f;
  d=1.0f;
  e=1.25f;
  c0.Pick(red255);
  c1.Pick(black);
  c2.Pick(black);
  c3.Pick(black);
  c4.Pick(black);
  overlap=30.0f;
  widthHint=32.0f;
 }

 KEYWORDS({
  KEYWORD("it",{it=w;})
  else KEYWORD("n",{n=w;})
  else KEYWORD("m",{m=w;})
  else KEYWORD("m0",{m0=w;})
  else KEYWORD("m1",{m1=w;})
  else KEYWORD("m2",{m2=w;})
  else KEYWORD("m3",{m3=w;})
  else KEYWORD("m4",{m4=w;})
  else KEYWORD("t0",{t0=w;})
  else KEYWORD("t1",{t1=w;})
  else KEYWORD("t2",{t2=w;})
  else KEYWORD("t3",{t3=w;})
  else KEYWORD("t4",{t4=w;})
  else KEYWORD("ts0",{ts0=w;})
  else KEYWORD("ts1",{ts1=w;})
  else KEYWORD("ts2",{ts2=w;})
  else KEYWORD("ts3",{ts3=w;})
  else KEYWORD("ts4",{ts4=w;})
  else KEYWORD("a",{a=w;})
  else KEYWORD("b",{b=w;})
  else KEYWORD("c",{c=w;})
  else KEYWORD("d",{d=w;})
  else KEYWORD("e",{e=w;})
  else KEYWORD("c0",{c0.CSS(w);})
  else KEYWORD("c1",{c1.CSS(w);})
  else KEYWORD("c2",{c2.CSS(w);})
  else KEYWORD("c3",{c3.CSS(w);})
  else KEYWORD("c4",{c4.CSS(w);})
  else KEYWORD("pTex0",{pTex0=library.Load(w);})
  else KEYWORD("pTex1",{pTex1=library.Load(w);})
  else KEYWORD("pTex2",{pTex2=library.Load(w);})
  else KEYWORD("pTex3",{pTex3=library.Load(w);})
  else KEYWORD("pTex4",{pTex4=library.Load(w);})
  else KEYWORD("widthHint",{widthHint=w;})
  else KEYWORD("lo_temperature",{lo_temperature=w;})
  else KEYWORD("lo_nions",{lo_nions=w;})
  else KEYWORD("lo_infrared",{lo_infrared=w;})
  else KEYWORD("lo_rads",{lo_rads=w;})
  else KEYWORD("hi_temperature",{hi_temperature=w;})
  else KEYWORD("hi_nions",{hi_nions=w;})
  else KEYWORD("hi_infrared",{hi_infrared=w;})
  else KEYWORD("hi_rads",{hi_rads=w;})
 })
 WORDKEYS({
  SSUBKEY("it",it)
  SSUBKEY("n",n)
  SSUBKEY("m",m)
  SSUBKEY("m0",m0)
  SSUBKEY("m1",m1)
  SSUBKEY("m2",m2)
  SSUBKEY("m3",m3)
  SSUBKEY("m4",m4)
  SSUBKEY("t0",t0)
  SSUBKEY("t1",t1)
  SSUBKEY("t2",t2)
  SSUBKEY("t3",t3)
  SSUBKEY("t4",t4)
  SSUBKEY("ts5",ts0)
  SSUBKEY("ts1",ts1)
  SSUBKEY("ts2",ts2)
  SSUBKEY("ts3",ts3)
  SSUBKEY("ts4",ts4)
  SSUBKEY("a",a)
  SSUBKEY("b",b)
  SSUBKEY("c",c)
  SSUBKEY("d",d)
  SSUBKEY("e",e)
  SSUBKEYCustom("c0",c0,toCSS)
  SSUBKEYCustom("c1",c1,toCSS)
  SSUBKEYCustom("c2",c2,toCSS)
  SSUBKEYCustom("c3",c3,toCSS)
  SSUBKEYCustom("c4",c4,toCSS)
  if ( pTex0 ) WORDKEY("pTex0",pTex0->filename)
  if ( pTex1 ) WORDKEY("pTex1",pTex1->filename)
  if ( pTex2 ) WORDKEY("pTex2",pTex2->filename)
  if ( pTex3 ) WORDKEY("pTex3",pTex3->filename)
  if ( pTex4 ) WORDKEY("pTex4",pTex4->filename)
  SSUBKEY("widthHint",widthHint)
  SSUBKEY("lo_temperature",lo_temperature)
  SSUBKEY("lo_nions",lo_nions)
  SSUBKEY("lo_infrared",lo_infrared)
  SSUBKEY("lo_rads",lo_rads)
  SSUBKEY("hi_temperature",hi_temperature)
  SSUBKEY("hi_nions",hi_nions)
  SSUBKEY("hi_infrared",hi_infrared)
  SSUBKEY("hi_rads",hi_rads)
 })
 void Defaults() {
  pTex0=library.Load("data/images/flares/neoflare1.png",tllto_Tiling);
  pTex1=library.Load("data/images/flares/neoflare2.png",tllto_Tiling);
  pTex2=library.Load("data/images/flares/neoflare3.png",tllto_Tiling);
  pTex3=library.Load("data/images/flares/neoflare4.png",tllto_Tiling);
  pTex4=library.Load("data/images/flares/neoflare5.png",tllto_Tiling);
 }
 void Set( TweenBeamProfile *tbp ) {
  it=tbp->it;
  n=tbp->n;
  m=tbp->m;
  m0=tbp->m0;
  m1=tbp->m1;
  m2=tbp->m2;
  m3=tbp->m3;
  m4=tbp->m4;
  overlap=tbp->overlap;
  ts0=tbp->ts0;
  ts1=tbp->ts1;
  ts2=tbp->ts2;
  ts3=tbp->ts3;
  ts4=tbp->ts4;
  t0=tbp->t0;
  t1=tbp->t1;
  t2=tbp->t2;
  t3=tbp->t3;
  t4=tbp->t4;
  a=tbp->a;
  b=tbp->b;
  c=tbp->c;
  d=tbp->d;
  e=tbp->e;
  pTex0=tbp->pTex0;
  pTex1=tbp->pTex1;
  pTex2=tbp->pTex2;
  pTex3=tbp->pTex3;
  pTex4=tbp->pTex4;
  c0.fromCrayon(tbp->c0);
  c1.fromCrayon(tbp->c1);
  c2.fromCrayon(tbp->c2);
  c3.fromCrayon(tbp->c3);
  c4.fromCrayon(tbp->c4);
  widthHint=tbp->widthHint;
  lo_temperature=tbp->lo_temperature;
  lo_nions=tbp->lo_nions;
  lo_infrared=tbp->lo_infrared;
  lo_rads=tbp->lo_rads;
  hi_temperature=tbp->hi_temperature;
  hi_nions=tbp->hi_nions;
  hi_infrared=tbp->hi_infrared;
  hi_rads=tbp->hi_rads;
 }
 void BinaryRead( BinaryFile *open ) {
  open->read(&it);
  open->read(&n);
  open->read(&m);
  open->read(&m0);
  open->read(&m1);
  open->read(&m2);
  open->read(&m3);
  open->read(&m4);
  open->read(&overlap);
  open->read(&ts0);
  open->read(&ts1);
  open->read(&ts2);
  open->read(&ts3);
  open->read(&ts4);
  open->read(&t0);
  open->read(&t1);
  open->read(&t2);
  open->read(&t3);
  open->read(&t4);
  open->read(&a);
  open->read(&b);
  open->read(&c);
  open->read(&d);
  open->read(&e);
  string image;
  open->read(&image); pTex0=library.Load(image.c_str(),tllto_Tiling);
  open->read(&image); pTex1=library.Load(image.c_str(),tllto_Tiling);
  open->read(&image); pTex2=library.Load(image.c_str(),tllto_Tiling);
  open->read(&image); pTex3=library.Load(image.c_str(),tllto_Tiling);
  open->read(&image); pTex4=library.Load(image.c_str(),tllto_Tiling);
  c0.BinaryRead(open);
  c1.BinaryRead(open);
  c2.BinaryRead(open);
  c3.BinaryRead(open);
  c4.BinaryRead(open);
  open->read(&widthHint);
  open->read(&lo_temperature);
  open->read(&lo_nions);
  open->read(&lo_infrared);
  open->read(&lo_rads);
  open->read(&hi_temperature);
  open->read(&hi_nions);
  open->read(&hi_infrared);
  open->read(&hi_rads);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->scrawl((int)it);
  open->scrawl((int)n);
  open->scrawl((int)m);
  open->scrawl((int)m0);
  open->scrawl((int)m1);
  open->scrawl((int)m2);
  open->scrawl((int)m3);
  open->scrawl((int)m4);
  open->scrawl((float)overlap);
  open->scrawl((float)ts0);
  open->scrawl((float)ts1);
  open->scrawl((float)ts2);
  open->scrawl((float)ts3);
  open->scrawl((float)ts4);
  open->scrawl((float)t0);
  open->scrawl((float)t1);
  open->scrawl((float)t2);
  open->scrawl((float)t3);
  open->scrawl((float)t4);
  open->scrawl((float)a);
  open->scrawl((float)b);
  open->scrawl((float)c);
  open->scrawl((float)d);
  open->scrawl((float)e);
  open->scrawl(pTex0->filename); 
  open->scrawl(pTex1->filename); 
  open->scrawl(pTex2->filename); 
  open->scrawl(pTex3->filename); 
  open->scrawl(pTex4->filename); 
  c0.BinaryWrite(open);
  c1.BinaryWrite(open);
  c2.BinaryWrite(open);
  c3.BinaryWrite(open);
  c4.BinaryWrite(open);
  open->scrawl((float)widthHint);
  open->scrawl((float)lo_temperature);
  open->scrawl((float)lo_nions);
  open->scrawl((float)lo_infrared);
  open->scrawl((float)lo_rads);
  open->scrawl((float)hi_temperature);
  open->scrawl((float)hi_nions);
  open->scrawl((float)hi_infrared);
  open->scrawl((float)hi_rads);
 }
 void ResetReadings() {  
  lo_temperature=lo_nions=lo_infrared=lo_rads=hi_temperature=hi_nions=hi_infrared=hi_rads=0;
 }
 void BeamTestFiring() {
  float temperature,nionization,infrared,radioactivity;
  temperature=0.0f;
  nionization=0.0f;
  infrared=0.0f;
  radioactivity=0.0f;
  glPush();
  fbo512.background.Pick(black);
  fbo512.Bind();
  Apply();
  fbo512.QuadYInverted(&tweenBeam);
  fbo512.Unbind();
  glFinish();
  float bgra[4];
  bgra[0]=bgra[1]=bgra[2]=bgra[3]=0.0f;
  fbo512.BindWithoutClearing();
  for ( int i=1; i<19; i++ ) {
   for ( int j=1; j<19; j++ ) {
    fbo512.ReadPixel(gl_color0, (512/20)*i, (512/20)*j);
    bgra[0]+=(float)fbo512.ub_bgra[0]; // coming out as RGBA in FBOColor..
    bgra[1]+=(float)fbo512.ub_bgra[1];
    bgra[2]+=(float)fbo512.ub_bgra[2];
    bgra[3]+=(float)fbo512.ub_bgra[3];
   }
  }
  fbo512.Unbind();
  glPop();
  fbo512.background.Pick(clear);
  temperature=((bgra[0]+bgra[1]+bgra[2])/3.0f)/19.0f;
  nionization=(bgra[1]+bgra[0]-bgra[2])/19.0f;
  infrared=(bgra[2]-bgra[0]+bgra[1]/2.0f)/19.0f;
  radioactivity=bgra[1]/19.0f;
  if ( lo_temperature == 0.0f || temperature < lo_temperature ) lo_temperature=temperature;
  if ( hi_temperature == 0.0f || temperature > hi_temperature ) hi_temperature=temperature;
  if ( lo_nions == 0.0f || nionization < lo_nions ) lo_nions=nionization;
  if ( hi_nions == 0.0f || nionization > hi_nions ) hi_nions=nionization;
  if ( lo_infrared == 0.0f || infrared < lo_infrared ) lo_infrared=infrared;
  if ( hi_infrared == 0.0f || infrared > hi_infrared ) hi_infrared=infrared;
  if ( lo_rads == 0.0f || radioactivity < lo_rads ) lo_rads=radioactivity;
  if ( hi_rads == 0.0f || radioactivity > hi_rads ) hi_rads=radioactivity;
  glFinish();
 }
 void FakeBeamTestFiring() {
  float temperature,nionization,infrared,radioactivity;
  temperature=0.0f;
  nionization=0.0f;
  infrared=0.0f;
  radioactivity=0.0f;
  float r,g,b,c;
  r=(c0.rf+c1.rf+c2.rf+c3.rf+c4.rf)/4.0f;
  g=(c0.gf+c1.gf+c2.gf+c3.gf+c4.gf)/4.0f;
  b=(c0.bf+c1.bf+c2.bf+c3.bf+c4.bf)/4.0f;
  c=(r+g+b)/3.0f;
  temperature=c*255.0f;
  nionization=(g+b-r)*255.0f;
  infrared=(r-b+g/2.0f)*255.0f;
  radioactivity=g*255.0f;
  if ( lo_temperature == 0.0f || temperature < lo_temperature ) lo_temperature=temperature;
  if ( hi_temperature == 0.0f || temperature > hi_temperature ) hi_temperature=temperature;
  if ( lo_nions == 0.0f || nionization < lo_nions ) lo_nions=nionization;
  if ( hi_nions == 0.0f || nionization > hi_nions ) hi_nions=nionization;
  if ( lo_infrared == 0.0f || infrared < lo_infrared ) lo_infrared=infrared;
  if ( hi_infrared == 0.0f || infrared > hi_infrared ) hi_infrared=infrared;
  if ( lo_rads == 0.0f || radioactivity < lo_rads ) lo_rads=radioactivity;
  if ( hi_rads == 0.0f || radioactivity > hi_rads ) hi_rads=radioactivity;
 }
 void Apply() {
  tweenBeam.iterations=it;
  tweenBeam.overlap=overlap;
  tweenBeam.n=n;
  tweenBeam.m=m;
  tweenBeam.m0=m0;
  tweenBeam.m1=m1;
  tweenBeam.m2=m2;
  tweenBeam.m3=m3;
  tweenBeam.m4=m4;
  tweenBeam.t0=t0;
  tweenBeam.t1=t1;
  tweenBeam.t2=t2;
  tweenBeam.t3=t3;
  tweenBeam.t4=t4;
  tweenBeam.ts0=ts0;
  tweenBeam.ts1=ts1;
  tweenBeam.ts2=ts2;
  tweenBeam.ts3=ts3;
  tweenBeam.ts4=ts4;
  tweenBeam.a=a;
  tweenBeam.b=b;
  tweenBeam.c=c;
  tweenBeam.d=d;
  tweenBeam.e=e;
  tweenBeam.c0.fromCrayon(c0);
  tweenBeam.c1.fromCrayon(c1);
  tweenBeam.c2.fromCrayon(c2);
  tweenBeam.c3.fromCrayon(c3);
  tweenBeam.c4.fromCrayon(c4);
  tweenBeam.pTex0=&pTex0->texture;
  tweenBeam.pTex1=&pTex1->texture;
  tweenBeam.pTex2=&pTex2->texture;
  tweenBeam.pTex3=&pTex3->texture;
  tweenBeam.pTex4=&pTex4->texture;
 }
 void Render( int x, int y, int x2, int y2 ) {
  Apply();
  tweenBeam.time+=FRAMETIME;
  Blending(additive);
  art.Stretch(&tweenBeam,x,y,x2,y2);
 }
 void Render( double x, double y, double width, double length, double angle ) {
  double w2=width/2.0;
  double h2=length/2.0;
  Apply();
  tweenBeam.time+=FRAMETIME;
  art.Pivot(&tweenBeam,x,y,h2,w2,angle);
 }
};

class TemperatureTestTest : public GLWindow {
public:
 TweenBeamProfile tbp;
 float keyDelay;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  keyDelay=0.0f;
 }
 void Render() {
  tbp.BeamTestFiring();
  tbp.Render( 64,64,64+1024,64+256 );
  char buf[128];
  Blending(none);
  FORMAT(buf,128,"Temperature: %1.2f - %1.2f Ionization: %1.2f - %1.2f Infrared: %1.2f - %1.2f RADS: %1.2f - %1.2f",
   (float) tbp.lo_temperature,
   (float) tbp.hi_temperature,
   (float) tbp.lo_nions,
   (float) tbp.hi_nions,
   (float) tbp.lo_infrared,
   (float) tbp.hi_infrared,
   (float) tbp.lo_rads,
   (float) tbp.hi_rads);
  glColor3d(1.0,1.0,1.0);
  Text(buf,10,10,5,7,false);
 }
 void Between() {
  tweenBeam.time+=FRAMETIME;
  if ( input->KeyDown(DX_LEFT) ) {
   tbp.t0+=1.0f/(float)tweens.count;
   tbp.t1+=1.0f/(float)tweens.count;
   tbp.t2+=1.0f/(float)tweens.count;
   tbp.t3+=1.0f/(float)tweens.count;
   tbp.t4+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   tbp.t0-=1.0f/(float)tweens.count;
   tbp.t1-=1.0f/(float)tweens.count;
   tbp.t2-=1.0f/(float)tweens.count;
   tbp.t3-=1.0f/(float)tweens.count;
   tbp.t4-=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_UP) ) {
   tbp.overlap+=1.0f;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_DOWN) ) {
   tbp.overlap-=1.0f;
   tbp.ResetReadings();
  }
  if ( keyDelay > 0.0f ) keyDelay-=FRAMETIME;
  if ( keyDelay <= 0.0f ) {
   if ( input->KeyDown(DX_A) ) {
    tbp.it++;
    tbp.ResetReadings();
    keyDelay=1.0f;
    OUTPUT("Iterations is %d", (int) tbp.it);
   } else
   if ( input->KeyDown(DX_Z) ) {
    if ( tbp.it > 1 ) tbp.it--;
    tbp.ResetReadings();
    keyDelay=1.0f;
    OUTPUT("Iterations is %d", (int) tbp.it);
   } //else
  }
  if ( input->KeyDown(DX_0) ) {
   tbp.c0.Pick(black);
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_1) ) {
   tbp.c1.Pick(black);
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_2) ) {
   tbp.c2.Pick(black);
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_3) ) {
   tbp.c3.Pick(black);
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_4) ) {
   tbp.c4.Pick(black);
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_5) ) {
   tbp.c0.Any();
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_6) ) {
   tbp.c1.Any();
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_7) ) {
   tbp.c2.Any();
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_8) ) {
   tbp.c3.Any();
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_9) ) {
   tbp.c4.Any();
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_Q) ) {
   tbp.t0+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_W) ) {
   tbp.t1+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_E) ) {
   tbp.t2+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_R) ) {
   tbp.t3+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }
  if ( input->KeyDown(DX_T) ) {
   tbp.t4+=1.0f/(float)tweens.count;
   tbp.ResetReadings();
  }

   if ( input->KeyDown(DX_F) ) {
    tbp.ts0+=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_G) ) {
    tbp.ts1+=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_H) ) {
    tbp.ts2+=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_J) ) {
    tbp.ts3+=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_K) ) {
    tbp.ts4+=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_C) ) {
    tbp.ts0-=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_V) ) {
    tbp.ts1-=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_B) ) {
    tbp.ts2-=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_N) ) {
    tbp.ts3-=FRAMETIME;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_M) ) {
    tbp.ts4-=FRAMETIME;
    tbp.ResetReadings();
   }
   if ( input->KeyDown(DX_Y) ) {
    if ( tbp.m0 == 0 ) tbp.m0=1;
    else tbp.m0=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_U) ) {
    if ( tbp.m1 == 0 ) tbp.m1=1;
    else tbp.m1=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_I) ) {
    if ( tbp.m2 == 0 ) tbp.m2=1;
    else tbp.m2=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_O) ) {
    if ( tbp.m3 == 0 ) tbp.m3=1;
    else tbp.m3=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_P) ) {
    if ( tbp.m4 == 0 ) tbp.m4=1;
    else tbp.m4=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_SPACE) ) {
    if ( tbp.n == 0 ) tbp.n=1;
    else if ( tbp.n == 1 ) tbp.n=2;
    else tbp.n=0;
    tbp.ResetReadings();
   } else
   if ( input->KeyDown(DX_RETURN) ) {
    if ( tbp.m == 0 ) tbp.m=1;
    else if ( tbp.m == 1 ) tbp.m=2;
    else tbp.m=0;
    tbp.ResetReadings();
   }
   if ( input->KeyDown(DX_LBRACKET) ) {
    tbp.ts0=tbp.ts1=tbp.ts2=tbp.ts3=tbp.ts4=1.0f;
    tbp.ResetReadings();
   }
 }
};