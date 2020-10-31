#pragma once

#include "Icons.h"
#include "GameObject.h"

#include "Crew.h"
#include "SpaceCar.h"

////////////////////////////////////////////////
// A slot onto which a utility can be loaded,
// contained in a vehicle module.
////////////////////////////////////////////////
enum UtilitySlotTypes {
 us_None=0,           // None is reserved for mount info.
 us_Cargo=1,          // Contains cargo
 us_ColonyPod=2,      // Colonial supply
 us_Science=3,        // Research
 us_LandingPad=4,     // For cars to land on
 us_Drone=5,          // Drone hanger for escape pods, cryo pods, fighters, robots and probes
 us_Repair=6,         // Repair ray
 us_Tractorbeam=7,    // Tractor beam
 us_MiningLaser=8,    // Mining beam
 us_ShieldGenerator=9,// A shield generator
 us_Constructor=10    // Constructor
};

ONE(UtilitySlot,{type=us_None;mounted=us_None;health=1.0;})
 Zp<SpaceCar> docked; Zstring dockedString;
 UtilitySlotTypes type,mounted;
 Zdouble strength,health;
 Cartesiand location;
 CrewSlots crewSlots;
 Delay launchDelay;
 BINARY_IN({
  B_IN_STRING(name)
  B_IN_ENUM(UtilitySlotTypes,type)
  B_IN_ENUM(UtilitySlotTypes,mounted)
  B_IN_NUM(strength)
  B_IN_NUM(health)
  B_IN_SSUB(location)
  B_IN_SSUB(crewSlots)
  B_IN_STRING(dockedString)
 })
 BINARY_OUT({
  B_OUT_STRING(name)
  B_OUT_ENUM(UtilitySlotTypes,type)
  B_OUT_ENUM(UtilitySlotTypes,mounted)
  B_OUT_NUM(strength)
  B_OUT_NUM(health)
  B_OUT_SSUB(location)
  B_OUT_SSUB(crewSlots)
  B_OUT_STRING(dockedString)
 })
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("type",{type=(UtilitySlotTypes)atoi(w);})
  else KEYWORD("mounted",{mounted=(UtilitySlotTypes)atoi(w);})
  else KEYWORD("strength",{strength=w;})
  else KEYWORD("health",{health=w;})
  else KEYWORD("location",{location.fromString(w);})
  else KEYWORD("crewslots",{crewSlots.fromString(w);})
  else KEYWORD("docked",{dockedString=w;})
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("type",(int)type)
  WORDKEY("mounted",(int)mounted)
  WORDKEY("strength",strength)
  WORDKEY("health",health)
  SSUBKEY("location",location)
  SSUBKEY("crewslots",crewSlots)
  if ( docked ) WORDKEY("docked",docked->name)
 })
 CLONE(UtilitySlot,{
  DUPE(name)
  DUPE(type)
  DUPE(mounted)
  DUPE(docked)
  DUPE(strength)
  DUPE(health)
  a->location.Set(&location);
  a->crewSlots.Duplicate(&crewSlots);
 })
 void ParseReferences() {
  docked=spacecars.firstNamed(dockedString.c_str());
 }
MANY(UtilitySlot,UtilitySlotHandle,UtilitySlotHandles,"UtilitySlot",UtilitySlots,{})
 CALLEACH(UtilitySlot,ParseReferences);
 KEYWORDSGroup("UtilitySlot",UtilitySlot);
 WORDKEYSGroup("UtilitySlot",UtilitySlot);
 BINARY_INOUT(UtilitySlot)
 static void DeployTypes( Strings *s ) {
  s->Add("None");
  s->Add("Cargo");
  s->Add("Colony Pod");
  s->Add("Science");
  s->Add("Spacecar Landing Pad");
  s->Add("Drone");
  s->Add("Repair");
  s->Add("Tractor Beam");
  s->Add("Mining Laser");
  s->Add("Shield Generator");
  s->Add("Construction");
 }
 static void DeployIcons( Icons *icons ) {
  icons->Add("data/images/icons/hardpoint_t.png"); // None is reserved for mount info.
  icons->Add("data/images/icons/smallcube.png"); // Contains cargo
  icons->Add("data/images/icons/planet.png"); // Colonial supply
  icons->Add("data/images/icons/papapishu_Lab_icon_1.png"); // Research
  icons->Add("data/images/icons/vehicles.png"); // For cars to land on
  icons->Add("data/images/icons/robot.png"); // Drone hanger for escape pods, cryo pods, fighters, robots and probes
  icons->Add("data/images/icons/johnny_automatic_Services_12.png"); // Repair ray
  icons->Add("data/images/icons/papapishu_Lab_icon_1.png"); // Tractor beam
  icons->Add("data/images/icons/Land_recreation.png"); // Mining beam
  icons->Add("data/images/icons/shield.png"); // A shield generator
  icons->Add("data/images/icons/SnarkHunter_Hammer_and_Nails.png"); // Constructor
 }
 CLONES(UtilitySlot,UtilitySlots)
DONE(UtilitySlot);