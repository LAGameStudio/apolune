/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include <Windows.h>
#include <stdio.h>
#include "macros.h"
#include "moremath.h"
#include "Cartesian.h"

class GLSetup;

struct Display {
public:
 int w,h;
 Cartesian windowOffset;
 Cartesian viewport;
 double wd,hd,w2d,h2d;
 float wf,hf,w2f,h2f;
 int midx,midy,w2,h2;
 float aspect,inverseaspect;
 int bits;
 HWND hwnd;
 HINSTANCE hinstance;
 HDC hdc;
 HGLRC hglrc;
 GLSetup *gl;
 Cartesian virtualResolution;
 Cartesiand virtualRatio,inverseVirtualRatio;
 Cartesian screen;
 Cartesiand screend;
 bool probablyMatroxTripleHead2Go,probablyMatroxDualMode,probablyMatroxTripleHead;
 void Constructor() {
  w=h=midx=midy=w2=h2=0;
  bits=8;
  wd=hd=0.0;
  w2d=h2d=0.0;
  wf=hf=0.0f;
  w2f=h2f=0.0f;
  aspect=4.0f/3.0f;
  inverseaspect=3.0f/4.0f;
  hwnd=NULL;
  hinstance=NULL;
  hdc=NULL;
  hglrc=NULL;
  gl=null;
  probablyMatroxDualMode=probablyMatroxTripleHead=probablyMatroxTripleHead2Go=false;
 }
 Display(void) { Constructor(); }
 Display(int ww, int hh, int bpp ) {
  Constructor();
  bits=bpp;
  Resized(ww,hh);
 }
 void SetWindowOffsets(int addWidth, int addHeight) {
  windowOffset.Set(addWidth,addHeight);
 }
 void Virtual( int ww, int hh ) {
  virtualResolution.SetRect(0,0,ww,hh);
  virtualRatio.Set(iratiod(ww,w),iratiod(hh,h));
  inverseVirtualRatio.Set(iratiod(w,ww),iratiod(h,hh));
 }
 void VirtualScale2d();
 void UnVirtualScale2d();
 void Set2DViewport( double DPI=1.0 );
 void Resized( int xx, int yy, int ww, int hh, bool updateWindowManager=true );
 void Resized( int ww, int hh, bool updateWindowManager=true );
 bool MakeCurrent() {
 	if(!wglMakeCurrent(hdc, hglrc))	{
 		OUTPUT("Display:MakeCurrent() is unable to make Display the current context");
 		return false;
 	} 
 	return true;
 }
 void CommandLine(); // For forcing modes on the window
 void CommandLineAlt();  // For forcing modes on the virtual display
 //Save a screenshot
 void SaveScreenshot();
~Display(void) {}
};

extern double windowDisplayMouseRatioX, windowDisplayMouseRatioY;

void SetDisplayWindowMouseRatio(Display *display,Display *screenWindow);

extern Display display;
extern Display *displayPointer;