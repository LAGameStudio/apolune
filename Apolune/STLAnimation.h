#pragma once

#include "STLLibrary.h"

class STLAnimation : public ListItem {
public:
 Zstring filename;
 Indexed<Zp<STLFile>> frame;
 Zint frames;
 void fromString( const char * s ) {
  Zstring words(s);
  const char *w=words.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"frame") ) {
    w=words.Next();
    frame.Increase();
    frame[frame.length-1]=stls.Load(w);
   }
   w=words.Next();
  }
 }
 STLFile *GetFrame( double expired, double duration ) {
  if ( frames == 0 ) return null;
  int frameno=(int)((expired/duration)*(double)(frames-1));
  return frame[frameno%frames];
 }
};

class STLAnimations : public LinkedList {
public:
 STLAnimation *find( const char *s ) {
  FOREACH(STLAnimation,a) if ( !str_cmp(a->filename.c_str(),s) ) return a;
  return null;
 }
 STLAnimation *Load( const char *s ) {
  STLAnimation *a=find(s);
  if ( a ) return a;
  string frames=file_as_string(s);
  a=new STLAnimation;
  a->filename=s;
  a->fromString(frames.c_str());
  Append(a);
  return a;
 }
 CLEARLISTRESET(STLAnimation);
};

extern STLAnimations stlAnimations;