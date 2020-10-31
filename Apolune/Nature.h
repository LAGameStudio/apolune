#pragma once

#include "ListItem.h"
#include "LinkedList.h"

#include "ParticleSystem.h"

// Holds ParticleSystems and their associated models within the T.Z.

#include "Vertex.h"
#include "MapObject.h"

class ParticleBinding : public ListItem {
public:
 Particle *p;
 MapObject *m;
};

class ParticleBindings : public LinkedList {
public:
 virtual void BindInOrder( Particles *ps, MapObjects *m ) {
  if ( count > 0 ) RemoveAll();
  for ( MapObject *v=(MapObject *) (m->first); v; v=(MapObject *) (v->next) ) 
   for ( Particle *p=(Particle *) (ps->first); p; p=(Particle *) (p->next) ) {
   ParticleBinding *pb=new ParticleBinding;
   pb->p=p;
   pb->m=v;
  }
 }
 ~ParticleBindings() {
  ParticleBinding *pb,*n;
  for ( pb=(ParticleBinding *) first; pb; pb!=null ) {
   n=(ParticleBinding *) (pb->next);
   delete pb;
  }
 }
};

class NaturalEvent : public ListItem {
public:
 ParticleSystem particles;
 MapObjects *objects;
 ParticleBindings pb;
 void Create( MapObjects *objects ){
  for ( MapObject *o=(MapObject *)(objects->first); o; o=(MapObject *)(o->next) )
   particles.makeParticle(1.0f,o->location.x,o->location.y,o->location.z);
  pb.BindInOrder(particles.particles,objects);
 }
 virtual void Step() {
  particles.step(1.0f);
 }
 ~NaturalEvent(){}
};

class Nature : public LinkedList
{
public:
 void Step() {
  for ( NaturalEvent *e=(NaturalEvent *)(first); e; e=(NaturalEvent *)(e->next) ) 
   e->Step();
 }
 Nature(void){}
 ~Nature(void){}
};

class NE_Planet : public NaturalEvent {
public: 
 NE_Planet( MapObjects *objects, Vertex gravity ) {
  Create( objects );
  particles.SetGravity( gravity.x,gravity.y,gravity.z );
 }
};

class NE_Outerspace : public NaturalEvent {
 NE_Outerspace( MapObjects *objects ) {
  Create( objects );
  particles.SetGravity( 0.0f,0.0f,0.0f );
 }
};