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
 ***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once

#include "macros.h"
#include "moremath.h"

#include "Numbers.h"

#include "Particles.h"

class Scatter : public ParticleSystem {
public:
 float duration;
 float expired;

 Scatter( int n, float d ) {
  Constructor();
  expired=0.0f;
  duration=d;
  int x=n;
  while ( x-- > 0 ) {
   Particle *p=makeParticle(1.0f,0.0f,0.0f,0.0f);  
//   p->force.Set(x,Y,Z);
   p->position.getRandomVector();
   p->velocity.getRandomVector();
   float speed=(float) random(4.5f)+0.5f;
   p->velocity.multiplyf(speed);
   particles.Append(p);
  }
 }

 Scatter( int n, float d, float G ) {
  Constructor();
  expired=0.0f;
  duration=d;
  int x=n;
  setGravity(G);
  while ( x-- > 0 ) {
   Particle *p=makeParticle(1.0f,0.0f,0.0f,0.0f);  
   p->position.getRandomVector();
   p->velocity.getRandomVector();
   float speed=(float) random(4.5f)+0.5f;
   p->velocity.multiplyf(speed);
   particles.Append(p);
  }
 }

 Scatter( int n, float d, float gx, float gy, float gz ) {
  Constructor();
  expired=0.0f;
  duration=d;
  int x=n;
  SetGravity(gx,gy,gz);
  while ( x-- > 0 ) {
   Particle *p=makeParticle(1.0f,0.0f,0.0f,0.0f);  
   p->position.getRandomVector();
   p->velocity.getRandomVector();
   float speed=(float) random(4.5f)+0.5f;
   p->velocity.multiplyf(speed);
   particles.Append(p);
  }
 }

 void Update( float dt ) {
  expired+=dt;
  this->tick();
 }
};


class Pendulum : public ParticleSystem {
public:
 Particle *pendulum;
 Particle *anchor;
 Spring *spring;
 float G;
 float drag;
 float mass;
 Vertex scale;
 float springConstant, damping, restLength;

 Pendulum() {
  Constructor();
  G=5.0f;
  drag=1.0f;
  mass=1.0f;
  pendulum=null;
  anchor=null;
  spring=null;
  scale.Set(1.0f,1.0f,1.0f);
  springConstant=1.0f;
  damping=0.1f;
  restLength=100.0f;
 }

 void Setup() {
  setGravity(G);
  setDrag(drag);
  pendulum = makeParticle( 1.0f, scale.x, scale.y, scale.z );
  anchor = makeParticle( 1.0f, scale.x, scale.y, scale.z );
  anchor->makeFixed(); 
  spring = makeSpring( pendulum, anchor, springConstant, damping, restLength );
 }
 
// void Draw() {
  //tick();
  //line( p.position().x(), p.position().y(), anchor.position().x(), anchor.position().y() );
  //ellipse( anchor.position().x(), anchor.position().y(), 5, 5 );
  //ellipse( p.position().x(), p.position().y(), 20, 20 );
// }

};


class Chain : public ParticleSystem {
public:
 float G;
 float drag;
 float mass;
 int links;
 float springConstant, damping, restLength;
 Vertex scale;
 Particle *anchor;
 Particle *pendulum;
 Pointers pointers;

 Chain() {
  Constructor();
  G=5.0f;
  drag=1.0f;
  mass=1.0f;
  scale.Set(1.0f,1.0f,1.0f);
  links=10;
  springConstant=2.0f;
  damping=0.1f;
  restLength=0.01f;
 }

 void Setup() {
  Particle *last=null, *current=null;
  setGravity(G);
  setDrag(drag);
  pointers.append( (void *) (anchor=makeParticle( 1.0, scale.x, scale.y, scale.z )) );
  anchor->makeFixed();
  last=anchor;
  for ( int i=0; i<links; i++ ) {
   pointers.append( (void *) (current=makeParticle( mass/(links/2), scale.x, scale.y, scale.z )) );
   makeSpring( last, current, springConstant, damping, restLength );
  }
  pointers.append( (void *) (pendulum=makeParticle( mass, scale.x, scale.y, scale.z )) );
 }

// void Draw() {
 /*
 beginShape( LINE_STRIP );
  curveVertex( particles[0].position().x(), particles[0].position().y() );
  for ( int i = 0; i < particles.length; ++i )
  {
    curveVertex( particles[i].position().x(), particles[i].position().y() );
  }
  curveVertex( particles[particles.length-1].position().x(), particles[particles.length-1].position().y() );
  endShape();
  
  ellipse( particles[0].position().x(), particles[0].position().y(), 5, 5 );
  ellipse( particles[particles.length-1].position().x(), particles[particles.length-1].position().y(), 20, 20 );
 */
// }
};

class Smoke : public ParticleSystem {
public:
 float G,mass,drag,springConstant,damping,restLength;
 Vertex cold,heatVector;
 int puffs;
 int age;
 Smoke() {
  Constructor();
  G=-0.1f;
  drag=0.001f;
  mass=1.0f;
  springConstant=0.1f;
  damping=0.1f;
  restLength=10.0f;
  puffs=5;
  heatVector.Set( 2.0f, 3.0f, 0.0f );
  cold.Set( -1.0f, -5.0f, 0.0f );
  age=96;
 }
 void Setup() {
  setGravity(G);
  setDrag(drag);
 }
 void Update() {
  Particle *last=null;
  if ( puffs%2!=0 ) puffs++;
  for ( int i=0; i<puffs; i++ ) {
   Particle *p=makeParticle(1.0f,0.0f,0.0f,0.0f);
   p->velocity.Set(
    cold.x+float_range(0.0f,heatVector.x),
    cold.y+float_range(0.0f,heatVector.y),
    cold.z+float_range(0.0f,heatVector.z) );
   if ( last != null ) makeSpring( p, last, springConstant, damping, restLength );
   last=p;
  }
  Particle *n;
  for ( Particle *p=(Particle *) particles.first; p; p=n ) {
   n=(Particle *) (p->next);
   if ( p->age > age ) {
    particles.Remove(p);
    delete p;
   }
  }
 }  
};

