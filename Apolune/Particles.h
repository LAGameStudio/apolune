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

#include "macros.h"
#include "Strings.h"

#include "ListItem.h"
#include "Vertex.h"
#include "Particle.h"
#include "Force.h"
#include "Attraction.h"
#include "Spring.h"

#include "Integrator.h"

#include "GLWindow.h"

class Particles : public LinkedList {
public:
 CLEARLISTRESET(Particle);
};

#define DEFAULT_GRAVITY 0
#define DEFAULT_DRAG    0.001f

class ParticleSystem : public ListItem
{
public:
 Particles particles;
 Forces springs;
 Forces attractions;
 Forces customForces;
 Integrator *integrator;
 Vertex gravity;
 float drag;
 bool hasDeadParticles;

 ParticleSystem *Copy() {
  ParticleSystem *p=new ParticleSystem;
  for ( Particle *q=(Particle *) particles.first; q; q=(Particle *) (q->next) )
   p->particles.Append( q->Copy() );
  for ( Force *f=(Force *) springs.first; f; f=(Force *) (f->next) )
   p->springs.Append( f->Copy() );
  for ( Force *f=(Force *) attractions.first; f; f=(Force *) (f->next) )
   p->attractions.Append( f->Copy() );
  for ( Force *f=(Force *) customForces.first; f; f=(Force *) (f->next) )
   p->customForces.Append( f->Copy() );
  p->integrator=integrator->Copy();
  p->gravity=gravity;
  p->drag=drag;
  p->hasDeadParticles=hasDeadParticles;
 }

 ParticleSystem() {
  prev=next=null;
  hasDeadParticles=false;
  integrator=null;
 }

 void Constructor() {
  prev=next=null;
  hasDeadParticles=false;
  integrator=null;
 }

 ~ParticleSystem() {
  if ( integrator ) delete integrator;
 }
 
 void setIntegrator( Integrator *i );

  // default down gravity
 void setGravity( float g );
 void SetGravity( float x, float y, float z );
 void setDrag( float d );
  
 void step( float t );
 void tick();
 
 Particle *makeParticle();
 Particle *makeParticle( float m, float x, float y, float z );
 Particle *makeParticle( Particle *p, float m, float x, float y, float z );
 Particle *makeDefaultParticle();
 /*
 Springs connect 2 particles and try to keep them a certain distance apart. 
 They have 3 properties:

   1. Rest Length, the spring wants to be at this length and acts on the particles to 
      push or pull them exactly this far apart at all times.

   2. Strength, If they are strong they act like a stick. If they are weak they take 
      a long time to return to their rest length.

   3. Damping, If springs have high damping they don't overshoot and they settle down 
      quickly, with low damping springs oscillate. 

  Springs can also be turned on or off.
  
  Particle getOneEnd()
  Particle getTheOtherEnd()
  Return the particles that are on either end of this spring.
  
  float currentLength()
  The current length of the spring.
  
  float restLength()
  void setRestLength( float l )
  
  float strength()
  void setStrength( float s )
  
  float damping()
  void setDamping( float s )
  
  void turnOff()
  void turnOn()
  boolean isOn()
  boolean isOff()
 */
 Spring *makeSpring( Particle *a, Particle *b, float strength, float damping, float restLength );
 /*
 G*m1*m2/d2 
 make an attraction (or repulsion) force between two particles.
 If the strength is negative they repel each other, if the strength
 is positive they attract. There is also a minimum distance that
 limits how strong this force can get close up.

 As always you can turn this force on and off.

 float strength()
 void setStrength( float s )
 Positive strength is attraction negative strength is repulsion.

 Particle getOneEnd()
 Particle getTheOtherEnd()
 Return the particles being attracting or repeling each other.

 float minimumDistance()
 void setMinimumDistance( float d )
 Get and set the minimum distance, which limits how strong the force can get close up. 
 */
 Attraction *makeAttraction( Particle *a, Particle *b, float strength, float minDistance );
 void clear();

 void Init( float g, float somedrag );
 void Init( float gx, float gy, float gz, float somedrag );
  
 void Init();
 void applyForces();
 void clearForces();
};

class ParticleSystems : public LinkedList {
public:
 CLEARLISTRESET(ParticleSystem);
};

class ParticleMovie : public ListItem {
public:
 string name;
 ParticleSystems frames;
 ParticleMovie() {
  name=string("");
 }
 void Compute( ParticleSystem *source, int granularity ) {
  for ( int i=0; i<granularity; i++ ) {
   frames.Append( (source->Copy()) );
   source->tick();
  }
 }
 ParticleSystem *frame( float duration, float expired ) {
  float current=duration/expired;
  int currenti=(int) current;
  return (ParticleSystem *) frames.Element(currenti%frames.count);
 }
};

class ParticleMovies : public LinkedList {
public:
 ParticleMovie *find( const char *s ) {
  for ( ParticleMovie *p=(ParticleMovie *) first; p; p=(ParticleMovie *) (p->next) )
   if ( !str_cmp(p->name.c_str(),s) ) return p;
  return null;
 }
 CLEARLISTRESET(ParticleMovie);
};

class ParticleProjector : public ListItem {
public:
 ParticleMovie *pm;
 ParticleSystem *current;
 float expired;
 float duration;
 void Rewind() {
  expired=0.0f;
  current=pm->frame( duration, expired );
 }
 ParticleSystem *Update( float dt ) {
  expired+=dt;
  current=pm->frame( duration, expired );
 }
 virtual void Draw(GLWindow *surface) {
 }
};