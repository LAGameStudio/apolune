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
#include "macros.h"
#include "dhpowareMath.h"

class Euler
{
public:
 double yaw, pitch, roll; // aka theta/phi/alpha aka x/y/z
 SKEYSTART
  SKEY("yaw",  {SPARAM(yaw,double,atof);})
  SKEY("pitch",{SPARAM(pitch,double,atof);})
  SKEY("roll", {SPARAM(roll,double,atof);})
 SKEYEND("Euler");
 string toString() {
  FORMAT(buf,80,"yaw %1.5f pitch %1.5f roll %1.5f", (float) yaw, (float) pitch, (float) roll );
  return string(buf);
 }
 Euler(void) {
  yaw=pitch=roll=0.0f;
 }

 Euler( double y, double p, double r ) {
  yaw=y;
  pitch=p;
  roll=r;
 }

 void Set( double y, double p, double r ) {
  yaw=y;
  pitch=p;
  roll=r;
 }

 void Set( float y, float p, float r ) {
  yaw=(double) y;
  pitch=(double) p;
  roll=(double) r;
 }

 void Add(Euler *e1, Euler *e2)
 {
    yaw   = e1->yaw   + e2->yaw;
    pitch = e1->pitch + e2->pitch;
    roll  = e1->roll  + e2->roll;
 }

 void Sub(Euler *e1, Euler *e2)
 {
    yaw   = e1->yaw   - e2->yaw;
    pitch = e1->pitch - e2->pitch;
    roll  = e1->roll  - e2->roll;
 }

 void ofLine( void *s, void *e );
};


class Matrix16
{
public:
 double a[16];
 double m[4][4];
 double b[16];
 SKEYSTART
  SKEY("a", {
   SPARAM(a[0],double,atof);   SPARAM(a[1],double,atof);   SPARAM(a[2],double,atof);   SPARAM(a[3],double,atof);
   SPARAM(a[4],double,atof);   SPARAM(a[5],double,atof);   SPARAM(a[6],double,atof);   SPARAM(a[7],double,atof);
   SPARAM(a[8],double,atof);   SPARAM(a[9],double,atof);   SPARAM(a[10],double,atof);  SPARAM(a[11],double,atof);
   SPARAM(a[12],double,atof);  SPARAM(a[13],double,atof);  SPARAM(a[14],double,atof);  SPARAM(a[15],double,atof);
  });
  SKEY("m", {
   SPARAM(m[0][0],double,atof);   SPARAM(m[0][1],double,atof);   SPARAM(m[0][2],double,atof);   SPARAM(m[0][3],double,atof);
   SPARAM(m[1][0],double,atof);   SPARAM(m[1][1],double,atof);   SPARAM(m[1][2],double,atof);   SPARAM(m[1][3],double,atof);
   SPARAM(m[2][0],double,atof);   SPARAM(m[2][1],double,atof);   SPARAM(m[2][2],double,atof);   SPARAM(m[2][3],double,atof);
   SPARAM(m[3][0],double,atof);   SPARAM(m[3][1],double,atof);   SPARAM(m[3][2],double,atof);   SPARAM(m[3][3],double,atof);
  });
  SKEY("b", {
   SPARAM(b[0],double,atof);   SPARAM(b[1],double,atof);   SPARAM(b[2],double,atof);   SPARAM(b[3],double,atof);
   SPARAM(b[4],double,atof);   SPARAM(b[5],double,atof);   SPARAM(b[6],double,atof);   SPARAM(b[7],double,atof);
   SPARAM(b[8],double,atof);   SPARAM(b[9],double,atof);   SPARAM(b[10],double,atof);  SPARAM(b[11],double,atof);
   SPARAM(b[12],double,atof);  SPARAM(b[13],double,atof);  SPARAM(b[14],double,atof);  SPARAM(b[15],double,atof);
  });
 SKEYEND("Matrix16");
 string toString() {
  incdent();
  string out=string("");
  decdent();
  FORMAT(buf,1024,"a %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f ",
   (float) a[0],  (float) a[1],  (float) a[2],  (float) a[3],
   (float) a[4],  (float) a[5],  (float) a[6],  (float) a[7],
   (float) a[8],  (float) a[9],  (float) a[10], (float) a[11],
   (float) a[12], (float) a[13], (float) a[14], (float) a[15]
  );
  out+=indention+string(buf)+string("\n");
  FORMAT(buf,1024,"m %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f ",
   (float) m[0][0], (float) m[0][1], (float) m[0][2], (float) m[0][3],
   (float) m[1][0], (float) m[1][1], (float) m[1][2], (float) m[1][3],
   (float) m[2][0], (float) m[2][1], (float) m[2][2], (float) m[2][3],
   (float) m[3][0], (float) m[3][1], (float) m[3][2], (float) m[3][3]
  );
  out+=indention+string(buf)+string("\n");
  FORMAT(buf,1024,"b %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f %1.5f ",
   (float) b[0],  (float) b[1],  (float) b[2],  (float) b[3],
   (float) b[4],  (float) b[5],  (float) b[6],  (float) b[7],
   (float) b[8],  (float) b[9],  (float) b[10], (float) b[11],
   (float) b[12], (float) b[13], (float) b[14], (float) b[15]
  );
  out+=indention+string(buf)+string("\n");
  return out;
 }
 void Identity()
 {
  memset(a,0,sizeof(double)*16);
  memset(b,0,sizeof(double)*16);
  memset(m,0,sizeof(double)*4*4);
  m[0][0]=m[1][1]=m[2][2]=m[3][3]=1;
 }

