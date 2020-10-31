#pragma once

#include <Windows.h>
#include <math.h>
#include "LinkedList.h"
#include "ListItem.h"

class PhysVector3D : public LinkedList
{
public:
	float x;
	float y;
	float z;

 PhysVector3D();
 ~PhysVector3D();

	bool isZero()	{	return x == 0.0f && y == 0.0f && z == 0.0f;	}

 void Set( float X, float Y, float Z ) { x = X; y = Y; z = Z; }
	void Copy( PhysVector3D p )				       { x = p.x; y = p.y; z = p.z; }
		
	void setX( float X )	{ x = X; }
	void setY( float Y )	{ y = Y; }
	void setZ( float Z ) { z = Z; }
			
	void add( PhysVector3D p )      { x += p.x; y += p.y; z += p.z; }
	void subtract( PhysVector3D p )	{ x -= p.x; y -= p.y; z -= p.z; }
	  
	void Add( float a, float b, float c )   		 { x += a; y += b; z += c; } 
	void Subtract( float a, float b, float c ) { x -= a; y -= b; z -= c; } 
		  
	PhysVector3D multiplyBy( float f )			     { x *= f; y *= f; z*= f; return *this; }
	
	float distanceTo( PhysVector3D p )     		 { return (float)sqrt( distanceSquaredTo( p ) ); }
	float distanceSquaredTo( PhysVector3D p ) { float dx = (float) (x-p.x); float dy = (float) (y-p.y); float dz = (float) (z-p.z); return (float) (dx*dx + dy*dy + dz*dz); }
	
	float DistanceTo( float x, float y, float z )
	{
		float dx = this->x - x;
		float dy = this->y - y;
		float dz = this->z - z;
		return (float) sqrt( dx*dx + dy*dy + dz*dz );
	}
	
	float dot( PhysVector3D p )         	{ return x*p.x + y*p.y + z*p.z; }
	float length()                 			{ return (float) sqrt( x*x + y*y + z*z ); }
	float lengthSquared()						{ return x*x + y*y + z*z; }
	  
	void clear()                   			{ x = 0.0f; y = 0.0f; z = 0.0f; }

	//String toString()              				{ return new String( "(" + x + ", " + y + ", " + z + ")" ); }

	PhysVector3D cross( PhysVector3D p )
	{
		PhysVector3D a;
  a.x=this->y*p.z - this->z*p.y;
		a.y=this->x*p.z - this->z*p.x;
		a.z=this->x*p.y - this->y*p.x;
  return a;
	}
};

class PhysVector3Ds : public LinkedList {};
