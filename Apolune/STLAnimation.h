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