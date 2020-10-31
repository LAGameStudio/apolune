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
////////////////////////////////////////////////////////////
// Extricated then assimilated from:
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
#pragma once
#include "Art.h"
#include "Crayon.h"
#include "Vertex.h"
#include "ListItem.h"

////////////////////////////////////////////////////////////
/// Shape defines a drawable convex shape ; it also defines
/// helper functions to draw simple shapes like
/// lines, rectangles, circles, etc.
////////////////////////////////////////////////////////////
class Shape2d : public ListItem { // AKA Convex Hull
public:
 Vertexd position;
 Blends blend;

 ////////////////////////////////////////////////////////////
 /// Defines a simple 2D point
 ////////////////////////////////////////////////////////////
 struct Point2d {
 public:
  Point2d(Vertexd Pos = Vertexd(0, 0), Crayon C = Crayon(255, 255, 255), Crayon OutlineC = Crayon(255, 255, 255));
  Vertexd position; ///< Position
  Vertexd normal;   ///< Extruded normal
  Crayon color;     ///< Color of the point
  Crayon outline;   ///< Outline color of the point
 };
 Point2d centroid;

 ////////////////////////////////////////////////////////////
 // Member data
 ////////////////////////////////////////////////////////////
 std::vector<Point2d> points;   ///< Points composing the shape
 double             stroke;     ///< Outline width
 bool               filled;     ///< Should we draw the inside if the shape ?
 bool               outlined;   ///< Should we draw the outline if the shape ?
 bool               compiled;   ///< Compiled state of the shape

 ////////////////////////////////////////////////////////////
 /// Default constructor
 ///
 ////////////////////////////////////////////////////////////
 Shape2d();

 ////////////////////////////////////////////////////////////
 /// Add a point to the shape
 ///
 /// \param X, Y :       Position of the point
 /// \param Col :        Color of the point (white by default)
 /// \param OutlineCol : Outline color of the point (black by default)
 ///
 ////////////////////////////////////////////////////////////
 void AddPoint(double X, double Y, Crayon c = Crayon(255, 255, 255), Crayon o = Crayon(0, 0, 0)) {
  AddPoint(Vertexd(X,Y),c,o);
 }

 ////////////////////////////////////////////////////////////
 /// Add a point to the shape
 ///
 /// \param Position :   Position of the point
 /// \param Col :        Color of the point (white by default)
 /// \param OutlineCol : Outline color of the point (black by default)
 ///
 ////////////////////////////////////////////////////////////
 void AddPoint(Vertexd p, Crayon c = Crayon(255, 255, 255), Crayon o = Crayon(0, 0, 0)) {
  points.push_back(Point2d(p, c, o));
  compiled = false;
 }

 ////////////////////////////////////////////////////////////
 /// Get the number of points composing the shape
 ///
 /// \param Total number of points
 ///
 ////////////////////////////////////////////////////////////
 unsigned int TotalPoints() { return static_cast<unsigned int>(points.size() - 1); }

 ////////////////////////////////////////////////////////////
 /// Set the position of a point
 ///
 /// \param Index :    Index of the point, in range [0, GetNbPoints() - 1]
 /// \param Position : New position of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 void SetPointPosition(unsigned int Index, Vertexd Position) {
  points[Index].position.Set(Position);
  compiled = false;
 }

 ////////////////////////////////////////////////////////////
 /// Set the position of a point
 ///
 /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
 /// \param X :     New X coordinate of the Index-th point
 /// \param Y :     New Y coordinate of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 void SetPointPosition(unsigned int Index, double X, double Y) {
  SetPointPosition(Index, Vertexd(X, Y));
 }

 ////////////////////////////////////////////////////////////
 /// Set the color of a point
 ///
 /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
 /// \param Col :   New color of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 void SetPointColor(unsigned int Index, Crayon C) {
  points[Index].color.fromCrayon(C);
  compiled = false;
 }

 ////////////////////////////////////////////////////////////
 /// Set the outline color of a point
 ///
 /// \param Index :      Index of the point, in range [0, GetNbPoints() - 1]
 /// \param OutlineCol : New outline color of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 void SetPointOutlineColor(unsigned int Index, Crayon O) {
  points[Index].outline.fromCrayon(O);
  compiled = false;
 }

 ////////////////////////////////////////////////////////////
 /// Get the position of a point
 ///
 /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
 ///
 /// \return Position of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 Vertexd GetPointPosition(unsigned int Index) {
  return points[Index].position;
 }

 ////////////////////////////////////////////////////////////
 /// Get the color of a point
 ///
 /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
 ///
 /// \return Color of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 Crayon GetPointColor(unsigned int Index) {
  return points[Index].color;
 }

 ////////////////////////////////////////////////////////////
 /// Get the outline color of a point
 ///
 /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
 ///
 /// \return Outline color of the Index-th point
 ///
 ////////////////////////////////////////////////////////////
 Crayon GetPointOutlineColor(unsigned int Index) {
  return points[Index].outline;
 }

 ////////////////////////////////////////////////////////////
 /// Create a shape made of a single line (use floats)
 ///
 /// \param P1X, P1Y :   Position of the first point
 /// \param P2X, P2Y :   Position second point
 /// \param thickness :  Line thickness
 /// \param fill :       Color used to draw the line
 /// \param weight :     Outline width (0 by default)
 /// \param stroke :     Color used to draw the outline (black by default)
 ///
 ////////////////////////////////////////////////////////////
 void Line(double P1X, double P1Y, double P2X, double P2Y, double thickness, Crayon fill, double weight, Crayon stroke);

 ////////////////////////////////////////////////////////////
 /// Create a shape made of a single rectangle (use floats)
 ///
 /// \param P1X, P1Y :   Position of the first point
 /// \param P2X, P2Y :   Position second point
 /// \param fill :       Color used to fill the rectangle
 /// \param weight :     Outline width (0 by default)
 /// \param stroke :     Color used to draw the outline (black by default)
 ///
 ////////////////////////////////////////////////////////////
 void Rectangle(double P1X, double P1Y, double P2X, double P2Y, Crayon fill, double weight, Crayon stroke);

 ////////////////////////////////////////////////////////////
 /// Create a shape made of a single circle (use floats)
 ///
 /// \param X, Y :       Position of the center
 /// \param Radius :     Radius
 /// \param Col :        Color used to fill the circle
 /// \param Outline :    Outline width (0 by default)
 /// \param OutlineCol : Color used to draw the outline (black by default)
 ///
 ////////////////////////////////////////////////////////////
 void Circle(double X, double Y, double Radius, Crayon fill, double weight, Crayon stroke, int precision=40 );

 ////////////////////////////////////////////////////////////
 /// /see Drawable::Render
 ///
 ////////////////////////////////////////////////////////////
 virtual void Render();
 //virtual void Render( GLWindow *surface ); needs written

 ////////////////////////////////////////////////////////////
 /// Compile the shape : compute its center and its outline
 ///
 ////////////////////////////////////////////////////////////
 void Compile();

 ////////////////////////////////////////////////////////////
 /// Compute the normal of a given 2D segment
 ///
 /// \param P1 :     First point of the segment
 /// \param P2 :     Second point of the segment
 /// \param Normal : Calculated normal
 ///
 /// \return False if the normal couldn't be calculated (segment is null)
 ///
 ////////////////////////////////////////////////////////////
 bool ComputeNormal(Vertexd P1, Vertexd P2, Vertexd *Normal);
};

class Shape2ds : public LinkedList {
public:
 void Render() { FOREACH(Shape2d,shape) shape->Render(); }
 CLEARLISTRESET(Shape2d);
 ~Shape2ds() { Clear(); }
};