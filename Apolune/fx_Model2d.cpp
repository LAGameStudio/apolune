
/***********************************************************************************************
 * OpenGL Window and Animation Library (formerly the Open Art Engine)                          *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once

#include <Windows.h>
#include <math.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "TexFont.h"

#include "fx_Object3d.h"
#include "fx_Model2d.h"

#include "ModelEdit.h"
#include "ComponentEdit.h"

extern TexFont microFont;
extern Interface input;
extern GLWindowManager windows;

void fx_Model2d::Draw(GLWindow *surface) {
 Crayon t;
 if ( hover ) t.Pick(alabaster);
 else t.Pick(cyan);
 Blending(transparency);
 Area(surface,background,x,y,x2,y2);
 Blending(additive);
 Rectangle(surface, t, x,y,midx,y2);
 Rectangle(surface, t,midx,y,x2,y2);
 if ( component ) {
  component->matters.RenderView2d( TOP, x,y,midx,y2 );
  component->matters.RenderView2d( LEFT, midx,y,x2,y2 );
  WriteTexFont(surface,&microFont,crayons.Pick(alabaster),additive,x+5,y+5,7,16,2,"Top");
  WriteTexFont(surface,&microFont,crayons.Pick(alabaster),additive,midx+5,y+5,7,16,2,"Side");
 }
}

void fx_Model2d::LeftDown(void) {
 MousePosition(&mx,&my);
 hover=within(mx,my);
 if ( !hover) return;
 if ( toEditor ) {
  modelEdit->SetComponent(this->component);
  modelEdit->visible=true;
  modelEdit->techtypes->addingTech=false;
  componentEdit->visible=false;
  modelEdit->namebox->SetValue(componentEdit->namebox->out);
  windows.stopDrill=true;
 } else {
 }
}

void fx_Model2d::MouseMove() {
 MousePosition(&mx,&my);
 hover=within(mx,my);
}