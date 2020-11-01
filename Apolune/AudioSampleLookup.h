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
#include "Strings.h"
#include "ZeroTypes.h"
#include "LinkedList.h"

ONE(AudioSampleLookup,{
})
 Zstring fp;
 WORDKEYS({
  WORDKEY("name",name)
  WORDKEY("file",fp)
 })
 KEYWORDS({
  KEYWORD("name",{name=w;})
  else KEYWORD("file",{fp=w;})
  else BADKEYWORD;
 })
MANY(AudioSampleLookup,AudioSampleLookupHandle,AudioSampleLookupHandles,"sample",AudioSampleLookups,{})
 KEYSWORDSGroups("sample",AudioSampleLookup)
 string Get( const char *search ) {
  AudioSampleLookup *asl=named(search);
  return asl ? asl->fp : string("");
 }
 void Load( const char *txt ) {
  fromString( file_as_string(txt).c_str() );
 }
DONE(AudioSampleLookup)

extern AudioSampleLookups sampleLookup;