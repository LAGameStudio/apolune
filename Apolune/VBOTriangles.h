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
#include "Indexed.h"
#include "Vertex.h"
#include "Vertexd.h"
#include "Polygon.h"
#include "Numbers.h"

/* VBO Triangle
 * Helper classes for generative transfer used in Primitives3d.
 */

class VBOTriangle : public ListItem {
public:
 Integers indices;
 Indexed<int> buffer;
 string toString() {
  return indices.toString();
 }
 void fromString( const char *s ) {
  indices.fromString(s);
 }
 VBOTriangle() { prev=next=null; }
 VBOTriangle( int a, int b, int c ) {
  prev=next=null;
  indices.Add(a);
  indices.Add(b);
  indices.Add(c);
 }
 Indexed<int> *asArray() {
  buffer.Size(indices.count);
  Integer *integer=(Integer *) (indices.first);
  for ( int i=0; i<indices.count; i++ ) {
   buffer[i]=integer->i;
   integer=(Integer *) integer->next;
  }
  return &buffer;
 }
};

class VBOTriangles : public LinkedList {
public:
 string toString() {
  int i=0;
  string r=string("");
  EACH(first,VBOTriangle,v) {
   r+=string("[")+v->toString()+string("], ");
   i++;
   if ( i % 3 == 0 ) r+=string("\n");
  }
  return r;
 }
 void fromString( const char *s ) {
  string buf;
  const char *p=s;
  while ( *p != '\0' ) {
   p=string_argument(p,&buf);
   if ( buf.length() > 0 ) {
    VBOTriangle *v=new VBOTriangle;
    v->fromString((char *)buf.c_str());
    Append(v);
   }
  }
 }
 VBOTriangle *find( int i ) {
  FOREACH(VBOTriangle,t) { Integer *point=t->indices.find(i); if ( point!=null ) return t; }
  return null;
 }
 void Add( int a, int b, int c ) { Append(new VBOTriangle(a,b,c)); }
 CLEARLISTRESET(VBOTriangle);
 ~VBOTriangles() { Clear(); }
};