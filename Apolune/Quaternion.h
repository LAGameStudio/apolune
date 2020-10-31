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

#include <Windows.h>

#include <math.h>
#include "Matrix16.h"
#include "Vertex.h"

class Quaternion
{
public:
 double w, x, y, z;

 void Identity(void)
 {
  x = y = z = 0;
  w = 1;
 }
 
 Quaternion(void) { Identity(); }
 Quaternion( double ww, double xx, double yy, double zz ) {
  w=ww;
  x=xx;
  y=yy;
  z=zz;
 }
 ~Quaternion(void) {}

 void Set( double ww, double xx, double yy, double zz ) {
  w=ww;
  x=xx;
  y=yy;
  z=zz;
 }

 void Normalize(Quaternion *source)
 {
  double m = sqrt(source->x * source->x +
                  source->y * source->y +
                  source->z * source->z +
                  source->w * source->w);
  if(m) {
   x = (double)(source->x/m);
   y = (double)(source->y/m);
   z = (double)(source->z/m);
   w = (double)(source->w/m);
  }
  else w = x = y = z = 0.0;
 }

 void Multiply(Quaternion *q1, Quaternion *q2)
 {
  w = q1->w * q2->w - q2->x * q1->x - q1->y * q2->y - q1->z * q2->z;
  x = q1->w * q2->x + q2->w * q1->x + q1->y * q2->z - q1->z * q2->y;
  y = q1->w * q2->y + q2->w * q1->y + q1->z * q2->x - q1->x * q2->z;
  z = q1->w * q2->z + q2->w * q1->z + q1->x * q2->y - q1->y * q2->x;
 }

 void Divide(Quaternion *q1, Quaternion *q2)
 {
  w =  q2->w * q1->w + q2->x * q1->x + q2->y * q1->y + q2->z * q1->z;
  x =  q2->w * q1->x - q2->x * q1->w + q2->y * q1->z - q2->z * q1->y;
  y =  q2->w * q1->y - q2->x * q1->z - q2->y * q1->w + q2->z * q1->x;
  z =  q2->w * q1->z + q2->x * q1->y - q2->y * q1->x - q2->z * q1->w;
 }

 void RotationAxis(Vertex *axis, float angleDegrees )
 {
  double sa = sin( angleDegrees / 2.0 );
  double ca = cos( angleDegrees / 2.0 );

  x = (double)(axis->x * sa);
  y = (double)(axis->y * sa);
  z = (double)(axis->z * sa);
  w = (double)ca;
 }

 Quaternion *RotationYawPitchRoll(Euler *euler)
 {
  Quaternion *rotation=new Quaternion;
  Quaternion *result=new Quaternion;

  result->Yaw(euler->yaw);
  rotation->Pitch(euler->pitch);
  Multiply(rotation, result);
  rotation->Roll(euler->roll);
  result->Multiply(rotation, this);
  Normalize(result);
  delete rotation;
  return result;
 }
 
 void SetQuaternion( Euler *euler, Quaternion *value ) {
  double y = 2 * ( value->y*value->z + value->w*value->x );
  if(fabs(y) <= 1.0 - 0.000005) {
   double x =     2.0 * ( value->x*value->z - value->w*value->y );
   double z = 1.0 - 2.0 * ( value->x*value->x + value->y*value->y );
   double yaw = -atan2(x, z);
   double pitch = atan2(y, sqrt(x * x + z * z));
   double sYaw = sin( yaw / 2.0 );
   double cYaw = cos( yaw / 2.0 );
   double sPitch = sin( pitch / 2.0 );
   double cPitch = cos( pitch / 2.0 );
   Quaternion *yp=new Quaternion;
   yp->w=cPitch * cYaw;
   yp->x=sPitch * cYaw;
   yp->y=cPitch * sYaw;
   yp->z=sPitch * sYaw;
   double rollW = yp->w * value->w + yp->x * value->x + yp->y * value->y + yp->z * value->z;
   double rollZ = yp->w * value->z + yp->x * value->y - yp->y * value->x - yp->z * value->w;

   euler->yaw = yaw;
   euler->pitch = pitch;
   euler->roll = atan2(rollZ, rollW) * 2;
   delete yp;
  }
  else
  {
   // 90 degrees pitch case
   double sin45 = sin(QUARTER_PI);
   double yawW = sin45 * value->w + sin45 * value->x;
   double yawY = sin45 * value->y + sin45 * value->z;

   euler->yaw = atan2(yawY, yawW) * 2;
   euler->pitch = HALF_PI;
   euler->roll = 0;
  }
 }

