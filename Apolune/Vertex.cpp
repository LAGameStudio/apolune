
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

 /* Some parts written by Jerome Jacovella-St. Louis (c) 2009 and perhaps earlier.  I didn't have to
    put this notice here, but out of courtesy I did. */

/* Generic Vector Class (and associated Generic Matrix Class) courtesy 3DR3 (Black-and-White) */

#include "Vertex.h"

Vertex::Vertex( float xx, float yy ) {
 Constructor();
 x=xx;
 y=yy;
}

Vertex::Vertex( float xx, float yy, float zz ) {
 Constructor();
 x=xx;
 y=yy;
 z=zz;
}

Vertex::Vertex( float xx, float yy, float zz, float ww ) {
 Constructor();
 x=xx;
 y=yy;
 z=zz;
 w=ww;
}

Vertex::Vertex( double xx, double yy ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
}

Vertex::Vertex( double xx, double yy, double zz ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
 z=(float) zz;
}

Vertex::Vertex( int xx, int yy ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
}

Vertex::Vertex( int xx, int yy, int zz ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
 z=(float) zz;
}

Vertex::Vertex( double xx, double yy, double zz, double ww ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
 z=(float) zz;
 w=(float) ww;
}

Vertex::Vertex( int xx, int yy, int zz, int ww ) {
 Constructor();
 x=(float) xx;
 y=(float) yy;
 z=(float) zz;
 w=(float) ww;
}

Vertex::Vertex( Vertexd *v ) {
 Constructor();
 x=(float) v->x;
 y=(float) v->y;
 z=(float) v->z;
}

Vertex *Vertex::Copy() {
  Vertex *w=new Vertex;
  w->hc = hc;
  w->vc = vc;
  w->x=x;  w->y=y;  w->z=z;
  w->A=A;  w->B=B;  w->C=C;
  w->c=c;
  w->rgba[0] = rgba[0]; 
  w->rgba[1] = rgba[1]; 
  w->rgba[2] = rgba[2]; 
  w->rgba[3] = rgba[3];
  CopyAssociatedData();
  w->q=q;
  w->r=r;
  w->s=s;
  w->t=t;
//  w->u=u;  w->v=v;
  if ( normal ) w->normal->Duplicate(normal);//->Copy();
  if ( vector ) w->vector->Duplicate(vector);// = vector->Copy();
  if ( tangent ) w->tangent->Duplicate(tangent);// = tangent->Copy();
  w->R=R;
  w->flags=flags;
  return w;
 }


void Vertex::Duplicate( Vertex *dest ) {
 dest->hc = hc;
 dest->vc = vc;
 dest->x=x;  dest->y=y;  dest->z=z;
 dest->A=A;  dest->B=B;  dest->C=C;
 dest->c=c;
 dest->rgba[0] = rgba[0]; 
 dest->rgba[1] = rgba[1]; 
 dest->rgba[2] = rgba[2]; 
 dest->rgba[3] = rgba[3]; 
 dest->q=q;
 dest->r=r;
 dest->s=s;
 dest->t=t;
// dest->u=u;  dest->v=v;
// if ( normal ) dest->normal = normal->Copy();
// if ( vector ) dest->vector = vector->Copy();
 dest->R=R;
 dest->flags=flags;
}

void Vertex::Divide( double d ) {
 if ( d == 0 ) x=0.0f; else x/=(float)d;
 if ( d == 0 ) y=0.0f; else y/=(float)d;
 if ( d == 0 ) z=0.0f; else z/=(float)d;
}

void Vertex::Divide( float d ) {
 if ( d == 0.0f ) x=0.0f; else x/=d;
 if ( d == 0.0f ) y=0.0f; else y/=d;
 if ( d == 0.0f ) z=0.0f; else z/=d;
}

void Vertex::Divide( float dx, float dy, float dz ) {
 if ( dx == 0 ) x=0.0f; else x/=(float)dx;
 if ( dy == 0 ) y=0.0f; else y/=(float)dy;
 if ( dz == 0 ) z=0.0f; else z/=(float)dz;
}

void Vertex::Divide( Vertexd *divisor ) {
 if ( divisor->x == 0 ) x=0.0f; else x/=(float)divisor->x;
 if ( divisor->y == 0 ) y=0.0f; else y/=(float)divisor->y;
 if ( divisor->z == 0 ) z=0.0f; else z/=(float)divisor->z;
}

void Vertex::Set( Vertex *p ) {
 hc=p->hc;
 vc=p->vc;
 x=p->x; y=p->y; z=p->z;
 A=p->A; B=p->B; C=p->C;
 c=p->c;
 rgba[0]=p->rgba[0];
 rgba[1]=p->rgba[1];
 rgba[2]=p->rgba[2];
 rgba[3]=p->rgba[3];
 q=p->q;
 r=p->r;
 s=p->s;
 t=p->t;
 R=p->R;
 flags=p->flags;
 // clear normals? duplicate normals? tangents?
}


void Vertex::Set( const Vertex& p ) {
 hc=p.hc;
 vc=p.vc;
 x=p.x; y=p.y; z=p.z;
 A=p.A; B=p.B; C=p.C;
 c=p.c;
 rgba[0]=p.rgba[0];
 rgba[1]=p.rgba[1];
 rgba[2]=p.rgba[2];
 rgba[3]=p.rgba[3];
 q=p.q;
 r=p.r;
 s=p.s;
 t=p.t;
 R=p.R;
 flags=p.flags;
 // clear normals? duplicate normals? tangents?
}


