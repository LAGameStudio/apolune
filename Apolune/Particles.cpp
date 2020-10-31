
#include "Particle.h"
#include "Force.h"
#include "Attraction.h"
#include "Spring.h"
#include "Integrator.h"
#include "RungeKuttaIntegrator.h"
#include "ModifiedEulerIntegrator.h"

#include "Particles.h"

 void ParticleSystem::setIntegrator( Integrator *i )
 {
  if ( this->integrator ) delete this->integrator;
  this->integrator=i;
 }
  
  // default down gravity
 void ParticleSystem::setGravity( float g )                   { gravity.Set( 0.0f, g, 0.0f ); }
 void ParticleSystem::SetGravity( float x, float y, float z ) { gravity.Set( x, y, z ); }
 void ParticleSystem::setDrag( float d )                      { drag = d; }
  
 void ParticleSystem::step( float t ) { integrator->step( t ); }
 void ParticleSystem::tick()          {	step( 1.0f );          }

 Particle *ParticleSystem::makeParticle()
 {
  Particle *p= new Particle;
  p->mass = 0.0f;
  p->position.Set( 0.0f, 0.0f, 0.0f );
  particles.Append( p );
  return p;
 }

 Particle *ParticleSystem::makeParticle( float m, float x, float y, float z )
 {
  Particle *p= new Particle;
  p->mass = m;
  p->position.Set( x, y, z );
  particles.Append( p );
  return p;
 }

 Particle *ParticleSystem::makeParticle( Particle *p, float m, float x, float y, float z )
 {
  p->mass = m;
  p->position.Set( x, y, z );
  particles.Append( p );
  return p;
 }

 Particle *ParticleSystem::makeDefaultParticle()  { return makeParticle( 1.0f, 0.0f, 0.0f, 0.0f ); }
  
 Spring *ParticleSystem::makeSpring( Particle *a, Particle *b, float strength, float damping, float restLength )
 {
  Spring *s= new Spring;
  s->Init( a,b,strength,damping,restLength );
  springs.Append( s );
  return s;
 }
  
 Attraction *ParticleSystem::makeAttraction( Particle *a, Particle *b, float strength, float minDistance )
 {
  Attraction *m= new Attraction;
  m->Init( a, b, strength, minDistance );
  attractions.Append( m );
  return m;
 }
  
 void ParticleSystem::clear()
 {
  particles.Clear();
  springs.Clear();
  attractions.Clear();
  customForces.Clear();
 }
  
 void ParticleSystem::Init( float g, float somedrag )
 {
  delete integrator; 
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.Set( 0.0f, g, 0.0f );
  drag = somedrag;
 }
  
 void ParticleSystem::Init( float gx, float gy, float gz, float somedrag )
 {
  delete integrator;
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.Set( gx, gy, gz );
  drag = somedrag;
 }
  
 void ParticleSystem::Init()
 {
  delete integrator;
  integrator = (Integrator *) (new RungeKuttaIntegrator);
  integrator->Init(this);
  clear();
  gravity.x=0.0f;
  gravity.y=DEFAULT_GRAVITY;
  gravity.z=0.0f;
  drag = DEFAULT_DRAG;
 }
  
 void ParticleSystem::applyForces()
 {
  Particle *p;
  if ( !gravity.isZero() ) for( p=(Particle *) particles.first; p; p=(Particle *) (p->next) ) p->force.add( &gravity );
  for( p=(Particle *) (particles.first);                 p; p=(Particle *) (p->next)   ) p->force.Add( p->velocity.x * -drag, p->velocity.y * -drag, p->velocity.z * -drag ); 
  for( Spring *s=(Spring *) (springs.first);             s; s=(Spring *) (s->next)     ) s->apply();
  for( Attraction *a=(Attraction *) (attractions.first); a; a=(Attraction *) (a->next) ) a->apply();
  for( Force *f=(Force *) (customForces.first);          f; f=(Force *) (f->next)      ) f->apply();
 }
  
 void ParticleSystem::clearForces() { 
  Particle *p;
  for ( p=(Particle *) (particles.first); p; p=(Particle *) (p->next) ) p->force.clear(); 
 }