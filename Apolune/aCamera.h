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
/************************************************************
*	APRON TUTORIALS PRESENTED BY MORROWLAND					*
*************************************************************
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
*	Version					: English (UK)					*
************************************************************/

#include "GLWindow.h"
#include "GLImage.h"
#include "TextureLibrary.h"

#define CAMERASPEED	0.03f				// The Camera Speed

//////////////////////////////////////
//The tVector3 Struct
//////////////////////////////////////
typedef struct tVector3	{			
	tVector3() { x=y=z=0.0f; }	// constructor
	tVector3 (float new_x, float new_y, float new_z) // initialize constructor	 
	{x = new_x; y = new_y; z = new_z;}
	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}
	float x, y, z;						// 3D vector coordinates
} tVector3;

//////////////////////////////////////
//The CCamera Class
//////////////////////////////////////
class aCamera {
public:
		tVector3 mPos;	
		tVector3 mView;		
		tVector3 mUp;

        Cartesian oldMouse;
		int mWindowWidth, mWindowHeight;
        void MovedMouseTo( int mx, int my );

		void Rotate_View(float x, float y, float z);
		void Move_Camera(float cameraspeed);
		void Position_Camera(float pos_x, float pos_y,float pos_z,
			 				 float view_x, float view_y, float view_z,
							 float up_x,   float up_y,   float up_z);
};