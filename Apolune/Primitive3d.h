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

#include <malloc.h>

#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "Cartesian.h"
#include "Display.h"

#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "OldVBO.h"
#include "Chemistry.h"

#include "Numbers.h"

// "Singleton Parent"
class Primitive3d : public ListItem {
public:
 ImmediateModel model;
 VBO16 vbo16;
 PolygonList *pList;
 string name;
 Primitive3d() : ListItem() {
  pList=null;
  name=string("");
 }
 virtual void Init() {}
 void Name( const char *s ) { name=string(s); }
 void Name( string s ) { name=s; }
/* virtual PolygonList *Immediate( ImmediateModel *m, Substance *s ) { return new PolygonList; }
 virtual void VertexBuffer( VBO *v, Substance *s ) {} */
 virtual void VertexBufferAndImmediate() {
  vbo16.Clear();
  model.polygons.Clear();
  if ( model.dlist.uploaded ) model.dlist.Delete();
  if ( pList ) delete pList;
  pList=new PolygonList;
 }
 PolygonList *Create() {
  Init();
  if ( pList ) delete pList;
  vbo16.Clear();
  model.polygons.Clear();
  if ( model.dlist.uploaded ) model.dlist.Delete();
  VertexBufferAndImmediate();
 }
 ~Primitive3d(void) {
  if ( pList ) delete pList;
 }
};

class Primitives3d : public LinkedList {
public:
 CLEARLISTRESET(Primitive3d);
 ~Primitives3d(void) {
  Clear();
 }
};