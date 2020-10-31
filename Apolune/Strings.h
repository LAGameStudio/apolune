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
#pragma warning (disable:4100)

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
using namespace std;
#include "macros.h"
#include "LinkedList.h"
#include "string_functions.h"
#include "BinaryFile.h"

class String : public ListItem {
public:
 string s;
 int integer;
 float floating;
 char c;
 String() : ListItem() {
  s = string("");
  integer=0;
  floating=0.0f;
  c='\0';
 } 
 String( const char *i ) : ListItem() {
  s=string(i);
  integer=0;
  floating=0.0f;
  c='\0';
 }
 String( string i ) : ListItem() {
  s=i;
  integer=0;
  floating=0.0f;
  c='\0';
 }
 String( string i, bool UsesSpecialCodes ) : ListItem() {
  s = UnStringSpecialCodes(i);
  integer=0;
  floating=0.0f;
  c='\0';
 }
 String( String *t ) : ListItem() {
  s=t->s;
  integer=t->integer;
  floating=t->floating;
  c='\0';
 }
 String *Duplicate() { return new String(this); }
 String( const char *k, int i ) {
  integer=i;
  s = string(k);
  prev=next=null;
 }
 const char *c_str(void); // converts the node to a C-str
 virtual ListItem *Copy() {
  return (ListItem *) new String( this );
 }

 VSKEYSTART
  this->s=UnStringSpecialCodes(key);
  SKEYPAIR; if ( is_integer((char *) key.c_str()) ) { integer=atoi(key.c_str()); } else if ( is_decimal((char *) key.c_str()) ) { floating=(float)atof(key.c_str()); }
  SKEYPAIR; if ( is_decimal((char *) key.c_str()) ) { floating=(float)atof(key.c_str()); } else if ( is_integer((char *) key.c_str()) ) { integer=atoi(key.c_str()); }
 SKEYEND("String");
 virtual string toString() {
  string out=string("");
  out+=indention+string("{")+StringSpecialCodes(this->s)+string("}");
  if ( integer != 0 ) {
   char buf[80];
   FORMAT(buf,80," %d",(int) integer);
   out+=string(buf);
  }
  if ( floating != 0.0f ) {
   char buf[80];
   FORMAT(buf,80," %1.9f",(float) floating);
   out+=string(buf);
  }
  return out;
 }
};

class KeyValuePair : public String {
public:
 VSKEYSTART
  this->s=UnStringSpecialCodes(key);
  SKEYPAIR; this->value=UnStringSpecialCodes(key); if ( is_integer((char *) key.c_str()) ) { integer=atoi(key.c_str()); } else if ( is_decimal((char *) key.c_str()) ) { floating=(float)atof(key.c_str()); }
  SKEYPAIR; if ( is_decimal((char *) key.c_str()) ) { floating=(float)atof(key.c_str()); } else if ( is_integer((char *) key.c_str()) ) { integer=atoi(key.c_str()); }
 SKEYEND("KeyValuePair");
 virtual string toString() {
  return string("{")+s+string("} \"")+value+string("\"");
 }
 string value;
 KeyValuePair() {
  s = string("");
  value = string("");
  prev=next=null;
 }
 KeyValuePair( KeyValuePair *t ) {
  prev=next=null;
  s=t->s;
  integer=t->integer;
  floating=t->floating;
  value=t->value;
 }
 KeyValuePair( const char *k, const char *v ) {
  s = string(k);
  value = string(v);
  prev=next=null;
 }
 KeyValuePair( const char *k, int i ) {
  static char buf[1024];
  integer=i;
  s = string(k);
  FORMAT( buf,1024, "%d", (int) i );
  value=string(buf);
  prev=next=null;
 }
 KeyValuePair( const char *k, float f ) {
  static char buf[1024];
  floating =f;
  s = string(k);
  FORMAT( buf,1024,  "%f", (float) f );
  value=string(buf);
  prev=next=null;
 }
 void Set( int i ) {  
  static char buf[1024];
  integer=i;
  FORMAT( buf,1024, "%d", (int) i );
  value=string(buf);
 }
 void Set( float f ) {  
  static char buf[1024];
  floating =f;
  FORMAT( buf,1024,  "%f", (float) f );
  value=string(buf);
 }
 void Set( const char *v ) {
  value=string(v);
 }
 void Set( const char *k, const char *v ) {
  s=string(k);
  value=string(v);
 }
 virtual ListItem *Copy() {
  return new KeyValuePair( this );
 }
};

