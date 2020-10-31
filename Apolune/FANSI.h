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

#include "Crayon.h"

extern const int fANSI[256][3];

class fANSIClass {
public:
 Indexed<Crayon> color;
 fANSIClass() {
  color.Size(256);
  for ( unsigned int i=0; i<256; i++ )
   color(i).Int(fANSI[i][0],fANSI[i][1],fANSI[i][2],255);
 }
 unsigned int Near( int r, int g, int b ) {
  unsigned int candidate=0;
  float totald=255.0f*3.0f;
  for ( unsigned int i=0; i<256; i++ ) {
   float td=fdistance(
    (float)r,(float)g,(float)b,
    (float) color(i).r,
    (float) color(i).g,
    (float) color(i).b
   );
   if ( td < totald ) {
    candidate=i;
    totald=td;
   }
  }
  return candidate;
 }
 unsigned int Near( float r, float g, float b ) {
  unsigned int candidate=0;
  float totald=255.0f*3.0f;
  for ( unsigned int i=0; i<256; i++ ) {
   float td=fdistance(
    r,g,b,
    color(i).rf,
    color(i).gf,
    color(i).bf
   );
   if ( td < totald ) {
    candidate=i;
    totald=td;
   }
  }
  return candidate;
 }
 unsigned int Near( Crayon c ) {
  return Near(c.rf,c.gf,c.bf);
 }
};

extern fANSIClass FANSI;