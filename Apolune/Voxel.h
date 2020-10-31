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
#include "macros.h"
#include "moremath.h"
#include "ZeroTypes.h"
#include "Indexed.h"
#include "Display.h"
#include "Vertex.h"
#include "Vertexd.h"
#include "VBO.h"
#include "Model.h"

#define DEFAULT_UNIT_PER_VOXEL_x  (1.0/3.0)
#define DEFAULT_UNIT_PER_VOXEL_y  (1.0/3.0)
#define DEFAULT_UNIT_PER_VOXEL_z  (1.0/5.0)


// 16*16*16 = 4096 Voxels Per Slice
#define DEFAULT_SLICE_RESOLUTION 16

#define DEFAULT_SLICE_UNIT_SIZE_X (DEFAULT_UNIT_PER_VOXEL_x * DEFAULT_SLICE_RESOLUTION)
#define DEFAULT_SLICE_UNIT_SIZE_Y (DEFAULT_UNIT_PER_VOXEL_y * DEFAULT_SLICE_RESOLUTION)
#define DEFAULT_SLICE_UNIT_SIZE_Z (DEFAULT_UNIT_PER_VOXEL_z * DEFAULT_SLICE_RESOLUTION)

// 16*16*16 = 4096 Slices for 256x256x256 Resolution At 16777216 (16MB) Voxels
#define DEFAULT_WORLD_RESOLUTION 16

#define DEFAULT_WORLD_UNIT_SIZE_X (DEFAULT_SLICE_UNIT_SIZE_X * DEFAULT_WORLD_RESOLUTION)
#define DEFAULT_WORLD_UNIT_SIZE_Y (DEFAULT_SLICE_UNIT_SIZE_Y * DEFAULT_WORLD_RESOLUTION)
#define DEFAULT_WORLD_UNIT_SIZE_Z (DEFAULT_SLICE_UNIT_SIZE_Z * DEFAULT_WORLD_RESOLUTION)

/*
 * A unit Voxel
 *
 * Voxel neighbor tree: 
 *
 *    [17][18][19]
 *   [20][21][22]
 *  [23][24][25]
 *
 *    [ 9][10][11]
 *   [12]SELF[13]
 *  [14][15][16]
 *
 *   [ 0][ 1][ 2]
 *  [ 3][ 4][ 5]
 * [ 6][ 7][ 8]
 */
class Voxel : public ListItem {
public:
 unsigned int x,y,z;  // Lookup
 Vertexd Lx,Ly,Lz;       // Location
 bool filled;         // Solid?
 Zuint group;         // 0 = ungrouped, base group
 Voxel* neighbors[9+8+9]; // Neighbor tree, 0-8 floor, 9-16 eye, 17-25 ceiling
 Voxel() : ListItem() {
  filled=false;
  x=0; y=0; z=0;
  InitNeighbors();
 }
 void InitNeighbors() {
  unsigned int i=0;
  for ( ; i<(9+8+9); i++ ) {
   neighbors[i]=null;
  }
 }
 void Fill() { filled=true; }
 void Empty() { filled=false; }
 void asTriSphere( Polygons *out, int resolution ) {
 }
 void asTriCube( Polygons *out ) {
 }
 void fromString ( const char *ch ) {}
 string toString() { return string(""); }
};

/*
 * A selection of voxels from a chunk or chunks.
 */
HANDLES(Voxel,VoxelHandle,VoxelHandles,"voxel");

/*
 * Series of voxels are held in a chunk, a slice of the world.
 * Slices are the structure on which a VBO is assembled.  Slices
 * have unit voxels of a given width, where usually w=h, that
 * make up a Voxel Object.  A series of slice-related VBOs can
 * be queried and rendered if they fall inside the viewable
 * region.  When a slice is modified, it must be rebuilt.
 */
class Voxels : public ListItem {
public:
 Indexed<Voxel> v;
 Zuint width,height,depth;
 Zbool empty,solid;
 Zdisposable<VBOStaticV> vbo;
 Voxels() : ListItem() {
  v.Size(DEFAULT_SLICE_RESOLUTION*DEFAULT_SLICE_RESOLUTION*DEFAULT_SLICE_RESOLUTION);
  width=DEFAULT_SLICE_RESOLUTION;
  height=DEFAULT_SLICE_RESOLUTION;
  depth=DEFAULT_SLICE_RESOLUTION;
  empty=true;
 }
 Voxels( unsigned int w, unsigned int h, unsigned int d ) : ListItem() {
  v.Size(w*h*d);
  width=w;
  height=h;
  depth=d;
  empty=true;
 }
 Voxel *Get( unsigned int x, unsigned int y, unsigned int z ) {
  unsigned int position=x+y*width.value+z*width.value*height.value;
  return &v.list[position];
 }
 Voxel *Get( unsigned int position ) {
  return &v.list[position];
 }
 bool Solid( unsigned int x, unsigned int y, unsigned int z ) {
  Voxel *result=Get( x,y,z );
  return result->filled;
 }
 void Fill( unsigned int x, unsigned int y, unsigned int z ) {
  Voxel *result=Get( x,y,z );
  result->filled=true;
  empty=false;
 }
 unsigned int Filled() {
  unsigned int result=0;
  unsigned int i=0;
  for ( ; i<v.length; i++ ) {
   Voxel *voxel=Get(i);
   if ( voxel->filled ) result++;
  }
  return result;
 }
 // Reconnect Voxel Tree for a single chunk (connect fills)
 // neighbors: a 9+8+9 tree node set
 void RebuildNeighborTree( Indexed<Voxel*> neighbors ) {
 }
 // Rebuild the slice VBO
 void Rebuild() {
  vbo.Recycle();
 }
};

