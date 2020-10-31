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
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
using namespace std;
#include "macros.h"
#include "moremath.h"
#include "LinkedList.h"
#include "Strings.h"
#include "BinaryFile.h"

class Integer : public ListItem {
public:
 string name;
 Zint i;
 void BinaryWrite( BinaryFile *open ) {
  open->scrawl(name);
  open->scrawl(i.value);
 }
 void BinaryRead( BinaryFile *open ) {
  open->read(&name);
  open->read(&i.value);
 }
 Integer() : ListItem() {
  name=string("");
  next=prev=null;
  i=0;
 }
 Integer( int j ) : ListItem() {
  name=string("");
  next=prev=null;
  i=j;
 }
 string toStringKV() {
  char buf[255+4+128];
  return FORMAT( buf, 255+4+128, "\"%s\"=%d", name.c_str(), (int) i );
 }
 string toString() {
  char buf[255+4+128];
  return FORMAT( buf, 255+4+128, "%d", (int) i );
 }
 int fromNumericString( const char *s ) {
  return (i=atoi(s));
 }
 void fromString( const char *s ) {
  Zstring w=s;
  const char *p=w.next();
  int i=0;
  while ( *p == '\0' ) {
   if ( i == 1 ) i=atoi(p);
   else if ( i == 0 ) name=string(s);
   p=w.next();
   i++;
  }
 }
 void Assign( const char *s, int j ) {
  Name(s);
  Set(j);
 }
 void Name( const char *s ) { name=string(s); }
 void Set( const char *s )  { Set( atoi(s) ); }
 void Set( int j )      { i=j; }
 void Add( int j )      { Set(i+j); }
 void Multiply( int j ) { Set(i*j); }
 void Divide( int j )   { Set(i/j); }
 void Modulo( int j )   { Set(i%j); }
 float Sqrt( int j ) {
  float f;
  Set( (int) (f=isqrt(j)) );
  return f;
 }
 ~Integer() {}
};

class Integers : public LinkedList {
public:
 Integer *GetElement(int byIndex) { return (Integer *)Element(byIndex%count); }
 int Get(int byIndex) { Integer *i = (Integer *)Element(byIndex%count.value); return i ? i->i : -1; }
 int Random() { return Get((int)GetTickCount()); }
 void ReadAppend( BinaryFile *open ) {
  Integer *n=new Integer;
  n->BinaryRead(open);
  Append(n);
 }
 string toKVString() {
  string r=string("");
  EACH(first,Integer,i) r+=string("[")+i->toStringKV()+string("] ");
  return r;
 }
 string toString() {
  string r=string("");
  EACH(first,Integer,i) r+=i->toString()+string(" ");
  return r;
 }
 void fromString( const char *s ) {
  Zstring zs=s;
  const char *p=zs.next();
  while ( *p != '\0' ) {
   Integer *i=new Integer;
   i->fromNumericString(p);
   Append(i);
   p=zs.next();
  }
 }
 void Duplicate( Integers *i ) {
  EACH(i->first,Integer,j) {
   Integer *k=new Integer(j->i.value);
   Append(k);
  }
 }
 Integer *find( int j ) {
  FOREACH(Integer,n) if ( n->i==j ) return n;
  return null;
 }
 Integer *find( const char *s ) {
  FOREACH(Integer,n) if ( !str_cmp( s, n->name.c_str() ) ) return n;
  return null;
 }
 Integer *find_prefix( const char *s ) {
  FOREACH(Integer,n) if ( !str_prefix( s, n->name.c_str() ) ) return n;
  return null;
 }
 void Add( int i ) { Append(new Integer(i)); } 
 void Assign( const char *s, int i )   {
  Integer *n=find(s);
  if ( !n ) {
   n=new Integer;
   n->Name(s);
   Append(n);
  }
  n->Set(i);
 }
 void Assign( const char *s, const char *v ) {
  Integer *n=find(s);
  if ( !n ) {
   n=new Integer;
   n->Name(s);
   Append(n);
  }
  n->Set(v);
 }
 SORTING(Integer,SortAscending,{},{ if ( A->i < B->i ) result=-1; },{})
 Integer *Sum() {
  Integer *sum=new Integer;
  FOREACH(Integer,factor) sum->i+=factor->i;
  return sum;
 }
 Integer *Average() {
  Integer *avg=new Integer;
  if ( count == 0 ) return avg;
  FOREACH(Integer,factor) avg->i+=factor->i; 
  avg->i/=count;
  return avg;
 }
 CLEARLISTRESET(Integer);
};