Vertex *Vertex::Normal(void) {
 normal.Instantiate();
 normal->x = x/sqrtf( x*x );
 normal->y = y/sqrtf( y*y );
 normal->z = z/sqrtf( z*z );
 return normal.pointer;
}

// Given a face (a,b,this) calculate the face's normal and then the tangent vector and assign to this.
Vertex *Vertex::Tangent( Vertex *a, Vertex *b ) {
 tangent.Instantiate();
 tangent->x = x/sqrtf( x*x );
 tangent->y = y/sqrtf( y*y );
 tangent->z = z/sqrtf( z*z );
 return tangent.pointer;
}

// Given a face (a,b,this) calculate the face's normal and then the tangent vector and assign to this.
Vertex *Vertex::BiTangent( Vertex *a, Vertex *b ) {
 bitangent.Instantiate();
 bitangent->x = x/sqrtf( x*x );
 bitangent->y = y/sqrtf( y*y );
 bitangent->z = z/sqrtf( z*z );
 return bitangent.pointer;
}

void Vertex::TangentBasis( Vertex *a, Vertex *b ) {
		// Shortcuts for vertices
		glm::vec3 v0( x, y, z );
		glm::vec3 v1( a->x, a->y, a->z );
		glm::vec3 v2( b->x, b->y, b->z ); 
		// Shortcuts for UVs
		glm::vec2 uv0(q,r);
		glm::vec2 uv1(a->q,a->r);
		glm::vec2 uv2(b->q,b->r); 
		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1-v0;
		glm::vec3 deltaPos2 = v2-v0; 
		// UV delta
		glm::vec2 deltaUV1 = uv1-uv0;
		glm::vec2 deltaUV2 = uv2-uv0;
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 t  = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bt = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
  glm::vec3 n( normal->x, normal->y, normal->z );
//		t = glm::normalize(t - n * glm::dot(n, t));
//		if (glm::dot(glm::cross(n, t), bt) < 0.0f) {
//			t = t * -1.0f;
//		}
  tangent.Instantiate();   tangent->Set( t.x, t.y, t.z );
  bitangent.Instantiate();	bitangent->Set( bt.x, bt.y, bt.z );
 }

// Given a triangle, calculate the normal. (Windedness matters here.)
Vertex *Vertex::TriNormal( Vertex *a, Vertex *b, Vertex *c ) {
 Vertex *normal=new Vertex;
 Vertex U; U.Subtract(b,a);
 Vertex V; V.Subtract(c,a);
 normal->x=(U.y * V.z) - (U.z * V.y);
 normal->y=(U.z * V.x) - (U.x * V.z);
 normal->z=(U.x * V.y) - (U.y * V.x);
 normal->normalize();
 return normal;
}

// Given a triangle, calculate the normal. (Windedness matters here.)
void Vertex::TriNormal( Vertexd *a, Vertexd *b, Vertexd *c ) {
 Vertex A(a->x,a->y,a->z);
 Vertex B(b->x,b->y,b->z);
 Vertex C(c->x,c->y,c->z);
 Vertex U; U.Subtract(&B,&A);
 Vertex V; V.Subtract(&C,&A);
 x=(U.y * V.z) - (U.z * V.y);
 y=(U.z * V.x) - (U.x * V.z);
 z=(U.x * V.y) - (U.y * V.x);
 normalize();
}

// Given a triangle, calculate the normal. (Windedness matters here.)
void Vertex::SetTriNormal( Vertex *a, Vertex *b, Vertex *c ) {
 Vertex U; U.Subtract(b,a);
 Vertex V; V.Subtract(c,a);
 x=(U.y * V.z) - (U.z * V.y);
 y=(U.z * V.x) - (U.x * V.z);
 z=(U.x * V.y) - (U.y * V.x);
 normalize();
}

Vertex *Vertex::Normal( Vertex *a, Vertex *b, Vertex *c ) {
 Vertex *normal=new Vertex;
 Vertex A,B;
 A.Add(a);
 A.Subtract(b);
 B.Add(c);
 B.Subtract(a);
 normal=A.cross(&B);
 return normal;
}

void Vertex::MoveBy( float byx, float byy, float byz ) {
 x+=byx;
 y+=byy;
 z+=byz;
}

void Vertex::MoveBy( float byx, float byy, float byz, float byw ) {
 x+=byx;
 y+=byy;
 z+=byz;
 w+=byw;
}

void Vertex::Add(Vertex *vector1, Vertex *vector2)
{
 x = vector1->x + vector2->x;
 y = vector1->y + vector2->y;
 z = vector1->z + vector2->z;
}

void Vertex::Subtract(Vertex *vector1, Vertex *vector2)
{
 x = vector1->x - vector2->x;
 y = vector1->y - vector2->y;
 z = vector1->z - vector2->z;
}
   
void Vertex::Scale(Vertex *vector1, float s)
{
 x *= vector1->x * s;
 y *= vector1->y * s;
 z *= vector1->z * s;
}


void Vertex::MultMatrix(Vertex *source, Matrix16 *matrix)
{
 x = (float)(source->x * matrix->m[0][0] + source->y * matrix->m[1][0] + source->z * matrix->m[2][0] + matrix->m[3][0]);
 y = (float)(source->x * matrix->m[0][1] + source->y * matrix->m[1][1] + source->z * matrix->m[2][1] + matrix->m[3][1]);
 z = (float)(source->x * matrix->m[0][2] + source->y * matrix->m[1][2] + source->z * matrix->m[2][2] + matrix->m[3][2]);
}

