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
#pragma once

#include "GameObject.h"
#include "VehicleWeaponBlueprint.h"
#include "VehicleWeapon.h"
#include "Crew.h"

class Vehicle;
class VehicleModule;
class AttachmentPoint;

////////////////////////////////////////////////
// A slot onto which a weapon can be loaded,
// contained in a vehicle module.
////////////////////////////////////////////////
enum WeaponSlotTypes {
 ws_Any=0,
 ws_Gun=1,
 ws_Missile=2,
 ws_Mine=3,
 ws_Shield=4
};

ONE(WeaponSlot,{type=ws_Any;})
 Zp<VehicleWeaponBlueprint> mounts; Zstring mountsString;
 Zdis<VehicleWeapon> mounted;
 WeaponSlotTypes type;
 Cartesiand location;
 CrewSlots crewSlots;
 Zint ammo;
 ///
 Zdouble time;
 Zbool reloading;
 Delay reloadDelay; 
 Zdouble facingAngle,aimingAngle,turningSpeed;
 KEYWORDS_debugStart {
  ENUMWORD("type",type,WeaponSlotTypes)
  else TXTWORD("name",name)
  else KEYWORD("location",{location.fromString(w);})
  else KEYWORD("crewslots",{crewSlots.fromString(w);})
  else KEYWORD("mounted",{
   mounted.Recycle();
   mounted->fromString(w);
  })
  else KEYWORD("mounts",{mountsString=w;})
  else NUMWORD("ammo",ammo)
  else BADKEY(identity().c_str())
  NEXTWORD
 }
 KEYWORDS_debugEnd
 WORDKEYS_debugStart {
  WORDKEY("type",(int)type)
  TXTKEY("name",name)
  SSUBKEY("location",location)
  SSUBKEY("crewslots",crewSlots)
  if ( mounted ) SUBKEY("mounted",mounted)
  if ( mounts ) WORDKEY("mounts",mounts->name)
  NUMBKEY("ammo",ammo)
 }
 WORDKEYS_debugEnd
 CLONE(WeaponSlot,{
  DUPE(name);
  DUPE(type);
  DUPE(location);
  a->crewSlots.Duplicate(&crewSlots);
  DUPE(ammo);
  DUPE(mounts);
  DUPE(mountsString);
 })
 void ParseReferences() {
  mounts=veh_wbs.firstNamed(mountsString.c_str());
  if ( mounted ) mounted->ParseReferences();
 }
 // Creates and installs a weapon, replacing existing.
 VehicleWeapon *Install( VehicleWeaponBlueprint *vwb ) {
  return Install(new VehicleWeapon(vwb));
 }
 // Installs a weapon, replacing existing.
 VehicleWeapon *Install( VehicleWeapon *vw ) {
  if ( vw == null ) {
   VehicleWeapon *dvw=null;
   VehicleWeaponBlueprint *mounting=mounts?mounts:(mounts=veh_wbs.firstNamed(mountsString.c_str()));
   if ( mounting != null ) {
    mounted.Recycle(dvw=new VehicleWeapon(mounting));
   return dvw;
   } else {
    return null;
   }
  }
  mounted.Recycle(vw);
  return vw;
 }
 void Fire( Vehicle *v, VehicleModule *weapon );
 void Fire( Vehicle *v, VehicleModule *vm, AttachmentPoint *ap, VehicleModule *vm2 );
MANY(WeaponSlot,WeaponSlotHandle,WeaponSlotHandles,"WeaponSlot",WeaponSlots,{})
 CALLEACH(WeaponSlot,ParseReferences);
 KEYWORDSGroup("WeaponSlot",WeaponSlot);
 WORDKEYSGroup("WeaponSlot",WeaponSlot);
 BINARY_INOUT(WeaponSlot)
 void Fire( Vehicle *v, VehicleModule *weapon );
 void Fire( Vehicle *v, VehicleModule *vm, AttachmentPoint *ap, VehicleModule *vm2 );
 static void DeployTypes( Strings *s ) {
  s->Add("Any");
  s->Add("Gun");
  s->Add("Missile");
  s->Add("Mine");
  s->Add("Shield");
 }
 CLONES(WeaponSlot,WeaponSlots)
DONE(WeaponSlot);