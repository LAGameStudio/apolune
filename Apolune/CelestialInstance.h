/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "GameObject.h"
#include "PlanetProfile.h"
#include "ScriptSection.h"

ONE(CelestialFragment,{
 tint.Pick(alabaster);
 drag=1.0;
})
 Zp<PlanetProfile> source;
 Zdouble spin,angle,mass,drag;
 Crayon tint;
 Cartesiand position,velocity;
 void MovedBy( double x, double y ) {
  position.MoveBy(x,y);
 }
 void Between() {
  angle+=spin;
  position.x+=velocity.x;
  position.y+=velocity.y;
  velocity.x*=drag; if ( velocity.x < 0.01 ) velocity.x=0.0;
  velocity.y*=drag; if ( velocity.y < 0.01 ) velocity.y=0.0;
 }
MANY(CelestialFragment,CelestialFragmentHandle,CelestialFragmentHandles,"",CelestialFragments,{})
 void MovedBy( double x, double y ) { FOREACH(CelestialFragment,cf) cf->MovedBy(x,y); }
 CALLEACH(CelestialFragment,Between)
DONE(CelestialFragment);

#define RTID_CelestialInstance 113
RC_ONE(CelestialInstance,RTID_CelestialInstance,GameObject,{
 tint.Pick(alabaster);
 drag=0.0;
})
 Zp<PlanetProfile> source;
// Zp<LocationMarker> placement;
 CelestialFragments fragments;
 Cartesiand position;
 Crayon tint;
 Zdouble enlargement;
 Zdouble time,drag;
 Zbool canBeStruck,mainDestroyed;
 ScriptSectionHandles _hailed,_died,_activated,_locked,_unlocked,_exit,_enter,_land,_read,_upload,_download,_use,_touched;
 int CentroidX() { return (int)(position.x+position.w/2.0); }
 int CentroidY() { return (int)(position.y+position.h/2.0); }
 ///
 void Assign( PlanetProfile * pp ) {
  source=pp;
  switch ( source->size ) {
    case pprs_512: source->fbo=&fboCDS512; break;
   case pprs_1024: source->fbo=&fboCDS1024; break;
 //  case pprs_2048: fbo=&fboCDS2048; break; 
          default: break;
  }
  position.SetRect(position.x-source->fbo->w/2,position.y-source->fbo->h/2,source->fbo->w,source->fbo->w);
 }
 void CheckScriptSections() {
  _hailed.Clear();    scriptSections.Sections("hailed",&_hailed); scriptSections.Sections("hail",&_hailed); scriptSections.Sections("talk",&_hailed);
  _died.Clear();      scriptSections.Sections("dies",&_died,true); scriptSections.Sections("died",&_died,true);
  _activated.Clear(); scriptSections.Sections("activated",&_activated); scriptSections.Sections("activate",&_activated);
  _locked.Clear();    scriptSections.Sections("locked",&_locked);     scriptSections.Sections("lock",&_locked);
  _unlocked.Clear();  scriptSections.Sections("unlocked",&_unlocked); scriptSections.Sections("unlock",&_unlocked);
  _exit.Clear();      scriptSections.Sections("exit",&_exit);
  _enter.Clear();     scriptSections.Sections("enter",&_enter);
  _land.Clear();      scriptSections.Sections("land",&_land);
  _read.Clear();      scriptSections.Sections("read",&_read);
  _upload.Clear();    scriptSections.Sections("upload",&_upload);
  _download.Clear();  scriptSections.Sections("download",&_download);
  _use.Clear();       scriptSections.Sections("use",&_use);
  _touched.Clear();   scriptSections.Sections("touched",&_touched); scriptSections.Sections("touch",&_touched);
  lockable = (_locked.count > 0 || _unlocked.count > 0);
  locked = lockable;
 }
 void Fragment( double massWH1, double forceV, double angle ) {
  CelestialFragment *cf=new CelestialFragment;
  cf->position.SetRect( position.x, position.y, massWH1*position.w, massWH1*position.h );
  cf->mass=massWH1;
  cf->velocity.Set(forceV,0.0);
  cf->velocity.Rotate(angle);
  cf->tint.fromCrayon(tint);
  cf->tint.Smear();
  cf->drag=drag;
 }
 void MovedBy( double x, double y ) {
  fragments.MovedBy(x,y);
  position.MoveBy(x,y);
 }
 void GravityHazard();
 Cartesiand gravityDrawArea;
 Zdouble playerDistance;
 void ScaleTowardPlayer();
 void Between() {
  fragments.Between();
  time+=FRAMETIME;
  x=(int)position.x;
  y=(int)position.y;
  ScaleTowardPlayer();
  if ( source->stellarScale ) GravityHazard();
 }
 bool OnScreen() {
  if ( gravityDrawArea.x > display.wd
    || gravityDrawArea.y > display.hd
    || gravityDrawArea.x2 < 0
    || gravityDrawArea.y2 < 0 ) {} else return true;
  EACH(fragments.first,CelestialFragment,f)
   if ( WITHIN(f->position.x,f->position.y,0.0,0.0,display.wd,display.hd) 
     || WITHIN(f->position.x,f->position.y2,0.0,0.0,display.wd,display.hd)
     || WITHIN(f->position.x2,f->position.y,0.0,0.0,display.wd,display.hd)
     || WITHIN(f->position.x2,f->position.y2,0.0,0.0,display.wd,display.hd) ) return true;
  return false;
 }
 void Render() {
  if ( !source ) return;
  if ( !OnScreen() ) return;
  source->fbo->background.Pick(clear);
  source->SetTime(time/10.0);
  source->Render();
  if ( !mainDestroyed ) {
   source->fbo->RenderUpsideDown(tint,gravityDrawArea.x,gravityDrawArea.y,gravityDrawArea.x2,gravityDrawArea.y2);
   //else source->fbo->RenderUpsideDown(tint,position.x,position.y,position.x2,position.y2);
  }
//  source->Render();???
  EACH(fragments.first,CelestialFragment,frag) {
   source->fbo->RenderRotatedRect(frag->tint,frag->position.x,frag->position.y,frag->position.x2,frag->position.y2,frag->angle);
  }
 }
 void Render( int x, int y, int w, int h ) {
  if ( !source ) return;
  source->fbo->background.Pick(clear);
  source->SetTime(time/10.0);
  source->Render();
  if ( !mainDestroyed ) {
   source->fbo->RenderUpsideDown(tint,x,y,x+w,y+h);
  }
 }
 void Render( Crayon tint, int x, int y, int w, int h ) {
  if ( !source ) return;
  source->fbo->background.Pick(clear);
  source->SetTime(time/10.0);
  source->Render();
  if ( !mainDestroyed ) {
   source->fbo->RenderUpsideDown(tint,x,y,x+w,y+h);
  }
 }
MANY(CelestialInstance,CelestialInstanceHandle,CelestialInstanceHandles,"",CelestialInstances,{})
DONE(CelestialInstance);