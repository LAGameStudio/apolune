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

#include "ZeroTypes.h"
#include "Tweens.h"

SINGLE(ComplexWave,{
 factor=8.0;
 Set("One");
 Set("Zero");
 usePhase=false; 
});
 Zp<Tween> overall,phase;
 Zbool usePhase,moduloOverall;
 Zdouble factor;
 double Compute( double a ) {
  double o=0.0;
  if ( overall && moduloOverall ) {
   o=overall->tweend(a,true);
  } else {
   o=overall->tweend(a);
  }
  if ( phase && usePhase ) {
   o+=phase->tweend(a*factor,true)/factor;
  }
  return o;
 }
 void Set( const char * s ) {
  overall=tweens.find(s);
 }
 void Set( const char * s, const char *p ) {
  overall=tweens.find(s);
  phase=tweens.find(p);
 }
PLURAL(ComplexWave,ComplexWaves,{});
ENDSET(ComplexWaves);

SINGLE(ComplexWaveStatement,{});
 ComplexWave waveform;
 double Compute( double a ) { return waveform.Compute(a); }
PLURAL(ComplexWaveStatement,ComplexWaveStatements,{});
ENDSET(ComplexWaveStatements);

SINGLE(ComplexWaveProgram,{});
 Zstring name;
 ComplexWaveStatements statements;
 Zp<ComplexWaveStatement> current;
 Zdouble expired,duration;
 double Compute(double a) { if ( current ) current->Compute(a); else return 0.0; }
 void Rewind() {
  expired=0.0;
  current=(ComplexWaveStatement *) statements.first;
 }
 void Advance() {
  if ( !current ) current=(ComplexWaveStatement *) statements.first;
  else {
   current=(ComplexWaveStatement *)current->next;
  }
 }
 void Between() {
  if ( !current ) current=(ComplexWaveStatement *) statements.first;
  expired+=FRAMETIMEd;
  if ( expired >= duration ) {
   expired=0.0;
   Advance();
  }
 }
PLURAL(ComplexWaveProgram,ComplexWavePrograms,{});
 ComplexWaveProgram *find( const char *matching ) {
  FOREACH(ComplexWaveProgram,b) if ( b->name.begins(matching) ) return b;
  return null;
 }
ENDSET(ComplexWaveProgram);
