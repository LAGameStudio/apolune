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

#include "Delay.h"
#include "Cartesian.h"

#include "Mulligan.h"

class VehicleModule;
class VehicleWeapon;

////////////////////////////////////////////////
// Emission types refers to points on a vehicle
// module that emit smoke, or on a weapon where
// it fires out of, etc.
////////////////////////////////////////////////
enum EmissionTypes {
 emi_Thrust=0,      // For forward thrust
 emi_DirectionalThrust=1,  // For RCS effect
 emi_Beam=2,        // For "beam" weapons
 emi_PointBeam=3,   // For point defense
 emi_Projectile=4,  // For "Bullet" guns
 emi_Particle=5,    // For particle emittors
 emi_Smoke=6,       // Particles emitted "sometimes"
 emi_Ordinance=7    // For shell-launchers, where and what direction the ordinance is stored.
};
ONE(EmissionPoint,{type=emi_Particle; rate=30.0; burstRate=0.1; intermittant=true; health=1.0; blend=additive; tint.x_White(); offsetAngle=180.0; })
 EmissionTypes type;
 Cartesiand location;
 Zbool intermittant, // Intermittant: not tied to firing or thrusting 
  supplementary; // Supplementary: for beams, to create a combination beam
 Mulligan mulligan;
 Zdouble angle,rate,burstRate; // rate is used for emi_Smoke, emi_Particle, burst rate is a length of time
 Zdouble width1,width2,height;  // width1 and width2 are for the isotrap of thrust, height is also for isotrap of thrust.
 Zdouble health;
 Zp<Film> film; Zstring filmLibraryString,filmString;
 Blends blend;
 Crayon tint;
 Zdouble offsetAngle; // Lets you adjust the film angle in the case that the film is recorded at an odd angle.
 /// what's been mounted here.
 Zp<VehicleModule> installed;
 Zp<VehicleWeapon> mounted;
 CLONE(EmissionPoint,{
  DUPE(name)
  DUPE(type)
  DUPE(location)
  DUPE(intermittant)
  DUPE(supplementary)
  a->mulligan.Set(&mulligan);
  DUPE(angle)
  DUPE(offsetAngle)
  DUPE(rate)
  DUPE(burstRate)
  DUPE(width1)
  DUPE(width2)
  DUPE(height)
  DUPE(health)
  DUPE(film)
  DUPE(filmString)
  DUPE(filmLibraryString)
  a->tint.fromCrayon(tint);
  DUPE(blend)
 })
 KEYWORDS({
  KEYWORD("type",{type=(EmissionTypes)atoi(w);})
  else KEYWORD("location",{location.fromString(w);})
  else KEYTAG("intermittant",{intermittant=true;})
  else KEYTAG("supplementary",{supplementary=true;})
  else KEYWORD("mulligan",{mulligan.fromString(w);})
  else KEYWORD("angle",{angle=w;})
  else KEYWORD("offsetAngle",{offsetAngle=w;})
  else KEYWORD("rate",{rate=w;})
  else KEYWORD("burstRate",{burstRate=w;})
  else KEYWORD("width1",{width1=w;})
  else KEYWORD("width2",{width2=w;})
  else KEYWORD("height",{height=w;})
  else KEYWORD("health",{health=w;})
  else KEYWORD("filmLibrary",{filmLibraryString=w;})
  else KEYWORD("film",{filmString=w; film=vault.find(filmLibraryString.c_str(),w);})
  else KEYWORD("tint",{tint.CSS(w);})
  else KEYWORD("blend",{blend.fromString(w);})
 })
 WORDKEYS({
  WORDKEY("type",(int)type)
  SSUBKEY("location",location)
  TAGKEY("intermittant",intermittant)
  TAGKEY("supplementary",supplementary)
  SSUBKEY("mulligan",mulligan)
  WORDKEY("angle",angle)
  WORDKEY("offsetAngle",offsetAngle)
  WORDKEY("rate",rate)
  WORDKEY("burstRate",burstRate)
  WORDKEY("width1",width1)
  WORDKEY("width2",width2)
  WORDKEY("height",height)
  WORDKEY("health",health)
  if ( film ) {
   FilmLibrary *L=film->GetLibrary();
   WORDKEY("filmLibrary",L->title.c_str())
   WORDKEY("film",film->title.c_str())
  }
  SSUBKEYCustom("tint",tint,toCSS)
  SSUBKEY("blend",blend)
 })
MANY(EmissionPoint,EmissionPointHandle,EmissionPointHandles,"EmissionPoint",EmissionPoints,{})
 KEYWORDSGroup("EmissionPoint",EmissionPoint);
 WORDKEYSGroup("EmissionPoint",EmissionPoint);
 static void DeployTypes( Strings *s ) {  
  s->Add("Thrust");
  s->Add("Directional RCS");
  s->Add("Beam Emitter");
  s->Add("Point Beam");
  s->Add("Projectile");
  s->Add("Particle");
  s->Add("Smoke");
  s->Add("Ordinance");
 }
 CLONES(EmissionPoint,EmissionPoints)
DONE(EmissionPoint);