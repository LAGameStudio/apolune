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

#include "GLWindow.h"
#include "TTFLibrary.h"

class TTFLibraryBrowser : public GLWindow {
public:
 Zpointer<TTFTypeset> selected;
 Zpointer<TTFGlyph> glyph;
 Zdouble keyDelay;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  selected=(TTFTypeset *)ttf.typesets.first;
  if ( selected ) glyph=(TTFGlyph *)selected->glyphs.first;
 }
 void Between() {
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else {
   if ( input->KeyDown(Left) && glyph ) { // Select previous glyph in list
    if ( glyph->prev ) glyph=(TTFGlyph *) glyph->prev;
    else glyph=(TTFGlyph *)selected->glyphs.last;
    keyDelay=0.25;
   } else if ( input->KeyDown(Right) && glyph ) { // Select next glyph in list
    if ( glyph->next ) glyph=(TTFGlyph *) glyph->next;
    else glyph=(TTFGlyph *)selected->glyphs.last;
    keyDelay=0.25;
   } else if ( input->KeyDown(Up) && selected ) { // Select previous font in library
    if ( selected ) {
     if ( selected->prev ) selected=(TTFTypeset *) selected->prev;
     else selected=(TTFTypeset *)ttf.typesets.last;
     glyph=(TTFGlyph *)selected->glyphs.first;
    }
    keyDelay=0.25;
   } else if ( input->KeyDown(Down) && selected ) { // Select next font in library
    if ( selected ) {
     if ( selected->next ) selected=(TTFTypeset *) selected->next;
     else selected=(TTFTypeset *)ttf.typesets.first;
     glyph=(TTFGlyph *)selected->glyphs.first;
    }
    keyDelay=0.25;
   } else if ( input->KeyDown(PgDn) ) { // Advance font list by one page
   } else if ( input->KeyDown(PgUp) ) {
   }
  }
 }
 void Render() {
  int x=16, y=16;
  // Draw currently selected font and active font list on left
  EACH(ttf.typesets.first,TTFTypeset,t) {
   crayons.Pick(alabaster).gl();
   if ( t==selected ) {
    Area(crayons.Pick(blue),x,y,x+512,y+20);
    crayons.Pick(macNcheese).gl();
   }
   Text(t->filename.c_str(),(double)x+8.0,(double)y+16.0,8.0,12.0,false);
   y+=20;
  }
  // Draw currently selected glyph in 3d and 2d and data
  if ( glyph ) {
   x=512;
   y=16;
   Text(FORMAT("Character: %c (%d of %d)",glyph->ch,(int)glyph->ch,(int)selected->glyphs.count).c_str(),
    (double)x,(double)y,8.0,12.0,false );
   glyph->model2d.polygons.RenderXYOffsetScaledYInverted(512,32+512,0.25f,0.25f);
   glyph->model2d.polygons.RenderXYOffsetScaledYInverted(512+512,32+512,0.1f,0.1f);
  }
  // Draw charset selector
 }
};