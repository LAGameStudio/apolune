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

#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Display.h"
#include "Interface.h"

#include "Presentation.h"

#include "TitleMenu.h"

#define SHOW 1

class TitleSequence : public Presentation {
public:
 TitleSequence( Display *d, Interface *i, Audio *a  ) : Presentation(d,i,a) { }

 void RenderPaused() {}

 void OnLoad();

 bool OnComplete();

 void RightDown();
 
 Indexed<Zfloat> xStar, yStar;
 void PreProcessing() {
  Crayon t;
  for ( unsigned int i=0; i<xStar.length; i++ ) {
   float xx=xStar[i]-(float)((int)xStar[i]);
   xStar[i]+=0.1f;
   if ( xStar[i] > display->wf ) xStar[i]=-1.0f;
   t.rf=0.5f*((float)i/200.0f); 
   t.gf=0.5f*((float)i/200.0f); 
   t.bf=0.5f+((float)i/400.0f); 
   t.af=1.0f;
   DrawPixelf(this,t,xStar[i],yStar[i]);
   //OUTPUT( "DrawPixelf(%f,%f)\n", xStar[i],yStar[i]);
  }
 }

};

extern Zpointer<TitleSequence> titles;