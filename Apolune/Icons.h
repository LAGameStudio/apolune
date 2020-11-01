/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "GLImage.h"
#include "AtlasTileset.h"

ONE(Icon,{ tint.Pick(alabaster); b=transparency; })
 Zp<GLImage> image;
 Zp<AtlasTile> at; Zp<AtlasTileset> ats;
 Zstring label;
 Zint integer;
 Zdouble decimal;
 Zint type;
 Zstring other,alt;
 Blends b;
 Crayon tint;
MANY(Icon,IconHandle,IconHandles,"Icon",Icons,{})
 GLImage *GetIcon( having invis, int i ) {
  Icon *a=(Icon *) Element(i);
  if ( a ) return a->image ? a->image : invisiblePNG;
  return invisiblePNG;
 }
 GLImage *GetIcon( int i ) {
  Icon *a=(Icon *) Element(i);
  if ( a ) return a->image;
  return null;
 }
 Icon *Add( const char *imagefile ) {
  Icon *a=new Icon;
  a->image=library.Load(imagefile);
  Append(a);
  return a;
 }
 Icon *Add( GLImage *i, const char *s ) {
  Icon *a=new Icon;
  a->image=i;
  a->label=s;
  Append(a);
  return a;
 }
 Icon *Add( GLImage *i ) {
  Icon *a=new Icon;
  a->image=i;
  Append(a);
  return a;
 }
DONE(Icon);
