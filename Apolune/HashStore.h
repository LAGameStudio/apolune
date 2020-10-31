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

#include "LinkedList.h"
#include "Indexed.h"
#include "PackedString.h"

template<class ITEM, class LIST>
class HashStore : public ListItem {
public:
 Indexed<LIST> store;
 size_t depth;
 HashStore( size_t depth=1024 ) : ListItem() {
  this->depth=depth;
  store.Size(depth,depth);
  Init();
 }
 virtual void Init() {}
 LIST *Bucket( PackedString *packed ) {
  unsigned int x=packed->Length() > 0 ? packed->Get(0) : 0;
  unsigned int y=packed->Length() > 1 ? packed->Get(1) : 0;
  x %= (int)depth;
  y %= (int)depth;
  return &store(x,y);
 }
 LIST *Get( unsigned int x ) { return &store(x); }
 LIST *Get( unsigned int x, unsigned int y ) { return &store(x,y); }
 void Append( ITEM *I, PackedString *packed ) {
  LIST *L=Bucket(packed);
  L->Append(I);
 }
 bool Append( ITEM *I, PackedString *packed, bool check_if_stored ) {
  LIST *L=Bucket(packed);
  if ( !L->inList(I) ) {
   L->Append(I);
   return true;
  }
  return false;
 }
 void Prepend( ITEM *I, PackedString *packed ) {
  LIST *L=Bucket(packed);
  L->Prepend(I);
 }
 bool Prepend( ITEM *I, PackedString *packed, bool check_if_stored ) {
  LIST *L=Bucket(packed);
  if ( !L->inList(I) ) {
   L->Prepend(I);
   return true;
  }
  return false;
 }
 bool Stored( ITEM *I, PackedString *packed ) {
  LIST *L=Bucket(packed);
  return ( L->inList(I) );
 }
 ITEM *Extract( ITEM *I, PackedString *packed ) {
  LIST *L=Bucket(packed);
  L->Remove(I);
  return I;
 }
 void Restore( ITEM *I, PackedString *packed ) {
  Extract(I);
  Append(I,packed);
 }
 void RestorePrepend( ITEM *I, PackedString *packed ) {
  Extract(I);
  Prepend(I,packed);
 }
 /* To be used in the template instantiation:
 int Total() {
  int total=0;
  size_t length=SQ(depth);
  for ( unsigned int i=0; i<length; i++ ) total+=store(i).count.value;
  return total;
 }
 */
};
