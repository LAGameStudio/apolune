#include "QMapViewer.h"

#include "DialoguePlayer.h"
#include "TradeScreen.h"
#include "Player.h"
#include "GamePanels.h"

bool QMapViewer::ExecuteScript( const char *script ) {
 ScriptSections *ss=new ScriptSections;
 ScriptSection *global=ss->Sectionize(script);
 if ( !player ) return true;
 if ( global ) {
  if ( player ) player->lastCrew=null;
  EACH(global->lines.first,ScriptLine,sl) {
   if ( !sl->FirstWord() ) continue;
   Zstring one=sl->FirstWord()->s;
   if ( one == "stop" || one == "break" ) { break; } else
   if ( one == "play" ) {
    player->Play(sl);
   } else if ( one == "music" ) {
    player->Music(sl);
   } else if ( one == "silence" ) {
    player->Silence(sl);
   } else if ( one == "dialog" ) {
    if ( !sl->SecondWord() ) continue;
    Zstring two=sl->SecondWord()->s;
    Zstring four=sl->Word4();
    Dialogue *d=dialogues.named(two.c_str());
    if ( !d ) { Return(); return true; }
    DialogueBeat *db=null;
    if ( four.length > 0 ) db=d->beats.named(four.c_str());
    if ( !db ) db=d->beats.named("start");
    if ( !db ) db=(DialogueBeat *)d->beats.first;
    if ( !db ) { Return(); return true; }
    DialoguePlayer *dp=new DialoguePlayer;
    dp->Set(d,sl->FifthWord()?sl->Integer5():1,((double)sl->Integer3()/100.0));
    dp->AdvanceBeat(db);
    dp->back_to=back_to;
    this->destroyMe=true;
    windows.Add(dp);
    return true;
   } else if ( one == "qmap" ) {
   QMap *q=qmaps.named(sl->Word2().c_str());
   if ( q ) {
    QMapViewer *qv=new QMapViewer(q);
    ///
    windows.Add(qv);
    qv->back_to=this->back_to;
    this->destroyMe=true;
    return true;
   }
  } /* else if ( one == "mood" ) {
    if ( portraitPaperdoll.portrait ) {
     Strings moods; MouthController::DeployMoods(&moods);
     String *moodsValue=moods.match(sl->Word2().c_str());
     if ( moodsValue ) {
      portraitPaperdoll.Set(portraitPaperdoll.mouth.mouth,(MouthMoods)moods.IndexOf(moodsValue));
     }
    }
    if ( lifeform && paperdoll ) {
    }
   } */else if ( one == "starmap" ) {
    player->StarMap(sl);
   } else if ( one == "go" ) {
    if ( !player ) { Return(); return true; }
    if ( !sl->SecondWord() ) continue;
    if ( !player->Go(sl,null,true) ) {
     Return();
     return false;
    }
   } else if ( one == "panel" ) {
    return GamePanelByName(sl->Word2().c_str(),sl->Word3().c_str(),this);
   } else if ( one == "back" ) {
    Return();
    return false;
   } else if ( one == "has" ) {
    if ( !player ) continue;
    bool result=player->Has(sl);
    if ( result ) continue;
    else {
     return true;
    }
   } else if ( one == "take" ) {
    if ( !player ) continue;
    player->Take(sl);
   } else if ( one == "give" ) {
    if ( !player ) continue;
    player->Give(sl);
   } else if ( one == "blueprint" ) {
    if ( !player ) continue;
    player->Blueprint(sl);
   } else if ( one == "pay" ) {
    if ( !player ) continue;
    player->Pay(sl);
   } else if ( one == "charge" ) {
    if ( !player ) continue;
    Zstring three;
    bool result=player->Charge(sl);
    if ( !result ) {
     return false;
    }
    continue;
   } else if ( one == "cloud" ) {
    if ( !player ) continue;
    bool result=player->Cloud(sl);
   } else if ( one == "bonus" ) {
    if ( !player ) continue;
    player->Bonus(sl);
   } else if ( one == "stat" ) {
    if ( !player ) continue;
    player->Statting(sl);
   } else if ( one == "quest" ) {
   ScriptResultEnum result=player->Questing(sl);
   if ( result == sre_ConditionFailed ) return false;
   else continue;
   } else if ( one == "check" ) {
    if ( !player ) continue;
    bool result=player->Checking(sl);
   } else if ( one == "skill" ) {
    if ( !player ) continue;
    bool result=player->Skilling(sl);
   } else if ( one == "teach" ) {
    if ( !player ) continue;
    player->Teaching(sl);
   } else {} // bad command ignored...
  }
 }
 return false;
}