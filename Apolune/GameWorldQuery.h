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

#include "Proce55ors.h"

class BuiltBases;

class GameObject;
class GameObjectHandles;

class SoundEffect;

class BulletInstancer;

class ListItem;
class ListItemHandles;

enum SpaceSignPurposes;
class SpaceSign;
class SpaceSignHandles;

class ThingInstance;
class ThingInstanceHandles;

class SpaceTrooperInstance;
class SpaceTrooperInstanceHandles;

class CelestialInstance;
class CelestialInstanceHandles;

enum SpaceCarAITypes;
class SpaceDriver;
class SpaceDriverHandles;

class Vehicle;
class VehicleHandles;
class AirlockState;

class NavigationController;

class LifeformInstance;
class LifeformInstanceHandles;

class GameWorldQuery
{
public:
 static bool isFriendOf( bool iAmEnemy, bool theyAreEnemy );
 static SpaceSign *FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList );
 static SpaceSign *FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindNearestSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindNearestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindNearestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose );
 static SpaceSign *FindFarthestSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, int decisionSeed=0 );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed=0 );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, SpaceSign *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed=0 );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, SpaceSignHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed=0 );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, SpaceSignPurposes purpose, int decisionSeed=0 );
 static SpaceSign *FindRandomSign( ListItem *self, NavigationController *nav, ListItemHandles *notIncluding, SpaceSignHandles *signList, int decisionSeed=0 );
 static SpaceDriver *FindNearestSpaceCarEnemy( ListItem *self, NavigationController *nav, SpaceDriverHandles *driverList, bool iAmEnemy );
 static SpaceDriver *FindNearestSpaceCarEnemy( ListItem *self, NavigationController *nav, SpaceDriverHandles *driverList, SpaceCarAITypes type, bool iAmEnemy );
 static Vehicle *FindNearestVehicleEnemy( ListItem *self, NavigationController *nav, VehicleHandles *vehicleList, bool iAmEnemy );
 static Vehicle *FindNearestVehicleEnemy( ListItem *self, NavigationController *nav, ListItem *excepting, VehicleHandles *vehicleList, bool iAmEnemy );
 static AirlockState *AirlockOn( Vehicle *v );
 static void ScreenRelativeGain( const char *soundfx, double x, double y, bool one_at_a_time=false );
 static void PlayPlayerRelative( NavigationController *nav, SoundEffect *sfx );
};

class LocationMarker;
class SpaceGameView;
class InteriorGameView;
class GameWorldDatabase {
public:
 GameWorldDatabase() { Init(); }
 void Init();
 void Clear() { Dispose(); }
 void Register( SpaceGameView *sgv );
 void Register( InteriorGameView *igv );
 void Deregister( SpaceGameView *sgv );
 void Deregister( InteriorGameView *igv );
 FarWorldStorageHandles activeFarWorldStorage;
 Proce55orsHandles proce55orLists;
 Zdis_manual<SpaceSignHandles> signList;
 Zdis_manual<SpaceDriverHandles> driverList;
 Zdis_manual<VehicleHandles> vehicleList;
 Zdis_manual<CelestialInstanceHandles> celestialList;
 Zdis_manual<CelestialInstanceHandles> celestialHittable;
 Zdis_manual<ThingInstanceHandles> thingList;
 Zdis_manual<SpaceTrooperInstanceHandles> trooperList;
 Zdis_manual<BulletInstancer> bullets;
 Zdis_manual<LifeformInstanceHandles> lifeformList;
 Zdis_manual<LifeformInstanceHandles> lifeformHittable;
 Zbool renderEmissive;
 Proce55ors fx;
 Zp<Proce55ors> effects;
 void Dispose();
 void Find( LocationMarker *marker, GameObjectHandles *out );
 void Find( GameObject *self, ThingInstanceHandles *out, double radius=128.0 );
 void Add( SpaceSign *ss );
 void Add( SpaceDriver *sd );
 void Add( Vehicle *vh );
 void Add( CelestialInstance *ci );
 void Add( ThingInstance *ti );
 void Add( SpaceTrooperInstance *st );
 void Add( LifeformInstance *li );
 void Add( GameObject *o );
 void Remove( SpaceSign *ss, bool completely=false );
 void Remove( SpaceDriver *sd, bool completely=false );
 void Remove( Vehicle *vh, bool completely=false );
 void Remove( CelestialInstance *ci, bool completely=false );
 void Remove( ThingInstance *ti, bool completely=false );
 void Remove( SpaceTrooperInstance *st, bool completely=false );
 void Remove( LifeformInstance *li, bool completely=false );
 void Remove( GameObject *o, bool ignorePlayerReferences );
 void RemoveCompletely( GameObject *o, bool ignorePlayerReferences=false );
 GameObject *RemoveCompletely( const char *s, bool ignorePlayerReferences=false );
 GameObject *GetGenericByReference( const char *s );
 void MoveBy( double dx, double dy );
 void Between();
 void Render( BuiltBases *bb=null );
 void Destructor();
 ~GameWorldDatabase() { Destructor(); }
};

extern GameWorldDatabase wdb;