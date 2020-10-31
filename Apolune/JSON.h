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

#include "LinkedList.h"

/*
 * JSON Encoder
 */
class JSONTree;
class JSONNodeHandles;
class JSONNode : public ListItem {
public:
 Zp<JSONTree> tree;
 Zp<JSONNode> parent;
 Zdisposable<JSONNodeHandles> elements,children;
 Zstring key,value;
 Zint loopCatcher;
 JSONNode( const char *k, const char *v ) : ListItem() { key=k; value=v; children.Instantiate(); elements.Instantiate(); }
 JSONNode( string k, string v ) : ListItem() { key=k; value=v; children.Instantiate(); elements.Instantiate(); }
 JSONNode() : ListItem() { children.Instantiate(); elements.Instantiate(); }
 void AddChild( JSONNode *n );
 void AddElement( JSONNode *n );
 void Loop() { loopCatcher++; }
 bool Looped() { return loopCatcher > 0; }
 void Zero() { loopCatcher=0; }
};

HANDLES(JSONNode,JSONNodeHandle,JSONNodeHandles,"JSONNode");

class JSONNodes : public LinkedList {
public:
 Zp<JSONTree> tree;
 JSONNode *Add( JSONNode *node ) {
  Append(node);
  node->tree=tree;
  return node;
 }
 JSONNode *AddElement( JSONNode *parent, string key, string value ) {
  JSONNode *added=new JSONNode(key,value);
  parent->AddElement(added);
  return added;
 }
 JSONNode *AddChild( JSONNode *parent, string key, string value ) {
  JSONNode *added=new JSONNode(key,value);
  parent->AddChild(added);
  return added;
 }
 JSONNode *Add( JSONNode *parent, string key, string value ) {
  return Add(new JSONNode(key,value));
 }
 void Roots(JSONNodeHandles *out) {
  out->Clear();
  FOREACH(JSONNode,node) if ( !node->parent ) out->Add(node);
 }
 void Prune(JSONNode *target ) {
  Remove(target);
  FOREACHN(JSONNode,node,{
   if ( node->parent == target ) Remove(node);
   Prune(node);
   delete node;
  });
  delete target;
 }
};

class JSONTree : public ListItem {
public:
 JSONNodes nodes;
 JSONNodeHandles roots;
 JSONTree() : ListItem() {
  nodes.tree=this;
 }
 JSONNode *AddNode( JSONNode *parent, const char *k, const char *v="" ) {
  JSONNode *added=nodes.Add(parent,string(k),string(v));
  added->tree=this;
  return added;
 }
 JSONNode *AddElement( JSONNode *parent, const char *k, const char *v="" ) {
  return nodes.AddElement(parent,string(k),string(v));
 }
 JSONNode *AddElement( JSONNode *parent, string key, string value ) {
  return nodes.AddElement(parent,key,value);
 }
 JSONNode *AddChild( JSONNode *parent, string key, string value ) {
  return nodes.AddChild(parent,key,value);
 }
 JSONNode *AddChild( JSONNode *parent, const char *k, const char *v="" ) {
  return nodes.AddChild(parent,string(k),string(v));
 }
 void Roots() {
  nodes.Roots(&roots);
 }
 string rtrim_comma( string in ) {
  Zstring out(in);
  out.rTrim(" \t\v\r\n," );
  return out.value;
 }
 string brackets( string in ) {
  return string("{")+in+string("}");
 }
 string handle_quotes( string in ) {
  Zstring out(in);
  out("\\","\\\\");
  out("\"","\\\"");
  return out.value;
 }
 string quotes( string in ) {
  return string("\"")+handle_quotes(in)+string("\"");
 }
 string toJSON(JSONNodeHandle *node ) {
  Zstring out;
  if ( node->p->children->count > 0 ) {
   EACH(node->p->elements->first,JSONNodeHandle,nh) out+=toJSON(nh)+string(",");
   return quotes(node->p->key)+string(":")+brackets(rtrim_comma(out.value));
  } else if ( node->p->elements->count > 0 ) {
   EACH(node->p->elements->first,JSONNodeHandle,nh) out+=toJSON(nh)+string(",");
   return quotes(node->p->key)+string(":")+brackets(rtrim_comma(out.value));
  } else return quotes(node->p->key)+string(":")+quotes(node->p->value)+string(",");
 }
 string toJSON() {
  Zstring out;
  Roots();
  EACH(roots.first,JSONNodeHandle,nh) {
   out+=toJSON(nh);
  }
 }
};

HANDLED(JSONTree,JSONTreeHandle,JSONTreeHandles,JSONTReeHandlesHandle,JSONTreeHandlesHandles);

class JSONTrees : public LinkedList {
public:
 CLEARLISTRESET(JSONTree);
};