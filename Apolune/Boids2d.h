#pragma once
#include "MapPosition.h"
#include "Vertexd.h"

// This is a controller class for anything with a MapPosition

class Boids2d;

struct BoidForces {
 Vertexd alignment,cohesion,separation;
 double aMod,cMod,sMod; //modifiers for the three forces on the boid
 BoidForces() {
  aMod = 1.0;
  cMod = 4.0;
  sMod = 1.0;
 }
 void Reset() {
  alignment.None();
  cohesion.None();
  separation.None();
 }
};

class Boid2d : public ListItem {
public:
 MapPosition *map;
 BoidForces forces;
 double awareness;     // Awareness distance
 double maxSpeed;      // Maximum speed
 double maxSteerForce; // Turn Rate
 Vertexd avoid;
 Vertexd *avoiding;
 Vertexd steer;
 Vertexd zeroed;
 bool alpha;
  
 //constructors
 Boid2d( MapPosition *m ) : ListItem() {
  map=m;
  awareness=50.0;
  maxSpeed=5.0;       
  maxSteerForce=0.05;
  avoiding=null;
  alpha=false;
 }
    
 void Move()
 {
   map->velocity.Add(&map->acceleration); //add acceleration to velocity
   map->velocity.Limit(maxSpeed); //make sure the velocity vector magnitude does not exceed maxSpeed
   map->position.Add(&map->velocity); //add velocity to positionm
   double d=ddistance(map->position.x,map->position.y,0.0,0.0);
   if ( !avoiding && d < 300.0 ) {
    avoiding = &zeroed;
   } else if ( d > 2*display.w ) {
    avoiding=null;
   }
 }
  
 void Debug() {
  glPush();
  glTranslated(map->position.x,map->position.y,0.0);
  glRotated(atan2(map->velocity.y,map->velocity.x),0.0,1.0,0.0); //rotate drawing matrix to direction of velocity
  Crayon drawing;
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_TRIANGLES);
  glVertex2d(3.0,0.0);
  glVertex2d(-3.0,2.0);
  glVertex2d(-3.0,-2.0);
  glEnd();
  glPop();
 }
  
 //Avoidance; If InvSquareAvoid == true avoidance vector is larger when closer to target
 void Avoid(bool InvSquareAvoid) {
  avoid.SetSubtract(&map->position,avoiding);
  if( InvSquareAvoid ) {
   avoid.Multiply(1.0/SQ(ddistance(map->position.x,map->position.y,avoiding->x,avoiding->y)));
  }
 }
};

/*BoidList object class
* Matt Wetmore
* Changelog
* ---------
* 12/18/09: Started work
*/
 
class Boids2d : public LinkedList {
public:

 Boid2d *Add( MapPosition *p ) {
  Boid2d *b;
  Append(b=new Boid2d(p));
  b->map=p;
  return b;
 }

 Boid2d *find( MapPosition *p ) {
  FOREACH(Boid2d,b) if ( b->map == p ) return b;
  OUTPUT("Boids2d: Could not find specified position in list.\n");
  return null;
 }

 void Between() {
  Vertexd repulse;
  FOREACH(Boid2d,us) if ( !us->alpha ) {
   int interactions=0;
   double d;
   us->forces.Reset();
   if ( us->avoiding ) us->Avoid(true);
   FOREACH(Boid2d,them) if ( us != them
    && (d=(double)idistance(us->map->position.x,us->map->position.y,them->map->position.x,them->map->position.y))
       <= us->awareness && d > 0.0 ) {
    // Cumulative Alignment Force
    us->forces.alignment.Add(&them->map->velocity);
    // Cumulative Cohesion Force
    us->forces.cohesion.Add(&them->map->position);
    // Cumulative Separation Force
    repulse.SetSubtract( &us->map->position, &them->map->position );
    repulse.Normalize();
    repulse.Divide(d);
    us->forces.separation.Add(&repulse);
    interactions++;
   }
   if ( interactions > 0 ) { // Average
    us->forces.alignment.Divide((double)interactions);
    us->forces.alignment.Limit(us->maxSteerForce);
    us->forces.cohesion.Divide((double)interactions);
   }
   us->forces.cohesion.Subtract(&us->map->position);
   us->forces.cohesion.Limit(us->maxSteerForce);
   // Mod the vectors
   us->forces.alignment.Multiply(us->forces.aMod);
   us->forces.cohesion.Multiply(us->forces.cMod);
   us->forces.separation.Multiply(us->forces.sMod);
   // Calculate the acceleration by adding component vectors
   us->map->acceleration.None();
   us->map->acceleration.Add(&us->avoid);
   us->map->acceleration.Add(&us->forces.alignment);
   us->map->acceleration.Add(&us->forces.cohesion);
   us->map->acceleration.Add(&us->forces.separation);
   us->Move();
  }
 }
};

