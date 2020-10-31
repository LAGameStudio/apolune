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
 ***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once
#include <string>

#include "ListItem.h"
#include "LinkedList.h"
#include "Material.h"

extern Materials materials;

class Substance : public ListItem {
public:
 string element_name;
 string material_name;
 Material *material;
 float Value;
 Substance() {
  material=&materials.defaultMaterial;
  material_name=string("");
  element_name=string("");
  Value=1.0f;
 }
 void Assign( const char *n, float v, Material *mat ) {
  element_name=string(n);
  material_name=string(mat->name);
  material=mat;
  Value=v;
 }
};

class Substances : public LinkedList {
public:
 Substance *Random() { return (Substance *) Element(upto(count)); }
 Substance *Random(int seed) { return (Substance *) Element(seed%count); }
 Substance *Random(MaterialCategory category) {
  Substance *s=(Substance *) Element(upto(count));
  while ( s->material->category != category ) s=(Substance *) Element(upto(count));
  return s;
 }
 Substance *Add( Material *m ) {
  Substance *s=new Substance;
  s->material=m;
  Append(s);
  return s;
 }
 Substance *find( const char *match ) {
  FOREACH(Substance,s) if ( !str_cmp( s->element_name.c_str(), match ) ) return s;
  return null;
 }
 Substance *findByMaterial( const char *match ) {
  FOREACH(Substance,s) if ( !str_cmp( s->material->name.c_str(), match ) ) return s;
  OUTPUT("Substance::findByMaterial `%s` not found\n",match);
  return null;
 }
 Substance *findPrefix( const char *pre ) {
  FOREACH(Substance,s) if ( !str_prefix( pre, s->element_name.c_str() ) ) return s;
  return null;
 }
 Substance *CreateWorldShader( const char *name, const char *fn, int num_lights, float value );
 CLEARLISTRESET(Substance);
 ~Substances() { Clear(); }
};

extern Substances substances;