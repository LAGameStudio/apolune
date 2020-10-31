#pragma once

#include "Art2d.h"
#include "Atlas.h"
#include "ZIndexed.h"

class AtlasLookupGrid : public Indexed<Zint> {
public:
 Zint _w,_h;
 void Init() {
  _w=(int)w;
  _h=(int)h;
  for ( int i=0; i<(int)length; i++ ) list[(unsigned int)i]=-1;
 }
 void MirrorRight() {
  for ( unsigned int j=0; j<(unsigned int)_h.value; j++ ) {
   for ( unsigned int i=0; i<(unsigned int)_w.value/2; i++ ) {
    unsigned int x=i, y=j, x2=_w-1-i, y2=j;
	   int temp=list[this->_xy(x2,y2)];
    list[this->_xy(x,y)].value=list[this->_xy(x2,y2)].value;
	   list[this->_xy(x2,y2)].value=temp;
   }
  }
 }
 void MirrorDown() {
  for ( unsigned int j=0; j<(unsigned int)_h.value/2; j++ ) {
   for ( unsigned int i=0; i<(unsigned int)_w.value; i++ ) {
    unsigned int x=i, y=j, x2=i, y2=_h-1-j;
	   int temp=list[this->_xy(x2,y2)];
    list[this->_xy(x,y)]=list[this->_xy(x2,y2)];
	   list[this->_xy(x2,y2)]=temp;
   }
  }
 }
 void MirrorUp() {
  for ( unsigned int j=0; j<(unsigned int)_h.value/2; j++ ) {
   for ( unsigned int i=0; i<(unsigned int)_w.value; i++ ) {
    unsigned int x=i, y=j, x2=i, y2=_h-1-j;
	   int temp=list[this->_xy(x2,y2)];
    list[this->_xy(x,y)]=list[this->_xy(x2,y2)];
	   list[this->_xy(x2,y2)]=temp;
   }
  }
 }
 void MirrorLeft() {
  for ( unsigned int j=0; j<(unsigned int)_h.value; j++ ) {
   for ( unsigned int i=_w.value/2; i<(unsigned int)_w.value; i++ ) {
    unsigned int x=i, y=j, x2=_w-1-i, y2=j;
	   int temp=list[this->_xy(x2,y2)];
    list[this->_xy(x,y)].value=list[this->_xy(x2,y2)].value;
	   list[this->_xy(x2,y2)].value=temp;
   }
  }
 }
 void FlipHoriz() {
  for ( unsigned int j=0; j<(unsigned int)_h.value; j++ ) {
   for ( unsigned int i=0; i<(unsigned int)_w.value/2; i++ ) {
    unsigned int x=i, y=j, x2=_w-1-i, y2=j;
	   int temp=list[this->_xy(x,y)];
    list[this->_xy(x,y)].value=list[this->_xy(x2,y2)].value;
	   list[this->_xy(x2,y2)].value=temp;
   }
  }
 }
 void FlipVert() {
  for ( unsigned int j=0; j<(unsigned int)_h.value/2; j++ ) {
   for ( unsigned int i=0; i<(unsigned int)_w.value; i++ ) {
    unsigned int x=i, y=j, x2=i, y2=_h-1-j;
	   int temp=list[this->_xy(x,y)];
    list[this->_xy(x,y)]=list[this->_xy(x2,y2)];
	   list[this->_xy(x2,y2)]=temp;
   }
  }
 }
 void RotateLeft() {
 }
 void RotateRight() {
  for ( unsigned int i=0;  i<(unsigned int)_w.value/2; i++ ) {
   for ( unsigned int j=0; j<(unsigned int)_h.value/2; j++ ) {
    unsigned int x=i, y=j, x2=j, y2=i;
 	   int temp=list[this->_xy(x,y)];
     list[this->_xy(x,y)]=list[this->_xy(x2,y2)];
 	   list[this->_xy(x2,y2)]=temp;
   }
  }
  for ( unsigned int i=(unsigned int)_w.value/2; i<(unsigned int)_w.value; i++ ) {
   for ( unsigned int j=(unsigned int)_h.value/2; j<(unsigned int)_h.value; j++ ) {
    unsigned int x=i, y=j, x2=j, y2=i;
 	   int temp=list[this->_xy(x,y)];
     list[this->_xy(x,y)]=list[this->_xy(x2,y2)];
 	   list[this->_xy(x2,y2)]=temp;
   }
  }
 }
 KEYWORDS({
  NUMWORD("w",_w);
  NUMWORD("h",_h);
  KEYWORD("values",{
   Zstring inside(w);
   this->Size((size_t)(int)_w,(size_t)(int)_h);
   for ( int i=0; i<(int) length; i++ ) {
    const char *v=inside.Next();
    list[(unsigned int)i]=atoi(v);
   }  
  });
 })
 WORDKEYS({
  WORDKEY("w",I2S(w));
  WORDKEY("h",I2S(h));
  Zstring values; for ( int i=0; i<(int) length; i++ ) { values+=I2S((*this)[(unsigned int)i])+string(" "); }
  WORDKEY("values",values);
 })
};

