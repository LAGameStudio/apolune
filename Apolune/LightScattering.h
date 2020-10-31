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

#if defined(USE_PBUFFER)

#include "macros.h"
#include "Lights.h"
#include "GLWindow.h"
#include "Material.h"
#include "Camera.h"
#include "Film.h"
#include "pBuffer.h"
#include "dhpowareMath.h"
#include "Material.h"

#include "Postprocessing.h"

class LightScattering : public Postprocessor {
public:
 int w, h;
	unsigned int light;
	unsigned int exposure;
	unsigned int decay;
	unsigned int density;
	unsigned int weight;
	unsigned int myTexture;
	float uniformLightX ;
	float uniformLightY ;
	float uniformExposure;
	float uniformDecay;
	float uniformDensity;
	float uniformWeight;

 void Init( int width, int height, char *prefix, int num_lights, char *equation, char *lightloop ) {
  w=width; h=height;
		uniformExposure = 0.0034f;
		uniformDecay = 1.0f;
		uniformDensity = 0.84f;
		uniformWeight = 5.65f;	
  effect.AssignShader( prefix, num_lights, equation, lightloop );
 }
 void Post( Display *display, pBuffer *pbuffer, gCamera *camera ) {

 }

 Vertex *getLightScreenCoord( Light *light ) {
  double modelView[16];
  double projection[16];
  GLint viewport[4];
  double depthRange[2];

 	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_DEPTH_RANGE, depthRange);

 	GLdouble winX=0, winY=0, winZ=0;

 	gluProject(	light->position[0], light->position[1], light->position[2],
			 	modelView,
		 		projection,
		 		viewport,
		 		&winX,
		 		&winY,
		 		&winZ);

 	uniformLightX = (float) winX/((float)w);// /OFF_SCREEN_RENDER_RATIO);
 	uniformLightY = (float) winY/((float)h);// /OFF_SCREEN_RENDER_RATIO);
 }
};

#endif