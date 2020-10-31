#include "SystemViewer.h"

#include "QuadrantViewer.h"
#include "StarShip.h"
#include "Player.h"

/*
void CoursePlotButton::WhenClicked() {
 windows.stopDrill=true;
 if ( players.active->profile.active->vessel 
   && players.active->profile.active->vesselType == spacecraft ) {
  Starship *ship=(Starship *) players.active->profile.active->vessel;
  ship->course=system;
  audio.manager.CueAndPlayIfNotPlaying("data/sounds/fx/beep1.wav",1.0f,1.0f,false,true);
 }
}*/

void SystemViewer::RightDown() {
 quadrantViewer->systemViewers.Drop(this);
 manager->stopDrill=true;
 //this.Destroy(0); 
 return; 
}

void SystemViewer::MouseMove() {
 //quadrantViewer.sunScreen.visible=true;
 if ( quadrantViewer->dragging ) { 
  quadrantViewer->MouseMove();  /*OnMouseMove(x+position->x,y+position->y,mods);*/
  return;
 } else
 if ( dragging ) {
  int mx=input->mxi-this->x;
  int my=input->myi-this->y;
  diffx=mx-lx; diffy=my-ly;
  mx+=diffx;
  my+=diffy;
  lx=mx;
  ly=my;
 }
}