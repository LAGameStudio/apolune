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

//Wraps: enum cars { ford, volvo, saab, subaru, toyota };

#include "LinkedList.h"
#include "Indexed.h"
#include "Strings.h"
#include "SafeMediaLookup.h"

template <typename T>
class Enumerated { public:
 LinkedList dictionary; // for storing other associated lookups
 Indexed<Zstring> strings;
 SafeImageLookupInt images;
 SafeSoundLookup sounds;
 T value;
 void Values( unsigned int total ) {
  strings.Size(total);
 }
 void SetName( T value, const char *name ) {
  unsigned int i=(unsigned int) value;

 }
 void SetImage( T value, const char *name ) {
  unsigned int i=(unsigned int) value;
 }
 void SetSound( T value, const char *name ) {
  unsigned int i=(unsigned int) value;
 }
 void Define( T value, const char *name ) {
  unsigned int i=(unsigned int) value;
 }
 Enumerated& operator() (T value) {
  this->value=value;
 }
};