#include "GameObject.h"
#include "Campaign.h"
#include "GameDereference.h"
#include "GameWorldQuery.h"
#include "Player.h"
#include "GamePanels.h"

#include "SpaceTrooperInstance.h"

void GameObject::OnStore( int i, int j ) {
  switch ( this->rtid.value ) {
  case RTID_CelestialInstance: 
  case RTID_ThingInstance:
  case RTID_SpaceSign: 
  case RTID_Vehicle: 
  case RTID_SpaceDriver:
  case RTID_LifeformInstance:
  case RTID_SpaceTrooperInstance:
   wdb.Remove(this,true);
  break;
  default: break;
 }
}

void GameObject::OnRetrieve() {
  switch ( this->rtid.value ) {
  case RTID_CelestialInstance: 
  case RTID_ThingInstance:
  case RTID_SpaceSign: 
  case RTID_Vehicle: 
  case RTID_SpaceDriver:
  case RTID_LifeformInstance:
  case RTID_SpaceTrooperInstance:
   wdb.Add(this);
  break;
  default: break;
 }
}

float GameObject::GetSoundGain( double toofar ) {
 double d=idistance((int)x,(int)y,player->screen.x,player->screen.y);
 return (float)UMAX(0.0,1.0-d/toofar);
}

void GameObjects::AssignUnassigned( Campaign *c ) {
 FOREACH(GameObject,m) if ( !m->campaign ) m->campaign=c;
}

void GameObjects::Query( Campaign *c, GameObjectHandles *out ) {
 FOREACH(GameObject,q) if ( q->campaign == c ) out->Add(q);
}

void GameObject::Implement() {
}


void GameObject::Contains( int qty, const char *reference ) {
 GameDereference gd;
 gd.Reference(reference,campaign);
 if ( gd.commodity ) {
  contents.Deposit(gd.commodity,qty);
 }
 if ( gd.commodities ) {
  EACH(gd.commodities->first,CommodityHandle,ch) contents.Deposit(ch->p,qty);
 }
 if ( gd.thing ) {
  for ( int i=0; i<qty; i++ ) contents.Deposit(gd.thing);
 }
 if ( gd.stuff ) {
  EACH(gd.stuff->first,ThingHandle,th) for ( int i=0; i<qty; i++ ) contents.Deposit(th->p);
 }
 if ( gd.spacecar ) {
  for ( int i=0; i<qty; i++ ) contents.Deposit(gd.spacecar);
 }
 if ( gd.spacecars ) {
  EACH(gd.spacecars->first,SpaceCarHandle,sch) for ( int i=0; i<qty; i++ ) contents.Deposit(sch->p);
 }
}

bool GameObject::ExecuteScript( ScriptSection *ss ) {
 EACH(ss->lines.first,ScriptLine,sl) {
  Zstring one(sl->Word1());
  if ( one.length > 0 ) {   
   if ( one == "panel" ) return GamePanelByName(sl->Word2().c_str(),sl->Word3().c_str(),activeEngineWindow);
   else if ( one == "lock" ) Lock(sl);
   else if ( one == "unlock" ) Unlock(sl);
   else if ( one == "open" ) { bool result=Open(sl); if ( result ) return result; }
   else if ( one == "close" ) Close(sl);
   else if ( one == "activate" ) Activate(sl);
   else if ( one == "deactivate" ) Deactivate(sl);
   else if ( player ) {
    if ( one == "play" ) player->Play(sl);
    else if ( one == "music" ) player->Music(sl);
    else if ( one == "silence" ) player->Silence(sl);
    else if ( one == "announce" ) player->Announce(sl);
    else if ( one == "go" ) return player->Go(sl);
    else if ( one == "pay" ) player->Pay(sl);
    else if ( one == "bonus" )player->Bonus(sl);
    else if ( one == "give" ) player->Give(sl);
    else if ( one == "blueprint" ) player->Blueprint(sl);
    else if ( one == "take" ) player->Take(sl);
    else if ( one == "dialog" ) { bool result=player->Dialog(sl);   if ( result ) return result; }
    else if ( one == "cloud" )  { bool result=player->Cloud(sl);    if ( result ) return result; }
    else if ( one == "quest" )  {
     ScriptResultEnum result=player->Questing(sl);
     if ( result == sre_ConditionSuccess || result == sre_NoEffect ) continue;
     else return result != sre_ConditionFailed;
    }
    else if ( one == "charge" ) { bool result=player->Charge(sl);   if ( result ) return result; }
   } 
  }
 }
 return false;
}