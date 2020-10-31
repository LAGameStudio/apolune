#pragma once
#include "fx_FireAndForget.h"
#include "Boids2d.h"
/*
class SimpleSpaceCritter : public FAFProce55or {
public:
 Boid2d *boid;
 Boids2d *boids;
 GLImage *icon;
 Crayon tint;
 Cartesian screen;
 MapPosition map;
 float turnMaxSpeed,turnSpeed,torque,turning,turningTowards,heading,
   speed,targetSpeed,maxSpeed,accelerationRate,targettingDelay;
 CritterModes mode,oldMode;
 float health;
 float conciousness; // when 0, a creature is stunned or sleeping
 MapPosition destination;
 MapPosition *target;
 bool dead;
 Tween *yMove;
 Tween *xMove;
 float xDisplace,yDisplace,xDisplacing,yDisplacing;
 float wakes,waits;
 string emoteSound,attackSound,fleeSound;
 string prey,rival,friends;
 bool spits,claws,wimpy,herbivore;
 double lazyness,aggression;
 float sedentariness;
 Cartesiand calculator;
 float movedx,movedy;
 float approach;
 int seed,randomer;
 SimpleSpaceCritter( int seed ) : FAFProce55or() {
  this->seed=seed;
  emoteSound=string("");
  attackSound=string("");
  fleeSound=string("");
  icon=library.find(simple_critters[seed%4]);
  w=icon->width;
  h=icon->height;
  x2=x+w;
  y2=y+h;
  movedx=movedy=0;
  tint.Any();
  oldMode=critter_born;
  mode=critter_wander;
  speed=0.0f;
  target=&destination;
  targetSpeed=0.0f;
  maxSpeed=5.0f;
  accelerationRate=0.5f;
  health=1.0f;
  conciousness=1.0f;
  turning=0.0f;
  turnSpeed=0.0f;
  turnMaxSpeed=5.0f;
  targetSpeed=0.0f;
  turningTowards=float_range(0.0f,359.99999f);
  torque=0.5f;
  dead=false;
  waits=wakes=0.0f;
  heading=float_range(0.0f,359.99999f);
  aggression=0.1;
  lazyness=0.1;
  sedentariness=3.0f;
  xMove=null;
  yMove=null;
  xDisplacing=yDisplace=xDisplace=yDisplace=0.0f;
  targettingDelay=0.0f;
  herbivore=spits=claws=wimpy=false;
  prey=string("");
  rival=string("");
  friends=string("");
  approach=16.0f;
  boid=null;
  randomer=0;
 }
 virtual void SetPosition( int x, int y ) {
  screen.x=this->x=x;
  screen.y=this->y=y;
  screen.x2=this->x2=x+w;
  screen.y2=this->y2=y+h;
  screen.w=this->w;
  screen.h=this->h;
  map.position.x=(double)x;
  map.position.y=(double)y;
  Rezone();
 }
 virtual void SetPosition( int x, int y, int w, int h ) {
  screen.x=this->x=x;
  screen.y=this->y=y;
  screen.x2=this->x2=x+w;
  screen.y2=this->y2=y+h;
  screen.w=this->w=w;
  screen.h=this->h=h;
  map.position.x=(double)x;
  map.position.y=(double)y;
  Rezone();
 }
 virtual void Translate( Cartesiand *screen ) {
  translated.w=this->screen.w;
  translated.h=this->screen.h;
  translated.x=this->screen.x-(int)screen->x;
  translated.y=this->screen.y-(int)screen->y;
  translated.x2=this->screen.x2-(int)screen->x;
  translated.y2=this->screen.y2-(int)screen->y;
 }
 virtual int Y() {  return translated.y+translated.h; }
 void MoveBy( int xx, int yy ) {
  map.position.x+=(double)xx;
  map.position.y+=(double)yy;
  x+=xx;
  x2+=xx;
  y+=yy;
  y2+=yy;
  Rezone();
  WhenMoved();
 }
 bool Collide( int mx, int my ) {
 }
 void Stun( float one ) {
 }
 void Damage( float one ) {
 }
 void GoToSleep() {
 }
 void TurnTowards() {
  turnSpeed+=torque;
  if ( turnSpeed > turnMaxSpeed ) turnSpeed=turnMaxSpeed;
  if ( turning > 0 ) {
   if ( heading+turnSpeed <= turningTowards ) heading+=turnSpeed;
   else {
    heading=turningTowards;
    turning=0;
   }
  } else if ( turning < 0 ) {
   if ( heading-turnSpeed >= turningTowards ) heading-=turnSpeed;
   else {
    heading=turningTowards;
    turning=0;
   }
  }
 }
 string ModeToString() {
  switch ( mode ) {
 case critter_born: return string("born");
 case critter_wander: return string("wander");
 case critter_seek: return string("seek");
 case critter_wait: return string("wait");
 case critter_charge: return string("charge");
 case critter_scramble: return string("scramble");
 case critter_sleep: return string("sleep");
 case critter_flock: {
  if ( boid ) { if ( boid->alpha ) return string("flock alpha"); else return string("flock beta"); }
  else return string("flock - error");
                     }
 case critter_is_flora: return string("flora");
 default: return string("unknown");
  }
 }
 virtual void _Wander() {
  if ( uniform() < lazyness ) {
   mode=critter_wait;
   oldMode=critter_wander;
   waits=float_range(0.0f,sedentariness);
  } else {
   if ( heading > turningTowards-2.0f && heading < turningTowards+2.0f ) {
    turningTowards=float_range(0.0f,359.99999f);
    turnSpeed=0.0f;
   }
   if ( turningTowards > heading ) turning=1.0f; else turning=-1.0f;
   TurnTowards();
   Accellerate();
  }
 }
 virtual void _Seek() {
  targettingDelay-=FRAMETIME;
  if ( target && targettingDelay <= 0.0f ) {
   // Calculate the line angle to hit the target and attempt to seek.
   calculator.x=(double) map.position.x;
   calculator.y=(double) map.position.y;
   calculator.x2=target->position.x;
   calculator.y2=target->position.y;
   calculator.d=calculator.LineAngle();
   targettingDelay=0.012f;
   turningTowards=norm_deg(rad2deg((float) calculator.d));
   if ( turningTowards > heading ) turning=1.0f;
   else if ( turningTowards < heading ) turning=-1.0f;
  }
  TurnTowards();
  if ( heading > turningTowards-30.0f && heading < turningTowards+30.0f ) Accellerate();
  else SlowToStop();
  if ( destination.Distance2df(&map.position) < approach ) {
   speed=0.0f;
   waits=float_range(0.0f,sedentariness);
   mode=critter_wait;
   oldMode=critter_wander;
  }
 }
 virtual void _Wait() {
  waits-=FRAMETIME;
  SlowToStop();
  if ( waits <= 0.0f ) { // Make a decision based on previous mode, situation, etc
   randomer++;
   switch ( oldMode ) {
    case critter_wander:
     mode=critter_wander;
     if ( uniform() < aggression ) { // Pick a random location
      destination.position.x=(double)((int)map.position.x-500+randify(seed+(randomer++),1000));
      destination.position.y=(double)((int)map.position.y-500+randify(seed+(randomer++),1000));
      target=&destination;
      mode=critter_seek;
     } else {
      destination.position.x=(double)((int)map.position.x-500+randify(seed+(randomer++),1000));
      destination.position.y=(double)((int)map.position.y-500+randify(seed+(randomer++),1000));
      target=&destination;
      mode=critter_seek;
     }
    break;
    case critter_charge:
     mode=critter_charge;
    break;
   }
  }
 }
 virtual void _Charge() {
 }
 virtual void _Scramble() {
 }
 virtual void _Sleep() {
 }
 void Accellerate() {
  speed+=accelerationRate;
  if ( speed > maxSpeed ) speed=maxSpeed;
 }
 void SlowToStop() {
  speed-=accelerationRate;
  if ( speed< 0.0f ) speed=0.0f;
 }
 virtual void EachPre() {}
 virtual void EachPost() {}
 Cartesian excess;
 virtual void Between() {
  expired+=FRAMETIME;
  EachPre();
  if ( mode != critter_is_flora ) {
   if ( mode != critter_flock ) {
    switch ( mode ) {
    case critter_wander: // wander aimlessly with a chance to stop
      _Wander();
     break;
    case critter_seek: // seek a set destination
      // set the target heading
      _Seek();
     break;
    case critter_wait:
      _Wait();
     break;
    case critter_charge: // rush or flee toward/away from target
      _Charge();
     break;
    case critter_scramble: // react in surprise / fear
      _Scramble();
     break;
    case critter_sleep: // sleeping state
      _Sleep();
     break;
    }
    movedx=( speed * cosf(deg2rad(heading)) );
    movedy=( speed * sinf(deg2rad(heading)) );
    // Displacement by Tween
    if ( xMove ) {
    }
    if ( yMove ) {
    }
    // Store excess (fractional), apply movement.
    float fx=movedx+excess.fx, fy=movedy+excess.fy;
    int ix=(int) fx, iy=(int) fy;
    excess.Setf( fx - (float) ix, fy - (float) iy );
    x+=ix; x2=x+w;
    y+=iy; y2=y+h;
    // Update position
    map.position.x=(double)x;
    map.position.y=(double)y;
   } else {
    movedx=(float)x;
    movedy=(float)y;
    x=(int)map.position.x;
    y=(int)map.position.y;
    speed=idistance(movedx,movedy,(float)x,(float)y);
    movedx-=(float)x;
    movedy-=(float)y;
   }
  }
  screen.SetRect(x-w/2,y-h/2,w,h);
  Rezone();
  // Post processing.
  EachPost();
 }
 virtual void Render( GLWindow *target ) {
  if ( !screen.OnScreen() ) return;
  QuadiNoAlpha(icon,tint,transparency,screen.x,screen.y,screen.w,screen.h);
  PostProcessing();
  //Debug();
 }
 virtual void RenderRelative( GLWindow *surface, Cartesiand *offset ) {
  QuadiNoAlpha(icon,tint,transparency,translated.x,translated.y,translated.w,translated.h);
  PostProcessing();
  DebugTranslated();
 }
 virtual void Debug() {
  string m=ModeToString(); Text((char*)m.c_str(),(double)x,(double)y+15.0,8.0,10.0,true);
  if ( mode==critter_seek && target ) {
   char buf[85];
   FORMAT(buf,85,"%d,%d", (int) (this->target->position.x), (int) (this->target->position.y) );
   Text(buf,(double)x,(double)y+30.0,8.0,10.0,true);
  }
  DrawLine2Color(crayons.Pick(clear),crayons.Pick(red255),
   map.position.x,map.position.y,map.position.x+(double)(50.0f * cosf(heading)),
   map.position.y+(double)(50.0f * sinf(heading)));
  DrawLine2Color(crayons.Pick(clear),crayons.Pick(green),
   map.position.x,map.position.y,map.position.x+(double)(50.0f * cosf(heading)),
   map.position.y+(double)(50.0f * sinf(turningTowards)));
 }
 virtual void DebugTranslated() {
  string m=ModeToString(); Text((char*)m.c_str(),(double)translated.x,(double)translated.y+15.0,8.0,10.0,true);
  if ( mode==critter_seek && target ) {
   char buf[85];
   FORMAT(buf,85,"%d,%d", (int) (this->target->position.x), (int) (this->target->position.y) );
   Text(buf,(double)translated.x,(double)translated.y+30.0,8.0,10.0,true);
  }
  DrawLine2Color(
   crayons.Pick(clear),crayons.Pick(red255),
   (double)translated.x,
   (double)translated.y,
   (double)translated.x+(double)(50.0f * cosf(heading)),
   (double)translated.y+(double)(50.0f * sinf(heading))
  );
  DrawLine2Color(
   crayons.Pick(clear),crayons.Pick(green),
   (double)translated.x,
   (double)translated.y,
   (double)translated.x+(double)(50.0f * cosf(heading)),
   (double)translated.y+(double)(50.0f * sinf(turningTowards))
  );
 }
 virtual void PostProcessing() {}
 virtual bool Done() {
  if ( dead ) return true;
  if ( health <= 0.0f ) {
   OnDeath();
   dead=true;
   return true;
  }
  return false;
 }
 virtual void OnDeath() {}
 bool OnScreenTranslated() {
  return ( translated.x > -translated.w && translated.x2 < display.w+translated.w )
      && ( translated.y > -translated.h && translated.y2 < display.h+translated.h );
 }
};
*/