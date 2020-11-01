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
#pragma once

#include "GLImage.h"
#include "Indexed.h"
#include "StringIndexed.h"
#include "Vertex.h"
#include "Art2d.h"

// Implements support for TexturePacker files.
ONE(AtlasFrame,{})
 Cartesiand extents,sourceSize,spriteSourceSize,pivot;
 Zbool texturePacker,trimmed,rotated;
 Zint stash_id;
 void GenGeo( bool mirrored, GLImage *source, Indexed<int> *elements, int k );
 KEYWORDS({
  KEYWORD("name",{name=w;})
  TAGWORD("texturePacker",texturePacker)
  TAGWORD("trimmed",trimmed)
  TAGWORD("rotated",rotated)
  SUBWORD("extents",extents)
  SUBWORD("size",sourceSize)
  SUBWORD("sprite",spriteSourceSize)
  SUBWORD("pivot",pivot)
 })
 WORDKEYS({
  WORDKEY("name",name)
  TAGKEY("texturePacker",texturePacker)
  TAGKEY("trimmed",trimmed)
  TAGKEY("rotated",rotated)
  SSUBKEY("extents",extents)
  SSUBKEY("size",sourceSize)
  SSUBKEY("sprite",spriteSourceSize)
  SSUBKEY("pivot",pivot)
 })
MANY(AtlasFrame,AtlasFrameHandle,AtlasFrameHandles,"frame",AtlasFrames,{})
 KEYSWORDSGroups("frame",AtlasFrame);
 void GenGeo( bool mirrored, GLImage *source, Indexed<int> *elements ) {
  int k=0;
  if ( mirrored ) elements->Size((size_t)(int)count*4);
  else elements->Size((size_t)(int)count);
  FOREACH(AtlasFrame,af) {
   af->GenGeo(mirrored,source,elements,k);
   k++;
  }
 }
 Indexed<Zp<AtlasFrame>> lookup;
 void BuildLookup() {
  lookup.Size((size_t)(int)count);
  int i=0; FOREACH(AtlasFrame,af) { lookup[i]=af; i++; }
 }
DONE(AtlasFrame);

ONE(AtlasFilm,{})
 Numbers keyframes;
 Zdouble duration;
MANY(AtlasFilm,AtlasFilmHandle,AtlasFilmHandles,"AtlasFilm",AtlasFilms,{})
DONE(AtlasFilm);

/*
 * Implementation of a Texture Atlas.
 */
