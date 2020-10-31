/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
/////////////////////////////////////////////////////// This file is based on code provided in://
////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include "Shape2d.h"

////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Shape2d::Shape2d() {
 stroke=0.0f;
 filled=true;
 outlined=true;
 compiled=false;
}

////////////////////////////////////////////////////////////
/// Create a shape made of a single line
////////////////////////////////////////////////////////////
void Shape2d::Line(double P1X, double P1Y, double P2X, double P2Y, double thickness, Crayon fill, double weight, Crayon stroke)
{
 Vertexd P1(P1X, P1Y);
 Vertexd P2(P2X, P2Y);

 // Compute the extrusion direction
 Vertexd Normal;
 ComputeNormal(P1, P2, &Normal);
 Normal.Multiply(thickness / 2.0);

 // Create the shape's points
 AddPoint(P1.x - Normal.x, P1.y - Normal.y, fill, stroke);
 AddPoint(P2.x - Normal.x, P2.y - Normal.y, fill, stroke);
 AddPoint(P2.x + Normal.x, P2.y + Normal.y, fill, stroke);
 AddPoint(P1.x + Normal.x, P1.y + Normal.y, fill, stroke);

 this->stroke=weight;
 outlined= ( weight > 0.0 );

 // Compile it
 Compile();
}

////////////////////////////////////////////////////////////
/// Create a shape made of a single rectangle
////////////////////////////////////////////////////////////
void Shape2d::Rectangle(double P1X, double P1Y, double P2X, double P2Y, Crayon fill, double weight, Crayon stroke) {
 // Create the shape's points
 AddPoint(P1X, P1Y, fill, stroke);
 AddPoint(P2X, P1Y, fill, stroke);
 AddPoint(P2X, P2Y, fill, stroke);
 AddPoint(P1X, P2Y, fill, stroke);
 this->stroke=weight;
 outlined= ( weight > 0.0 );

 // Compile it
 Compile();
}

////////////////////////////////////////////////////////////
/// Create a shape made of a single circle
////////////////////////////////////////////////////////////
void Shape2d::Circle(double X, double Y, double Radius, Crayon fill, double weight, Crayon stroke, int precision ) {
 // Create the points set
 for (int i = 0; i < precision; ++i) {
  double Angle = i * TWO_PI / precision;
  Vertexd Offset(cos(Angle), sin(Angle));
  AddPoint( X+Offset.x * Radius, Y+Offset.y * Radius, fill, stroke );
 }
 // Compile it
 this->stroke=weight;
 outlined = ( weight > 0.0 );
 Compile();
}

////////////////////////////////////////////////////////////
/// /see Drawable::Render
////////////////////////////////////////////////////////////
void Shape2d::Render() {
// glMatrixMode(GL_MODELVIEW);
// glPushMatrix();
 glTranslated(position.x,position.y,position.z);
 Crayon calculator;
 // Make sure the shape has at least 3 points (4 if we count the center)
 if (points.size() < 3) return;
 // Make sure the shape is compiled
 if (!compiled) Compile();
 // Shapes only use color, no texture (SAYS WHO??!)
 glDisable(GL_TEXTURE_2D);
 // Draw the shape
 if (filled) {
  glBegin(GL_TRIANGLE_FAN);
  for (std::vector<Point2d>::const_iterator i = points.begin(); i != points.end(); ++i) {
   glColor4fv(i->color.floats);
   glVertex2d(i->position.x, i->position.y);
  }
  // Close the shape by duplicating the first point at the end
  glColor4fv(points[0].color.floats);
  glVertex2d(points[0].position.x, points[0].position.y);
  glEnd();
 }
 // Draw the outline
 if (outlined)
 {
  glBegin(GL_TRIANGLE_STRIP);
  for (std::size_t i = 0; i < points.size(); ++i) {
   glColor4fv(points[i].color.floats);
   glVertex2d(points[i].position.x, points[i].position.y);
   glColor4fv(points[i].color.floats);
   glVertex2d(points[i].position.x + points[i].normal.x * stroke, centroid.position.y + centroid.normal.y * stroke);
  }
  // Close the shape by duplicating the first point at the end
  glColor4fv(points[0].color.floats);
  glVertex2d(points[0].position.x, centroid.position.y);
  glColor4fv(points[0].color.floats);
  glVertex2d(points[0].position.x + points[0].normal.x * stroke, points[0].position.y + points[0].normal.y * stroke);
  glEnd();
 }
 glTranslated(-position.x,-position.y,-position.z);

// glPopMatrix();
}


////////////////////////////////////////////////////////////
/// Compile the shape : compute its center and its outline
////////////////////////////////////////////////////////////
void Shape2d::Compile()
{
 // Compute the center
 float NbPoints = static_cast<float>(points.size() - 1);
 float R = 0, G = 0, B = 0, A = 0;
 centroid.position.Set(Vertexd(0, 0));
 centroid.color.Pick(clear);
 for (std::size_t i = 1; i < points.size(); ++i) {
  centroid.position.Add( &points[i].position );
  centroid.position.Divide( (double) NbPoints );
  R += points[i].color.rf / NbPoints;
  G += points[i].color.gf / NbPoints;
  B += points[i].color.bf / NbPoints;
  A += points[i].color.af / NbPoints;
 }
 centroid.color.Float(R,G,B,A);
 // Compute the outline
 for (std::size_t i = 1; i < points.size(); ++i)
 {
  // Get the two segments shared by the current point
  Point2d& P0 = (i == 1) ? points[points.size() - 1] : points[i - 1];
  Point2d& P1 = points[i];
  Point2d& P2 = (i == points.size() - 1) ? points[1] : points[i + 1];
  // Compute their normal
  Vertexd Normal1, Normal2;
  if (!ComputeNormal(P0.position, P1.position, &Normal1) 
   || !ComputeNormal(P1.position, P2.position, &Normal2))
      continue;
  // Add them to get the extrusion direction
  double Factor = 1.0 + (SQ(Normal1.x) + SQ(Normal1.y));
  P1.normal.Set(Normal1);
  P1.normal.Add(&Normal2);
  P1.normal.Divide(Factor);
  // Make sure it points towards the outside of the shape
  double Dot = (P1.position.x - centroid.position.x) * P1.normal.x + (P1.position.y - centroid.position.y) * P1.normal.y;
  if (Dot < 0) P1.normal.Multiply(-1.0);
 }
 compiled=true;
}


////////////////////////////////////////////////////////////
/// Compute the normal of a given 2D segment
////////////////////////////////////////////////////////////
bool Shape2d::ComputeNormal(Vertexd P1, Vertexd P2, Vertexd *Normal)
{
 Normal->x = P1.y - P2.y;
 Normal->y = P2.x - P1.x;
 double Len = sqrt(Normal->x * Normal->x + Normal->y * Normal->y);
 if (Len == 0.f) return false;
 Normal->x /= Len;
 Normal->y /= Len;
 return true;
}


////////////////////////////////////////////////////////////
/// Default constructor for Point
////////////////////////////////////////////////////////////
Shape2d::Point2d::Point2d(Vertexd Pos, Crayon C, Crayon O) {
 position.Set(Pos);
 normal.Set(0.0,0.0);
 color.fromCrayon(C);
 outline.fromCrayon(O);
}