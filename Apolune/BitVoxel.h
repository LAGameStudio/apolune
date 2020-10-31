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

/**************************************************************
  Author: H. Elwood Gilliland III
  Simple voxel class
 **************************************************************/

#include "ZeroTypes.h"
#include "Indexed.h"
#include "Vertex.h"
#include "Vertexd.h"
#include "VBO.h"
#include "Model.h"

#include <bitset>

/*
 * A Lightweight Voxel Class based on an 8-bit segment, organized into a 1x8x1 byte block (1x8x1 voxels),
 * from a world sliced by 2x16x16 bytes representing cubic regions of 16x16x16 voxels (4096 voxels)
 */

class BitVoxelSlice {
public:
 Indexed<Coord3ui> voxel_lookup;
 Indexed<bool> query;
 VBOStaticVNT vbo;
 BitVoxelSlice() { }
 void Construct(
  unsigned int sx, unsigned int sy, unsigned int sz, // lower left rear point
  Coord3ui *voxelsPerSlice
 ) {
  voxel_lookup.Size(voxelsPerSlice->w*voxelsPerSlice->h*voxelsPerSlice->d);
  query.Size(voxelsPerSlice->w*voxelsPerSlice->h*voxelsPerSlice->d);
  unsigned int i,j,k;
  for ( i=0; i<voxelsPerSlice->w; i++ ) {
   for ( j=0; j<voxelsPerSlice->h; j++ ) {
    for ( k=0; k<voxelsPerSlice->d; k++ ) {
     unsigned int v=i+j*voxelsPerSlice->w+k*voxelsPerSlice->w*voxelsPerSlice->h;
     voxel_lookup[v].w=i+sx*voxelsPerSlice->w;
     voxel_lookup[v].h=j+sy*voxelsPerSlice->h;
     voxel_lookup[v].d=k+sz*voxelsPerSlice->d;
     query[v]=false;
    }
   }
  }
 }
};

class BVoxel {
public:
 Zbool filled;
};