ONE(Atlas,{})
 Zbool dontGenElements;
 Zstring textureName,heightName,normalName;
 Zp<GLImage> texture,height,normal;  // Alternative images can be provided and indexed.
 Zbool uniformGrid,nearest,mirrored;
 Zint rows,columns;
 Indexed<Cartesiand> coords;
 Indexed<int> elements;
 Cartesiand sourceSize;
 Cartesiands centerOffsets,individuals;
 Cartesiand size; Coord2i dimensions;
 AtlasFrames frames;  // Loaded packed quads from file are converted here.
 Atlas( GLImage *source, int rows, int cols ) : ListItem() { Create(source,rows,cols); }
 Atlas( GLImage *source, Cartesiands *orderedElements ) : ListItem() { Create(source,orderedElements); }
 void Create( GLImage *source, int rows, int cols ) {
  this->texture=source;
  uniformGrid=true;
  dimensions.Set(rows,cols);
  size.SetRect(0,0,iratiod(source->width,cols),iratiod(source->height,rows));
  this->rows=rows;
  this->columns=cols;
  if ( rows > 0 && cols > 0 ){
   coords.Size((size_t)cols,(size_t)rows);
   for ( int i=0; i<cols; i++ ) for ( int j=0; j<rows; j++ ) {
    coords(i,j).SetRect(size.w*i,size.h*j,size.w,size.h);
//    individuals.Append(coords(i,j).Duplicate());
   }
   GenElements();
  }
 }
 void Create( GLImage *source, Cartesiands *orderedElements ) {
  this->texture=source;
  uniformGrid=false;
  coords.Size((size_t)orderedElements->count);
  int i=0;
  EACH(orderedElements->first,Cartesiand,c) { /*individuals.Append(c->Duplicate());*/ coords(i).Set(c); }
  GenElements();
 }
 void Create( GLImage *source ) {
  this->texture=source;
  uniformGrid=false;
  coords.Size((size_t)frames.count);
  frames.GenGeo(mirrored,source,&elements);
  elements.Size(coords.length);
 }
 void RenderPreview( int x, int y, int w, int h ) {
  art.Stretch(texture,crayons.Pick(alabaster),x,y,w,h);
 }
 void GenElements();
 int GetElement( int i )       { return elements(i); }
 int GetElementFlipX( int i )  { return elements(i*2); }
 int GetElementFlipXY( int i ) { return elements(i*3); }
 int GetElementFlipY( int i )  { return elements(i*4); }
 KEYWORDS_POST({
  KEYWORD("name",{name=w;})
  else KEYWORD("texture",{textureName=w;})
  else KEYWORD("height",{heightName=w;})
  else KEYWORD("normal",{normalName=w;})
  else TAGWORD("grid",uniformGrid)
  else TAGWORD("nearest",nearest)
  else TAGWORD("mirrored",mirrored)
  else TAGWORD("dontGenElements",dontGenElements)
  else NUMWORD("rows",rows)
  else NUMWORD("columns",columns)
  else NUMWORD("cols",columns)
  else SUBWORD("elements",frames)
  else SUBWORD("centers",centerOffsets)
  else BADKEYWORD;
  NEXTWORD;
 },{
  if ( file_exists(heightName.c_str()) ) {
   if ( mirrored ) {
    if ( nearest ) height=library.Load(heightName.c_str(),tllto_Mirrored_Nearest);
    else height=library.Load(heightName.c_str(),tllto_Mirrored);
   } else {
    if ( nearest ) height=library.Load(heightName.c_str(),tllto_Mirrored_Nearest);
    else height=library.Load(heightName.c_str(),tllto_Mirrored);
   }
  }
  if ( file_exists(normalName.c_str()) ) {
   if ( mirrored ) {
    if ( nearest ) normal=library.Load(normalName.c_str(),tllto_Mirrored_Nearest);
    else normal=library.Load(normalName.c_str(),tllto_Mirrored);
   } else {
    if ( nearest ) normal=library.Load(normalName.c_str(),tllto_Mirrored_Nearest);
    else normal=library.Load(normalName.c_str(),tllto_Mirrored);
   }
  }
  if ( file_exists(textureName.c_str()) ) {
   if ( mirrored ) {
    if ( nearest ) texture=library.Load(textureName.c_str(),tllto_Mirrored_Nearest);
    else texture=library.Load(textureName.c_str(),tllto_Mirrored);
   } else {
    if ( nearest ) texture=library.Load(textureName.c_str(),tllto_Mirrored_Nearest);
    else texture=library.Load(textureName.c_str(),tllto_Mirrored);
   }
   if ( uniformGrid ) Create(texture,rows,columns);
   else Create(texture);
  }
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("texture",textureName)
  TAGKEY("grid",uniformGrid)
  TAGKEY("nearest",nearest)
  TAGKEY("mirrored",mirrored)
  NUMBKEY("rows",rows)
  NUMBKEY("columns",columns)
  SSUBKEY("elements",frames)
  SSUBKEY("centers",centerOffsets)
  TAGKEY("dontGenElements",dontGenElements)
 })
MANY(Atlas,AtlasHandle,AtlasHandles,"Atlas",AltasLibrary,{})
 KEYSWORDSGroups("Atlas",Atlas)
  void Load( const char *p ) {
   if ( file_exists(p) ) {
    fromString(file_as_string(p).c_str());
   }
  }
DONE(Atlas);

#define DEFAULT_ATLASTEXT_LINGER (600)
ONE(AtlasFontGlyph,{c='\0';})
 Cartesiand texCoords,offsetSize,finetune;
 Zint ord;
 Zstring letter;
 char c;
 void CalculateTexCoords( double x, double y, double w, double h, double textureW, double textureH ) {
  offsetSize.SetRect(x,y,w,h);
  texCoords.SetRect(
   offsetSize.x/textureW,
   offsetSize.y/textureH,
   offsetSize.w/textureW,
   offsetSize.h/textureH
  );
 }
