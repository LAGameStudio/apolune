
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author); Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Globals.h"

SafeImageLookupInt beam_flares;
SafeImageLookupInt weapon_flares;

bool HUD_GUI_LOCKED=true;

// Credits
Strings credits;
// ForestPrimeval
#include "Image2Model.h"

//Image2Models forest_place_large;

// Disposables
#include "SpherePainter.h" // sphereTextureLibrary, solsystemSphereTextureLibrary
#include "Skybox.h" // skyboxesNear, skyboxesMid, skyboxesFar

void InitializeGlobals() {

 OUTPUT("Initializing global disposables.\n");

 OUTPUT("Initializing global strings and optimized texture lookups.\n");

 credits.Add("Produced by Lost Astronaut");
 credits.Add("Game and Graphics Designed by Herbert Elwood Gilliland III");
 credits.Add("Psuedo-random number seeds provided by Random.org");
 credits.Add("Website score by Mad Merv and Evan Dean");
 credits.Add("Thanks to John Carmack for moral support");

 
 beam_flares.Default();
 beam_flares.Add("data/images/flares/ball1.png");  
 beam_flares.Add("data/images/flares/ball2.png"); 
 beam_flares.Add("data/images/flares/ball3.png");  
 beam_flares.Add("data/images/flares/ball4.png");  
 beam_flares.Add("data/images/flares/ball5.png");  
 beam_flares.Add("data/images/flares/ball6.png");  
 beam_flares.Add("data/images/flares/ball7.png"); 
 beam_flares.Add("data/images/flares/smoke.png");  
 beam_flares.Add("data/images/flares/ball9.png");  
 beam_flares.Add("data/images/flares/ball10.png");  
 beam_flares.Add("data/images/flares/ball11.png");  
 beam_flares.Add("data/images/flares/ball12.png");  
 beam_flares.Add("data/images/flares/base.png");  
 beam_flares.Add("data/images/flares/flare.png");  
 beam_flares.Add("data/images/flares/flare2.png");  
 beam_flares.Add("data/images/flares/Purple_Flare.jpg");
 beam_flares.Add("data/images/flares/radiation.png");
 beam_flares.Add("data/images/flares/smoke2.png");
 beam_flares.Add("data/images/flares/neoflare1.png");
 beam_flares.Add("data/images/flares/neoflare2.png");
 beam_flares.Add("data/images/flares/neoflare3.png");
 beam_flares.Add("data/images/flares/neoflare4.png");
 beam_flares.Add("data/images/flares/neoflare5.png");
 beam_flares.Add("data/images/flares/neoflare6.png");
 beam_flares.Add("data/images/flares/neoflare7.png");
 beam_flares.Add("data/images/flares/neoflare8.png");
 beam_flares.Add("data/images/flares/neoflare9.png");
 beam_flares.Add("data/images/flares/neoflare10.png");
 beam_flares.Add("data/images/flares/neoflare11.png");
 beam_flares.Add("data/images/flares/neoflare12.png");
 beam_flares.Add("data/images/flares/neoflare13.png");
 beam_flares.Add("data/images/flares/neoflare14.png");
 beam_flares.Add("data/images/flares/neoflare15.png");
 beam_flares.Add("data/images/flares/neoflare16.png");
 beam_flares.Add("data/images/flares/neoflare17.png");
 beam_flares.Add("data/images/flares/neoflare18.png");
 beam_flares.Add("data/images/flares/neoflare19.png");
 beam_flares.Add("data/images/flares/neoflare20.png");
 beam_flares.Add("data/images/flares/neoflare21.png");

 weapon_flares.Default();
 weapon_flares.Add("data/images/flares/ball1.png");  
 weapon_flares.Add("data/images/flares/ball2.png"); 
 weapon_flares.Add("data/images/flares/ball3.png");  
 weapon_flares.Add("data/images/flares/ball4.png");  
 weapon_flares.Add("data/images/flares/ball5.png");  
 weapon_flares.Add("data/images/flares/ball6.png");  
 weapon_flares.Add("data/images/flares/ball7.png"); 
 weapon_flares.Add("data/images/flares/smoke.png");  
 weapon_flares.Add("data/images/flares/ball9.png");  
 weapon_flares.Add("data/images/flares/ball10.png");  
 weapon_flares.Add("data/images/flares/ball11.png");  
 weapon_flares.Add("data/images/flares/ball12.png");  
 weapon_flares.Add("data/images/flares/base.png");  
 weapon_flares.Add("data/images/flares/flare.png");  
 weapon_flares.Add("data/images/flares/Purple_Flare.jpg");
 weapon_flares.Add("data/images/flares/radiation.png");
 weapon_flares.Add("data/images/flares/smoke2.png");
 weapon_flares.Add("data/images/flares/neoflare1.png");
 weapon_flares.Add("data/images/flares/neoflare2.png");
 weapon_flares.Add("data/images/flares/neoflare3.png");
 weapon_flares.Add("data/images/flares/neoflare4.png");
 weapon_flares.Add("data/images/flares/neoflare5.png");
 weapon_flares.Add("data/images/flares/neoflare6.png");
 weapon_flares.Add("data/images/flares/neoflare7.png");
 weapon_flares.Add("data/images/flares/neoflare8.png");
 weapon_flares.Add("data/images/flares/neoflare9.png");
 weapon_flares.Add("data/images/flares/neoflare10.png");
 weapon_flares.Add("data/images/flares/neoflare11.png");
 weapon_flares.Add("data/images/flares/neoflare12.png");
 weapon_flares.Add("data/images/flares/neoflare13.png");
 weapon_flares.Add("data/images/flares/neoflare14.png");
 weapon_flares.Add("data/images/flares/neoflare15.png");
 weapon_flares.Add("data/images/flares/neoflare16.png");
 weapon_flares.Add("data/images/flares/neoflare17.png");
 weapon_flares.Add("data/images/flares/neoflare18.png");
 weapon_flares.Add("data/images/flares/neoflare19.png");
 weapon_flares.Add("data/images/flares/neoflare20.png");
 weapon_flares.Add("data/images/flares/neoflare21.png");
 weapon_flares.Add("data/images/flares/smoke5.png");


 skyboxesFar.Define( "data/images/skyboxes/1024/indigo/indigo" );
 skyboxesFar.Define( "data/images/skyboxes/1024/starrynight/starrynight" );
 skyboxesFar.Define( "data/images/skyboxes/1024/stars/stars" );
 skyboxesFar.Define( "data/images/skyboxes/1024/wisp/wisp" );
 skyboxesFar.Define( "data/images/skyboxes/blocks/bg2/bg2" );
 skyboxesMid.Define( "data/images/skyboxes/blocks/dust1/dust1" );
 skyboxesNear.Define( "data/images/skyboxes/blocks/locals1/neardust1" );

// forest_static_flowers;
// forest_rocks;

 OUTPUT("Loading Global Image2Models: ");
// EACH(forest_placements_large.first,String,s) forest_place_large.Append(new Image2Model(s->s.c_str()));
 OUTPUT("complete.\n");

}