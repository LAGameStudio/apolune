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
#include "ForeverWrappingQuadLayers.h"

class FWTFTest : public GLWindow {
public:
 ForeverWrappingTiledFilms films;
 void OnLoad() {
  Fullscreen();
  background.Pick(hotPink);
  films.screen.Set(0,0,display->w,display->h);
  ForeverWrappingTiledFilm *tf=films.Add(vault.find("Rain","Snow"),transparency,crayons.Pick(black));
  films.MoveBy(0.0,0.0);
  tf->tiles.Set(10.0,10.0);
 }
 void Between() {
  films.Between();
  DebugArrows();
 }
 void Render() {
  films.Render();
 }
 void DebugArrows() {
  Cartesiand move;
  if ( input->KeyDown(DX_LEFT) )  {
   move.x=-20.0;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   move.x=20.0;
  }
  if ( input->KeyDown(DX_UP) ) {
   move.y=-20.0;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   move.y=20.0;
  }
  if ( move.x != 0 || move.y != 0 ) {
   films.MoveBy(move.x,move.y);
  }
 }
};