class Strings : public LinkedList {
public:
 SKEYSTART
  SKEY("string",   { SKEYPAIR; String *s=new String; s->fromString(key.c_str()); Append(s); })
  SKEY("keyvalue", { SKEYPAIR; KeyValuePair *s=new KeyValuePair; s->fromString((char *)key.c_str()); Append(s); })
  SKEY("kv",       { SKEYPAIR; KeyValuePair *s=new KeyValuePair; s->fromString((char *)key.c_str()); Append(s); })
  SKEY("key",      { SKEYPAIR; KeyValuePair *s=new KeyValuePair; s->fromString((char *)key.c_str()); Append(s); })
  SKEY("assign",   { SKEYPAIR; KeyValuePair *s=new KeyValuePair; s->fromString((char *)key.c_str()); Append(s); })
 SKEYEND("Strings");
 void DuplicateAsStrings( Strings *out ) {
  FOREACH(String,x) out->Add(x->c_str(),x->integer,x->floating);
 }
 // Populate with a list of files in a folder (non-recursive).
 void ls_( const char *targetPath, bool listFiles=true, bool prependPath=true, bool filterOSFiles=true );
 void files( const char *t ) { ls_(t); }
 void folder( const char *t ) { ls_(t); }
 Strings& b64kEncode() {
  FOREACH(String,s) s->s=b64k_encode(s->s.c_str());
  return *this;
 }
 Strings& b64kDecode() {
  FOREACH(String,s) s->s=b64k_decode(s->s.c_str());
  return *this;
 }
 // Wordnumbers start at 1, not 0, are compared to another string
 bool Word( int wordnumber, const char *compared ) {
  if ( wordnumber < 0 || wordnumber > count ) return false;
  String *e=(String *)Element(wordnumber-1);
  if ( e ) {
   if ( !str_cmp(e->s.c_str(), compared) ) return true;
  }
  return false;
 }
 // Returns 0 when it fails, use IsNumeric to check first if needed.
 int Integer( int wordnumber ) {
  if ( wordnumber < 0 || wordnumber > count ) return 0;
  String *e=(String *)Element(wordnumber-1);
  if ( e ) { return atoi(e->s.c_str()); }
  return 0;
 }
 // Returns 0 when it fails, use IsNumeric to check first if needed.
 double Double( int wordnumber ) {
  if ( wordnumber < 0 || wordnumber > count ) return 0;
  String *e=(String *)Element(wordnumber-1);
  if ( e ) { return atof(e->s.c_str()); }
  return 0.0;
 }
 bool IsNumeric( int wordnumber, const char *compared ) {
  if ( wordnumber < 0 || wordnumber > count ) return false;
  String *e=(String *)Element(wordnumber-1);
  if ( e ) { if ( e->s.length() > 0 && is_integer(e->s.c_str()) || is_decimal(e->s.c_str()) ) return true; }
  return false;
 }
 void fromSimpleString( const string& in ) {
  string piece=string("");
  const char *p=string_argument(in.c_str(),&piece);
  while ( piece.length() > 0 ) {
   this->Add(piece);
   p=string_argument(p,&piece);
  }
 }
 void fromSimpleString( const char *in ) {
  string piece=string("");
  const char *p=string_argument(in,&piece);
  while ( *p != '\0' ) {
   this->Add(piece);
   p=string_argument(p,&piece);
  }
 }
 void Output() {
  OUTPUT("%s", toString().c_str() );
 }
 BINARY_OUTCustom(BinaryWriteStringList,{
  open->scrawl(count.value);
  FOREACH(String,s) { open->scrawl(s->s.c_str()); }
 })
 virtual string toStringList() {
  string out=string("");
  FOREACH(String,s) {
   out+=string("{")+s->s+string("}\n");
  }
  return out;
 }
 BINARY_INCustom(BinaryReadStringList,{
  int total=0; open->read(&total);
  for ( int i=0; i<total; i++ ) { String *s=new String; Append(s); open->read(&s->s); }
 })
 void fromStringList( const char * s ) {
  Zstring in(s);
  const char *w=in.Next();
  while ( *w != '\0' ) {
   String *str=new String(w);
   Append(str);
   w=in.Next();
  }
 }