MANY(AtlasFontGlyph,AtlasFontGlyphHandle,AtlasFontGlyphHandles,"AtlasFontGlyph",AtlasFontGlyphLibrary,{})
 void Add( int ord, char letter, double x, double y, double ox, double oy, double w, double h, double textureW, double textureH ) {
  AtlasFontGlyph *afg=new AtlasFontGlyph;
  afg->ord=ord;
  afg->letter=FORMAT("%c",letter);
  afg->name=afg->letter;
  afg->c=(letter);
  afg->finetune.Set(ox,oy);
  afg->CalculateTexCoords(x,y,w,h,textureW,textureH);
  Append(afg);
  OUTPUT("bmfont GLYPH: %c (%d) xy: %1.1f,%1.1f size: %1.1f x %1.1f in %1.1f x %1.1f +fine: %1.1f,%1.1f TC: %1.1f,%1.1f -> %1.1f,%1.1f\n",
   URANGE(' ',letter,'~'),ord,x,y,w,h,textureW,textureH,ox,oy,
   (double)afg->texCoords.x,(double)afg->texCoords.y,(double)afg->texCoords.x2,(double)afg->texCoords.y2
  );
 }
 AtlasFontGlyph *Ord( int id ) { FOREACH(AtlasFontGlyph,afg) if ( afg->ord == id ) return afg; return null; }
 AtlasFontGlyph *Ord( char c ) { FOREACH(AtlasFontGlyph,afg) if ( afg->c == c ) return afg; return null; }
DONE(AtlasFontGlyph);

extern Zint globalFrameCounter;
ONE(AtlasText,{ destroyWhen=globalFrameCounter+DEFAULT_ATLASTEXT_LINGER; linger=DEFAULT_ATLASTEXT_LINGER; })
 Zint geometry;
 Zint destroyWhen,linger;
 Zstring content;
 Cartesiand bounds;
 AtlasText( const char *content, int geometry, int linger=DEFAULT_ATLASTEXT_LINGER ) : ListItem() {
  this->geometry=geometry;
  this->content=content;
  this->linger=linger;
  this->Refresh();
 }
 void Render( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h ) {
  art.Element(source,tint,blend,x,y,w,h);
 }
 void Refresh() { this->destroyWhen=globalFrameCounter+linger; }
 void GetThisStringIndex( Zstring *out ) { (*out) = content; }
 ~AtlasText() {
  art.stash.Replace(this->geometry);
 }
MANY(AtlasText,AtlasTextHandle,AtlasTextHandles,"AtlasText",AtlasTextCache,{})
 StringIndexed<AtlasText> cache;
 void Between() {
  AtlasText *a=cache.Next();
  if ( a ) {
   if ( a->destroyWhen < globalFrameCounter ) {
    cache.Remove();
    delete a;
   }
  }
 }
 AtlasText *Add( const char *content, int geometry, Cartesiand *bounds, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=new AtlasText(content,geometry,linger);
  at->bounds.Set(bounds);
  cache.Store(content,at);
  return at;
 }
DONE(AtlasText);

