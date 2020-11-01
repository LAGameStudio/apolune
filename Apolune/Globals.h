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
// FlagEdit.h
#include "SafeMediaLookup.h"
#include "ZeroTypes.h"
#include "Numbers.h"

void InitializeGlobals();

extern bool HUD_GUI_LOCKED;

// FlagEdit, MaterialEdit
extern SafeImageLookupInt blending_icons;
extern SafeImageLookupInt betsy_ross;
// TractorEdit
extern SafeImageLookupInt beam_flares;
// WeaponEdit
extern SafeImageLookupInt weapon_flares;
// MapObject
extern Strings vehicle_strings;
// Universe
extern Strings MKK;
extern Strings planet_strings;
extern SafeImageLookupInt anomaly_images;
extern Strings anomaly_type_strings;
extern SafeImageLookupInt nebula_images;
extern Strings nebula_type_strings;
// TitleSequence
extern SafeSoundLookup logo_sounds;
// TitleMenu
extern SafeImageLookupInt nebulae;
extern SafeImageLookupInt glowball;
extern SafeImageLookupInt lightning;
extern SafeImageLookupInt bgfg;
extern SafeImageLookupInt flares;
extern SafeImageLookupInt buttontexts;
// Aliens
extern SafeImageLookupInt body_parts;
// Hardpoint
extern SafeSoundLookup blaster_sounds;
extern SafeSoundLookup bullet_sounds;
// Troid3d
extern Strings crystal_models;
extern Strings spark_films;
extern SafeSoundLookup ricochet_sounds;
extern SafeSoundLookup laser_sounds;
extern SafeSoundLookup electric_sounds;
extern SafeSoundLookup thunder_claps;
extern SafeSoundLookup rock_grinding;
extern SafeSoundLookup boom_sounds;
extern SafeSoundLookup bang_sounds;
extern SafeSoundLookup geiger_sounds;
extern SafeSoundLookup fwub_sounds;
extern SafeImageLookupInt snowflakes;
// Tech
extern Strings tech_type;
extern SafeImageLookupInt tech_type_icons;
// AIDJ
extern SafeSoundLookup tracks;
// Component
extern Strings component_types;
extern SafeImageLookupInt component_type_icons;
extern SafeImageLookupInt component_type_images;
extern Floats component_valuations;
// PlanetaryMap
extern SafeImageLookupInt additive_detail_surfaces;
extern SafeImageLookupInt transparent_detail_surfaces;
// PlanetPainter
extern SafeImageLookupInt planet_craters;
extern SafeImageLookupInt planet_city_decals;
extern SafeImageLookupInt planet_spots;
extern SafeImageLookupInt planet_bands;
extern SafeImageLookupInt planet_geologic;
extern SafeImageLookupInt planet_terrain_textures;
// Credits
extern Strings credits;
// SS2d Tile Sounds
extern SafeSoundLookup tile_walking_sounds;
// SS2d Tile Grimes and other Decals
extern SafeImageLookupInt tile_grimes;
extern SafeImageLookupInt tile_supergrimes;
extern SafeImageLookupInt tile_puddles;
extern SafeImageLookupInt tile_splatters;
// Matter detail textures
extern SafeImageLookupInt matter_detail_textures;
// Starship Troopers
extern SafeImageLookupInt starship_troopers;
extern SafeImageLookupInt ship_garbage;
extern SafeImageLookupInt cargo_crates;
extern SafeImageLookupInt fireballs;
extern SafeImageLookupInt burnt_corpses;
extern SafeSoundLookup burning_screams;
// Geology and Craters
extern SafeImageLookupInt geo_craters;
extern SafeImageLookupInt geo_geysers;
extern SafeImageLookupInt geo_mountains;
extern SafeImageLookupInt geo_stamps;
extern SafeImageLookupInt geo_all_materials;
extern SafeImageLookupInt geo_nontech_materials;
extern SafeImageLookupInt geo_tech_materials;
extern SafeImageLookupInt geo_water_noise;
extern SafeImageLookupInt geo_high_contrast;
// ForestPrimeval
class Image2Models;
extern Strings forest_placements_large;
extern Image2Models forest_place_large;
extern Strings forest_placements_small;
extern Image2Models forest_place_small;
extern Strings forest_placements_tiny;
extern Image2Models forest_place_tiny;
extern Strings forest_placements_mini;
extern Image2Models forest_place_mini;
extern SafeImageLookupInt forest_static_flowers;
extern SafeImageLookupInt forest_rocks;

class Image2Models;
extern Image2Models color_placements;