 Matrix16(void) { Identity(); }
 ~Matrix16(void) {}

 void Transpose(Matrix16 *source)
 {
  int i,j;
  for(i=0; i<4; i++)
  for(j=0; j<4; j++)
  m[j][i] = source->m[i][j];
 }

 Matrix4 *asMatrix4() {
  return new Matrix4( (float) m[0][0], (float) m[0][1], (float)  m[0][2], (float) m[0][3],
                      (float) m[1][0], (float) m[1][1], (float)  m[1][2], (float) m[1][3],
                      (float) m[2][0], (float) m[2][1], (float)  m[2][2], (float) m[2][3],
                      (float) m[3][0], (float) m[3][1], (float)  m[3][2], (float) m[3][3] );
 }

 void Multiply(Matrix16 *A, Matrix16 *B) {
  m[0][0]=A->m[0][0]*B->m[0][0] + A->m[0][1]*B->m[1][0] + A->m[0][2]*B->m[2][0];
  m[0][1]=A->m[0][0]*B->m[0][1] + A->m[0][1]*B->m[1][1] + A->m[0][2]*B->m[2][1];
  m[0][2]=A->m[0][0]*B->m[0][2] + A->m[0][1]*B->m[1][2] + A->m[0][2]*B->m[2][2];
  m[0][3]=0;

  m[1][0]=A->m[1][0]*B->m[0][0] + A->m[1][1]*B->m[1][0] + A->m[1][2]*B->m[2][0];
  m[1][1]=A->m[1][0]*B->m[0][1] + A->m[1][1]*B->m[1][1] + A->m[1][2]*B->m[2][1];
  m[1][2]=A->m[1][0]*B->m[0][2] + A->m[1][1]*B->m[1][2] + A->m[1][2]*B->m[2][2];
  m[1][3]=0;
     
  m[2][0]=A->m[2][0]*B->m[0][0] + A->m[2][1]*B->m[1][0] + A->m[2][2]*B->m[2][0];
  m[2][1]=A->m[2][0]*B->m[0][1] + A->m[2][1]*B->m[1][1] + A->m[2][2]*B->m[2][1];
  m[2][2]=A->m[2][0]*B->m[0][2] + A->m[2][1]*B->m[1][2] + A->m[2][2]*B->m[2][2];
  m[2][3]=0;
     
  m[3][0]=A->m[3][0]*B->m[0][0] + A->m[3][1]*B->m[1][0] + A->m[3][2]*B->m[2][0] + B->m[3][0];
  m[3][1]=A->m[3][0]*B->m[0][1] + A->m[3][1]*B->m[1][1] + A->m[3][2]*B->m[2][1] + B->m[3][1];
  m[3][2]=A->m[3][0]*B->m[0][2] + A->m[3][1]*B->m[1][2] + A->m[3][2]*B->m[2][2] + B->m[3][2];
  m[3][3]=1;
 }

