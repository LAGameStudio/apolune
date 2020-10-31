#pragma once

#include "LinkedList.h"
#include "Indexed.h"

// String hashing template based on Indexed

template<class LISTITEMCHILD>
class StringIndexed {
public:
 Indexed<LinkedList> lists;
 Zint last_index;
 Zint index_iterator;
 Zp<LinkedList> lists_iterator;
 Zp<ListItem> list_iterator;
 StringIndexed() {
  Init();
 }
 void Init( size_t depth=256 ) {
  Depth(depth);
  Rewind();
 }
 void Rewind() {
  index_iterator=0;
  lists_iterator=&lists[index_iterator];
  list_iterator=lists_iterator->first;
 }
 LISTITEMCHILD *Next() {
  if ( !lists_iterator ) Rewind();
  else {
   if ( list_iterator ) list_iterator=list_iterator->next;
   if ( !list_iterator ) {
    index_iterator++;
    if ( index_iterator >= lists.length ) { Rewind(); return (LISTITEMCHILD *) list_iterator.pointer; }
    lists_iterator=&lists[index_iterator];
    list_iterator=lists_iterator->first;
   }
  }
  return (LISTITEMCHILD *) list_iterator.pointer;
 }
 void Remove() { if ( lists_iterator && list_iterator ) lists_iterator->Remove(list_iterator.pointer); }
 void Depth( size_t depth=256 ) { lists.Size(depth); }
 int Encode( const char *s ) {
  int index=0;
  int len=strlen(s);
  if ( len <= 0 ) return 0;
  int a=len/4;        if ( a > len-1 ) a=len; int c_a=(int)s[a];
  int b=len/2;        if ( b > len-1 ) b=len; int c_b=(int)s[b];
  int c=len/2+len/4;  if ( c > len-1 ) c=len; int c_c=(int)s[c];
  int d=len/3;        if ( d > len-1 ) d=len; int c_d=(int)s[d];
  int e=len/3*2;      if ( e > len-1 ) e=len; int c_e=(int)s[e];
  int f=len-1;        if ( f > len-1 ) f=len; int c_f=(int)s[f];
  return (int)(last_index=((c_a+c_b+c_c+c_d+c_e+c_f)%(int)lists.length));
 }
 LinkedList *GetList( const char * s ) {
  Encode(s);
  return &lists[(size_t)(int)last_index];
 }
 void Store( const char *s, ListItem *single ) {
  LinkedList *list=GetList(s);
  list->Append(single);
 }
 ListItem *SimpleGet( const char *s ) {
  LinkedList *list=GetList(s);
  Zstring test;
  EACH(list->first,ListItem,i) {
   i->GetThisStringIndex(test.self());
   if ( test.Equals(s) ) return i;
  }
  return null;
 }
 LISTITEMCHILD *Get( const char *s ) {
  LinkedList *list=GetList(s);
  Zstring test;
  EACH(list->first,ListItem,i) {
   i->GetThisStringIndex(test.self());
   if ( test.Equals(s) ) return (LISTITEMCHILD *)i;
  }
  return null;
 }
 LISTITEMCHILD *Remove( const char *s ) {
  ListItem *got=SimpleGet(s);
  if ( !got ) return null;
  if ( list_iterator == (ListItem *) got ) Next();
  if ( got ) lists[last_index].Remove(got);
  return (LISTITEMCHILD *)got;
 }
 ListItem &operator() ( const char *s ) { return *Get(s); }
};

