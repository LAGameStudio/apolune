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

#include "TextureLibrary.h"
#include "TweenBeamProfile.h"
#include "LayerCombinerProfile.h"
#include "Tween.h"
#include "SoundEffect.h"
#include "Mulligan.h"
#include "GameObject.h"

class Campaign;

enum MunitionTypes {
 mun_EnergyBullet=1,
 mun_Contiguous=2, // beam
 mun_Dashed=3, // 'dashed' beam
 mun_Missile=4 // Uses the missile image
};

////////////////////////////////////////////////
// Describes a bullet, beam, missile or mine.
////////////////////////////////////////////////
ONE(Munition,{
 lifespan=5.0;
 damage=0.1;
 thinkInterval=10.0;
 laserLineLength=64.0;
 bulletH=128.0;
 bulletW=128.0;
})
 Zp<Campaign> campaign; // Source campaign for keeping items relative
 Mulligan impactMulligan,detonationMulligan; // Describes what happens on impact, detonation
 Zdis<LayerCombinerProfile> bullet;
 Zdis<TweenBeamProfile> beam;
 Zp<GLImage> missile;
 Zstring design_name;
 Zdouble lifespan,damage,thinkInterval;
 Zbool guided,sticky,fragmentation,laserLines; // non-contiguous beams are line-lasers
 Zbool proximitySeek;
 Zdouble bulletH,bulletW,laserLineLength;
 Zdouble guidanceSkill,seekRadius,hitRadius;
 Zdouble turnSpeed,turnAccel,mass,force,speed,armor;
 Zp<Munition> fragment; Zstring fragmentReference;
 Zint fragments;
 Zdouble fragmentSpeed; // Speed at which fragments travel
 Zdouble fragmentLifeSpan; // Lifespan of fragments in seconds
 Zdouble fragmentSpin; // Speed at which fragments spin while flying
 Zdouble fragAngle; // What direction fragmentation occurs from path of travel
 Zdouble heatDamage,coldDamage,electricalDamage,radiativeDamage,concussiveDamage;
 SoundEffect metalImpactSound,stoneImpactSound,plasticImpactSound,ambient;
 double Value() {
  return 1.0+lifespan+laserLineLength+guidanceSkill
   +(guided?seekRadius:0.0)+hitRadius+turnSpeed
   +turnAccel+mass+force+speed+armor
   +(fragmentation?(fragments*(fragmentSpeed+fragmentLifeSpan+fragmentSpin+fragAngle)):0.0)
   +heatDamage+coldDamage+electricalDamage+radiativeDamage+concussiveDamage
   +thinkInterval+(proximitySeek?1.0:0.0)
   ;
 }
 KEYWORDS({
  TXTWORD("name",name)
  else TXTWORD("design_name",design_name)
  else KEYWORD("impactMulligan",{impactMulligan.fromString(w);})
  else KEYWORD("detonationMulligan",{detonationMulligan.fromString(w);})
  else KEYWORD("bullet",{bullet.Recycle(new LayerCombinerProfile); bullet->fromString(w);})
  else KEYWORD("beam",{beam.Recycle(new TweenBeamProfile); beam->fromString(w);})
  else KEYWORD("missile",{missile=library.Load(w);})
  else KEYWORD("lifespan",{lifespan=w;})
  else KEYWORD("damage",{damage=w;})
  else KEYWORD("thinkInterval",{thinkInterval=w;})
  else KEYTAG("guided",{guided=true;})
  else KEYTAG("sticky",{sticky=true;})
  else KEYTAG("fragmentation",{fragmentation=true;})
  else KEYTAG("laserLines",{laserLines=true;})
  else KEYTAG("proximitySeek",{proximitySeek=true;})
  else KEYWORD("bulletH",{bulletH=w;})
  else KEYWORD("bulletW",{bulletW=w;})
  else KEYWORD("laserLineLength",{laserLineLength=w;})
  else KEYWORD("guidanceSkill",{guidanceSkill=w;})
  else KEYWORD("seekRadius",{seekRadius=w;})
  else KEYWORD("hitRadius",{hitRadius=w;})
  else KEYWORD("turnSpeed",{turnSpeed=w;})
  else KEYWORD("turnAccel",{turnAccel=w;})
  else KEYWORD("mass",{mass=w;})
  else KEYWORD("force",{force=w;})
  else KEYWORD("speed",{speed=w;})
  else KEYWORD("armor",{armor=w;})
  else KEYWORD("fragment",{fragmentReference=w;})
  else KEYWORD("fragments",{fragments=w;})
  else KEYWORD("fragmentSpeed",{fragmentSpeed=w;})
  else KEYWORD("fragmentLifeSpan",{fragmentLifeSpan=w;})
  else KEYWORD("fragmentSpin",{fragmentSpin=w;})
  else KEYWORD("fragAngle",{fragAngle=w;})
  else KEYWORD("heatDamage",{heatDamage=w;})
  else KEYWORD("coldDamage",{coldDamage=w;})
  else KEYWORD("electricalDamage",{electricalDamage=w;})
  else KEYWORD("radiativeDamage",{radiativeDamage=w;})
  else KEYWORD("concussiveDamage",{concussiveDamage=w;})
  else KEYWORD("metal",{metalImpactSound.fromString(w);})
  else KEYWORD("stone",{stoneImpactSound.fromString(w);})
  else KEYWORD("plastic",{plasticImpactSound.fromString(w);})
  else KEYWORD("ambient",{ambient.fromString(w);})
  else BADKEY("Munition");
 });
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("design_name",design_name)
  SSUBKEY("impactMulligan",impactMulligan)
  SSUBKEY("detonationMulligan",detonationMulligan)
  if ( bullet ) WORDKEY("bullet",bullet->toString())
  if ( beam ) WORDKEY("beam",beam->toString())
  if ( missile ) WORDKEY("missile",missile->filename)
  WORDKEY("lifespan",lifespan)
  WORDKEY("damage",damage)
  WORDKEY("thinkInterval",thinkInterval)
  TAGKEY("guided",guided)
  TAGKEY("sticky",sticky)
  TAGKEY("fragmentation",fragmentation)
  TAGKEY("laserLines",laserLines)
  TAGKEY("proximitySeek",proximitySeek)
  WORDKEY("bulletH",bulletH)
  WORDKEY("bulletW",bulletW)
  WORDKEY("laserLineLength",laserLineLength)
  WORDKEY("guidanceSkill",guidanceSkill)
  WORDKEY("seekRadius",seekRadius)
  WORDKEY("hitRadius",hitRadius)
  WORDKEY("turnSpeed",turnSpeed)
  WORDKEY("turnAccel",turnAccel)
  WORDKEY("mass",mass)
  WORDKEY("force",force)
  WORDKEY("speed",speed)
  WORDKEY("armor",armor)
  WORDKEY("fragment",fragmentReference)
  WORDKEY("fragments",fragments.toString())
  WORDKEY("fragmentSpeed",fragmentSpeed)
  WORDKEY("fragmentLifeSpan",fragmentLifeSpan)
  WORDKEY("fragmentSpin",fragmentSpin)
  WORDKEY("fragAngle",fragAngle)
  WORDKEY("heatDamage",heatDamage)
  WORDKEY("coldDamage",coldDamage)
  WORDKEY("electricalDamage",electricalDamage)
  WORDKEY("concussiveDamage",concussiveDamage)
  SSUBKEY("metal",metalImpactSound)
  SSUBKEY("stone",stoneImpactSound)
  SSUBKEY("plastic",plasticImpactSound)
  SSUBKEY("ambient",ambient)
 })
MANY(Munition,MunitionHandle,MunitionHandles,"munition",Munitions,{})
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, MunitionHandles *out );
 void find(MunitionHandles *out,Strings *in) { EACH(in->first,String,s) { Munition *m=firstNamed(s->s.c_str()); if ( m ) out->Add(m); } }
 KEYWORDSGroup("munition",Munition);
 WORDKEYSGroup("munition",Munition);
DONE(Munition);
extern Munitions munitionTypes;

////////////////////////////////////////////////
// An instance of a munition after being fired
// from a weapon.  Contains firing reference,
// targetting information, speed, projectile,
// turning status, etc.
////////////////////////////////////////////////
C_ONE(Projectile,GameObject,{type=got_Projectile;})
 Projectile( Munition *m ) : GameObject() {
  munition=m;
  type=got_Projectile;
 }
 Zp<Campaign> campaign; // Source campaign for keeping items relative
 Zp<GameObject> firedBy,targetting;
 Zp<Munition> munition;
  Zdouble expired,thinkDelay;
 void Render() {
 }
 void Between() {
 }
MANY(Projectile,ProjectileHandle,ProjectileHandles,"p",Projectiles,{})
DONE(Projectile);
