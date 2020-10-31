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
