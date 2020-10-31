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