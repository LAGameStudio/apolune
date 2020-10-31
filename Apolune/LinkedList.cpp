
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "LinkedList.h"

#include "BinaryFile.h"
#include "Randoms.h"

void LinkedList::BinaryRead( BinaryFile *open ) {
 int length;
 fflush(open->fp);
 open->read(&length);
 if ( length == 0 ) return;
 BinaryReadData(open);
 for ( int i=0; i<length; i++ ) {
  fflush(open->fp);
  ReadAppend(open);
 }
}

void LinkedList::BinaryWrite( BinaryFile *open ) {
 fflush(open->fp);
 open->scrawl(count.value);
 BinaryWriteData(open);
 FOREACH(ListItem,L) {
  fflush(open->fp);
  L->BinaryWrite(open);
 }
}

ListItem *LinkedList::Any( int seed ) {
 if ( count == 0 ) return null;
 Random *set=randoms.GetSet(seed+666);
 return Element((int)((float)count*2.0f*set->unique(seed,seed+1,seed+2))%count);
}

void LinkedList::sendUp(ListItem *L, bool wrap) 
{
 if ( wrap ) {
  if ( L->prev ) {
   ListItem *before=L->prev;
   Remove(L);
   InsertBefore(L,before);
  } else {
   Remove(L);
   Append(L);
  }
 } else if ( L->prev ) {
  ListItem *before=L->prev;
  Remove(L);
  InsertBefore(L,before);
 }
}

void LinkedList::sendDown(ListItem *L, bool wrap)
{
 if ( wrap ) {
  if ( L->next ) {
   ListItem *after=L->next;
   Remove(L);
   InsertAfter(L,after);
  } else {
   Remove(L);
   Prepend(L);
  }
 } else if ( L->next ) {
  ListItem *after=L->next;
  Remove(L);
  InsertAfter(L,after);
 }
}

// Adds to end of list
void LinkedList::Append(ListItem *L) {
 _Append(L);
}

// Adds to end of list
void LinkedList::_Append(ListItem *L) {
 count++;
 if ( last ) { 
  last->next=L;
  L->prev=last; 
  L->next=null;
  last=L; 
  return; 
 }
 L->next=L->prev=null;
 first=last=L;
}

// Adds to beginning of list
void LinkedList::Prepend(ListItem *L) {
 count++;
 if ( first ) {
  first->prev=L;
  L->next=first;
  L->prev=null;
  first=L;
  return;
 }
 L->next=L->prev=null;
 first=last=L;
}

// Finds element at index i, if none returns null
// 0=first
 ListItem *LinkedList::Element(int i) {
 ListItem *l;
 if ( count.value == 0 || i >= count.value ) return null;
 int c=0;
 for ( l=first; l; l=l->next ) {
  if ( c==i ) return l;
  c++;
 }
 if ( first ) return first;
 if ( last ) return last;
 OUTPUT("LinkedList::Element(index) returned null because the list was empty.\n");
 return null;
}

ListItem *LinkedList::Element(int i, bool wrap) {
 ListItem *l;
 if ( count.value == 0 ) return null;
 i%=count.value;
 int c=0;
 for ( l=first; l; l=l->next ) {
  if ( c==i ) return l;
  c++;
 }
 if ( first ) return first;
 if ( last ) return last;
 OUTPUT("LinkedList::Element(index) returned null because the list was empty.\n");
 return null;
}

int LinkedList::Element( ListItem *q ) {
 int i=0;
 FOREACH(ListItem,p) if ( p == q ) return i; else i++;
 OUTPUT("LinkedList::Element(item) returned -1 because the item was not found.\n");
 return -1;
}

// Inserts element L before p ListItem in List
void LinkedList::InsertBefore(ListItem *insert, ListItem *before) {
 if ( !before ) { Prepend(insert); return; }
 insert->prev=before->prev;
 if ( insert->prev ) insert->prev->next=insert;
 insert->next=before;
 before->prev=insert;
 if ( before == first ) first=insert;
 count++;
}

// Inserts element L after p ListItem in List
void LinkedList::InsertAfter(ListItem *insert, ListItem *after) {
 if ( !after ) { Prepend(insert); return; }
 insert->next=after->next;
 if ( after->next ) after->next->prev=insert;
 after->next=insert;
 insert->prev=after;
 if ( after == last ) last=insert;
 count++;
}

// Inserts element at index i (before), if index i exists, otherwise returns false
void LinkedList::InsertAt(ListItem *insert, int i) {
 ListItem *p=Element(i);
 InsertBefore(insert,p);
}

// Removes element L from list
void LinkedList::_Remove(ListItem *L) {
#if defined(DEBUG)
 if ( !this->ListIteminList(L) ) {
  OUTPUT("Warning: attempted to remove something not in this list.\n\r");
  return;
 }
#endif
 if ( first==L ) {
  if ( first==last ) {
   first=last=null; 
   L->next=L->prev=null;
   count=0;
   return;
  }
  first=first->next;
  if ( first ) first->prev=null;
  L->next=L->prev=null;
  count--;
  return;
 }
 if ( last==L ) {
  last=last->prev;
  if ( last ) last->next=null;
  L->next=L->prev=null;
  count--;
  return;
 }
 ListItem *p=L->prev,*n=L->next;
 if ( p ) p->next=n;
 if ( n ) n->prev=p;
 L->next=L->prev=null;
 count--;
 return;
}

// Removes all elements from the list
void LinkedList::RemoveAll(void) {
 first=last=null;
 count=0;
}

// Deletes (frees) element L from list
void LinkedList::Delete(ListItem *L) {
 Remove(L);
 delete L;
}

// Deletes (frees) element L from list, clears incoming
void LinkedList::Delete(ListItem **L) {
 Remove(*L);
 delete (*L);
 *L=null;
}


void LinkedList::RandomizeOrder() {
 LinkedList newList;
 EACHN(first,ListItem,i,{
  Remove(i);
  if ( uniform() >= 0.5 ) {
   newList.Prepend(i);
  } else {
   newList.Append(i);
  }
 });
 this->first=newList.first; newList.first=null;
 this->last=newList.last; newList.last=null;
 count=newList.count; newList.count=0;
 return;
}

void LinkedList::RandomizeOrder(int seed) {
 LinkedList newList;
 Random *set=randoms.GetSet(seed);
 int iteration=(int)(100000.0f*set->unique(seed,seed));
 EACHN(first,ListItem,i,{
  Remove(i);
  if ( set->unique(seed,iteration) >= 0.5f ) newList.Prepend(i);
  else newList.Append(i);
  iteration=(int)(100000.0f*set->unique(seed,iteration+1));
 });
 this->first=newList.first; newList.first=null;
 this->last=newList.last; newList.last=null;
 count=newList.count; newList.count=0;
 return;
}