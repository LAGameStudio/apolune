#pragma once

#include "Proce55ors.h"
#include "ZIndexed.h"

// Instead of moving all proce55ors in the grid, we move a single variable, then poll
// this grid.  Whenever a proce55or moves, it reassigns itself to a new grid position.
// We only draw and update proce55ors within the active region, defined based on the
// player position (or moved position).

#define PROCE55ORS_GRID_DIM      (200)
#define PROCE55ORS_GRID_HALF_DIM (100)
#define PROCE55ORS_GRID_PIXELS   (2048)
#define PROCE55ORS_GRID_CENTER   ((PROCE55ORS_GRID_DIM*PROCE55ORS_GRID_PIXELS)/2)

ONE(Proce55orsGrid,{ 
 fx.Size((size_t)PROCE55ORS_GRID_DIM,(size_t)PROCE55ORS_GRID_DIM);
})
 Cartesian moved;
 ZIndexed<Proce55ors> fx;
 Proce55ors left,upLeft,up,upRight,right,downRight,down,downLeft;    // Outer limits..
 int ScreenToGrid( int x ) {
  return (x-PROCE55ORS_GRID_CENTER)/PROCE55ORS_GRID_PIXELS+PROCE55ORS_GRID_HALF_DIM;
 }
 void Clear() {
  left.Clear();
  upLeft.Clear();
  up.Clear();
  upRight.Clear();
  right.Clear();
  downRight.Clear();
  down.Clear();
  downLeft.Clear();
  for ( int i=0; i<PROCE55ORS_GRID_DIM*PROCE55ORS_GRID_DIM; i++ ) fx[i].Clear();
 }
 void Add( Proce55or *p, GLWindow *parent=null ) {
  p->parent=parent;
  UpdateProce55orGridPosition(p);
 }
 void Remove( Proce55or *p ) {
  if ( p->zone.left ) left.Remove(p);
  else if ( p->zone.right ) right.Remove(p);
  else if ( p->zone.up ) up.Remove(p);
  else if ( p->zone.down ) down.Remove(p);
  else if ( p->zone.upLeft ) upLeft.Remove(p);
  else if ( p->zone.downLeft ) downLeft.Remove(p);
  else if ( p->zone.downRight ) downRight.Remove(p);
  else if ( p->zone.upRight ) upRight.Remove(p);
  else fx[p->zone.x,p->zone.y].Remove(p);
 }
 void Drop( Proce55or *p ) {
  if ( p->zone.unzoned ) delete p;
  else {
   Remove(p);
   delete p;
  }
 }
 void UpdateProce55orGridPosition( Proce55or *p ) { // Call after a proce55or has moved (happens before Between() in Between() below)
  int gridX=ScreenToGrid(p->x);
  int gridY=ScreenToGrid(p->y);
  if ( p->zone.unzoned || p->zone.x != gridX || p->zone.y != gridY ) {
   if ( !p->zone.unzoned ) Remove(p);
   p->zone.Set(gridX,gridY);
   if ( gridX < 0 ) {
    if ( gridY < 0 ) { p->zone.upLeft=true; upLeft.Add(p,p->parent); }
    else if ( gridY < PROCE55ORS_GRID_DIM ) { p->zone.left=true; left.Add(p,p->parent); }
    else { p->zone.downLeft=true; downLeft.Add(p,p->parent); }
   } else if ( gridX >= PROCE55ORS_GRID_DIM ) {
    if ( gridY < 0 ) { p->zone.upRight=true; upRight.Add(p,p->parent); }
    else if ( gridY < PROCE55ORS_GRID_DIM ) { p->zone.right=true; right.Add(p,p->parent); }
    else { p->zone.downRight=true; downRight.Add(p,p->parent); }
   } else {
    if ( gridY < 0 ) { p->zone.down=true; down.Add(p,p->parent); }
    else if ( gridY >= PROCE55ORS_GRID_DIM ) { p->zone.up=true; up.Add(p,p->parent); }
    else fx[p->zone.x,p->zone.y].Add(p,p->parent);
   }
  }
 }
 void GetActive( int x, int y, int w, int h, Proce55orsHandles *out ) {
  out->Clear();
  int startX=ScreenToGrid(x);
  int endX=startX+(w/PROCE55ORS_GRID_PIXELS)+1;
  int startY=ScreenToGrid(y);
  int endY=startY+(h/PROCE55ORS_GRID_PIXELS)+1;
  int clamped_startX=URANGE(0,startX,PROCE55ORS_GRID_DIM-1);
  int clamped_startY=URANGE(0,startY,PROCE55ORS_GRID_DIM-1);
  int clamped_endX=URANGE(0,startY,PROCE55ORS_GRID_DIM-1);
  int clamped_endY=URANGE(0,endY,PROCE55ORS_GRID_DIM-1);
  for ( int i=clamped_startX; i<clamped_endX; i++ )
   for ( int j=clamped_startY; j<clamped_endY; j++ )
    out->Add(&fx[i,j]);
  if ( startX < 0 ) {
   if ( startY < 0 ) out->Add(&upLeft);
   else if ( startY > PROCE55ORS_GRID_DIM-1 ) out->Add(&downLeft);
   else out->Add(&left);
  } else if ( startX > PROCE55ORS_GRID_DIM-1 ) {
   if ( startY < 0 ) out->Add(&upRight);
   else if ( startY > PROCE55ORS_GRID_DIM-1 ) out->Add(&downRight);
   else out->Add(&right);
  } else {
   if ( startY < 0 ) out->Add(&up);
   else if ( startY > PROCE55ORS_GRID_DIM-1 ) out->Add(&down);
  }
  if ( endX < 0 ) {
   if ( endY < 0 ) out->Add(&upLeft);
   else if ( endY > PROCE55ORS_GRID_DIM-1 ) out->Add(&downLeft);
   else out->Add(&left);
  } else if ( endX > PROCE55ORS_GRID_DIM-1 ) {
   if ( endY < 0 ) out->Add(&upRight);
   else if ( endY > PROCE55ORS_GRID_DIM-1 ) out->Add(&downRight);
   else out->Add(&right);
  } else {
   if ( endY < 0 ) out->Add(&up);
   else if ( endY > PROCE55ORS_GRID_DIM-1 ) out->Add(&down);
  }
 }
 void Between( Proce55orsHandles *active ) {
  EACH(active->first,Proce55orsHandle,ph) {
   EACHN(ph->p->first,Proce55or,p,{UpdateProce55orGridPosition(p);})
  }
  EACH(active->first,Proce55orsHandle,ph) ph->p->Between();
 }
 void Render( Proce55orsHandles *active ) {
  EACH(active->first,Proce55orsHandle,ph) ph->p->Render();
 }
MANY(Proce55orsGrid,Proce55orsGridHandle,Proce55orsGridHandles,"Proce55orsGrid",Proce55orsGrids,{})
DONE(Proce55orsGrid);