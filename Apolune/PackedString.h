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

#include "ZeroTypes.h"
#include "Indexed.h"

class PackedString : private Indexed<uint32> {
public:
 inline uint32 PACK(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3) {
  return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
 } 
 inline uint32 PACK(char c0, char c1, char c2, char c3) {
  return PACK((unsigned char)c0, (unsigned char)c1, (unsigned char)c2, (unsigned char)c3);
 }
 PackedString() {}
 PackedString( const char *s, bool lowercase=true ) {
  Set(s,lowercase);
 }
 size_t Length() { return this->length; }
 size_t W() { return this->w; }
 size_t H() { return this->h; }
 uint32 Get( unsigned int idx ) { return (*this)(idx); }
 void Set( const char *s, bool lowercase=true ) {
  size_t len=strlen(s);
  this->Size(len/4 + (len%4 == 0 ? 0 : 1));
  const char *p=s;
  char c[4];
  memset(c,0,sizeof(char)*4);
  int i=0,j=0;
  while ( *p != '\0' ) {
   c[i]=lowercase ? LOWER(*p) : (*p);
   i++;
   if ( *(p+1) == '\0' || i > 3 ) {
    i=0;
    this->list[j]=PACK(c[0],c[1],c[2],c[3]);
    memset(c,0,sizeof(char)*4);
    j++;
   }
   p++;
  }
 }
 bool exactly( const PackedString *in ) {
  if ( in->length != length ) return false;
  for ( size_t i=0; i<length; i++ ) if ( this->list[i] != in->list[i] ) return false;
  return true;
 }
 PackedString& operator= ( const char *s ) { Set(s,true);  return *this; }
 PackedString& operator&=( const char *s ) { Set(s,false); return *this; }
};


inline bool operator == (PackedString& a, PackedString& b);
inline bool operator != (PackedString& a, PackedString& b);