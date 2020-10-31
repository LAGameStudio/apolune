#pragma once
#include "Audio.h"
#include "WrappingProce55Viewer2d.h"
#include "fx_Object3d.h"
#include "Tweens.h"
#include "fx_FireAndForget.h"
#include "Technology.h"
#include "Player.h"
#include "Globals.h"

extern Tweens tweens;

class Driftwood : public FAFProce55or {
public:
 Matters matters;
 MattersProjected3d projection;
 LookAtPerspective lap;
 ImmediateLighting lights;
 Cartesiand drift;
 Vertexd rotating;
 Vertexd rotationDelta;
 Variable *x_factor,*y_factor;
 float lifespan;
 float sizeRatio;
 bool exploding;
 bool randomNewWood;
 int w2;
 void Nothing() {}
 COMPONENT *c;
 PROGRAMMABLE(Driftwood,BehaviorPtr,behavior,Behavior);
 Driftwood() : FAFProce55or() {
  x_factor=y_factor=null;
  behavior=&Driftwood::Nothing;
 // lap.Near=0.0001f;
  lap.Far=50.0f;
  lap.eye.Set(0,0,30);
  lifespan=float_range(0.5f,25.0f);
  randomNewWood=true;
  if ( lifespan > 10.0f ) lifespan=float_range(10.0f,55.0f);
  exploding=false;
  rotating.x=(double)(float_range(0.0f,5.0f)*RandomSign());
  rotating.y=(double)(float_range(0.0f,5.0f)*RandomSign());
  drift.x=double_range(0.5,2.1)*(int)RandomSign();
  drift.y=double_range(0.5,2.1)*(int)RandomSign();
  rotationDelta.x=1.0f-(float) uniform()*2.0f;
  rotationDelta.y=1.0f-(float) uniform()*2.0f;
  duration=10.0f;
  projection.matters=&matters;
  c=null;
  w2=x=y=w=h=x2=y2=0;
  sizeRatio=1.0f;
 }
 void Extents( int x, int y, int h ) {
  w2=h/2;
  this->x=x-w2;
  this->y=y-w2;
  this->w=this->h=h;
  this->sizeRatio=(float)h/256.0f;
  this->x2=this->x+w;
  this->y2=this->y+h;
  if ( w2 > 256 ) projection.f=&fboScreen1;
  else projection.f=&fbo512;
 }
 void LoadTestModels(Substance *s, int complexity) {
  Matter *m=new Matter(lowPLYprimitives.Random(),s);
  projection.matters->Append(m);
  for ( int i=0; i<complexity; i++ ) {
   Substance *sub=new Substance;
   m=new Matter(lowPLYprimitives.Random(),s);//substances.Random());
   projection.matters->Append(m);
   m->position.z-=(float) (RandomSign()*1.0);
   m->position.y-=(float) (RandomSign()*1.0+(double)i/30.0);
   m->position.x-=(float) (RandomSign()*1.0);
   m->substance=substances.Random();
   projection.matters->substance=substances.Random();
  }
 }
 void fromComponent( COMPONENT *c ) {
  this->c=c;
  EACH(c->matters.first,Matter,m) {
   matters.Append(new Matter(m));
  }
  randomNewWood=false;
 }
 // Call to set up default
 void Controlled() {
  behavior=&Driftwood::Control;
  x_factor=controller.variables.NewfT("x-factor",0.0f,1.0f,1.0f,tweens.randomNot1());
  y_factor=controller.variables.NewfT("y-factor",0.0f,1.0f,1.0f,tweens.randomNot1());
 }
 void Control() {
  controller.Updatef();
  rotationDelta.y=rotating.x*x_factor->f;
  rotationDelta.x=rotating.y*y_factor->f;
 }
 void Between() {
  Behavior();
  projection.rotation.y+=(float) (rotating.x*rotationDelta.y); // up and down
  projection.rotation.x+=(float) (rotating.y*rotationDelta.x); // left and right
  MoveByd(drift.x,drift.y);
  lifespan-=FRAMETIME;
  if ( uniform() < 0.2 ) {
   Crayon color;
   color.Float(0.07f,0.06f,0.08f,1.0f);
   EmitPuff("data/images/flares/smoke10.png",color/*.fuzz(0.5f, 0.75f)*/,additive,
    float_range(1.0f,3.0f),
    x+w2,y+w2,w2,w2,
    tweens.find("Humanized Gradual Rise"),
    tweens.find("Humanized Gradual Fall"),
    0.0,0.0
    );
  }
  if ( lifespan < 2.0f && lifespan > 0.25f && uniform() < 0.2 ) {
   if ( uniform() < 0.2 ) {
    int sound=(int)(uniform()*10.0);
    string toplay;
    switch ( sound ) {
     case 0: toplay=string("data/sounds/fx/electric/lectric1.wav"); break;
     case 1: toplay=string("data/sounds/fx/electric/lectric2.wav"); break;
     case 2: toplay=string("data/sounds/fx/electric/lectric3.wav"); break;
     case 3: toplay=string("data/sounds/fx/electric/lectric4.wav"); break;
     case 4: toplay=string("data/sounds/fx/crackle2.wav"); break;
     case 5: toplay=string("data/sounds/fx/electric/lectric5.wav"); break;
     case 6: toplay=string("data/sounds/fx/pop2.wav"); break;         
     case 7: toplay=string("data/sounds/fx/pop3.wav"); break;
     case 8: toplay=string("data/sounds/fx/shutter.wav"); break;
     case 9: toplay=string("data/sounds/fx/electric/lectric6.wav"); break;
    }
    ALSound *playing=audio.manager.CueAndPlay((char *) toplay.c_str(),float_range(0.5f,1.5f),float_range(sizeRatio/2.0f,sizeRatio)*distanceGain(x,y,0.05f),false,true);
    playing->source->LinearPan(iratiof(x,display.w*2)-1.0f);
   }
   if ( x2<this->parent->x || y2<this->parent->y || x>this->parent->x2 || y>this->parent->y2 ) return;
   EmitLegacyPuff(weapon_flares.Any(),crayons.Any(),additive,
    float_range(1.0f,3.0f),
    x+w2,y+w2,w2,w2,
    tweens.find("Humanized Gradual Rise"),
    tweens.find("Humanized Gradual Fall"),
    0.0,0.0
    );
  }
 }
 void Render( GLWindow *surface ) {
  if ( x2<this->parent->x || y2<this->parent->y || x>this->parent->x2 || y>this->parent->y2 ) return;
  projection.f->background.Pick(clear);
  projection.f->Bind();
  glPush();
  lights.On();
  lap.Apply();
  projection.Render();
  lights.Off();
  glPop();
  projection.f->Unbind();
  projection.f->blend=transparency;
  projection.f->tint.Float(1.0f,1.0f,1.0f,(lifespan>0.25f?1.0f:lifespan/0.25f));
  projection.f->Render(x,y,x2,y2);
  glColor3d(0.0,1.0,0.0);
  Blending(none);
//  if ( x_factor ) Text((char *)x_factor->tween->name.c_str(),(double)x,(double)(y+h),5,7,true);
//  if ( y_factor ) Text((char *)y_factor->tween->name.c_str(),(double)x,(double)(y+h+10),5,7,true);
//  if ( c ) Text((char *)c->name.c_str(),(double)x,(double)(y+h+20),5,7,true);
//  char buf[64]; FORMAT(buf,64,"V: %1.3f", matters.RoughVolume()); Text(buf,x,y,5,7,false);
 }
 /*
 int mx,my;
 void LeftDown() {
  MousePosition(&mx,&my);
  if ( within(mx,my) ) {
   rotating.x=-rotating.x;
   rotating.y=-rotating.y;
   drift.x=-drift.x;
   drift.y=-drift.y;
  }
 }
 */
 bool Done() {
  bool done= ( !this->OnScreen() && lifespan <= 0.0f );
  if (!exploding && lifespan<0.25f) {
   exploding=true;
   float w2f=(float)(w/2.0f);
   Crayon color;
   if ( this->behavior == &Driftwood::Control ) color.Float(0.5f,1.0f,0.5f,1.0f);
   else color.Pick(alabaster);
   ALSound *playing=audio.manager.CueAndPlay(bang_sounds.AnyChar(),float_range(0.8f,1.2f),float_range(sizeRatio/2.0f,sizeRatio)*distanceGain(x,y,0.05f),false,true);
   playing->source->LinearPan(iratiof(x,display.w*2)-1.0f);
   EmitFilmRotatedRect(vault.find("Explosions","SFE Additive"),color,additive,1.0f,
    x+w2,y+h/2,h,float_range(0.0f,360.0f),0.0f,w2f);
   if ( randomNewWood ) EmitMoreWood(projection.matters->count/2,projection.matters->count/3);
   else ExplodeMatters();
  }
  return done;
 }
 void EmitFilmTrap( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w1, float w2 ) {
  fx_FilmTrap *f=new fx_FilmTrap(film,duration,angle);
  f->blend=blend;
  f->tint.fromCrayon(tint);
  f->spin=spin;
  f->x=emx;
  f->y=emy;
  f->h=h;
  f->w1=w1;
  f->w2=w2;
  faf->fx.Add(f,faf);
  f->Init();
 }
 void EmitFilmRotatedRect( Film *film, Crayon tint, Blends blend, float duration, int emx, int emy, int h, float angle, float spin, float w2 ) {
  fx_FilmRotatedRect *f=new fx_FilmRotatedRect(film,duration,angle);
  f->blend=blend;
  f->tint.fromCrayon(tint);
  f->spin=spin;
  f->x=emx;
  f->y=emy;
  f->h=h;
  f->w2=w2;
  faf->fx.Add(f,faf);
  f->Init();
 }
 void EmitMoreWood( int density, int complexity ) {
  if ( h <= 64 || density == 0 || complexity == 0 ) return;
  if ( randoms.Query() ) return;
  for ( int i=0; i < density; i++ ) {
   Driftwood *d=new Driftwood;
   d->Extents(
    x+w/2,
    y+h/2,
    number_range(32,h/2)
    );
   d->duration=10.0f;
   d->rotating.x=(double)(float_range(0.0f,5.0f)*RandomSign());
   d->rotating.y=(double)(float_range(0.0f,5.0f)*RandomSign());
   if ( number_range(0,20)==2 ) d->Controlled();
   else {
    d->rotationDelta.x=1.0f-(float) uniform()*2.0f;
    d->rotationDelta.y=1.0f-(float) uniform()*2.0f;
   }
   d->drift.x=double_range(0.5,2.5)*(int)RandomSign();
   d->drift.y=double_range(0.5,2.5)*(int)RandomSign();
   d->LoadTestModels(substances.Random(Textured),number_range(1,complexity));
   faf->AddPrepend(d,true);
  }
 }
 void ExplodeMatters() {
  if ( (matters.count > 4 && upto(2) == 0) ) {
   int splitA=matters.count/2+upto(matters.count/2);
   int splitB=splitA+upto(matters.count/2);
   if ( splitB < splitA ) { int temp=splitB; splitB=splitA; splitA=temp; }
   if ( splitB >= matters.count ) splitB = matters.count-1;
   if ( splitA > 0 ) {
    Driftwood *d=new Driftwood;
    d->EjectingMatters(this,0,splitA);
    d->lifespan*=0.75f;
    faf->Add(d,true);
   }
   if ( splitB-splitA > 0 ) {
    Driftwood *d=new Driftwood;
    d->EjectingMatters(this,splitA,splitB);
    d->lifespan*=0.75f;
    faf->Add(d,true);
   }
   if ( matters.count - 1 - splitB > 0 ) {
    Driftwood *d=new Driftwood;
    d->EjectingMatters(this,splitB,matters.count);//-1);
    d->lifespan*=0.75f;
    faf->Add(d,true);
   }
  } else if ( matters.count > 1 ) {
   int splitAt=upto(matters.count/2);
   if ( splitAt > 0 ) {
    Driftwood *d=new Driftwood;
    d->EjectingMatters(this,0,splitAt);
    d->lifespan*=0.75f;
    faf->Add(d,true);
   }
   if ( matters.count - 1 - splitAt > 0 ) {
    Driftwood *d=new Driftwood;
    d->EjectingMatters(this,splitAt,matters.count-1);
    d->lifespan*=0.75f;
    faf->Add(d,true);
   }
  } else {
   EmitMoreWood(projection.matters->count/2,projection.matters->count/3);
  }
 }
 void EjectingMatters( Driftwood *old, int start, int end ) {
//  int end=upto(matters.count);
  if ( old->matters.count == 0 ) return;
  if ( start==end || end==0 || end < start ) return;
  Matter *m=(Matter *)old->matters.first;
  int i=0;
  while ( i < start ) { m=(Matter *) m->next; i++; }
  for ( ; i<end; i++ ) {
   matters.Append(new Matter(m));
   m=(Matter *) m->next;
  }
  Extents(
   old->x+old->w2,
   old->y+old->w2,
   old->w
  );
  rotating.Set(old->rotating);
  if ( number_range(0,20)==2 ) Controlled();
  else {
   rotationDelta.x=1.0f-(float) uniform()*2.0f;
   rotationDelta.y=1.0f-(float) uniform()*2.0f;
  }
  drift.x=double_range(0.5,2.5)*(int)RandomSign();
  drift.y=double_range(0.5,2.5)*(int)RandomSign();
  randomNewWood=(matters.count<=2);
 }
};