class AtlasTileset;
ONE(AtlasTile,{
 level1.Size((size_t)2,(size_t)2);   level1.Init();
 level2.Size((size_t)4,(size_t)4);   level2.Init();
 level3.Size((size_t)8,(size_t)8);   level3.Init();
 level4.Size((size_t)16,(size_t)16); level4.Init();
})
 AtlasLookupGrid level1,level2,level3,level4;
 Rotated2dQuads quads1,quads2,quads3,quads4,quads;
 Zp<AtlasTileset> ats; // Heightmap
 Zp<AtlasTile> at;
 Zstring atlasTilesetName,atlasTileName;
 ///
 Zint stashed_id;
 TwoDeeTriangles triangles;
 CLONE(AtlasTile,{
  a->level1.Size(level1.w,level1.h); for ( int i=0; i<(int)a->level1.w; i++ ) for ( int j=0; j<(int)a->level1.h; j++ ) a->level1(i,j)=level1(i,j);
  a->level2.Size(level2.w,level2.h); for ( int i=0; i<(int)a->level2.w; i++ ) for ( int j=0; j<(int)a->level2.h; j++ ) a->level2(i,j)=level2(i,j);
  a->level3.Size(level3.w,level3.h); for ( int i=0; i<(int)a->level3.w; i++ ) for ( int j=0; j<(int)a->level3.h; j++ ) a->level3(i,j)=level3(i,j);
  a->level4.Size(level4.w,level4.h); for ( int i=0; i<(int)a->level4.w; i++ ) for ( int j=0; j<(int)a->level4.h; j++ ) a->level4(i,j)=level4(i,j);
  a->quads.Duplicate(&quads);
  a->quads1.Duplicate(&quads1);
  a->quads2.Duplicate(&quads2);
  a->quads3.Duplicate(&quads3);
  a->quads4.Duplicate(&quads4);
  DUPE(atlasTilesetName)
  DUPE(atlasTileName)
  a->PostLoad();
 })
 void FlipHoriz() { level1.FlipHoriz(); level2.FlipHoriz(); level3.FlipHoriz(); level4.FlipHoriz(); }
 void FlipVert() { level1.FlipVert(); level2.FlipVert(); level3.FlipVert(); level4.FlipVert(); }
 void RotateLeft() { level1.RotateLeft(); level2.RotateLeft(); level3.RotateLeft(); level4.RotateLeft(); }
 void RotateRight() { level1.RotateRight(); level2.RotateRight(); level3.RotateRight(); level4.RotateRight(); }
 void MirrorRight() { level1.MirrorRight(); level2.MirrorRight(); level3.MirrorRight(); level4.MirrorRight(); }
 void MirrorDown() { level1.MirrorDown(); level2.MirrorDown(); level3.MirrorDown(); level4.MirrorDown(); }
 void MirrorUp() { level1.MirrorUp(); level2.MirrorUp(); level3.MirrorUp(); level4.MirrorUp(); }
 void MirrorLeft() { level1.MirrorLeft(); level2.MirrorLeft(); level3.MirrorLeft(); level4.MirrorLeft(); }
 void SetAtlasTile( AtlasTileset *ats, AtlasTile *at );
 void Update( Atlas *in ) {
  if ( stashed_id != 0 ) art.stash.Replace(stashed_id);
  OUTPUTLong("UPDATING AtlasTile -> Stashing tile:\n%s\n",toString().c_str());
  triangles.Clear();
  toTriangles(&triangles,in);
  stashed_id=art.stash.GenTriangles(&triangles);
 }
 void Release() {
  if ( stashed_id != 0 ) {
   art.stash.Replace(stashed_id);
   stashed_id=0;
  }
 }
 void toTriangles(TwoDeeTriangles *out, Atlas *in ) {
  EACH(quads1.first,Rotated2dQuad,q) q->toTriangles(out);
  {
   double deltaX=1.0/(double)level1.w;
   double deltaY=1.0/(double)level1.h;
   double deltaXd2=deltaX/2.0;
   double deltaYd2=deltaY/2.0;
   for ( int i=0; i<(int)level1.w; i++ ) {
    for ( int j=0; j<(int)level1.h; j++ ) if ( level1(i,j) >=0 ) {
     int lookup=level1(i,j);
     Cartesiand atlasCoords;
     if ( in->uniformGrid ) {
      double ax=lookup%in->columns / (double)in->columns;
      double ay=lookup/in->columns / (double)in->rows;
      atlasCoords.SetRect( ax, ay, (1.0/(double)in->columns), (1.0/(double)in->rows) );
     } else {
      atlasCoords.Set(&in->coords[in->GetElement(lookup)]);
     }
     Rotated2dQuad rquad;
     rquad.x=deltaX*(double)i+deltaXd2;
     rquad.y=deltaY*(double)j+deltaYd2;
     rquad.w=deltaX;
     rquad.h=deltaY;
     rquad.tc.SetRect(atlasCoords.x,atlasCoords.y,atlasCoords.w,atlasCoords.h);
     rquad.toTriangles(out);
    }
   }
  }
  EACH(quads2.first,Rotated2dQuad,q) q->toTriangles(out);
  {
   double deltaX=1.0/(double)level2.w;
   double deltaY=1.0/(double)level2.h;
   double deltaXd2=deltaX/2.0;
   double deltaYd2=deltaY/2.0;
   for ( int i=0; i<(int)level2.w; i++ ) {
    for ( int j=0; j<(int)level2.h; j++ ) if ( level2(i,j) >=0 ) {
     int lookup=level2(i,j);
     Cartesiand atlasCoords;
     if ( in->uniformGrid ) {
      double ax=lookup%in->columns / (double)in->columns;
      double ay=lookup/in->columns / (double)in->rows;
      atlasCoords.SetRect( ax, ay, (1.0/(double)in->columns), (1.0/(double)in->rows) );
     } else {
      atlasCoords.Set(&in->coords[in->GetElement(lookup)]);
     }
     Rotated2dQuad rquad;
     rquad.x=deltaX*(double)i+deltaXd2;
     rquad.y=deltaY*(double)j+deltaYd2;
     rquad.w=deltaX;
     rquad.h=deltaY;
     rquad.tc.SetRect(atlasCoords.x,atlasCoords.y,atlasCoords.w,atlasCoords.h);
     rquad.toTriangles(out);
    }
   }
  }
  EACH(quads3.first,Rotated2dQuad,q) q->toTriangles(out);
  {
   double deltaX=1.0/(double)level3.w;
   double deltaY=1.0/(double)level3.h;
   double deltaXd2=deltaX/2.0;
   double deltaYd2=deltaY/2.0;
   for ( int i=0; i<(int)level3.w; i++ ) {
    for ( int j=0; j<(int)level3.h; j++ ) if ( level3(i,j) >=0 ) {
     int lookup=level3(i,j);
     Cartesiand atlasCoords;
     if ( in->uniformGrid ) {
      double ax=(double)(lookup%in->columns) / (double)in->columns;
      double ay=(double)(lookup/in->columns) / (double)in->rows;
      atlasCoords.SetRect( ax, ay, (1.0/(double)in->columns), (1.0/(double)in->rows) );
     } else {
      atlasCoords.Set(&in->coords[in->GetElement(lookup)]);
     }
     Rotated2dQuad rquad;
     rquad.x=deltaX*(double)i+deltaXd2;
     rquad.y=deltaY*(double)j+deltaYd2;
     rquad.w=deltaX;
     rquad.h=deltaY;
     rquad.tc.SetRect(atlasCoords.x,atlasCoords.y,atlasCoords.w,atlasCoords.h);
     rquad.toTriangles(out);
    }
   }
  }
  EACH(quads4.first,Rotated2dQuad,q) q->toTriangles(out);
  {
   double deltaX=1.0/(double)level4.w;
   double deltaY=1.0/(double)level4.h;
   double deltaXd2=deltaX/2.0;
   double deltaYd2=deltaY/2.0;
   for ( int i=0; i<(int)level4.w; i++ ) {
    for ( int j=0; j<(int)level4.h; j++ ) if ( level4(i,j) >=0 ) {
     int lookup=level4(i,j);
     Cartesiand atlasCoords;
     if ( in->uniformGrid ) {
      double ax=lookup%in->columns / (double)in->columns;
      double ay=lookup/in->columns / (double)in->rows;
      atlasCoords.SetRect( ax, ay, (1.0/(double)in->columns), (1.0/(double)in->rows) );
     } else {
      atlasCoords.Set(&in->coords[in->GetElement(lookup)]);
     }
     Rotated2dQuad rquad;
     rquad.x=deltaX*(double)i+deltaXd2;
     rquad.y=deltaY*(double)j+deltaYd2;
     rquad.w=deltaX;
     rquad.h=deltaY;
     rquad.tc.SetRect(atlasCoords.x,atlasCoords.y,atlasCoords.w,atlasCoords.h);
     rquad.toTriangles(out);
    }
   }
  }
  EACH(quads.first,Rotated2dQuad,q) q->toTriangles(out);
 }
 void Render( Crayon tint, Atlas *in, double x, double y, double w, double h ) {
  if ( stashed_id > 0 && !art.stash.stashed[stashed_id]->replace ) {
   art.Element(in->texture,tint,transparency,stashed_id,x,y,w,h);
  }
 }
 void Render( Crayon tint, Blends b, Atlas *in, double x, double y, double w, double h ) {
  if ( stashed_id > 0 && !art.stash.stashed[stashed_id]->replace ) {
   art.Element(in->texture,tint,b,stashed_id,x,y,w,h);
  }
 }
 KEYWORDS({
  TXTWORD("name",name)
  else SUBWORD("L1",level1)
  else SUBWORD("L2",level2)
  else SUBWORD("L3",level3)
  else SUBWORD("L4",level4)
  else SUBWORD("quads",quads)
  else SUBWORD("quads1",quads1)
  else SUBWORD("quads2",quads2)
  else SUBWORD("quads3",quads3)
  else SUBWORD("quads4",quads4)
  else TXTWORD("atSet",atlasTilesetName)
  else TXTWORD("atTile",atlasTileName)
 })
 WORDKEYS({ 
  TXTKEY("name",name)
  SSUBKEY("L1",level1)
  SSUBKEY("L2",level2)
  SSUBKEY("L3",level3)
  SSUBKEY("L4",level4)
  SSUBKEY("quads",quads)
  SSUBKEY("quads1",quads)
  SSUBKEY("quads2",quads)
  SSUBKEY("quads3",quads)
  SSUBKEY("quads4",quads)
  if ( at ) { TXTKEY("atSet",atlasTilesetName) TXTKEY("atTile",atlasTileName) }
 })
 void PostLoad();
