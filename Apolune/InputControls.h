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
#include "Interface.h"
#include "Enumerated.h"

extern Interface input;

class InputControl : public ListItem {
public:
 KeyList keys;
 Zstring name;
 Zdouble duration;
 Zbool wasPressed;
 bool pressed( KeyList *in ) {
  Indexed<Zbool> found;
  found.Size( in->count );
  Zi i;
  EACH(keys.first,KEY,j) {
   EACH(in->first,KEY,k) if ( k->code == j->code ) found[i]=true;
   i++;
  }
  bool result=true;
  for ( i=0; i<(int)found.length; i++ ) result&=found[i];
  return result;
 }
 bool pressed( KeyList *in, bool exclusively ) {
  if (keys.count != in->count) return false;
  return pressed(in);
 }
};

class InputControls : public LinkedList {
public:
 Zstring profile;
 InputControls() : LinkedList() {}
 InputControl *find( const char *name ) {
  FOREACH(InputControl,c) if ( c->name == name ) return c;
  OUTPUT("InputControls:find could not find `%s`\n",name);
  return null;
 }
 bool pressed( const char *name ) {
  InputControl *control=find(name);
  if ( control ) return control->pressed(&input.keys);
 }
 bool pressed( const char *name, bool exclusively ) {
  InputControl *control=find(name);
  if ( control ) return control->pressed(&input.keys,true);
 }
 bool pressed( string name ) {
  InputControl *control=find(name.c_str());
  if ( control ) return control->pressed(&input.keys);
 }
 bool pressed( string name, bool exclusively ) {
  InputControl *control=find(name.c_str());
  if ( control ) return control->pressed(&input.keys,true);
 }
 CLEARLISTRESET(InputControl);
};

class InputControlProfiles : public LinkedList {
public:
 CLEARLISTRESET(InputControls);
};

extern InputControlProfiles inputControlProfiles;