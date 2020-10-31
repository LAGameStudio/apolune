#pragma once

#include "GLWindow.h"
#include "GridSplitter.h"
#include "Delay.h"

class GridSplitterTest : public GLWindow {
public:
 GridSplits gs;
 Zdisposable<GridSplitter> splitter;
 Cartesian size;
 Delay inputDelay;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  size.SetRect(x,y,w,h);
  gs=gs_Unknown;
  splitter.Recycle(new GridSplitter(gs,size.x,size.y,size.w,size.h));
  inputDelay.Set(0.2f);
 }
 void Render() {
  Rectangle(crayons.Pick(blue255),-1,-1,input->mxi+1,input->myi+1);
  splitter->DrawDebug();
  Rectangle(crayons.Pick(orange),
   splitter->outer.dx()-1,splitter->outer.dy()-1,
   splitter->outer.dx()+splitter->outer.dw()+2,
   splitter->outer.dy()+splitter->outer.dh()+2
  );
  Text(FORMAT("Type: %s (%d)",GridSplitsName(gs).c_str(),(int)gs).c_str(),16.0,16.0,8.0,11.0,false);
 }
 void Between() {
  inputDelay.Between();
  if ( input->left ) {
   size.SetRect(input->mxi,input->myi,size.w,size.h);
   splitter.Recycle(new GridSplitter(gs,size.x,size.y,size.w,size.h));
  } else if ( input->right ) {
   size.SetRect(size.x,size.y,input->mxi,input->myi);
   splitter.Recycle(new GridSplitter(gs,size.x,size.y,size.w,size.h));
  }
  if ( !inputDelay ) {
   if ( input->KeyDown(DX_UP) ) {
    splitter.Recycle();
    splitter->Best((int)gs);
   } else if ( input->KeyDown(DX_LEFT) ) {
    inputDelay.Reset();
    int newvalue=(int)gs-1;
    if ( newvalue < 1 ) newvalue=(int)MAX_GridSplits;
    gs=(GridSplits) newvalue;
    splitter.Recycle(new GridSplitter(gs,size.x,size.y,size.w,size.h));
   } else if ( input->KeyDown(DX_RIGHT) ) {
    inputDelay.Reset();
    int newvalue=(int)gs+1;
    if ( newvalue > (int) MAX_GridSplits ) newvalue=1;
    gs=(GridSplits) newvalue;
    splitter.Recycle(new GridSplitter(gs,size.x,size.y,size.w,size.h));
   }
  }
 }
};

