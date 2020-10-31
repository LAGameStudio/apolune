/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
/* IN collaboration with: */
/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    * (c) 2014 PieceMaker Technologies
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        *----------------------------------
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * TTF 3D Glyph Libraries for rendering
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * 3D text that has been precomputed.
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * 
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/
#pragma once

#include "LinkedList.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "BinaryFile.h"
#include "STLFile.h"

/* A true-type extruded 3d font processed through THREE.js and loaded either from ascii or binary */

class TTFExGlyph : public ListItem {
public:
 Zuint codepoint;
 char ch;
 STLFacets facets,glyph;
 ImmediateModel cache;
 TTFExGlyph() : ListItem() {
  ch='?';
 }
 void FromExtrusion() {
  EACH(facets.first,STLFacet,f)
   if ( f->a.z == 0 && f->a.z == f->b.z && f->b.z == f->c.z )
    glyph.Append(new STLFacet(f));
 }
 void ExtrudetoVBO( VBOStaticVCLines *normals, VBOStaticVC *polys ) {
  facets.toVBO( normals, polys );
 }
 void ExtrudetoVBOGroup( VBONormalsPolysGroup *vbos ) {
  facets.toVBOGroup(vbos);
 }
 void ExtrudetoModel( ImmediateModel *model ) {
  facets.toModel(model);
 }
 void ExtrudetoVBOStaticVNT( VBOStaticVNT *vbo, bool clearcpu=true ) {
  facets.toVBOStaticVNT(vbo,clearcpu);
 }
 void ExtrudetoVBOStaticVC( VBOStaticVC *vbo, bool clearcpu=true ) {
  facets.toVBOStaticVC(vbo,clearcpu);
 }
 void ExtrudetoVBOStaticVCLines( VBOStaticVCLines *vbo, bool clearcpu=true ) {
  facets.toVBOStaticVCLines(vbo,clearcpu);
 }
 void toVBO( VBOStaticVCLines *normals, VBOStaticVC *polys ) {
  facets.toVBO( normals, polys );
 }
 void toVBOGroup( VBONormalsPolysGroup *vbos ) {
  glyph.toVBOGroup(vbos);
 }
 void toModel( ImmediateModel *model ) {
  glyph.toModelTM(model);
 }
 void toVBOStaticVNT( VBOStaticVNT *vbo, bool clearcpu=true ) {
  glyph.toVBOStaticVNT(vbo,clearcpu);
 }
 void toVBOStaticVC( VBOStaticVC *vbo, bool clearcpu=true ) {
  glyph.toVBOStaticVC(vbo,clearcpu);
 }
 void toVBOStaticVCLines( VBOStaticVCLines *vbo, bool clearcpu=true ) {
  glyph.toVBOStaticVCLines(vbo,clearcpu);
 }
 void ExportGlyphAsUnit(const char *named) {
  ImmediateModel a;
  facets.toModel(&a,false);
  a.Unit1Centered();
//  a.calculateNormals();
  STLFile out(&a);
  out.filename=named;
  out.ascii=true;
  out.Encode();
 }
};

