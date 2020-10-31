/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "GLWindow.h"
#include "TextureLibrary.h"
#include "Tweens.h"
#include "FBO.h"
#include "UglyFont.h"
#include "LayerCombiner.h"
#include "Globals.h"
#include "Art2d.h"

class LayerCombinerTest : public GLWindow { 
 Zfloat time;
 Zfloat ts1,ts2,ts3,ts4,ts5;
 Zfloat t1,t2,t3,t4,t5; // tween index (0-tweens.count)
 Zfloat a1,b1,a2,b2,a3,b3,a4,b4,a5,b5; // scale factor
 Zfloat w1,w2,w3,w4,w5;
 Crayon c1,c2,c3,c4,c5;
 Zp<GLImage> pTex0,pTex1,pTex2,pTex3,pTex4;

 float keyDelay;
 bool iterativeABCDE;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Float(0.2f,0.2f,0.2f,1.0f);
  iterativeABCDE=false;
  time=0.0f;
  ts1=ts2=ts3=ts4=ts5=1.0f;
  t1=t2=t3=t4=t5=(float)uniform();
  a1=a2=a3=a4=a5=0.0f;
  b1=b2=b3=b4=b5=1.0f;
  w1=0.2f;
  w2=0.4f;
  w3=0.6f;
  w4=0.8f;
  w5=1.0f;
  c1.Pick(red255);
  c2.Pick(blue);
  c3.Pick(green);
  c4.Pick(orange);
  c5.Pick(violet);
  pTex0=library.Load("data/images/flares/neoflare1.png");
  pTex1=library.Load("data/images/flares/neoflare2.png");
  pTex2=library.Load("data/images/flares/neoflare3.png");
  pTex3=library.Load("data/images/flares/neoflare4.png");
  pTex4=library.Load("data/images/flares/neoflare5.png");
  keyDelay=0.0f;
 }
 void DebugOutput() {
  char buf[256];
  FORMAT(buf,256,"time: %1.2f\nw1: %1.2f\nw2: %1.2f\nw3: %1.2f\nw4: %1.2f\nw5: %1.2f\nts1: %1.2f\nts2: %1.2f\nts3: %1.2f\nts4: %1.2f\nts5: %1.2f\n", 
   (float) layerCombiner.time,
   (float) w1,
   (float) w2,
   (float) w3,
   (float) w4,
   (float) w5,
   (float) ts1,
   (float) ts2,
   (float) ts3,
   (float) ts4,
   (float) ts5
  );
  Text(buf,512.0,512.0,8.0,10.0);
 }
 void Between() {
  if ( input->KeyDown(DX_W) ) {
   w1=(float)uniform();
   w2=(float)uniform();
   w3=(float)uniform();
   w4=(float)uniform();
   w5=(float)uniform();
  }
  if ( input->KeyDown(DX_Q) ) {
   w1=0.9f;
   w2=0.9f;
   w3=0.9f;
   w4=0.9f;
   w5=0.9f;
  }
  if ( input->KeyDown(DX_E) ) {
   w1=0.1f;
   w2=0.1f;
   w3=0.1f;
   w4=0.1f;
   w5=0.1f;
  }
  if ( input->KeyDown(DX_UP) ) {
   w1+=w1*0.01f;
   w2+=w2*0.01f;
   w3+=w3*0.01f;
   w4+=w4*0.01f;
   w5+=w5*0.01f;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   w1*=0.99f;
   w2*=0.99f;
   w3*=0.99f;
   w4*=0.99f;
   w5*=0.99f;
  }
  if ( input->KeyDown(DX_1) ) {
   w1=1.0f;
  }
  if ( input->KeyDown(DX_2) ) {
   w2=1.0f;
  }
  if ( input->KeyDown(DX_3) ) {
   w3=1.0f;
  }
  if ( input->KeyDown(DX_4) ) {
   w4=1.0f;
  }
  if ( input->KeyDown(DX_5) ) {
   w5=1.0f;
  }
  if ( input->KeyDown(DX_6) ) {
   w1=0.0f;
  }
  if ( input->KeyDown(DX_7) ) {
   w2=0.0f;
  }
  if ( input->KeyDown(DX_8) ) {
   w3=0.0f;
  }
  if ( input->KeyDown(DX_9) ) {
   w4=0.0f;
  }
  if ( input->KeyDown(DX_0) ) {
   w5=0.0f;
  }
  if ( input->KeyDown(DX_LEFT) ) {
   ts1+=ts1*0.1f;
   ts2+=ts2*0.1f;
   ts3+=ts3*0.1f;
   ts4+=ts4*0.1f;
   ts5+=ts5*0.1f;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   ts1*=0.9f;
   ts2*=0.9f;
   ts3*=0.9f;
   ts4*=0.9f;
   ts5*=0.9f;
  }
  if ( input->KeyDown(DX_T) ) {
   ts1=(float)uniform();
   ts2=(float)uniform();
   ts3=(float)uniform();
   ts4=(float)uniform();
   ts5=(float)uniform()*3.0f;
  }
  if ( input->KeyDown(DX_SPACE) ) {
   t1=t2=t3=t4=t5=(float)uniform();
  }
  if ( input->KeyDown(DX_RETURN) ) {
   pTex0=weapon_flares.Any();
   pTex1=weapon_flares.Any();
   pTex2=weapon_flares.Any();
   pTex3=weapon_flares.Any();
   pTex4=weapon_flares.Any();
  }
  if ( input->KeyDown(DX_RSHIFT) ) {
   c1.Any();
   c2.Any();
   c3.Any();
   c4.Any();
   c5.Any();
  }
 }
 Cartesiand quad;
 void Render() {
  layerCombiner.time+=FRAMETIME;
  layerCombiner.t1=t1;
  layerCombiner.t2=t2;
  layerCombiner.t3=t3;
  layerCombiner.t4=t4;
  layerCombiner.t5=t5;
  layerCombiner.a1=a1;
  layerCombiner.a2=a2;
  layerCombiner.a3=a3;
  layerCombiner.a4=a4;
  layerCombiner.a5=a5;
  layerCombiner.b1=b1;
  layerCombiner.b2=b2;
  layerCombiner.b3=b3;
  layerCombiner.b4=b4;
  layerCombiner.b5=b5;
  layerCombiner.w1=w1;
  layerCombiner.w2=w2;
  layerCombiner.w3=w3;
  layerCombiner.w4=w4;
  layerCombiner.w5=w5;
  layerCombiner.ts1=ts1;
  layerCombiner.ts2=ts2;
  layerCombiner.ts3=ts3;
  layerCombiner.ts4=ts4;
  layerCombiner.ts5=ts5;
  layerCombiner.c1.fromCrayon(c1);
  layerCombiner.c2.fromCrayon(c2);
  layerCombiner.c3.fromCrayon(c3);
  layerCombiner.c4.fromCrayon(c4);
  layerCombiner.c5.fromCrayon(c5);
  layerCombiner.pTex0=&pTex0->texture;
  layerCombiner.pTex1=&pTex1->texture;
  layerCombiner.pTex2=&pTex2->texture;
  layerCombiner.pTex3=&pTex3->texture;
  layerCombiner.pTex4=&pTex4->texture;
  double angle = rad2deg(quad.LineAngle());
  art.Pivot(&layerCombiner,quad.x,quad.y,quad.w,quad.h,angle);
  DebugOutput();
 }
};


