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
#include "LinkedList.h"
#include "GLWindow.h"
#include "Cartesian.h"
#include "Display.h"

#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "OldVBO.h"
#include "Chemistry.h"
#include "Primitive3d.h"

class Cube3d : public Primitive3d {
public:

/*  H--------G
   /|       /|               +x face: BFGC triangles: BFG BGC
	 / |      / |    y          -x face: EADH triangles: EAD EDH
	D--|-----C  |    |          
	|  E-----|--F    o--x       +y face: DCGH triangles: DCG DGH
	| /      | /    /           -y face: EFBA triangles: EFB EBA
	|/       |/    z
	A--------B                 	+z face: ABCD triangles: ABC ACD
			      face: ABCD         	-z face: FEHG triangles: FEH FHG 
			 triangles: ABC ACD
 */
 
 void VertexBufferAndImmediate() {
  Vertex a,b,c;
  mPolygon *p;
  Vertex *w;
  model.groups++;
  int group=(model.groups);

  /*
		+x face: BFGC triangles: BFG BGC  // A	-0.5f, -0.5f,  0.5f	
		-x face: EADH triangles: EAD EDH  // B	 0.5f, -0.5f,  0.5f	
                                          // C	 0.5f,  0.5f,  0.5f	
		+y face: DCGH triangles: DCG DGH  // D	-0.5f,  0.5f,  0.5f	
		-y face: EFBA triangles: EFB EBA
                                          // E	-0.5f, -0.5f, -0.5f	
		+z face: ABCD triangles: ABC ACD  // F	 0.5f, -0.5f, -0.5f	
		-z face: FEHG triangles: FEH FHG  // G	 0.5f,  0.5f, -0.5f
                                          // H	-0.5f,  0.5f, -0.5f	
  tc: 123 134
		0.0f, 0.0f,			// lower left
		1.0f, 0.0f,			// lower right
		1.0f, 1.0f,	 		// upper right
		0.0f, 1.0f			// upper left
  */
  a.normal.Instantiate();
  b.normal.Instantiate();
  c.normal.Instantiate();

  a.Set(  0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // B 0
  a.normal->Set( 1.0f, 0.0f, 0.0f );
  b.Set(  0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // F 1
  b.normal->Set( 1.0f, 0.0f, 0.0f );
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // G 2
  c.normal->Set( 1.0f, 0.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(0,1,2);

  a.Set(  0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // B 3
  a.normal->Set( 1.0f, 0.0f, 0.0f );
  b.Set(  0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // G 4 
  b.normal->Set( 1.0f, 0.0f, 0.0f );
  c.Set(  0.5f,  0.5f,  0.5f	); c.r=0.0f; c.q=1.0f; // C 5
  c.normal->Set( 1.0f, 0.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(3,4,5);

  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(5);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(0); w->indices.Add(3); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(1);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(2); w->indices.Add(4);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 6
  a.normal->Set( -1.0f, 0.0f, 0.0f );
  b.Set( -0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // A 7
  b.normal->Set( -1.0f, 0.0f, 0.0f );
  c.Set( -0.5f,  0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // D 8
  c.normal->Set( -1.0f, 0.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(6,7,8);

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 9
  a.normal->Set( -1.0f, 0.0f, 0.0f );
  b.Set( -0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // D 10
  b.normal->Set( -1.0f, 0.0f, 0.0f );
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // H 11
  c.normal->Set( -1.0f, 0.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(9,10,11);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(6); w->indices.Add(9);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z= 0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(7);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(8); w->indices.Add(10);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(11);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f,  0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // D 12
  a.normal->Set( 0.0f, 1.0f, 0.0f );
  b.Set(  0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // C 13
  b.normal->Set( 0.0f, 1.0f, 0.0f );
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // G 14
  c.normal->Set( 0.0f, 1.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(12,13,14);

  a.Set( -0.5f,  0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // D 15
  a.normal->Set( 0.0f, 1.0f, 0.0f );
  b.Set(  0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // G 16
  b.normal->Set( 0.0f, 1.0f, 0.0f );
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // H 17
  c.normal->Set( 0.0f, 1.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(15,16,17);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f;  w->q=0.0f; w->r=0.0f; w->indices.Add(12); w->indices.Add(15);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f;  w->q=0.0f; w->r=1.0f; w->indices.Add(13);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f;  w->q=1.0f; w->r=1.0f; w->indices.Add(14); w->indices.Add(16);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f;  w->q=1.0f; w->r=0.0f; w->indices.Add(17);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 18
  a.normal->Set( 0.0f, -1.0f, 0.0f );
  b.Set(  0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // F 19
  b.normal->Set( 0.0f, -1.0f, 0.0f );
  c.Set(  0.5f, -0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // B 20
  c.normal->Set( 0.0f, -1.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(18,19,20);

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 21
  a.normal->Set( 0.0f, -1.0f, 0.0f );
  b.Set(  0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // B 22
  b.normal->Set( 0.0f, -1.0f, 0.0f );
  c.Set( -0.5f, -0.5f,  0.5f ); c.r=0.0f; c.q=1.0f; // A 23
  c.normal->Set( 0.0f, -1.0f, 0.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(21,22,23);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(20); w->indices.Add(22);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(18); w->indices.Add(21); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(19); 
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(23);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // A 24
  a.normal->Set( 0.0f, 0.0f, 1.0f );
  b.Set(  0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // B 25
  b.normal->Set( 0.0f, 0.0f, 1.0f );
  c.Set(  0.5f,  0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // C 26
  c.normal->Set( 0.0f, 0.0f, 1.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(24,25,26);

  a.Set( -0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // A 27
  a.normal->Set( 0.0f, 0.0f, 1.0f );
  b.Set(  0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // C 28
  b.normal->Set( 0.0f, 0.0f, 1.0f );
  c.Set( -0.5f,  0.5f,  0.5f ); c.r=0.0f; c.q=1.0f; // D 29
  c.normal->Set( 0.0f, 0.0f, 1.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(27,28,29);

  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(24); w->indices.Add(27); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(25);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(26); w->indices.Add(28);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(29);
  p->draw_method=GL_POLYGON;

  a.Set(  0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // F 30
  a.normal->Set( 0.0f, 0.0f, -1.0f );
  b.Set( -0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // E 31
  b.normal->Set( 0.0f, 0.0f, -1.0f );
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // H 32
  c.normal->Set( 0.0f, 0.0f, -1.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(30,31,32);

  a.Set(  0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // F 33
  a.normal->Set( 0.0f, 0.0f, -1.0f );
  b.Set( -0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // H 34
  b.normal->Set( 0.0f, 0.0f, -1.0f );
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // G 35
  c.normal->Set( 0.0f, 0.0f, -1.0f );
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(33,34,35);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(30); w->indices.Add(33);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(31);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(32); w->indices.Add(34);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(35);
  p->draw_method=GL_POLYGON;
 }

};

extern Cube3d cube3d;


class BottomlessCube3d : public Primitive3d {
public:

/* 	   H--------G
   	  /|       /|               +x face: BFGC triangles: BFG BGC
	 / |      / |    y          -x face: EADH triangles: EAD EDH
	D--|-----C  |    |          
	|  E-----|--F    o--x       +y face: DCGH triangles: DCG DGH
	| /      | /    /           -y face: EFBA triangles: EFB EBA
	|/       |/    z
   	A--------B              	+z face: ABCD triangles: ABC ACD
			      face: ABCD   	-z face: FEHG triangles: FEH FHG 
			 triangles: ABC ACD
 */
 
 void VertexBufferAndImmediate() {
  Vertex a,b,c;
  mPolygon *p;
  Vertex *w;
  model.groups++;
  int group=(model.groups);

  /*
		+x face: BFGC triangles: BFG BGC  // A	-0.5f, -0.5f,  0.5f	
		-x face: EADH triangles: EAD EDH  // B	 0.5f, -0.5f,  0.5f	
                                          // C	 0.5f,  0.5f,  0.5f	
		+y face: DCGH triangles: DCG DGH  // D	-0.5f,  0.5f,  0.5f	
		-y face: EFBA triangles: EFB EBA
                                          // E	-0.5f, -0.5f, -0.5f	
		+z face: ABCD triangles: ABC ACD  // F	 0.5f, -0.5f, -0.5f	
		-z face: FEHG triangles: FEH FHG  // G	 0.5f,  0.5f, -0.5f
                                          // H	-0.5f,  0.5f, -0.5f	
  tc: 123 134
		0.0f, 0.0f,			// lower left
		1.0f, 0.0f,			// lower right
		1.0f, 1.0f,	 		// upper right
		0.0f, 1.0f			// upper left
  */

  a.Set(  0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // B 0
  b.Set(  0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // F 1
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // G 2
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(0,1,2);

  a.Set(  0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // B 3
  b.Set(  0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // G 4 
  c.Set(  0.5f,  0.5f,  0.5f	); c.r=0.0f; c.q=1.0f; // C 5
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(3,4,5);

  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(5);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(0); w->indices.Add(3); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(1);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(2); w->indices.Add(4);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 6
  b.Set( -0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // A 7
  c.Set( -0.5f,  0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // D 8
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(6,7,8);

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 9
  b.Set( -0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // D 10
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // H 11
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(9,10,11);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(6); w->indices.Add(9);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z= 0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(7);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(8); w->indices.Add(10);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(11);
  p->draw_method=GL_POLYGON;

  a.Set( -0.5f,  0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // D 12
  b.Set(  0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // C 13
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // G 14
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(12,13,14);

  a.Set( -0.5f,  0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // D 15
  b.Set(  0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // G 16
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // H 17
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(15,16,17);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f;  w->q=0.0f; w->r=0.0f; w->indices.Add(12); w->indices.Add(15);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f;  w->q=0.0f; w->r=1.0f; w->indices.Add(13);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f;  w->q=1.0f; w->r=1.0f; w->indices.Add(14); w->indices.Add(16);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f;  w->q=1.0f; w->r=0.0f; w->indices.Add(17);
  p->draw_method=GL_POLYGON;

  /*
  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 18
  b.Set(  0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // F 19
  c.Set(  0.5f, -0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // B 20
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(18,19,20);

  a.Set( -0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // E 21
  b.Set(  0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // B 22
  c.Set( -0.5f, -0.5f,  0.5f ); c.r=0.0f; c.q=1.0f; // A 23
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(21,22,23);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(20); w->indices.Add(22);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(18); w->indices.Add(21); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(19); 
  w=new Vertex; p->addVertex(w); w->x=-0.5f;18 w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(23);
  p->draw_method=GL_POLYGON;
  */

  a.Set( -0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // A 24
  b.Set(  0.5f, -0.5f,  0.5f ); b.r=1.0f; b.q=0.0f; // B 25
  c.Set(  0.5f,  0.5f,  0.5f ); c.r=1.0f; c.q=1.0f; // C 26
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(24,25,26);

  a.Set( -0.5f, -0.5f,  0.5f ); a.r=0.0f; a.q=0.0f; // A 27
  b.Set(  0.5f,  0.5f,  0.5f ); b.r=1.0f; b.q=1.0f; // C 28
  c.Set( -0.5f,  0.5f,  0.5f ); c.r=0.0f; c.q=1.0f; // D 29
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(27,28,29);

  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z= 0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(24); w->indices.Add(27); 
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z= 0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(25);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z= 0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(26); w->indices.Add(28);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z= 0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(29);
  p->draw_method=GL_POLYGON;

  a.Set(  0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // F 30
  b.Set( -0.5f, -0.5f, -0.5f ); b.r=1.0f; b.q=0.0f; // E 31
  c.Set( -0.5f,  0.5f, -0.5f ); c.r=1.0f; c.q=1.0f; // H 32
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(30,31,32);

  a.Set(  0.5f, -0.5f, -0.5f ); a.r=0.0f; a.q=0.0f; // F 33
  b.Set( -0.5f,  0.5f, -0.5f ); b.r=1.0f; b.q=1.0f; // H 34
  c.Set(  0.5f,  0.5f, -0.5f ); c.r=0.0f; c.q=1.0f; // G 35
  vbo16.PushTriangle(&a,&b,&c); model.triangles.Add(33,34,35);
  
  p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=1.0f; w->indices.Add(30); w->indices.Add(33);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y=-0.5f; w->z=-0.5f; w->q=0.0f; w->r=0.0f; w->indices.Add(31);
  w=new Vertex; p->addVertex(w); w->x=-0.5f; w->y= 0.5f; w->z=-0.5f; w->q=1.0f; w->r=0.0f; w->indices.Add(32); w->indices.Add(34);
  w=new Vertex; p->addVertex(w); w->x= 0.5f; w->y= 0.5f; w->z=-0.5f; w->q=1.0f; w->r=1.0f; w->indices.Add(35);
  p->draw_method=GL_POLYGON;
 }

};

extern BottomlessCube3d bottomlesscube3d;