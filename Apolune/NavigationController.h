#pragma once
#include "Cartesian.h"
#include "Delay.h"
#include "Trigger.h"

extern int netGameSeed;

#define NAV_MIN_FORCE 0.1  // tenth of a pixel
#define NAV_TURNING_INTELLIGENCE_ERROR 60.0        // FPS, or 1 second wrong estimation
#define NAV_THRUST_FACTOR_MINIMUM 0.1 // tenth of a pixel
#define NAV_TURN_LEFT -1.0
#define NAV_TURN_RIGHT 1.0
#define NAV_TURN_STRAIGHT 0.0
#define NAV_TURN_NEARNESS 2.0  // degrees

class NavigationController {
public:
 Zdouble intelligence;  // has to do with turn slowdown calculation.
 Cartesiands forces;
 Cartesiand position,velocity,thrustVector,travelTarget;
 Zdouble angle,acceleration,reverseSpeed,brakeSpeed,thrust,turnAccel,turnBraking,turnVelocity,turningTo,speedingLimit,thrustLimit,turnSpeedLimit,
  turnDirection,turnDirectionDesired,orbitSpeed;
 Zbool reversing,wheelJammed,braking,accelerating,slowingTurn;
 Zbool fleeingFrom,flyingBy,flybyOnce,controlsLocked,confused;
 Zint mySeed;
 Delay controlStillLocked;
 NavigationController() {
  Construct();
  mySeed=pseudorandom(444,666+netGameSeed);
 }
 void Construct() {
  acceleration=0.01; // pixels/frame
  reverseSpeed=0.125; // pixels/frame
  angle=0.0;         // degrees
  turningTo=0.0;     // degrees
  brakeSpeed=0.2;    // pixels/frame
  turnAccel=0.25;      // degrees/frame
  turnBraking=0.25;
  intelligence=1.0;  // perfect estimator
  speedingLimit=0.0;  // 0.0 or negative is conservative, 2.0 is reckless speeding
  thrustLimit=10.0;   // maximum pixels/frame
  turnSpeedLimit=4.0; // maximum turn deg/frame
  turnDirection=0.0;
  turnDirectionDesired=0.0;
  orbitSpeed=2.0;     // seconds before it turns around
 }
 void Reset() {
  velocity.Set(0.0,0.0);
 }
 void LockControls ( double duration=1.0 ) {
  controlsLocked=true;
  controlStillLocked.Set((float)duration);
  controlStillLocked.Reset();
 }
 Zbool accelerated,braked,turned;
 void Between( double worldDragFactor=1.0 ) {
  BeginTurn();
  if ( controlsLocked ) {
   controlStillLocked.Between();
   if ( !controlStillLocked ) {
    controlsLocked=false;
    if ( flybyOnce ) flyingBy=false;
   }
   Accelerate();
  } else {
   if ( accelerating ) Accelerate();
   if ( braking ) Brake();
   if ( !wheelJammed ) Steer(worldDragFactor);
  }
  DiminishForces(worldDragFactor);
  EndTurn(); // updates speed and position
 }
 void PlayerBetween( bool turning, double worldDragFactor=1.0 ) {
  BeginTurn();
  if ( !turning ) Unturn(); else {
   if ( turnDirectionDesired != turnDirection ) {
    Unturn();
    if ( turnVelocity < NAV_MIN_FORCE ) turnDirection=turnDirectionDesired;
   } else {
    Turn(worldDragFactor);
   }
  }
  if ( thrust > thrustLimit ) thrust=thrustLimit;
  if ( thrust < -thrustLimit ) thrust=-thrustLimit;
  DiminishForces(worldDragFactor);
  EndTurn(); // updates speed and position
 }
 Cartesiand *ApplyForce( double x, double y, double diminishFactor=0.5 ) {
  Cartesiand *force=new Cartesiand(x,y);
  force->w=diminishFactor;
  forces.Append(force);
  return force;
 }
 bool TravelTo( Cartesiand * target, double reached_proximity=30.0, double speedLimit=15.0 ) {
  travelTarget.Set(target);
  TurnTowards(target);
  bool nearEnough=!fleeingFrom && (ddistance(target->x,target->y,position.x,position.y) < reached_proximity);
  double confusion=1.0-intelligence;
  if ( nearEnough ) {
   if ( flyingBy ) {
   LockControls((orbitSpeed+confusion*10.0));
   } else {
    braking=true;
   }
  } else {
   double confusion=1.0-intelligence;
   accelerating = ( thrust < speedLimit+confusion*speedingLimit );
   braking = !accelerating;
  }
  return nearEnough;
 }
private:
 void BeginTurn() {
  accelerated=false;
  braked=false;
  turned=false;
 }
 void DiminishForces( double dragFactor=1.0 ) {
  EACHN(forces.first,Cartesiand,force,{
   double drag=UMIN(force->w*dragFactor,0.95);
   force->Set(force->x*drag,force->y*drag);
   if ( abs(force->x) < NAV_MIN_FORCE ) force->x=0.0;
   if ( abs(force->y) < NAV_MIN_FORCE ) force->y=0.0;
   if ( force->x == 0.0 && force->y == 0.0 ) {
    forces.Remove(force);
    delete force;
   }
  });
  double drag=UMIN(dragFactor,0.95);
  velocity.Set(velocity.x*drag,velocity.y*drag);
  if ( abs(velocity.x) < NAV_MIN_FORCE ) velocity.x=0.0;
  if ( abs(velocity.y) < NAV_MIN_FORCE ) velocity.y=0.0;
 }
 void TurnTowards( Cartesiand *target ) {
  Line line(target->x,target->y,position.x,position.y);
  double toward=rad2deg(line.LineAngle())+ (fleeingFrom ? 0.0 : 180.0);
  toward=norm_deg(toward);
//  angle=norm_deg(angle);
  double offAngle=ADIFF(toward,angle);
  slowingTurn = ( offAngle < NAV_TURN_NEARNESS );
  turningTo=toward;
  if ( toward < angle ) { turnDirectionDesired=NAV_TURN_LEFT; } // turn left
  else if ( toward > angle ) { turnDirectionDesired=NAV_TURN_RIGHT; } // turn right
  else { turnDirectionDesired=NAV_TURN_STRAIGHT; }
 }
 void Turn( double dragFactor=1.0 ) {
  turnVelocity += turnAccel * dragFactor;
  if ( turnVelocity > turnSpeedLimit ) turnVelocity=turnSpeedLimit;
  if ( turnVelocity < -turnSpeedLimit ) turnVelocity=-turnSpeedLimit;
 }
 void Unturn() {
  turnVelocity -= turnBraking;
  if ( turnVelocity <= 0.0 ) {
   turnDirection=turnDirectionDesired;
   if ( turnDirection != NAV_TURN_STRAIGHT ) turnVelocity=-turnVelocity;
   else turnVelocity=0.0;
  }
 }
 void Steer( double dragFactor=1.0 ) {
  if ( turned ) return;
  turned=true;
  double confusion=1.0-intelligence;
  confused = netGameSeed+mySeed % (101-(int)(100.0*confusion)) < 1;
  double distanceToDestination=ddistance(position.x,position.y,travelTarget.x,travelTarget.y);
  double linearVelocity=abs(ddistance(0.0,0.0,velocity.x,velocity.y)); if ( linearVelocity == 0.0 ) linearVelocity=NAV_MIN_FORCE;
  double turnFramesToDestination=distanceToDestination/linearVelocity;
  double turnFramesToStopped=linearVelocity/brakeSpeed;
  if ( turnFramesToStopped < turnFramesToDestination && !flyingBy ) {
   braking=true;
   accelerating=false;
  }
  double turnFramesToDestinationAtCurrentTurnSpeed=abs(turningTo)/(turnVelocity<NAV_MIN_FORCE?NAV_MIN_FORCE:turnVelocity)+confusion*NAV_TURNING_INTELLIGENCE_ERROR;
  double turnFramesToStopTurning=turnVelocity/turnBraking+confusion*NAV_TURNING_INTELLIGENCE_ERROR;
  if ( intelligence > 0.25 ) {
   if ( (int)turnFramesToStopTurning >= (int)turnFramesToDestinationAtCurrentTurnSpeed-1
     || linearVelocity < 2*NAV_MIN_FORCE ) {
    Unturn();
   } else {
    if ( turnDirectionDesired == turnDirection ) {
     if ( slowingTurn ) Unturn();
     else if ( turnDirectionDesired != NAV_TURN_STRAIGHT ) Turn(dragFactor); else Unturn();
    } else Unturn();
   }
  } else {
   if ( (int)turnFramesToStopTurning >= (int)turnFramesToDestinationAtCurrentTurnSpeed
     && confused ) {
    Unturn();
   } else {
    Turn(dragFactor);
   }
  }
  if ( intelligence < 0.8 && confused ) {
   accelerating = !accelerating;
   braking = !accelerating;
   slowingTurn = !slowingTurn;
   LockControls(confusion*10.0);
  }
 }
 void Accelerate() {
  if ( accelerated ) return;
  accelerated=true;
  thrust+= reversing ? -reverseSpeed : acceleration;
  if ( thrust > thrustLimit ) thrust=thrustLimit;
 }
 void Brake() {
  if ( braked ) return;
  braked=true;
  if ( thrust >= 0.0 ) {
   thrust-=brakeSpeed;
  } else {
   thrust+=brakeSpeed;
  }
  if ( abs(thrust) < brakeSpeed*2.0 ) thrust=0.0;
 }
 void CalculateThrustVector() {
  Cartesiand oldThrustVector(&thrustVector);
  Cartesiand newThrustVector(thrust,0.0);
  newThrustVector.Rotate(angle);
  if ( ddistance(oldThrustVector.x,oldThrustVector.y,newThrustVector.x,newThrustVector.y) < NAV_THRUST_FACTOR_MINIMUM )
   thrustVector.Set(&newThrustVector);
  else {
   thrustVector.Set(
    (oldThrustVector.x+newThrustVector.x)/2.0,
    (oldThrustVector.y+newThrustVector.y)/2.0
   );
  }
 }
 void ApplyForces() {
  velocity.Set(0.0,0.0);
  EACH(forces.first,Cartesiand,f) velocity.Set(velocity.x+f->x,velocity.y+f->y);
  velocity.Set(velocity.x+thrustVector.x,velocity.y+thrustVector.y);
 }
 void ApplyTurn() {
  double wasAngle=angle;
  angle+=turnVelocity * (turnDirection != NAV_TURN_STRAIGHT ? turnDirection : 1.0);
  if ( BETWEEN(turningTo,wasAngle,angle) ) angle=turningTo;
  position.x+=velocity.x;
  position.y+=velocity.y;
 }
 void EndTurn() {
  CalculateThrustVector();
  ApplyForces();
  ApplyTurn();
 }
 void MoveTo( double x, double y) {
  position.Set(x,y);
 }
 // Collision Angle is the angle of the "wall" being collided with.
 void Bounce( double collisionAngle ) {
 }
};