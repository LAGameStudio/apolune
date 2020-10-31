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
#include "Strings.h"
#include "GLImage.h"
#include "TextureLibrary.h"

class SafeImageElement : public ListItem {
public:
 int value;
 GLImage *lookup;
 SafeImageElement( const char *filename, int association, GLImage *defaultImage, ImageProcess *ip ) {
  value=association;
  GLImage *uploaded=library.find(filename);
  if ( !uploaded ) uploaded=defaultImage;
  lookup=library.Load(uploaded,ip,false);
  if ( !lookup ) lookup=defaultImage;
 }
 SafeImageElement( const char *filename, int association, GLImage *defaultImage ) : ListItem() {
  value=association;
  lookup=library.Load(filename);
  if ( !lookup ) lookup=defaultImage;
 }
 SafeImageElement( const char *filename, int association, GLImage *defaultImage, TextureLibraryLoadTextureOptions opts ) : ListItem() {
  value=association;
  lookup=library.Load(filename,opts);
  if ( !lookup ) lookup=defaultImage;
 }
};

template<typename T>
class SafeImageLookup : public LinkedList {
public:
 string name;
 GLImage *defaultImage;
 SafeImageLookup() : LinkedList() {
  name=string("");
  defaultImage=null;
 }
 void Name( const char *s ) { name=string(s); }
 void Name( string s ) { name=s; }
 void Default( const char *s ) { 
  defaultImage=library.Load(s);
 }
 void Default() {
  defaultImage=library.Load("data/images/elements/white.png");
 }
 void Load( string filename ) {
  Load(filename.c_str());
 }
 virtual int Decipher( const char *value ) {
  return atoi(value);
 }
 void Load( const char *filename ) {
  Strings file;
  file.Load(filename);
  char buf[STRING_SIZE];
  EACH(file.first,String,s) {
   const char *p=s->s.c_str();
   p=one_argument(p,buf);
   SafeImageElement *e=new SafeImageElement(p,Decipher(value),defaultImage);
   Append(e);
  }
 }
 void Add( const char *filename, T association ) {
  SafeImageElement *e=new SafeImageElement(filename,(int)association,defaultImage);
  Append(e);
 }
 void Add( T association, const char *filename ) {
  SafeImageElement *e=new SafeImageElement(filename,(int)association,defaultImage);
  Append(e);
 }
 void Add( const char *filename ) { // Autonumber
  SafeImageElement *e=new SafeImageElement(filename,count,defaultImage);
  Append(e);
 }
 void Add( const char *filename, GLint S, GLint T ) { // Autonumber
  SafeImageElement *e=new SafeImageElement(filename,count,defaultImage,S,T);
  Append(e);
 }
 void Add( const char *filename, ImageProcess *ip ) {
  SafeImageElement *e=new SafeImageElement(filename,count,defaultImage,ip);
  Append(e);
 }
 GLImage *Any() {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345)%count);
  if ( !k || !k->lookup ) return defaultImage;
  return k->lookup;
 }
 GLImage *Any( int lo, int hi ) {
  SafeImageElement *k=(SafeImageElement *) Element(lo+upto(hi-lo)%count);
  if ( !k || !k->lookup ) return defaultImage;
  return k->lookup;
 }
 GLImage *Any( int seed ) {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345,seed)%count);
  if ( !k || !k->lookup ) return defaultImage;
  return k->lookup;
 }
 GLImage *Any( int seed, int lo, int hi ) {
  SafeImageElement *k=(SafeImageElement *) Element(lo+upto(hi-lo,seed)%count);
  if ( !k || !k->lookup ) return defaultImage;
  return k->lookup;
 }
 GLImage *AnyIndex( int rotation ) {
  SafeImageElement *k=(SafeImageElement *) Element(rotation%count);
  if ( !k || !k->lookup ) return defaultImage;
  return k->lookup;
 }
 const char *AnyFilename() {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345)%count);
  if ( !k || !k->lookup ) return defaultImage->filename.c_str();
  return k->lookup->filename.c_str();
 }
 const char *AnyFilename( int seed ) {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345,seed)%count);
  if ( !k || !k->lookup ) return defaultImage->filename.c_str();
  return k->lookup->filename.c_str();
 }
 string AnyString() {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345)%count);
  if ( !k || !k->lookup ) return defaultImage->filename.c_str();
  return k->lookup->filename;
 }
 string AnyString( int seed ) {
  SafeImageElement *k=(SafeImageElement *) Element(upto(12345,seed)%count);
  if ( !k || !k->lookup ) return defaultImage->filename.c_str();
  return k->lookup->filename;
 }
 GLImage *image( T value ) {
  FOREACH(SafeImageElement,k) if ( k->value == (int) value ) return k->lookup;
  OUTPUT("SafeImageLookup: Set named `%s` reported value %d not found!", name.c_str(), (int) value);
  return defaultImage;
 }
 GLImage *i( T value ) { return image(value); }
 const char *filename( T value ) {
  GLImage *looked_up=image(value);
  return looked_up->filename.c_str();
 }
};

class SafeImageLookupInt : public SafeImageLookup<int> {
public:
};

class SafeSoundLookup : public Strings {
public:
 string name;
 string defaultSound;
 SafeSoundLookup() : Strings() {
  name=string("");
  defaultSound=string("data/sounds/default.wav");
 }
 void Name( const char *s ) { name=string(s); }
 void Name( string s ) { name=s; }
 void Add( const char *s ) {
  String *S=new String(s);
  S->integer=count;
  Append(S);
 }
 const char *sound( int index ) {
  String *candidate=find(index);
  if ( !candidate ) return (char *) defaultSound.c_str();
  return candidate->s.c_str();
 }
 const char *random() {
  String *candidate=(String *) Any();
  if ( !candidate ) return defaultSound.c_str();
  return candidate->s.c_str();
 }
 const char *random( int hi, int lo ) {
  String *candidate=(String *) Element(lo+upto(hi-lo));
  if ( !candidate ) return defaultSound.c_str();
  return candidate->s.c_str();
 }
 const char *random( int seed ) {
  String *candidate=(String *) Element(upto(count,seed));
  if ( !candidate ) return defaultSound.c_str();
  return candidate->s.c_str();
 }
 const char *random( int seed, int hi, int lo ) {
  String *candidate=(String *) Element(lo+upto(hi-lo,seed));
  if ( !candidate ) return defaultSound.c_str();
  return candidate->s.c_str();
 }
 int randomi() {
  String *candidate=(String *) Any();
  if ( !candidate ) return 0;
  return candidate->integer;
 }
 int randomi( int hi, int lo ) {
  String *candidate=(String *) Element(lo+upto(hi-lo));
  if ( !candidate ) return 0;
  return candidate->integer;
 }
 int randomi( int seed ) {
  String *candidate=(String *) Element(upto(count,seed));
  if ( !candidate ) return 0;
  return candidate->integer;
 }
 int randomi( int seed, int hi, int lo ) {
  String *candidate=(String *) Element(lo+upto(hi-lo,seed));
  if ( !candidate ) return 0;
  return candidate->integer;
 }
};