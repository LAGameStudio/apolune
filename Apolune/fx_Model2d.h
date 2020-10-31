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

#include "GLWindow.h"
#include "GLImage.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Art.h"
#include "Audio.h"

#include "Proce55Controller.h"
#include "Proce55or.h"

#include "Model.h"
#include "Technology.h"

class fx_Model2d : public Proce55or {
public:
 Zint mx,my;
 Zbool hover;
 Zbool Top;
 Zbool toEditor;
 Crayon background,lines;
 Zpointer<COMPONENT> component;
 Zint midx,w2;
 void Setup() {
  x2=x+w;
  y2=y+h;
  w2=w/2;
  midx=x+w2;
  toEditor=true;
 }
 void Draw(GLWindow *surface);
 void LeftDown();

 void MouseLeave() { hover=false; }
 void MouseMove();

};
