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
#include "ListItem.h"
#include "LinkedList.h"

class Collar : public ListItem {
 unsigned int _references;
 Collar() : ListItem() { _references=0; }
 void Leash() {
  _references++;
 }
 void Unleash() {
#if defined(_DEBUG)
  if ( _references > 0 ) {
   OUTPUT("Collar::Dereference() warns: tried to dereference when no references were made!\n");
  } else
#endif
  _references--;
 }
};

template<class T> class Leash;
template<class T> class Recyclables;
template<class T>
class Recyclable : public Collar {
public:
 friend Recyclables<T>;
 Recyclables<T> *bin;
 unsigned int _recycled;
 Recyclable( Recyclables<T> *bin ) : Collar() {
  _recycled=0;
  this->bin=bin;
 }
 void Recycle() {
  bin->Append(this);
  _recycled++;
 }
 void Upcycle() {
  references=0;
  Upcycled();
 }
 virtual void Upcycled() {}
};

template<class T>
class Leash : public ListItem {
public:
 int recycled_on_leash; // Keeps track of the current recycle generation for later comparison
 Recyclable<T> *p;
 Leash() {
  p=null;
 }
 Leash( Recyclable<T> *p ) {
  this->p=p;
  p->Leash();
 }
 ~Leash() {
  if ( p ) {
   p->Unleash();
  }
 }
 void AttachLeash( Recyclable<T> *p ) {
#if defined(_DEBUG)
  if ( p  ) {
   OUTPUT("Leash::AttachLeash() warns that you are attaching a Leash that is already attached to a Collar.\n");
  }
#endif
  this->p=p;
  this->p->Leash();
  recycled_on_leash=this->p->_recycled;
 }
 void Releash( Recyclable<T> *p ) {
  if ( !p ) {
#if defined(_DEBUG)
   OUTPUT("Leash::Releash() warns that you are releashing a Leash that is not attached to anything.\n");
#endif
  } else this->p->Unleash();
  this->p=p;
  recycled_on_leash=this->p->_recycled;
 }
 void Unleash() {
  if ( !p ) {
#if defined(_DEBUG)
   OUTPUT("Leash::Unleash() warns that you are unleashing a Leash that has not been attached to anything.\n");
#endif
  } else {
   this->p->Unleash();
   this->p=null;
   recycled_on_leash=this->p->_recycled;
  }
 }
 T* operator->() {
#if defined(_DEBUG)
  if ( !p ) {
   OUTPUT("Leash::operator->() was used but Leash is not attached to a Collar.\n");
  } else
  if ( p->_recycled != recycled_on_leash ) {
   OUTPUT("Leash::operator->() warns that this Leash is pointing to an object that has been recycled since it was leashed!\n");
  }
#endif
  return p;
 } 
};

template<class T>
class Recyclables : public LinkedList {
public:
 T *Upcycle() {
  if ( count == 0 ) return new T;
  T *candidate=first;
  Remove(first);
  return first;
 }
 void Recycle( T *p ) {
  p->Recycle();
 }
};