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
/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "Strings.h"

ONE(Stat,{ allowBeyondRange=true; value="0"; maxDecimal=100.0; maxInteger=100; });
 Zstring description;
 Zbool allowBeyondRange;
 Zstring value;
 Zdouble decimal,minDecimal,maxDecimal;
 Zint integer,minInteger,maxInteger;
 Zdouble lifespan,t;
 KEYWORDS({
  TXTWORD("name",name)
  else TXTWORD("desc",description)
  else KEYTAG("allow",{allowBeyondRange=true;})
  else KEYWORD("v",{value=w;})
  else KEYWORD("d",{decimal=w;})
  else KEYWORD("minD",{minDecimal=w;})
  else KEYWORD("maxD",{maxDecimal=w;})
  else KEYWORD("i",{integer=w;})
  else KEYWORD("minI",{minInteger=w;})
  else KEYWORD("maxI",{maxInteger=w;})
  else KEYWORD("t",{t=w;})
  else KEYWORD("l",{lifespan=w;})
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("desc",description)
  TAGKEY("allow",allowBeyondRange)
  WORDKEY("v",value)
  WORDKEY("d",decimal.toString())
  WORDKEY("minD",minDecimal.toString())
  WORDKEY("maxD",maxDecimal.toString())
  WORDKEY("i",integer.toString())
  WORDKEY("minI",minInteger.toString())
  WORDKEY("maxI",maxInteger.toString())
  NUMBKEY("t",t);
  NUMBKEY("l",lifespan);
 })
 void Set( const char *stringValue ) { Set(value.decimal()); value=stringValue; }
 void Set( float f ) { Set((double)f); }
 void Set( double d ) { value=D2S(d); decimal=d; integer=(int)d; Lower(0.0); Raise(0.0); }
 void Set( int i ) { value=I2S(i); integer=i; decimal=(double)i; Lower(0); Raise(0); }
 void Lower( float f ) { Lower((double)f); }
 void Lower( double d ) { decimal-=d; if ( decimal < minDecimal ) decimal=minDecimal; }
 void Lower( int i ) { integer+=i; if ( integer < minInteger ) integer=minInteger; }
 void Raise( float f ) { Raise((double)f); }
 void Raise( double d ) { decimal+=d; if ( decimal > maxDecimal ) decimal=maxDecimal; }
 void Raise( int i ) { integer+=i; if ( integer > maxInteger ) integer=maxInteger; }
 void Limits( float minf, float maxf ) { Limits((double)minf,(double)maxf); }
 void Limits( double mind, double maxd ) { minDecimal=mind; maxDecimal=maxd; }
 void Limits( int mini, int maxi ) { minInteger=mini; maxInteger=maxi; }
 double Percentd() { return Ratiod() * 100.0; }
 double Percenti() { return Ratioi() * 100.0; }
 double Ratioi() { if ( maxInteger == 0 ) return iratiod(integer,100); return iratiod(integer-minInteger,maxInteger-minInteger); }
 double Ratiod() { if ( maxDecimal == 0.0 ) return decimal/1.0; return (decimal-minDecimal)/(maxDecimal-minDecimal); }
 int Integer() { return integer; }
 double Decimal() { return decimal; }
 CLONE(Stat,{
  DUPE(name)
  DUPE(description)
  DUPE(allowBeyondRange)
  DUPE(value)
  DUPE(decimal)
  DUPE(minDecimal)
  DUPE(maxDecimal)
  DUPE(integer)
  DUPE(minInteger)
  DUPE(maxInteger)
  DUPE(t)
  DUPE(lifespan)
 })
