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


#include "ZeroTypes.h"
#include "TextureLibrary.h"
#include "Art2d.h"
#include "Cartesian.h"

using namespace std;

/*
 * Atlas-ified Tex Fonts, a variant of TexFont that uses naive atlases to assist in faster rendering.
 * Not as extensive as AtlasFont, a different implementation.  All characters must appear in a
 * specific grid.  Written as a drop-in replacement for TexFont.
 */

extern Zint ATFontCached_items;

/*
 * One of many glyphs
 * order: in order from top left 0...N,
 */

ONE(ATexGlyph,{  c='\0';  })
 char c;
 Cartesian position;
 Cartesiand texcoords;
 Cartesiand offset;
 Zint order;
 KEYWORDS({
  SUBWORD("offset",offset)
  else SUBWORD("position",position)
  else SUBWORD("texcoords",texcoords)
 })
 void Calculate( GLImage *atlas, Cartesian *padding, Cartesian *cellSize ) {
  int cols = atlas->width / (padding->x + cellSize->x);
  position.Set(order % cols,order / cols);
  texcoords.SetRect(
   ( (double) position.x * (double) (cellSize->x + padding->x) ) / (double) atlas->width,
   ( (double) position.y * (double) (cellSize->y + padding->y) ) / (double) atlas->height,
   (double) cellSize->x / (double) atlas->width,
   (double) cellSize->y / (double) atlas->height
  );
  OUTPUT(" >Glyph `%c` #%-3d position %d, %d @ %1.1f,%1.1f @ %1.1f x %1.1f\n",c,(int)order,(int)position.x,(int)position.y,(double)texcoords.x,(double)texcoords.y,(double)texcoords.w,(double)texcoords.h);
 }
MANY(ATexGlyph,ATexGlyphHandle,ATexGlyphHandles,"ATexGlyph",ATexGlyphs,{})
DONE(ATexGlyph);

class ATexFont;
ONE(ATexFontCached,{element=-1;})
 Zstring content;
 Zint element;
 Zdouble fw,fh,cs,ls;
 Cartesiand bounds;
 int Create( ATexFont *atf, const char *s, double fw, double fh, double cs, double ls );
 void ClearCache() {
  if ( element > 0 ) art.stash.Replace(element);
  ATFontCached_items--;
 }
MANY(ATexFontCached,ATexFontCachedHandle,ATexFontCachedHandles,"ATexFontCached",ATexFontCacheList,{})
 ATexFontCached *find( const char *s, double fw, double fh, double cs, double ls ) {
  FOREACH(ATexFontCached,atfc) if (atfc->fw == fw && atfc->fh == fh && atfc->cs == cs && atfc->ls == ls && atfc->content == s) return atfc;
  return null;
 }
 void ClearCache() {
  FOREACH(ATexFontCached,atfc) atfc->ClearCache();
  Clear();
 }
DONE(ATexFontCached);

#define ATexFontCache_SIZE 64
ONE(ATexFontCache,{ table.Size(ATexFontCache_SIZE); })
 ZIndexed<ATexFontCacheList> table;
 int GetHashCode( const char *s, double fw, double fh, double cs, double ls ) {
  int len=strlen(s);
  if ( len <= 0 ) return 0;
  int code1=(int)(s[0]);
  int code2=(int)(s[len/2]);
  int code3=(int)(s[len-1]);
  return (code1+code2+code3+(int)fw+(int)fh+(int)cs+(int)ls)%ATexFontCache_SIZE;
 }
 ATexFontCached *find( const char *s, double fw, double fh, double cs, double ls ) {
  int hc=GetHashCode(s,fw,fh,cs,ls);
  return table[hc].find(s,fw,fh,cs,ls);
 }
 ATexFontCached *find( int *hc, const char *s, double fw, double fh, double cs, double ls ) {
  *hc=GetHashCode(s,fw,fh,cs,ls);
  return table[(*hc)].find(s,fw,fh,cs,ls);
 }
 ATexFontCached *Store( ATexFont *atf, int *hc, const char *s, double fw, double fh, double cs, double ls );
 ATexFontCached *GetOrStore( ATexFont *atf, const char *s, double fw, double fh, double cs, double ls );
 void ClearCache() {
  for ( int i=0; i<ATexFontCache_SIZE; i++ ) table[i].ClearCache();
 }