void Vertex::DivideMatrix(Vertex *source, Matrix16 *matrix)
{
 /*
 solve(
 {
    vectorX=sourceX*m00+sourceY*m10+sourceZ*m20+m30,
    vectorY=sourceZ*m01+sourceY*m11+sourceZ*m21+m31,
    vectorZ=sourceX*m02+sourceY*m12+sourceZ*m22+m32
 }, { sourceX, sourceY, sourceZ });
 */

 float var1 = (float)(
    matrix->m[2][0] * matrix->m[0][2] * matrix->m[1][1]
  - matrix->m[0][2] * matrix->m[2][1] * matrix->m[1][0]
  - matrix->m[2][2] * matrix->m[0][0] * matrix->m[1][1]
  - matrix->m[0][2] * matrix->m[0][1] * matrix->m[1][0]
  + matrix->m[2][1] * matrix->m[0][0] * matrix->m[1][2]
  + matrix->m[0][1] * matrix->m[0][0] * matrix->m[1][2]);
 
 x = (float)(
    - matrix->m[2][2] * source->x * matrix->m[1][1]
    + matrix->m[2][2] * matrix->m[1][0] * source->y
    - matrix->m[2][2] * matrix->m[1][0] * matrix->m[3][1]
    + matrix->m[2][2] * matrix->m[3][0] * matrix->m[1][1] 
    - matrix->m[2][0] * matrix->m[3][2] * matrix->m[1][1]
    + source->x * matrix->m[2][1] * matrix->m[1][2]
    + source->x * matrix->m[0][1] * matrix->m[1][2]
    - matrix->m[1][0] * matrix->m[0][1] * source->z
    + matrix->m[1][0] * matrix->m[2][1] * matrix->m[3][2]
    + matrix->m[1][0] * matrix->m[0][1] * matrix->m[3][2]
    - matrix->m[1][0] * matrix->m[2][1] * source->z
    - matrix->m[3][0] * matrix->m[2][1] * matrix->m[1][2]
    - matrix->m[2][0] * matrix->m[1][2] * source->y
    + matrix->m[2][0] * matrix->m[1][2] * matrix->m[3][1]
    + matrix->m[2][0] * source->z * matrix->m[1][1]
    - matrix->m[3][0] * matrix->m[0][1] * matrix->m[1][2]
    ) / var1;
 
 y = - (float)(
    - matrix->m[2][0] * matrix->m[0][2] * source->y
    + matrix->m[2][1] * matrix->m[0][0] * matrix->m[3][2]
    + matrix->m[2][0] * matrix->m[0][2] * matrix->m[3][1]
    + matrix->m[0][1] * matrix->m[0][0] * matrix->m[3][2]
    - matrix->m[2][1] * matrix->m[0][0] * source->z
    - matrix->m[0][2] * matrix->m[2][1] * matrix->m[3][0]
    + matrix->m[0][2] * matrix->m[0][1] * source->x
    - matrix->m[0][2] * matrix->m[0][1] * matrix->m[3][0]
    + matrix->m[0][2] * matrix->m[2][1] * source->x
    + matrix->m[2][2] * matrix->m[0][0] * source->y
    - matrix->m[0][1] * matrix->m[0][0] * source->z
    - matrix->m[2][2] * matrix->m[0][0] * matrix->m[3][1]
    ) / var1;

 z = (float)(
    source->x * matrix->m[0][2] * matrix->m[1][1]
    + matrix->m[0][0] * matrix->m[3][2] * matrix->m[1][1]
    + matrix->m[0][0] * matrix->m[1][2] * source->y
    - matrix->m[0][0] * matrix->m[1][2] * matrix->m[3][1]
    - matrix->m[0][0] * source->z * matrix->m[1][1]
    - matrix->m[1][0] * matrix->m[0][2] * source->y
    + matrix->m[1][0] * matrix->m[0][2] * matrix->m[3][1]
    - matrix->m[3][0] * matrix->m[0][2] * matrix->m[1][1]
    ) / var1;
}

inline float Vertex::Magnitude (Vertex *V2)				// Calculate The Length Of The Vector
{
 return sqrtf (x * V2->x + y * V2->y + z * V2->z);	
}

Vertex *Vertex::Normalize (Vertex *V2)	{ // Creates A Vector With A Unit Length Of 1
 float M = Magnitude(V2); // Calculate The Length Of The Vector 
 Vertex *v= new Vertex;
 if (M != 0.0f)			// Make Sure We Don't Divide By 0 
 {
  v->x = x / M;		// Normalize The 3 Components 
  v->y = y / M;
  v->z = z / M;
 } else { 
  v->x=x; 
  v->y=y; 
  v->z=z; 
 }    
 return v;
}