class TTFExGlyphs : public LinkedList {
public:
 Vertexd widest,baseline_mostCommonYLeast;
 Zdouble baselineBottomOffset;
 TTFExGlyph *find( char ch ) {
  FOREACH(TTFExGlyph,t) if ( t->ch == ch ) return t;
  return null;
 }
 TTFExGlyph *find( unsigned int codepoint ) {
  FOREACH(TTFExGlyph,t) if ( t->codepoint.value == codepoint ) return t;
  return null;
 }
 void Post() {
  widest.Set(0,0,0);
  baselineBottomOffset=0.0;
  FOREACH(TTFExGlyph,glyph) {
   if ( widest.x < glyph->facets.extents.x ) widest.x=glyph->facets.extents.x;
   if ( widest.y < glyph->facets.extents.y ) widest.y=glyph->facets.extents.y;
   if ( widest.z < glyph->facets.extents.z ) widest.z=glyph->facets.extents.z;
  }
 }
 // Saves a binary format
 void Save( const char *filename ) {
  BinaryFile file;
  file.Open(filename,true);
  file.scrawl(count.value);
  FOREACH(TTFExGlyph,t) {
   file.scrawl(t->ch);
   file.scrawl(t->codepoint.value);
   t->facets.BinaryWrite(&file);
  }
 }
 void ExportGlyphsAsUnitSTL( const char *prefix ) {
  Zstring named;
  FOREACH(TTFExGlyph,glyph) {
   named=FORMAT("data/out/%s-char_%d.stl",prefix,(int)glyph->ch);
   OUTPUT("Exporting Glyph %s\n",named.c_str());
   glyph->ExportGlyphAsUnit(named.c_str());
  }
 }
 // Loads a binary format
 void Binary( const char *filename ) {
  if ( file_exists(filename) ) {
   BinaryFile file;
   file.Open(filename,false);
   int total=0;
   file.read(&total);
   while ( total > 0 && file.fileok() ) {
    TTFExGlyph *glyph=new TTFExGlyph;
    file.Fread(1,&glyph->ch,sizeof(char));
    file.Fread(1,&glyph->codepoint.value,sizeof(unsigned int));
    glyph->facets.BinaryRead(&file);
    glyph->FromExtrusion();
    glyph->facets.CalculateExtents();
    Append(glyph);
    total--;
   }
  }
 }
 // Loads an ascii format
 void Ascii( const char *filename )  {
  if ( file_exists(filename) ) {
   Vertex A,B,C,N;
   Zpointer<TTFExGlyph> glyph;
   Zstring file=file_as_string(filename);
   const char *werd=file.Next();
   while ( *werd != '\0' ) {
    if ( !str_cmp(werd,"g") ) {
     werd=file.Next();
     if ( glyph ) { // Close up previous iteration
      glyph->FromExtrusion();
      glyph->facets.CalculateExtents();
     }
     glyph=new TTFExGlyph;
     glyph->codepoint = (unsigned int) atoi(werd);
     if ( glyph->codepoint.value < 256 ) glyph->ch=(char) glyph->codepoint.value; // UTF-8 conversion
     Append(glyph.pointer);
    } else if ( !glyph ) OUTPUT("TTFExGlyphs:Load(`%s`) Out of scope: %s\n", filename, werd );
    else if ( !str_cmp(werd,"f") ) {
     werd=file.Next(); A.fromSimpleString(werd);
     werd=file.Next(); B.fromSimpleString(werd);
     werd=file.Next(); C.fromSimpleString(werd);
     werd=file.Next(); N.fromSimpleString(werd);
     glyph->facets.PushTriangle(&A,&B,&C,&N);
    }
    werd=file.Next();
   }
   if ( glyph ) { // Close up last iteration
    glyph->FromExtrusion();
    glyph->facets.CalculateExtents();
   }
  }
 }
 CLEARLISTRESET(TTFExGlyph);
};