MANY(ATexFontCache,ATexFontCacheHandle,ATexFontCacheHandles,"ATexFontCache",ATexFontCaches,{})
DONE(ATexFontCache);



ONE(ATexFont,{
 glyphs.Size(256);
 baselineOffset=0.0;
})
 ATexFontCache cache;
 Zp<GLImage> image;
 Zstring imageFilepath;
 Indexed<ATexGlyph> glyphs;
 Zbool nearest,fixed,capsonly;
 Zdouble spaceWidth, charSpace, lineSpace, baselineOffset;
 Blends blend;
 Cartesian padding,cellsize;
 Cartesian fixedSize;
 Cartesiand aspect,size;
 ATexFont( std::string s ) : ListItem() {
  name=s;
  glyphs.Size(256);
  baselineOffset=0.0;
  blend=additive;
  size.Set(14.0,17.0);
  spaceWidth=1.0;
 }
 ATexGlyph *glyph( char c ) { return &glyphs[URANGE(0,(int) c,(int)glyphs.length-1)]; }
 KEYWORDS_POST({
  KEYWORD("name",{name=w;})
  else KEYWORD("image",{ imageFilepath=w; })
  else TAGWORD("nearest",nearest)
  else TAGWORD("caps",capsonly)
  else TAGWORD("capsonly",capsonly)
  else NUMWORD("spaceWidth",spaceWidth)
  else NUMWORD("charSpace",charSpace)
  else NUMWORD("lineSpace",lineSpace)
  else NUMWORD("baseline",baselineOffset)
  else BLENDWORD("blend",blend)
  else SUBWORD("fixed",fixedSize)
  else SUBWORD("cells",cellsize)
  else SUBWORD("padding",padding)
  else SUBWORD("size",size)
  else KEYWORD("glyphs",{ if ( file_exists(w) ) { LoadGlyphs(w); } })
  else KEYWORD("glyph",{ LoadGlyph(w); })
  NEXTWORD;
 },{ Postprocess(); })
 void LoadGlyph( const char *w ) {
  Zstring interior(w);
  const char *ku=interior.Next(); // 
  int order=atoi(ku);
  ku=interior.Next_case();
  char t=(*ku);
  ku=interior.Next();
  OUTPUT("\nGLYPH: %d, %c %s\n",order,t,ku);
  int index=(int)t;
  if ( index >= 0 && index <= 255 ) {
   glyphs[index].c=t;
   glyphs[index].order=order;
   glyphs[index].fromString(interior.p);
  }
 }
 void LoadGlyphs( const char *w ) {
  std::string file=file_as_string(w);
  const char *p=file.c_str();
  int order=0;
  while ( *p != '\0' ) {
   if ( *p == '\r' || *p == '\n' ) { p++; continue; }
   char t=(*p);
   OUTPUT("GLYPH %d=%c\n",order,t,v);
   int index=(int)t;
   if ( index >= 0 && index <= 255 ) {
    glyphs[index].c=t;
    glyphs[index].order=order;
   }
   order++;
   p++;
  }
 }