MANY(Stat,StatHandle,StatHandles,"stat",Stats,{})
 CLONES(Stat,Stats)
 KEYSWORDSGroups("stat",Stat)
 void Bonus( const char *name, double value, double duration, const char *reason="" ) {
  Stat *s=new Stat;
  s->Set(value);
  s->t=duration;
  s->lifespan=duration;
  s->description=reason;
  Append(s);
 }
 void Between() { // Used when Stats is a bonuses to deteriorate bonuses
  FOREACHN(Stat,bonus,{
    if ( bonus->lifespan > 0.0 ) {
    bonus->t-=FRAMETIME;
    if ( bonus->t <= 0.0 ) {
     Remove(bonus);
     delete bonus;
    }
   }
  });
 }
 void Describe( const char * name, int mini, int maxi, int defaultInteger, const char *desc="" ) {
  if ( this->named(name) ) return;
  Stat *s=new Stat;
  s->name=name;
  s->Limits(mini,maxi);
  s->Set(defaultInteger);
  s->description=desc;
  Append(s);
 }
 void Describe( const char * name, float minf, float maxf, float defaultFloat, const char *desc="" ) {
  Describe(name,(double)minf,(double)maxf,(double)defaultFloat,desc);
 }
 void Describe( const char * name, double mind, double maxd, double defaultDouble, const char *desc="" ) {
  if ( this->named(name) ) return;
  Stat *s=new Stat;
  s->name=name;
  s->Limits(mind,maxd);
  s->Set(defaultDouble);
  s->description=desc;
  Append(s);
 }
 void Describe( const char * name, const char *defaultString, const char *desc="" ) {
  if ( this->named(name) ) return;
  Stat *s=new Stat;
  s->name=name;
  s->Set(defaultString);
  s->description=desc;
  Append(s);
 }
 void Set( const char *statname, int value ) {
  Stat *s=named(statname);
  if ( s ) s->Set(value);
  else { s=new Stat; s->name=statname; s->Set(value); Append(s); }
 }
 bool Check( int against, const char *statname, Stats *bonuses=null ) {
  double value=Get(statname,bonuses);
  return ( against > (int) value );
 }
 double Get( const char *statname, Stats *bonuses=null ) {
  if ( !bonuses ) {
   Stat *s=named(statname);
   return s ? s->Decimal() : 0.0;
  } else {
   Stat *s=named(statname);
   double value=s ? s->Decimal() : 0.0;
   EACH(bonuses->first,Stat,b) if ( !str_cmp(b->name.c_str(),statname) ) value+=b->Decimal();
   return value;
  }
 }
DONE(Stat);

ONE(StatRange,{});
 Zstring description;
 Zint lowInteger,highInteger;
 Zdouble lowDecimal,highDecimal;
 bool within( double value ) { return lowDecimal <= value && highDecimal >= value; }
 bool within( int value ) { return lowInteger <= value && highInteger >= value; }
MANY(StatRange,StatRangeHandle,StatRangeHandles,"statrange",StatRanges,{});
 void Describe( const char *name, const char *description, int low, int high ) {
  StatRange *sr=new StatRange(name);
  sr->description=description;
  sr->lowInteger=low;
  sr->highInteger=high;
  sr->lowDecimal=(double)low;
  sr->highDecimal=(double)high;
  Append(sr);
 }
 void Describe( const char *name, const char *description, double low, double high ) {
  StatRange *sr=new StatRange(name);
  sr->description=description;
  sr->lowDecimal=low;
  sr->highDecimal=high;
  sr->lowInteger=(int)low;
  sr->highInteger=(int)high;
  Append(sr);
 }
 void Describe( const char *name, const char *description, float low, float high ) {
  Describe(name,description,(double)low,(double)high);
 }
 void within( StatRangeHandles *out, int value ) {
  FOREACH(StatRange,sr) if ( sr->within(value) ) out->Add(sr);
 }
 void within( StatRangeHandles *out, double value ) {
  FOREACH(StatRange,sr) if ( sr->within(value) ) out->Add(sr);
 }
 void within( StatRangeHandles *out, float value ) {
  within(out,(double)value);
 }
DONE(StatRange);

extern Strings master_skill_list;

class Skills;
extern Skills master_skill_tree;

