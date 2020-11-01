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
/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "Proce55or.h"
#include "Stats.h"
#include "Commodity.h"
#include "GLTransform.h"
#include "Art2d.h"
#include "CollisionProfile.h"
#include "Numbers.h"
#include "Delay.h"
#include "ScriptSection.h"
#include "ItemContentTree.h"

struct SpritePivot {
public:
 Zdouble angle;
 Zdouble minAngle,maxAngle;
 Zdouble turnSpeed;
 KEYWORDS({
  KEYWORD("angle",{angle=w;})
  else KEYWORD("minAngle",{minAngle=w;})
  else KEYWORD("maxAngle",{maxAngle=w;})
  else KEYWORD("turnSpeed",{turnSpeed=w;})
 })
 WORDKEYS({
  WORDKEY("angle",angle)
  WORDKEY("min",minAngle)
  WORDKEY("max",maxAngle)
  WORDKEY("turn",turnSpeed)
 })
};

enum GameObjectTypes {
 got_Vehicle=0,
 got_DroneFighter=1,
 got_Projectile=2,
 got_FX=3,
 got_Item=4,
 got_Creature=5,
 got_Prop=6
};

class Campaign;
class GameWorld;
class GameObjects;
class GameObjectHandles;
class LocationMarker;
C_ONE(GameObject,Proce55or,{ health=1.0; type=got_Prop; })
 Zp<Campaign> campaign;
 GameObjectTypes type;
 Zp<GameObject> owner;
 Zp<LocationMarker> marker;
 Skills skills;
 Stats stats,bonuses;
 Zdouble facing;
 Zdouble density,value;
 Zdouble health,healed;
 GLTransform transform;
 Zbool persistent,destroyMe,alive;
 Box box;
 Zdisposable<CollisionProfile> collision;  // If not instantiated, object does not collide
 ScriptSections scriptSections;
 ItemContentTree contents;
 Zint pickability;              // Temporary variables set by LocationMarker scripts
 Zbool lockable,closeable,locked,closed;  // for locking/unlocking/open/close
 Zbool switchable,activated; // for switches and levers
 Zstring requires_key,as_key;   // and pickability
 Zbool enemy;
 void Contains(int qty,const char *reference);
 bool isKey() { return as_key.length > 0; }
 bool hasLock() { return locked || requires_key.length > 0; }
 bool isOpen() { return !closed; }
 bool isCloseable() { return closeable; }
 void Activate( ScriptLine *sl ) {
  activated=true;
  switchable=true;
 }
 void Deactivate( ScriptLine *sl ) {
  activated=false;
  switchable=true;
 }
 void AsKey( ScriptLine *sl ) {
  as_key=sl->Word2();
 }
 void Lock( ScriptLine *sl ) {
  if ( sl->SecondWord() ) requires_key=sl->Word2();
  if ( sl->ThirdWord() ) pickability=sl->Integer3();
  locked=true;
  lockable=true;
  closeable=true;
  closed=true;
 }
 void Unlock( ScriptLine *sl ) { locked=false; }
 void Close( ScriptLine *sl ) { closed=true; closeable=true; }
 bool Open( ScriptLine *sl ) {
  if ( !locked ) { closed=false; return true; }
  else return false;
 }
 void Enemy( ScriptLine *sl ) { enemy=true; }
 void Friend( ScriptLine *sl ) { enemy=false; }
 virtual void Announced( const char *announcement ) {}
 /*
 virtual void BirthScript( ScriptSection *lines ) {}    // Called at start
 virtual void DeathScript( ScriptSection *lines ) {}    // Called when destroyed
 virtual void HailScript( ScriptSection *lines ) {}     // Called when hailed
 virtual void CollectScript ( ScriptSection *lines ) {} // Called when collected
 virtual void UseScript( ScriptSection *lines ) {}      // Called when used
 */
 virtual bool isEnemyOfPlayer() { return enemy; }
 bool ExecuteScript( ScriptSection *ss );
 bool ExecuteScript( const char *section="" ) {
  EACH(scriptSections.first,ScriptSection,ss) if ( !str_cmp(ss->name.c_str(),section) ) {
   bool result=ExecuteScript(ss); if ( result ) return result;
  }
 }
 virtual double Density( double factor=1.0 ) {
  return 0.0;
 }
 virtual double Value( double factor=1.0 ) { // Returns a base fiscal value
  return 0.0;
 }
 void Hurt( double amount ) {
  health-=amount;
  if ( health < 0.0 ) {
   Die();
  }
 }
 // Called when hurt to death
 virtual void Die() {
  alive=false;
 }
 void OnStore( int i, int j );
 void OnRetrieve();
 // Called when the object is stored in the "world database" like in the contents of something, and not actively animated in the GameView.
 virtual void StoredBetween() {}
 // Called when the object is created (copied) from a pristine version to a non-pristine version.
 virtual void OnImplement() {}
 void Implement();
 float GetSoundGain( double sound_too_far=512.0 );
MANY(GameObject,GameObjectHandle,GameObjectHandles,"o",GameObjects,{})
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, GameObjectHandles *out );
DONE(GameObject);