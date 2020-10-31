#include "AtlasTileset.h"

AtlasTilesets atlasTilesets;

void AtlasTile::SetAtlasTile( AtlasTileset *ats, AtlasTile *at ) {
 if ( !ats || !at ) { ats=null; at=null; return; }
 this->ats=ats;
 this->atlasTilesetName=ats->name;
 this->at=at;
 this->atlasTileName=at->name;
}

void AtlasTile::PostLoad() {  
 if ( atlasTilesetName.length > 0 && atlasTileName.length > 0 ) {
  ats=atlasTilesets.GetTileset(atlasTilesetName.c_str());
  at=atlasTilesets.GetTile(ats,atlasTileName.c_str());
 }
}