class Float : public ListItem {
public:
 Zstring name;
 Zfloat f;
 void BinaryRead( BinaryFile *open ) {
  open->read(&name);
  open->read(&f.value);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->scrawl(name);
  open->scrawl(f.value);
 }
 Float( float g ) : ListItem() {
  f=g;
 }
 Float() : ListItem() {}
 void Assign( const char *s, float g ) { Name(s); Set(g); }
 void Name( const char *s )            { name=string(s); }
 void Set( float g )       { f=g; }
 void Set( const char *s ) { Set( (float) atof(s) ); }
 void Add( float g )       { Set(f+g); }
 void Multiply( float g )  { Set(f*g); }
 void Divide( float g )    { Set(f/g); }
 void Sqrt( float g )      { Set(sqrt(g)); }
};

class Floats : public LinkedList {
public: 
 void ReadAppend( BinaryFile *open ) {
  Float *n=new Float;
  n->BinaryRead(open);
  Append(n);
 }
 Float *find( float g ) {
  FOREACH(Float,n) if ( n->f==g ) return n;
  return null;
 }
 Float *find( const char *s ) {
  FOREACH(Float,n) if ( !str_cmp( s, n->name.c_str() ) ) return n;
  return null;
 }
 Float *find( string s ) {
  FOREACH(Float,n) if ( n->name == s ) return n;
  return null;
 }
 Float *find_prefix( const char *s ) {
  FOREACH(Float,n) if ( !str_prefix( s, n->name.c_str() ) ) return n;
  return null;
 }
 float Value( int i ) {
  Float *f=(Float *) Element(i);
  if ( !f ) {
   OUTPUT("Floats:Value() warns no element %d in list, returning 0.0f\n",
    (int) i);
   return 0.0f;
  }
  return f->f;
 }
 void Add( float g ) { Append( new Float(g) ); }
 void Assign( const char *s, float f )  {
  Float *n=find(s);
  if ( !n ) {
   n=new Float;
   n->Name(s);
   Append(n);
  }
  n->Set(f);
 }
 void Assign( const char *s, const char *v )  {
  Float *n=find(s);
  if ( !n ) {
   n=new Float;
   n->Name(s);
   Append(n);
  }
  n->Set(v);
 }
 SORTING(Float,SortAscending,{},{ if ( A->f < B->f ) result=-1; },{})
 Float *Sum() {
  Float *sum=new Float;
  FOREACH(Float,factor) sum->f+=factor->f;
  return sum;
 }
 Float *Average() {
  Float *avg=new Float;
  if ( count == 0 ) return avg;
  FOREACH(Float,factor) avg->f+=factor->f; 
  avg->f/=count;
  return avg;
 }
 CLEARLISTRESET(Float);
};

ONE(Double,{})
 Zdouble d;
 KEYWORDS({  TXTWORD("n",name)  NUMWORD("d",d) })
 WORDKEYS({  TXTKEY("n",name)   NUMBKEY("d",d) })
 BINARY_IN({  B_IN_STRING(name);  B_IN_NUM(d); })
 BINARY_OUT({ B_OUT_STRING(name); B_OUT_NUM(d);})
 CLONE(Double,{
  DUPE(name)
  DUPE(d)
 })
 Double( double e ) : ListItem() {
  name=string("");
  d=e;
 }
 void Assign( const char *s, double e ) { Name(s); Set(e); }
 void Name( const char *s )             { name=string(s); }
 void Set( double e )      { d=e; }
 void Set( const char *s ) { Set( (float) atof(s) ); }
 void Add( double e )      { Set(d+e); }
 void Multiply( double e ) { Set(d*e); }
 void Divide( double e )   { Set(d/e); }
 void Sqrt( double e )     { Set(sqrt(e)); }