class TTFExFont : public ListItem {
public:
 Zstring filename,name;
 TTFExGlyphs glyphs;
 TTFExFont() : ListItem() {}
 TTFExFont( const char *filename, bool binary=true ) {
  OUTPUT("TTFExFont:Load(`%s`) %s ... ", filename, binary ? "binary" : "ascii" );
  this->filename=filename;
  size_t slash=this->filename.value.find_last_of('/');
  size_t dot=this->filename.value.find_first_of('.');
  this->name=this->filename.value.substr(slash+1,dot-slash-1);
  if ( binary ) glyphs.Binary(filename);
  else glyphs.Ascii(filename);
  glyphs.Post();
  OUTPUT("%d glyphs, widest is %s\n", (int) glyphs.count, glyphs.widest.toString().c_str() );
 }
 void Save( const char *filename ) {
  glyphs.Save(filename);
 }
 int Text( const char *s, int x, int y, int fw, int fh, int cs=1, int ls=3 ) {
  const char *p=s;
  int fx=x;
  int fy=y;
  while ( *p != '\0' ) {
   if ( *p == '\t' ) {
    fx+=(fw*5)+cs;
   } else if ( *p == ' ' ) {
    fx+=fw+cs;
   } else if ( *p == '\n' ) {
    fx=x;
    fy+=fh+ls;
   } else if ( *p == '\r' ) {
   } else {
    TTFExGlyph *C= glyphs.find(*p);
    if ( C ) {
     if ( C->cache.polygons.count == 0 ) {
      C->glyph.toModel(&C->cache,false);
      C->cache.ColorTransitiveMapVertices(crayons.Pick(gray),crayons.Pick(alabaster),crayons.Pick(black),crayons.Pick(gray));
     }
     float diffW=(float) fw * C->cache.greatests.x/100.0f;
     C->cache.Render2dC((float)fx,(float)fy,(float)fw/100.0f,(float)(-fh)/100.0f);
     fx+=(int)(diffW+0.5f)+cs;
    }
   }
   p++;
  }
  return fx;
 }
 int Text( const char *s, Crayon multiply, int x, int y, int fw, int fh, int cs=1, int ls=3 ) {
  const char *p=s;
  int fx=x;
  int fy=y;
  while ( *p != '\0' ) {
   if ( *p == '\t' ) {
    fx+=(fw*5)+cs;
   } else if ( *p == ' ' ) {
    fx+=fw+cs;
   } else if ( *p == '\n' ) {
    fx=x;
    fy+=fh+ls;
   } else if ( *p == '\r' ) {
   } else {
    TTFExGlyph *C=glyphs.find(*p);
    if ( C ) {
     if ( C->cache.polygons.count == 0 ) {
      C->glyph.toModel(&C->cache,false);
      C->cache.ColorTransitiveMapVertices(crayons.Pick(gray),crayons.Pick(alabaster),crayons.Pick(black),crayons.Pick(gray));
     }
     float diffW=(float) fw * C->cache.greatests.x/100.0f;
     C->cache.Render2dCM( multiply, (float)fx,(float)fy,(float)fw/100.0f,(float)(-fh)/100.0f);
     fx+=(int)(diffW+0.5f)+cs;
    }
   }
   p++;
  }
  return fx;
 }
 double GlyphWRatio( TTFExGlyph *g ) {
  if ( g->cache.polygons.count == 0 ) g->glyph.toModel(&g->cache,false);
  return (double) (g->cache.greatests.x)/100.0;
 }
 double GlyphHRatio( TTFExGlyph *g ) {
  if ( g->cache.polygons.count == 0 ) g->glyph.toModel(&g->cache,false);
  return (double) (g->cache.greatests.y)/100.0;
 }
 double GlyphWHAspect( TTFExGlyph *g ) {
  if ( g->cache.polygons.count == 0 ) g->glyph.toModel(&g->cache,false);
  return (double) (g->cache.greatests.x)/(g->cache.greatests.y);
 }
 double GlyphHWAspect( TTFExGlyph *g ) {
  if ( g->cache.polygons.count == 0 ) g->glyph.toModel(&g->cache,false);
  return (double) (g->cache.greatests.y)/(g->cache.greatests.x);
 }
 int Width( const char *s,int fw, int fh, int cs=1, int ls=3 ) {
  const char *p=s;
  int fx=0;
  int fy=0;
  while ( *p != '\0' ) {
   if ( *p == '\t' ) {
    fx+=(fw*5)+cs;
   } else if ( *p == ' ' ) {
    fx+=fw+cs;
   } else if ( *p == '\n' ) {
    fx=0;
    fy+=fh+ls;
   } else if ( *p == '\r' ) {
   } else {
    TTFExGlyph *C=glyphs.find(*p);
    if ( C ) {
     double diffW=(double) fw * GlyphWRatio( C );
     fx+=(int)(diffW+0.5)+cs;
    }
   }
   p++;
  }
  return fx;
 }
};

HANDLES(TTFExFont,TTFExFontHandle,TTFExFontHandles,"TTFExFontHandle");

class TFFExFonts : public LinkedList {
public:
 TTFExFont *find( const char *filename ) {
  FOREACH(TTFExFont,f) if ( !str_cmp(filename,f->filename.c_str()) ) return f;
  return null;
 }
 CLEARLISTRESET(TTFExFont);
};

