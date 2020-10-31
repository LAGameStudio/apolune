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
#include "macros.h"
#include "LinkedList.h"

#define INPUT_DELAY_SHORT  0.2f
#define INPUT_DELAY_MEDIUM 0.5f
#define INPUT_DELAY_LONG   1.0f

/* Simple temporal delay class. */

class Delay {
public:
 float reset;
 float instant;
 Delay( float magnitude, float delay ) {
  instant=magnitude;
  this->reset=delay;
 }
 Delay( float magnitude ) {
  instant=0.0f;
  reset=magnitude;
 }
 Delay() {
  instant=0.0f;
  reset=1.0f;
 }
 void Between() { if ( instant > 0.0f ) instant-=FRAMETIME; }
 void Set( float f ) { reset=f; }
 void Set( double d ) { reset=(float)d; }
 void Set()     { instant=reset; }
 void Reset()   { instant=reset; }
 void Deplete() { instant=0.0f;  }
 bool Running() { return instant > 0.0f; }
 float Ratio() { return instant / reset; }
 Delay& operator= ( const Delay& D ) { instant=D.instant; reset=D.reset; return *this; }
 Delay& operator= ( const int i    ) { instant=(float)i; return *this; }
 Delay& operator= ( const float f  ) { instant=f;        return *this; }
 Delay& operator= ( const double d ) { instant=(float)d; return *this; }
 Delay& operator+= ( const int i )    { instant+=(float)i; return *this; }
 Delay& operator+= ( const float f )  { instant+=f;        return *this; }
 Delay& operator+= ( const double d ) { instant+=(float)d; return *this; }
 operator bool()   { return ( instant > 0.0f );  }
 bool operator()() { return ( instant > 0.0f );  }
 bool operator !() { return ( instant <= 0.0f ); }
 KEYWORDS({
  KEYWORD("reset",{reset=(float)atof(w);})
  else KEYWORD("instant",{instant=(float)atof(w);})
 })
 WORDKEYS({
  WORDKEY("reset",FORMAT("%1.4f",reset))
  WORDKEY("instant",FORMAT("%1.4f",instant))
 })
};

ONE(DelayString,{})
 Delay delay;
 KEYWORDS({
  KEYWORD("delay",{delay.fromString(w);})
 })
 WORDKEYS({
  WORDKEY("delay",delay.toString())
 })
MANY(DelayString,DelayStringHandle,DelayStringHandles,"DelayString",DelayStrings,{})
 Zp<DelayString> current;
 void Advance() {
  if ( !current ) current=(DelayString *)first;
  else current=(DelayString *)current->next;
  if ( !current ) current=(DelayString *)first;
  if ( current ) {
   current->delay.Reset();
  }
 }
DONE(DelayString);
