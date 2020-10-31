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

#include "TTFEx.h"
#include "GLWindow.h"
#include "LookAtPerspective.h"
#include "VBOGroup.h"

class TTFExLibraryBrowser : public GLWindow {
public:
 Zstring info;
 LookAtPerspective lap;
 Vertexd rotation;
 VBONormalsPolysGroup vbo;
 Zp<TTFExFont> font;
 Zp<TTFExGlyph> glyph;
 Zdouble keyDelay;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  font = (TTFExFont *) ttfex.fonts.first;
  if ( font ) {
     glyph = (TTFExGlyph *) font->glyphs.first;
     UpdateInfo();
     ReplaceModel();
  }
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=10000.0;
 }
 void ReplaceModel(bool showGlyphFront=false) {
  if ( glyph ) {
   vbo.Clear();
   if ( showGlyphFront  ) glyph->glyph.toVBOGroup(&vbo,true);
   else glyph->facets.toVBOGroup(&vbo,true);
  }
 }
 void UpdateInfo() {
  if ( font ) {
   info="";
   EACH(font->glyphs.first,TTFExGlyph,g) {
    info+=g->ch;
//    info+='(';
//    info+=g->codepoint.toString();
//    info+=')';
   }
  }
 }
 void Between() {
  if ( input->KeyDown(LAlt) ) {
   rotation.z=360.0f*(input->mxi-display->w2)/display->wf;
  } else {
   rotation.x=360.0f*(input->myi-display->h2)/display->hf;
   rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  }
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else {
   if ( input->KeyDown(NumpadPlus) ) { // Zoom in
    lap.eye.z/=2.0;
    keyDelay=0.25;
   } else if ( input->KeyDown(NumpadMinus) ) { // Zoom out
    lap.eye.z*=2.0;
    keyDelay=0.25;
   } else if ( input->KeyDown(Left) && glyph ) { // Select previous glyph in list
    if ( glyph->prev ) glyph=(TTFExGlyph *) glyph->prev;
    else glyph=(TTFExGlyph *)font->glyphs.last;
    ReplaceModel(input->KeyDown(LShift)>0);
    keyDelay=0.25;
   } else if ( input->KeyDown(Right) && glyph ) { // Select next glyph in list
    if ( glyph->next ) glyph=(TTFExGlyph *) glyph->next;
    else glyph=(TTFExGlyph *)font->glyphs.last;
    ReplaceModel(input->KeyDown(LShift)>0);
    keyDelay=0.25;
   } else if ( input->KeyDown(Up) && font ) { // Select previous font in library
    if ( font ) {
     if ( font->prev ) font=(TTFExFont *) font->prev;
     else font=(TTFExFont *)ttfex.fonts.last;
     glyph=(TTFExGlyph *)font->glyphs.first;
     UpdateInfo();
     ReplaceModel(input->KeyDown(LShift)>0);
    }
    keyDelay=0.25;
   } else if ( input->KeyDown(Down) && font ) { // Select next font in library
    if ( font ) {
     if ( font->next ) font=(TTFExFont *) font->next;
     else font=(TTFExFont *)ttfex.fonts.first;
     glyph=(TTFExGlyph *)font->glyphs.first;
     UpdateInfo();
     ReplaceModel(input->KeyDown(LShift)>0);
    }
    keyDelay=0.25;
   } 
  }
 }
 void Render() {
  if ( !glyph || !font ) {
   Text("No fonts loaded in TTFEx library",16,16,8,12,false);
   return;
  }
  // Draw currently selected font and active font list on left
  int fx=16, fy=64;
  EACH(ttfex.fonts.first,TTFExFont,f) {
   crayons.Pick(alabaster).gl();
   if ( f==font ) {
    Area(crayons.Pick(blue),fx,fy,fx+512,fy+20);
    crayons.Pick(macNcheese).gl();
   }
   Text(f->filename.c_str(),(double)fx+8.0,(double)fy+16.0,8.0,12.0,false);
   fy+=20;
  }
  crayons.Pick(skyBlue).gl();
  Text(
   FORMAT("Character %c (%d) - Codepoint %d - Extrusion: %d, Glyph Front: %d Triangles - Font: %s",
    glyph->ch, (int) glyph->ch,
    (int) glyph->codepoint.value,
    (int) glyph->facets.count,
    (int) glyph->glyph.count,
    font->filename.c_str()
    ).c_str(),
    16,16,8,12,false
  );
  glLineWidth(2);
  Text( info.c_str(), 16.0, 48.0, 10.0, 14.0, false );
  fx=16;
  fy=display->h-256;
  crayons.Pick(macNcheese).gl();
  glLineWidth(1);
  glPush();
   lap.Apply();
   glRotated(rotation.x,1,0,0);
   glRotated(rotation.y,0,1,0);
   glRotated(rotation.z,0,0,1);
   glTranslated(
    -glyph->facets.extents.x/2.0,
    -glyph->facets.extents.y/2.0,
    -glyph->facets.extents.z/2.0
   );
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.1,1000.0); //  glDepthRange(0.001,1.0);
   glDepthFunc(GL_LEQUAL);
   glColor3d(0.65,0.1,0.65);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
   vbo.Render();//Polys();
  glPop();
  EACH(font->glyphs.first,TTFExGlyph,g) {
   Zstring s;
   s+=(g->ch);
   if ( g == glyph ) {
    Area(crayons.Pick(peru),fx-2,fy-16,fx+2+10,fy+2);
   } else {
    Rectangle(crayons.Pick(blue255),fx-2,fy-16,fx+2+10,fy+2);
   }
   crayons.Pick(alabaster).gl();
   Text(s.c_str(),(double)fx,(double)fy,10,14,false);
   fx+=24;
   if ( fx > display->w - 24 ) {
    fx=16;
    fy+=24;
   }
  }
 }
};