class TTFExFontLibrary {
public:
 TFFExFonts fonts;
 TTFExFont *find( const char *filename ) {
  return fonts.find(filename);
 }
 // Saves in binary format, appending '.bin' to the filename
 void Save( const char *filename ) {
  Zstring listFile;
  EACH(fonts.first,TTFExFont,f) {
   Zstring fn(f->filename);
   fn+=".bin";
   OUTPUT("TTFExFontLibrary:Save(`%s`) writing out binary file `%s`\n", filename, fn.value.c_str() );
   listFile+=fn;
   listFile+='\n';
   f->Save(fn.c_str());
  }
  listFile.Save(filename);
 }
 // Loads in Ascii format from a list
 void Ascii( const char *filename ) {
  OUTPUT("TTFExLibrary:Ascii(`%s`) loading list\n", filename);
  if ( file_exists(filename) && fonts.find(filename) == null ) {
   Zstring file=file_as_string(filename);
   char *werd=file.next();
   while ( *werd != '\0' ) {
    fonts.Append(AsciiFont(werd));
    werd=file.next();
   }
  }
 }
 // Loads in Binary format from a list
 void Binary( const char *filename ) {
  OUTPUT("TTFExLibrary:Binary(`%s`) loading list\n", filename);
  if ( file_exists(filename) && fonts.find(filename) == null ) {
   Zstring file=file_as_string(filename);
   char *werd=file.next();
   while ( *werd != '\0' ) {
    fonts.Append(BinaryFont(werd));
    werd=file.next();
   }
  }
 }
 TTFExFont *LoadAsciiFont( const char *filename ) {
  TTFExFont *font = new TTFExFont(filename,false);
  fonts.Append(font);
  return font;
 }
 TTFExFont *LoadBinaryFont( const char *filename ) {
  TTFExFont *font = new TTFExFont(filename);
  fonts.Append(font);
  return font;
 }
 TTFExFont *AsciiFont( const char *filename ) {
  TTFExFont *font = new TTFExFont(filename,false);
  return font;
 }
 TTFExFont *BinaryFont( const char *filename ) {
  TTFExFont *font = new TTFExFont(filename);
  return font;
 }
};

extern TTFExFontLibrary ttfex;

#include "VBOGroup.h"

class Generated3DText : public ListItem {
public:
 Zstring content;
 Zp<TTFExFont> font;
 VBONormalsPolysGroup vbos;
 ImmediateModel text;
 Vertexd scale;
 Zbool fit,vertical; 
 Generated3DText() : ListItem() {}
 Generated3DText( TTFExFont *font, string t, int characters, Vertexd *scale, bool fit, bool vertical ) : ListItem() {
  Generate(font,t,characters,scale,&text,fit,vertical);
 }
 void Generate( TTFExFont *font, string t, int characters, Vertexd *scale, ImmediateModel *a, bool fit, bool vertical ) {
  if ( !font ) {
   OUTPUT("Generated3DText:Generate called with no font!\n");
   return;
  }
  float charspace=12.0f;
  this->fit=fit;
  this->vertical=vertical;
  this->scale.Set(scale);
  this->font=font;
  this->content=t;
  content=t;
  this->font=font;
  a->polygons.Clear();
  const char *p=t.c_str();
  char previousChar='\0';
  Zdouble advancement;
  int c=0;
  while ( *p != '\0' ) {
   if ( *p == ' ' ) advancement+=50.0f;
   else {
    TTFExGlyph *g=font->glyphs.find(*p);
    if ( g ) {
     ImmediateModel glyph;
     g->facets.toModel(&glyph,crayons.Pick(alabaster),false);
     if (vertical) glyph.Rotate(0.0f,0.0f,90.0f);
     if ( advancement > 0.0 ) glyph.moveBy((float)advancement,0.0f,0.0f);
     advancement+=(vertical?glyph.greatests.y:glyph.greatests.x)+charspace;
     a->Concat(&glyph,c);
    }
   }
   previousChar=*p;
   c++;
   p++;
  }
  a->BoxBounds();
  // Calculate the scaling difference between original and 1
  if ( fit ) {
   a->Unit1Centered();
   a->Scale(iratiof((int)t.length(),characters),1.0f,1.0f);
   a->BoxBounds();
   a->Centered();
   /*
   a->Zeroed();
   float fitting=UMIN(1.0f/a->boxMagnitude.x, 1.0f/a->boxMagnitude.y);
   EACH(a->polygons.first,mPolygon,p) {
    EACH(p->points.first,Vertex,v) {
     v->x=(v->x*fitting);
     v->y=(v->y*fitting);
     v->z=(v->z/a->boxMagnitude.z);
    }
   }
   a->BoxBounds();
   a->Centered();
   */
  } else a->Unit1Centered(); // Stretch
  vbos.fromTriModel(a,crayons.Pick(black),crayons.jami(0,200,200,200,255));
 }
 void glScale() {
  if ( fit ) {
  } else {
   glScaled(scale.x,scale.y,scale.z);
  }
 }
};