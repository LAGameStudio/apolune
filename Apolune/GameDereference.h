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
#include "Campaign.h"

enum DereferenceTypes {
 dt_error=-1,
 dt_commodities=0,
 dt_things=1,
 dt_species=2,
 dt_lifeforms=3,
 dt_munitions=4,
 dt_weapons=5,
 dt_modules=6,
 dt_vehicles=7,
 dt_interiors=8,
 dt_celestials=9,
 dt_locations=10,
 dt_spacecars=11
};

/* Template Switch Statement for enum DereferenceTypes
  switch ( (DereferenceTypes)(int)value ) {
   case dt_error: {
   } break;
   case dt_commodities: {
   } break;
   case dt_things: {
   } break;
   case dt_species: {
   } break;
   case dt_lifeforms: {
   } break;
   case dt_vehicles: {
   } break;
   case dt_gametiles: {
   } break;
   case dt_celestials: {
   } break;
   default: OUTPUT("DereferenceTypes: Invalid value: %d\n",(int)value); break;
  }
 */

class GameDereference {
public:
 Zdis<CommodityHandles> commodities;
 Zp<Commodity> commodity;
 Zdis<ThingHandles> stuff;
 Zp<Thing> thing;
 Zdis<SpeciesHandles> specieses;
 Zp<Species> species;
 Zdis<LifeformHandles> lifeforms;
 Zp<Lifeform> lifeform;
 Zdis<MunitionHandles> munitions;
 Zp<Munition> munition;
 Zp<SpaceCar> spacecar;
 Zdis<SpaceCarHandles> spacecars;
 Zdis<VehicleWeaponBlueprintHandles> weapons;
 Zp<VehicleWeaponBlueprint> weapon;
 Zdis<VehicleModuleBlueprintHandles> modules;
 Zp<VehicleModuleBlueprint> module;
 Zdis<VehicleBlueprintHandles> vehicles;
 Zp<VehicleBlueprint> vehicle;
 Zdis<PlanetProfileHandles> celestials;
 Zp<PlanetProfile> celestial;
 Zdis<InteriorHandles> interiors;
 Zp<Interior> interior;
 Zdis<GameLocationHandles> locations;
 Zp<GameLocation> location;
 DereferenceTypes dereferencedAs;
 GameDereference() { dereferencedAs=dt_error; Clear(); }
 bool isValid() { return dereferencedAs != dt_error; }
 GameDereference &operator=(std::string a) {
  Reference(a.c_str());
  return *this;
 }
 GameDereference &operator=(const char *a) {
  Reference(a);
  return *this;
 }
 void Clear();
 static void DeployTypes( Strings *s ) {
  s->Add("error",-1);
  s->Add("commodity",0);
  s->Add("thing",1);
  s->Add("species",2);
  s->Add("lifeform",3);
  s->Add("munition",4);
  s->Add("weapon",5);
  s->Add("module",6);
  s->Add("vehicle",7);
  s->Add("interior",8);
  s->Add("celestial",9);
  s->Add("location",10);
  s->Add("spacecar",11);
 }
 void Reference( const char *r, Campaign *c=null );
 Zp<GLImage> image;
 GLImage *Representation();
 Poseable2d *RepresentationPaperdoll();
private:
 void SearchCampaign( bool exact, const char *search, Campaign *c, DereferenceTypes scope );
 void SearchCampaign( bool exact, const char *search, Campaign *c );
 void SearchCampaigns( bool exact, const char *search );
 void SearchCampaigns( bool exact, const char *search, DereferenceTypes scope );
 void GetLocations( bool exact, const char *search, Campaign *c );
 void GetInteriors( bool exact, const char *search, Campaign *c );
 void GetCelestials( bool exact, const char *search, Campaign *c );
 void GetSpaceCars( bool exact, const char *search, Campaign *c );
 void GetVehicles( bool exact, const char *search, Campaign *c );
 void GetModules( bool exact, const char *search, Campaign *c );
 void GetWeapons( bool exact, const char *search, Campaign *c );
 void GetMunitions( bool exact, const char *search, Campaign *c );
 void GetLifeforms( bool exact, const char *search, Campaign *c );
 void GetSpecies( bool exact, const char *search, Campaign *c );
 void GetThings( bool exact, const char *search, Campaign *c );
 void GetCommodities( bool exact, const char *search, Campaign *c );
 void GetLocations( bool exact, const char *search );
 void GetInteriors( bool exact, const char *search );
 void GetCelestials( bool exact, const char *search );
 void GetSpaceCars( bool exact, const char *search );
 void GetVehicles( bool exact, const char *search );
 void GetModules( bool exact, const char *search );
 void GetWeapons( bool exact, const char *search );
 void GetMunitions( bool exact, const char *search );
 void GetLifeforms( bool exact, const char *search );
 void GetSpecies( bool exact, const char *search );
 void GetThings( bool exact, const char *search );
 void GetCommodities( bool exact, const char *search );
};

extern GameDereference dereferencer;