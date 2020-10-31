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
#include "LinkedList.h"
#include "Numbers.h"
#include "Strings.h"

class Statistic : public ListItem {
public:
 Zint unique; // for fast lookups, must be unique, can be managed
 Zstring name;
 Zstring units;
 Zdouble quantity;
 Zbool integer;
 Zint averaged;
 Statistic( int unique, string name, string units, double quantity, bool asInteger ) {
  this->unique=unique;
  this->name=name;
  this->units=units;
  this->quantity=quantity;
  this->integer=asInteger;
 }
 string asUnits() {
  return name.value
   + (integer ? FORMAT(buf,128,"%d",(int)quantity) : FORMAT(buf,128,"%1.2f", (double) quantity) )
   + string(" ") + units.value;
 }
 string asString() {
  return name.value
   + (integer ? FORMAT(buf,128,"%d",(int)quantity) : FORMAT(buf,128,"%1.2f", (double) quantity) )
   + string(" ") + units.value;
 }
 void Set( int i ) { quantity=(double)i; }
 void Set( float f ) { quantity=(double)f; }
 void Set( double d ) { quantity=d; }
 void Add( int i ) { quantity+=(double)i; }
 void Add( float f ) { quantity+=(double)f; }
 void Add( double d ) { quantity+=d; }
 void Multiply( int i ) { quantity*=(double)i; }
 void Multiply( float f ) { quantity*=(double)f; }
 void Multiply( double d ) { quantity*=d; }
 void Average( int i ) { quantity=(quantity*(double)averaged+(double)i)/(double)(averaged+1); averaged++; }
 void Average( float f ) { quantity=(quantity*(double)averaged+(double)f)/(double)(averaged+1); averaged++; }
 void Average( double d ) { quantity=(quantity*(double)averaged+d)/(double)(averaged+1); averaged++; }
 void AverageWith( int i ) { quantity= (quantity+(double)i)/2.0; }
 void AverageWith( float f ) { quantity= (quantity+(double)f)/2.0; }
 void AverageWith( double d ) { quantity= (quantity+d)/2.0; }
};

