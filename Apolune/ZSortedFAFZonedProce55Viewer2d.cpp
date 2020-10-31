#include "ZSortedFAFZonedProce55Viewer2d.h"

#define DEBUG_W 40

void ZSortedFAFZonedProce55Viewer2d::DebugDrawScreenPosition() {  
 double sx=(DEBUG_W*(double)grid.size.x);
 double sy=(DEBUG_W*(double)grid.size.y);
 // Draw screen position
 Cartesian rect;
 drawing.Pick(alabaster);
 double nx,ny,nx2,ny2;
 grid.extents.GetNormalizedPosition(grid.screen.dx,grid.screen.dy,&nx,&ny);
 grid.extents.GetNormalizedPosition(grid.screen.dx2,grid.screen.dy2,&nx2,&ny2);
 rect.Corners(
  10+(int)(nx*sx),
  10+(int)(ny*sy),
  10+(int)(nx2*sx),
  10+(int)(ny2*sy)
 );
 Rectangle(drawing,&rect);
 drawing.Pick(purple);
 grid.extents.GetNormalizedPosition(grid.activeArea.dx,grid.activeArea.dy,&nx,&ny);
 grid.extents.GetNormalizedPosition(grid.activeArea.dx2,grid.activeArea.dy2,&nx2,&ny2);
 rect.Corners(
  10+(int)(nx*sx),
  10+(int)(ny*sy),
  10+(int)(nx2*sx),
  10+(int)(ny2*sy)
 );
 Rectangle(drawing,&rect);
}

void ZSortedFAFZonedProce55Viewer2d::Debug()  {
 if ( !active ) return;
 Zint totalActiveProce55ors,totalOnScreenProce55ors;
 EACH(active->first,ZonedProce55orsHandle,p) {
  totalActiveProce55ors+=p->p->fx.count;
  EACH(p->p->fx.first,Proce55or,q) if ( q->OnScreenTranslated() ) totalOnScreenProce55ors++;
 }
 char buf[200];
 FORMAT(buf,200,"%d zones, %d proce55ors (%d on screen), %f,%f (%fx%f)",
  (int) active->count,
  (int) totalActiveProce55ors,
  (int) totalOnScreenProce55ors,
  (float) (screen.x+screen.w/2),
  (float) (screen.y+screen.h/2),
  (float) screen.w,
  (float) screen.h
 );
 Blending(none);
 drawing.Pick(orange);
 glColor4fv(drawing.floats);
 Text(buf,30,(double)(display->h-30),10,8,false);
 Blending(additive);
 // Draw a small scale view of the active versus inactive and their members
 for ( int i=0; i<grid.size.x; i++ ) for ( int j=0; j<grid.size.y; j++ ) {
  ZonedProce55ors *m=&grid.zones(i,j);
  bool found=false;
  EACH(active->first,ZonedProce55orsHandle,p) if ( p->p == m ) { found=true; break; }
  drawing.Pick( found ? red255 : blue );
  Cartesian rect;
  rect.SetRect(10+i*DEBUG_W,10+j*DEBUG_W,DEBUG_W,DEBUG_W);
  Rectangle(drawing,&rect);
  EACH(m->fx.first,Proce55or,p) {
   double px,py;
   p->GetNormalizedZonedPosition(&grid,&px,&py);
   px*=(double)(DEBUG_W*grid.size.x);
   py*=(double)(DEBUG_W*grid.size.y);
   if ( found ) {
    bool onScreen=p->OnScreenTranslated();
    drawing.Pick( onScreen ? red255 : orange );
   } else {
    drawing.Pick( cyan );
   }
   Area(drawing,
    10+(int)px-1,
    10+(int)py-1,
    10+(int)px+1,
    10+(int)py+1);
  }
 }
 DebugDrawScreenPosition();
}

void ZSortedFAFZonedProce55Viewer2d::MiniDebug() {
 Zint totalActiveProce55ors;
 ZonedProce55orsList *active=grid.GetActiveZones();
 EACH(active->first,ZonedProce55orsHandle,p) {
  totalActiveProce55ors+=p->p->fx.count;
 }
 char buf[200];
 FORMAT(buf,200,"%d zones, %d proce55ors, %f,%f (%fx%f)",
  (int) active->count,
  (int) totalActiveProce55ors,
  (float) (screen.x+screen.w/2),
  (float) (screen.y+screen.h/2),
  (float) screen.w,
  (float) screen.h
 );
 Blending(none);
 Crayon drawing;
 drawing.Pick(orange);
 glColor4fv(drawing.floats);
 Text(buf,30,(double)(display->h-30),10,8,false);
 Blending(additive);
 // Draw a small scale view of the active zones and their members
 Cartesian b;
 grid.find(grid.screen.x+grid.screen.w/2-grid.screen.w,grid.screen.y+grid.screen.h/2-grid.screen.h);
 b.Set(grid.c.x,grid.c.y);
 grid.find(grid.screen.x+grid.screen.w/2+grid.screen.w,grid.screen.y+grid.screen.h/2+grid.screen.h);
 // Draw the grid of zones
 for ( int i=0; i<grid.size.x; i++ ) for ( int j=0; j<grid.size.y; j++ ) {
  drawing.Pick( blue );
  Cartesian rect;
  rect.SetRect(10+i*DEBUG_W,10+j*DEBUG_W,DEBUG_W,DEBUG_W);
  Rectangle(drawing,&rect);
 }
 // Draw the active zones and their contents.
 for ( int i=b.x; i<=grid.c.x; i++ ) for ( int j=b.y; j<=grid.c.y; j++ ) {
  ZonedProce55ors *m=&grid.zones(i,j);
  bool found=false;
  EACH(active->first,ZonedProce55orsHandle,p) if ( p->p == m ) { found=true; break; }
  if ( !found ) continue;
  drawing.Pick( found ? red255 : blue );
  Cartesian rect;
  rect.SetRect(10+i*DEBUG_W,10+j*DEBUG_W,DEBUG_W,DEBUG_W);
  Rectangle(drawing,&rect);
  drawing.Pick( found ? red255 : cyan );
  EACH(m->fx.first,Proce55or,p) {
   double px,py;
   p->GetNormalizedZonedPosition(&grid,&px,&py);
   px*=(double)(DEBUG_W*grid.size.x);
   py*=(double)(DEBUG_W*grid.size.y);
   Area(drawing,
    10+(int)px-1,
    10+(int)py-1,
    10+(int)px+1,
    10+(int)py+1);
  }
 }
 // Draw screen position
 DebugDrawScreenPosition();
}