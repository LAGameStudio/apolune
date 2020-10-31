/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "HUD.h"

void OptionsToggle::WhenClicked() {
}

// void Launch() is in Planetary engine with void EnterCave()

void HUD::Repopulate() {
 Anchor(0.0f,0.0f,0.0f,0.0f);
 /*
 cw=new ChatWindow;
 cw->hidden=true;
 Gauge ( "Chat", cw, 32, display->h-display->h/4-16 );
 */

 ot=new OptionsToggle;
 ot->w=16;
 ot->h=16;
// ot->manager=this;
 Gauge ( "Options", ot, 64, 16+8 );

 // Add "building hud" and init
// buildMode=SpaceBuilding;
// Gauge( "SpaceBuilder",         spacebuilder=new SpaceBuildMenu );
// Gauge( "SpaceBuilderOverlay",  spacebuilt=new SpaceBuiltBasesOverlay);
// Gauge( "PlanetBuilder",        planetbuilder=new PlanetBuildMenu);
// Gauge( "PlanetBuilderOverlay", planetbuilt=new PlanetaryBuiltBasesOverlay);
}

void HUD::BetweenFrames()  {
 /*
 if ( !fx_UglyTextBox_is_focused ) {
  keypressed=false;
  //arrow keys?
  if ( !keypressed ) {
  }
 }
 */
}

void HUD::MouseMove() {
 fx.MouseMove();
}

void HUD::LeftDown() {
// mc->LeftDown();
 fx.LeftDown();
}

void HUD::LeftUp() {
// mc->LeftUp();
 fx.LeftUp(); 
}

void HUD::RightDown() {
 fx.RightDown();
}

void HUD::RightUp() {
 fx.RightDown();
}

Zpointer<HUD> hud;