ONE(AtlasFont,{ glyphSheet=-1; })
 Zbool error;
 Zbool nearest,bmfont;
 Atlas atlas;
 AtlasFontGlyphLibrary glyphs;
 Strings legend;
 Zstring imgfile,glyphfile;
 Zdouble base,spaceWidth,lineHeight,kern;
 Zint rows,columns;
 Cartesiand stride;
 Zint glyphSheet,fontNameText;
 Zbool glyphSheetGenerated;
 AtlasTextCache atlasTextCache;
 void Between() { atlasTextCache.Between(); }
 void GenGlyphSheet() {
  if ( glyphSheet > 0 ) art.stash.Replace(glyphSheet);
  Zstring genText;
  EACH(glyphs.first,AtlasFontGlyph,afg) genText+=afg->c;
  if ( genText.length > 0 ) glyphSheet=GenText(genText.c_str(),1024.0,false);
  fontNameText=GenText(name.c_str());
 }
 // Generates text using using the x/y scale method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double w, double h, double xScale, double yScale, bool centered, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xScale,yScale,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x,y,w,h);
 }
 // Generates text using using the x/y scale method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double xScale, double yScale, bool centered, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xScale,yScale,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x,y+at->bounds.y,at->bounds.w,at->bounds.h);
 }
 // Generates text using using the x/y scale method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double scale, double xScale, double yScale, bool centered, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xScale,yScale,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x*scale,y+at->bounds.y*scale,at->bounds.w*scale,at->bounds.h*scale);
 }
 // Generates text using using the x/y scale method
 void BoundaryGenText( Cartesiand *out, const char *s, double xScale, double yScale, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    y+=lineHeight*yScale;
    x=0.0;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=lineHeight*yScale;
    x=0.0;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x+g->finetune.x*xScale;
   q->y=y+g->finetune.y*yScale;
   q->x2=q->x+g->offsetSize.w*xScale;
   q->y2=q->y+g->offsetSize.h*yScale;
   quads.Append(q);
   x+=(g->finetune.x+g->offsetSize.w+kern)*xScale;
   p++;
  }
  quads.Bounds2dRect(out);
 }
 // Generates text using using the x/y scale method
 int GenText( const char *s, double xScale, double yScale, Cartesiand *out=null, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads,texcoords;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    y+=lineHeight*yScale;
    x=0.0;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=lineHeight*yScale;
    x=0.0;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x+g->finetune.x*xScale;
   q->y=y+g->finetune.y*yScale;
   q->x2=q->x+g->offsetSize.w*xScale;
   q->y2=q->y+g->offsetSize.h*yScale;
   quads.Append(q);
   texcoords.Append(g->texCoords.Duplicate());
   x+=(g->finetune.x+g->offsetSize.w+kern)*xScale;
   p++;
  }
  if ( out ) quads.Bounds2dRect(out);
  if ( fitTo1 ) quads.FitTo1();
  return art.stash.GenQuads(&quads,&texcoords);
 }
 // Generates text using using the x/y fit method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double w, double h, having fit, double xFit, double yFit, double lineSpace, double charSpace, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xFit,yFit,lineSpace,charSpace,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x,y,w,h);
 }
 // Generates text using using the x/y fit method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, having fit, double xFit, double yFit, double lineSpace, double charSpace, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xFit,yFit,lineSpace,charSpace,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x,y+at->bounds.y,at->bounds.w,at->bounds.h);
 }
 // Generates text using using the x/y fit method
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double scale, double xFit, double yFit, double lineSpace, double charSpace, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,xFit,yFit,lineSpace,charSpace,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x*scale,y+at->bounds.y*scale,at->bounds.w*scale,at->bounds.h*scale);
 }
 // Generates text using using the x/y fit method
 void BoundaryGenText( Cartesiand *out, const char *s, double xFit, double yFit, double lineSpace, double charSpace, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads;
  double _lineheight=yFit+lineSpace;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    y+=_lineheight;
    x=0.0;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=_lineheight;
    x=0.0;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x;
   q->y=y;
   q->x2=q->x+xFit;
   q->y2=q->y+yFit;
   quads.Append(q);
   x+=xFit+charSpace;
   p++;
  }
  quads.Bounds2dRect(out);