MANY(AtlasTile,AtlasTileHandle,AtlasTileHandles,"AtlasTile",AtlasTiles,{})
 CALLEACH(AtlasTile,PostLoad)
 KEYSWORDSGroups("AtlasTile",AtlasTile)
 void Update( Atlas *in ) {
  FOREACH(AtlasTile,at) at->Update(in);
 }
 void Release() {
  FOREACH(AtlasTile,at) at->Release();
 }
DONE(AtlasTile);

ONE(AtlasTileset,{ tint.Pick(alabaster); })
 Zstring atlasName;
 AtlasTiles tiles;
 Crayon tint;
 Zbool flipX,flipY;
///
 Zp<Atlas> atlas;
 KEYWORDS_POST({
  TXTWORD("name",name)
  else TXTWORD("atlas",atlasName)
  else SUBWORD("tiles",tiles)
  else TINTWORD("tint",tint)
  else TAGWORD("flipX",flipX)
  else TAGWORD("flipY",flipY)
  NEXTWORD
 },{
  atlas=atlasLibrary.named(atlasName.c_str());
  if (atlas) tiles.Update(atlas);
 })
 WORDKEYS({
  TXTKEY("name",name)
  if ( atlas ) TXTKEY("atlas",atlas->name);
  SSUBKEY("tiles",tiles)
  TINTKEY("tint",tint)
  TAGKEY("flipX",flipX)
  TAGKEY("flipY",flipY)
 })
  void PostLoad() {
   tiles.PostLoad();
  }