void Vertex::normalize()	{ // Creates A Vector With A Unit Length Of 1
 float M = Magnitude(this); // Calculate The Length Of The Vector 
 if (M != 0.0f)			// Make Sure We Don't Divide By 0 
 {
  x /= M;		// Normalize The 3 Components 
  y /= M;
  z /= M;
 }    
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateXY( Vertex *basePoint, float rotationAngle ) 
{  
 float offsetX;  
 float offsetY;  
 float rotateX;  
 float rotateY;  
 float rotationRadians;  
 Vertex *retPoint = new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetX = (x -basePoint->x);  
 offsetY = (y -basePoint->y);  
 rotationRadians = deg2rad(rotationAngle);
 //calculate new x coord  
 rotateX = (float) (offsetX * cos(rotationAngle));  
 rotateY = (float) (offsetY * sin(rotationAngle));
 //convert new x and y back to decimal degrees
 rotateX = rad2deg(rotateX);
 rotateY = rad2deg(rotateY);
 //shift new x and y relative to base point  
 rotateX = (rotateX + basePoint->x);  
 rotateY = (rotateY + basePoint->y);
 //return new point  
 retPoint->x = rotateX;
 retPoint->y = rotateY;
 return retPoint;  
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateZY( Vertex *basePoint, float rotationAngle ) 
{  
 float offsetZ;  
 float offsetY;  
 float rotateZ;  
 float rotateY;  
 float rotationRadians;  
 Vertex *retPoint=new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetZ = (z -basePoint->z);  
 offsetY = (y -basePoint->y);  
 rotationRadians = deg2rad(rotationAngle);
 //calculate new x coord  
 rotateZ = (float) (offsetZ * cos(rotationAngle));  
 rotateY = (float) (offsetY * sin(rotationAngle));
 //convert new x and y back to decimal degrees  
 rotateZ = rad2deg(rotateZ);
 rotateY = rad2deg(rotateY);
 //shift new x and y relative to base point  
 rotateZ = (rotateZ + basePoint->z);  
 rotateY = (rotateY + basePoint->y);
 //return new point  
 retPoint->z = rotateZ;
 retPoint->y = rotateY;
 return retPoint;  
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateXZ( Vertex *basePoint, float rotationAngle ) 
{  
 float offsetX;  
 float offsetZ;  
 float rotateX;  
 float rotateZ;  
 float rotationRadians;  
 Vertex *retPoint=new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetX = (x -basePoint->x);  
 offsetZ = (z -basePoint->z);  
 rotationRadians = deg2rad(rotationAngle);
 //calculate new x coord  
 rotateX = (float) (offsetX * cos(rotationAngle));  
 rotateZ = (float) (offsetZ * sin(rotationAngle));
 //convert new x and y back to decimal degrees  
 rotateX = rad2deg(rotateX);
 rotateZ = rad2deg(rotateZ);
 //shift new x and y relative to base point  
 rotateX = (rotateX + basePoint->x);  
 rotateZ = (rotateZ + basePoint->z);
 //return new point  
 retPoint->x = rotateX;
 retPoint->z = rotateZ;
 return retPoint;  
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateXY( Vertex *basePoint, double rotationRadians ) 
{  
 float offsetX;  
 float offsetY;  
 float rotateX;  
 float rotateY;
 float degrees=rad2deg((float) rotationRadians);
 Vertex *retPoint = new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetX = (x -basePoint->x);  
 offsetY = (y -basePoint->y);  
 //calculate new x coord  
 rotateX = (float) (offsetX * cos(degrees));  
 rotateY = (float) (offsetY * sin(degrees));
 //convert new x and y back to decimal degrees  
 rotateX = rad2deg(rotateX);
 rotateY = rad2deg(rotateY);
 //shift new x and y relative to base point  
 rotateX = (rotateX + basePoint->x);  
 rotateY = (rotateY + basePoint->y);
 //return new point  
 retPoint->x = rotateX;
 retPoint->y = rotateY;
 return retPoint;  
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateZY( Vertex *basePoint, double rotationRadians ) 
{  
 float offsetZ;  
 float offsetY;  
 float rotateZ;  
 float rotateY;
 float degrees=rad2deg((float) rotationRadians);
 Vertex *retPoint=new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetZ = (z -basePoint->z);  
 offsetY = (y -basePoint->y);  
 //calculate new x coord  
 rotateZ = (float) (offsetZ * cos(degrees));  
 rotateY = (float) (offsetY * sin(degrees));
 //convert new x and y back to decimal degrees  
 rotateZ = rad2deg(rotateZ);
 rotateY = rad2deg(rotateY);
 //shift new x and y relative to base point  
 rotateZ = (rotateZ + basePoint->z);  
 rotateY = (rotateY + basePoint->y);
 //return new point  
 retPoint->z = rotateZ;
 retPoint->y = rotateY;
 return retPoint;  
}

// basePoint - the center of the rotation
// sourcePoint - the point to be rotated
// retPoint - the new point
Vertex *Vertex::rotateXZ( Vertex *basePoint, double rotationRadians ) 
{  
 float offsetX;
 float offsetZ;  
 float rotateX;  
 float rotateZ;
 float degrees=rad2deg((float) rotationRadians);
 Vertex *retPoint = new Vertex;  
 //shift x and y relative to 0,0 origin  
 offsetX = (x -basePoint->x);  
 offsetZ = (z -basePoint->z);  
 //calculate new x coord  
 rotateX = (float) (offsetX * cos(degrees));  
 rotateZ = (float) (offsetZ * sin(degrees));
 //convert new x and y back to decimal degrees  
 rotateX = rad2deg(rotateX);
 rotateZ = rad2deg(rotateZ);
 //shift new x and y relative to base point  
 rotateX = (rotateX + basePoint->x);  
 rotateZ = (rotateZ + basePoint->z);
 //return new point  
 retPoint->x = rotateX;
 retPoint->z = rotateZ;
 return retPoint;  
}

void Vertex::setPosition( Vertex *b ) {
 x=b->x;
 y=b->y;
 z=b->z;
}

float &Vertex::operator [] (const int index){
	return *(&x + index);
}

void Vertex::operator += (const Vertex &v){
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vertex::operator -= (const Vertex &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vertex::operator *= (const float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vertex::operator /= (const float dividend){
 x /= dividend;
 y /= dividend;
 z /= dividend;
}

bool operator == (const Vertex &v0, const Vertex &v1){
	return (v0.x == v1.x && v0.y == v1.y && v0.z == v1.z);
}

bool operator != (const Vertex &v0, const Vertex &v1){
	return (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z);
}

Vertex operator + (const Vertex &u, const Vertex &v){
	return Vertex(u.x + v.x,u.y + v.y,u.z + v.z);
}

Vertex operator - (const Vertex &u, const Vertex &v){
	return Vertex(u.x - v.x,u.y - v.y,u.z - v.z);
}

Vertex operator - (const Vertex &v){
	return Vertex(-v.x,-v.y,-v.z);
}

float operator * (const Vertex &u, const Vertex &v){
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vertex operator * (const float scalar, const Vertex &v){
	return Vertex(scalar * v.x,scalar * v.y,scalar * v.z);
}

Vertex operator * (const Vertex &v, const float scalar){
	return Vertex(scalar * v.x,scalar * v.y,scalar * v.z);
}

Vertex operator / (const Vertex &v, const float dividend){
	return Vertex(v.x / dividend,v.y / dividend,v.z / dividend);
}

Vertex operator % (const Vertex &V1, const Vertex &V2 ) {
 return Vertex(
  V1.y * V2.z - V1.z * V2.y,
  V1.z * V2.x - V1.x * V2.z,
  V1.x * V2.y - V1.y * V2.x
 );
}

Vertex Vertex::fastNormalize() {
 Vertex v=*this;
	float invLen = rsqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return v * invLen;
}

// Vertices
#include "Polygon.h"
#include "Trigger.h"

bool Vertices::within( double x, double y ) {
 mPolygon poly;
 poly.points.first=first;
 poly.points.last=last;
 poly.points.count=count;
 bool inPoly=poly.pointInPolygon((float)x,(float)y);
 poly.points.first=null;
 poly.points.last=null;
 poly.points.count=0;
 return inPoly;
}

// Queries a position returning the two vertices of the line and the closest point on the line,
// assumes the vertices are describe a line segment list from first to last (LINELOOP style),
// and returns false when a point cannot be determined (does not write out in this case).
bool Vertices::PointLineQuery( double x, double y, double z, Vertex **outA, Vertex **outB, Vertex *out, double *outDist ) {
 if ( count < 2 ) return false;
 Vertex *a,*b,*oA=null,*oB=null;
 // Let's start by finding the nearest point and labelling it A.  Note that A and B are returned in the
 // same order as they fall in the list.
 a=this->closest((float)x,(float)y,(float)z);
 if ( !a ) return false;
 // Now let's find point B by testing the point before or after this point (whichever is closest) in the list. 
//if ( !a->prev && !a->next ) return false; // should never happen
 if ( a->prev && !a->next ) { // End point
  oA=b=(Vertex *) a->prev;
  oB=(Vertex *) a;
 } else if ( a->next && !a->prev ) { // Start point
  oA=(Vertex *) a;
  oB=b=(Vertex *) a->next;
 } else { // Inside point (whichever is closest)
  Vertex *P1=(Vertex *) a->prev;
  Vertex *P2=(Vertex *) a->next;
  float dist1=fdistance(P1->x,P1->y,P1->z,(float)x,(float)y,(float)z);
  float dist2=fdistance(P2->x,P2->y,P2->z,(float)x,(float)y,(float)z);
  if ( dist1 < dist2 ) {
  oA=b=(Vertex *) a->prev;
  oB=(Vertex *) a;
  } else {
  oA=(Vertex *) a;
  oB=b=(Vertex *) a->next;
  }
 }
// if ( !oA && !oB ) return false;
// if ( !b ) return false; // should never happen
 // Now that we have a line segment A->B, compute the vertex using Line
 Line line;
 Vertexd intersection;
 line.Set(a->x,a->y,a->z,b->x,b->y,b->z);
 bool result=line.DistancePointLine(x,y,z,&intersection,outDist);
 if ( !result ) return false;
 out->Set(intersection.x,intersection.y,intersection.z);
 *outA=oA;
 *outB=oB;
 return true;
}

void Vertices::GetPoint( Vertex *out, double time ) {
 if ( count == 1 ) {
  Vertex *A=(Vertex *)first;
  out->x=A->x;
  out->y=A->y;
  out->z=A->z;
  return;
 }
 if ( count < 1 ) return;
 double s=abs(time)*(double)count;
 int a=(int)s;
 int b=a+1;
 Vertex *A=(Vertex *)Element(a);
 Vertex *B;
 if ( b == count ) out->Set(A);
 else if ( b < count ) {
  double distance_upto=0.0;
  double total_distance=0.0;
  bool hit_A=false;
  FOREACH(Vertex,v) {
   Vertex *n=(Vertex *)v->next;
   if ( n ) { 
    double d=(double)fdistance(v->x,v->y,v->z,n->x,n->y,n->z);
    total_distance+=d;
    if ( !hit_A ) distance_upto+=d;
    if ( n==A ) hit_A=true;
   }
  }
  double total_distance_time=total_distance*time;
  B=(Vertex *)Element(b);
  Line line;
  line.Set(A->x,A->y,A->z,B->x,B->y,B->z);
  Vertexd v;
  double segment_distance=(double)fdistance(A->x,A->y,A->z,B->x,B->y,B->z);
  line.Point(segment_distance/(total_distance_time-distance_upto),&v);
  out->x=(float)v.x;
  out->y=(float)v.y;
  out->z=(float)v.z;
 }
}

#include "Tweens.h"
void Vertices::GetPoint( Vertex *out, double time, TweenHandles *tween ) {
 if ( tween->count != count ) {
  GetPoint(out,time);
  return;
 }
 if ( count == 1 ) {
  Vertex *A=(Vertex *)first;
  out->x=A->x;
  out->y=A->y;
  out->z=A->z;
  return;
 }
 if ( count < 1 ) return;
 double s=abs(time)*(double)count;
 int a=(int)s;
 int b=a+1;
 Vertex *A=(Vertex *)Element(a);
 Tween *t=((TweenHandle *)tween->Element(a))->p;
 Vertex *B;
 if ( b == count ) out->Set(A);
 else if ( b < count ) {
  double distance_upto=0.0;
  double total_distance=0.0;
  bool hit_A=false;
  FOREACH(Vertex,v) {
   Vertex *n=(Vertex *)v->next;
   if ( n ) { 
    double d=(double)fdistance(v->x,v->y,v->z,n->x,n->y,n->z);
    total_distance+=d;
    if ( !hit_A ) distance_upto+=d;
    if ( n==A ) hit_A=true;
   }
  }
  double total_distance_time=total_distance*time;
  B=(Vertex *)Element(b);
  Line line;
  line.Set(A->x,A->y,A->z,B->x,B->y,B->z);
  Vertexd v;
  double segment_distance=(double)fdistance(A->x,A->y,A->z,B->x,B->y,B->z);
  line.Point(t->tweend(segment_distance/(total_distance_time-distance_upto)),&v);
  out->x=(float)v.x;
  out->y=(float)v.y;
  out->z=(float)v.z;
 }
}

// Returns a set of circles treating X as a ratio of the radius
void Vertices::toCirclesX( Circles *out, float unitRadius ) {
 FOREACH(Vertex,v) out->Add(v->x*unitRadius);
}

// Returns a set of circles treating Y as a ratio of the radius
void Vertices::toCirclesY( Circles *out, float unitRadius ) {
 FOREACH(Vertex,v) out->Add(v->y*unitRadius);
}

// Returns a set of circles treating Z as a ratio of the radius
void Vertices::toCirclesZ( Circles *out, float unitRadius ) {
 FOREACH(Vertex,v) out->Add(v->z*unitRadius);
}

// VertexList

// The averaged center of N points 
Vertex *VertexList::barycenter(void) {
 Vertex *r=new Vertex;  // result
 VertexHandle *vL;
 int rolling_total=0;
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) { 
  rolling_total++;
  r->x+= vL->p->x;
  r->y+= vL->p->y;
  r->z+= vL->p->z; 
 }
 if ( rolling_total > 0 ) { r->x/=rolling_total; r->y/=rolling_total; r->z/=rolling_total; }
 return r;
}

 // Find the "least" point on a certain view plane
Vertex *VertexList::leastPoint(void) {
 VertexHandle *vL=(VertexHandle *) first;
 Vertex *r = new Vertex; // result;
 if ( vL && vL->p ) { r->x=vL->p->x; r->y=vL->p->y; r->z=vL->p->z; }
 for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) { 
  if ( vL->p->x < r->x ) r->x=vL->p->x;
  if ( vL->p->y < r->y ) r->y=vL->p->y;
  if ( vL->p->z < r->z ) r->y=vL->p->z; 
 }
 return r;
}

void VertexList::move( float x, float y, float z ) { 
 VertexHandle *vL; 
 for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) /*if ( vL.v )*/ { 
  vL->p->x+=x; 
  vL->p->y+=y; 
  vL->p->z+=z; 
 } 
}

void VertexList::rotateXY( float angleDeg ) {
 VertexHandle *vL;
 Vertex *center=barycenter();
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->rotateXY( center, angleDeg );
 delete center;
}

void VertexList::rotateZY( float angleDeg ) {
 VertexHandle *vL;
 Vertex *center=barycenter();
 center->x = center->z;
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) {
  Vertex *w = new Vertex;
  Vertex *rotated;
  w->x=vL->p->z; 
  w->y=vL->p->y;
  rotated=w->rotateZY( center, angleDeg );
  vL->p->z=rotated->x;
  vL->p->y=rotated->y;
  delete rotated;
  delete w;
 }   
 delete center;
}

void VertexList::Push( Vertex *v ) { 
 Prepend(new VertexHandle(v));
}

VertexHandle *VertexList::find( Vertex *v ) {
 VertexHandle *vL;
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) if ( vL->p == v ) return vL;
 return null;
}

/*
void VertexList::Postpend( VertexList *appendix ) {
 VertexHandle *vL, *n; 
 for ( vL=(VertexHandle *) appendix->first; vL; vL=n ) {
  n= (VertexHandle *) vL->next;
  Push(vL->v);
 }
}*/

// delete from list, p's vertex list, not p
void VertexList::deleteVertex( Vertex *p ) {
 VertexHandle *curr, *n;
 for ( curr=(VertexHandle *) first; curr; curr=n ) {
  n=(VertexHandle *) (curr->next); 
  if ( curr->p == p ) {
   Remove(curr);
   delete curr;
  }
 }
}

bool VertexList::inList( Vertex *v ) { 
 VertexHandle *vL;
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) if ( vL->p == v ) return true; 
 return false; 
}

void VertexList::addIfUnique( Vertex *v ) { 
 if ( inList(v) ) return; 
 Push(v);
}

/*
 * removes if already a part of the list
 */
void VertexList::toggleVertex( Vertex *v ) {
 VertexHandle *vL;
 if ( (vL=find(v)) ) {
  Remove( vL );
  delete vL;
 }
 else Push(v);
}

// void freeListandVertices() { VERTEX_LIST_ITEM vL; for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) delete vL.v; Free(); }

VertexList *VertexList::copy(void) { 
 VertexList *new_list=new VertexList; 
 VertexHandle *vL; 
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) new_list->addIfUnique(vL->p);
 return new_list;
}

VertexList *VertexList::Duplicate(void) { 
 VertexList *new_list=new VertexList;
 VertexHandle *vL;
 for ( vL=(VertexHandle *)first; vL; vL=(VertexHandle *) (vL->next) ) new_list->Push(vL->p); 
 return new_list; 
}

void VertexList::setEqual( VertexList *b ) { 
 VertexHandle *vL,*wL; 
 for ( vL=(VertexHandle *) b->first; vL; vL=(VertexHandle *) (vL->next) ) 
  for ( wL=(VertexHandle *) first; wL; wL=(VertexHandle *) (wL->next) )
   wL->p->setPosition(vL->p); 
}

 Vertex *Vertex::calcTangentVector(Vertexd *v1, Vertexd *v2, Vertexd *v3, Vertexd *normal) {
  Vertex a(v1->x,v1->y,v1->z);
  Vertex b(v2->x,v2->y,v2->z);
  Vertex c(v3->x,v3->y,v3->z);
  Vertex n(normal->x,normal->y,normal->z);
  return calcTangentVector(&a,&b,&c,&n);
 }

 Vertex *Vertex::calcTangentVector(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *normal) {
  Vertex *tangent=new Vertex;

 	// Given the 3 vertices (position and texture coordinates) of a triangle
 	// calculate and return the triangle's tangent vector.
 
 	// Create 2 vectors in object space.
 	//
 	// edge1 is the vector from vertex positions pos1 to pos2.
 	// edge2 is the vector from vertex positions pos1 to pos3.
 	Vertex edge1(v2->x - v1->x, v2->y - v1->y, v2->z - v1->z);
 	Vertex edge2(v3->x - v1->x, v3->y - v1->y, v3->z - v1->z);
 
 	edge1.normalize();
 	edge2.normalize();
 
 	// Create 2 vectors in tangent (texture) space that point in the same
 	// direction as edge1 and edge2 (in object space).
 	//
 	// texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
 	// texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
 	Cartesian texEdge1(v2->r - v1->q, v2->r - v1->q);
 	Cartesian texEdge2(v3->r - v1->q, v3->r - v1->q);
 
 	texEdge1.normalizef();
 	texEdge2.normalizef();
 
 	// These 2 sets of vectors form the following system of equations:
 	//
 	//  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
 	//  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
 	//
 	// Using matrix notation this system looks like:
 	//
 	//  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
 	//  [       ]  =  [                        ]  [           ]
 	//  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
 	//
 	// The solution is:
 	//
 	//  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
 	//  [           ]  =  -------  [                         ]  [       ]
 	//  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
 	//
 	//  where:
 	//        [ texEdge1.x  texEdge1.y ]
 	//    A = [                        ]
 	//        [ texEdge2.x  texEdge2.y ]
 	//
 	//    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
 	//
 	// From this solution the tangent space basis vectors are:
 	//
 	//    tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
 	//  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
 	//     normal = cross(tangent, bitangent)
 
 	Vertex b;
 
 	float det = (texEdge1.fx * texEdge2.fy) - (texEdge1.fy * texEdge2.fx);
 
 	if (dhpowareMath::closeEnough(det, 0.0f))
 	{
 		tangent->Set(1.0f, 0.0f, 0.0f);
 		b.Set(0.0f, 1.0f, 0.0f);
 	}
 	else
 	{
 		det = 1.0f / det;
 
 		tangent->x = (texEdge2.y * edge1.x - texEdge1.y * edge2.x) * det;
 		tangent->y = (texEdge2.y * edge1.y - texEdge1.y * edge2.y) * det;
 		tangent->z = (texEdge2.y * edge1.z - texEdge1.y * edge2.z) * det;
 
 		b.x = (-texEdge2.x * edge1.x + texEdge1.x * edge2.x) * det;
 		b.y = (-texEdge2.x * edge1.y + texEdge1.x * edge2.y) * det;
 		b.z = (-texEdge2.x * edge1.z + texEdge1.x * edge2.z) * det;
 
 		tangent->normalize();
 		b.normalize();
 	}
 
 	// Calculate the handedness of the local tangent space.
 	// The bitangent vector is the cross product between the triangle face
 	// normal vector and the calculated tangent vector. The resulting bitangent
 	// vector should be the same as the bitangent vector calculated from the
 	// set of linear equations above. If they point in different directions
 	// then we need to invert the cross product calculated bitangent vector. We
 	// store this scalar multiplier in the tangent vector's 'w' component so
 	// that the correct bitangent vector can be generated in the normal mapping
 	// shader's vertex shader.
 
 	Vertex bitangent; bitangent.StoreCrossProduct(normal, tangent);
 	tangent->w = (bitangent.dot(b) < 0.0f) ? -1.0f : 1.0f;

  return tangent;
 }

 Vertex origin;

 // Generic Vector Class

 //
// vector.cpp
//
//
//

// vars

// functions

Vector Vector::operator +(Vector &v)	// return VectorAdd(this, v)
{
	Vector temp(x+v.x, y+v.y, z+v.z);
	return temp;
}
void Vector::operator +=(Vector &v)		// this = VectorAdd(this, v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector Vector::operator -(Vector &v)	// return VectorSub(this, v)
{
	Vector temp(x-v.x, y-v.y, z-v.z);

	return temp;
}
void Vector::operator -=(Vector &v)		// this = VectorSub(this, v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector Vector::operator *(Matrix &m)	// return MatrixMulVector(this, m)
{
	return (m * (*this));
}
Vector Vector::operator *(Vector &v)	// return VectorScale(this, v)
{
	Vector temp(x*v.x, y*v.y, z*v.z);
	return temp;
}
Vector Vector::operator *(float f)		// return VectorScale(this, f)
{
	Vector temp(x*f, y*f, z*f);
	return temp;
}
void Vector::operator *=(Matrix &m)		// this = MatrixMulVector(this, m)
{
	*this = *this * m;
}
void Vector::operator *=(Vector &v)		// this = VectorScale(this, v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}
void Vector::operator *=(float f)		// this = VectorScale(this, v)
{
	x *= f;
	y *= f;
	z *= f;
}

Vector Vector::operator ^(Vector &v)	// return VectorCross(this, v)
{
	Vector temp;

	temp.x = y*v.z - z*v.y;
	temp.y = z*v.x - x*v.z;
	temp.z = x*v.y - y*v.x;

	return temp;
}
void Vector::operator ^=(Vector &v)		// this = VectorCross(this, v)
{
	Set(y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x);
}

Vector Vector::operator !()
{
	Vector temp(-x,-y,-z);
	return temp;
}

void Vector::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float Vector::Dot(Vector &v)
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector::SelfDot()
{
	return x*x + y*y + z*z;
}

Vector Vector::Normalize()
{
	Vector	temp;

	temp = *this * (1.f / sqrtf(SelfDot()));

	return temp;
}


void Vector::SelfNormalize()
{
	*this *= 1.f / sqrtf(SelfDot());
}

float Vector::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector::Distance(Vector &v)
{
	return (*this - v).Length();
}

Vector Vector::Lerp(Vector &v, float f)
{
	Vector temp;

	temp = (*this * f) + (v * (1.f-f));

	return temp;
}

void Vector::SelfLerp(Vector &v, float f)
{
	*this = (*this * f) + (v * (1.f-f));
}







///////////////

void vmake(Vector *a, float x, float y, float z)
{
	a->x = x;
	a->y = y;
	a->z = z;
}

//---

void vadd(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x+b->x;
	c->y = a->y+b->y;
	c->z = a->z+b->z;
}

void vselfadd(Vector *a, Vector *b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

//---

void vsub(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x-b->x;
	c->y = a->y-b->y;
	c->z = a->z-b->z;
}

void vselfsub(Vector *a, Vector *b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
}

//---

void vmul(Vector *a, Vector *b, Vector *c)
{
	c->x = a->x*b->x;
	c->y = a->y*b->y;
	c->z = a->z*b->z;
}

void vselfmul(Vector *a, Vector *b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
}

//---

void vscale(Vector *a, float d, Vector *c)
{
	c->x = a->x * d;
	c->y = a->y * d;
	c->z = a->z * d;
}

void vselfscale(Vector *a, float d)
{
	a->x *= d;
	a->y *= d;
	a->z *= d;
}

//---

void vneg(Vector *a, Vector *c)
{
	c->x = -a->x;
	c->y = -a->y;
	c->z = -a->z;
}

void vselfneg(Vector *a)
{
	a->x *= -1.f;
	a->y *= -1.f;
	a->z *= -1.f;
}

//---

float vdot(Vector *a, Vector *b)
{
	return( a->x*b->x + a->y*b->y + a->z*b->z );
}

float vselfdot(Vector *a)
{
	return ( SQ(a->x) + SQ(a->y) + SQ(a->z) );
}

//---

void vcross(Vector *a, Vector *b, Vector *c)
{
	c->x = a->y*b->z - a->z*b->y;
	c->y = a->z*b->x - a->x*b->z;
	c->z = a->x*b->y - a->y*b->x;
}

//---

void vnorm(Vector *a)
{
	float Length, C;

	Length = SQ(a->x) + SQ(a->y) + SQ(a->z);

	if (Length != 0.f)
	{
		C = 1.f / (float)sqrt(Length);
		a->x *= C;
		a->y *= C;
		a->z *= C;
	}
}

//---

float vlen(Vector *a)
{
	return (float)sqrt( SQ(a->x) + SQ(a->y) + SQ(a->z) );
}

//---

float vdist(Vector *a, Vector *b)
{
	float x;
	
	x = SQ(b->x-a->x) + SQ(b->y-a->y) + SQ(b->z-a->z);

	if (x != 0.f)
		return (float)sqrt( x );
	else return 0.f;
}

//---

void vselflerp(Vector *a, Vector *b, float f)
{
	float	t;
	
	t = 1.f-f;

	a->x = a->x*f + b->x*t;
	a->y = a->y*f + b->y*t;
	a->z = a->z*f + b->z*t;
}

void vlerp(Vector *a, Vector *b, float f, Vector *c)
{
	float	t;
	
	t = 1.f-f;

	c->x = a->x*f + b->x*t;
	c->y = a->y*f + b->y*t;
	c->z = a->z*f + b->z*t;
}

Vertex vertexZero;