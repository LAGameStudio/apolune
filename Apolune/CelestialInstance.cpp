#include "CelestialInstance.h"

#include "Player.h"

void CelestialInstance::ScaleTowardPlayer() {
 playerDistance=ddistance(position.x,position.y,player->spaceGameView->lp->x,player->spaceGameView->lp->y);
 double r1=position.w*2.0*(1.0-(playerDistance/2048.0));
 double r2=position.h*2.0*(1.0-(playerDistance/2048.0));
 gravityDrawArea.SetRect(position.CenterX()-r1,position.CenterY()-r2,r1*2.0,r2*2.0);
}

void CelestialInstance::GravityHazard() {
 if ( !player->spaceGameView ) return;
 if ( playerDistance < 512.0 ) {
  if ( !player->spaceGameView->screenShaking ) {
   double dratio=(playerDistance/512.0);
   player->spaceGameView->ShakeScreen(10.0*(double)(1.0-playerDistance/512.0),3);
   if ( dratio > 0.1 && dratio < 0.5 ) {
    player->vehicle->Hurt(0.05*dratio);
    if ( player->vehicle->health <= 0.0 ) player->Killed(FORMAT("Your ship was incinerated by %s",source->name.c_str()).c_str());
   } else if ( dratio < 0.1 ) {
    player->Killed(FORMAT("Your ship was incinerated by %s",source->name.c_str()).c_str());
   }
  }
 }
}
