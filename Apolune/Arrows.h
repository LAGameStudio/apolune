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

#include <Windows.h>
#include <math.h>
#include "moremath.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"
#include "GLImage.h"
#include "GLWindow.h"
#include "Art.h"
#include "Cartesian.h"
#include "Crayon.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Presentation.h"
/*
 *          Be      bordered arrow
 *          |     
 *   Ba_____|Bd  e  hh
 *    |a         |\  ^
 *    | +--------| \ |              . sx,sy       
 *    | |.sx,sy^ d  \|   \          |_______. ex,ey
 *    | |      h c  /g  / Bg 
 *    | +--------| /(ex,ey)
 *    |b         |/
 *   Bb       Bc  f
 *            Bf <-->hw
 */

class Arrow {
 mPolygon arrow;
 mPolygon Border;
 Vertex rotation;
 Cartesian start,end;
 float angle;
 float border;
 float floating;
 Crayon filling,borderColor;

 Arrow( int x, int y, int Ex, int Ey, int w, int h, int hw, int hh, float b ) {
  floating=false;
  border=b;
  start.x=x;
  start.y=y;
  start.w=w;
  start.h=h;
  end.x=Ex;
  end.y=Ey;
  end.w=w;
  end.h=h;
  angle=rad2deg(atan2((float)(Ey-y),(float)(Ex-x)));
//  Vertex *v;
 }
 
 void Render( GLWindow *surface ) {
  /*glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glPopMatrix();
  glPopAttrib();*/
 }

 void DrawLines( GLWindow *surface ) {
/*
  DrawLine(surface,borderColor,(surface->x+a.x),(surface->y+a.y), (surface->x+b.x),(surface->y+b.y));
  DrawLine(surface,borderColor,(surface->x+b.x),(surface->y+b.y), (surface->x+c.x),(surface->y+c.y));
  DrawLine(surface,borderColor,(surface->x+c.x),(surface->y+c.y), (surface->x+d.x),(surface->y+d.y));
  DrawLine(surface,borderColor,(surface->x+d.x),(surface->y+d.y), (surface->x+e.x),(surface->y+e.y));
  DrawLine(surface,borderColor,(surface->x+e.x),(surface->y+e.y), (surface->x+f.x),(surface->y+f.y));
  DrawLine(surface,borderColor,(surface->x+f.x),(surface->y+f.y), (surface->x+g.x),(surface->y+g.y));
//  surface.DrawLine((int)(surface.offset.x+f.fx),(int)(surface.offset.y+f.fy), (int)(surface.offset.x+g.fx),(int)(surface.offset.y+g.fy));
  DrawLine(surface,borderColor,(surface->x+g.x),(surface->y+g.y), (surface->x+a.x),(surface->y+a.y));
  */
 }

 ~Arrow() {
 }
};

class Bouncing : Presentation {
};