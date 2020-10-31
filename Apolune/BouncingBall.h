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
#include "TextureLibrary.h"
#include "Cartesian.h"
#include "Art.h"

class BouncingBall : public GLWindow {
public:
 GLImage *ball;
 Cartesian ballPosition;
 int size;
 void OnLoad() {
  Anchor(0.25f,0.25f,0.25f,0.25f);
  background.Pick(gray);
  ball=library.Load("data/images/flares/green.png");
  ballPosition.SetRect(0,0,size=32,32);
 }
 void Between() {
  if ( input->left ) ballPosition.SetRect( input->mxi-(size/2), input->myi-(size/2), size,size);
 }
 void Render() {
  QuadiNoAlpha(ball,crayons.Pick(alabaster),transparency,&ballPosition);
 }
 void WheelUp() {
  size+=1;
  ballPosition.SetRect( input->mxi-(size/2), input->myi-(size/2), size,size);
 }
 void WheelDown() {
  size-=1; if ( size < 0 ) size=32;
  ballPosition.SetRect( input->mxi-(size/2), input->myi-(size/2), size,size);
 }
};

