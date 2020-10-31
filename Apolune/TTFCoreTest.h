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

#if defined(NEVER)

// For version 1.0 of TTF Triangulator

#include "TTF.h"
#include "GLWindow.h"
#include "Polygon.h"
#include "Freetype.h"

using namespace Utility::TTF;

class TTFCoreTest : public GLWindow {
public:
 Zpointer<FreetypeFont> font;
 Mesh triangles;
 Polygons polygons;
 string s;
 Zdouble keyDelay;
 Zfloat lineWidth;
 void OnLoad() {
  s=string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890=@&+$#()\"'%@~!,.?");
  font=(FreetypeFont *) freetype.fonts.first;
  Fullscreen();
  background.Pick(indigo);
  Font *f = new Font("data/ttf/Test.ttf");
  triangles=f->GetTriangulation('a');
  delete f;
  for ( unsigned int i=0; i<triangles.verts.size(); i+=3 ) {
   mPolygon *p=new mPolygon;
   Vertex *a=new Vertex(triangles.verts[i].pos.x,triangles.verts[i].pos.y,0.0f); a->Color(crayons.Pick(red255));
   p->points.Append(a);
   Vertex *b=new Vertex(triangles.verts[i+1].pos.x,triangles.verts[i+1].pos.y,0.0f); b->Color(crayons.Pick(blue255));
   p->points.Append(b);
   Vertex *c=new Vertex(triangles.verts[i+2].pos.x,triangles.verts[i+2].pos.y,0.0f); c->Color(crayons.Pick(green255));
   p->points.Append(c);
   polygons.Append(p);
  }
 }
 void Between() {
  if ( keyDelay > 0.0 ) keyDelay -=FRAMETIME;
  if ( keyDelay <= 0.0 ) {
   if ( input->KeyDown(Up) ) {
    keyDelay=0.25;
    lineWidth+=0.5;
   }
   if ( input->KeyDown(Down) ) {
    keyDelay=0.25;
    lineWidth-=0.5;
    if ( lineWidth < 0.5 ) lineWidth=0.5;
   }
  }
 }
 void Render() {
  crayons.Pick(red255).gl();
  polygons.RenderXYOffsetScaledYInverted(input->mx,input->my,0.5f,0.5f);
  font->Render("Supercalifragalisticexpialadocious\nThis is the first line.\nThis is the second line.",
   crayons.Pick(skyBlue),16.0f,128.0f,0.1f,0.1f,42.0f,1.0f);
  glLineWidth(lineWidth.value);
  font->Render("Supercalifragalisticexpialadocious\nThis is the first line.\nThis is the second line.",
   crayons.Pick(skyBlue),16.0f,256,0.05f,0.05f,18.0f,1.0f,16.0f);
  glLineWidth(1);
 }
};

#endif