// Only load once
class DriftwoodTest : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 float spaceDelay;
 void OnLoad() {
  spaceDelay=0.25f;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);

  int sizeW,sizeH,startX,startY;
  wrapLimit.SetRect(
   (startX=(-display->w*2)),
   (startY=(-display->h*2)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  BLUEPRINT *b=(BLUEPRINT *) players.active->profile.active->inventions.Any();
  OUTPUT("Picked `%s` as blueprint.\n", b->name.c_str() );

  Driftwood *part=new Driftwood;
  part->fromComponent(b->base.component);
  part->Extents(
   display->w2,display->h2,
   1024-256
   );
  part->sizeRatio=1.0f;
  part->faf=this;
  part->ExplodeMatters();
  delete part;

  EACH(b->components.first,Plug,p) {
   part=new Driftwood;
   part->fromComponent(p->component);
   part->Extents(
    display->w2,display->h2,
    p->settings.type == _COUPLER ? 256 : 128
    );
   part->faf=this;
   part->sizeRatio=(float)part->h/256.0f;
   Add(part);
  }

  for ( int i=0; i < 10; i++ ) {
   Driftwood *d=new Driftwood;
   d->Extents(
    display->w2, //(int) (uniform() * (double) sizeW/9)+0;//startX;
    display->h2,
     //(int) (uniform() * (double) sizeH/9)+0;//startY;
    number_range(32,256)
   );
   if ( number_range(0,20)==2 ) d->Controlled();
   else {
    d->rotationDelta.x=1.0f-(float) uniform()*2.0f;
    d->rotationDelta.y=1.0f-(float) uniform()*2.0f;
   }
   d->LoadTestModels(substances.Random(Textured),number_range(1,6));
   Add(d);
   fx_FilmRotatedRect *f=new fx_FilmRotatedRect(vault.find("Explosions","Round1"),0.75f+(float)uniform(),(float)(uniform()*360.0));
   f->blend=additive;
   f->tint.fromCrayon(crayons.Any());
   f->spin=(float)(uniform()*360.0);
   f->x=d->x+d->w;
   f->y=d->y+d->h;
   f->h=h;
   f->w2=(float)(d->w/2);
   Add(f);
  }
  Init();
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/sfe.wav",1.0f+float_range(0.25f,0.5f),3.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/spacenoiseloop.wav",1.0f+float_range(0.25f,0.5f),3.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/radio8.wav",1.0f+float_range(0.25f,0.5f),1.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/shutter.wav",1.0f+float_range(0.25f,0.5f),2.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/buffet.wav",1.0f+float_range(0.25f,0.5f),2.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/boom1.wav",1.0f+float_range(0.25f,0.5f),2.0f,false,true);
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/boom3.wav",1.0f+float_range(0.25f,0.5f),2.0f,false,true);
 }

 void Between() {
  int moveX=0,moveY=0;
  if ( input->KeyDown(DX_LEFT) ) moveX=-5;
  if ( input->KeyDown(DX_RIGHT) ) moveX=5;
  if ( input->KeyDown(DX_UP) ) moveY=-5;
  if ( input->KeyDown(DX_DOWN) ) moveY=5;
  if ( spaceDelay <= 0.0f && input->KeyDown(DX_SPACE) ) {
   spaceDelay=0.25f;
   fx.Clear();
   OnLoad();
  } else spaceDelay-=FRAMETIME;
  MoveBy(moveX,moveY); // This updates the "wrapping"
 }

 void PostProcessing() {
  char buf[128];
  FORMAT(buf,128,"FX: %d", (int) this->fx.count);
  glColor3d(1.0,0.0,0.0);
  Text(buf,10.0,10.0,5.0,7.0,false);
 }
};