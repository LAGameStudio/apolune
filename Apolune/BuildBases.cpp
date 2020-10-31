#include "BuildBases.h"

#include "Player.h"

#include "HUD.h"  // unused I think

Zbool builderignoreclick,builderActive,builderTop=true,builderBottom=true;
BuildModes buildMode=SpaceBuilding;
Zp<SpaceBuildMenu> spacebuilder;
Zp<SpaceBuiltBasesOverlay> spacebuilt;

char *strut_image_files[50] = {
 "data/images/struts/cross/struts1.png",
 "data/images/struts/cross/struts2.png",
 "data/images/struts/cross/struts3.png",
 "data/images/struts/cross/struts4.png",
 "data/images/struts/cross/struts5.png",
 "data/images/struts/cross/struts6.png",
 "data/images/struts/cross/struts7.png",
 "data/images/struts/cross/struts8.png",
 "data/images/struts/cross/struts9.png",
 "data/images/struts/cross/struts10.png",
 "data/images/struts/cross/struts11.png",
 "data/images/struts/cross/struts12.png",
 "data/images/struts/cross/struts13.png",
 "data/images/struts/cross/struts14.png",
 "data/images/struts/cross/struts15.png",
 "data/images/struts/cross/struts16.png",
 "data/images/struts/cross/struts17.png",
 "data/images/struts/cross/struts18.png",
 "data/images/struts/cross/struts19.png",
 "data/images/struts/cross/struts20.png",
 "data/images/struts/cross/struts21.png",
 "data/images/struts/cross/struts22.png",
 "data/images/struts/cross/struts23.png",
 "data/images/struts/cross/struts24.png",
 "data/images/struts/cross/struts25.png",
 "data/images/struts/cross/struts26.png",
 "data/images/struts/cross/struts27.png",
 "data/images/struts/cross/struts28.png",
 "data/images/struts/cross/struts29.png",
 "data/images/struts/cross/struts30.png",
 "data/images/struts/LR/struts1.png",
 "data/images/struts/LR/struts2.png",
 "data/images/struts/LR/struts3.png",
 "data/images/struts/LR/struts4.png",
 "data/images/struts/LR/struts5.png",
 "data/images/struts/LR/struts6.png",
 "data/images/struts/LR/struts7.png",
 "data/images/struts/LR/struts8.png",
 "data/images/struts/LR/struts9.png",
 "data/images/struts/LR/struts10.png",
 "data/images/struts/UD/struts1.png",
 "data/images/struts/UD/struts2.png",
 "data/images/struts/UD/struts3.png",
 "data/images/struts/UD/struts4.png",
 "data/images/struts/UD/struts5.png",
 "data/images/struts/UD/struts6.png",
 "data/images/struts/UD/struts7.png",
 "data/images/struts/UD/struts8.png",
 "data/images/struts/UD/struts9.png",
 "data/images/struts/UD/struts10.png"
};

char *strut_tile_names[50] = {
 "X0", "X1", "X2", "X3", "X4",
 "X5", "X6", "X7", "X8", "X9",
 "XA", "XB", "XC", "XD", "XE",
 "XF", "XG", "XH", "XI", "XJ",
 "XK", "XL", "XM", "XN", "XO",
 "XP", "XQ", "XR", "XS", "XT",
 "H0", "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8", "H9",
 "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9"
};

 BuiltBase *BuiltBases::Add( int base_id_group, double x, double y, Cartesian *bc ) {
  OUTPUT("BuiltBases::Add(%1.1f,%1.1f) Adding a new base\nBase coord: %s",x,y,bc->toString().c_str());
  BuiltBase *b=new BuiltBase;
  b->tiles=&tiles;
  b->baseCoord.Set(bc);
  b->Init(true);
  b->baseOffset.Set(x,y);
  b->MoveByd(x+moved1.x,y+moved1.y);
  b->base_id=player->bases.GetNextID();
  b->locale=player->locale;
  Append(b);
  player->bases.Add(player->locale.c_str(),base_id_group);
  changedSinceLastSave=true;
  return b;
 }

void BuiltBase::Init( bool generate ) {
  extents1.x2=extents1.w=BB_totalW/2;
  extents1.y2=extents1.h=BB_totalH/2;
  extents2.x2=extents2.w=BB_totalW;
  extents2.y2=extents2.h=BB_totalH;
  if ( generate ) {
   Strings s;
   back  =maps.AddLayer( clampTile, translated, 1.0f, player->spaceGameView, tiles, &s, BB_tilesize+1,BB_tilesize+1,  BB_tilesize2, BB_tilesize2, 2 ); 
   middle=maps.AddLayer( clampTile, translated, 2.0f, player->spaceGameView, tiles, &s, BB_tilesize+1,BB_tilesize+1,  BB_tilesize,  BB_tilesize, 2 ); 
  }
 }

