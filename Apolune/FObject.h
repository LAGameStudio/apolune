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

#include "ListItem.h"
#include "MapPosition.h"
#include "ScreenPosition.h"
#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"

// the FObject class is a 2+3d object "fbo object" that includes
// several carefully planned rendering layers.

// This is a sprite-type object, that offers some of the functionality
// in MapObject but without any specialized rendering routines (it is a "Framework" for
// game objects that are to be rendered this way)

// The goal of this class is to bridge several helper classes, organize the
// render pipeline, and work in various features

class FObject : public ListItem {
public:
 ZoneData zone;
 FBOColorDepthStencil fbo3d;
 FBOColor planeOverlay;
 FBOColor composite;
 MapPosition map;
 ScreenPosition screen;
 FireAndForgetLimitedWrappingProce55Viewer2d post,postAdditive; // drawn on an FBO and made relative
 virtual void WhenMoved( double dx, double dy ) {}
 virtual void WhenMoved( double dx, double dy, double dz ) {}
 void MoveBy( int x, int y ) { MoveBy( (double)x, (double)y ); }
 void MoveBy( float x, float y ) { MoveBy( (double)x, (double)y ); }
 void MoveBy( Vertexd *v ) {
  map.position.Add(v);
  WhenMoved(v->x,v->y,v->z);
 }
 void MoveBy( double x, double y ) {
  map.position.x+=x;
  map.position.y+=y;
  screen.MoveBy(x,y);
  WhenMoved(x,y);
 }
 void Move2d() { MoveBy(map.velocity.x,map.velocity.y); }
 void Move3d() { MoveBy(&map.velocity); }
 virtual void Between() {}
 virtual void Render() {}
 virtual void RenderAdditive() {}
 virtual void RenderPost() {}
 virtual void Post() {}
 virtual void Damage( double amount ) {}
 virtual void ConsumePower( double amount ) {}
 virtual void PowerDepleted() {}
 virtual void Destroyed() {}
 virtual bool Dead() { return false; }
 virtual void Died() {}
};