// WORDKEYS({ OUTPUT("ATexFont: Not supported!\n"); return; })
 void Postprocess() {
  if ( fixedSize.x != 0 && fixedSize.y != 0 ) fixed=true;
  if ( file_exists(imageFilepath.c_str()) ) {
   if ( nearest ) image=library.Load(imageFilepath.c_str(),tllto_Nearest);
   else image=library.Load(imageFilepath.c_str());
  }
  aspect.Set(iratiod(cellsize.x,cellsize.y),iratiod(cellsize.y,cellsize.x));
  charSpace = fixed ? (double) fixedSize.x * charSpace : size.w * charSpace;
  lineSpace = fixed ? (double) fixedSize.y * charSpace : size.h * lineSpace;
  if ( image ) for ( int i=0; i<(int)glyphs.length; i++ ) glyphs[i].Calculate(image,&padding,&cellsize);
 }
 void Load( const char *fontfile ) {
  OUTPUT("ATexFont: Loading font `%s`.\n", fontfile);
  std::string file=file_as_string(fontfile);
  fromString(file.c_str());
  OUTPUT("\n");
 }
 // Calculates the output geometry for shading
 void Calculate( Cartesiand &bounds, Cartesiands *quads, Cartesiands *texcoords, const char *s, double fw, double fh, double cs, double ls );
 // Return the character width ratio to a character
 double CharWidth() { return (fixed?(double)fixedSize.x:size.w); }
 // Return the character height ratio to a character
 double CharHeight() { return (fixed?(double)fixedSize.y:size.h); }
 static void GetTextInfo(int *lines, int *widest, const char *s);
 // Write some text to the screen
 void Write( const char *s, Crayon tint, int x, int y ) { Write(s,tint,(double)x,(double)y); }
 // Write some text to the screen
 void Write( const char *s, Crayon tint, double x, double y ) { Write(s,tint,x,y,blend,CharWidth(),CharHeight(),lineSpace,charSpace); }
 // Write some text to the screen
 void Write( const char *s, Crayon tint, int x, int y, Blends b, int fontW, int fontH ) { Write( s, tint, (double)x, (double)y, b, (double) fontW, (double) fontH ); }
 // Write some text to the screen
 void Write( const char *s, Crayon tint, double x, double y, Blends b, double fontW, double fontH ) { Write(s, tint, (double)x, (double) y, b, (double) fontW, (double)fontH, (double) lineSpace, (double) charSpace); }
 // Write some text to the screen
 void Write( const char *s, Crayon tint, int x, int y, Blends b, int fontW, int fontH, int lineSpace, int charSpaceKern ) { Write(s, tint, (double)x, (double) y, b, (double) fontW, (double)fontH, (double) lineSpace, (double) charSpaceKern); }
 // Write some text to the screen
 void Write( const char *s, Crayon tint, double x, double y, Blends b, double fontW, double fontH, double lineSpace, double charSpaceKern );
 // Get the text width in pixels
 double Width( const char *s) { return Width(s,this->charSpace,CharWidth()); }
 // Get the text width in pixels
 double Width( const char *s, int charSpaceKern, int fontW ) { return Width(s, (double)charSpaceKern, (double) fontW); }
 // Get the text width in pixels
 double Width( const char *s, double charSpaceKern, double fontW );
 std::string FitWidth( const char *s, int maxWidth, int fontW, int fontH, int lineSpace, int charSpaceKern, bool onlyOneLine=true ) { return FitWidth(s,(double)maxWidth,(double)fontW,(double)fontH,(double)lineSpace,(double)charSpaceKern,onlyOneLine); }
 std::string FitWidth( const char *s, int maxWidth, bool onlyOneLine=true ) { return FitWidth(s,(double)maxWidth,CharWidth(),CharHeight(),lineSpace,charSpace,onlyOneLine); }
 std::string FitWidth( const char *s, double maxWidth, bool onlyOneLine=true ) { return FitWidth(s,maxWidth,CharWidth(),CharHeight(),lineSpace,charSpace,onlyOneLine); }
 std::string FitWidth( const char *s, double maxWidth, double fontW, double fontH, double lineSpace, double charSpaceKern, bool onlyOneLine=true );
 // Get a bounding box for the text in pixels
 void BoxBounds( Cartesiand *out, const char *s, double fontW, double fontH) { BoxBounds(out,s,fontW,fontH,this->lineSpace,this->charSpace); }
 // Get a bounding box for the text in pixels
 void BoxBounds( Cartesiand *out, const char *s, double fontW, double fontH, double lineSpace, double charSpaceKern );
MANY(ATexFont,ATexFontHandle,ATexFontHandles,"ATexFont",ATexFontLibrary,{})
 KEYSWORDSGroups("ATexFont",ATexFont)
 ATexFont *Load( const char *fontfile ) {
  ATexFont *a=new ATexFont;
  a->Load(fontfile);
  Append(a);
  return a;
 }
 ATexFont *Load( const char *name, const char *filename ) {
  OUTPUT("ATFont::Load(`%s`) named `%s`\n",filename, name);
  ATexFont *a=Load(filename);
  a->name = name;
  return a;
 }
 void LoadFonts();
 ATexFont *find(const char *name) {
  FOREACH(ATexFont,f) if ( !str_cmp(f->name.c_str(),name) ) return f;
  return null;
 }
 void ClearCaches() { FOREACH(ATexFont,f) f->cache.ClearCache(); }
DONE(ATexFont);

extern ATexFontLibrary atexfonts;

extern Zp<ATexFont> defaultATexFont;

extern Zp<ATexFont> abytes,acalibri,adigicaps,ametalFont,amicroFont,atermFont,aglowTermFont;

void LoadATexFonts();

ATexFont *atexFontByName( const char *s );