MANY(Double,DoubleHandle,DoubleHandles,"double",Doubles,{})
 KEYSWORDSGroups("d",Double);
 BINARY_INOUT(Double);
 CLONES(Double,Doubles);
 void ReadAppend( BinaryFile *open ) {
  Double *n=new Double;
  n->BinaryRead(open);
  Append(n);
 }
 double Get( int index ) {
  Double *d=(Double *)Element(index%count);
  return d?d->d:-NOOPd;
 }
 Double *find( double e ) {
  FOREACH(Double,n) if ( n->d == e ) return n;
  return null;
 }
 Double *find( const char *s ) {
  FOREACH(Double,n) if ( !str_cmp( s, n->name.c_str() ) ) return n;
  return null;
 }
 Double *find_prefix( const char *s ) {
  FOREACH(Double,n) if ( !str_prefix( s, n->name.c_str() ) ) return n;
  return null;
 }
 void Add( double e ) { Append( new Double(e) ); }
 void Assign( const char *s, double e ) {
  Double *n=find(s);
  if ( !n ) {
   n=new Double;
   n->Name(s);
   Append(n);
  }
  n->Set(e);
 }
 void Assign( const char *s, const char *v ) {
  Double *n=find(s);
  if ( !n ) {
   n=new Double;
   n->Name(s);
   Append(n);
  }
  n->Set(v);
 }
 SORTING(Double,SortAscending,{},{ if ( A->d < B->d ) result=-1; },{})
 double sum() {
  double sum=0.0;
  FOREACH(Double,factor) sum+=factor->d;
  return sum;
 }
 Double *Sum() {
  Double *sum=new Double;
  FOREACH(Double,factor) sum->d+=factor->d;
  return sum;
 }
 double average() {
  double avg=0.0;
  if ( count == 0 ) return avg;
  FOREACH(Double,factor) avg+=factor->d; 
  avg/=(double)count;
  return avg;
 }
 Double *Average() {
  Double *avg=new Double;
  if ( count == 0 ) return avg;
  FOREACH(Double,factor) avg->d+=factor->d; 
  avg->d/=count;
  return avg;
 }
DONE(Double);

ONE(Number,{
  name=string("");
  i=0;
  f=0.0f;
  d=0.0;
 })
 Zdouble d;
 Zfloat f;
 Zint i;
 Number( int j ) : ListItem() {
  name=string("");
  i=j;
  f=(float) j;
  d=(double) j;
 }
 Number( float g ) : ListItem() {
  name=string("");
  i=(int) g;
  f=g;
  d=(double) g;
 }
 Number( double e ) : ListItem() {
  name=string("");
  next=prev=null;
  i=(int) e;
  f=(float) e;
  d=e;
 }
 SKEYSTART
  SKEY("d", { SPARAM(d,double,atof); })
  SKEY("f", { SPARAM(f,float,atof); })
  SKEY("i", { SPARAM(i,int,atoi); })
  SKEY("name", { SPARAMS(name); })
 SKEYEND("Number");
 SOUTSTART
  if ( d != 0.0 ) SOUTV("d",float,d,"%1.9f");
  if ( f != 0.0f ) SOUTV("f",float,f,"%1.9f");
  if ( i != 0 ) SOUTV("i",int,i,"%d");
  if ( name.length > 0 ) SOUTV("name",char *,name.c_str(),"%s");
 SOUTEND
 void BinaryWrite( BinaryFile *open ) {
  open->write(name);
  open->write(&d.value);
  open->write(&f.value);
  open->write(&i.value);
 }
 void BinaryRead( BinaryFile *open ) {
  open->read(&name);
  open->read(&d.value);
  open->read(&f.value);
  open->read(&i.value);
 }
 Number *Clone() {
  Number *n=new Number;
  n->i=i;
  n->d=d;
  n->f=f;
  n->name=name;
  return n;
 }
 void Name( const char *s ) { name=string(s); }
 void Integer( int j ) {  i=j;  f=(float) j;  d=(double) j; }
 void Integer( const char *s ) { Integer( atoi(s) ); }
 void Float( const char *s )   { Float( (float) atof(s) ); }
 void Float( float g ) {  i=(int) g;  f=g;  d=(double) g; }
 void Double( double e ) {  i=(int) e;  f=(float) e;  d=e; }
 void Double( const char *s )  { Double( atof(s) ); }
 void Assign( const char *s, int j ) { Name(s); Integer(j); }
 void Assign( const char *s, float g ) { Name(s); Float(g); }
 void Assign( const char *s, double e ) { Name(s); Double(e); }
 void Add( int j    ) { Integer(i+j); }
 void Add( float g  ) { Float(f+g);   }
 void Add( double e ) { Double(d+e);  }
 void Multiply( int j    ) { Integer(i*j); }
 void Multiply( float g  ) { Float(f*g);   }
 void Multiply( double e ) { Double(d*e);  }
 void Divide( int j    ) { Integer(i/j); }
 void Divide( float g  ) { Float(f/g);   }
 void Divide( double e ) { Double(d/e);  }
 void Modulo( int j    ) { Integer(i%j); }
 void Sqrt( int j )    { Float(isqrt(j)); }
 void Sqrt( float g )  { Float(sqrtf(f)); }
 void Sqrt( double e ) { Double(sqrt(e)); }
