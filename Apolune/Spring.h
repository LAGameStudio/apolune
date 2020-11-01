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
#include <Windows.h>

#include "ListItem.h"
#include "LinkedList.h"

#include "Particle.h"
#include "Force.h"

class Spring : public Force
{
public:
 float springConstant;
 float damping;
 float restLength;
 Particle *a, *b;
 bool on;

 Spring() { on=true; }
 ~Spring() {}

 virtual Force *Copy() {
  Spring *s=new Spring;
  s->springConstant=springConstant;
  s->damping=damping;
  s->restLength=restLength;
  s->a=a;
  s->b=b;
  s->on=on;
  return (Force *)s;
 }

 void Init( Particle *A, Particle *B, float ks, float d, float r )
 {
  springConstant = ks;
  damping = d;
  restLength = r;
  a = A;
  b = B;
  on = true;
 }
 
 void turnOff()  {	on = false; }
 void turnOn()   {	on = true;  }
 bool isOn()     {	return on;  }
 bool isOff()    {	return !on; }

 Particle *getOneEnd()       { return a;  }
 Particle *getTheOtherEnd()  { return b;  }

 float currentLength()         { return a->position.distanceTo( &(b->position) );  }
 float strength()              { return springConstant;  }
 void setStrength( float ks )  { springConstant = ks;  }

 void setDamping( float d )    { damping = d;     }
 void setRestLength( float l ) { restLength = l;  }

 void setA( Particle *p )  { a = p; }
 void setB( Particle *p )  { b = p; }

 void apply()
 {	
 	if ( on && ( a->isFree() || b->isFree() ) )
 	{
 		float a2bX = a->position.x - b->position.x;
 		float a2bY = a->position.y - b->position.y;
 		float a2bZ = a->position.z - b->position.z;
 		float Va2bX, Va2bY, Va2bZ, r, dampingForce, springForce;
  	float a2bDistance = (float)sqrt( a2bX*a2bX + a2bY*a2bY + a2bZ*a2bZ );

 		if ( a2bDistance == 0 )		{	a2bX = 0;			a2bY = 0;	a2bZ = 0;	}
 		else                   		{	a2bX /= a2bDistance;		a2bY /= a2bDistance;	a2bZ /= a2bDistance;	}
 	
 		// spring force is proportional to how much it stretched 
 		springForce = -( a2bDistance - restLength ) * springConstant; 
 		
 		// want velocity along line b/w a & b, damping force is proportional to this
 		Va2bX = a->velocity.x - b->velocity.x;
 		Va2bY = a->velocity.y - b->velocity.y;
 		Va2bZ = a->velocity.z - b->velocity.z;
 		dampingForce = -damping * ( a2bX*Va2bX + a2bY*Va2bY + a2bZ*Va2bZ );
 		
 		// forceB is same as forceA in opposite direction
 		r = springForce + dampingForce;
 		a2bX *= r;		a2bY *= r;		a2bZ *= r;
 	    
 		if ( a->isFree() )			a->force.Add( a2bX, a2bY, a2bZ );
 		if ( b->isFree() )			b->force.Add( -a2bX, -a2bY, -a2bZ );
 	}
 }
};