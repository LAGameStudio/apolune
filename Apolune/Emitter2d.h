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

#include "ListItem.h"
#include "GLWindow.h"

class Emitter2d : public ListItem
{
public:
 bool dead;
 virtual void Between() { }
 virtual void Render( GLWindow *surface ) {}
 virtual void RemoveDeadParticles() {}
 Emitter2d(void) : ListItem() { dead=false; }
 ~Emitter2d(void) {}
};

class Emitters2d : public LinkedList {
public:
 virtual void Between() { FOREACH(Emitter2d,e) e->Between(); }
 virtual void Render( GLWindow *surface ) { FOREACH(Emitter2d,e) e->Render(surface); }
 CLEARLISTRESET(Emitter2d);
 Emitters2d(void) { Clear(); }
 ~Emitters2d(void) {}
};