//  if ( fitTo1 ) quads.FitTo1();
 }
 // Generates text using using the x/y fit method
 int GenText( const char *s, double xFit, double yFit, double lineSpace, double charSpace, Cartesiand *out=null, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads,texcoords;
  double _lineheight=yFit+lineSpace;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    y+=_lineheight;
    x=0.0;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=_lineheight;
    x=0.0;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x;
   q->y=y;
   q->x2=q->x+xFit;
   q->y2=q->y+yFit;
   quads.Append(q);
   texcoords.Append(g->texCoords.Duplicate());
   x+=xFit+charSpace;
   p++;
  }
  if ( out ) quads.Bounds2dRect(out);
  if ( fitTo1 ) quads.FitTo1();
  return art.stash.GenQuads(&quads,&texcoords);
 }
 // Generates text using the default settings inherent to the font
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double w, double h, having defaultSetup, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x,y,w,h);
 }
 // Generates text using the default settings inherent to the font
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, having defaultSetup, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x,y+at->bounds.y,at->bounds.w,at->bounds.h);
 }
 // Generates text using the default settings inherent to the font
 void Text( const char *s, Crayon tint, Blends blend, double x, double y, double scale, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true, int linger=DEFAULT_ATLASTEXT_LINGER ) {
  AtlasText *at=atlasTextCache.cache.Get(s);
  if ( !at ) {
   Cartesiand bounds;
   at=atlasTextCache.Add(s,GenText(s,&bounds,centered,wrapWidth,cutToEol,fitTo1),&bounds,linger);
  } else at->Refresh();
  at->Render(atlas.texture,tint,blend,x+at->bounds.x*scale,y+at->bounds.y*scale,at->bounds.w*scale,at->bounds.h*scale);
 }
 // Generates text using the default settings inherent to the font
 void BoundaryGenText( Cartesiand *out, const char *s, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    x=0.0;
    y+=lineHeight;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=lineHeight;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x+g->finetune.x;
   q->y=y+g->finetune.y;
   q->x2=q->x+g->offsetSize.w;
   q->y2=q->y+g->offsetSize.h;
   quads.Append(q);
   x+=g->finetune.x+g->offsetSize.w+kern;
   p++;
  }
  quads.Bounds2dRect(out);
 }
 // Generates text using the default settings inherent to the font
 int GenText( const char *s, Cartesiand *out=null, bool centered=false, double wrapWidth=0.0, bool cutToEol=true, bool fitTo1=true ) {
  Cartesiands quads,texcoords;
  double x=0.0,y=0.0;
  const char *p=s;
  while ( *p != '\0' ) {
   while ( *p == '\r' ) p++;
   if ( wrapWidth > 0.0 && x > wrapWidth ) {
    x=0.0;
    y+=lineHeight;
    if ( cutToEol ) {
     while ( *p != '\n' && *p != '\0' ) p++;
     while ( *p == '\r' ) p++;
     if ( *p == '\0' ) break;
    }
   }
   if ( *p == ' ' ) { x+=spaceWidth; p++; continue; }
   if ( *p == '\t' ) { x+=2*spaceWidth; p++; continue; }
   AtlasFontGlyph *g=glyphs.Ord(*p);
   if ( *p == '\n' ) {
    y+=lineHeight;
   }
   if ( !g ) g=glyphs.Ord('?');
   if ( !g ) { x+=spaceWidth; p++; continue; }
   Cartesiand *q=new Cartesiand;
   q->x=x+g->finetune.x;
   q->y=y+g->finetune.y;
   q->x2=q->x+g->offsetSize.w;
   q->y2=q->y+g->offsetSize.h;
   quads.Append(q);
   texcoords.Append(g->texCoords.Duplicate());
   x+=g->finetune.x+g->offsetSize.w+kern;
   p++;
  }
  if ( out ) quads.Bounds2dRect(out);
  if ( fitTo1 ) quads.FitTo1();
  return art.stash.GenQuads(&quads,&texcoords);
 }
 void Load() {
  if ( file_exists(imgfile.c_str()) ) {
   if ( nearest ) atlas.Create(library.Load(imgfile.c_str(),tllto_Tiling_Nearest),rows,columns);
   else atlas.Create(library.Load(imgfile.c_str()),rows,columns);
   if ( atlas.texture ) {
    if ( !bmfont ) { // Grid
     LegendToGlyphs();
    } else { // bmfont style placements (bin packed)
     LoadBMGlyphs();
    }
   }
  } else {
   error=true;
   OUTPUT("AtlasFont::Load file not found %s\n",imgfile.c_str());
  }
 }
 void LoadBMGlyphs() {
  if ( file_exists(glyphfile.c_str()) ) {
   Zstring file=file_as_string(glyphfile.c_str());
   Strings lines; lines.SplitPush(file.c_str(),'\n');
   EACH(lines.first,String,s) {
    Strings chunks; chunks.SplitPush(s->s.c_str(),' ');
    String *word=(String *)chunks.first;
    if ( str_cmp(word->s.c_str(),"char") ) continue;
    if ( chunks.count < 9 ) {
     continue;
    }
    String *c=null;
    int id=0;    c=(String*) chunks.Element(1);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { id=atoi(t->s.c_str()); } }
    int x =0;    c=(String*) chunks.Element(2);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { x =atoi(t->s.c_str()); } }
    int y =0;    c=(String*) chunks.Element(3);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { y =atoi(t->s.c_str()); } }
    int w =0;    c=(String*) chunks.Element(4);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { w =atoi(t->s.c_str()); } }
    int h =0;    c=(String*) chunks.Element(5);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { h =atoi(t->s.c_str()); } }
    int xo=0;    c=(String*) chunks.Element(6);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { xo=atoi(t->s.c_str()); } }
    int yo=0;    c=(String*) chunks.Element(7);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { yo=atoi(t->s.c_str()); } }
    int xa=0;    c=(String*) chunks.Element(8);  if (c) { Strings parts; parts.SplitPush(c->s.c_str(),'='); String *t=(String*)parts.Element(1); if (t) { xa=atoi(t->s.c_str()); } }
    char g='\0'; g=(char)id;
    glyphs.Add((int)g,g,(double)x,(double)y,(double)xo,(double)yo,(double)w,(double)h,(double)atlas.texture->width,(double)atlas.texture->height);
   }
  } else OUTPUT("AtlasFont::Load(`%s`) glyph file not found: %s\n",name.c_str(),glyphfile.c_str());
 }
 void LegendToGlyphs() {
  stride.Set(iratiod(atlas.texture->width,atlas.dimensions.x),iratiod(atlas.texture->height,atlas.dimensions.y));
  if ( legend.count == 0 ) { // OEM437 (0-255 ASCII)
   int k=0;
   for ( int j=0; j<rows; j++ ) for ( int i=0; i<columns; i++ ) {
    glyphs.Add(k,(char)k,(double)(i)*stride.x,(double)(j)*stride.y,0.0,0.0,stride.x,stride.y,(double)atlas.texture->width,(double)atlas.texture->height);
    k++;
   }
   return;
  }
  // Legend strings (ASCII subset)
  int j=0;
  EACH(legend.first,String,s) {
   const char *p=s->s.c_str();
   int i=0;
   while ( *p != '\0' && i < columns ) {
    glyphs.Add((int)(*p),(*p),(double)(i)*stride.x,(double)(j)*stride.y,0.0,0.0,stride.x,stride.y,(double)atlas.texture->width,(double)atlas.texture->height);
    i++;
   }
   j++;
   if ( j >= rows ) break;
  }
 }
 KEYWORDS_POST({
  KEYWORD("name",{name=w;})
  else KEYWORD("glyphs",{glyphfile=w;})
  else KEYWORD("img",{imgfile=w;})
  else TAGWORD("nearest",nearest)
  else TAGWORD("bmfont",bmfont)
  else SUBWORD("legend",legend)
  else NUMWORD("base",base)
  else NUMWORD("spaceWidth",spaceWidth)
  else NUMWORD("lineHeight",lineHeight)
  else NUMWORD("kern",kern)
  else NUMWORD("rows",rows)
  else NUMWORD("columns",columns)
  else BADKEYWORD;
  NEXTWORD;
 },{
  Load();
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("img",imgfile)
  if ( glyphfile.length > 0 ) WORDKEY("glyphs",glyphfile)
  TAGKEY("nearest",nearest)
  TAGKEY("bmfont",bmfont)
  SSUBKEY("legend",legend)
  NUMBKEY("base",base)
  NUMBKEY("spaceWidth",spaceWidth)
  NUMBKEY("lineHeight",lineHeight)
  NUMBKEY("kern",kern)
  NUMBKEY("rows",rows)
  NUMBKEY("columns",columns)
 })
MANY(AtlasFont,AtlasFontHandle,AtlasFontHandles,"AtlasFont",AtlasFonts,{})
 CALLEACH(AtlasFont,Between)
 void Add( const char *file ) {
  if ( !file_exists(file) ) {
   OUTPUT("AtlasFont::Add(`%s`) file not found\n",file);
   return;
  }
  AtlasFont *f=new AtlasFont;
  f->fromString(file_as_string(file).c_str());
  Append(f);
 }
 void Load( const char *index ) {
  Strings indexfile; indexfile.SplitPush(file_as_string(index).c_str());
  EACH(indexfile.first,String,s) Add(s->s.c_str());
 }
DONE(AtlasFont);

extern AtlasFonts atlasFonts;

void LoadAtlasFonts();

extern AltasLibrary atlasLibrary;