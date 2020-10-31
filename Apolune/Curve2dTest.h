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
#include "Vertex.h"
#include "Curve2d.h"
#include "Art.h"

class Curve2dTest : public GLWindow {
public:
 Zint setting;
 Cartesiand A,B,C,D;
 Vertices *result;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(green);
  background.ScaleByAlphaf(0.1f);
  curve2d.SetLOD(64);
  result=curve2d.Bezier( 300.0, 300.0, 250.0, 300.0, 150.0, 0.0, 200.0, 50.0 );
 }
 void LeftDown() {
  switch ( (int) setting % 4 ) {
   case 0: A.Set((double)input->mxi, (double)input->myi); break;
   case 1: B.Set((double)input->mxi, (double)input->myi); break;
   case 2: C.Set((double)input->mxi, (double)input->myi); break;
   case 3: D.Set((double)input->mxi, (double)input->myi); break;
  }
  delete result;
  result=curve2d.Bezier( A.x,A.y, B.x,B.y, C.x,C.y, D.x,D.y );
  setting++;
 }
 void RightDown() {
  switch ( (int) setting % 4 ) {
   case 0: A.Set((double)input->mxi, (double)input->myi); break;
   case 1: B.Set((double)input->mxi, (double)input->myi); break;
   case 2: C.Set((double)input->mxi, (double)input->myi); break;
   case 3: D.Set((double)input->mxi, (double)input->myi); break;
  }
  delete result;
  result=curve2d.Hermite( A.x,A.y, B.x,B.y, C.x,C.y, D.x,D.y );
  setting++;
 }
 void Render() {
  DrawLine2Color( crayons.Pick(green), crayons.Pick(yellow), (int) A.x, (int) A.y, (int) B.x, (int) B.y );
  DrawLine2Color( crayons.Pick(green), crayons.Pick(yellow), (int) B.x, (int) B.y, (int) C.x, (int) C.y );
  DrawLine2Color( crayons.Pick(green), crayons.Pick(yellow), (int) C.x, (int) C.y, (int) D.x, (int) D.y );
  crayons.Pick(pink);
  Text("A",A.x,A.y,8,10,false,0);
  Text("B",B.x,B.y,8,10,false,0);
  Text("C",C.x,C.y,8,10,false,0);
  Text("D",D.x,D.y,8,10,false,0);
  glColor4f(0.0f,1.0f,1.0f,1.0f);
  glBegin(GL_LINES);
  EACH(result->first,Vertex,p) {
   glVertex2f( p->x, p->y );
  }
  glEnd();
 }
};