MANY(AtlasTileset,AtlasTilesetHandle,AtlasTilesetHandles,"AtlasTileset",AtlasTilesets,{})
 CALLEACH(AtlasTileset,PostLoad)
 KEYSWORDSGroups("AtlasTileset",AtlasTileset)
 AtlasTileset *GetTileset( const char *set ) {
  AtlasTileset *ats=named(set);
  return ats;
 }
 AtlasTile *GetTile( AtlasTileset *ats, const char *tile ) {
  if ( ats ) return ats->tiles.named(tile);
  return null;
 }
 AtlasTile *Lookup( const char *set, const char *tile ) {
  AtlasTileset *ats=named(set);
  if ( ats ) return ats->tiles.named(tile);
  return null;
 }
 void Load( AtlasTilesetHandles *out, const char *filename ) {
  Zstring fileString(file_as_string(filename).c_str());
  const char *w=fileString.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"atlastileset") ) {
    w=fileString.Next();
    AtlasTileset *at=new AtlasTileset;
    at->fromString(w);
    Append(at);
    out->Add(at);
   } else BADKEY("AtlasTilesets")
   w=fileString.Next();
  }
 }
 void Save( AtlasTilesetHandles *out, const char *filename ) {
  Zstring output;
  EACH(out->first,AtlasTilesetHandle,ath) {
   output+=string("atlastileset {")+ath->p->toString()+string("}\n");
  }
  string_as_file(output.c_str(),filename);
 }
DONE(AtlasTileset);

extern AtlasTilesets atlasTilesets;