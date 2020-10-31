
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

 /* Parts written by Jerome Jacovella-St. Louis (c) 2009 and perhaps earlier.  I didn't have to
    put this notice here, but out of courtesy I did. */

#include <Windows.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"

#include "macros.h"
#include "Vertex.h"
#include "Matrix16.h"
#include "Quaternion.h"
#include "Camera.h"

gCamera::gCamera(void)
{
 needUpdate = true;
 type = Fixed;
 orientation.w = 1;
 aspectRatio = 0;
 fov = PI/2;
 zMin = 5;//0.1f;
 zMax = 10000;
 type=(CameraType) 0;
 target=null;
 fovDirection=(FovDirection) 0;
 aspectRatio=0.0f;
 fovLeft=fovRight=fovTop=fovBottom=fov;
 focalX=focalY=0;
 fovX=fovY=0;
 slerpAmount=slerpPosition=0.0f;
 width=height=0;
 cPosition.Set(0.0f,0.0f,1.0f);
 nearDistance=0.1f;
}


gCamera::~gCamera(void)
{
}

void gCamera::Setup(int width, int height, Vertex *origin)
{
   if(this)
   {
      Vertex vector;
      vector.x=0.0f;
      vector.y=0.0f;
      vector.z=1.0f;
      Quaternion quat;

      float aspectRatio = this->aspectRatio;
      float l, r, t, b;

      if(!aspectRatio)
         aspectRatio = (float)width/height;

      this->width = width;
      this->height = height;
      if(origin != null)
         this->origin = *origin;
      else
      {
         this->origin.x = (float) width /2.0f;
         this->origin.y = (float) height/2.0f;
      }

      l = this->origin.x - 0;
      r = this->origin.x - width;
      t = 0 - this->origin.y;
      b = height - this->origin.y;

      if(fovDirection == Horizontal || (fovDirection == Widest && width * this->aspectRatio > height))
      {
         focalX = (float)((width / 2) / tan(fov/2));
         focalY = focalX * height / width;
         focalY *= aspectRatio;
         fovX = fov;
      }
      else
      {
         focalY = (float)((height / 2) / tan(fov/2));
         focalX = focalY * width / height;
         focalX /= aspectRatio;
         fovY = fov;
      }

      fovX = atan((width / 2)  / focalX) * 2;
      fovY = atan((height / 2) / focalY) * 2;

      fovLeft   = atan(l / focalX);
      fovRight  = atan(r / focalX);
      fovTop    = atan(t / focalY);
      fovBottom = atan(b / focalY);

      // Compute Clipping Planes
      {
         Vertex normal;
         Vertex point;

         // --- Left ---
         quat.Yaw(fovLeft - PI/2);
         quat.ToDirection(&normal);
         viewClippingPlanes[0].FromPointNormal(normal, point);

         // --- Right ---
         quat.Yaw(fovRight + PI/2);
         quat.ToDirection(&normal);
         viewClippingPlanes[1].FromPointNormal(normal, point);

         // --- Top ---
         quat.Pitch(fovTop + PI/2);
         quat.ToDirection(&normal);
         viewClippingPlanes[2].FromPointNormal(normal, point);

         // --- Bottom ---
         quat.Pitch(fovBottom - PI/2);
         quat.ToDirection(&normal);
         viewClippingPlanes[3].FromPointNormal(normal, point);

         // --- Near ---
         normal.x = 0; normal.y = 0; normal.z = 1;
         point.z = zMin;
         viewClippingPlanes[4].FromPointNormal(normal, point);

         // --- Far ---
         normal.x = 0; normal.y = 0; normal.z = -1;
         point.z = zMax;
         viewClippingPlanes[5].FromPointNormal(normal, point);
      }

      needUpdate = true;
   }
}

void gCamera::AdjustPosition(Vertex *p)
{
   int c;
   Matrix16 transpose = viewMatrix;

   cPosition.Set( p );

   transpose.m[0][3] = cPosition.x;
   transpose.m[1][3] = cPosition.y;
   transpose.m[2][3] = cPosition.z;
   inverseTranspose.Inverse(&transpose);

   for(c = 0; c<6; c++)
      worldClippingPlanes[c].MultMatrix(viewClippingPlanes[c], inverseTranspose);
}

void gCamera::AdjustAngle(Quaternion *a)
{
   cAngle = *a;
   a->RotationQuaternion(&inverseMatrix,a);
   
//   inverseMatrix.RotationQuaternion(angle);
   viewMatrix.Transpose(&inverseMatrix);

   AdjustPosition(&cPosition);
}

