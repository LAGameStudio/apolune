// CCommandLine.cpp
// By Johan

#include <iostream>
#include <string>
using namespace std;

#include "CommandLine.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

CCommandLine CmdLine;

CCommandLine::~CCommandLine() {	
//	entries.Clear();
}

CCommandLine::CCommandLine() {
 char *p=m_CommandLine=GetCommandLineA();
 Zstring complete=string(p);
 execution=complete.next_case();
 size_t location=execution.value.find_last_of('/');
 if ( location == string::npos ) location=execution.value.find_last_of('\\');
 if ( location != string::npos ) working=execution.value.substr(0,location+1);
 options=complete.p;
 options.rewind();
 const char *opt=options.Next_case();
 while ( *opt != '\0' ) {
  entries.Push(opt);
  opt=options.Next_case();
 }
 options.rewind();
 OUTPUT("---------------- Command Line ---------------------------- Begin\n");
 OUTPUT("Work Path: `%s`\n", working.c_str() );
 OUTPUT("Executable: `%s`\n", execution.c_str() );
 OUTPUT("Options string: `%s`\n", options.c_str() );
 OUTPUT("Found the following command line options:\n");
 EACH(entries.first,String,s) OUTPUT("%s\n",s->c_str());
 OUTPUT("---------------- Command Line ---------------------------- End\n");
}

String *CCommandLine::Option(char *Entry) {
 String *result=entries.matchPrefix(Entry);
 if ( !result ) {
  OUTPUT("Command line option checked but not found: %s\n", Entry );
 } else {
  OUTPUT("Command line option found: %s -> %s\n", Entry, result->c_str() );
 }
 return result;
}

string CCommandLine::OptionPair(char *Entry) {
 String *opt=Option(Entry);
 if ( opt ) {
  if ( opt->next ) {
   String *value=(String *)opt->next;
   return value->s;
  }
 }
 return string("");
}

Strings *CCommandLine::Options(char *Prefix) {
 Strings *out=new Strings;
 entries.matchesPrefix(Prefix,out);
 if ( out->count == 0 ) { delete out; out=null; }
 return out;
}

void CCommandLine::Options(char *Prefix, Strings *out ) {
 entries.matchesPrefix(Prefix,out);
}