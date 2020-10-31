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
#include "Proce55ors.h"

class ZonedProce55ors : public ListItem {
public:
 ZoneData zone;
 Proce55ors fx;
 ZonedProce55ors() : ListItem() {}
};

class ZonedProce55orsHandle : public Handle<ZonedProce55ors> {
public:
};

class ZonedProce55orsList : public Handles {
public:
 bool inList( ZonedProce55ors *zp ) {
  FOREACH(ZonedProce55orsHandle,p) if ( p->p==zp ) return true;
  return false;
 }
 virtual void MoveBy( int dx, int dy ) {
  FOREACH(ZonedProce55orsHandle,p) p->p->fx.MoveBy(dx,dy);
 }
 virtual ADDLIST(ZonedProce55ors,p,ZonedProce55orsHandle);
 virtual CLEARLISTRESET(ZonedProce55orsHandle);
 ~ZonedProce55orsList() { 
  Clear();
 }
};

class ZonedProce55orsGrid : public ListItem {
public:
 ZoneData size;
 Zpointer<GLWindow> parent;
 matrix2d<ZonedProce55ors> zones;
 Zint w,h;
 Zbool OptimizeRezoning;
 ScreenPosition extents;
 ScreenPosition screen;
 Cartesiand midpoint;
 ScreenPosition activeArea; // How wide the search is.
 ZoneData c;
 ZonedProce55orsGrid() : ListItem() {
  screen.Set(display.wd/2.0,display.hd/2.0,display.wd,display.hd);
  activeArea.Set(
   display.w2d-display.wd, display.h2d-display.hd, 
   display.wd*2.0, display.hd*2.0
  );
 }
 // Sets the dimensions of the zones.  Should be bigger than one screen.  Recommend 3x
 void RenumberZones() {
  for ( int i=0; i<size.x; i++ ) {
   for ( int j=0; j<size.y; j++ ) {
    ZonedProce55ors *m=&zones(i,j);
    m->zone.x=i;
    m->zone.y=j;
   }
  }
 }
 void Dimensions( int zoneW, int zoneH, int zonesX, int zonesY, bool centered ) {
  size.x=zonesX;
  size.y=zonesY;
  zones.resize(zonesX,zonesY);
  RenumberZones();
  w=zoneW;
  h=zoneH;
  extents.Width(zoneW*(zonesX-1));
  extents.Height(zoneH*(zonesY-1));
  if ( centered ) {
   extents.PositionX(-extents.dw/2.0);
   extents.PositionY(-extents.dh/2.0);
  }
 }
 void RezoneToSparsestZone( Proce55or *o, bool avoidScreen=true ) {
  ZonedProce55ors *zp=findSparsestZone();
  if ( zp->zone.x == o->zone.x
    && zp->zone.y == o->zone.y ) return;
  // Calculate the coordinate difference to move the object into the zone (c is set to target zone's zone coords)
  int dx=(c.x-o->zone.x)*w;
  int dy=(c.y-o->zone.y)*h;
  // Put the object in the zone
  o->zone.x=c.x;
  o->zone.y=c.y;
  zp->fx.Append(o);
  o->proce55ors=&zp->fx;
  // Move the object
  o->MoveBy(dx,dy);
  if ( avoidScreen ) {
   ZonedProce55orsList *zpl=GetActiveZones();
   if ( zpl->inList(zp) ) { 
    // since we're in the active list, we need to determine if the
    // move will coincide with the player's screen, and move it to a screen edge
    // this was chosen over finding another zone because it will move all
    // of the objects out of the zone and not permit them to land near the player at all
    // Note: there may be a bug here, also there is a recursion chance
    // A further complication of this is that if you move the object outside of a zone it may temporarily
    // be in the wrong place.  Currently the theory is that if the object is outside the correct zone it will
    // simply be rezoned away when the player arrives in the zone, and since the zones are larger than a
    // single screen, objects smaller than a screen will not be seen as they are rezoned on entry or before
    // they are even rendered.
    if ( WITHIN(o->x,o->y,screen.x,screen.y,screen.x2,screen.y2)
      || WITHIN(o->x2,o->y2,screen.x,screen.y,screen.x2,screen.y2) ) {
     // picked randomly
     switch ( randoms.Upto(6,c.x,c.y) ) {
     case 0: o->MoveBy(-(o->translated.x+o->translated.w), -(o->translated.y+o->translated.h)); break; // top left
     case 1: o->MoveBy(-(o->translated.x+o->translated.w), 0); break; // left
     case 2: o->MoveBy((display.w-o->translated.x),0); break; // right
     case 3: o->MoveBy(0,(display.h-o->translated.y)); break; // bottom
     case 4: o->MoveBy(0, -(o->translated.y+o->translated.h)); break; // top 
     case 5: o->MoveBy((display.w-o->translated.x),(display.h-o->translated.y)); break; // bottom right
     }
    }
   }
   delete zpl;
  }
  o->rezoneMe=false; // stops redundant rezoning
 }
 // Rezones a Proce55or that has moved.
 void Rezone( Proce55or *o ) {
  o->rezoneMe=false;
  o->proce55ors->Remove(o);
  if ( OptimizeRezoning && !o->unRezoned && !( o->OnScreen() || o->OnScreenTranslated() ) ) {
   // If the object is being rezoned while it is on screen, defer...
   RezoneToSparsestZone( o );
  } else {
   find(o->x,o->y);
   o->zone.x=c.x;
   o->zone.y=c.y;
   ZonedProce55ors *m=&zones(o->zone.x,o->zone.y); 
   o->proce55ors=&m->fx;
   m->fx.Append(o);
   m->fx.ZSort();
  }
  o->unRezoned=false;
 }
 // Rezones all objects from zones with too many (called when initializing a large map)
 void OptimizeZones( int count, int clamp=-1 ) {
  // Mark proce55ors that are going to be moved
  for ( int i=0; i<size.x; i++ ) {
   for ( int j=0; j<size.y; j++ ) {
    ZonedProce55ors *m=&zones(i,j);
    if ( m->fx.count > count ) {
     int diff=count;
     for ( int k=0; k<diff; k++ ) {
      Proce55or *p=(Proce55or *) m->fx.Any(count+i+j+k); // choose randomly
      int d=0;
      while ( p->rezoneMe ) {
       d++;
       p=(Proce55or *) m->fx.Any(count+i+j+k+d); // choose randomly
       if ( d > 100 ) {
        // Just find one because our random numbers weren't so random.
        EACH(m->fx.first,Proce55or,q) if ( !q->rezoneMe ) { p=q; break; }
        break;
       }
      }
      if ( !p->rezoneMe ) p->rezoneMe=true;
     }
     // Now go through and actually move them to sparser zones
     EACHN(m->fx.first,Proce55or,q,{
      if ( q->rezoneMe ) {
       m->fx.Remove(q);
       RezoneToSparsestZone(q,false);
       q->rezoneMe=false;
      }
     });
    }
   }
  }
  //Now go through clamping
  if ( clamp > 0 ) {   
   for ( int i=0; i<size.x; i++ ) {
    for ( int j=0; j<size.y; j++ ) {
     ZonedProce55ors *m=&zones(i,j);
     if ( m->fx.count > clamp ) {
      int diff=m->fx.count-clamp;
      EACHN(m->fx.first,Proce55or,q,{
       m->fx.Remove(q);
       delete q;
      });
     }
    }
   }
  }
 }
 // Removes a Proce55or from its zone.
 void Remove( Proce55or *o ) {
  ZonedProce55ors *m=&zones(o->zone.x,o->zone.y);
  m->fx.Remove(o);
  o->proce55ors=null;
 }
 // Places a Proce55or into the correct zone based on position.
 void Zone( Proce55or *o, GLWindow *w, Proce55or *placeBehind=null  ) {
  o->zone=*find(o->x,o->y);
  ZonedProce55ors *m=&zones(o->zone.x,o->zone.y);
  m->fx.Add(o,w,placeBehind); 
 }
 // Places a Proce55or into the correct zone based on position.
 void Zone( Proce55or *o ) {
//  o->zone=*find(o->x+extents.x,o->y+extents.y);
  o->zone=*find(o->x,o->y);
  ZonedProce55ors *m=&zones(o->zone.x,o->zone.y);
  m->fx.Add(o,parent); 
 }
 // Places a Proce55or into the correct zone based on position.
 void Add( Proce55or *o, GLWindow *w,  Proce55or *placeBehind=null ) { Zone(o,w,placeBehind); }
 bool AddIfLessThan( int limit, Proce55or *o, GLWindow *w ) {
  Zone(o,w);
 }
 void AddOriginCentered( Proce55or *o, GLWindow *w ) {
  o->x+=(int)extents.x;
  o->y+=(int)extents.y;
  Zone(o,w);
 }
 void Add( Proce55or *o )              { Zone(o,parent); }
 // Returns the x,y relative to the zones.  If your coordinate system is not
 // all positive, you need to manipulate the incoming values to compensate,
 // since this system is not "infinite" and is limited to
 // zoneW * zoneX by zoneH * zoneY
 ZoneData *find( double x, double y ) {
  double nx,ny;
  extents.GetNormalizedPosition(x,y,&nx,&ny);
  c.x=(int)(nx*10.0);
  c.y=(int)(ny*10.0);
  // Hard limiter
  if ( c.x < 0 ) c.x=0;
  if ( c.y < 0 ) c.y=0;
  if ( c.x >= size.x ) c.x=size.x-1;
  if ( c.y >= size.y ) c.y=size.y-1;
  return &c;
 }
 // Returns the x,y relative to the zones.  If your coordinate system is not
 // all positive, you need to manipulate the incoming values to compensate,
 // since this system is not "infinite" and is limited to
 // zoneW * zoneX by zoneH * zoneY
 ZonedProce55ors *findSparsestZone() {
  ZonedProce55ors *candidate=null;
  int zw=zones.M();
  int zh=zones.N();
  for ( int i=0; i<zw; i++ ) for ( int j=0; j<zh; j++ ) {
   ZonedProce55ors *zp=&zones(i,j);
   if ( candidate==null || zp->fx.count < candidate->fx.count ) candidate=zp;
  }
  if (!candidate) return null;
  c.x=candidate->zone.x;
  c.y=candidate->zone.y;
  return candidate;
 }
 // Returns a set of zones where the center is x,y and the extents are
 // defined as x-zw2, y-zh2 to x+zw2, y+zh2
 // Z does not mean Z axis
 // This allows you to render multiple zones when you approach the seams,
 // and it also is used for "area" modification, selects all relevant zones
 // that overlap a given area.
 ZonedProce55orsList *GetZones( double x, double y, double zw2, double zh2 ) {
  ZonedProce55orsList *mhs=new ZonedProce55orsList;
  if ( zones.M() == 0 || zones.N() == 0 ) return mhs;
  Cartesian b;
  find(x-zw2,y-zh2);
///  b.Set(c.x!=0?c.x+1:0,c.y!=0?c.y+1:0); //?
  find(x+zw2,y+zh2);
  // Make B small and C big
  if ( b.x > c.x ) { int t=c.x; c.x=b.x; b.x=t; }
  if ( b.y > c.y ) { int t=c.y; c.y=b.y; b.y=t; }
  // Clamp to 0-size
  int maxX=(int)(zones.M()-1);
  int maxY=(int)(zones.N()-1);
  b.x=CLAMPTO(b.x,0,maxX);
  b.y=CLAMPTO(b.y,0,maxY);
  c.x=CLAMPTO(c.x,0,maxX);
  c.y=CLAMPTO(c.y,0,maxY);
  for ( int j=b.y; j<=c.y; j++ ) {
   for ( int i=b.x; i<=c.x; i++ ) {
    mhs->Add(&(zones(i,j)));
   }
  }
  return mhs;
 }
 ZonedProce55orsList *GetActiveZones() {
  return GetZones(activeArea.x,activeArea.y,activeArea.w,activeArea.h); // Selected region is 2x screen
 }
 bool NearEdgeZone( Vertexd *o ) {
  find(o->x,o->y);
  return ( c.x==1 || c.y==1 || c.x==w-2 || c.y==h-2 );
 }
 bool BeyondEdge( Vertexd *o ) {
  return ( o->x >= (double) w-extents.w || o->y >= (double) h-extents.h || o->x <= extents.w || o->y <= extents.h );
 }
 void Render( double x, double y ) {
  ZonedProce55orsList *maps=GetActiveZones();
  EACH(maps->first,ZonedProce55orsHandle,p) p->p->fx.Render();
  delete maps;
 }
 void Render( GLWindow *w, double x, double y ) {
  ZonedProce55orsList *maps=GetActiveZones();
  EACH(maps->first,ZonedProce55orsHandle,p) p->p->fx.Render(w);
  delete maps;
 }
 void Between( double x, double y ) {
  ZonedProce55orsList *maps=GetActiveZones();
  EACH(maps->first,ZonedProce55orsHandle,p) p->p->fx.Between();
  delete maps;
 }
 void MoveBy( int x, int y ) {
  ZonedProce55orsList *maps=GetActiveZones();
  EACH(maps->first,ZonedProce55orsHandle,p) p->p->fx.MoveBy(x,y);
  delete maps;
 }
 void MoveBy( float x, float y ) {
  ZonedProce55orsList *maps=GetActiveZones();
  EACH(maps->first,ZonedProce55orsHandle,p) p->p->fx.MoveBy(x,y);
  delete maps;
 } 
};