class Statistics : public LinkedList {
public:
 CLEARLISTRESET(Statistic);
 int GetUniqueID() {
  int i=0;
  bool found=true;
  do {
   i++;
   found=false;
   FOREACH(Statistic,s) if ( s->unique == i ) { found=true; break; }
  } while ( found );
  return i;
 }
 Statistic *find( string match ) {
  FOREACH(Statistic,s) if ( s->name == match ) return s;
  OUTPUT("Statistics:find(`%s`) did not match\n", match.c_str() );
  return null;
 }
 Statistic *find( int match ) {
  FOREACH(Statistic,s) if ( s->unique == match ) return s;
  OUTPUT("Statistics:find(`%d`) did not match\n", match );
  return null;
 }
 double Tally( Strings *names ) {
  double total=0.0;
  EACH(names->first,String,s) {
   Statistic *stat=find(s->s);
   if ( !stat ) {
    OUTPUT("Statistics:Tally ignored missing value for `%s`\n", s->s.c_str() );
    continue;
   }
   total+=stat->quantity;
  }
  return total;
 }
 double Tally( Integers *uniques ) {
  double total=0.0;
  EACH(uniques->first,Integer,i) {
   Statistic *stat=find((int)i->i);
   if ( !stat ) {
    OUTPUT("Statistics:Tally ignored missing value for `%d`\n", (int)i->i);
    continue;
   }
   total+=stat->quantity;
  }
  return total;
 }
 double Average( Strings *names ) {
  double total=0.0;
  int tallied=0;
  EACH(names->first,String,s) {
   Statistic *stat=find(s->s);
   if ( !stat ) {
    OUTPUT("Statistics:Average ignored missing value for `%s`\n", s->s.c_str() );
    continue;
   }
   total+=stat->quantity;
   tallied++;
  }
  return total/(double)tallied;
 }
 double Average( Integers *uniques ) {
  double total=0.0;
  int tallied=0;
  EACH(uniques->first,Integer,i) {
   Statistic *stat=find((int)i->i);
   if ( !stat ) {
    OUTPUT("Statistics:Average ignored missing value for `%d`\n", (int)i->i );
    continue;
   }
   total+=stat->quantity;
   tallied++;
  }
  return total/(double)tallied;
 }
 Statistic *New( int unique, string name, string units, double quantity, bool asInteger=false ) {
  Statistic *s=new Statistic(unique,name,units,quantity,asInteger);
  Append(s);
  return s;
 }
 Statistic *New( string name, string units, double quantity, bool asInteger=false ) {
  Statistic *s=new Statistic(GetUniqueID(),name,units,quantity,asInteger);
  Append(s);
  return s;
 }
 void Set( int match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(%d) found no match\n", match);
   return;
  }
  s->Set(i);
 }
 void Set( int match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(%d) found no match\n", match);
   return;
  }
  s->Set(f);
 }
 void Set( int match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(%d) found no match\n", match);
   return;
  }
  s->Set(d);
 }
 void Set( string match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Set(i);
 }
 void Set( string match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Set(f);
 }
 void Set( string match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Set(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Set(d);
 }
 void Add( int match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(%d) found no match\n", match);
   return;
  }
  s->Add(i);
 }
 void Add( int match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(%d) found no match\n", match);
   return;
  }
  s->Add(f);
 }
 void Add( int match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(%d) found no match\n", match);
   return;
  }
  s->Add(d);
 }
 void Add( string match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Add(i);
 }
 void Add( string match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Add(f);
 }
 void Add( string match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Add(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Add(d);
 }
 void Multiply( int match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(%d) found no match\n", match);
   return;
  }
  s->Multiply(i);
 }
 void Multiply( int match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(%d) found no match\n", match);
   return;
  }
  s->Multiply(f);
 }
 void Multiply( int match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(%d) found no match\n", match);
   return;
  }
  s->Multiply(d);
 }
 void Multiply( string match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Multiply(i);
 }
 void Multiply( string match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Multiply(f);
 }
 void Multiply( string match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Multiply(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Multiply(d);
 }
 void Average( int match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(%d) found no match\n", match);
   return;
  }
  s->Average(i);
 }
 void Average( int match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(%d) found no match\n", match);
   return;
  }
  s->Average(f);
 }
 void Average( int match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(%d) found no match\n", match);
   return;
  }
  s->Average(d);
 }
 void Average( string match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Average(i);
 }
 void Average( string match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Average(f);
 }
 void Average( string match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:Average(`%s`) found no match\n", match.c_str());
   return;
  }
  s->Average(d);
 }
 void AverageWith( int match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(%d) found no match\n", match);
   return;
  }
  s->AverageWith(i);
 }
 void AverageWith( int match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(%d) found no match\n", match);
   return;
  }
  s->AverageWith(f);
 }
 void AverageWith( int match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(%d) found no match\n", match);
   return;
  }
  s->AverageWith(d);
 }
 void AverageWith( string match, int i ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(`%s`) found no match\n", match.c_str());
   return;
  }
  s->AverageWith(i);
 }
 void AverageWith( string match, float f ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(`%s`) found no match\n", match.c_str());
   return;
  }
  s->AverageWith(f);
 }
 void AverageWith( string match, double d ) {
  Statistic *s=find(match);
  if ( !s ) {
   OUTPUT("Statistics:AverageWith(`%s`) found no match\n", match.c_str());
   return;
  }
  s->AverageWith(d);
 }
};

class Total : public ListItem {
public:
 Zstring name;
 Strings names;
 Integers uniques;
 void Update( Statistics *stats ) {
 }

};

class Totals : public LinkedList {
public:
 CLEARLISTRESET(Total);
 void Update( Statistics *stats ) {
  FOREACH(Total,t) t->Update(stats);
 }
};