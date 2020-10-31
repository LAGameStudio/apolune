#pragma once

#include "FastGUI.h"
#include "GLWindow.h"

class BestRectTest : public GLWindow {
public:
 Cartesiand xy;
 Zbool first;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  first=true;
 }
 void Render() {
  if ( input->right ) {
   xy.SetRect(xy.x,xy.y,(double)input->mxi-xy.x,(double) input->myi-xy.y);
  }
  if ( input->leftReleased() ) {
   if ( first ) {
    xy.SetRect((double)input->mxi,(double) input->myi,xy.w,xy.h);
    first=false;
   } else {
    xy.SetRect(xy.x,xy.y,(double)input->mxi-xy.x,(double) input->myi-xy.y);
    first=true;
   }
  }
  int recommended=fast.bestrectforaspect(xy.w,xy.h);
  art.Element2(crayons.Pick(red255),crayons.jami(0,127,0,0,0),recommended,xy.x,xy.y,xy.w,xy.h);
  crayons.Pick(alabaster).gl();
  Blending(none);
  Text(I2S(recommended),64.0,64.0,14.0,16.0,false);
 }
};