class LayerCombinerProfile : public ListItem {
public:
 Zfloat time;
 Zfloat ts1,ts2,ts3,ts4,ts5;
 Zfloat t1,t2,t3,t4,t5; // tween index (0-tweens.count)
 Zfloat a1,b1,a2,b2,a3,b3,a4,b4,a5,b5; // scale factor
 Zfloat w1,w2,w3,w4,w5;
 Crayon c1,c2,c3,c4,c5;
 Zp<GLImage> pTex0,pTex1,pTex2,pTex3,pTex4;
 LayerCombinerProfile() : ListItem() {
  time=0.0f;
  ts1=ts2=ts3=ts4=ts5=1.0f;
  t1=t2=t3=t4=t5=(float)uniform();
  a1=a2=a3=a4=a5=0.0f;
  b1=b2=b3=b4=b5=1.0f;
  w1=0.2f;
  w2=0.4f;
  w3=0.6f;
  w4=0.8f;
  w5=1.0f;
  c1.Pick(red255);
  c2.Pick(blue);
  c3.Pick(green);
  c4.Pick(orange);
  c5.Pick(violet);
  pTex0=library.Load("data/images/flares/neoflare1.png");
  pTex1=library.Load("data/images/flares/neoflare2.png");
  pTex2=library.Load("data/images/flares/neoflare3.png");
  pTex3=library.Load("data/images/flares/neoflare4.png");
  pTex4=library.Load("data/images/flares/neoflare5.png");
 }
 KEYWORDS({
  KEYWORD("time",{time=w;})
  else KEYWORD("ts1",{ts1=w;})
  else KEYWORD("ts2",{ts2=w;})
  else KEYWORD("ts3",{ts3=w;})
  else KEYWORD("ts4",{ts4=w;})
  else KEYWORD("ts5",{ts5=w;})
  else KEYWORD("t1",{t1=w;})
  else KEYWORD("t2",{t2=w;})
  else KEYWORD("t3",{t3=w;})
  else KEYWORD("t4",{t4=w;})
  else KEYWORD("t5",{t5=w;})
  else KEYWORD("a1",{a1=w;})
  else KEYWORD("b1",{b1=w;})
  else KEYWORD("a2",{a2=w;})
  else KEYWORD("b2",{b2=w;})
  else KEYWORD("a3",{a3=w;})
  else KEYWORD("b3",{b3=w;})
  else KEYWORD("a4",{a4=w;})
  else KEYWORD("b4",{b4=w;})
  else KEYWORD("a5",{a5=w;})
  else KEYWORD("b5",{b5=w;})
  else KEYWORD("w1",{w1=w;})
  else KEYWORD("w2",{w2=w;})
  else KEYWORD("w3",{w3=w;})
  else KEYWORD("w4",{w4=w;})
  else KEYWORD("w5",{w5=w;})
  else KEYWORD("c1",{c1.CSS(w);})
  else KEYWORD("c2",{c2.CSS(w);})
  else KEYWORD("c3",{c3.CSS(w);})
  else KEYWORD("c4",{c4.CSS(w);})
  else KEYWORD("c5",{c5.CSS(w);})
  else KEYWORD("pTex0",{pTex0=library.Load(w);})
  else KEYWORD("pTex1",{pTex1=library.Load(w);})
  else KEYWORD("pTex2",{pTex2=library.Load(w);})
  else KEYWORD("pTex3",{pTex3=library.Load(w);})
  else KEYWORD("pTex4",{pTex4=library.Load(w);})
 })
 WORDKEYS({
  SSUBKEY("time",time)
  SSUBKEY("ts1",ts1)
  SSUBKEY("ts2",ts2)
  SSUBKEY("ts3",ts3)
  SSUBKEY("ts4",ts4)
  SSUBKEY("ts5",ts5)
  SSUBKEY("t1",t1)
  SSUBKEY("t2",t2)
  SSUBKEY("t3",t3)
  SSUBKEY("t4",t4)
  SSUBKEY("t5",t5)
  SSUBKEY("a1",a1)
  SSUBKEY("b1",b1)
  SSUBKEY("a2",a2)
  SSUBKEY("b2",b2)
  SSUBKEY("a3",a3)
  SSUBKEY("b3",b3)
  SSUBKEY("a4",a4)
  SSUBKEY("b4",b4)
  SSUBKEY("a5",a5)
  SSUBKEY("b5",b5)
  SSUBKEY("w1",w1)
  SSUBKEY("w2",w2)
  SSUBKEY("w3",w3)
  SSUBKEY("w4",w4)
  SSUBKEY("w5",w5)
  SSUBKEYCustom("c1",c1,toCSS)
  SSUBKEYCustom("c2",c2,toCSS)
  SSUBKEYCustom("c3",c3,toCSS)
  SSUBKEYCustom("c4",c4,toCSS)
  SSUBKEYCustom("c5",c5,toCSS)
  if ( pTex0 ) WORDKEY("pTex0",pTex0->filename)
  if ( pTex1 ) WORDKEY("pTex1",pTex1->filename)
  if ( pTex2 ) WORDKEY("pTex2",pTex2->filename)
  if ( pTex3 ) WORDKEY("pTex3",pTex3->filename)
  if ( pTex4 ) WORDKEY("pTex4",pTex4->filename)
 })
 void Set( LayerCombinerProfile *lcp ) {
  time=lcp->time;
  ts1=lcp->ts1;
  ts2=lcp->ts2;
  ts3=lcp->ts3;
  ts4=lcp->ts4;
  ts5=lcp->ts5;
  t1=lcp->t1;
  t2=lcp->t2;
  t3=lcp->t3;
  t4=lcp->t4;
  t5=lcp->t5;
  a1=lcp->a1;
  a2=lcp->a2;
  a3=lcp->a3;
  a4=lcp->a4;
  a5=lcp->a5;
  b1=lcp->b1;
  b2=lcp->b2;
  b3=lcp->b3;
  b4=lcp->b4;
  b5=lcp->b5;
  w1=lcp->w1;
  w2=lcp->w2;
  w3=lcp->w3;
  w4=lcp->w4;
  w5=lcp->w5;
  c1.fromCrayon(lcp->c1);
  c2.fromCrayon(lcp->c2);
  c3.fromCrayon(lcp->c3);
  c4.fromCrayon(lcp->c4);
  c5.fromCrayon(lcp->c5);
  pTex0=lcp->pTex0;
  pTex1=lcp->pTex1;
  pTex2=lcp->pTex2;
  pTex3=lcp->pTex3;
  pTex4=lcp->pTex4;
 }
 void Apply() {
  layerCombiner.time=time;
  layerCombiner.a1=a1;
  layerCombiner.a2=a2;
  layerCombiner.a3=a3;
  layerCombiner.a4=a4;
  layerCombiner.a5=a5;
  layerCombiner.b1=b1;
  layerCombiner.b2=b2;
  layerCombiner.b3=b3;
  layerCombiner.b4=b4;
  layerCombiner.b5=b5;
  layerCombiner.w1=w1;
  layerCombiner.w2=w2;
  layerCombiner.w3=w3;
  layerCombiner.w4=w4;
  layerCombiner.w5=w5;
  layerCombiner.t1=t1;
  layerCombiner.t2=t2;
  layerCombiner.t3=t3;
  layerCombiner.t4=t4;
  layerCombiner.t5=t5;
  layerCombiner.ts1=ts1;
  layerCombiner.ts2=ts2;
  layerCombiner.ts3=ts3;
  layerCombiner.ts4=ts4;
  layerCombiner.ts5=ts5;
  layerCombiner.c1.fromCrayon(c1);
  layerCombiner.c2.fromCrayon(c2);
  layerCombiner.c3.fromCrayon(c3);
  layerCombiner.c4.fromCrayon(c4);
  layerCombiner.c5.fromCrayon(c5);
  layerCombiner.pTex0=&pTex0->texture;
  layerCombiner.pTex1=&pTex1->texture;
  layerCombiner.pTex2=&pTex2->texture;
  layerCombiner.pTex3=&pTex3->texture;
  layerCombiner.pTex4=&pTex4->texture;
 }
 void Between() {
  time+=FRAMETIME;
 }
 void Render( int x, int y, int w2, int h2 ) {
  Render((double)x,(double)y,(double)w2,double(h2));
 }
 void Render( double x, double y, double w2, double h2, double angle=0.0 ) {
  Apply(); 
  art.Pivot(&layerCombiner,x,y,w2,h2,angle);
 }
};