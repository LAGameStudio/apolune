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
#include "ImageCartesiansEditor.h"
#include "SoundEffect.h"
#include "Vehicle.h"
#include "AtlasTileset.h"

enum ThingTypes {
 tt_Junk=0,
 tt_Medical=1,
 tt_Ore=2,
 tt_Commodity=3,
 tt_Artifact=4,
 tt_Biology=5,
 tt_Weapon=6,
 tt_Ammo=7,
 tt_Disc=8,
 tt_Implant=9,
 tt_Parts=10,
 tt_Food=11,
 tt_Oxygen=12
};

class Campaign;
ONE(Thing,{
 type=tt_Junk;
 tint.Pick(alabaster);
 closeuptint.Pick(alabaster);
})
 Zp<Campaign>campaign;
 Zbool unique,vessel;
 Zint level,containerLimit,secondaryType,uses;
 ThingTypes type;
 Zdouble value,mass,rarity;
 SoundEffect sounds;
 Zp<GLImage> image,closeup,height,emission,opened,openedheight,damaged,destroyed;
 Zp<Film> foverlay,femission,fopening;
 Zstring named,described,script;
 Crayon tint,closeuptint;
 Zbool solid,wall,metal,wood,glass,electrical,explosive,holdable,persist,gettable;
 Cartesians collider;
 VehicleLights lights;
 Zbool door,leftRight,closesAutomatically;
 Zp<AtlasTileset> ats;
 Zp<AtlasTile> at;
 Zstring atlasTilesetName,atlasTileName;
 Zbool atlas2X,atlas4X;
 string GetReference() {
  return string("thing/")+name;
 }
 static void DeployTypes( Strings *s ) {
  s->Add("Junk",0);
  s->Add("Medical Supply",1);
  s->Add("Ore",2);
  s->Add("Commodity",3);
  s->Add("Artifact",4);
  s->Add("Biological Sample",5);
  s->Add("Weapon",6);
  s->Add("Ammunition",7);
  s->Add("Data Disc",8);
  s->Add("Cybernetic Implant",9);
  s->Add("Repair Part",10);
  s->Add("Food",11);
  s->Add("Oxygen",12);
 }
 void Set( Thing *in ) {
  unique=in->unique;
  vessel=in->vessel;
  level=in->level;
  containerLimit=in->containerLimit;
  type=in->type;
  secondaryType=in->secondaryType;
  uses=in->uses;
  value=in->value;
  mass=in->mass;
  rarity=in->rarity;
  sounds.Set(&in->sounds);
  image=in->image;
  closeup=in->closeup;
  height=in->height;
  emission=in->emission;
  opened=in->opened;
  openedheight=in->openedheight;
  named=in->named;
  described=in->described;
  tint.fromCrayon(in->tint);
  closeuptint.fromCrayon(in->closeuptint);
  solid=in->solid;
  wall=in->wall;
  metal=in->metal;
  wood=in->wood;
  glass=in->glass;
  electrical=in->electrical;
  lights.Duplicate(&in->lights);
  collider.Duplicate(&in->collider);
  foverlay=in->foverlay;
  femission=in->femission;
  fopening=in->fopening;
  explosive=in->explosive;
  holdable=in->holdable;
  door=in->door;
  leftRight=in->leftRight;
  closesAutomatically=in->closesAutomatically;
  gettable=in->gettable;
  at=in->at;
  ats=in->ats;
  atlasTilesetName=in->atlasTilesetName;
  atlasTileName=in->atlasTileName;
  atlas2X=in->atlas2X;
  atlas4X=in->atlas4X;
 }
 KEYWORDS({
  KEYWORD("name",{name=w;})
  else TXTWORD("atSet",atlasTilesetName)
  else TXTWORD("atTile",atlasTileName)
  else TAGWORD("unique",unique)
  else TAGWORD("vessel",vessel)
  else NUMWORD("level",level)
  else NUMWORD("containerLimit",containerLimit)
  else NUMWORD("secondary",secondaryType)
  else NUMWORD("uses",uses)
  else ENUMWORD("type",type,ThingTypes)
  else NUMWORD("value",value)
  else NUMWORD("mass",mass)
  else NUMWORD("rarity",rarity)
  else SUBWORD("sounds",sounds)
  else IMGWORD("image",image)
  else IMGWORD("closeup",closeup)
  else IMGWORD("height",height)
  else IMGWORD("opened",opened)
  else IMGWORD("openedheight",openedheight)
  else IMGWORD("emission",emission)
  else IMGWORD("damaged",damaged)
  else IMGWORD("destroyed",destroyed)
  else TXTWORD("named",named)
  else TXTWORD("described",described)
  else TXTWORD("script",script)
  else TINTWORD("tint",tint)
  else TINTWORD("closeuptint",closeuptint)
  else TAGWORD("solid",solid)
  else TAGWORD("wall",wall)
  else TAGWORD("metal",metal)
  else TAGWORD("wood",wood)
  else TAGWORD("glass",glass)
  else TAGWORD("electrical",electrical)
  else SUBWORD("boxes",collider)
  else SUBWORD("lights",lights)
  else FILMWORD("fover",foverlay)
  else FILMWORD("femis",femission)
  else FILMWORD("fop",fopening)
  else TAGWORD("explosive",explosive)
  else TAGWORD("holdable",holdable)
  else TAGWORD("door",door)
  else TAGWORD("leftRight",leftRight)
  else TAGWORD("closes",closesAutomatically)
  else TAGWORD("gettable",gettable)
  else TAGWORD("a2x",atlas2X)
  else TAGWORD("a4x",atlas4X)
  else BADKEY("Thing")
 })
 WORDKEYS({
  WORDKEY("name",name)
  TAGKEY("unique",unique)
  TAGKEY("vessel",vessel)
  NUMBKEY("level",level)
  NUMBKEY("containerLimit",containerLimit)
  NUMBKEY("secondary",secondaryType)
  NUMBKEY("uses",uses)
  ENUMKEY("type",type)
  NUMBKEY("value",value)
  NUMBKEY("mass",mass)
  NUMBKEY("rarity",rarity)
  SSUBKEY("sounds",sounds)
  IMGKEY("image",image)
  IMGKEY("closeup",closeup)
  IMGKEY("height",height)
  IMGKEY("emission",emission)
  IMGKEY("opened",opened)
  IMGKEY("openedheight",openedheight)
  IMGKEY("damaged",damaged)
  IMGKEY("destroyed",destroyed)
  TXTKEY("named",named)
  TXTKEY("described",described)
  TXTKEY("script",script)
  TINTKEY("tint",tint)
  TINTKEY("closeuptint",closeuptint)
  TAGKEY("solid",solid)
  TAGKEY("wall",wall)
  TAGKEY("metal",metal)
  TAGKEY("wood",wood)
  TAGKEY("glass",glass)
  TAGKEY("electrical",electrical)
  SSUBKEY("boxes",collider)
  SSUBKEY("lights",lights)
  FILMKEY("fover",foverlay)
  FILMKEY("femis",femission)
  FILMKEY("fop",fopening)
  TAGKEY("explosive",explosive)
  TAGKEY("holdable",holdable)
  TAGKEY("door",door)
  TAGKEY("leftRight",leftRight)
  TAGKEY("closes",closesAutomatically)
  TAGKEY("gettable",gettable)
  TAGKEY("a2x",atlas2X)
  TAGKEY("a4x",atlas4X)
  if ( at ) {
   TXTKEY("atSet",atlasTilesetName)
   TXTKEY("atTile",atlasTileName)
  }
 })
 CLONE(Thing,{
  DUPE(unique)
  DUPE(vessel)
  DUPE(level)
  DUPE(containerLimit)
  DUPE(type)
  DUPE(value)
  DUPE(mass)
  DUPE(rarity)
  a->sounds.Set(&sounds);
  DUPE(solid)
  DUPE(wall)
  DUPE(metal)
  DUPE(wood)
  DUPE(glass)
  a->collider.Duplicate(&collider);
  a->lights.Duplicate(&lights);
  DUPE(image)
  DUPE(closeup)
  DUPE(height)
  DUPE(emission)
  DUPE(opened)
  DUPE(openedheight)
  DUPE(damaged)
  DUPE(destroyed)
  DUPE(closeuptint)
  DUPE(script)
  DUPE(tint)
  DUPE(named)
  DUPE(described)
  DUPE(secondaryType)
  DUPE(foverlay)
  DUPE(femission)
  DUPE(fopening)
  DUPE(explosive)
  DUPE(holdable)
  DUPE(door)
  DUPE(leftRight)
  DUPE(closesAutomatically)
  DUPE(gettable)
  DUPE(ats)
  DUPE(at)
  DUPE(atlasTilesetName)
  DUPE(atlasTileName)
  DUPE(atlas2X)
  DUPE(atlas4X)
 })  
 void SetAtlasTile( AtlasTileset *ats, AtlasTile *at ) {
  if ( !ats || !at ) { ats=null; at=null; return; }
  this->ats=ats;
  this->atlasTilesetName=ats->name;
  this->at=at;
  this->atlasTileName=at->name;
 }
 void GetReferencedTile() {
  if ( atlasTilesetName.length > 0 && atlasTileName.length > 0 ) {
   ats=atlasTilesets.GetTileset(atlasTilesetName.c_str());
   at=atlasTilesets.GetTile(ats,atlasTileName.c_str());
  }
 }
MANY(Thing,ThingHandle,ThingHandles,"thing",Things,{})
 CALLEACH(Thing,GetReferencedTile);
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, ThingHandles *out );
 KEYSWORDSGroups("thing",Thing)
 CLONES(Thing,Things)
DONE(Thing);

extern Things things;