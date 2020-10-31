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

#include <Windows.h>

#include "ListItem.h"
#include "Vertex.h"
#include "Integrator.h"
#include "Particles.h"

class RungeKuttaIntegrator : public Integrator
{	
public:
	Vertices originalPositions;
	Vertices originalVelocities;
	Vertices k1Forces;
	Vertices k1Velocities;
	Vertices k2Forces;
	Vertices k2Velocities;
	Vertices k3Forces;
	Vertices k3Velocities;
	Vertices k4Forces;
	Vertices k4Velocities;
 ParticleSystem *s;

 RungeKuttaIntegrator() {
  s=null;
 }

 ~RungeKuttaIntegrator() { 
 }
	
 void Init( void *ps )	{
		this->s = (ParticleSystem *)ps;
		originalPositions.Clear();
  originalVelocities.Clear();
  k1Forces.Clear();
  k1Velocities.Clear();
  k2Forces.Clear();
  k2Velocities.Clear();
  k3Forces.Clear();
  k3Velocities.Clear();
  k4Forces.Clear();
  k4Velocities.Clear();
	}
	
	void allocateParticles()
	{
		while ( s->particles.count > originalPositions.count )
		{
	 	originalPositions.Append(  (ListItem *) (new Vertex) );
			originalVelocities.Append( (ListItem *) (new Vertex) );
			k1Forces.Append(           (ListItem *) (new Vertex) );
			k1Velocities.Append(       (ListItem *) (new Vertex) );
			k2Forces.Append(           (ListItem *) (new Vertex) );
			k2Velocities.Append(       (ListItem *) (new Vertex) );
			k3Forces.Append(           (ListItem *) (new Vertex) );
			k3Velocities.Append(       (ListItem *) (new Vertex) );
			k4Forces.Append(           (ListItem *) (new Vertex) );
			k4Velocities.Append(       (ListItem *) (new Vertex) );
		}
	}
	