void BuiltBases::Load( int base_id_group ) {
 player->bases.Matching(player->locale.c_str(),base_id_group);
 EACH(player->bases.matches.first,BuiltBaseLookupHandle,bblh) {
  string fn=specialPaths.historic(FORMATv("bases%d_%d_%d.dat",(int) player->local_id, (int) bblh->p->base_id, (int) bblh->p->base_id_group ).c_str());
  if ( file_exists(fn.c_str()) ) {
   OUTPUT("BuiltBases: Loading `%s`\n",fn.c_str());
   BuiltBase *b=new BuiltBase;
   b->Init(false);
   b->fromString(file_as_string(fn.c_str()).c_str());
   b->tiles=&tiles;
   b->base_id=bblh->p->base_id;
   b->base_id_group=base_id_group;
   Append(b);
   OUTPUT("BuiltBases: Load complete.\n");
  } else
  OUTPUT("BuiltBases: Unable to load `%s` (base does not yet exist)\n",fn.c_str());
 }
 changedSinceLastSave=false;  
}

void BuiltBases::Save( int base_id_group ) {
 if ( !changedSinceLastSave ) return;
 if ( count > 0 ) {
  Cartesiand _moved1; _moved1.Set(moved1.x,moved1.y);
  MoveByd(-_moved1.x,-_moved1.y);
  FOREACH(BuiltBase,b) {
   string fn=specialPaths.historic(FORMATv("bases%d_%d_%d.dat",(int) player->local_id, b->base_id, (int) base_id_group ).c_str());
   OUTPUT("BuiltBases: Saving `%s`\n",fn.c_str());
   string_as_file(b->toString().c_str(),fn.c_str());
  }
  MoveByd(_moved1.x,_moved1.y);
  changedSinceLastSave=false;
 }
}

#if defined(NEVER)
void SpaceBuildMenu::HideHUD() {
  if ( !hud ) return;
  if ( this->visible ) { // Hide HUD
   hud->Visible("Fabricator", false);
   hud->Visible("Fabricate",  false);
   hud->Visible("Cargo",      false);
   hud->Visible("Repair",     false);
   hud->Visible("Chemistry",    false);
   hud->Visible("Radar",      false);
   hud->Visible("Map",   false);
   hud->Visible("Away Team",  false);
   hud->Visible("Fleet",      false);
   hud->Visible("Crew",       false);
   hud->Visible("BlueprintEdit",false);
   hud->Visible("ComponentEdit",false);
   hud->Visible("ModelEdit",false);
   hud->Visible("Chat",       false);
   hud->Visible("ToggleChat",false);
   hud->Visible("SmileyControl",false);
   hud->Visible("Scanner", false);
   hud->Visible("Weapon", false);
   hud->Visible("Radio", false);
   hud->Visible("RadioControl", false);
   hud->Visible("Smileys", false);
   hud->Visible("Disembark", false);
   hud->Visible("Tractor", false);
   hud->Visible("AwayInventory", false );
   hud->Visible("AwayRepair", false );
   hud->Visible("AwayGun", false );
   hud->Visible("AwayScience", false );
  } else if ( !this->visible ) { // Show HUD
   hud->Visible("Fabricate",  true);
   hud->Visible("Cargo",      true);
   hud->Visible("Repair",     true);
   hud->Visible("Chemistry",    true);
   hud->Visible("Radar",      true);
   hud->Visible("Map",   true);
   hud->Visible("Away Team",  true);
   hud->Visible("Fleet",      true);
   hud->Visible("Crew",       true);
   hud->Visible("ToggleChat", true);
   hud->Visible("Scanner",    true);
   hud->Visible("Weapon",     true);
   hud->Visible("Radio",      true);
   hud->Visible("Disembark",  true);
   hud->Visible("Tractor",    true);/*
   if ( players.active->profile.active->vessel != players.active->profile.active->starship ) {
    hud->arb->hidden=false;
    hud->agb->hidden=false;
    hud->asb->hidden=false;
    hud->aib->hidden=false;    
    hud->db->hidden=true;
    hud->sb->hidden=true;
    hud->cb->hidden=true;
    hud->fb->hidden=true;
    hud->wb->hidden=true;
    hud->rb->hidden=true;
    hud->tb->hidden=true;
    hud->pb->hidden=true;
    hud->sb->hidden=true;
   }
  */
  }
}

#endif

