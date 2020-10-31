#include "GameDereference.h"

#include  "Campaign.h"

GameDereference dereferencer;

void GameDereference::Clear() {
 commodities.Recycle();
 commodity=null;
 munitions.Recycle();
 munition=null;
 stuff.Recycle(); // why no Things?
 thing=null;
 specieses.Recycle();
 species=null;
 lifeforms.Recycle();
 lifeform=null;
 modules.Recycle();
 module=null;
 weapons.Recycle();
 weapon=null;
 vehicles.Recycle();
 vehicle=null;
 celestials.Recycle();
 celestial=null;
 interiors.Recycle();
 interior=null;
 locations.Recycle();
 location=null;
 spacecars.Recycle();
 spacecar=null;

 dereferencedAs=dt_error;
 image=null;
}

// Establishes a "game object reference syntax"
// type/search     only exact matches
// type/search*    any matches (not a true wildcard, asterisk at end only)
// search          across all types
// #search         across all types, across all campaigns
// #type/search    specific type, across all campaigns
// #type/search*   specific type, across all campaigns, non-exact
void GameDereference::Reference( const char *r, Campaign *c ) {
 Strings types;
 DeployTypes(&types);
 Strings ref,pieces;
 ref.SplitPush(r,',');
 EACH(ref.first,String,s) if ( s->s.length() > 0 ) {
  pieces.Clear();
  pieces.SplitPush(s->s.c_str(),'/');
  DereferenceTypes dt=dt_error;
  Zstring search;
  bool exact=false;
  bool all=false;
  if ( pieces.count == 2 ) { // type:search
   String *typeString=(String *) pieces.first;
   String *searchString=(String *) pieces.last;
   exact=char_in('*',searchString->s.c_str());
   search=searchString->s.c_str();
   search("*","");
   Zstring typeStr=typeString->s;
   if ( char_in('#',typeStr.c_str()) ) {
    all=true;
    typeStr("#","");
   }
   typeStr.Trim();
   if ( typeStr.length == 0 ) continue;
   EACH(types.first,String,s) if ( typeStr == s->s.c_str() ) dt=(DereferenceTypes) s->integer;
  } else {
   search=s->s.c_str();
  }
  if ( char_in('#',search.c_str()) ) {
   all=true;
   search("#","");
  }
  if ( search.length == 0 ) continue;
  if ( dt == dt_error ) { // No valid type specified, search everything
   if ( c ) SearchCampaign(exact,search.c_str(),c);
   else SearchCampaigns(exact,search.c_str());
  } else { // Search only one type
   switch ( dt ) {
    case dt_error: { 
     if ( c ) SearchCampaign(exact,search.c_str(),c);
     else SearchCampaigns(exact,search.c_str());
    } break;
    case dt_commodities: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetCommodities(exact,search.c_str(),c);
    } break;
    case dt_things: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetThings(exact,search.c_str(),c);
    } break;
    case dt_species: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetSpecies(exact,search.c_str(),c);
    } break;
    case dt_lifeforms: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetLifeforms(exact,search.c_str(),c);
    } break;
    case dt_munitions: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetMunitions(exact,search.c_str(),c);
    } break;
    case dt_weapons: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetWeapons(exact,search.c_str(),c);
    } break;
    case dt_modules: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetModules(exact,search.c_str(),c);
    } break;
    case dt_vehicles: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetVehicles(exact,search.c_str(),c);
    } break;
    case dt_celestials: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetCelestials(exact,search.c_str(),c);
    } break;
    case dt_spacecars: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetSpaceCars(exact,search.c_str(),c);
    } break;
    case dt_interiors: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetInteriors(exact,search.c_str(),c);
    } break;
    case dt_locations: {
     if ( all || !c ) SearchCampaigns(exact,search.c_str(),dt);
     else GetLocations(exact,search.c_str(),c);
    } break;
    default: OUTPUT("DereferenceTypes: Invalid value: %d\n",(int)dt); break;
   }   
  }
 }
 image=Representation();
}

#define GDEREFGETC(A,B,C,D) void GameDereference::A(bool exact,const char*search,Campaign *c) { \
  if ( !c ) { A(exact,search); return; } c->AllOptimized(search,B,exact); \
  if ( B->count > 0 ) C=((D*)B->first)->p; }
GDEREFGETC(GetLocations,locations,location,GameLocationHandle)
GDEREFGETC(GetInteriors,interiors,interior,InteriorHandle)
GDEREFGETC(GetCelestials,celestials,celestial,PlanetProfileHandle)
GDEREFGETC(GetSpaceCars,spacecars,spacecar,SpaceCarHandle)
GDEREFGETC(GetVehicles,vehicles,vehicle,VehicleBlueprintHandle)
GDEREFGETC(GetModules,modules,module,VehicleModuleBlueprintHandle)
GDEREFGETC(GetWeapons,weapons,weapon,VehicleWeaponBlueprintHandle)
GDEREFGETC(GetMunitions,munitions,munition,MunitionHandle)
GDEREFGETC(GetSpecies,specieses,species,SpeciesHandle)
GDEREFGETC(GetLifeforms,lifeforms,lifeform,LifeformHandle)
GDEREFGETC(GetThings,stuff,thing,ThingHandle)
GDEREFGETC(GetCommodities,commodities,commodity,CommodityHandle)

#define GDEREFGET(A,B,C,D,E,F) void GameDereference::A(bool exact,const char*search) { \
  if ( exact ) { EACH(B.first,C,c) if ( c->name == search ) D->Add(c); } \
  else { EACH(B.first,C,c) if (c->name.length > 0 && (c->name == search || c->name.prefix(search)) ) D->Add(c); } \
  if ( D->count >0 ) E=((F *) D->first )->p; }


