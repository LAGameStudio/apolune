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

#include "GLWindow.h"
#include "STLLibrary.h"
#include "VBOGroup.h"

class STLLibraryBrowser : public GLWindow {
public:
 VBONormalsPolysGroup vbos;
 VBONormalsPolysGroupHandles handles;
 Zpointer<STLFile> selected;
 Zdouble keyDelay;
 LookAtPerspective lap;
 Cartesiand position,rotation;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  EACH(stls.first,STLFile,stl) {
   stl->vboGroup = &vbos;
  }
  selected=(STLFile *)stls.last;
  RecomputeVBOs();
 }
 void RecomputeVBOs() {
  vbos.Clear();
  selected->toVBOGroup(&vbos);
 }
 void Between() {
  if ( input->KeyDown(LAlt) ) {
   rotation.z=360.0f*(input->mxi-display->w2)/display->wf;
  } else {
   rotation.x=360.0f*(input->myi-display->h2)/display->hf;
   rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  }
  if ( keyDelay > 0.0f ) keyDelay-=(float)FRAMETIME;
  else {
   // Catalog selection
   if ( input->KeyDown(Spacebar) || input->KeyDown(NumpadDecimal) ) { position.Set(0,0); }
   if ( input->KeyDown(LessSign) ) { /// Next item
    if ( selected->next ) selected=(STLFile *)selected->next;
    else selected=(STLFile *)stls.first;
    RecomputeVBOs();
   } else
   if ( input->KeyDown(GreaterSign) ) { /// Previous item
    if ( selected->prev ) selected=(STLFile *)selected->prev;
    else selected=(STLFile *)stls.last;
    RecomputeVBOs();
   }
   // Positional adjustments
   if ( input->KeyDown(DX_W) ) {
    position.z+=0.1;
   } else
   if ( input->KeyDown(DX_S) ) {
    position.z-=0.1;
   } else
   if ( input->KeyDown(DX_A) ) {
    position.x+=0.1;
   } else
   if ( input->KeyDown(DX_D) ) {
    position.x-=0.1;
   } else
   if ( input->KeyDown(DX_Q) ) {
    position.y+=0.1;
   } else
   if ( input->KeyDown(DX_E) ) {
    position.y-=0.1;
   }
   // Camera zoom
   if ( input->KeyDown(NumpadPlus) ) { lap.eye.Set(0.0,0.0,lap.eye.z/2); keyDelay=0.25f; }
   if ( input->KeyDown(NumpadMinus) ) { lap.eye.Set(0.0,0.0,lap.eye.z*2); keyDelay=0.25f; }   
   // Arrow keys (adjust camera orbit)
   if ( input->KeyDown(Left) ) {
   } else
   if ( input->KeyDown(Right) ) {
   } else
   if ( input->KeyDown(Up) ) {
   } else
   if ( input->KeyDown(Down) ) {
   }
  }
 }
 void Render() {
  if ( selected ) {
   selected->VBOGroupRender(&lap,&position,&rotation,16,16,800,600.0);
   crayons.Pick(tangelo).gl();
   glLineWidth(2);
   Text(selected->filename.c_str(),16.0,600.0+32.0,8.0,12.0,false);
   glLineWidth(1);
  }
 }
};