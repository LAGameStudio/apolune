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
#include "Camera.h"
#include "Film.h"
#include "pBuffer.h"
#include "dhpowareMath.h"
#include "Material.h"

class Postprocessor : public ListItem {
public:
 Display *display;
 Material effect;
 virtual void Init( Display *d, char *prefix, int num_lights, char *equation, char *lightloop ) {
  effect.AssignShader( prefix, num_lights, equation, lightloop );  
 }
 virtual void Post( pBuffer *pbuffer, gCamera *camera ) {}
};

class Postprocessing : public LinkedList {
public:
 void Init( Display *display, char *prefix, int num_lights, char *equation, char *lightloop ) {
  FOREACH(Postprocessor,p) p->Init( display, prefix, num_lights, equation, lightloop );
 }
 void Step( pBuffer *pbuffer, gCamera *camera ) {
  FOREACH(Postprocessor,p) p->Post( pbuffer, camera );
 }
 CLEARLISTRESET(Postprocessor);
 ~Postprocessing() {
  Clear();
 }
 }
};

#endif