GDEREFGET(GetLocations,gameLocations,GameLocation,locations,location,GameLocationHandle)
GDEREFGET(GetCelestials,planetProfiles,PlanetProfile,celestials,celestial,PlanetProfileHandle)
GDEREFGET(GetInteriors,(*Campaigns::GetInteriors()),Interior,interiors,interior,InteriorHandle)
GDEREFGET(GetSpaceCars,veh_bs,SpaceCar,spacecars,spacecar,SpaceCarHandle)
GDEREFGET(GetVehicles,veh_bs,VehicleBlueprint,vehicles,vehicle,VehicleBlueprintHandle)
GDEREFGET(GetModules,veh_mbs,VehicleModuleBlueprint,modules,module,VehicleModuleBlueprintHandle)
GDEREFGET(GetWeapons,veh_wbs,VehicleWeaponBlueprint,weapons,weapon,VehicleWeaponBlueprintHandle)
GDEREFGET(GetMunitions,munitionTypes,Munition,munitions,munition,MunitionHandle)
GDEREFGET(GetSpecies,evolution,Species,specieses,species,SpeciesHandle)
GDEREFGET(GetLifeforms,(*Campaigns::GetLifeforms()),Lifeform,lifeforms,lifeform,LifeformHandle)
GDEREFGET(GetThings,things,Thing,stuff,thing,ThingHandle)
GDEREFGET(GetCommodities,(*Campaigns::GetCommodities()),Commodity,commodities,commodity,CommodityHandle)


void GameDereference::SearchCampaign( bool exact, const char *search, Campaign *c, DereferenceTypes scope ) {
 switch ( scope ) {
  case dt_error: {
  } break;
  case dt_commodities:{ GetCommodities(exact,search,c); } break;
  case dt_things:     { GetThings(exact,search,c);      } break;
  case dt_species:    { GetSpecies(exact,search,c);     } break;
  case dt_lifeforms:  { GetLifeforms(exact,search,c);   } break;
  case dt_munitions:  { GetMunitions(exact,search,c);   } break;
  case dt_modules:    { GetModules(exact,search,c);     } break;
  case dt_weapons:    { GetWeapons(exact,search,c);     } break;
  case dt_vehicles:   { GetVehicles(exact,search,c);    } break;
  case dt_celestials: { GetCelestials(exact,search,c);  } break;
  case dt_interiors:  { GetInteriors(exact,search,c);   } break;
  case dt_locations:  { GetLocations(exact,search,c);   } break;
  case dt_spacecars:  { GetSpaceCars(exact,search,c);   } break;
  default: OUTPUT("DereferenceTypes: Invalid value: %d\n",(int)scope); break;
 }
}

void GameDereference::SearchCampaign( bool exact, const char *search, Campaign *c ) {
 GetLocations(   exact, search, c );
 GetCelestials(  exact, search, c );
 GetInteriors(   exact, search, c );
 GetSpaceCars(   exact, search, c );
 GetVehicles(    exact, search, c );
 GetModules(     exact, search, c );
 GetWeapons(     exact, search, c );
 GetMunitions(   exact, search, c );
 GetLifeforms(   exact, search, c );
 GetSpecies(     exact, search, c );
 GetThings(      exact, search, c );
 GetCommodities( exact, search, c );
}

void GameDereference::SearchCampaigns( bool exact, const char *search ) {
 GetLocations(   exact, search );
 GetCelestials(  exact, search );
 GetInteriors(   exact, search );
 GetSpaceCars(   exact, search );
 GetVehicles(    exact, search );
 GetModules(     exact, search );
 GetWeapons(     exact, search );
 GetMunitions(   exact, search );
 GetLifeforms(   exact, search );
 GetSpecies(     exact, search );
 GetThings(      exact, search );
 GetCommodities( exact, search );
}

void GameDereference::SearchCampaigns( bool exact, const char *search, DereferenceTypes scope ) {
 switch ( scope ) {
  case dt_error: {
  } break;
  case dt_commodities: { GetCommodities(exact,search); } break;
  case dt_things:      { GetThings(exact,search);      } break;
  case dt_species:     { GetSpecies(exact,search);     } break;
  case dt_lifeforms:   { GetLifeforms(exact,search);   } break;
  case dt_munitions:   { GetMunitions(exact,search);   } break;
  case dt_modules:     { GetModules(exact,search);     } break;
  case dt_weapons:     { GetWeapons(exact,search);     } break;
  case dt_vehicles:    { GetVehicles(exact,search);    } break;
  case dt_interiors:   { GetInteriors(exact,search);   } break;
  case dt_celestials:  { GetCelestials(exact,search);  } break;
  case dt_locations:   { GetLocations(exact,search);   } break;
  case dt_spacecars:   { GetSpaceCars(exact,search);   } break;
  default: OUTPUT("DereferenceTypes: Invalid value: %d\n",(int)scope); break;
 }
}

// Gets a preview image by a specific order of elimination
GLImage *GameDereference::Representation() {
 if ( vehicle && vehicle->hull && vehicle->hull->blueprint && vehicle->hull->blueprint->image ) return vehicle->hull->blueprint->image.pointer;
 if ( thing.pointer && thing->image.pointer ) {
  return thing->image.pointer;
 }
 if ( stuff && stuff->count > 0 ) {
  EACH(stuff->first,Thing,t)
   if (t->image.pointer) return t->image.pointer;
 }
 return null;
}

// Gets a preview image by a specific order of elimination
Poseable2d *GameDereference::RepresentationPaperdoll() {
 Poseable2d *p=null;
 if ( lifeform && lifeform->species ) return &lifeform->species->paperdoll;
 if ( lifeforms && lifeforms->count > 0 ) {
  EACH(lifeforms->first,Lifeform,t) if (t->species ) return &t->species->paperdoll;
 }
 return p;
}