#pragma once

#include "ZeroTypes.h"
#include "Indexed.h"

class BoxBool : public Indexed<Zbool> {
public:
 BoxBool() : Indexed() {
  this->Size(6);
 }
 BoxBool( bool up, bool down, bool left, bool right, bool front, bool back ) {
  Set(up,down,left,right,front,back);
 }
 string toString() {
  if ( list[0] && list[1] && list[2] && list[3] && list[4] && list[5] ) return string("true");
  if ( !list[0] && !list[1] && !list[2] && !list[3] && !list[4] && !list[5] ) return string("false");
  Zstring out;
  if ( Up()    ) out+="up ";
  if ( Down()  ) out+="down ";
  if ( Left()  ) out+="left ";
  if ( Right() ) out+="right ";
  if ( Front() ) out+="front ";
  if ( Back()  ) out+="back ";
  trim(&out.value);
  return out.value;
 }
 void fromString( const char *s ) {
  Zstring words(s);
  const char *w=words.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"true") ) {
    Set(true,true,true,true,true,true);
   } else if ( !str_cmp(w,"false") ) {
    Set(false,false,false,false,false,false);
   } else if ( !str_cmp(w,"up") ) {
    Up(true);
   } else if ( !str_cmp(w,"down") ) {
    Down(true);
   } else if ( !str_cmp(w,"left") ) {
    Left(true);
   } else if ( !str_cmp(w,"right") ) {
    Right(true);
   } else if ( !str_cmp(w,"front") ) {
    Front(true);
   } else if ( !str_cmp(w,"back") ) {
    Back(true);
   }
   w=words.Next();
  }
 }
 void Set( bool up, bool down, bool left, bool right, bool front, bool back ) {
  Up(up);
  Down(down);
  Left(left);
  Right(right);
  Front(front);
  Back(back);
 }
 bool Up() { return list[0]; }
 void Up( bool exterior ) {
  list[0]=exterior;
 }
 bool Down() { return list[1]; }
 void Down( bool exterior ) {
  list[1]=exterior;
 }
 bool Left() { return list[2]; }
 void Left( bool exterior ) {
  list[2]=exterior;
 }
 bool Right() { return list[3]; }
 void Right( bool exterior ) {
  list[3]=exterior;
 }
 bool Front() { return list[4]; }
 void Front( bool exterior ) {
  list[4]=exterior;
 }
 bool Back() { return list[5]; }
 void Back( bool exterior ) {
  list[5]=exterior;
 }
 void operator =( BoxBool &b ) {
  list[0]=b.list[0];
  list[1]=b.list[1];
  list[2]=b.list[2];
  list[3]=b.list[3];
  list[4]=b.list[4];
  list[5]=b.list[5];
 }
 bool operator ==( BoxBool &b ) {
  return
     (bool)list[0]==(bool)b.list[0]
  && (bool)list[1]==(bool)b.list[1]
  && (bool)list[2]==(bool)b.list[2]
  && (bool)list[3]==(bool)b.list[3]
  && (bool)list[4]==(bool)b.list[4]
  && (bool)list[5]==(bool)b.list[5]
  ;
 }
};