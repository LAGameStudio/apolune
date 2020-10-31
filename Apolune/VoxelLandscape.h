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
#include "VBO.h"

struct VoxelColumn {
 float r,g,b,a;
 double height;
 void fromCrayon( Crayon *c ) {
  r=c->rf;
  g=c->gf;
  b=c->bf;
  a=c->af;
 }
 void fromCrayon( Crayon c ) {
  r=c.rf;
  g=c.gf;
  b=c.bf;
  a=c.af;
 }
};

class VoxelLandscapeVC {
public:
 Cartesian size;
 VBODynamicVC vboVC;
 matrix2d<VoxelColumn> columns;
 void ModifyCircle( int x, int y, int r ) {
 }
 void ModifySlopedConcentricCircle( int x,  int y, int r ) {
 }
 void FillWithNoise( int w, int h ) {
  columns.resize(w,h);
  size.Set(w,h);
  for ( int i=0; i<w; i++ ) for ( int j=0; j<h; j++ ) {
   VoxelColumn *c=&columns(i,j);
   c->height=uniform();
   c->fromCrayon(crayons.Any());
  }
 }
 void RemakeVBO() {
  if ( vboVC.uploaded ) vboVC.Release();
  for ( int i=0; i<size.x; i++ ) for ( int j=0; j<size.y; j++ ) {
   // Generate a bottomless box for each column with corners set to neighbor colors.
  }
 }
};

