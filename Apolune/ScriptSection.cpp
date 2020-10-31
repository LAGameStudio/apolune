#include "ScriptSection.h"

#include "Player.h"

NamedScripts namedScripts;

 ScriptSection *ScriptSections::Sectionize( const char *script ) {
  Zstring code(remove_slash_comments(script));
  Strings lines;
  lines.FScriptPush(script);
  ScriptSection *ss=new ScriptSection;
  ss->name="global_scope";
  global=ss;
  Append(ss);
  EACH(lines.first,String,line) {
   ScriptLine *sl=new ScriptLine;
   sl->Set(line);
   String *one=sl->FirstWord();
   if ( one ) {
    Zstring word1(one->c_str());
    if ( word1.contains(':') ) { // New Section, of the format   sectionName: <param1> <param2>...
     word1(":","");
     word1.Trim();
     ss=new ScriptSection;
     ss->name=word1.c_str();
     ss->line.Recycle(sl); // stores the parameters
     Append(ss);
    } else {
     ss->lines.Append(sl);
    }
   }
  }
  FOREACH(ScriptSection,ss) {
   EACHN(ss->lines.first,ScriptLine,sl,{
    if ( sl->Word1("include") ) {
     n=namedScripts.Include(sl,ss);
     Remove(sl);
     delete sl;
    } else if ( sl->Word1("execute") ) {
     n=namedScripts.Include(sl,ss);
     Remove(sl);
     delete sl;
    }
   });
  }
  return global;
 }

void ScriptSections::Sections( const char *named, ScriptSectionHandles *out, bool overrideRequirements ) {
 FOREACH(ScriptSection,s) if ( !str_cmp(named,s->name.c_str())) {
  if ( overrideRequirements || !s->line || !player || player->SectionRequire(s->line) ) { // Omits sections that have requirements not met at the time of the poll
   out->Add(s);
  }
 }
}

void NamedScripts::ExecuteReplace( ScriptLine *newLine, ScriptLine *executionParameters ) {
 int seed=netGameSeed;
 int seed2=(int)(pseudo(seed)*100.0f);
 EACH(newLine->words.first,String,s) {
  Zstring str(s->s.c_str());
  str("^here",player->reloadHere.c_str());
  str("^was",player->lastLocale.c_str());
  str("^rank",player->rank.c_str());
  str("^first",player->firstName.c_str());
  str("^last",player->lastName.c_str());
  str("^name",FORMAT("%s %s",player->firstName.c_str(),player->lastName.c_str()).c_str());  
  str("^namerank",FORMAT("%s %s %s",player->rank.c_str(),player->firstName.c_str(),player->lastName.c_str()).c_str());
  str("^rankname",FORMAT("%s %s %s",player->rank.c_str(),player->firstName.c_str(),player->lastName.c_str()).c_str());
  str("^ship",player->vehicle?player->vehicle->ship_name.c_str():"the spaceship");
  str("^percent",I2S(pseudorandom(seed++,netGameSeed+seed2)%100));
  str("^who",player->lastCrew?player->lastCrew->name.c_str():"last person you interacted with");
  str("^what",player->lastThing?player->lastThing->name.c_str():"last thing you interacted with");
  str("^when",FORMAT("SD %d %d-%d",player->stardateYear.value,player->stardateMonth.value,player->stardateDay.value).c_str());
  str("^where",player->spaceGameView?player->spaceGameView->locale->named.c_str():player->interiorGameView?player->interiorGameView->interior->caption.c_str():"Somewhere in the Universe");
  str("^reference",player->lastReference().c_str());
  s->s=str.value;
 }
 int param=0;
 EACH(executionParameters->words.first,String,s) {
  EACH(newLine->words.first,String,ms) {
   Zstring str(ms->s);
   str(FORMAT("%%%d%%",param).c_str(),s->s.c_str());
   ms->s=str.value;
  }
  param++;
 }
}
