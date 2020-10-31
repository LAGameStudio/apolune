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
#pragma once


#include "TTF.h"

#include "Polygon.h"
#include "Model.h"
#include "VBO.h"
#include "Strings.h"
#include "VBOGroup.h"

using namespace TTF;

class TTFGlyph : public ListItem {
public:
 Zpointer<TTFCore::Font> font;
 char ch;
 size_t precision;
 int16_t depth;
 TTFCore::FontMetrics metrics;
 GlyphMetrics glyph_metrics;
 Zfloat width;
 TTFCore::MeshLarge triangles2d, triangles3d;
 ImmediateModel model2d,model3d;
 VBOStaticVNT vbo2d,vbo3d;
 Zbool error;
 TTFGlyph( Font *font, char c ) : ListItem() {
  precision=8;
  depth=1;
  ch=c;
  this->font=font;
  try {
   metrics=font->GetFontMetrics();
  } catch ( FontException e ) {
   OUTPUT("TTFGlyph: Error: %s\n", e.msg.c_str() );
   error=true;
   return;
  }
  glyph_metrics=font->GetGlyphMetrics((TTFCore::CodePoint)c);
  width=metrics.advance_width_max;

  TriangulatorIIFixed t2(precision);

  font->GetGlyph(c,t2,triangles2d);
  // Copies triangulation of front face to model2d.
  size_t vertices=triangles2d.VertexCount();
  for ( unsigned int i=0; i<vertices; i+=3 ) {
   mPolygon *p=new mPolygon;
   p->draw_method=GL_TRIANGLES;
   Vertex *a=new Vertex((float)triangles2d.verts[i].x,(float)triangles2d.verts[i].y,0.0f);
   a->Color(crayons.Pick(red255));
   p->points.Append(a);
   Vertex *b=new Vertex((float)triangles2d.verts[i+1].x,(float)triangles2d.verts[i+1].y,0.0f);
   b->Color(crayons.Pick(blue255));
   p->points.Append(b);
   Vertex *c=new Vertex((float)triangles2d.verts[i+2].x,(float)triangles2d.verts[i+2].y,0.0f);
   c->Color(crayons.Pick(green255));
   p->points.Append(c);
   model2d.polygons.Append(p);
  }
  model2d.BoxBounds();

  TriangulatorIIIFixed t3(depth,precision);

  font->GetGlyph(c,t3,triangles3d);
  vertices=triangles3d.VertexCount();
  for ( unsigned int i=0; i<vertices; i+=3 ) {
   mPolygon *p=new mPolygon;
   p->draw_method=GL_TRIANGLES;
   Vertex *a=new Vertex((float)triangles3d.verts[i].x,(float)triangles2d.verts[i].y,0.0f);
   a->Color(crayons.Pick(red255));
   p->points.Append(a);
   Vertex *b=new Vertex((float)triangles3d.verts[i+1].x,(float)triangles2d.verts[i+1].y,0.0f);
   b->Color(crayons.Pick(blue255));
   p->points.Append(b);
   Vertex *c=new Vertex((float)triangles3d.verts[i+2].x,(float)triangles2d.verts[i+2].y,0.0f);
   c->Color(crayons.Pick(green255));
   p->points.Append(c);
   model2d.polygons.Append(p);
  }
  model2d.BoxBounds();

  vbo2d.fromModel(&model2d);
  vbo2d.Create();
  vbo3d.fromModel(&model3d);
  vbo3d.Create();
 }
};

class TTFGlyphs : public LinkedList {
public:
 TTFGlyph *find( char c ) {
  FOREACH(TTFGlyph,t) if ( t->ch == c ) return t;
  return null;
 }
 void Add( Font *font, char c ) {
  Append(new TTFGlyph(font,c));
 }
 CLEARLISTRESET(TTFGlyph);
};

/*
 * A font
 */
class TTFTypeset : public ListItem {
public:
 Zbool error;
 Zpointer<TTFCore::Font> font;
 TTFGlyphs glyphs;
 Zstring typeset;
 Zstring filename;
 TTFTypeset( const char *filename ) : ListItem() {
  this->filename=filename;
  try {
   font = new Font(filename);
  } catch ( FontException e ) {
   error=true;
   OUTPUT("TTFTypeset(`%s`) error: %s\n", filename, e.msg.c_str() );
  }
  if ( !error ) {
   typeset = string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890=@&+$#()\"'%@*<>~!,.?/\\");
   const char *p=typeset.c_str();
   while ( *p != '\0' ) {
    glyphs.Add(font,*p);
    p++;
   }
  }
 }
};

/*
 * A list of fonts.
 */
