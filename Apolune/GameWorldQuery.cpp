#include "GameWorldQuery.h"

#include "SpaceSign.h"
#include "SpaceDriver.h"
#include "Vehicle.h"
#include "CelestialInstance.h"
#include "ThingInstance.h"
#include "SpaceTrooperInstance.h"
#include "CombatController.h"
#include "BulletInstance.h"
#include "LifeformInstance.h"

#include "Player.h"

GameWorldDatabase wdb;

bool GameWorldQuery::isFriendOf( bool iAmEnemy, bool theyAreEnemy ) {
 return iAmEnemy == theyAreEnemy;
}

SpaceSign *GameWorldQuery::FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindNearestSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p != notIncluding && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindNearestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}


SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p != notIncluding && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindFarthestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d > cand ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

extern Zint netSeed;

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed+decisionSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
  i++;
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i+decisionSeed) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed+decisionSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && ssh->p != notIncluding && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed+decisionSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) && ssh->p->purpose == purpose ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i+decisionSeed) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceSign *GameWorldQuery::FindRandomSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, int decisionSeed ) {
 if ( !signList ) return null;
 SpaceSign *candidate=null;
 double cand=-1.0;
 int i=netSeed+decisionSeed;
 EACH(signList->first,SpaceSignHandle,ssh) if ( ssh->p != self && !notIncluding->find(ssh->p) ) {
  double d=ddistance(ssh->p->position.x,ssh->p->position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || pseudo(i) > 0.5f ) {
   candidate=ssh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceDriver *GameWorldQuery::FindNearestSpaceCarEnemy( ListItem *self, NavigationController *nav, SpaceDriverHandles *driverList, bool iAmEnemy ) {
 if ( !driverList ) return null;
 SpaceDriver *candidate=null;
 double cand=-1.0;
 EACH(driverList->first,SpaceDriverHandle,dh) if ( dh->p != self && !isFriendOf(iAmEnemy,dh->p->isEnemy) ) {
  double d=ddistance(dh->p->nav.position.x,dh->p->nav.position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=dh->p;
   cand=d;
  }
 }
 return candidate;
}

SpaceDriver *GameWorldQuery::FindNearestSpaceCarEnemy( ListItem *self, NavigationController *nav, SpaceDriverHandles *driverList, SpaceCarAITypes ai, bool iAmEnemy ) {
 if ( !driverList ) return null;
 SpaceDriver *candidate=null;
 double cand=-1.0;
 EACH(driverList->first,SpaceDriverHandle,dh) if ( dh->p != self && dh->p->type == ai && !isFriendOf(iAmEnemy,dh->p->isEnemy) ) {
  double d=ddistance(dh->p->nav.position.x,dh->p->nav.position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=dh->p;
   cand=d;
  }
 }
 return candidate;
}

Vehicle *GameWorldQuery::FindNearestVehicleEnemy( ListItem *self, NavigationController *nav, VehicleHandles *vehicleList, bool iAmEnemy ) {
 if ( !vehicleList ) return null;
 Vehicle *candidate=null;
 double cand=-1.0;
 EACH(vehicleList->first,VehicleHandle,vh) if ( vh->p != self && !isFriendOf(iAmEnemy,vh->p->isEnemy) ) {
  double d=ddistance(vh->p->nav.position.x,vh->p->nav.position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=vh->p;
   cand=d;
  }
 }
 return candidate;
}

Vehicle *GameWorldQuery::FindNearestVehicleEnemy( ListItem *self, NavigationController *nav, ListItem *excluding, VehicleHandles *vehicleList, bool iAmEnemy ) {
 if ( !vehicleList ) return null;
 Vehicle *candidate=null;
 double cand=-1.0;
 EACH(vehicleList->first,VehicleHandle,vh) if ( vh->p != self && vh->p != excluding && !isFriendOf(iAmEnemy,vh->p->isEnemy) ) {
  double d=ddistance(vh->p->nav.position.x,vh->p->nav.position.y,nav->position.x,nav->position.y);
  if ( cand < 0.0 || d < cand ) {
   candidate=vh->p;
   cand=d;
  }
 }
 return candidate;
}

AirlockState *GameWorldQuery::AirlockOn( Vehicle *v ) {
 AirlockState *candidate=null;
 return candidate;
}


void GameWorldQuery::ScreenRelativeGain( const char *soundfx, double x, double y, bool one_at_a_time ) {
 double soundTooFar=1920.0;
 double distanceToMidXY=ddistance(display.w2d,display.h2d,x,y);
 if ( distanceToMidXY > soundTooFar ) return;
 double finalgain=1.0-(distanceToMidXY / soundTooFar);
 double finalpitch=1.0-finalgain*0.1;
 if ( one_at_a_time ) {
  audio.manager.CueAndPlayIfNotPlaying(soundfx,(float)finalpitch,(float)finalgain,false,true);
 } else {
  audio.manager.CueAndPlayLimited(soundfx,8,(float)finalpitch,(float)finalgain,false,true);
 }
}

void GameWorldQuery::PlayPlayerRelative( NavigationController *nav, SoundEffect *sfx ) {
 if ( !nav ) return;
 if ( !sfx ) return;
 if ( sfx->sounds.count == 0 ) return;
 String *fx=(String *) sfx->sounds.Any();
 if ( !fx ) return;
 GameWorldQuery::ScreenRelativeGain(fx->c_str(),nav->position.x,nav->position.y,false);
}

//------------------------------------------------------------------------------

void GameWorldDatabase::Init() {
 Dispose();
 signList=new SpaceSignHandles;
 driverList=new SpaceDriverHandles;
 vehicleList=new VehicleHandles;
 celestialList=new CelestialInstanceHandles;
 celestialHittable=new CelestialInstanceHandles;
 thingList=new ThingInstanceHandles;
 trooperList=new SpaceTrooperInstanceHandles;
 bullets=new BulletInstancer;
 lifeformList=new LifeformInstanceHandles;
}

void GameWorldDatabase::Dispose() {
 signList.Delete();
 driverList.Delete();
 vehicleList.Delete();
 celestialList.Delete();
 celestialHittable.Delete();
 thingList.Delete();
 lifeformList.Delete();
 trooperList.Delete();
 bullets.Delete();
 fx.Clear();
}


void GameWorldDatabase::Register( SpaceGameView *sgv ) {
 activeFarWorldStorage.Add(&sgv->_wdb_fx);
 activeFarWorldStorage.Add(&sgv->_fx);
 activeFarWorldStorage.Add(&sgv->_fx_ground);
 proce55orLists.Add(&sgv->fx);
 proce55orLists.Add(&sgv->fx_ground);
 proce55orLists.Add(&sgv->foreground_fx);
}

void GameWorldDatabase::Register( InteriorGameView *igv ) {
 activeFarWorldStorage.Add(&igv->_wdb_fx);
 activeFarWorldStorage.Add(&igv->_fx);
 activeFarWorldStorage.Add(&igv->_lights);
 proce55orLists.Add(&igv->fx);
 proce55orLists.Add(&igv->lights);
}

void GameWorldDatabase::Deregister( SpaceGameView *sgv ) {
 activeFarWorldStorage.Drop(&sgv->_wdb_fx);
 activeFarWorldStorage.Drop(&sgv->_fx);
 activeFarWorldStorage.Drop(&sgv->_fx_ground);
 proce55orLists.Drop(&sgv->fx);
 proce55orLists.Drop(&sgv->fx_ground);
 proce55orLists.Drop(&sgv->foreground_fx);
}

void GameWorldDatabase::Deregister( InteriorGameView *igv ) {
 activeFarWorldStorage.Drop(&igv->_wdb_fx);
 activeFarWorldStorage.Drop(&igv->_fx);
 activeFarWorldStorage.Drop(&igv->_lights);
 proce55orLists.Drop(&igv->fx);
 proce55orLists.Drop(&igv->lights);
}

void GameWorldDatabase::Find( LocationMarker *marker, GameObjectHandles *out ) {
 EACH(fx.first,GameObject,o) if ( o->marker == marker ) out->Add(o);
}


void GameWorldDatabase::Find( GameObject *self, ThingInstanceHandles *out, double radius ) {
 if ( !thingList ) return;
 EACH(thingList->first,ThingInstanceHandle,tih) if ( !tih->p->farStorage && tih->p != self && ddistance(tih->p->x,tih->p->y,self->x,self->y) < radius ) {
  out->Add(tih->p);
 }
}

void GameWorldDatabase::Add( GameObject *o ) {
 switch ( o->rtid.value ) {
  case RTID_CelestialInstance: celestialList->Add((CelestialInstance *)o); break;
  case RTID_ThingInstance: thingList->Add((ThingInstance *)o); break;
  case RTID_SpaceSign: signList->Add((SpaceSign *)o); break;
  case RTID_Vehicle: vehicleList->Add((Vehicle *)o); break;
  case RTID_SpaceDriver: driverList->Add((SpaceDriver *)o); break;
  case RTID_LifeformInstance: lifeformList->Add((LifeformInstance *)o); break;
  case RTID_SpaceTrooperInstance: trooperList->Add((SpaceTrooperInstance *)o); break;
  default: break;
 }
}

// TODO: Handle "Space Navigation Memory"
void GameWorldDatabase::RemoveCompletely( GameObject *o, bool ignorePlayerReferences ) {
 switch ( o->rtid.value ) {
  case RTID_CelestialInstance: if ( celestialList->ListIteminList(o) ) celestialList->Remove((CelestialInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((CelestialInstance *)o); break;
  case RTID_ThingInstance: if ( thingList->ListIteminList(o) ) thingList->Remove((ThingInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((ThingInstance *)o); break;
  case RTID_SpaceSign: if ( signList->ListIteminList(o) ) signList->Remove((SpaceSign *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceSign *)o); break;
  case RTID_Vehicle: if ( vehicleList->ListIteminList(o) ) vehicleList->Remove((Vehicle *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((Vehicle *)o); break;
  case RTID_SpaceDriver: if ( driverList->ListIteminList(o) ) driverList->Remove((SpaceDriver *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceDriver *)o); break;
  case RTID_LifeformInstance: if ( lifeformList->ListIteminList(o) ) lifeformList->Remove((LifeformInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((LifeformInstance *)o); break;
  case RTID_SpaceTrooperInstance: if ( trooperList->ListIteminList(o) ) trooperList->Remove((SpaceTrooperInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceTrooperInstance *)o); break;
  default: break;
 }
 if ( fx.ListIteminList(o) ) fx.Remove(o);
 EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(o);
 EACH(proce55orLists.first,Proce55orsHandle,plist) if  ( plist->p->ListIteminList(o) ) plist->p->Remove(o);
}

GameObject *GameWorldDatabase::RemoveCompletely( const char *s, bool ignorePlayerReferences ) {
 GameObject *o=GetGenericByReference(s);
 if ( o ) {
  RemoveCompletely(o,ignorePlayerReferences);
 }
 return o;
}

void GameWorldDatabase::Remove( GameObject *o, bool ignorePlayerReferences ) {
 switch ( o->rtid.value ) {
  case RTID_CelestialInstance: celestialList->Drop((CelestialInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((CelestialInstance *)o); break;
  case RTID_ThingInstance: thingList->Drop((ThingInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((ThingInstance *)o); break;
  case RTID_SpaceSign: signList->Drop((SpaceSign *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceSign *)o); break;
  case RTID_Vehicle: vehicleList->Drop((Vehicle *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((Vehicle *)o); break;
  case RTID_SpaceDriver: driverList->Drop((SpaceDriver *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceDriver *)o); break;
  case RTID_LifeformInstance: lifeformList->Drop((LifeformInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((LifeformInstance *)o); break;
  case RTID_SpaceTrooperInstance: trooperList->Drop((SpaceTrooperInstance *)o); if ( !ignorePlayerReferences ) player->RemoveReferences((SpaceTrooperInstance *)o); break;
  default: break;
 }
 if ( fx.ListIteminList(o) ) fx.Drop(o);
}

GameObject *GameWorldDatabase::GetGenericByReference( const char *s ) {
 // to be determined
 return null;
}

void GameWorldDatabase::Add( SpaceSign *ss ) {
 signList->Add(ss);
 fx.Add(ss);
}

void GameWorldDatabase::Add( SpaceDriver *sd ) {
 driverList->Add(sd);
 fx.Add(sd);
}

void GameWorldDatabase::Add( Vehicle *vh ) {
 vehicleList->Add(vh);
 fx.Add(vh);
}

void GameWorldDatabase::Add( CelestialInstance *ci ) {
 if ( ci->canBeStruck ) celestialHittable->Add(ci);
 else celestialList->Add(ci);
 fx.Add(ci);
}

void GameWorldDatabase::Add( ThingInstance *ti ) {
 thingList->Add(ti);
 fx.Add(ti);
}

void GameWorldDatabase::Add( SpaceTrooperInstance *st ) {
 trooperList->Add(st);
 fx.Add(st);
}

void GameWorldDatabase::Add( LifeformInstance *st ) {
 lifeformList->Add(st);
 fx.Add(st);
}

void GameWorldDatabase::Remove( SpaceSign *ss, bool completely ) {
 signList->Drop(ss);
 fx.Remove(ss);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(ss);
}

void GameWorldDatabase::Remove( SpaceDriver *sd, bool completely ) {
 driverList->Drop(sd);
 fx.Remove(sd);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(sd);
}

void GameWorldDatabase::Remove( Vehicle *vh, bool completely ) {
 vehicleList->Drop(vh);
 fx.Remove(vh);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(vh);
}

void GameWorldDatabase::Remove( CelestialInstance *ci, bool completely ) {
 celestialList->Drop(ci);
 celestialHittable->Drop(ci);
 fx.Remove(ci);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(ci);
}

void GameWorldDatabase::Remove( ThingInstance *ti, bool completely ) {
 thingList->Drop(ti);
 fx.Remove(ti);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(ti);
}

void GameWorldDatabase::Remove( SpaceTrooperInstance *st, bool completely ) {
 trooperList->Drop(st);
 fx.Remove(st);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(st);
}

void GameWorldDatabase::Remove( LifeformInstance *st, bool completely ) {
 lifeformList->Drop(st);
 fx.Remove(st);
 if ( completely ) EACH(activeFarWorldStorage.first,FarWorldStorageHandle,fwsh) fwsh->p->Remove(st);
}

void GameWorldDatabase::MoveBy( double dx, double dy ) {
 bullets->MoveBy(dx,dy);
 fx.MoveBy(dx,dy);
}

void GameWorldDatabase::Between() {
 fx.Between();
 if ( bullets ) bullets->Between();
}

#include "BuildBases.h"

void GameWorldDatabase::Render( BuiltBases *bb ) {
 EACH(celestialList->first,CelestialInstanceHandle,cih) cih->p->Render();
 if ( bb && !bb->planetarySurface ) bb->RenderBackground();
 EACH(signList->first,SpaceSignHandle,ssh) ssh->p->Render();
 EACH(celestialHittable->first,CelestialInstanceHandle,cih) cih->p->Render();
 EACH(vehicleList->first,VehicleHandle,vh) vh->p->Render();
 EACH(driverList->first,SpaceDriverHandle,sdh) sdh->p->Render();
 if ( !renderEmissive ) { EACH(thingList->first,ThingInstanceHandle,tih) tih->p->Render(); }
 else { EACH(thingList->first,ThingInstanceHandle,tih) { tih->p->Render(); tih->p->RenderEmissive(false); } }
 EACH(trooperList->first,SpaceTrooperInstanceHandle,stih) stih->p->Render();
 EACH(lifeformList->first,LifeformInstanceHandle,lfih) lfih->p->Render();
 //bullets->Render();
}

void GameWorldDatabase::Destructor() {
 Dispose();
}