bool gCamera::Update(void)
{
 bool result = false;
 if(this)
 {
  Matrix16 matrix;
  Transform * target = null;//this->target ? &(this->target->transform) : null;
  Vertex oldPosition = cPosition, newPosition;

  switch(this->type)
  {
   case Fixed:
   case fixedQuaternion:
   {
      newPosition.Set( &position );
      toAngle = orientation;
      break;
   }
   case Attached:
   case attachedQuaternion:
   {
      toAngle.w=1; toAngle.x=toAngle.y=toAngle.z=0;
      if(target)
      {
       if(type == Attached)
       {
          Euler eulerCamera;
          orientation.SetQuaternion(&eulerCamera,&orientation);
          Euler eulerTarget;
          orientation.SetQuaternion(&eulerTarget,&(target->orientation));
          Euler euler;

          euler.Add(&eulerCamera, &eulerTarget);

          // OUTPUT("yaw = %f, pitch = %f\n", eulerCamera.yaw, eulerCamera.pitch);

          Quaternion *q=toAngle.GetQuaternion(&euler);
          toAngle = *q;
          delete q;
       }
       else if(type == attachedQuaternion)
          toAngle.Multiply(&orientation, &(target->orientation)); 
      }
      else
         toAngle = orientation;

      toAngle.RotationMatrix(&matrix);
      newPosition.MultMatrix(&position, &matrix);
      if(target)
         newPosition.Add(&newPosition, &(target->position));
      break;
   }
   case lookAt:
   {
      Quaternion result;
      Vertex direction;
      newPosition.Set( &position );
      if(target)
      {
         direction.Subtract(&(target->position), &position);
         toAngle.RotationDirection(&direction);
      }
      else
      {
         Vertex p;
         direction.Subtract(&p, &(this->position));
         toAngle.RotationDirection(&direction);
      }
      result.Multiply(&orientation, &toAngle);
      toAngle = result;
      break;
   }
   case lookAtObject:/*
   {
      MapObject *cameraObject = (this->target);
      toAngle = cameraObject->transform.orientation;
      if( !cameraObject->parent)
         newPosition = cameraObject->transform.position;
      else
         newPosition.MultMatrix(&(cameraObject->transform.position), &(cameraObject->parent->matrix));
      break;
   }*/break;
  }

  if(cAngle.w != toAngle.w || cAngle.x != toAngle.x || 
     cAngle.y != toAngle.y || cAngle.z != toAngle.z ||
     needUpdate)
  {
   cPosition.Set( &newPosition );
   if(slerpAmount && slerpPosition < 1.0)
   {
      Quaternion angle;
      slerpPosition += slerpAmount;
      slerpPosition = min(slerpPosition, 1.0f);
      angle.Slerp(&fromAngle, &toAngle, slerpPosition);
      AdjustAngle(&angle);
   }
   else
      AdjustAngle(&toAngle);

   result = true;
   this->needUpdate = false;
  }
  else if(newPosition.x != oldPosition.x || newPosition.y != oldPosition.y || newPosition.z != oldPosition.z)
  {
   AdjustPosition(&newPosition);
   result = true;
  }
 }
 return result;
}

bool gCamera::SphereVisible(Vertex *center, float radius)
{
   // TURN BACK ON
   /*
   if(wLeftNormal.DotProduct(center) + wLeftD < -radius)
      return false;
   if(wRightNormal.DotProduct(center) + wRightD < -radius)
      return false;
   if(wTopNormal.DotProduct(center) + wTopD < -radius)
      return false;
   if(wBottomNormal.DotProduct(center) + wBottomD < -radius)
      return false;
   if(wNearNormal.DotProduct(center) + wNearD < -radius)
      return false;
/*   if(wFarNormal.DotProduct(center) + wFarD < -radius)
      return false;*/
   return true;
}


bool gCamera::PointsVisible(Vertex * points, int numPoints, double treshold)
{
 int p;
 int c;
 for(p = 0; p<6; p+=2)
 {
  bool out1a = true, out2a = true;
  bool out1b = true, out2b = true;
  GPlane * plane = &worldClippingPlanes[p];
  for(c = 0; c<numPoints; c++)
  {
   double dot = 
    plane->a * points[c].x + 
    plane->b * points[c].y +
    plane->c * points[c].z;
   if(dot + plane->d > 0)
   {
    out1a = out1b = false;
    break;
   }
   else if(dot + plane->d > -treshold) out1a = false;
  }
  plane = &worldClippingPlanes[p+1];
  for(c = 0; c<numPoints; c++)
  {
   double dot = 
    plane->a * points[c].x + 
    plane->b * points[c].y + 
    plane->c * points[c].z;
   if(dot + plane->d > 0)
   {
    out2a = out2b = false;
    break;
   }
   else if(dot + plane->d > -treshold) out2a = false;
  }

  if((out1a && !out2b) || (out2a && !out1b))
     return false;
 }
 return true;
}

