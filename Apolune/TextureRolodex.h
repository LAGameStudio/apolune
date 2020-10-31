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

#include "GLImage.h"
#include "TextureLibrary.h"
#include "Strings.h"
#include "Chemistry.h"
#include "Masking.h"

class TextureCard : public NamedHandle<Substance> {
public:
 GLImage *i,*mask;
 TextureCard() : NamedHandle() { i=mask=null; name=string(""); }
 TextureCard( const char *n, Substance *M ) : NamedHandle() { p=M; i=mask=null; name=string(n); }
 TextureCard( const char *n, GLImage *I ) : NamedHandle() { p=null; i=I; mask=null; name=string(n); }
 TextureCard( const char *n, GLImage *I, GLImage *_mask )  {
  p=null;
  i=I;
 }
};

class TextureRolodex : public LinkedList {
public:
 void addSubstance( const char *name, const char *m ) {
  Substance *S=substances.find(m);
  if ( S ) Append(new TextureCard(name,S));
 }
 void addSubstance( const char *name, Substance *S ) { if ( S ) Append(new TextureCard(name,S)); }
 void addImage( const char *name, char *i ) {
  GLImage *I=library.find(i);
  if ( I ) Append(new TextureCard(name,I));
 }
 /*
  * Format: substance substance.name symbol
  *         image filename symbol
  */
 void imagesFromString( const char *s ) {
  char buf[1024];
  string name;
  const char *p=s;
  while ( p && *p != '\0' ) {
   p=one_argcase(p,buf);
   if ( LOWER(*p)=='s' ) {
    int lights=1;
    p=one_argcase(p,buf);
    name=string(buf);
    p=one_argcase(p,buf);
    addSubstance(buf,(char *) name.c_str());
   } else
   if ( LOWER(*p)=='i' ) {
    p=one_argcase(p,buf);
    name=string(buf);
    p=one_argcase(p,buf);    
    addImage(buf,(char *) name.c_str());
   }
  }
 }
 CLEARLISTRESET(TextureCard);
 ~TextureRolodex(void) { Clear(); }
};