 BINARY_OUTCustom(BinaryWriteDecimalList,{
  open->scrawl(count.value);
  FOREACH(String,s) { open->scrawl(s->floating); open->scrawl(s->s.c_str()); }
 })
 virtual string toStringDecimalList() {
  string out=string("");
  FOREACH(String,s) {
   out+=string("{")+s->s+string("} ")+FORMAT("%1.2f",s->floating)+string("\n");
  }
  return out;
 }
 BINARY_INCustom(BinaryReadDecimalList,{
  int total=0; open->read(&total);
  for ( int i=0; i<total; i++ ) { String *s=new String; Append(s); open->read(&s->floating); open->read(&s->s); }
 })
 void fromStringDecimalList( const char * s ) {
  Zstring in(s);
  const char *w=in.Next();
  while ( *w != '\0' ) {
   String *str=new String(w);
   w=in.Next();
   str->floating=(float)atof(w);
   Append(str);
   w=in.Next();
  }
 }
 
 BINARY_OUTCustom(BinaryWriteIntegerList,{
  open->scrawl(count.value);
  FOREACH(String,s) { open->scrawl(s->integer); open->scrawl(s->s.c_str()); }
 })
 virtual string toStringIntegerList() {
  string out=string("");
  FOREACH(String,s) {
   out+=string("{")+s->s+string("} ")+FORMAT("%d",s->integer)+string("\n");
  }
  return out;
 }
 BINARY_INCustom(BinaryReadIntegerList,{
  int total=0; open->read(&total);
  for ( int i=0; i<total; i++ ) { String *s=new String; Append(s); open->read(&s->integer); open->read(&s->s); }
 })
 void fromStringIntegerList( const char * s ) {
  Zstring in(s);
  const char *w=in.Next();
  while ( *w != '\0' ) {
   String *str=new String(w);
   w=in.Next();
   str->integer=atoi(w);
   Append(str);
   w=in.Next();
  }
 }
 string toString() {
  string out=string("");
  incdent();
  FOREACH(String,s) {
   string classname=typeid(*s).name();
   string discard;
   const char *q=string_argument(classname.c_str(),&discard); // discards the words "class or struct"
   if ( !str_cmp(q,"String") ) out+=indention+string("string {\n")+s->toString()+string("}\n");
   else
   if ( !str_cmp(q,"KeyValuePair") ) {
    KeyValuePair *kv=(KeyValuePair *)s;
    out+=indention+string("kv {\n")+kv->toString()+string("}\n");
   }
  }
  decdent();
  return out;
 }
 int longest() {
  int candidate=0;
  FOREACH(String,s) if ( (int)s->s.length() < candidate ) candidate=(int)s->s.length();
  return candidate;
 }
 String *Longest() {
  String *candidate=null;
  FOREACH(String,s) if ( candidate ) candidate=s;
  return candidate;
 }
 Strings() : LinkedList() {}
 Strings( const char *filename ) : LinkedList() {
  Load(filename);
 } /* Returns a list of lines */
 bool AddIfUnique( const char *s ) {
  FOREACH(String,st) if ( !str_cmp(s,st->s.c_str()) ) return false;
  Add(s);
  return true;
 }
 String *Add( const char *s ) {
  String *S=new String(s);
  S->integer=count;
  Append(S);
  return S;
 }
 String *Add( string s ) {
  String *S=new String(s);
  S->integer=count;
  Append(S);
  return S;
 }
 String *Add( const char *s, char c ) {
  Append(new String(s));
  String *L=(String *) last;
  L->c=c;
  return L;
 }
 String *Add( char c, const char *s ) {
  Append(new String(s));
  String *L=(String *) last;
  L->c=c;
  return L;
 }
 String *Add( string s, char c ) {
  Append(new String(s));
  String *L=(String *) last;
  L->c=c;
  return L;
 }
 String *Add( char c, string s ) {
  Append(new String(s));
  String *L=(String *) last;
  L->c=c;
  return L;
 }
 String *Add( const char *s, int i ) {
  Append(new String(s));
  String *L=(String *) last;
  L->integer=i;
  return L;
 }
 String *Add( string s, int i ) {
  Append(new String(s));
  String *L=(String *) last;
  L->integer=i;
  return L;
 }
 String *Add( const char *s, float i ) {
  Append(new String(s));
  String *L=(String *) last;
  L->floating=i;
  return L;
 }
 String *Add( string s, float i ) {
  Append(new String(s));
  String *L=(String *) last;
  L->floating=i;
  return L;
 }
 String *Add( string s, int i, float f ) {
  Append(new String(s));
  String *L=(String *) last;
  L->integer=i;
  L->floating=f;
  return L;
 }
 String *Add( const char *s, int i, float f ) {
  Append(new String(s));
  String *L=(String *) last;
  L->integer=i;
  L->floating=f;
  return L;
 }
 KeyValuePair *addKeyValue( const char *s, const char *v ) {
  KeyValuePair *k=new KeyValuePair( s, v );
  Append(k);
  return k;
 }
 KeyValuePair *addKeyValue( const char *s, int v ) {
  KeyValuePair *k=new KeyValuePair( s, v );
  Append(k);
  return k;
 }
 KeyValuePair *addKeyValue( const char *s, float v ) {
  KeyValuePair *k=new KeyValuePair( s, v );
  Append(k);
  return k;
 }
 void Load( const char *filename );  /* Returns a list of lines */
 void Save( const char *filename );
 bool inList( string s );
 String *matchPrefix( const char *s );
 void matchesPrefix( const char *s, Strings *out );
 // caseless match or returns null
 String *match( const char *s );
 String *find( string s );
 String *find( const char *s );
 String *find( const char c ) {
  FOREACH(String,s) if ( s->c==c ) return s;
  return null;
 }
 String *find( int i ) {
  FOREACH(String,s) if ( s->integer==i ) return s;
  return null;
 }
 const char *AnyChar() {
  String *ele=(String *)Any();
  if ( !ele ) {
  OUTPUT("Strings:AnyChar() found no random element (is the list empty?)\n");
  }
  return ele->c_str();
 }
 const char *AnyChar( int seed ) {
  String *ele=(String *)Any(seed);
  if ( !ele ) {
  OUTPUT("Strings:AnyChar(%d) found no random element (is the list empty?)\n", (int) seed);
  }
  return ele->c_str();
 }
 const char *Chars( int i ) {
  FOREACH(String,s) if ( s->integer==i ) return s->s.c_str();
  OUTPUT("Strings:Chars(%d) reports no such element in list, returning empty string\n", (int) i);
  return &string_error;
 }
 // Gets the value or empty string if none
 string Get( int i ) {
  String *s=(String *) Element(i);
  if ( s ) return s->s;
  return string("");
 }
 // Gets the value or empty string if none, wraps
 string Get( int i, having modulo ) {
  String *s=(String *) Element(i % count.value);
  if ( s ) return s->s;
  return string("");
 }
 string strings( int i ) {
  FOREACH(String,s) if ( s->integer==i ) return s->s;
  return string("");
 }
 bool includes( Strings *all_of_these ) {
  FOREACH(String,s) EACH(all_of_these->first,String,t) if ( str_cmp(s->s.c_str(),t->s.c_str()) ) return false;
  return true;
 }
 bool is_included( Strings *cloud ) {
  EACH(cloud->first,String,t) FOREACH(String,s) if ( str_cmp(s->s.c_str(),t->s.c_str()) ) return false;
  return true;
 }
 String *findCaseless( string s );
 String *findCaseless( const char *s );
 String *findSubstring( const char *s );
 String *findNextSubstring( String *start, const char *s );
 string concatToNextSubstring( String *start, const char *end );
 int IndexOf( String *s );
 string Concatenate( having Seamless ); // Returns duplicated concatenations of Strings
 string Concatenate( char seperator='\n' ); // Returns duplicated concatenations of Strings
 void appendFromString( const char *s, char c );
 void appendFromString( string s, char c );
 void show(void);
 void show( const char *sep, const char *end );
 // Splits using string_argument
 void SplitPush( const char *s );
 void SplitBy( const char *source, const char *delim="" );
 // Splits using string_argument_case
 void SplitPushCase( const char *s );
 // Splits using string_argument_case, keeping the distinguishers like quotes and brackets
 void SplitPushCase( having keepDistinguishers, const char *s );
 // Splits using string_argument_case_ignorequotes, keeping the distinguishers like quotes, commas and parens
 void SplitPushCaseIgnoreQuotes( const char *s );
 // Splits by seperator char
 void SplitPush( string s, char seperator );
 // Splits by char_isof, ex. delimiters of newline/space="\n "
 void SplitPush( const char *s, const char *delimiters );
 // Splits by seperator char
 void SplitPush( const char *s, char seperator );
 // Splits by script language rules
 void ScriptPush( const char *s );
 void FScriptPush( const char *s );
 void Push( string s );
 void Push( const char *s );
 String *PushAndGet( const char *s );
 void DuplicateAndPush( const char * s );
 void DuplicateAndPush( Strings *in ) { Copy(in); }
 void Copy( Strings *ss ) {
  EACH(ss->first,String,s) Append( s->Copy() );
 }
 void ClearThenCopy( Strings *ss ) {
  Clear();
  Copy(ss);
 }
 void printf( const char * fmt, ...) {
  static char buf [32768];
  va_list args;
  va_start (args, fmt);
  vsprintf_s (buf, fmt, args);
  va_end (args);
  Push(buf);
 }
 SORTING(String,SortAZ,{},{
  const char *p=A->c_str(); while ( *p != '\0' && !IS_ALPHA(*p) ) p++;
  const char *q=B->c_str(); while ( *q != '\0' && !IS_ALPHA(*q) ) q++;
  while ( *p!= '\0' && *q!='\0' ) {
   if ( (int) LOWER(*p) > (int) LOWER(*q) ) { result=1; break; }
   if ( (int) LOWER(*p) < (int) LOWER(*q) ) { result=-1; break; }
   p++; while ( *p != '\0' && !IS_ALPHA(*p) ) p++;
   q++; while ( *q != '\0' && !IS_ALPHA(*q) ) q++;
  }
  if ( result ==0 && *p != '\0' ) result=1;
  if ( result ==0 && *q != '\0' ) result=-1;
 },{});
 bool NotMatchesKeyValues( Strings *pairs ) {
  FOREACH(KeyValuePair,k) {
   KeyValuePair *kv=(KeyValuePair *) pairs->find(k->s);
   if ( kv->value != k->value ) return true;
  }
  return false;
 }
 virtual CLEARLISTRESET(String);
 ~Strings(void) { Clear(); }
};

