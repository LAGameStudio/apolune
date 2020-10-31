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
 /* Parts written by Jerome Jacovella-St. Louis (c) 2009 and perhaps earlier.  I didn't have to
    put this notice here, but out of courtesy I did. */
#pragma once

#include "GLWindow.h"
#include "GLImage.h"
#include "Cartesian.h"
#include "Vertex.h"
#include "Geometry.h"
#include "Quaternion.h"

enum CameraType { Fixed=0, fixedQuaternion=1, Attached=2, attachedQuaternion=3, lookAt=4, lookAtObject=5 };
enum FovDirection { Widest=0, Horizontal=1, Vertical=2 };
//enum ClippingPlane { ClipLeft=0, Right=1, Top=2, Bottom=3, Near=4, Far=5 };

class gCamera {
public:
 CameraType type;
 Vertex position;
 Quaternion orientation;
 Vertex cPosition;
 Quaternion cAngle;
 float zMin, zMax;
 void *target;
 FovDirection fovDirection;
 float aspectRatio, nearDistance;
 Cartesian size;
 double fovLeft, fovRight, fovTop, fovBottom;
 double fov;
 float focalX, focalY;
 double fovX,fovY;
 float slerpAmount, slerpPosition;
 GPlane viewClippingPlanes[6], worldClippingPlanes[6];
 Matrix16 inverseTranspose, inverseMatrix;
 Quaternion fromAngle, toAngle;
 bool needUpdate;
 Matrix16 viewMatrix;
 int width, height;
 Vertex origin;
 Vector3 lookingAt;
 Vector3 eyeVector;

 gCamera(void);
 ~gCamera(void);

 void Setup(int width, int height, Vertex *origin);
 void AdjustPosition(Vertex *position);
 void AdjustAngle(Quaternion *angle);
 bool Update(void);
 bool SphereVisible(Vertex *center, float radius);
 bool PointsVisible(Vertex *points, int numPoints, double treshold);
 void TransformPoint(Vertex *dest, Vertex *src);
 void TransformNormal(Vertex *dest, Vertex *src);
 void TransformMatrix(Matrix16 *dest, Matrix16 *src);
 void RotatePitch(double amount, double m, double ma);
 void RotateYaw(double amount, double m, double ma);
 void RotateRoll(double amount, double m, double ma);
 void Slerp(float amount);
 void Move(Vertex *direction);
 bool Project(Vertex *vector, Vertex *point);
 void Unproject(Vertex *point, Vertex *vector);
 bool ProjectSize(Vertex *vector, Cartesian *point);
 void Untransform(Vertex *src, Vertex *result);

 
 // Builds a look-at style view matrix.
 // This is essentially the same matrix used by gluLookAt().
 void LookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
  lookingAt = target;
  eyeVector=eye;

 	Vector3 zAxis = eye - target;
 	zAxis.normalize();
 
 	Vector3 xAxis = Vector3::cross(up, zAxis);
 	xAxis.normalize();
 
 	Vector3 yAxis = Vector3::cross(zAxis, xAxis);
 	yAxis.normalize();
 
 	viewMatrix.m[0][0] = xAxis.x;
 	viewMatrix.m[1][0] = xAxis.y;
 	viewMatrix.m[2][0] = xAxis.z;
 	viewMatrix.m[3][0] = -Vector3::dot(xAxis, eye);

  viewMatrix.m[0][1] = yAxis.x;
 	viewMatrix.m[1][1] = yAxis.y;
 	viewMatrix.m[2][1] = yAxis.z;
 	viewMatrix.m[3][1] = -Vector3::dot(yAxis, eye);

  viewMatrix.m[0][2] = zAxis.x;
 	viewMatrix.m[1][2] = zAxis.y;
 	viewMatrix.m[2][2] = zAxis.z;
 	viewMatrix.m[3][2] = -Vector3::dot(zAxis, eye);

  viewMatrix.m[0][3] = 0.0f;
 	viewMatrix.m[1][3] = 0.0f;
 	viewMatrix.m[2][3] = 0.0f;
 	viewMatrix.m[3][3] = 1.0f;
 }

};