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
#include "GLImage.h"
#include "Vertex.h"

class GLine
{
public:
   Vertex p0;
   Vertex delta;

   GLine(void) {};
};

struct GPlane
{
public:
   double a, b, c, d;
   Vertex normal;

   GPlane(void) {
    a=b=c=d=0;
   }

   void FromPoints(Vertex v1, Vertex v2, Vertex v3)
   {
      Vertex a, b;

      a.Subtract(&v3, &v1);
      b.Subtract(&v2, &v1);
      normal.CrossProduct(&a, &b);
      normal.Normalize(&normal);

      d = (double) -normal.DotProduct(&v1);
   }

   void FromPointsf(Vertex v1, Vertex v2, Vertex v3)
   {
      Vertex v1d;
      v1d.x=v1.x;
      v1d.y=v1.y;
      v1d.z=v1.z;
      Vertex v2d;
      v2d.x=v2.x;
      v2d.y=v2.y;
      v2d.z=v2.z;
      Vertex v3d;
      v3d.x=v3.x;
      v3d.y=v3.y;
      v3d.z=v3.z;
      Vertex a, b;

      a.Subtract(&v3d, &v1d);
      b.Subtract(&v2d, &v1d);
      normal.CrossProduct(&a, &b);
      normal.Normalize(&normal);

      d = -normal.DotProduct(&v1d);
   }

   void MultMatrix(GPlane source, Matrix16 inverseTranspose)
   {
      a = source.a * inverseTranspose.m[0][0] + 
          source.b * inverseTranspose.m[1][0] +
          source.c * inverseTranspose.m[2][0] + 
          source.d * inverseTranspose.m[3][0];
      b = source.a * inverseTranspose.m[0][1] + 
          source.b * inverseTranspose.m[1][1] +
          source.c * inverseTranspose.m[2][1] + 
          source.d * inverseTranspose.m[3][1];
      c = source.a * inverseTranspose.m[0][2] + 
          source.b * inverseTranspose.m[1][2] +
          source.c * inverseTranspose.m[2][2] + 
          source.d * inverseTranspose.m[3][2];
      d = source.a * inverseTranspose.m[0][3] + 
          source.b * inverseTranspose.m[1][3] +
          source.c * inverseTranspose.m[2][3] + 
          source.d * inverseTranspose.m[3][3];
   }

   void IntersectLine(GLine line, Vertex *result)
   {
      double divisor = a * line.delta.x + b * line.delta.y + c * line.delta.z;

      result->x = (GLfloat) ( (b * line.delta.y * line.p0.x - b * line.delta.x * line.p0.y + 
                   c * line.delta.z * line.p0.x - c * line.delta.x * line.p0.z -
                   d * line.delta.x ) / divisor );

      result->y = (GLfloat) ( (a * line.delta.x * line.p0.y - a * line.delta.y * line.p0.x + 
                   c * line.delta.z * line.p0.y - c * line.delta.y * line.p0.z - 
                   d * line.delta.y ) / divisor );

      result->z = (GLfloat) ( (a * line.delta.x * line.p0.z - a * line.delta.z * line.p0.x + 
                   b * line.delta.y * line.p0.z - b * line.delta.z * line.p0.y -
                   d * line.delta.z ) / divisor );
   }

   void IntersectLinef(GLine line, Vertex *result)
   {
      double divisor = a * line.delta.x + b * line.delta.y + c * line.delta.z;

      result->x = (GLfloat)((b * line.delta.y * line.p0.x - b * line.delta.x * line.p0.y + 
                  c * line.delta.z * line.p0.x - c * line.delta.x * line.p0.z -
                  d * line.delta.x ) / divisor);

      result->y = (GLfloat)((a * line.delta.x * line.p0.y - a * line.delta.y * line.p0.x + 
                  c * line.delta.z * line.p0.y - c * line.delta.y * line.p0.z - 
                  d * line.delta.y ) / divisor);

      result->z = (GLfloat)((a * line.delta.x * line.p0.z - a * line.delta.z * line.p0.x + 
                  b * line.delta.y * line.p0.z - b * line.delta.z * line.p0.y -
                  d * line.delta.z ) / divisor);
   }

   void FromPointNormal(Vertex normal, Vertex point)
   {
      this->normal = normal;
      this->d = -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
   }
};