enum ClothesPins {
 pinNone=-1,
 pinTop=0,
 pinLeft=1,
 pinRight=2,
 pinBottom=3,
 pinTopCorners=4,
 pinBottomCorners=5,
 pinLeftCorners=6,
 pinRightCorners=7,
 pinTopLeft=8,
 pinTopRight=9,
 pinBottomLeft=10,
 pinBottomRight=11,
 pinCorners=12
};

class Cloth : public ParticleSystem {
public:
 float G,mass,drag,springConstant,damping;
 Vertex scale;
 Vertex slack;
 Euler initial;
 int w,h;
 ClothesPins pins;
 Cloth() {
  Constructor();
  w=8;
  h=10;
  mass=0.2f;
  initial.pitch=PI/2;
  initial.yaw=0.0f;
  initial.roll=0.0f;
  scale.Set(1.0f,1.0f,1.0f);
  slack.Set(0.0f,0.0f,0.0f);
  pins=pinTop;
 }
 void Fasten( ClothesPins wooden ) {
  Particle *p;
  int i,j;
  switch ( wooden ) {
    case pinTop: for ( i=0; i<w; i++ ) { p=(Particle *) particles.Element(i); p->makeFixed(); } break;
   case pinLeft: for ( j=0; j<h; j++ ) { p=(Particle *) particles.Element((w-1)+j*w); p->makeFixed(); } break;
  case pinRight: for ( j=0; j<h; j++ ) { p=(Particle *) particles.Element(j); p->makeFixed(); } break;
 case pinBottom: j=(h-1)*w; for ( i=0; i<w; i++ ) { p=(Particle *) particles.Element(i+j); p->makeFixed(); } break;
    case pinTopCorners: p= (Particle *) particles.first; p->makeFixed();
                        p= (Particle *) particles.Element(h-1);
                       break;
 case pinBottomCorners: p=(Particle *) particles.Element((h-1)*w-(w-1)); p->makeFixed();
                        p=(Particle *) particles.last; p->makeFixed();
                       break;
   case pinLeftCorners: p=(Particle *) particles.first; p->makeFixed();
                        p=(Particle *) particles.Element((h-1)*w-(w-1)); p->makeFixed();
                       break;
  case pinRightCorners: p=(Particle *) particles.Element(h-1); p->makeFixed();
                        p=(Particle *) particles.last; p->makeFixed();
                       break;
       case pinTopLeft: { p=(Particle *) particles.first; p->makeFixed(); } break;
      case pinTopRight: { p=(Particle *) particles.Element(h-1); p->makeFixed(); } break;
    case pinBottomLeft: { p=(Particle *) particles.Element((h-1)*w-(w-1)); p->makeFixed(); } break;
   case pinBottomRight: { p=(Particle *) particles.last; p->makeFixed(); } break;
   case pinCorners:  
     p=(Particle *) particles.first; p->makeFixed();
     p=(Particle *) particles.Element(h-1); p->makeFixed();
     p=(Particle *) particles.Element((h-1)*w-(w-1)); p->makeFixed();
     p=(Particle *) particles.last; p->makeFixed();
    break;
   default:break;
  }
 }
 void Setup() {
  setGravity(G);
  setDrag(drag);
  for ( int x=0; x<w; x++ ) for ( int y=0; y<h; y++ ) {
   Particle *p=makeParticle(mass, x*scale.x, y*scale.y, scale.z );
   p->position.q=(float) x/(float) w;
   p->position.r=(float) y/(float) h;
   if ( y>0 )
    makeSpring( (Particle *) particles.Element(x+((y-1)*w)), p, springConstant, damping, scale.x+slack.x );
  }
  for ( int x=0; x<w; x++ ) for ( int y=1; y<h; y++ ) {
   makeSpring( (Particle *) particles.Element(x+((y-1)*w)),
               (Particle *) particles.Element(x+y*w),
               springConstant, damping, scale.y+slack.y );
  }
  Fasten(pins);
 }
};


class Cloud : public ParticleSystem {
public:
 Particle *eye;
 float G,mass,drag,k,minDistance;
 Vertex scale;
 int puffs;
 Cloud() {
  Constructor();
  G=0.0f;
  mass=1.0f;
  drag=0.1f;
  puffs=100;
  k=5000.0f;
  minDistance=50.0f;
  scale.Set(1.0f,1.0f,1.0f);
 }
 void Setup() {
  setGravity(G);
  setDrag(drag);
  eye=makeDefaultParticle();
  eye->makeFixed();
  for ( int i=0; i<puffs; i++ ) {
   Particle *p=makeParticle( mass, float_range(0,scale.x), float_range(0,scale.y), float_range(0,scale.z) );
   makeAttraction( eye, p, k, minDistance );
  }
 }
};

class Arboretum : public ParticleSystem {
public:

};