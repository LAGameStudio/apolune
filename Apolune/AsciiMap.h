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

#include "Strings.h"
#include "TextureRolodex.h"
#include "Proce55or.h"
#include "BoxCollision2d.h"
#include "GLWindow.h"

class SimpleTile2di : public ListItem {
public:
 TextureCard *texture;
 int x, y;
 Blends blend;
 Crayon c;
};

class SimpleTiles2di : public LinkedList {
public:
 void Renderi( GLWindow *surface, int ox, int oy, Lights *lights ) {
  EACH(first,SimpleTile2di,t) {
   if ( t->texture->i ) {
    Blend(surface,t->texture->i,t->blend,ox+t->x,oy+t->y,0,0,t->texture->i->width,t->texture->i->height,t->texture->i->width,t->texture->i->height);
   } else if ( t->texture->p ) {
   }
  }
 }
 void Renderf( GLWindow *surface, float ox, float oy ) {
 }
};

class SimpleTileMap2d : public ListItem {
public:
 SimpleTile2di integers;

};

class AsciiMapLayer {
public:
 int charWidth;
 string map;
 AsciiMapLayer() { charWidth=2; map=string(""); }

 SimpleTileMap2d *toSimpleTileMap2d( TextureRolodex *textures ) {
 }
 BoxCollision2di *toBoxCollision2di( TextureRolodex *textures ) {
 }
 BoxCollision2df *toBoxCollision2df( TextureRolodex *textures ) {
 }
};

class fx_AsciiMap : public Proce55or {
public:
 void LoadMapFromStrings( Strings *strings ) {
 }
 void LoadMapFromString( String input ) {
 }
};