void gCamera::TransformPoint(Vertex *dest, Vertex *src)
{
   Vertex vector;
   vector.x=src->x - cPosition.x;
   vector.y=src->y - cPosition.y;
   vector.z=src->z - cPosition.z;
   dest->MultMatrix(&vector, &viewMatrix);
}

void gCamera::TransformNormal(Vertex *dest, Vertex *src)
{
   dest->MultMatrix(src, &viewMatrix);
}

void gCamera::TransformMatrix(Matrix16 *dest, Matrix16 *src)
{
   Matrix16 matrix = *src;
   matrix.m[3][0] -= cPosition.x;
   matrix.m[3][1] -= cPosition.y;
   matrix.m[3][2] -= cPosition.z;
   dest->Multiply(&matrix, &viewMatrix);
}

void gCamera::RotatePitch(double amount, double m, double ma)
{
   if(type == fixedQuaternion)
   {
      orientation.RotatePitch(amount);
   }
   else
   {
      Euler euler; 
      orientation.SetQuaternion(&euler,&orientation);
      euler.pitch += amount;
      if(m || ma)
      {
         euler.pitch = min(euler.pitch, ma);
         euler.pitch = max(euler.pitch, m);
      } 
      Quaternion *q=orientation.GetQuaternion(&euler);
      orientation=*q;
      delete q;
   }
}

void gCamera::RotateYaw(double amount, double m, double ma)
{
   if(type == fixedQuaternion)
   {
      orientation.RotateYaw(amount);
   }
   else
   {
      Euler euler; 
      orientation.SetQuaternion(&euler,&orientation);
      euler.yaw += amount;
      if(m || ma)
      {
         euler.yaw = min(euler.yaw, ma);
         euler.yaw = max(euler.yaw, m);
      } 
      Quaternion *q=orientation.GetQuaternion(&euler);
      orientation=*q;
      delete q;
   }
}

void gCamera::RotateRoll(double amount, double m, double ma)
{
   if(type == fixedQuaternion)
   {
      orientation.RotateRoll(amount);
   }
   else
   {
      Euler euler; 
      orientation.SetQuaternion(&euler,&orientation);
      euler.roll += amount;
      if(m || ma)
      {
         euler.roll = min(euler.roll, ma);
         euler.roll = max(euler.roll, m);
      } 
      Quaternion *q=orientation.GetQuaternion(&euler);
      orientation=*q;
      delete q;
   }
}

void gCamera::Slerp(float amount)
{
   fromAngle = cAngle;
   slerpAmount = amount;
   slerpPosition = 0;
}

void gCamera::Move(Vertex *d)
{
   Matrix16 matrix;

   switch(type)
   {
      case Fixed:
      {
         Vertex offset;
         orientation.RotationQuaternion(&matrix,&orientation);
         offset.MultMatrix(d, &matrix);
         position.Add(&position, &offset);
         break;
      }
      case attachedQuaternion:
      case Attached:
      case lookAt:
      {
         position.Add(&position,d);
         break;
      }
   }
}

bool gCamera::Project(Vertex *vector, Vertex *point)
{
   if(vector->z >= zMin)
   {
      float floatZ;
      point->x = (vector->x*focalX/vector->z);
      point->y = (vector->y*focalY/vector->z);
      point->z = (((zMax * zMin / -vector->z) + zMax) / (zMax - zMin));
      floatZ = ((((float)zMax * (float)zMin / -(float)vector->z) + (float)zMax) / ((float)zMax - (float)zMin));
      point->x += origin.x;
      point->y += origin.y;
      return (point->x >= 0 && point->y >= 0 && 
              point->x < width && point->y < height);
   }
   return false;
}

void gCamera::Unproject(Vertex *point, Vertex *vector)
{
   vector->z = (zMax * zMin / (zMax - point->z * (zMax-zMin)));
   vector->y = ((point->y - origin.y) * vector->z / focalY);
   vector->x = ((point->x - origin.x) * vector->z / focalX);
}

bool gCamera::ProjectSize(Vertex *vector, Cartesian *point)
{
   if(vector->z >= zMin)
   {
      point->x = (int)((double)vector->x*(double)focalX/(double)vector->z);
      point->y = (int)((double)vector->y*(double)focalY/(double)vector->z);
      return true;
   }
   return false;
}

void gCamera::Untransform(Vertex *src, Vertex *result)
{
   result->MultMatrix(src, &inverseMatrix);
   result->x += cPosition.x;
   result->y += cPosition.y;
   result->z += cPosition.z;
}