ONE(Skill,{})
 Zdouble amount; // percentage
 Strings prerequisites;
 bool Check( double modifier ) { return ( uniform()*100.0 > modifier+amount ); }
 KEYWORDS({
  NUMWORD("amount",amount)
  else TXTWORD("name",name)
  else KEYWORD("req",{prerequisites.fromStringList(w);})
 })
 WORDKEYS({
  TXTKEY("name",name)
  NUMBKEY("amount",amount)
  if ( prerequisites.count > 0 ) WORDKEY("req",prerequisites.toStringList())
 })
MANY(Skill,SkillHandle,SkillHandles,"Skill",Skills,{})
 Skill *InList( const char *skname ) {
  FOREACH(Skill,sk) if ( !str_cmp(skname,sk->name.c_str()) ) return sk;
  return null;
 }
 // For the master tree, and the "prereq" command
 void AddPrereq( const char *skname, const char *prereq="" ) {
  bool havingPrereq=strlen(prereq)>0;
  if ( havingPrereq ) master_skill_list.AddIfUnique(prereq);
  master_skill_list.AddIfUnique(skname);
  if ( havingPrereq ) Skill *pr=master_skill_tree.Set(prereq,100);
  Skill *sk=master_skill_tree.Set(skname,100);
  if ( havingPrereq ) sk->prerequisites.AddIfUnique(prereq);
 }
 bool HasPrereqs( const char *skname ) {
  Skill *sk=InList(skname);
  if ( !sk ) return true;
  if ( sk->prerequisites.count <= 0 ) return true;
  EACH(sk->prerequisites.first,String,pr) if ( !InList(pr->s.c_str()) ) return false;
  return true;
 }
 void GetPrereqs( SkillHandles *out, const char *skname ) {
  Skill *sk=InList(skname);
  if ( !sk ) return;
  EACH(sk->prerequisites.first,String,p) {
   Skill *pr=InList(p->s.c_str());
   if ( pr ) out->Add(pr);
  }
 }
 Skill *Teach( const char *skname, int by=1 ) {
  master_skill_list.AddIfUnique(skname);
  Skill *s=named(skname);
  if ( !s ) {
   s=new Skill;
   s->name=skname;
   s->amount=(double)by;
   Append(s);
  } else {
   s->amount+=(double)by;
  }
  return s;
 }
 Skill *Set( const char *skname, int by ) {
  master_skill_list.AddIfUnique(skname);
  Skill *s=named(skname);
  if ( !s ) {
   s=new Skill;
   s->name=skname;
   s->amount=(double)by;
   Append(s);
  } else {
   s->amount=(double)by;
  }
  return s;
 }
 Skill *Set( const char *skname, int by, const char *prereqs ) {
  master_skill_list.AddIfUnique(skname);
  Skill *s=named(skname);
  if ( !s ) {
   s=new Skill;
   s->name=skname;
   s->amount=(double)by;
   s->prerequisites.AddIfUnique(prereqs);
   Append(s);
  } else {
   s->amount=(double)by;
  }
  return s;
 }
 bool Check( const char *skname, int against ) {
  Skill *s=named(skname);
  if ( !s ) return false;
  return against < (int) s->amount;
 }
 KEYWORDSGroup("Skill",Skill);
 WORDKEYSGroup("Skill",Skill);
DONE(Skill);

class Campaign;
ONE(SkillRequirement,{})
 Zp<Campaign> campaign;
 Zdouble minimum;
 bool Meets( double amount ) { return amount >= minimum; }
 KEYWORDS({
  KEYWORD("minimum",{minimum=w;})
  else TXTWORD("name",name)
 })
 WORDKEYS({
  WORDKEY("minimum",minimum.toString())
  TXTKEY("name",name)
 })
MANY(SkillRequirement,SkillRequirementHandle,SkillRequirementHandles,"SkillRequirement",SkillRequirements,{})
// bool Meets( SkillHandles *...
 void AssignUnassigned( Campaign *c );
 KEYWORDSGroup("SkillRequirement",SkillRequirement);
 WORDKEYSGroup("SkillRequirement",SkillRequirement);
DONE(SkillRequirement);