 void Translate(double tx, double ty, double tz)
 {
  Matrix16 *tmat=new Matrix16;
  Matrix16 *mat1=new Matrix16;
  memset(tmat,0,sizeof(Matrix16));
  tmat->m[0][0]=tmat->m[1][1]=tmat->m[2][2]=tmat->m[3][3]=1;
  tmat->m[3][0]=tx; tmat->m[3][1]=ty; tmat->m[3][2]=tz;
  mat1->Multiply(this, tmat);
  *this = *mat1;
  delete tmat;
  delete mat1;
 }

 void Scale(double sx, double sy, double sz)
 {
  Matrix16 *smat=new Matrix16;
  Matrix16 *mat1=new Matrix16;
  memset(&smat,0,sizeof(Matrix16));
  smat->m[0][0]=sx; smat->m[1][1]=sy; smat->m[2][2]=sz; smat->m[3][3]=1;
  mat1->Multiply(this, smat);
  *this = *mat1;
  delete smat;
  delete mat1;
 }

 double Determinant(void)
 {
  double result = 0, i = 1;
  int n;
  for(n = 0; n < 4; n++, i *= -1)
  {
   double det;
   double msub3[3][3];
   int di, dj;

   for(di = 0; di < 3; di++) for(dj = 0; dj < 3; dj++) 
    {
     int si = di + ( ( di >= 0 ) ? 1 : 0 );
     int sj = dj + ( ( dj >= n ) ? 1 : 0 );
     msub3[di][dj] = m[si][sj];
    }
   det = msub3[0][0] * ( msub3[1][1]*msub3[2][2] - msub3[2][1]*msub3[1][2] )
       - msub3[0][1] * ( msub3[1][0]*msub3[2][2] - msub3[2][0]*msub3[1][2] )
       + msub3[0][2] * ( msub3[1][0]*msub3[2][1] - msub3[2][0]*msub3[1][1] );
   result += m[0][n] * det * i;
  }
  return result;
 }

 void Inverse(Matrix16 *source)
 {
  double det = source->Determinant();
  // if(Abs(det) < 0.0005)
  if(abs(det) < 0.00000001) Identity();
  else {
   int i, j, sign;   
   for ( i = 0; i < 4; i++ ) for ( j = 0; j < 4; j++ ) {
    int di, dj;
    double msub3[3][3];
    double m3det;
    sign = 1 - ( (i+j) % 2 ) * 2;
    for(di = 0; di < 3; di++) for(dj = 0; dj < 3; dj++) {
      int si = di + ( ( di >= i ) ? 1 : 0 );
      int sj = dj + ( ( dj >= j ) ? 1 : 0 );
      msub3[di][dj] = source->m[si][sj];
     }
    m3det = msub3[0][0] * ( msub3[1][1]*msub3[2][2] - msub3[2][1]*msub3[1][2] )
          - msub3[0][1] * ( msub3[1][0]*msub3[2][2] - msub3[2][0]*msub3[1][2] )
          + msub3[0][2] * ( msub3[1][0]*msub3[2][1] - msub3[2][0]*msub3[1][1] );
    m[j][i] = m3det * sign / det;
   }
  }
 }

 void ToEuler(Euler *euler)
 {
  if(fabs(m[2][1]) <= 1.0 - 0.000005)
  {
   euler->yaw   = atan2(-m[2][0], m[2][2]);
   euler->pitch = asin ( m[2][1]);
   euler->roll  = atan2(-m[0][1], m[1][1]);
  }
  else
  {
   euler->yaw = acos(m[0][0]);
   euler->pitch = PI/2;
   euler->roll = 0;
  }
 }

/*
  property Quaternion
   {
      set { RotationQuaternion(value); }
      get { value->RotationMatrix(this); }
   }
 */
};