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
//
// DynamicCamera.h
#include "macros.h"
#include "Vertex.h"
#include "Matrix.h"

class DynamicCamera {
public:
 Vector sideVec,upVec,dirVec;
 GLdouble Near,Far;
 Vector	camSrc, camVel, camAng;
 float	angSpeed, moveSpeed, newAngSpeed;
 float	camFov, camRoll;
 Matrix	viewMat;
 DynamicCamera();  
 DynamicCamera(Vector &vecSrc);
 bool Keys();
 void View();
 void Between();
};