class BitVoxel {
public:
 Indexed<BVoxel> map;
 Vertexd position,offset;  // Position in unit space
 Vertexd unitVolume;       // Size of voxel world boundaries in GL world space
 Vertexd unitsPerVoxel;    // Delta units
 Coord3ui voxelsPerSlice;  // Extents of the voxel world slice
 Coord3ui slicesPerVolume; // Extents of the slice library
 Coord3ui voxels;
 Zuint dimVoxel;
 Zuint dimSlice;
 Zuint dimVoxelVolume;
 Indexed<BitVoxelSlice> slices;
 BitVoxel() { SetExtents(100.0,100.0,100.0,16,16,16,8,8,8); }
 BitVoxel(   double uw, double uh, double ud, 
  unsigned int vw, unsigned int vh, unsigned int vd, 
  unsigned int sw, unsigned int sh, unsigned int sd ) {
  SetExtents(uw,uh,ud,vw,vh,vd,sw,sh,sd);
 }
 void SetExtents(
  double uw, double uh, double ud, 
  unsigned int vw, unsigned int vh, unsigned int vd, 
  unsigned int sw, unsigned int sh, unsigned int sd ) {
  unitVolume.Set(uw,uh,ud);
  voxelsPerSlice.w=vw;
  voxelsPerSlice.h=vh;
  voxelsPerSlice.d=vd;
  slicesPerVolume.w=sw;
  slicesPerVolume.h=sh;
  slicesPerVolume.d=sd;
  voxels.w=voxelsPerSlice.w*slicesPerVolume.w;
  voxels.h=voxelsPerSlice.h*slicesPerVolume.h;
  voxels.d=voxelsPerSlice.d*slicesPerVolume.d;
  dimVoxel=voxelsPerSlice.w*voxelsPerSlice.h*voxelsPerSlice.d;
  dimSlice=slicesPerVolume.w*slicesPerVolume.h*slicesPerVolume.d;
  dimVoxelVolume=voxels.w*voxels.h*voxels.d;
  unitsPerVoxel.x=unitVolume.x/slicesPerVolume.w/voxelsPerSlice.w;
  unitsPerVoxel.y=unitVolume.y/slicesPerVolume.h/voxelsPerSlice.h;
  unitsPerVoxel.z=unitVolume.z/slicesPerVolume.d/voxelsPerSlice.d;
  map.Size(voxels.w*voxels.h*voxels.d);
 }
 void Add( ImmediateModel *m, Vertexd *position, Vertexd *scale ) {
  Vertexd positionScaled(position);
  positionScaled.Multiply(scale);
  Vertexd endPosition;
  endPosition.Set(m->boxMagnitude.x,m->boxMagnitude.y,m->boxMagnitude.z);
  endPosition.Multiply(scale);
  endPosition.Add(position);
  // Figure out the start voxel
  Coord3ui start;
  start.Set(
   (int)(positionScaled.x/unitsPerVoxel.x),
   (int)(positionScaled.y/unitsPerVoxel.y),
   (int)(positionScaled.z/unitsPerVoxel.z)
  );
  // Figure out the target area dimensions in voxels
  Coord3ui end;
  end.Set(
   (int)(endPosition.x/unitsPerVoxel.x),
   (int)(endPosition.y/unitsPerVoxel.y),
   (int)(endPosition.z/unitsPerVoxel.z)
  );
  end.w+=1; end.h+=1; end.d+=1;
  // Figure out the voxel dimensions of the object;
  if ( start.w < 0 ) start.w=0;
  if ( start.h < 0 ) start.h=0;
  if ( start.d < 0 ) start.d=0;
  if ( end.w > voxels.w ) end.w=voxels.w;
  if ( end.h > voxels.h ) end.h=voxels.h;
  if ( end.d > voxels.d ) end.h=voxels.d;
  Coord3ui voxDims;
  voxDims.Set(end.w-start.w,end.h-start.h,end.d-start.d);
  // Figure out the least position of the model
  Vertex *least=m->leastPoint();
  // Iterate
  Coord3ui vCoord,step;
  Vertexd probe;
  vCoord.Set(start.w,start.h,start.d);
  for ( step.w=0; step.w < voxDims.w; step.w++ ) {
   vCoord.h=start.h;
   for ( step.h=0; step.h < voxDims.h; step.h++ ) {
    vCoord.d=start.d;
    for ( step.d=0; step.d < voxDims.d; step.d++ ) {
     probe.Set(
      uiratiod(step.w,voxDims.w)*m->boxMagnitude.x,
      uiratiod(step.h,voxDims.h)*m->boxMagnitude.y,
      uiratiod(step.d,voxDims.d)*m->boxMagnitude.z
     );
     probe.Add(least);
     if ( m->PointInside(probe.x,probe.y,probe.z) ) {
      Set(vCoord.w,vCoord.h,vCoord.d,true);
     }
     vCoord.d++;
    }
    vCoord.h++;
   }
   vCoord.w++;
  }
  return;
 }
 void Reslice() {
  slices.Size((size_t)(int)dimSlice);
  unsigned int i,j,k;
  for ( i=0; i<slicesPerVolume.w; i++ ) for ( j=0; j<slicesPerVolume.h; j++ ) for ( k=0; k<slicesPerVolume.d; k++ ) {
   unsigned int v=i+j*slicesPerVolume.w+k*slicesPerVolume.w*slicesPerVolume.h;
   slices[v].Construct(i*voxelsPerSlice.w,j*voxelsPerSlice.h,k*voxelsPerSlice.d,&voxelsPerSlice);
   Query(i,j,k);
  }
  for ( i=0; i<slicesPerVolume.w; i++ ) for ( j=0; j<slicesPerVolume.h; j++ ) for ( k=0; k<slicesPerVolume.d; k++ ) {
   RebuildVBO(i,j,k);
  }
 }
 void Reslice( unsigned int sx, unsigned int sy, unsigned int sz ) {
  slices[(sx+sy*slicesPerVolume.w+sz*slicesPerVolume.w*slicesPerVolume.h)].Construct(sx,sy,sz,&voxelsPerSlice);
  Query(sx,sy,sz);
  RebuildVBO(sx,sy,sz);
 }
 void Query( unsigned int sx, unsigned int sy, unsigned int sz ) {
  unsigned int v=sx+sy*slicesPerVolume.w+sz*slicesPerVolume.w*slicesPerVolume.h;
  unsigned int i;
  for ( i=0; i<slices[v].voxel_lookup.length; i++ )
   slices[v].query[i] = Get(slices[v].voxel_lookup[i].w, slices[v].voxel_lookup[i].h, slices[v].voxel_lookup[i].d);
 }
 void RebuildVBO( unsigned int sx, unsigned int sy, unsigned int sz ) {
  Coord3ui xyz;
  xyz.w=sx*voxelsPerSlice.w;
  xyz.h=sy*voxelsPerSlice.h;
  xyz.d=sz*voxelsPerSlice.d;
  Vertexd sliceOffset(
   xyz.w*unitsPerVoxel.x+position.x+offset.x,
   xyz.h*unitsPerVoxel.y+position.y+offset.y,
   xyz.d*unitsPerVoxel.z+position.z+offset.z
  );
  unsigned int v=sx+sy*slicesPerVolume.w+sz*slicesPerVolume.w*slicesPerVolume.h;
  if ( slices[v].vbo.uploaded ) slices[v].vbo.Release();
  slices[v].vbo.Clear();
  unsigned int i,j,k;
  for ( i=0; i<voxelsPerSlice.w; i++ ) for ( j=0; j<voxelsPerSlice.h; j++ ) for ( k=0; k<voxelsPerSlice.d; k++ ) if ( Get(xyz.w+i,xyz.h+j,xyz.d+k) ) {
   Vertex vert;
   double _left  =sliceOffset.x+i*unitsPerVoxel.x;
   double _bottom=sliceOffset.y+j*unitsPerVoxel.y;
   double _near  =sliceOffset.z+k*unitsPerVoxel.z;
   double _right =_left+unitsPerVoxel.x;
   double _top   =_bottom+unitsPerVoxel.y;
   double _far   =_near+unitsPerVoxel.z;
   unsigned int m=0,n=0,o=0;
   bool filled=false;
   // Testing Adjacency
   // Decide if this node is has an outward facing edge, wind CCW
   m=i-1; n=j; o=k;  // Left
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add left cube face to VBO
    vert.x=(float)_left;  vert.y=(float)_bottom;   vert.z=(float)_near;    vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;      vert.z=(float)_far;     vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;      vert.z=(float)_near;    vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom;   vert.z=(float)_near;    vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom;   vert.z=(float)_far;     vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;      vert.z=(float)_far;     vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
   m=i+1; n=j; o=k;  // Right
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add right cube face to VBO, window CCW
    vert.x=(float)_right;  vert.y=(float)_bottom;  vert.z=(float)_near;   vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right;  vert.y=(float)_top;     vert.z=(float)_near;   vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right;  vert.y=(float)_top;     vert.z=(float)_far;    vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right;  vert.y=(float)_top;     vert.z=(float)_far;    vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right;  vert.y=(float)_bottom;  vert.z=(float)_near;   vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right;  vert.y=(float)_bottom;  vert.z=(float)_far;   vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
   m=i; n=j-1; o=k;  // Floor
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add bottom cube face to VBO
    vert.x=(float)_left;  vert.y=(float)_bottom;  vert.z=(float)_near;  vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_bottom;  vert.z=(float)_far;   vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom;  vert.z=(float)_far;   vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom;  vert.z=(float)_near;  vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_bottom;  vert.z=(float)_near;   vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_bottom;  vert.z=(float)_far;  vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
   m=i; n=j+1; o=k;  // Ceil
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add ceiling cube face to VBO
    vert.x=(float)_left;  vert.y=(float)_top;  vert.z=(float)_far;   vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;  vert.z=(float)_near;  vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;  vert.z=(float)_near;  vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;  vert.z=(float)_far;   vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;  vert.z=(float)_far;   vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;  vert.z=(float)_near;  vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
   m=i; n=j; o=k-1;  // Front
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add front cube face to VBO
    vert.x=(float)_left;  vert.y=(float)_bottom; vert.z=(float)_near; vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;    vert.z=(float)_near; vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;    vert.z=(float)_near; vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;    vert.z=(float)_near; vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom; vert.z=(float)_near; vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_bottom; vert.z=(float)_near; vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
   m=i; n=j; o=k+1;  // Back
   filled=Get(xyz.w+m,xyz.h+n,xyz.d+o);
   if ( !filled ) { // Add back cube face to VBO
    vert.x=(float)_left;  vert.y=(float)_bottom; vert.z=(float)_far;  vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_top;    vert.z=(float)_far;  vert.q=0.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;    vert.z=(float)_far;  vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_top;    vert.z=(float)_far;  vert.q=1.0f; vert.r=0.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_left;  vert.y=(float)_bottom; vert.z=(float)_far;  vert.q=0.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
    vert.x=(float)_right; vert.y=(float)_bottom; vert.z=(float)_far;  vert.q=1.0f; vert.r=1.0f; vert.Normal(); slices[v].vbo.PushVertex(&vert);
   }
  }
  if ( slices[v].vbo.data.size() > 0 ) {
   slices[v].vbo.Create();
   // Dispose of vbo's CPU indices
   slices[v].vbo.ClearCPU();
  }
 }
 bool Get( unsigned int x, unsigned int y, unsigned int z ) {
  unsigned int t=x+y*voxels.w+z*voxels.w*voxels.h;
  if ( t >= map.length || t < 0 ) return false;
  return map.list[t].filled;
 }
 /*
 bool Get( double x, double y, double z ) {
  // Calculate the position + offset translation
  Vertexd p(x-position.x-offset.x,y-position.y-offset.y,z-position.z-offset.z);
  p.Multiply(&unitsPerVoxel);
  return Get((unsigned int) p.x,(unsigned int) p.y, (unsigned int) p.z);
 }
 */
 void Set( unsigned int x, unsigned int y, unsigned int z, bool on ) {
  unsigned int t=x+y*voxels.w+z*voxels.w*voxels.h;
  if ( t >= map.length || t < 0 ) return; // No guarantee to be set
  map.list[t].filled=on;
 }
 /*
 void Set( double x, double y, double z, bool on ) {
  // Calculate the position + offset translation
  Vertexd p(x-position.x-offset.x,y-position.y-offset.y,z-position.z-offset.z);
  p.Multiply(&unitsPerVoxel);
  Set((unsigned int) p.x,(unsigned int) p.y, (unsigned int) p.z, on);
 }
 */
 void Render() {
  unsigned int i;
  for ( i=0; i<slices.length; i++ ) if ( slices[i].vbo.uploaded ) slices[i].vbo.Render();
 }
};