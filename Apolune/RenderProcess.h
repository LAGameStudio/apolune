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

class RenderProcess : public ListItem {
public:
 virtual void Render( GLWindow *surface ) {
 }
 RenderProcess(void) : ListItem() {
 }
 ~RenderProcess(void) {
 }
};

class RenderProcesses : public LinkedList {
 void Render( GLWindow *surface ) {
  EACH(first,RenderProcess,r) r->Render(surface);
 }
 RenderProcesses(void) : LinkedList() {
 }
 CLEARLISTRESET(RenderProcess);
 ~RenderProcesses(void) {
  Clear();
 }
};
