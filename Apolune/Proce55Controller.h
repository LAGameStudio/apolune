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
#include <Windows.h>
#include "LinkedList.h"
#include "Strings.h"
#include "Variable.h"

struct Proce55Controller
{
public: 
 Variables variables;
 int increment,integer_precision;
 float period,instant;
 bool state, pingpong, floating, hold_at_end;
 int delta;
 float deltaf;

 Proce55Controller(void) {
  increment=0;
  integer_precision=60;
  period=1.0f;
  instant=0.0f;
  delta=1;
  deltaf=1.0f/FPS;
  pingpong=floating=state=hold_at_end=false;
 }
 ~Proce55Controller(void) {
 }

 // Integers and floats
 void Updatei() {
  Framei();
  for ( Variable *v=(Variable *) (variables.first); v; v=(Variable *) (v->next) ) v->Updatei(instant/period);
 }
 void Updatef() {
  Framef();
  for ( Variable *v=(Variable *) (variables.first); v; v=(Variable *) (v->next) ) v->Updatef(instant/period);
 }
 void Update() {
  Framei();
  Updatei();
  Framef();
  Updatef();
 }

private:
 void Framei() {
  increment+=delta;
  if ( increment >= integer_precision ) {
   if ( hold_at_end ) increment=integer_precision-1;
   else if (pingpong ) {
    increment=integer_precision-1;
    delta=-delta;
   } else increment=0;
  }
  else if ( increment < 0 ) {
   if (pingpong ) {
    increment=1;
    delta=-delta;
   } else increment=integer_precision;
  }
 }
 void Framef() {
  instant+= deltaf; 
  if ( instant > period  ) {
   if (pingpong ) {
    instant=period-1.0f/FPS;
    deltaf=-deltaf;
   } else instant=0.0f;
  }
  else if ( instant < 0 )  {
   if (pingpong ) {
    instant=(float) abs(delta);
    deltaf=-deltaf;
   } else instant=period;
  }
 }
};

struct GlobalProce55Calculator : public ListItem {
public:
 virtual void Calculate( float *factor ) {}
};

struct Floating1GlobalProce55Calculator : public GlobalProce55Calculator {
public:
 float max;
 float *f;
 Floating1GlobalProce55Calculator() : GlobalProce55Calculator() { max=1.0f; f=null; }
 Floating1GlobalProce55Calculator( float *in, float m ) : GlobalProce55Calculator() { max=m; f=in; }
 void Calculate( float *time ) { *f=*time/max; }
};

struct FloatingGlobalProce55Calculator : public GlobalProce55Calculator {
public:
 float scale;
 float low, magnitude;
 float *f;
 FloatingGlobalProce55Calculator() : GlobalProce55Calculator() { scale=1.0f; low=magnitude=0.0f; f=null; }
 FloatingGlobalProce55Calculator( float *in, float l, float h ) : GlobalProce55Calculator() { low=l; magnitude=h-l; scale=1.0f; f=in; }
 FloatingGlobalProce55Calculator( float *in, float l, float h, float s ) : GlobalProce55Calculator() { low=l; magnitude=h-l; scale=s; f=in; }
 void RangeAndScale( float l, float h, float s ) { Range(l,h); scale=s; }
 void Range( float l, float h ) { low=l; magnitude=h-l; }
 virtual void Calculate( float *time ) { *f=(*time * magnitude + low)*scale; }
};

struct IntegerGlobalProce55Calculator : public GlobalProce55Calculator {
public:
 int low, magnitude;
 int *i;
 IntegerGlobalProce55Calculator() : GlobalProce55Calculator() { low=magnitude=0; i=null; }
 IntegerGlobalProce55Calculator( int *in, int l, int h ) : GlobalProce55Calculator() { low=l; magnitude=h-l; i=in; }
 void Range( int l, int h ) { low=l; magnitude=h-l; }
 virtual void Calculate( float *time ) { *i=(int) ( (*time) * (float) magnitude ); *i+=low; }
};

struct TweenfGlobalProce55Calculator : public FloatingGlobalProce55Calculator {
public:
};

struct TweenifGlobalProce55Calculator : public IntegerGlobalProce55Calculator {
public:
};

struct GlobalProce55Calculators : public LinkedList {
public:
 void Calculate( float *time ) {
  EACH(first,GlobalProce55Calculator,g) g->Calculate(time);
 }
 CLEARLISTRESET(GlobalProce55Calculator);
 ~GlobalProce55Calculators() { Clear(); }
};

struct SimpleGlobalProce55Controller : public ListItem {
public:
 GlobalProce55Calculators calculators;
 virtual void Update() {}
};

struct OneWayLoopingGlobalProce55Controller : public SimpleGlobalProce55Controller {
public:
 float expired,duration,direction;
 OneWayLoopingGlobalProce55Controller() : SimpleGlobalProce55Controller() { expired=0.0f; duration=1.0f; direction=FPS; }
 void Update() {
  calculators.Calculate(&expired);
  expired+=direction;
  if ( expired > duration ) expired-=duration;
 }
};

struct OscillatingGlobalProce55Controller : public SimpleGlobalProce55Controller {
public:
 float expired,duration,direction;
 OscillatingGlobalProce55Controller() : SimpleGlobalProce55Controller() { expired=0.0f; duration=1.0f; direction=FPS; }
 void Update() {
  calculators.Calculate(&expired);
  expired+=direction;
  if ( expired > duration ) {
   expired=duration-(duration-expired);
   direction=-direction;
  } else
  if ( expired < 0 ) {
   expired=expired+duration;
   direction=-direction;
  }
 }
};

/*class LoopableGlobalProce55Controller : public SimpleGlobalProce55Controller {
public:
 float expired,duration,direction;
 int loop,loops;
 LoopableGlobalProce55Controller() : SimpleGlobalProce55Controller() { expired=0.0f; duration=1.0f; direction=FPS; }
 void Update() {
  calculators.Calculate(&expired);
  expired+=direction;
  if ( loops !=0 ) {
   if ( loop<loops ) {
   }
  } 
 }
};*/

struct GlobalProce55Controller : public SimpleGlobalProce55Controller {
public:
 GlobalProce55Calculators calculators;
 float expired,duration,direction;
 int loop,loops;
 GlobalProce55Controller() : SimpleGlobalProce55Controller() {
  expired=0.0f;
  duration=1.0f;
  direction=FPS;
  loop=0;
  loops=0;
 }
 void Update() {
  calculators.Calculate(&expired);
  expired+=direction;
  if ( loops !=0 ) {
   if ( loop<loops ) {
   }
  } 
 }
};

// One per application/thread or threaded.  Not thread safe. (add?)
class GlobalProce55ControllerQueue : public LinkedList {
public:
 void Update() { EACH(first,SimpleGlobalProce55Controller,s) s->Update(); }
 CLEARLISTRESET(SimpleGlobalProce55Controller);
 ~GlobalProce55ControllerQueue() { Clear(); }
};