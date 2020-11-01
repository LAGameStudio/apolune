/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "Particle.h"
#include "Force.h"

class Attraction : public Force {
public:
	Particle *a;
	Particle *b;
	float k;
	bool on;
	float distanceMin;
	float distanceMinSquared;
 
 virtual Force *Copy() {
  Attraction *A=new Attraction;
  A->a=a;
  A->b=b;
  A->k=k;
  A->on=on;
  A->distanceMin=distanceMin;
  A->distanceMinSquared=distanceMinSquared;
  return (Force *)A;
 }

	void Init( Particle *a, Particle *b, float k, float distanceMin )
	{
		this->a = a;
		this->b = b;
		this->k = k;
		on = true;
		this->distanceMin = distanceMin;
		this->distanceMinSquared = distanceMin*distanceMin;
	}

	void setA( Particle *p )	{	a = p;	}
	void setB( Particle *p ) {	b = p;	}
	float getMinimumDistance()	{	return distanceMin;	}
	void setMinimumDistance( float d )
	{
		distanceMin = d;
		distanceMinSquared = d*d;
	}

	void turnOff()	{	on = false;	}
 void turnOn()	{	on = true;	}

	void setStrength( float k )	{	this->k = k;	}
	float getStrength() {	return k;	}
 bool isOn() {	return on;	}
 bool isOff()	{	return !on;	}
 Particle *getOneEnd()	{	return a;	}
	Particle *getTheOtherEnd()	{	return b;	}

	void apply()
	{
		if ( on && ( a->isFree() || b->isFree() ) )	{
   float a2bX,a2bY,a2bZ,a2bDistanceSquared,distanceMinSquared=0.0f,force,length;

   a2bX = (float) (a->position.x - b->position.x);
			a2bY = (float) (a->position.y - b->position.y);
			a2bZ = (float) (a->position.z - b->position.z);

			a2bDistanceSquared = a2bX*a2bX + a2bY*a2bY + a2bZ*a2bZ;
			if ( a2bDistanceSquared < distanceMinSquared )	a2bDistanceSquared = distanceMinSquared;
   force = k * a->mass * b->mass / a2bDistanceSquared;
   length = (float)sqrt( a2bDistanceSquared );
			
			// make unit vector
			a2bX /= length;
			a2bY /= length;
			a2bZ /= length;
			
			// multiply by force 
			a2bX *= force;
			a2bY *= force;
			a2bZ *= force;

			// apply
			if ( a->isFree() )				a->force.Add( -a2bX, -a2bY, -a2bZ );
			if ( b->isFree() )				b->force.Add( a2bX, a2bY, a2bZ );
		}
	}
};

