#include "Overtalker.h"

#include "GamePanels.h"

#include "Player.h"

void Overtalker::SetBeat( OvertalkBeat *beat ) {
 gamepadButtonDelay.Set(1.0);
 gamepadButtonDelay.Reset();
 if ( !beat ) { ob=null; return; }
 this->ob=beat;
 hasButton=ob->button.length > 0;
 buttonPressed=false;
 buttonHeight=20+8;
 buttonWidth=(int)ob->button.length*20;
 nextTime.Set(10.0f);
 nextTime.Reset();
 seed=ob->seed.integer(); 
 portrait.Seed(seed,0,ob->male);
 portraitPaperdoll.Set( (player && ob->isPlayer) ? &player->portrait : &portrait);
 Strings moods;
 MouthController::DeployMoods(&moods);
 String *m=(String *)moods.match(ob->mood.c_str());
 portraitPaperdoll.Set(mt_8Bit,mt_Normal);
 if ( m ) {
  portraitPaperdoll.mouth.Mood((MouthMoods)m->integer);
 } else portraitPaperdoll.mouth.Mood(mt_None);
 portraitPaperdoll.Speak(ob->content.c_str());
 content=ob->content;
 if ( player ) {
  content("%first%",player->firstName.c_str());
  content("%last%",player->lastName.c_str());
  content("%name%",FORMAT("%s %s",player->firstName.c_str(),player->lastName.c_str()).c_str());
  content("%rankname%",FORMAT("%s %s %s",player->rank.c_str(),player->firstName.c_str(),player->lastName.c_str()).c_str());
  content("%namerank%",FORMAT("%s %s",player->rank.c_str(),player->lastName.c_str()).c_str());
  content("%ranklast%",FORMAT("%s %s",player->rank.c_str(),player->lastName.c_str()).c_str());
  content("%rank%",FORMAT("%s",player->rank.c_str()).c_str());
  content("%FIRST%",player->firstName.c_str());
  content("%LAST%",player->lastName.c_str());
  content("%NAME%",FORMAT("%s %s",player->firstName.c_str(),player->lastName.c_str()).c_str());
  content("%RANKNAME%",FORMAT("%s %s %s",player->rank.c_str(),player->firstName.c_str(),player->lastName.c_str()).c_str());
  content("%NAMERANK%",FORMAT("%s %s",player->rank.c_str(),player->lastName.c_str()).c_str());
  content("%RANKLAST%",FORMAT("%s %s",player->rank.c_str(),player->lastName.c_str()).c_str());
  content("%RANK%",FORMAT("%s",player->rank.c_str()).c_str());
 }
 timeout=(double)content.length/20.0;
 content=TexFitWidth(glowTermFont,-2,768,16,content);
 int lines=linefeeds(content.c_str());
 int textHeight=(lines+1)*(20+4);
 iconArea.SetRect(256+32,fbo1080p.h-16-textHeight-32-128,128,128);
 extents.SetRect(256+32+128+16,fbo1080p.h-16-textHeight-32-128,32+768,textHeight+32);
 if ( started ) ob->effect.Impulse();
 if ( ob->lifeformReference.length > 0 ) {
  species=evolution.named(ob->lifeformReference.c_str());
  if ( species ) {
   paperdoll=&species->paperdoll;
   paperdoll->Dereferences();
   if ( ob->poseReference.length > 0 ) {
    pose=paperdoll->sequences.named(ob->poseReference.c_str());
    if ( !pose ) pose=(Pose2dSequence *)paperdoll->sequences.first;
   }
  } else {
   paperdoll=null;
   pose=null;
  }
 }
}

#include "QMapViewer.h"

void Overtalker::ExecuteScript( const char *s ) {
 ScriptSections script(s);
 if ( script.global ) EACH(script.global->lines.first,ScriptLine,sl) {
  String *command=sl->FirstWord();
  if ( !command ) continue;
  Zstring one(sl->Word1());
  double damage=0.0;
  if ( sl->ThirdWord() ) damage=sl->Double3();
  Zstring beat=sl->Word4();
  beat.Trim();
  if ( one == "stop" || one == "break" ) { break; } else
  if ( one == "portrait" ) {
   if ( sl->Word2("remove") ) hasPortrait=false;
   else {
    PlayerPortrait pp; pp.Seed(sl->Integer3(),sl->Integer4(),sl->Word2("male")?true:false);
    portraitPaperdoll.Set(&pp);
    hasPortrait=true;
   }
  } else
  if ( one == "go" ) {
   if ( player && player->Go(sl) ) {
    return;
   } else return;
  } else if ( one == "panel" ) {
   bool done=GamePanelByName(sl->Word2().c_str(),sl->Word3().c_str(),activeEngineWindow);
//   if ( done ) return;
  } else  if ( one == "dialog" ) {
   Dialogue *d=dialogues.named(sl->Word2().c_str());
   if ( d ) {
    DialoguePlayer *dp=new DialoguePlayer;
    dp->Set(d,1,damage);
    ///
    dp->back_to=back_to;
    back_to->visible=false;
    windows.Add(dp);
    if ( beat.length > 0 ) { 
     if ( !dp->AdvanceBeat(beat.c_str()) ) {
      // Means it was a failure to find the referenced beat, abort?
     }
    }
    return;
   }
  } else if ( one == "qmap" ) {
   QMap *q=qmaps.named(sl->Word2().c_str());
   if ( q ) {
    QMapViewer *qv=new QMapViewer(q);
    ///
    qv->back_to=back_to;
    back_to->visible=false;
    windows.Add(qv);
    return;
   }
  } else if ( one == "narrate" || one == "overtalk" || one == "narration" ) {
   Overtalk *o=overtalks.named(sl->Word2().c_str());
   if ( o ) {    
    if ( back_to && back_to->is("SpaceGameView") ) {
     SpaceGameView *sgv=(SpaceGameView *) back_to.pointer;
     if ( sl->Word3("priority") ) sgv->overtalkers.Add(sgv,o,true);
     else sgv->overtalkers.Add(sgv,o);
    }
   }
  } else if ( one == "play" ) {
   player->Play(sl);
  } else if ( one == "music" ) {
   player->Music(sl);
  } else if ( one == "silence" ) {
   player->Silence(sl);
  } else if ( one == "cloud" ) {
   if ( !player->Cloud(sl) ) return;
  } else if ( one == "quest" ) {
   ScriptResultEnum result=player->Questing(sl);
   if ( result == sre_ConditionFailed ) return;
   else continue;
  } else if ( one == "bonus" ) {
   player->Bonus(sl,true);
  } else if ( one == "starmap" ) {
   player->StarMap(sl);
  } else if ( one == "announce" ) {
   player->Announce(sl);
  }
 }
}