/*
 * A group of Voxels which forms a "Voxel Model" or "Moxel"
 */
class VoxelCreation : public ListItem {
public:
 Indexed<Voxels> slices;
 Zuint width,height,depth;
 Vertexd brushAxis;  // The stored coordinate of the voxel that encompasses the origin from the original model
 Vertexd centroid;   // Voxel coordinate (not actual)
 Vertexd extents;
 Vertexd perVoxel;
 // Create an empty voxel canvas of maximum size.
 VoxelCreation() : ListItem() {
  SetExtents(
    DEFAULT_WORLD_RESOLUTION,DEFAULT_WORLD_RESOLUTION,DEFAULT_WORLD_RESOLUTION,
    DEFAULT_UNIT_PER_VOXEL_x,DEFAULT_UNIT_PER_VOXEL_y,DEFAULT_UNIT_PER_VOXEL_z
  );
 }
 VoxelCreation( unsigned int w, unsigned int h, unsigned int d ) : ListItem() {
  SetExtents(w,h,d,DEFAULT_UNIT_PER_VOXEL_x,DEFAULT_UNIT_PER_VOXEL_y,DEFAULT_UNIT_PER_VOXEL_z);
 }
 // Shrinks the incoming model to the DEFAULT_OBJECT_RESOLUTION if the model is too large
 // If the model is smaller than the DEFAULT_OBJECT_RESOLUTION, the voxel world is shrunk
 // to the size of the object (inclusive).  Defines the "brush axis" as the origin
 // This is the intake for voxel brushes described as polyhedrons
 VoxelCreation( ImmediateModel *in ) : ListItem() {
  ImmediateModel m;
  m.Duplicate(in);
  m.Stretch(1.0,1.0,1.0);
  m.BoxBounds();
 }
 void SetUnitVoxel( double vw, double vh, double vd ) {
  perVoxel.Set(vw,vh,vd);
  extents.Set(width*vw,height*vh,depth*vd);
 }
 void SetExtents( unsigned int w, unsigned int h, unsigned int d ) {
  this->width=w;
  this->height=h;
  this->depth=d;
 }
 void SetExtents( unsigned int w, unsigned int h, unsigned int d, double vw, double vh, double vd ) {
  SetExtents(w,h,d);
  SetUnitVoxel(vw,vh,vd);
  UpdateExtents();
 }
 void UpdateExtents() {

 }
 Voxels *Get( unsigned int x, unsigned int y, unsigned int z ) {
  unsigned int p=x+y*width.value+z*width.value*height.value;
  if ( p < slices.length ) return &slices.list[p];
  return null;
 }
 Indexed<Voxels *>neighbors;
 void GatherNeighbors( unsigned int x, unsigned int y, unsigned int z ) {
  neighbors.Size(9+8+9);
  neighbors[0]=Get(x,y,z);
 }
 void LinkSlices( unsigned int x, unsigned int y, unsigned int z ) {
 }
 void Union( VoxelCreation *b ) {
 }
 void Subtraction( VoxelCreation *b ) {
 }
 void Intersection( VoxelCreation *b ) {
 }
 // Adds the polyhedron to the voxel world
 void Union( ImmediateModel *m ) {

 }
 void Subtraction( ImmediateModel *m ) {
 }
 void Intersection( ImmediateModel *m ) {
 }
 void toModel( ImmediateModel *m ) {
 }
 void Load( const char *filename ) {
 }
 void Save( const char *filename ) {
  BinaryFile *file=new BinaryFile(filename,true);
  file->scrawl(string("Moxel"));
  file->scrawl(width.value);
  file->scrawl(height.value);
  file->scrawl(depth.value);
  int zero=0;
  int i=0;
  for ( ; i<80; i++ ) file->write(&zero);
  delete file;
 }
 // Renders all voxels indiscriminantly
 void Render() {
 }
 // Renders only slices that fall within the view frustum
 void Render( Vertexd *eye, Vertexd *direction, double near, double far ) {
 }
};
