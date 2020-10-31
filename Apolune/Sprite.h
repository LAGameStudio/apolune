/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "GLImage.h"
#include "Film.h"
#include "ColorController.h"
#include "Art2d.h"

// Smallest 2d quark-like graphics fragment, knows only how to render itself.

class Sprite : public ListItem {
public:
 Zstring name;
 Zp<GLImage> image;
 Zp<Film> film;
 Crayon tint;
 Blends blend;
 Cartesian extents,position;  // x,y of extents is center-point
 Zfloat rotation;
 ColorController colorController;
 Sprite() : ListItem() {
 }
 void Tint( Crayon c ) {
  colorController.tint=c;
  colorController.color=&tint;
 }
 void Between() {
  colorController.Between();
 }
 void Render() {
  Blending(blend);
  rotation=norm_deg(rotation);
  if ( image ) {
   art.Pivot(image,tint,blend,position.fx,position.fy,extents.fx,extents.fy,extents.h/2.0f,extents.w/2.0f,rotation);
  }
  if ( film ) {
   GLImage *f=film->frame();
   if ( f ) art.Pivot(f,tint,blend,position.fx,position.fy,extents.fx,extents.fy,extents.h/2.0f,extents.w/2.0f,rotation);
  }
 }
 Sprite *Clone() {
  Sprite *copy=new Sprite;
  copy->name=name;
  copy->film=film;
  copy->tint.fromCrayon(tint);
  copy->blend=blend;
  copy->extents.Set(&extents);
  copy->position.Set(&position);
  copy->rotation=rotation;
  return copy;
 }
};

