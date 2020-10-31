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

#include <Windows.h>
#include <math.h>
#include "moremath.h"
#include "Model.h"
#include "Primitives3D.h"

#define SRES 512

void plasmize( int hw, int s, float *map );
void smoothize( float *Map, int radius, int hw );

void pregenerateCubeToSphereMap();

struct C2Smap {
public:
 float x,y,z;
 float lambda,phi;
 C2Smap() { lambda=phi=x=y=z=0.0f; }
};

extern C2Smap C2S[SRES*SRES];

class HeightMap
{
public:
 float map[SRES*SRES];
 ImmediateModel *sphered;

 HeightMap(void) {
  sphered=null;
  for ( int x=0; x<SRES; x++ )
    for ( int y=0; y<SRES; y++ ) map[x+y*SRES]=0.0f;
 }

 void toSphere( float radius );
 void Generate( int s );
 
 ~HeightMap(void) {
  if (sphered) delete sphered;
 }
};