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

/**************************************************************
 Author: H. Elwood Gilliland III
 Test case for implementing FreeType6
 **************************************************************/

#include "GLWindow.h"
#include "FreeType.h"

class FreetypeTest : public GLWindow {
public:
 Zdouble keyDelay;
 Zpointer<FreetypeFont> font;
 Zpointer<FreetypeGlyph> glyph;
 Zstring s;
 void OnLoad() {
  s=string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890=@&+$#()\"'%@~!,.?");
  Fullscreen();
  background.Pick(indigo);
  font=(FreetypeFont *) freetype.fonts.first;
  glyph=(FreetypeGlyph *) font->glyphs.first;
 }
 void Between() {
  if ( input->KeyDown(Left) ) {
   if ( glyph->next ) glyph=(FreetypeGlyph *) glyph->next;
   else glyph=(FreetypeGlyph *) font->glyphs.first;
   keyDelay=0.25;
  }
 }
 void Render() {
  //glyph->contours.result.DebugDraw2DInvertY(input->mx+512,input->my,2.0f,2.0f);
  glLineWidth(2);
  /*
  EACH(glyph->contours.result1.first,Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *)v->next;
   DrawLine2Color(crayons.Pick(magenta),crayons.Pick(oliveDrab),
    v->x*2.0f+input->mxi,
    v->y*2.0f+input->myi,
    w->x*2.0f+input->mxi,
    w->y*2.0f+input->myi
   );
  }
  */
  glLineWidth(0);
  /*
  EACH(glyph->contours.result2.first,mPolygon,p) {
   p->points.DebugDraw2DInvertY(input->mx,input->my,2.0f,2.0f);
  }
  EACH(glyph->contours.first,FreetypeContour,c) {
   c->points.DebugDraw2DInvertY(input->mx+512.0f,input->my,1.0f,1.0f);
  }
  */
  EACH(glyph->result3.first,mPolygon,p) {
   p->points.DebugDraw2DInvertY(input->mx,input->my,2.0f,2.0f);
  }

  crayons.Pick(skyBlue).gl();
  Text( FORMAT("%s:%s:%s results in tesselation for glyph %ld '%c', contours: %d (outer) %d (inner) %d %d %d trigons", 
    glyph->result1.count.toString().c_str(),
    glyph->result2.count.toString().c_str(),
    glyph->result3.count.toString().c_str(),
    (long)glyph->character, '?',
    (int)glyph->outer.count,
    (int)glyph->inner.count,
    (int)glyph->result1.count,
    (int)glyph->result2.count,
    (int)glyph->result3.count
   ).c_str(), 16, 16, 8, 10, false
  );
  glLineWidth(3);
  EACH(glyph->outer.first,FreetypeContour,c) {
   c->points.DrawInvertY(crayons.Pick(alabaster),input->mx+512.0f,input->my+128.0f,0.2f,0.2f);
  }
  EACH(glyph->inner.first,FreetypeContour,c) {
   c->points.DrawInvertY(crayons.Pick(hotPink),input->mx+512.0f,input->my+128.0f,0.2f,0.2f);
  }
  glLineWidth(1);
 }
};

