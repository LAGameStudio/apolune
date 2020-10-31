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
#include "LinkedList.h"

#include "Particle.h"
#include "Vertex.h"
#include "Integrator.h"
#include "Particles.h"

class ModifiedEulerIntegrator : public Integrator 
{
public:
 ParticleSystem *s;
 ModifiedEulerIntegrator() {}
 ~ModifiedEulerIntegrator() {}

 void Init( void *ps )	{
		this->s = (ParticleSystem *)ps;
 }
	void step( float t )
	{
  float halftt;
		s->clearForces();
		s->applyForces();
		halftt=0.5f*t*t;
		for ( int i=0; i < s->particles.count; i++ )
		{
			Particle *p = (Particle *) s->particles.Element( i );
			if ( p->isFree() )
			{
				float ax = p->force.x/p->mass;
				float ay = p->force.y/p->mass;
				float az = p->force.z/p->mass;
				
				p->position.Add( p->velocity.x/t, p->velocity.y/t, p->velocity.z/t );
				p->position.Add( ax*halftt, ay*halftt, az*halftt );
				p->velocity.Add( ax/t, ay/t, az/t );
			}
   p->Virtual();
		}
	}
 virtual Integrator *Copy() {
  ModifiedEulerIntegrator *e=new ModifiedEulerIntegrator;
  e->s=s;
  return (Integrator *) e;
 }
};