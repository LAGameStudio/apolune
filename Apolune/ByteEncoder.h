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

#include "ListItem.h"
#include "ClassDecoder.h"

class ByteProgram : public ListItem {
public:
 void Encode( string s ) { Encode(s.c_str()); }
 void Encode( const char *s ) {
  string keyword;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == ' ' || *p == '\n' || *p == '\r' ) p++;
   if ( *p == '#' || ( *p == '/' && *(p+1) == '/' ) ) 
   p=string_argument(p,&keyword);
   String *className=classdecoder.FindClassNameByName(keyword);
   if ( className != null ) { // We found a class instantiation
   } else {
   }
  }
 }
};

class ByteEncoder {
public:
 ByteEncoder(void) {}
 ~ByteEncoder(void) {}
};

