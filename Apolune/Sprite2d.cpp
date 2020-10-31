#include "Sprite2d.h"

bool Inventory2d::Add( Item2d *i ) {
  if ( i->p->mass + weight > capacity ) return false;
  Increase(i,1);
  return true;
}

bool Inventory2d::Add( Item2d *i, int qty ) {
  if ( i->p->mass*qty + weight > capacity ) return false;
  Increase(i,qty);
  return true;
}

float Inventory2d::TotalWeight( Item2d *i ) {
 if ( i->combines ) {
  InventorySlot2d *holding=findByItem(i);
  if ( holding ) return (float) holding->quantity * i->p->mass;
  else return 0;
 }
 float total=0.0f;
 FOREACH(InventorySlot2d,j) if ( j->p==i ) total+=(float) j->quantity * i->p->mass;
 return total;
}

bool Inventory2d::CanHold( Item2d *i, int qty ) {
 if ( qty * i->p->mass + weight > capacity ) return false;
 return true;
}

void Inventory2d::Increase( Item2d *i, int qty ) {
 if( i->combines ) {
  InventorySlot2d *holding=findByItem( i );
  if ( !holding ) AddNewSlot(i);
  else holding->quantity+=qty;
 } else AddNewSlot(i);
 weight += i->p->mass * qty;
}

void Inventory2d::Decrease( Item2d *i, int qty ) {
 if ( qty > Total(i) ) qty=Total(i);
 if( i->combines ) {
  InventorySlot2d *holding=findByItem( i );
  if ( holding ) {
   holding->quantity-=qty;
   if ( holding->quantity == 0 ) {
    Remove(holding);
    weight -= i->p->mass * qty;
    delete holding;
   }
  }
 } else {
  weight -= i->p->mass * qty;
  InventorySlot2d *n;
  for ( InventorySlot2d *j=(InventorySlot2d *) first; j && qty > 0; j=n ) {
   n=(InventorySlot2d *) j->next;
   if ( j->p == i ) {
    Remove(j); qty--;
   }
  }
 }
}