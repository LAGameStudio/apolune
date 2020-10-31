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

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "Strings.h"
#include "Tween.h"

class Variable : public ListItem
{
public:
 Tween *tween;
 int periodi, positioni, deltai, i, mini, maxi;
 float periodf, positionf, deltaf, f, minf, maxf;
 float perc;
 string name;
 Variable(void) { 
  name=string("");
  tween=null;
  periodi=positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=minf=maxf=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n ) {
  name=string(n);
  tween=null;
  periodi=positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=minf=maxf=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n, int mi, int ma ) {
  name=string(n);
  tween=null;
  mini=mi;
  maxi=ma;
  periodi=positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=minf=maxf=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n, int mi, int ma, Tween *t ) {
  name=string(n);
  tween=t;
  mini=mi;
  maxi=ma;
  periodi=positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=minf=maxf=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n, int mi, int ma, int hz, Tween *t ) {
  name=string(n);
  tween=t;
  mini=mi;
  maxi=ma;
  periodi=hz;
  positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=minf=maxf=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n, float mi, float ma ) {
  if ( mi > 1.0f || ma > 1.0f ) { 
   mi=mi/255.0f;
   ma=ma/255.0f;
  }
  name=string(n);
  tween=null;
  minf=mi;
  maxf=ma;
  periodi=positioni=deltai=i=mini=maxi=0;
  periodf=positionf=deltaf=f=perc=0.0f;
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 Variable( const char *n, float mi, float ma, float hz, Tween *t ) {
  if ( mi > 1.0f || ma > 1.0f ) { 
   mi=mi/255.0f;
   ma=ma/255.0f;
  }
  name=string(n);
  tween=t;
  minf=mi;
  maxf=ma;
  periodf=hz;
  periodi=positioni=deltai=i=mini=maxi=0;
  positionf=deltaf=f=minf=maxf=perc=0.0f;  
  maxi=255;
  maxf=1.0f;
  periodi=1024;
  periodf=1.0f;
 }
 ~Variable(void) {}
 Variable *Updatei( float position ) {
  if ( tween ) {
   f=tween->tweenf(position);
   i=maxi+(int) (f*(maxi-mini));
  } else {
   i=maxi;
  }
  return this;
 }
 Variable *Updatef( float position ) {
  perc=(position * periodf);// / periodf; // Note: added / periodf, was removed prior to this?
  if ( tween ) {
   f=minf+(maxf-minf)*tween->tweenf(perc);
  } else {
   f=maxf;
  }
  return this;
 }
};

class Variables : public LinkedList {
public:
 CLEARLISTRESET(Variable);
 Variable *New( const char *n ) {
  Variable *old;
  if ( !(old=Get(n)) ) {
   old = new Variable(n);
   Append(old);
   return old;
  }
  return old;
 }
 Variable *Newi( const char *n, int lo, int hi ) {
  Variable *old;
  if ( !(old=Get(n)) ) {
   old = new Variable(n,lo,hi);
   Append(old);
   return old;
  }
  old->mini=lo;
  old->maxi=hi;
  return old;
 }
 Variable *Newf( const char *n, float lo, float hi ) {
  Variable *old;
  if ( !(old=Get(n)) ) {
   old = new Variable(n,lo,hi);
   Append(old);
   return old;
  }
  old->minf=lo;
  old->maxf=hi;
  return old;
 }
 Variable *NewiT( const char *n, int lo, int hi, int dur, Tween *t )       {
  Variable *old;
  if ( !(old=Get(n)) ) {
   old = new Variable(n,lo,hi,dur,t); 
   Append(old);
   return old;
  } 
  old->mini=lo;
  old->maxi=hi;
  old->tween=t;
  old->periodi=dur; 
  return old;
 }
 Variable *NewfT( const char *n, float lo, float hi, float dur, Tween *t ) { 
  Variable *old; 
  if ( lo > 1.0f || hi > 1.0f ) { 
   lo=lo/255.0f;
   hi=hi/255.0f;
  }
  if ( !(old=Get(n)) ) {
   old = new Variable(n,lo,hi,dur,t);
   Append(old);
   return old;
  }
  old->minf=lo; 
  old->maxf=hi;
  old->tween=t;
  old->periodf=dur;
  return old;
 }
 void Set( const char *n, float fi ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->i=(int) fi;
   v->f=fi;
  }
 }
 void Seti( const char *n, int ii ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->i=ii;
  }
 }
 void Setf( const char *n, float ff ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->f=ff;
  }
 }
 void SetRange( const char *n, float lo, float hi ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->mini=(int) lo;
   v->minf=lo;
   v->maxi=(int) hi;
   v->maxf=hi;
  }
 }
 void SetRangei( const char *n, int lo, int hi ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->mini=lo;
   v->maxi=hi;
  }
 }
 void SetRangef( const char *n, float lo, float hi ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) {
   v->minf=lo;
   v->maxf=hi;
  }
 } 
 Variable *Get( const char *n ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) return v;
  return null;
 }
 int Geti( const char *n ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) return v->i;
  return -1;
 }
 float Getf( const char *n ) {
  FOREACH(Variable,v) if ( !str_cmp(v->name.c_str(),n) ) return v->f;
  return -1.234567890f;
 }
};