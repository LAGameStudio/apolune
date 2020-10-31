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

#include "Cartesian.h"

class Vehicle;
class VehicleModule;
class VehicleModuleBlueprint;

ONE(AttachmentPoint,{health=1.0;})
 Cartesiand location;
 Zdis<VehicleModule> module; Zstring moduleString;
 Zp<VehicleModuleBlueprint> moduleBlueprint; Zstring moduleBlueprintString;
 Zbool loose, // loose means the object acts as a loose flap that is acted upon by thrust (physics simulation)
  antispin;   // antispin spins the attached the other direction to keep them directionally-relative
 Zdouble spin,minAngle,angle,maxAngle; // Spin Speed
 Zdouble strength,health;
 void AttachModule();
 void AttachModule( VehicleModule *in );
 void AttachModule( VehicleModuleBlueprint *vmb );
 string GetModuleReference();
 string GetModuleBlueprintReference();
 bool HasModule();
 bool HasModuleBlueprint();
 CLONE(AttachmentPoint,{
  DUPE(name)
  a->location.Set(&location);
  DuplicateModule(a);
  DUPE(moduleString)
  DUPE(moduleBlueprint)
  DUPE(moduleBlueprintString)
  DUPE(loose)
  DUPE(antispin)
  DUPE(spin)
  DUPE(minAngle)
  DUPE(angle)
  DUPE(maxAngle)
  DUPE(strength)
  DUPE(health)
 })
 void DuplicateModule( AttachmentPoint *a );
 KEYWORDS({
  KEYWORD("location",{location.fromString(w);})
  else KEYWORD("module",{moduleString=w;})
  else KEYWORD("moduleBlueprint",{moduleBlueprintString=w;})
  else KEYTAG("loose",{loose=true;})
  else KEYTAG("antispin",{antispin=true;})
  else KEYWORD("spin",{spin=w;})
  else KEYWORD("minAngle",{minAngle=w;})
  else KEYWORD("angle",{angle=w;})
  else KEYWORD("maxAngle",{maxAngle=w;})
  else KEYWORD("strength",{strength=w;})
  else KEYWORD("health",{health=w;})
 })
 WORDKEYS({
  SSUBKEY("location",location)
  if ( HasModule() ) WORDKEY("module",GetModuleReference())
  if ( HasModuleBlueprint() ) WORDKEY("moduleBlueprint",GetModuleBlueprintReference())
  TAGKEY("loose",loose)
  TAGKEY("antispin",antispin)
  WORDKEY("spin",spin)
  WORDKEY("minAngle",minAngle)
  WORDKEY("angle",angle)
  WORDKEY("maxAngle",maxAngle)
  WORDKEY("strength",strength)
  WORDKEY("health",health)
 })
 void Fire( Vehicle *v, VehicleModule *vm );
MANY(AttachmentPoint,AttachmentPointHandle,AttachmentPointHHandles,"AttachmentPoint",AttachmentPoints,{})
 void Fire( Vehicle *v, VehicleModule *vm );
 void ParseReferences();
 KEYWORDSGroup("AttachmentPoint",AttachmentPoint);
 WORDKEYSGroup("AttachmentPoint",AttachmentPoint);
 CLONES(AttachmentPoint,AttachmentPoints);
DONE(AttachmentPoint);

