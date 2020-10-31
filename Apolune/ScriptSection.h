#pragma once

#include "Strings.h"

ONE(ScriptLine,{})
 Strings words;
 ScriptLine( String *s ) : ListItem() { Set(s); }
 void Set( const char *in ) { words.SplitPush(in); }
 void Set( string in ) { words.SplitPush(in.c_str()); }
 void Set( String *in ) { words.SplitPushCase(in->c_str()); }
 String *FirstWord() {  return (String *)words.first; }
 String *SecondWord() {  return (String *)words.Element(1); }
 String *ThirdWord() {  return (String *)words.Element(2); }
 String *FourthWord() {  return (String *)words.Element(3); }
 String *FifthWord() {  return (String *)words.Element(4); }
 String *SixthWord() {  return (String *)words.Element(5); }
 String *SeventhWord() {  return (String *)words.Element(6); }
 String *EighthWord() {  return (String *)words.Element(7); }
 String *NinthWord() {  return (String *)words.Element(8);  }
 String *TenthWord() {  return (String *)words.Element(9);  }
 string Word1() { String *w=FirstWord() ; return w ? w->s : string(""); }
 string Word2() { String *w=SecondWord(); return w ? w->s : string(""); }
 string Word3() { String *w=ThirdWord() ; return w ? w->s : string(""); }
 string Word4() { String *w=FourthWord(); return w ? w->s : string(""); }
 string Word5() { String *w=FifthWord() ; return w ? w->s : string(""); }
 string Word6() { String *w=SixthWord() ; return w ? w->s : string(""); }
 string Word7() { String *w=SeventhWord();return w ? w->s : string(""); }
 string Word8() { String *w=EighthWord(); return w ? w->s : string(""); }
 string Word9() { String *w=NinthWord();  return w ? w->s : string(""); }
 string Word10(){ String *w=TenthWord();  return w ? w->s : string(""); }
 bool Word1( const char *compare ) { return !str_cmp(Word1().c_str(),compare);  }
 bool Word2( const char *compare ) { return !str_cmp(Word2().c_str(),compare);  }
 bool Word3( const char *compare ) { return !str_cmp(Word3().c_str(),compare);  }
 bool Word4( const char *compare ) { return !str_cmp(Word4().c_str(),compare);  }
 bool Word5( const char *compare ) { return !str_cmp(Word5().c_str(),compare);  }
 bool Word6( const char *compare ) { return !str_cmp(Word6().c_str(),compare);  }
 bool Word7( const char *compare ) { return !str_cmp(Word7().c_str(),compare);  }
 bool Word8( const char *compare ) { return !str_cmp(Word8().c_str(),compare);  }
 bool Word9( const char *compare ) { return !str_cmp(Word9().c_str(),compare);  }
 bool Word10( const char *compare ){ return !str_cmp(Word10().c_str(),compare); }
 int Integer2() { Zstring i=Word2(); return i.integer();  }
 int Integer3() { Zstring i=Word3(); return i.integer();  }
 int Integer4() { Zstring i=Word4(); return i.integer();  }
 int Integer5() { Zstring i=Word5(); return i.integer();  }
 int Integer6() { Zstring i=Word6(); return i.integer();  }
 int Integer7() { Zstring i=Word7(); return i.integer();  }
 int Integer8() { Zstring i=Word8(); return i.integer();  }
 int Integer9() { Zstring i=Word9(); return i.integer();  }
 int Integer10(){ Zstring i=Word10(); return i.integer(); }
 double Double2() { Zstring i=Word2(); return i.decimal();  }
 double Double3() { Zstring i=Word3(); return i.decimal();  }
 double Double4() { Zstring i=Word4(); return i.decimal();  }
 double Double5() { Zstring i=Word5(); return i.decimal();  }
 double Double6() { Zstring i=Word6(); return i.decimal();  }
 double Double7() { Zstring i=Word7(); return i.decimal();  }
 double Double8() { Zstring i=Word8(); return i.decimal();  }
 double Double9() { Zstring i=Word9(); return i.decimal();  }
 double Double10(){ Zstring i=Word10(); return i.decimal(); }
 CLONE(ScriptLine,{
  words.DuplicateAsStrings(&a->words);
 })
 KEYWORDS({ SSUBWORD("w",words) })
 WORDKEYS({ SSUBKEY("w",words)  })
MANY(ScriptLine,ScriptLineHandle,ScriptLineHandles,"L",ScriptLines,{})
 CLONES(ScriptLine,ScriptLines);
 KEYSWORDSGroups("L",ScriptLine)
DONE(ScriptLine);

ONE(ScriptSection,{})
 ScriptLines lines;
 Zdis<ScriptLine> line;
 ScriptSection( const char *sectionName, Strings *lines ) : ListItem() {
  name=sectionName;
  Set(lines);
 }
 void Set( Strings *in ) {
  Strings L;
  L.Concat(in);
  EACH(lines.first,String,s) {
   lines.Append(new ScriptLine(s));
  }
 }
 CLONE(ScriptSection,{
  DUPE(name)
  if ( line ) a->line.Recycle(line->Duplicate());
  a->lines.Duplicate(&lines);
 })
 KEYWORDS({
  SUBWORDdis("section",line)
  SSUBWORD("lines",lines)
 })
 WORDKEYS({
  if ( line ) SUBKEY("section",line)
  SSUBKEY("lines",lines)
 })
