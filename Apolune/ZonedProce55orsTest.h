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

#include "Proce55or.h"
#include "ZSortedFAFZonedProce55Viewer2d.h"
#include "ZonedMovingProce55orDebugger.h"

class ZonedProce55orsTest : public ZSortedFAFZonedProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  this->SetExtents(10,10);
//  this->SetExtents(51,51);
  for ( int i=0; i<1000; i++ ) {
   this->Add(new ZonedMovingProce55orDebugger(&grid));
  }
  Init(false);
 }
 void PreProcessing() {
  Debug(); // cannot really use above 50k objects, maps bigger than 21x21 zones
 }
 void Between() {
  BetweenFrames();
  int movedx=0,movedy=0;
  if ( input->KeyDown(DX_LEFT) ) movedx=-10;
  if ( input->KeyDown(DX_RIGHT) ) movedx=10;
  if ( input->KeyDown(DX_UP) ) movedy=-10;
  if ( input->KeyDown(DX_DOWN) ) movedy=10;
  this->MoveBy(movedx,movedy);
 }
};