void SpaceBuiltBasesOverlay::Between() {
 if ( !visible || destroyMe || !builderActive ) return;
 MousePosition(&mx,&my);
 // Find hovering
 /*
 if ( hovering ) {
  bases->hovering=hovering=bases->getAtPosition(hovering,mx,my);
 } else {
  bases->hovering=hovering=bases->getAtPosition(mx,my);
 }*/
#if defined(NEVER)
 // Avoid build button
 if ( (input->left || input->right)
   && WITHIN(mx,my,hud->bb->x,hud->bb->y,hud->bb->x2,hud->bb->y2) ) return;
#endif
 p.x=(mx-display->w2)-(int)bases->moved2.x;
 p.y=(my-display->h2)-(int)bases->moved2.y;
 // Is the user doing this within the buildable area?
 BuiltBase *b=GetBase( p.x, p.y, false );
 // Get the tile we are hovering
 GetImpactCoord( p.x, p.y ); // If the base does not exist, this value is reported incorrectly.
 if ( !clicked && input->left && !builderignoreclick ) { // Draw
  clicked=true;
  if ( !b ) {
   b=GetBase(p.x,p.y,true,base_id_group);
   if ( !b ) {
    audio.manager.CueAndPlayIfNotPlaying("data/sounds/spoken/nonewbase.wav",1.0f,1.0f,false,true);
    return;
   }
  }
  /// We now have a base.
  // Find a tile, because we're placing struts not objects.
  Tile2d *tile=GetTile();
  /// Now we have a tile.
  // Place the tile on the base at the baseCoord
  int built=0;
  int unbuilt=0;
  MassiveTileMapParallaxLayer2d *m=(MassiveTileMapParallaxLayer2d *) b->maps.layers.first;
  if ( builderBottom && m ) {
   if ( m->map.map(impact.x,impact.y) != null ) unbuilt++;
   m->map.map(impact.x,impact.y)=tile; built++;
  }
  m=(MassiveTileMapParallaxLayer2d *) b->maps.layers.last;
  if ( builderTop && m ) {
   if ( m->map.map(impact.x,impact.y) != null ) unbuilt++;
   m->map.map(impact.x,impact.y)=tile; built++;
  }
  if ( built > 0 || unbuilt > 0 ) bases->changedSinceLastSave=true;
  if ( built-unbuilt > 0 ) {
   b->built+=built-unbuilt;
   audio.manager.CueAndPlay("data/sounds/fx/impact3.wav",1.0f,0.5f);
  }
 } else if ( input->wasLeft && !input->left ) { // mouse released
  clicked=false; 
  builderignoreclick=false;
 }
 else if ( input->right /*&& !hovering*/ && !builderignoreclick ) { // Erase or abort
  if ( b ) {
   int unbuilt=0;
   MassiveTileMapParallaxLayer2d *m=(MassiveTileMapParallaxLayer2d *) b->maps.layers.first;
   if ( builderBottom && m ) {
    if ( m->map.map(impact.x,impact.y) != null ) {
     m->map.map(impact.x,impact.y)=null;
     unbuilt++;
    }
   }
   m=(MassiveTileMapParallaxLayer2d *) b->maps.layers.last;
   if ( builderTop && m ) {
    if ( m->map.map(impact.x,impact.y) != null ) {
     m->map.map(impact.x,impact.y)=null;
     unbuilt++;
    }
   }
   if ( unbuilt > 0 ) {
    b->built-=unbuilt;
    bases->changedSinceLastSave=true;
   }
  }
 }
 else { // no mouse button activity
  if ( b )  { // draw the pen
  }
 }
}

#if defined(NEVER)
///#include "PlanetaryBuiltBases.h"
#include "HUD.h"

void EnterBaseButton::WhenClicked() {
// hud->tce=new TinyComplexEngine;
// hud->AddChild(hud->tce);
// hud->tce->ForObject(s,p,m,this->building);
// hud->sse->visible=false;
// if ( hud->pe ) hud->pe->visible=false;
}

void BasesOverlay::MouseMove() {
 if ( ((buildMode==SpaceBuilding && !spacebuilder->visible)
     ||(buildMode==PlanetBuilding && !planetbuilt->visible))
  && visible && players.active->profile.active->vessel == astronaut ) {
  int mx,my;
  MousePosition(&mx,&my);
  hovering= (!hovering ? bases->getAtPosition(mx,my) : bases->getAtPosition(hovering,mx,my));
  if ( !hovering ) {
   if ( open && ebb ) {
    fx.Remove(ebb);
    delete ebb;
    ebb=null;
   }
  } else
  if ( hovering && hovering != open ) {
   if ( open && ebb ) {
    fx.Remove(ebb);
    delete ebb;
   }
   open=hovering;
   ebb=new EnterBaseButton;
   ebb->building=open;
   ebb->s=players.active->profile.active->locale;
   if ( hud->pe && hud->pe->visible ) ebb->p=hud->pe->painter->planet->planet;
   ebb->labelOffset.Set(0,-15);
   Add(ebb,true);
   ebb->MoveToCentered(open->screen.x+open->screen.w/2,open->screen.y+open->screen.h/2);
  } else {}
 }
}
#endif