#pragma once
#include "Cartesian.h"
#include "GLWindow.h"

/*
 * Simple map class.
 */
class Map : public ListItem {
public:
 string type; // Tells a MapObject what type of map it is on by keyword
 Cartesian gravity;
 Map() : ListItem() {
  type=string("Map");
 }

 virtual void Init() { }
 // Downloads map information in a thread
 virtual void Download() { }
 virtual void Upload()   { }

 virtual void MoveBy( int x, int y ) {
 }
 virtual void MoveBy( double x, double y ) {
 }

 void BetweenFrames() {
 }

 virtual void PreProcess() { }
 virtual void PostProcess() { }
 virtual void Render() {}
 virtual void Render( GLWindow *target ) { }
 virtual void Radar( GLWindow *surface ) { }
};

HANDLING(Map,MapHandle,"map");

class MapHandles : public LinkedList {
public:
 MapHandle *Add( Map *map ) {
  MapHandle *mh=new MapHandle;
  mh->p=map;
  Append(mh);
  return mh;
 }
 void Between() { FOREACH(MapHandle,m) m->p->BetweenFrames(); }
 void Render() {
  FOREACH(MapHandle,m) {
   m->p->PreProcess();
   m->p->Render();
   m->p->PostProcess();
  }
 }
 void MoveBy( int x, int y ) {
  FOREACH(MapHandle,m) m->p->MoveBy(x,y);
 }
 void MoveBy( double x, double y ) {
  FOREACH(MapHandle,m) m->p->MoveBy(x,y);
 }
 CLEARLISTRESET(MapHandle);
 ~MapHandles() { Clear(); }
};

template <class T>
class ZonedMaps : public ListItem {
public:
 matrix2d<T> zones;
 int w,h;
 Cartesiand extents;
 Cartesiand screen;
 Cartesian c;
 ZonedMaps() : ListItem() {
  w=h=0;
  screen.SetRect(display.w/2.0,display.h/2.0,display.w,display.h);
 }
 // Sets the dimensions of the zones.  Should be bigger than one screen.  Recommend 3x
 void Dimensions( int zoneW, int zoneH, int zonesX, int zonesY, bool centered ) {
  zones.resize(zonesX,zonesY);
  w=zoneW;
  h=zoneH;
  extents.w=zoneW*zonesX;
  extents.h=zoneH*zonesY;
  if ( centered ) {
   extents.x2=extents.w/2;
   extents.x=-extents.x2;
   extents.y2=extents.h/2;
   extents.y=-extents.y2;
  } else {
   extents.x=0;
   extents.x2=extents.w;
   extents.y=0;
   extents.y2=extents.h;
  }
 }
 // Rezones a map object that has moved.
 void Rezone( MapObject *o ) {
  T *m=&zones(o->zone.x,o->zone.y);
  m->objects.Remove(o);
  find(o->map.position.x+extents.x,o->map.position.y+extents.y);
  o->zone.x=c.x;
  o->zone.y=c.y;
  m=&zones(o->zone.x,o->zone.y);
  m->objects.Add(o);
 }
 // Removes a MapObject from its zone.
 void Remove( MapObject *o ) {
  T *m=&zones(o->zone.x,o->zone.y);
  m->objects.Remove(o);
 }
 // Places a MapObject into the correct zone based on position.
 void Zone( MapObject *o ) {
  o->zone=*find(o->map.position.x+extents.x,o->map.position.y+extents.y);
  T *m=&zones(o->zone.x,o->zone.y);
  m->objects.Add(o); 
 }
 // Returns the x,y relative to the zones.  If your coordinate system is not
 // all positive, you need to manipulate the incoming values to compensate,
 // since this system is not "infinite" and is limited to
 // zoneW * zoneX by zoneH * zoneY
 Cartesian *find( double x, double y ) {
  c.x=(int)x/w;
  c.y=(int)y/h;
  return &c;
 }
 // Returns a set of zones where the center is x,y and the extents are
 // defined as x-zw2, y-zh2 to x+zw2, y+zh2
 // Z does not mean Z axis
 // This allows you to render multiple zones when you approach the seams,
 // and it also is used for "area" modification.  Ahem.
 MapHandles *GetZones( double x, double y, double zw2, double zh2 ) {
  MapHandles *mhs=new MapHandles;
  Cartesian b;
  find(x-zw2,y-zh2);
  b.Set(c.x,c.y);
  find(x+zw2,y+zh2);
  for ( int i=b.x; i<=c.x; i++ ) {
   for ( int j=b.y; j<=b.y; j++ ) {
    mhs->Add((Map *) &(zones(i,j)));
   }
  }
  return mhs;
 }
 bool NearEdgeZone( Vertexd *o ) { find(o->x,o->y); return ( c.x==1 || c.y==1 || c.x==w-2 || c.y==h-2 ); }
 bool BeyondEdge( Vertexd *o ) { return ( o->x >= (double) w-extents.w || o->y >= (double) h-extents.h || o->x <= extents.w || o->y <= extents.h ); }
 void Add( MapObject *o ) {
  find(o->map.position.x,o->map.position.y);
  T *m=&zones(c.x,c.y);
 }
 void Render( double x, double y ) {
  MapHandles *maps=GetZones(x,y,screen.x,screen.y);
  maps->Render();
  delete maps;
 }
 void Between( double x, double y ) {
  MapHandles *maps=GetZones(x,y,screen.x,screen.y);
  maps->Between();
  delete maps;
 }
 void MoveBy( int x, int y ) {
  MapHandles *maps=GetZones(x,y,screen.x,screen.y);
  maps->MoveBy(x,y);
  delete maps;
 }
 void MoveBy( double x, double y ) {
  MapHandles *maps=GetZones(x,y,screen.x,screen.y);
  maps->MoveBy(x,y);
  delete maps;
 }
};