MANY(ScriptSection,ScriptSectionHandle,ScriptSectionHandles,"ss",ScriptSections,{})
 Zp<ScriptSection> global;
 KEYSWORDSGroups("ss",ScriptSection)
 CLONES(ScriptSection,ScriptSections)
 ScriptSections( const char *s ) : LinkedList() { global=Sectionize(s); }
 ScriptSection *Section( const char *s ) { return named(s); }
 void Sections( const char *named, ScriptSectionHandles *out, bool overrideRequirements=false );
 ScriptSection *Sectionize( const char *script );
 void Merge( const char *script ) {
  ScriptSections ss(script);
  Concat(&ss);
 }
 void Merge( const char *script, having includedGlobalScope ) {
  ScriptSections ss(script);
  EACHN(ss.first,ScriptSection,s,{
   ScriptSection *t=named(s->name.c_str());
   if ( t ) t->lines.Concat(&s->lines);
   else {
    ss.Remove(s);
    Append(s);
   }
  });
 }
 void Merge( ScriptSections *in ) {
  ScriptSections ss;
  ss.Duplicate(in);
  Concat(&ss);
 }
 void Merge( ScriptSections *in, having includedGlobalScope ) {
  ScriptSections ss;
  ss.Duplicate(in);
  EACHN(ss.first,ScriptSection,s,{
   ScriptSection *t=named(s->name.c_str());
   if ( t ) t->lines.Concat(&s->lines);
   else {
    ss.Remove(s);
    Append(s);
   }
  });
 }
DONE(ScriptSection)

struct ScriptBreakdown {
public:
 Zp<ScriptSections> sections;
 ScriptSectionHandles s_hailed,s_announced,s_unlocked;
 Zp<ScriptSection> s_saved,s_dies,s_triggered,s_worn,s_used,s_installed,s_key,s_lock,s_opened,s_activated,s_deactivated;
 Strings keys,locks;
 Zint lockDifficulty;
 void Breakdown( ScriptSections *in ) {
  sections=in;
  sections->Sections("announced",&s_announced);
  sections->Sections("hailed",&s_hailed);
  sections->Sections("unlocked",&s_unlocked);
  s_saved=sections->Section("saved");
  s_dies=sections->Section("dies");
  s_triggered=sections->Section("triggered");
  s_worn=sections->Section("worn");
  s_used=sections->Section("used");
  s_installed=sections->Section("installed");
  s_key=sections->Section("key");
  GetSettings(s_key,"unlocks",&keys);
  s_lock=sections->Section("lock");
  GetSettings(s_lock,"lock",&locks);
  s_opened=sections->Section("opened");
  s_activated=sections->Section("activated");
  s_deactivated=sections->Section("deactivated");
 }
 void GetSettings( ScriptSection *in, const char *command, Strings *out ) {
  if ( in ) {
   EACH(in->lines.first,ScriptLine,sl) {
    String *one=sl->FirstWord();
    if ( one && !str_cmp(one->c_str(),command) ) {
     String *two=sl->SecondWord();
     if ( two ) {
      if ( !out->find(two->c_str()) ) out->Add(two->c_str());
     }
    }
   }
  }
 }
};

ONE(NamedScript,{})
 Zstring filepath,named;
 ScriptSections ss;
MANY(NamedScript,NamedScriptHandle,NamedScriptHandles,"NamedScript",NamedScripts,{})
 NamedScript *Get(const char *s) {
  FOREACH(NamedScript,ns) if ( !str_cmp(ns->named.c_str(),s) ) return ns;
  return null;
 }
 // Returns the next line after the included section for use in Sectionize
 ScriptLine *Include( ScriptLine *line, ScriptSection *out ) {
  ScriptLine *insertBefore=(ScriptLine *) line->next;
  NamedScript *including=Get(line->Word2().c_str());
  if ( !including ) return insertBefore;
  ScriptSection *ss=including->ss.Section("global");
  if ( !ss ) return insertBefore;
  if ( !insertBefore ) {
   EACH(ss->lines.first,ScriptLine,sl) {
    out->lines.Append(sl->Duplicate());
   }
  } else {
   EACH(ss->lines.first,ScriptLine,sl) {
    out->lines.InsertBefore(sl->Duplicate(),line);
   }
  }
  return insertBefore;
 }
 void ExecuteReplace( ScriptLine *newLine, ScriptLine *executionParameters );
 ScriptLine *Execute( ScriptLine *line, ScriptSection *out ) {
  ScriptLine *insertBefore=(ScriptLine *) line->next;
  NamedScript *including=Get(line->Word2().c_str());
  if ( !including ) return insertBefore;
  ScriptSection *ss=including->ss.Section("global");
  if ( !ss ) return insertBefore;
  if ( !insertBefore ) {
   EACH(ss->lines.first,ScriptLine,sl) {
    ScriptLine *nl=sl->Duplicate();
    ExecuteReplace(nl,line);
    out->lines.Append(nl);
   }
  } else {
   EACH(ss->lines.first,ScriptLine,sl) {
    ScriptLine *nl=sl->Duplicate();
    ExecuteReplace(nl,line);
    out->lines.InsertBefore(nl,line);
   }
  }
  return insertBefore;
 }
 void AddFromPath( const char *path ) {
 }
DONE(NamedScript);

extern NamedScripts namedScripts;