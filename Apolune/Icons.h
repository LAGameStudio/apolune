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
