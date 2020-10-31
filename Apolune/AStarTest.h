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
#include "Art.h"
#include "GLWindow.h"
#include "AStar.h"
#include "UglyFont.h"

class AStarTest : public GLWindow {
public:
 Cartesian start,end;
 Cartesians *path;
 AStarPathfinder astar;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  astar.map.resize(100,100);
  for ( int i=20; i<80; i++ ) astar.map(i,50).traversible=false;
  start.Set(35,15);
  end.Set(55,55);
  path=null;
  Recalculate();
 }
 void Recalculate() {
  if ( path ) delete path;
  path=astar.FindPath(start.x,start.y,end.x,end.y);
  DrawOnFBO();
 }
 void DrawOnFBO() {
  fboScreen2.Bind();
  for ( int i=0; i<100; i++ ) for ( int j=0; j<100; j++ ) {
   if ( i==start.x && j==start.y ) Area(crayons.Pick(green),i*10,j*10,i*10+10,j*10+10);
   else 
   if ( i==end.x && j==end.y ) Area(crayons.Pick(red255),i*10,j*10,i*10+10,j*10+10);
   else {
    bool p=false;
    EACH(path->first,Cartesian,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(purple),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    EACH(astar.visited.first,AStarCell,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(vermillion),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    EACH(astar.open.first,AStarCell,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(orange),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    if ( astar.map(i,j).traversible ) Area(crayons.Pick(black),i*10,j*10,i*10+10,j*10+10);
    else Area(crayons.Pick(blue),i*10,j*10,i*10+10,j*10+10);
   }
  }
  int i=0;  
  EACH(path->first,Cartesian,c) {
   i++;
   glColor3i(0,0,0);
   char buf[10];
   FORMAT(buf,10,"%d",(int)i);
   Text(buf,(double)(c->x*10+5),(double)(c->y*10+5),5,5,true);
  }
  fboScreen2.Unbind();
 }
 void Render() {
  fboScreen2.RenderUpsideDown(x,y,x2,y2);
 }
 void LeftUp() {
  start.x=input->mxi/10;
  start.y=input->myi/10;
  Recalculate();
 }
 void RightUp() {
  end.x=input->mxi/10;
  end.y=input->myi/10;
  Recalculate();
 }
 void Between() {
  if ( input->middle ) {
   int i=input->mxi/10;
   int j=input->myi/10;
   astar.map(i,j).traversible=false;
  }
 }
 void WheelDown() {
  int i=input->mxi/10;
  int j=input->myi/10;
  astar.map(i,j).traversible=false;
 }
 void WheelUp() {
  int i=input->mxi/10;
  int j=input->myi/10;
  astar.map(i,j).traversible=true;
 }
 void MiddleUp() {
  Recalculate();
 }
};

class AStarThreadedTest;
class TestAStarThread : public AStarThread {
public:
 AStarThreadedTest *win;
 TestAStarThread(AStarThreadedTest *win, AStarPathfinder *astar, int x, int y, int x2, int y2);
 void OnComplete();
};

class AStarThreadedTest : public GLWindow {
public:
 Cartesians *path;
 bool doneCalculating,needsCalculated,needsRedrawn;
 Cartesian start,end;
 AStarPathfinder astar;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  doneCalculating=true;
  needsCalculated=true;
  needsRedrawn=false;
  path=null;
  start.x=5;
  start.y=5;
  end.x=95;
  end.y=95;
  astar.map.resize(100,100);
  // Draws a box-like region that has a section in which there is no possible path from out to in.
  for ( int i=20; i<80; i++ ) astar.map(i,50).traversible=false;  
  for ( int i=20; i<80; i++ ) astar.map(i,70).traversible=false;  
  for ( int i=50; i<70; i++ ) {
   astar.map(50,i).traversible=false;
   astar.map(70,i).traversible=false;  
  }
 }
 void Recalculate() {
  if ( needsCalculated && doneCalculating ) {
   doneCalculating=false;
   pthreads.New(new TestAStarThread(this,&astar,start.x,start.y,end.x,end.y));
   needsCalculated=false;
  }
 }
 void Between() {
  if ( needsCalculated ) Recalculate();
  if ( doneCalculating && input->middle ) {
   int i=input->mxi/10;
   int j=input->myi/10;
   astar.map(i,j).traversible=false;
  }
 }
 void Render() {
  if ( needsRedrawn ) DrawOnFBO();
  fboScreen2.RenderUpsideDown(x,y,x2,y2);
 }
 void DrawOnFBO() {
  needsRedrawn=false;
  fboScreen2.Bind();
  for ( int i=0; i<100; i++ ) for ( int j=0; j<100; j++ ) {
   if ( i==start.x && j==start.y ) Area(crayons.Pick(green),i*10,j*10,i*10+10,j*10+10);
   else 
   if ( i==end.x && j==end.y ) Area(crayons.Pick(red255),i*10,j*10,i*10+10,j*10+10);
   else {
    bool p=false;
    EACH(path->first,Cartesian,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(purple),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    EACH(astar.visited.first,AStarCell,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(vermillion),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    EACH(astar.open.first,AStarCell,c) if ( c->x==i && c->y==j ) {
     Area(crayons.Pick(orange),i*10,j*10,i*10+10,j*10+10);
     p=true;
    }
    if ( p ) continue;
    if ( astar.map(i,j).traversible ) Area(crayons.Pick(black),i*10,j*10,i*10+10,j*10+10);
    else Area(crayons.Pick(blue),i*10,j*10,i*10+10,j*10+10);
   }
  }
  int i=0;  
  EACH(path->first,Cartesian,c) {
   i++;
   glColor3i(0,0,0);
   char buf[10];
   FORMAT(buf,10,"%d",(int)i);
   Text(buf,(double)(c->x*10+5),(double)(c->y*10+5),5,5,true);
  }
  fboScreen2.Unbind();
 }
 void LeftUp() {
  if ( !doneCalculating ) return;
  start.x=input->mxi/10;
  start.y=input->myi/10;
  needsCalculated=true;
 }
 void RightUp() {
  if ( !doneCalculating ) return;
  end.x=input->mxi/10;
  end.y=input->myi/10;
  needsCalculated=true;
 }
 void WheelDown() {
  if ( !doneCalculating ) return;
  int i=input->mxi/10;
  int j=input->myi/10;
  astar.map(i,j).traversible=false;
  needsCalculated=true;
 }
 void WheelUp() {
  if ( !doneCalculating ) return;
  int i=input->mxi/10;
  int j=input->myi/10;
  astar.map(i,j).traversible=true;
  needsCalculated=true;
 }
 void MiddleUp() {
  needsCalculated=true;
 }
};