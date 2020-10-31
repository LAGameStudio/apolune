#pragma once


#include "Art.h"

#include "ZIndexed.h"
#include "Vertexd.h"

enum VV_CubePoints {
 vv_TLF=0, // Top Left Front
 vv_TRF=1, // Top Right Front
 vv_BRF=2, // Bottom Right Front
 vv_BLF=3, // Bottom Left Front
 vv_BLB=4, // Bottom Left Back
 vv_BRB=5, // Bottom Right Back
 vv_TRB=6, // Top Right Back
 vv_TLB=7  // Top Left Back
};

#define VV_TC_LEFT   (0.0)
#define VV_TC_RIGHT  (1.0)
#define VV_TC_TOP    (0.0)
#define VV_TC_BOTTOM (1.0)
#define VV_TC_FRONT  (1.0)
#define VV_TC_BACK   (0.0)

class VVCubeNode {
public:
 ZIndexed<Crayon> color;
};

class VVoxel {
public:
 VVCubeNode cube;
};

class VVNeighborQuery {
public:
 Zp<VVCubeNode> self,
  TLF,TRF,TRB,TLB,
  BRF,BLF,BLB,BRB,
  L, R, U, D, B, F;
};

class VVVoxelModel : public ListItem {
public:
 ZIndexed<VVCubeNode> node;
 VVVoxelModel() : ListItem() {
 }
};