MANY(Number,NumberHandle,NumberHandles,"no",Numbers,{})
 KEYSWORDSGroups("no",Number)
 SORTING(Number,SortAscending,{},{ if ( A->d < B->d ) result=-1; },{})
 void ReadAppend( BinaryFile *open ) {
  Number *n=new Number;
  n->BinaryRead(open);
  Append(n);
 }
 void Add( int j )    { Append( new Number(j) ); }
 void Add( float g )  { Append( new Number(g) ); }
 void Add( double e ) { Append( new Number(e) ); }
 void AddIfUnique( int j )    { if ( !find(j) ) Append(new Number(j)); }
 void AddIfUnique( float g )  { if ( !find(g) ) Append(new Number(g)); }
 void AddIfUnique( double e ) { if ( !find(e) ) Append(new Number(e)); }
 void Add( Integer *i ) { Number *n=new Number; n->Integer(i->i); Append(n); }
 void Add( Float *f )   { Number *n=new Number; n->Float(f->f);   Append(n); }
 void Add( Double *d )  { Number *n=new Number; n->Double(d->d);  Append(n); }
 void Add( Integer *i, bool Delete ) { Number *n=new Number; n->Integer(i->i); Append(n); if ( Delete ) delete i; }
 void Add( Float *f, bool Delete )   { Number *n=new Number; n->Float(f->f);   Append(n); if ( Delete ) delete f; }
 void Add( Double *d, bool Delete )  { Number *n=new Number; n->Double(d->d);  Append(n); if ( Delete ) delete d; }
 void Duplicate( Numbers *numbers ) {
  EACH(numbers->first,Number,n) Append(n->Clone());
 }
 Number *find( int j )          { FOREACH(Number,n) if ( n->i == j ) return n; return null; }
 Number *find( float g )        { FOREACH(Number,n) if ( n->f == g ) return n; return null; }
 Number *find( double e )       { FOREACH(Number,n) if ( n->d == e ) return n; return null; }
 Number *find( const char *s )        { FOREACH(Number,n) if ( !str_cmp( s, n->name.c_str() ) ) return n; return null; }
 Number *find_prefix( const char *s ) { FOREACH(Number,n) if ( !str_prefix( s, n->name.c_str() ) ) return n; return null; }
 void RemoveAll( int i ) { EACHN(first,Number,p,{ if ( p->i==i ) { Remove(p); delete p; } }); }
 void RemoveAll( float g ) { EACHN(first,Number,p,{ if ( p->f==g ) { Remove(p); delete p; } }); }
 void RemoveAll( double e ) { EACHN(first,Number,p,{ if ( p->d==e ) { Remove(p); delete p; } }); }
 void Assign( const char *s, int j )    { Number *n=find(s); if ( !n ) { n=new Number; n->Name(s); Append(n); } n->Integer(j); }
 void Assign( const char *s, float g )  { Number *n=find(s); if ( !n ) { n=new Number; n->Name(s); Append(n); } n->Float(g);   }
 void Assign( const char *s, double e ) { Number *n=find(s); if ( !n ) { n=new Number; n->Name(s); Append(n); } n->Double(e);  }
 void Assign( const char *s, char *v )  {
  Number *n=find(s);
  if ( !n ) { n=new Number; n->Name(s); Append(n); }
  double d=atof(v);
  float f=(float) d;
  int i=atoi(v);
  if ( (float) i== f ) n->Integer(i);
  else if ( (double) f== d ) n->Float(f);
  else n->Double(d);
 }
 Number *Sum() {
  Number *sum=new Number;
  FOREACH(Number,factor) {
   sum->i+=factor->i;
   sum->f+=factor->f;
   sum->d+=factor->d;
  }
  return sum;
 }
 Number *Average() {
  Number *avg=new Number;
  if ( count == 0 ) return avg;
  FOREACH(Number,factor) {
   avg->i+=factor->i;
   avg->f+=factor->f;
   avg->d+=factor->d;
  }
  avg->i/=count;
  avg->f/=(float) count;
  avg->d/=(double) count;
  return avg;
 }
DONE(Number)

class Pointer : public ListItem {
public:
 void *pointer;
 Pointer() : ListItem() { 
  pointer=null;
 }
};

class Pointers : public LinkedList {
public:
 Pointer *find( void * p ) { FOREACH(Pointer,pointer) if ( p==pointer->pointer ) return pointer; return null; }
 void append( void * p ) {
  Pointer *pointer=new Pointer;
  pointer->pointer=p;
  Append((ListItem *) pointer);
 }
 void push( void * p ) {
  Pointer *pointer=new Pointer;
  pointer->pointer=p;
  Prepend((ListItem *) pointer);
 }
 CLEARLISTRESET(Pointer);
 ~Pointers() { Clear(); }
};