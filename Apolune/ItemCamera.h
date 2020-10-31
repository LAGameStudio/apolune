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
#include "LookAtPerspective.h"

class ItemCamera : public ListItem {
public:
 Zstring name;
 LookAtPerspective lap;
 Cartesiand position,rotation;
 ItemCamera() : ListItem() {
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
 }
 void Reset() {
  lap.Reset();
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  position.SetRect(0,0,0,0);
  rotation.SetRect(0,0,0,0);
 }
 void Set( LookAtPerspective *lap, Cartesiand *position, Cartesiand *rotation ) {
  this->lap.Set(lap);
  this->position.Set(position);
  this->rotation.Set(rotation);
 }
 string toString() {
  return FORMAT( "name {%s} lap {%s} position {%s} rotation {%s}", 
   name.c_str(),
   lap.toString().c_str(),
   position.toString().c_str(),
   rotation.toString().c_str()
  );
 }
 void fromString(const char *s) {
  Zstring string(s);
  const char *w=string.Next();
  while ( *w != '\0' ) {
   if ( !str_cmp(w,"name") ) {
    w=string.Next();
    name=w;
   } else if ( !str_cmp(w,"lap") ) {
    w=string.Next();
    lap.fromString(w);
   } else if ( !str_cmp(w,"position") ) {
    w=string.Next();
    position.fromString(w);
   } else if ( !str_cmp(w,"rotation") ) {
    w=string.Next();
    rotation.fromString(w);
   } else { // Ignore old / defunct entries
    OUTPUT("ItemCamera: Invalid key: `%s`\n", w);
	   w=string.Next();
    OUTPUT("ItemCamera: Invalid value: `%s`\n", w);
   }
   w=string.Next();
  }
 }
};