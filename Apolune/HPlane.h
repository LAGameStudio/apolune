#pragma once
/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/


#ifndef _HPLANE_H_
#define _HPLANE_H_

#include "HVertex.h"

enum RELATION {BACK = -1, PLANAR = 0, FRONT = 1, CUTS = 2};

struct HPlane {
	HVertex normal;
	float offset;

	HPlane(){}
	HPlane(const HVertex &Normal, const float Offset);
	HPlane(const float a, const float b, const float c, const float d);
	HPlane(const HVertex &v0, const HVertex &v1, const HVertex &v2);

	void normalize();
	float distance(const HVertex &v) const;
	float getPlaneHitInterpolationConstant(const HVertex &v0, const HVertex &v1) const;
	RELATION getVertexRelation(const HVertex &v, float planarDiff = 0.03f) const;
};

#endif
