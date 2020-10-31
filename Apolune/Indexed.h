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
#include "string_functions.h"

// Lightweight vector.

template<class T>
class Indexed {
public:
 T oob;
 T *list;
 size_t unit,length,w,h;
 Indexed() {
  list=null;
  w=h=0;
  length=0;
  unit=sizeof(T);
  ZeroMemory(&oob,(SIZE_T)unit);
 }
 Indexed( size_t length ) { Size(length); unit=sizeof(T); w=length; h=1; }
 Indexed( size_t w, size_t h ) { Size(w,h); unit=sizeof(T); }
 void Swap( Indexed<T> *values ) {
  T *tempList = list;
  size_t tempLength = length;
  size_t tempW = w;
  size_t tempH = h;
  list=values->list;
  length=values->length;
  w=values->w;
  h=values->h;
  values->list=tempList;
  values->length=tempLength;
  values->w=tempW;
  values->h=tempH;
 }
 T *Increase() {
  size_t newsize=length+1;
  Size(newsize,true);
  return Element((unsigned int) newsize-1);
 }
 void IncreaseW() {
  IncreaseW(w+1);
 }
 void IncreaseH() {
  IncreaseH(h+1);
 }
#pragma warning( push ) 
#pragma warning( disable : 4101 )
 void IncreaseW( size_t neww ) {
  if ( neww < w ) { ClipW(neww); return; }
  if ( neww == w ) return;
  if ( h == 0 ) Size(neww,(size_t)1);
  else {
   size_t desiredLength=neww*h;
   T *newlist;
   try { newlist= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
   }
   for ( unsigned int i=0; i<(unsigned int)w; i++ ) for ( unsigned int j=0; j<(unsigned int)h; j++ ) newlist[i+j*w]=list[i+j*w];
   delete[] list;
   length=desiredLength;
   list=newlist;
   w=neww;
  }
 }
 void IncreaseH( size_t newh ) {
  if ( newh < h ) { ClipH(newh); return; }
  if ( newh == h ) return;
  if ( w == 0 ) Size(1,newh);
  else {
   size_t desiredLength=w*newh;
   T *newlist;
   try { newlist= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
   }
   for ( unsigned int i=0; i<(unsigned int)w; i++ ) for ( unsigned int j=0; j<(unsigned int)h; j++ ) newlist[i+j*w]=list[i+j*w];
   delete[] list;
   length=desiredLength;
   list=newlist;
   h=newh;
  }
 }
#pragma warning( pop )
 void ClipW() {
  if ( w==1 ) { Size(0); return; }
  else if ( w==0 ) return;
  ClipW(w-1);
 }
 void ClipH() {
  if ( h==1 ) { Size(0); return; }
  else if ( h==0 ) return;
  ClipW(h-1);
 }
#pragma warning( push ) 
#pragma warning( disable : 4101 )
 void ClipW( size_t neww ) {
  if ( neww==0 ) Size(0);
  else
  if ( neww > w ) IncreaseW(neww);
  else {
   size_t desiredLength=neww*h;
   T *newlist=null;
   try { newlist= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
   }
   for ( unsigned int i=0; i<(unsigned int)neww; i++ ) for ( unsigned int j=0; j<(unsigned int)h; j++ ) newlist[i+j*w]=list[i+j*w];
   delete[] list;
   length=desiredLength;
   list=newlist;
   w=neww;
  }
 }
 void ClipH( size_t newh ) {
  if ( newh==0 ) Size(0);
  else
  if ( newh > h ) IncreaseH(newh);
  else {
   size_t desiredLength=w*newh;
   T *newlist=null;
   try { newlist= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
   }
   for ( unsigned int i=0; i<(unsigned int)w; i++ ) for ( unsigned int j=0; j<(unsigned int)newh; j++ ) newlist[i+j*w]=list[i+j*w];
   delete[] list;
   length=desiredLength;
   list=newlist;
   h=newh;
  }
 }
 void Size( size_t w, size_t h ) {
  Size(w*h);
  this->w=w;
  this->h=h;
 }
 void Size( size_t desiredLength, bool keep ) {
  if ( length==desiredLength ) return;
  if ( desiredLength == 0 ) {
   if ( list ) {
    delete[] list;
    list=null;
   }
   length=0;
   return;
  }
  if ( list ) {
   T *newlist;
   try { newlist= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
	   return;
   }
   int kept=(int) (length < desiredLength ? length : desiredLength);
   for ( int i=0; i<kept; i++ ) newlist[i]=list[i];
   delete[] list;
   length=desiredLength;
   list=newlist;
  } else {
   length=desiredLength;
   try { list= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
    return;
   }
  }
 }
 void Size( size_t desiredLength ) {
  if ( length==desiredLength ) return;
  if ( desiredLength == 0 ) {
   if ( list ) delete[] list;
   list=null;
   length=0;
   w=0;
   h=0;
   return;
  }
  if ( list ) {
   delete[] list;
   try { list= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
    return;
   }
  } else {
   try { list= new T[desiredLength]; } catch (bad_alloc& ba) {
    OUTPUT("NEW: bad_alloc caught: %s requesting %d\n", ba.what(), (int) (sizeof(T)*desiredLength) );
    return;
   }
  }
  length=desiredLength;
 }
#pragma warning( pop )
 T *Element( unsigned int index ) {
  return &list[index];
 }
 T *Element( unsigned int x, unsigned int y ) {
  return &list[x+y*w];
 }
 T *Element( unsigned int index, bool boundaryCheck ) {
  if ( index >= (unsigned int) length ) return null;
  return &list[index];
 }
 T *Element( unsigned int x, unsigned int y, bool boundaryCheck ) {
  unsigned int index=x+y*(unsigned int)w;
  if ( index > (unsigned int) length ) return null;
  return &list[index];
 }
 T Value( unsigned int index ) {
  return list[index];
 }
 T Value( unsigned int x, unsigned int y ) { // unbounded
  return list[x+y*w];
 }
 T Value( unsigned int index, bool boundaryCheck ) {
  if ( index >= (unsigned int) length ) return oob;
  return list[index];
 }
 T Value( unsigned int x, unsigned int y, bool boundaryCheck ) {
  int linear=x+y*w;
  if ( linear > (unsigned int) length ) return oob;
  return list[linear];
 }
 unsigned int _xy(unsigned int x, unsigned int y) { return x+y*w; }
 T *_get(unsigned int x, unsigned int y) { return &list[x,y]; }
 T &Wrap(int index) {
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
  if ( length == 0 ) {
   OUTPUT("Warning: length was 0 when Indexed<T>.Wrap() was called\n");
   return oob;
  }
#endif
  return (*this)[absi(index)%length];
 }
 T &operator[] (unsigned int index) {
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
  if ( index >= (unsigned int) length ) {
   OUTPUT("Warning: %d was out of bounds when requested on type %s\n",
    (int) index, typeid(*this).name());
   return oob;
  }
#endif
  return list[index];
 }
 T &operator() (unsigned int x, unsigned int y) {
  unsigned int linear=(unsigned int)(x+y*w);
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
  if ( linear >= (unsigned int) length ) {
   OUTPUT("Warning: %d (%d,%d) was out of bounds when requested on type %s\n",
    linear, (int)x, (int)y, typeid(*this).name());
   return oob;
  }
#endif
  return list[linear];
 }
 T &operator() (unsigned int index) {
  return list[index];
 }
 ~Indexed() {
  if ( list ) delete[] list;
 }
};

void Indexed_Rigorous_Test();