 Quaternion *GetQuaternion( Euler *euler ) {
  double sYaw   = sin( euler->yaw   / 2.0 );
  double cYaw   = cos( euler->yaw   / 2.0 );
  double sPitch = sin( euler->pitch / 2.0 );
  double cPitch = cos( euler->pitch / 2.0 );
  double sRoll  = sin( euler->roll  / 2.0 );
  double cRoll  = cos( euler->roll  / 2.0 );
  Quaternion *yp=new Quaternion,*value= new Quaternion;
  yp->w=cPitch * cYaw;
  yp->x=sPitch * cYaw;
  yp->y=cPitch * sYaw;
  yp->z=sPitch * sYaw;

  value->w = yp->w * cRoll - yp->z * sRoll;
  value->x = yp->x * cRoll - yp->y * sRoll;
  value->y = yp->y * cRoll + yp->x * sRoll;
  value->z = yp->z * cRoll + yp->w * sRoll;
  delete yp;
  return value;
 }

 void RotationDirection(Vertex *direction)
 {
  float yaw = -atan2(direction->x, direction->z);
  float pitch = atan2(direction->y, sqrt(direction->x * direction->x + direction->z * direction->z));
  YawPitch(yaw, pitch);
 }

 void RotationMatrix(Matrix16 *m) {
  double t = m->m[0][0] + m->m[1][1] + m->m[2][2] + 1.0;
  if(t > 0) {
   double s = sqrt(t) * 2.0;
   w = (0.25 * s);
   x = (( m->m[2][1] - m->m[1][2] ) / s);
   y = (( m->m[0][2] - m->m[2][0] ) / s);
   z = (( m->m[1][0] - m->m[0][1] ) / s);
  } else {
   double q[3];
   double s;
   int i = 0,j,k;
   int nxt[3] = {1,2,0};
   if(m->m[1][1] > m->m[0][0]) i = 1;
   if(m->m[2][2] > m->m[i][i]) i = 2;
   j = nxt[i];
   k = nxt[j];
   s = sqrt(m->m[i][i] - (m->m[j][j] + m->m[k][k]) + 1.0) * 2;
   w = (double) ((m->m[k][j] - m->m[j][k]) / s);
   q[i] = (double) (0.25f * s);
   q[j] = (double) ((m->m[j][i] - m->m[i][j]) / s);
   q[k] = (double) ((m->m[k][i] - m->m[i][k]) / s);
   x = q[0];
   y = q[1];
   z = q[2];
  }
 }

#define DELTA 0

 void Slerp(Quaternion *from, Quaternion *to, float t) {
  double to1[4];
  double omega, cosom, sinom, scale0, scale1;
  cosom = from->x * to->x + from->y * to->y + from->z * to->z + from->w * to->w;
  if ( cosom < 0.0 ) {
   cosom = -cosom; 
   to1[0] = -to->x;
   to1[1] = -to->y;
   to1[2] = -to->z;
   to1[3] = -to->w;
  } else {
   to1[0] = to->x;
   to1[1] = to->y;
   to1[2] = to->z;
   to1[3] = to->w;
  }
  if ( (1.0 - cosom) > DELTA ) {
   omega = acos(cosom);
   sinom = sin(omega);
   scale0 = sin((1.0 - t) * omega) / sinom;
   scale1 = sin(t * omega) / sinom;
  } else {
   scale0 = 1.0 - t;
   scale1 = t;
  }
  x = (double)(scale0 * from->x + scale1 * to1[0]);
  y = (double)(scale0 * from->y + scale1 * to1[1]);
  z = (double)(scale0 * from->z + scale1 * to1[2]);
  w = (double)(scale0 * from->w + scale1 * to1[3]);
 }

 void Yaw(double angle) {
  double sa = sin( angle / 2.0 );
  double ca = cos( angle / 2.0 );
  x = 0.0;
  y = (double)sa;
  z = 0.0;
  w = (double)ca;
 }

 void YawPitch(double yaw, double pitch) {
  double sYaw   = sin( yaw / 2.0 );
  double cYaw   = cos( yaw / 2.0 );
  double sPitch = sin( pitch / 2.0 );
  double cPitch = cos( pitch / 2.0 );
  w = cPitch * cYaw;
  x = sPitch * cYaw;
  y = cPitch * sYaw;
  z = sPitch * sYaw;
 }

 void Pitch(double angle) {
  double sa = sin( angle / 2.0 );
  double ca = cos( angle / 2.0 );
  x = (double)sa;
  y = 0.0;
  z = 0.0;
  w = (double)ca;
 }

 void Roll(double angle) {
  double sa = sin( angle / 2.0 );
  double ca = cos( angle / 2.0 );
  x = 0.0;
  y = 0.0;
  z = (double)sa;
  w = (double)ca;
 }

 void RotatePitch(double pitch) {
  Quaternion *rotation=new Quaternion;
  Quaternion *result=new Quaternion;
  rotation->Pitch(pitch);
  result->Multiply(rotation, this);
  *this = *result;
  delete result;
  delete rotation;
 }

