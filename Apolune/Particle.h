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
#include "Vertex.h"

/*
 Particles can represent objects, corners of 2D or 3D shapes or abstract things that
 won't even be drawn. Particles have 4 properties:

   1. Mass
   2. Position
   3. Velocity
   4. Age 

 Particles can also be either fixed or free moving, or dead.
 */

class Particle : public ListItem
{
public:
 void *data;
 Vertex position;
 Vertex velocity;
 Vertex force;
 Vertex rotation,spin;
 Vertex deformation;
 float mass,age;
 bool dead,fixed;

 Particle(void) {
  mass=age=0.0f;
  dead=fixed=false;
  data=null;
 }

 ~Particle(void) {}

 Particle *Copy() {
  Particle *p=new Particle;
  p->data=data;
  p->position=position;
  p->velocity=velocity;
  p->force=force;
  p->rotation=rotation;
  p->spin=spin;
  p->deformation=deformation;
  p->mass=mass;
  p->age=age;
  p->dead=dead;
  p->fixed=fixed;
  return p;
 }
 
 float distanceTo( Particle *p ) { return this->position.distanceTo( &(p->position) ); }

 void makeFixed() {	fixed = true;  velocity.clear();  }
 void makeFree()  {	fixed = false;  }

 bool isFixed()   {	return fixed;   }
 bool isFree()    {	return !fixed;  } 
  
 void setMass( float m ) { mass = m; }

 void rotate() {
  rotation.add(&spin);
 }
  
 void reset()
 {
	 age = 0;
	 dead = false;
	 position.clear();
	 velocity.clear();
	 force.clear();
	 mass = 1.0f;
 }
};