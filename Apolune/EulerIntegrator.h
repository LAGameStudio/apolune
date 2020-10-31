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
#include "Particles.h"
#include "Integrator.h"

class EulerIntegrator : public Integrator
{
public:
 ParticleSystem *s;
 void Init( void *x )	{	this->s = (ParticleSystem *) x;	}
	void step( float t )
	{
		s->clearForces();
		s->applyForces();
		for ( int i = 0; i < s->particles.count; i++ )
		{
			Particle *p = (Particle *) s->particles.Element( i );
			if ( p->isFree() )
			{
				p->velocity.Add( p->force.x/(p->mass*t), p->force.y/(p->mass*t), p->force.z/(p->mass*t) );
				p->position.Add( p->velocity.x/t, p->velocity.y/t, p->velocity.z/t );
			}
   p->Virtual();
		}
	}
 virtual Integrator *Copy() {
  EulerIntegrator *e=new EulerIntegrator;
  e->s=s;
  return (Integrator *) e;
 }
};