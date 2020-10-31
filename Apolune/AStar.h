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

#include "macros.h"
#include "moremath.h"
#include "Cartesian.h"

extern int astar_unique_id;

/* A* pathfinder algorithm
 Implementation by H. Elwood Gilliland III
 (c) 2012 All rights reserved.
 Notes: http://www.policyalmanac.org/games/twoTiered.htm
 Helped: http://www.youtube.com/watch?v=NJOf_MYGrYs
*/


class AStarMapNode : public ListItem {
public:
 double cost;
 bool traversible;
 AStarMapNode() {
  traversible=true;
  cost=0.0;
 }
};

class AStarMapNodeHandle : public ListItem {
public:
 AStarMapNode *node;
 AStarMapNodeHandle( AStarMapNode *a ) {
  node=a;
 }
};

class AStarCell : public ListItem {
public:
 Zpointer<AStarCell> parent;
 Zint x,y;
 Zint id;
 Zdouble G; // accumulated distance from start + current
 Zdouble H; // heuristic (estimated distance to goal)
 AStarCell() {
  astar_unique_id++; id=astar_unique_id;
 }
 AStarCell( int x, int y, AStarCell *parent ) {
  this->x=x;
  this->y=y;
  astar_unique_id++; id=astar_unique_id;
  this->parent=parent;
 }
 double F() { return G.value+H.value; }
 double Manhattan( AStarCell *end ) {
  double dx=abs((double)(this->x-(int)end->x));
  double dy=abs((double)(this->y-(int)end->y));
  return dx+dy;
 }
};

class AStarCells : public LinkedList {
public:
 CLEARLISTRESET(AStarCell);
};

class AStarMapNodeHandles : public LinkedList {
public:
 CLEARLISTRESET(AStarMapNodeHandle);
};

class AStarPathfinder : public ListItem {
public:
 matrix2d<AStarMapNode> map;
 bool diagonal;
 bool found;
 double closeness;
 AStarCell goal;
 AStarCells open,visited;
 AStarPathfinder() {
  closeness=1.0;
  found=false;
  diagonal=true;
  start=null;
  path=null;
 }
 AStarCell *start;
 Cartesians *path;
 Cartesians *FindPath( int x, int y, int x2, int y2 ) {
  path=new Cartesians;
  open.Clear();
  visited.Clear();
  found=false;
  start=new AStarCell(x,y,null);
  goal.x=x2;
  goal.y=y2;
  start->G=0;
  start->H=start->Manhattan(&goal);
  open.Append(start);
  int max_iterations=map.M()*map.N();
  while ( !found && max_iterations > 0 ) {
   Iterate();
   max_iterations--;
  }
  return path;
 }
 void Iterate() {
  if ( open.count == 0 ) return;
  AStarCell *a=GetNext();
  if ( a->x == goal.x && a->y == goal.y ) {
   found=true;
   goal.parent=a->parent;
   AStarCell *p=&goal;
   while ( p ) {
    path->Prepend(new Cartesian(p->x,p->y));
    p=p->parent;
   }
  } else {
   Test(a->x+1, a->y, a->G+1.0, a);
   Test(a->x-1, a->y, a->G+1.0, a);
   Test(a->x, a->y+1, a->G+1.0, a);
   Test(a->x, a->y-1, a->G+1.0, a);
   if ( diagonal ) {
    Test(a->x+1, a->y+1, a->G+SQRTOF2, a);
    Test(a->x-1, a->y+1, a->G+SQRTOF2, a);
    Test(a->x+1, a->y-1, a->G+SQRTOF2, a);
    Test(a->x-1, a->y-1, a->G+SQRTOF2, a);
   }
   EACHN(open.first,AStarCell,o,{
    if ( a->x==o->x && a->y==o->y ) {
     open.Remove(o);
     delete o;
    }
   });
  }
 }
private:
 AStarCell * GetNext() {
  double bestF=999999999.0;
  AStarCell *index=null;
  EACH(open.first,AStarCell,o) {
   double F=o->F();
   if ( F < bestF ) {
    bestF=F;
    index=o;
   }
  }
  if ( index ) {
   open.Remove(index);
   visited.Append(index);
   return index;
  }
  return null;
 }
 void Test(int x, int y, double newCost, AStarCell *parent) {
  if ( x < 0 || x >= map.M() ) return;
  if ( y < 0 || y >= map.N() ) return;
  if ( !map(x,y).traversible ) return;
  newCost+=map(x,y).cost;
  EACH(visited.first,AStarCell,v) if ( v->x==x && v->y==y ) return;
  AStarCell *a=new AStarCell(x,y,parent);
  a->G=newCost;
  a->H=parent->Manhattan(&goal);
  EACH(open.first,AStarCell,o) if ( o->x==x && o->y==y ) {
   double newF = a->G + newCost + o->H;
   double F=o->F();
   if ( F > newF) {
    o->G=a->G + newCost;
    o->parent=a;
   } else {
    delete a;
    return;
   }
  }
  open.Append(a);
 }
};

#include "pThread.h"

class AStarThread : public pThread {
public:
 bool done;
 AStarPathfinder *astar;
 int x,y,x2,y2;
 Cartesians *path;
 AStarThread( AStarPathfinder *astar, int x, int y, int x2, int y2 ) : pThread() {
  this->astar=astar;
  done=false;
  this->x=x;
  this->y=y;
  this->x2=x2;
  this->y2=y2;
  path=null;
 }
 void Search() { path=astar->FindPath(x,y,x2,y2); }
 virtual void OnComplete() {}
 void Main() {
  Search();
  done=true;
  pthreads.Lock();
  OnComplete();
  pthreads.Unlock();
 }
};