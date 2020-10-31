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

#include "Strings.h"
#include "GLImage.h"

enum ItemTypes {
 it_none=0,
 it_spacecar=1,
 it_vehicle=2,
 it_thing=3,
 it_commodity=4
};

class Campaign;
class Commodity;
class Thing;
class ThingHandles;
enum ThingTypes;
class SpaceCar;
class Vehicle;
class VehicleBlueprint;
class ItemContentTree;
class ItemContentTreeHandles;
ONE(ItemContentNode,{
 Construct();
 item=it_none;
})
 Zp<Campaign> campaign;
 Zstring reference;
 Zint quantity;
 Zstring named;
 Zint seed;
 Zstring script;
 Zstring unpack;
 ItemTypes item;
 KEYWORDS({
  KEYWORD("c",{LoadCampaignString(w);})
  else TXTWORD("ref",reference)
  else NUMWORD("q",quantity)
  else TXTWORD("named",named)
  else NUMWORD("s",seed)
  else TXTWORD("sc",script)
  else ENUMWORD("it",item,ItemTypes)
  else TXTWORD("unpack",unpack)
 })
 WORDKEYS({
  WORDKEY("c",SaveCampaignString())
  TXTKEY("ref",reference)
  NUMBKEY("q",quantity)
  TXTKEY("named",named)
  NUMBKEY("s",seed)
  TXTKEY("sc",script)
  ENUMKEY("it",item)
  TXTKEY("unpack",unpack)
 })
 void from( Vehicle *c );
 void from( Commodity *c );
 void from( SpaceCar *c );
 void from( Thing *c );
 bool isCommodity() { return item == it_commodity; }
 bool isSpaceCar() { return item == it_spacecar; }
 bool isVehicle() { return item == it_vehicle; }
 bool isThing() { return item == it_thing; }
 Commodity *GetCommodity();
 SpaceCar *GetSpaceCar();
 Vehicle *GetVehicle();
 Thing *GetThing();
 void Construct();
 string SaveContentsString();
 string SaveCampaignString();
 void LoadContentsString( const char *w );
 void LoadCampaignString( const char *w );
 Zdis_manual<ItemContentTree> contains;
 ~ItemContentNode();
 GLImage *ItemIcon();
 string ItemName();
 double ItemValue();
 Crayon ItemTint();
MANY(ItemContentNode,ItemContentNodeHandle,ItemContentNodeHandles,"item",ItemContentTree,{})
 KEYSWORDSGroups("item",ItemContentNode)
 ItemContentNode *FindLike( Commodity *c );
 ItemContentNode *FindLike( Thing *c );
 ItemContentNode *FindLike( Vehicle *c );
 ItemContentNode *FindLike( SpaceCar *c );
// void Like( ItemContentNodeHandles *out, ThingTypes type );
 void Things( ThingHandles *out, ThingTypes type );
 void Deposit( Commodity *c, int quantity );
 void Deposit( Thing *t );
 void Deposit( SpaceCar *sc );
 void Deposit( Vehicle *v );
// Returns an int of how short you are if the quantity is not available
 int Withdraw( Commodity *c, int quantity );
 bool Withdraw( Thing *t );
 bool Withdraw( SpaceCar *sc );
 Vehicle *Withdraw( ItemContentNode *v );
 Zint containerLimit;
 bool RemoveInside( ItemContentNode *n );
 int Containing( Commodity *c );
 void Merge( ItemContentTree *in );
 void Merge( ItemContentNode *n, ItemContentTree *tree );
 void All( ItemContentNodeHandles *nodes, ItemContentTreeHandles *trees );
 void TopLevel( ItemContentNodeHandles *out );
 Zint talliedContent;  int TallyContent(); // number of items
 Zdouble talliedUnits; double TallyInUnits(); // number of units of weight carried
 void Tally() { TallyContent(); TallyInUnits(); }
DONE(ItemContentNode);

HANDLES(ItemContentTree,ItemContentTreeHandle,ItemContentTreeHandles,"");