	void step( float deltaT )
	{	
  Particle *p;
  int i;
		allocateParticles();

  i=0;
		for ( p=(Particle *) (s->particles.first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )	{
  		// save original position and velocities
    Vertex *v;
				v=(Vertex *) originalPositions.Element( i );  p->force.Duplicate(v);
    v=(Vertex *) originalVelocities.Element( i ); p->force.Duplicate(v);
			}
			p->force.clear();	// and clear the forces
   i++;
		}
		
		// get all the k1 values	
		s->applyForces();
		
  i=0;
		for ( p=(Particle *) (s->particles.first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )	{
  		// save the intermediate forces
    Vertex *v;
				v=(Vertex *) k1Forces.Element( i );     p->force.Duplicate(v);
				v=(Vertex *) k1Velocities.Element( i ); p->force.Duplicate(v);
			}		
			p->force.clear();

	 	// get k2 values
   if ( p->isFree() )
			{
				Vertex *originalPosition = (Vertex *) originalPositions.Element( i );
				Vertex *k1Velocity = (Vertex *) k1Velocities.Element( i );
				Vertex *originalVelocity,*k1Force;

				p->position.x = originalPosition->x + k1Velocity->x * 0.5f * deltaT;
				p->position.y = originalPosition->y + k1Velocity->y * 0.5f * deltaT;
				p->position.z = originalPosition->z + k1Velocity->z * 0.5f * deltaT;
				
				originalVelocity = (Vertex *) originalVelocities.Element( i );
				k1Force = (Vertex *) k1Forces.Element( i );
				
				p->velocity.x = originalVelocity->x + k1Force->x * 0.5f * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k1Force->y * 0.5f * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k1Force->z * 0.5f * deltaT / p->mass;
			}
   i++;
		}
		s->applyForces();

  i=0;
		for ( p=(Particle *) (s->particles.first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )
			{
  		// save the intermediate forces
    Vertex *v;
    v=(Vertex *) k2Forces.Element( i );     p->force.Duplicate(v);
				v=(Vertex *) k2Velocities.Element( i ); p->force.Duplicate(v);
			}			
			p->force.clear();	// and clear the forces now that we are done with them

 		// get k3 values
			if ( p->isFree() )
			{
 			Vertex *originalPosition = (Vertex *) originalPositions.Element( i );
				Vertex *k2Velocity = (Vertex *) k2Velocities.Element( i );
				Vertex *originalVelocity,*k2Force;
				
				p->position.x = originalPosition->x + k2Velocity->x * 0.5f * deltaT;
				p->position.y = originalPosition->y + k2Velocity->y * 0.5f * deltaT;
				p->position.z = originalPosition->z + k2Velocity->z * 0.5f * deltaT;
				
				originalVelocity = (Vertex *) originalVelocities.Element( i );
				k2Force = (Vertex *) k2Forces.Element( i );
				
				p->velocity.x = originalVelocity->x + k2Force->x * 0.5f * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k2Force->y * 0.5f * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k2Force->z * 0.5f * deltaT / p->mass;
			}
   i++;
		}
		s->applyForces();

  i=0;
		for ( p=(Particle *) (s->particles.first); p; p=(Particle *) (p->next) ) {
			if ( p->isFree() )
			{
  		// save the intermediate forces
    Vertex *v;
				v=(Vertex *) k3Forces.Element( i );     p->force.Duplicate(v);
				v=(Vertex *) k3Velocities.Element( i ); p->force.Duplicate(v);
			}			
			p->force.clear();	// and clear the forces now that we are done with them

 		// get k4 values
			if ( p->isFree() )
			{
				Vertex *originalPosition = (Vertex *) originalPositions.Element( i );
				Vertex *k3Velocity = (Vertex *) k3Velocities.Element( i );
				Vertex *originalVelocity, *k3Force;

				p->position.x = originalPosition->x + k3Velocity->x * deltaT;
				p->position.y = originalPosition->y + k3Velocity->y * deltaT;
				p->position.z = originalPosition->z + k3Velocity->z * deltaT;
				
				originalVelocity = (Vertex *) originalVelocities.Element( i );
				k3Force = (Vertex *) k3Forces.Element( i );
				
				p->velocity.x = originalVelocity->x + k3Force->x * deltaT / p->mass;
				p->velocity.y = originalVelocity->y + k3Force->y * deltaT / p->mass;
				p->velocity.z = originalVelocity->z + k3Force->z * deltaT / p->mass;
			}
   i++;
		}
		s->applyForces();

  i=0;
		for ( p=(Particle *) (s->particles.first); p; p=(Particle *) (p->next) ) {
   p->age += deltaT;
			if ( p->isFree() )
			{
  		// save the intermediate forces
    Vertex *v;
				v=(Vertex *) k4Forces.Element( i );     p->force.Duplicate(v);
				v=(Vertex *) k4Velocities.Element( i ); p->force.Duplicate(v);

  		// put them all together and what do you get?
    Vertex *originalVelocity,*k1Force,*k2Force,*k3Force,*k4Force;

    // update position
				Vertex *originalPosition = (Vertex *) originalPositions.Element( i );
				Vertex *k1Velocity = (Vertex *) k1Velocities.Element( i );
				Vertex *k2Velocity = (Vertex *) k2Velocities.Element( i );
				Vertex *k3Velocity = (Vertex *) k3Velocities.Element( i );
				Vertex *k4Velocity = (Vertex *) k4Velocities.Element( i );
				
				p->position.x = originalPosition->x + deltaT / 6.0f * ( k1Velocity->x + 2.0f*k2Velocity->x + 2.0f*k3Velocity->x + k4Velocity->x );
				p->position.y = originalPosition->y + deltaT / 6.0f * ( k1Velocity->y + 2.0f*k2Velocity->y + 2.0f*k3Velocity->y + k4Velocity->y );
				p->position.z = originalPosition->z + deltaT / 6.0f * ( k1Velocity->z + 2.0f*k2Velocity->z + 2.0f*k3Velocity->z + k4Velocity->z );
				
				// update velocity
				originalVelocity = (Vertex *) originalVelocities.Element( i );
				k1Force = (Vertex *) k1Forces.Element( i );
				k2Force = (Vertex *) k2Forces.Element( i );
				k3Force = (Vertex *) k3Forces.Element( i );
				k4Force = (Vertex *) k4Forces.Element( i );
				
				p->velocity.x = originalVelocity->x + deltaT / ( 6.0f * p->mass ) * ( k1Force->x + 2.0f*k2Force->x + 2.0f*k3Force->x + k4Force->x );
				p->velocity.y = originalVelocity->y + deltaT / ( 6.0f * p->mass ) * ( k1Force->y + 2.0f*k2Force->y + 2.0f*k3Force->y + k4Force->y );
				p->velocity.z = originalVelocity->z + deltaT / ( 6.0f * p->mass ) * ( k1Force->z + 2.0f*k2Force->z + 2.0f*k3Force->z + k4Force->z );
			}
   i++;
   p->Virtual();
  }
	}
 virtual Integrator *Copy() {
  RungeKuttaIntegrator *e=new RungeKuttaIntegrator;
  e->s=s;
  return (Integrator *) e;
 }
};