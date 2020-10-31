#include "VVoxel.h"

ZIndexed<Coord2d> vv_local_tc;

void InitVVLocalTextureCoordinates() {
 vv_local_tc.Size(8);
 for ( int i=0; i<8; i++ ){
  // Top Left Front
  // Top Right Front
  // Bottom Right Front
  // Bottom Left Front
  // Bottom Left Back
  // Bottom Right Back
  // Top Right Back
  // Top Left Back
 }

}