class TTFTypesets : public LinkedList {
public:
 void Load( const char *filename ) {
  // See if the font is already loaded
  FOREACH(TTFTypeset,t) {
   if ( t->filename == filename ) {
    OUTPUT("TTFLibrary:TTFTypesets:Load(`%s`) font was already loaded\n",filename);
    return;
   }
  }
  OUTPUT("TTFLibrary:TTFTypesets:Load(`%s`)\n",filename);
  // Load and precompute if the font is not
  Append(new TTFTypeset(filename));
 }
 CLEARLISTRESET(TTFTypeset);
};

/*
 * A placement of multiple letters and their relevant extrusion (if necessary)
 * It is essentially a way to create baked sets and recall an organized typesetting
 * of some predetermined content.
 */
enum TTFRuleupType {
 TTF_ruleup_2d=0,
 TTF_ruleup_3d=1
};

class TTFRuleupPlacement : public ListItem {
public:
 Vertexd position,rotation,scale;
 Zpointer<TTFGlyph> glyph;
 TTFRuleupPlacement() : ListItem() {}
 TTFRuleupPlacement( TTFGlyph *g, float x, float y, float sx, float sy ) {
  glyph=g;
  position.Set(x,y,0.0f);
  scale.Set(sx,sy,1.0f);
 }
};

class TTFRuleupPlacements : public LinkedList {
public:
 CLEARLISTRESET(TTFRuleupPlacement);
};

class TTFRuleup : public ListItem {
public:
 Zstring name;
 Vertexd position,rotation,scale;
 TTFRuleupType type;
 TTFRuleupPlacements placements;
 TTFRuleup() : ListItem() { type=TTF_ruleup_2d; }
 TTFRuleupPlacement *Add( TTFGlyph *g, float x, float y, float sx, float sy ) {
  TTFRuleupPlacement *t=new TTFRuleupPlacement( g, x, y, sx, sy );
  return t;
 }
 // Generates a VBO or composite model out of a series of placements.
 void Proof( bool twoAndThreeDimensional ) {
  if ( twoAndThreeDimensional ) {
  } else {
  }
 }
};

class TTFRuleups : public LinkedList {
public:
 CLEARLISTRESET(TTFRuleup);
 TTFRuleup *find( const char *name ) {
  FOREACH(TTFRuleup,t) if ( t->name == name ) return t;
  return null;
 }
};

/*
 * Handles the loading, management, extrusion and rendering of fonts using the
 * TTF triangulation library from SourceForge.
 */
class TTFLibrary {
public:
 TTFTypesets typesets;
 TTFRuleups ruleups;
 void Load(const char *filename) {
  Zstring file;
  file.Load(filename);
  char *werd=file.next();
  while ( *werd != '\0' ) {
   if ( file_exists(werd) ) {
    typesets.Load(werd);
   } else {
    OUTPUT("TTFLibrary:Load(`%s`) couldn't find the file `%s`\n",filename,file.c_str());
   }
   werd=file.next();
  }
 }
 TTFTypeset *find( const char *filename ) {
  EACH(typesets.first,TTFTypeset,t) if ( t->filename == filename ) return t;
  return null;
 }
 // Simple layout for 2d text, creates a ruleup including text transform data
 TTFRuleup *Layout( TTFTypeset *font, const char *phrase,
   float ox, float oy, float sx, float sy, 
   float lineHeight, float spaceWidth, float charSpace, bool setProof=true, bool twoAndThree=true, bool addToList=false ) {
  TTFRuleup *t=new TTFRuleup;
  if ( addToList ) ruleups.Append(t);
  float x=ox, y=oy;
  const char *p=phrase;
  while ( *p != '\0' ) {
   if ( *p == '\n' ) {
    x=ox;
    y+=lineHeight;
   } else if ( *p == ' ' ) {
    x+=spaceWidth;
   } else {
    TTFGlyph *g=font->glyphs.find(*p);
    if ( g ) {
     t->Add(g,x,y,sx,sy);
     x+=(float) g->width * sx;
    }
    x+=charSpace;
   }
   p++;
  }
  if ( setProof ) t->Proof(twoAndThree);
  return t;
 }
 // Accepts a KV pairing as the markup
 // You can set: fragment color, font/glyph type, size, position, scale, etc.
 // A pseudo-html
// TTFRuleup *Layout( Strings *markup, bool addToList=false ) {
//  TTFRuleup *t=new TTFRuleup;
  
//  if ( addToList ) ruleups.Append(t);
// }
 void Remove( TTFRuleup *t ) { ruleups.Remove(t); }
};

extern TTFLibrary ttf;