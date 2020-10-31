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
#include "Vertexd.h"
#include "VBO.h"
#include "Curve2d.h"

// Maintains and renders a bendable quad
//
// Uses a curve to modify a grid, sort of like a '2d cloth'
//

struct BendableQuadPrimitive {
 Vertexd point[4];
};

// Bends a quad into the 1.0 space (magnitude of curve is 1.0)
class QuadBender : public ListItem {
public:
 double a,b; // side ratios
 BendableQuadPrimitive primitive;
 VBOStaticVT vbo;
 Zdis<Vertices> result;
 QuadBender(void) {
  result=
  curve2d.Bezier(
   primitive.point[0].x, primitive.point[0].y,
   primitive.point[1].x, primitive.point[1].y,
   primitive.point[2].x, primitive.point[2].y,
   primitive.point[3].x, primitive.point[3].y
  );
 }
 ~QuadBender(void) {
 }
 void Bend( double bend ) {
  a=bend;
  b=1.0-bend;
 }
 void SkewTop( double ratio ) {
 }
};