// Text File Handler
class TextFile {
public:
 FILE *fp;
 errno_t err;
 TextFile( const char *name, bool write ) {
  err = fopen_s(&fp,name, ( write ? "w" : "r" ) );
 }
 ~TextFile() { if ( fp ) fclose(fp); }
 void Printf( const char *fmt, ... ) {
  va_list args;
  va_start (args, fmt);
  fprintf_s (fp, fmt, args);
  va_end (args);
 }
 void PrintCrayon( const char *key, float *floats ) {
  fprintf_s(fp, "%s {%f %f %f %f}", key, floats[0], floats[1], floats[2], floats[3] );
 }
 void print( const char *s ) {
  fprintf_s (fp, "%s", s );
 }
 void print( string s ) {
  fprintf_s (fp, "%s", s.c_str() );
 }
};

Strings *ls( const char *targetPath, bool listFiles=true, bool prependPath=true, bool filterOSFiles=true );
Strings *ls( const char *targetPath, bool listFiles, bool prependPath, Strings *filters );

// Opens a Get Folder dialog, allows you to browse "This PC"
string dialog_get_folder( const char *msg1="", const char *msg2="Selected folder directory from below:", bool newFolderButton=true );
// Opens a kinda lame Get Folder or File dialog, allows you to browse from "Desktop" down
string dialog_get_file_or_folder( const char *msg1="", const char *msg2="Select the file or folder from below:", bool newFolderButton=true );
// Opens the classic "multi-select files from a folder" dialog
string dialog_multiselect( Strings *out, const char *msg1="", const char *msg2="Select multiple files or folders:" );