 void RotateYaw(double yaw) {
  Quaternion *rotation=new Quaternion;
  Quaternion *result=new Quaternion;
  rotation->Yaw(yaw);
  result->Multiply(rotation, this);
  *this = *result;
  delete result;
  delete rotation;
 }

 void RotateRoll(double roll) {
  Quaternion *rotation=new Quaternion;
  Quaternion *result=new Quaternion;
  rotation->Roll(roll);
  result->Multiply(rotation, this);
  *this = *result;
  delete result;
  delete rotation;
 }

 void RotateYawPitch(double yaw, double pitch) {
  Quaternion *rotation=new Quaternion;
  Quaternion *result=new Quaternion;
  rotation->YawPitch(yaw, pitch);
  result->Multiply(rotation, this);
  *this = *result;
  delete result;
  delete rotation;
 }

 void ToDirection(Vertex *direction) {
      /*
      Vector3Df vector { 0,0,1 };
      Matrix mat;
      mat->RotationQuaternion(this);
      direction->Transform(vector, mat);
      */
  direction->x = (GLfloat)(    2.0 * ( x*z - w*y ));
  direction->y = (GLfloat)(    2.0 * ( y*z + w*x ));
  direction->z = (GLfloat)(1.0 - 2.0 * ( x*x + y*y ));
 }

 // Calculates and stores the angle between the two vectors.
 void AngleBetween( Vertex *v1, Vertex *v2 ) {
  float d = v1->DotProduct(v2);
  Vertex axis; axis.StoreCrossProduct(v1,v2);
  float qw = sqrtf(v1->length()*v2->length()) + d;
  if (qw < 0.0001f) { // vectors are 180 degrees apart
		 Set(0,-v1->z,v1->y,v1->x);
   Normalize(this);
  } 
	 Set(qw,axis.x,axis.y,axis.z); 
  Normalize(this);
 }

 Euler *toEuler() {
  Euler *e=new Euler;
  double exception= x*y + z*w;
  if ( exception== 0.5 ) {
   e->pitch = 2.0 * atan2(x,w);
   e->roll = 0.0;
  } else 
  if ( exception== -0.5 ) {
   e->pitch = -2 * atan2(x,w);
   e->roll = 0.0;
  } else {
   e->pitch= atan2(2.0*y*w-2.0*x*z , 1.0 - 2.0*SQ(y) - 2.0*SQ(z) );
   e->roll = atan2(2.0*x*w-2.0*y*z , 1.0 - 2.0*SQ(x) - 2.0*SQ(z) );
  }
  e->yaw  = asin(2.0*x*y + 2.0*z*w);
  return e;
 }

 void Inverse(Quaternion *source) { 
  w=-source->w; 
  x=source->x; 
  y=source->y; 
  z=source->z; 
 }

 void RotationQuaternion(Matrix16 *m16, Quaternion *quat)
 {
  double xx = quat->x*quat->x, yy = quat->y*quat->y, zz = quat->z*quat->z;
  double xy = quat->x*quat->y, xz = quat->x*quat->z, yz = quat->y*quat->z;
  double wx = quat->w*quat->x, wy = quat->w*quat->y, wz = quat->w*quat->z;

  m16->m[0][0] = (double) (1.0 - 2.0 * ( yy + zz )); 
  m16->m[0][1] = (double) (    2.0 * ( xy - wz ));
  m16->m[0][2] = (double) (    2.0 * ( xz + wy ));

  m16->m[1][0] = (double) (    2.0 * ( xy + wz ));
  m16->m[1][1] = (double) (1.0 - 2.0 * ( xx + zz ));
  m16->m[1][2] = (double) (    2.0 * ( yz - wx ));

  m16->m[2][0] = (double) (    2.0 * ( xz - wy ));
  m16->m[2][1] = (double) (    2.0 * ( yz + wx ));
  m16->m[2][2] = (double) (1.0 - 2.0 * ( xx + yy ));

  m16->m[0][3] = m16->m[1][3] = m16->m[2][3] = 0.0;
  m16->m[3][0] = m16->m[3][1] = m16->m[3][2] = 0.0;
  m16->m[3][3] = 1.0;
 }

 void RotateBy(Matrix16 *m16, Quaternion *quat)
 {
  Matrix16 *rmat=new Matrix16;
  Matrix16 *mat1=new Matrix16;
  quat->RotationQuaternion(rmat,quat);
  mat1->Multiply(m16, rmat);
  *m16 = *mat1;
  delete rmat;
  delete mat1;
 }

};

struct Transform
{
public:
 Vertex position;
 Quaternion orientation;
 Vertex scaling;
 Transform() {
  scaling.Set(1.0f,1.0f,1.0f);
 }
 void It() {
  glLoadIdentity();
  glTranslatef(position.x,position.y,position.z);
  glScalef(scaling.x,scaling.y,scaling.z);
  glRotatef( (float) orientation.w, (float) orientation.x, (float) orientation.y, (float) orientation.z );
 }
};