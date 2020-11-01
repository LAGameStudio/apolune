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