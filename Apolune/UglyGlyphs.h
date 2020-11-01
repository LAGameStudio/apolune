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
#pragma once

#include "Cartesian.h"
#include "Art2d.h"
#include "StringIndexed.h"

extern Zint globalFrameCounter;

ONE(UglyCachedText,{})
 Zint stash_id;
 Zint creationFrame;
 Zdouble longest,tallest;
 Zint charsWide,linesHigh;
 Zdouble  textWidth, textHeight, kern, lineSpaceMultiple,lineSpace;
 void Set( const char *s, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0 );
 void GetThisStringIndex( Zstring *out ) { (*out) = name; }
MANY(UglyCachedText,UglyTextHandle,UglyTextHandles,"UglyText",UglyTextCache,{})
 StringIndexed<UglyCachedText> cache;
 void ClearCache() { DecacheBefore(globalFrameCounter); }
 void DecacheBefore( int globalFrameCount ) {
  FOREACHN(UglyCachedText,ut,{
   if ( ut->creationFrame < globalFrameCount ) {
    art.stash.Replace(ut->stash_id); 
    cache.Remove(ut->name.c_str());
    delete ut;
   }
  })
 }
 UglyCachedText *Text( const char * s, double textWidth, double textHeight, double kern, double lineSpaceMultiple );
 void Extents( Cartesiand *out, const char * s, double textWidth=5.0, double textHeight=7.0, double kern=2.0, double lineSpaceMultiple=1.0 );
 void Render( const char *s, Crayon tint, Blends blend, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple, bool centered );
DONE(UglyCachedText);