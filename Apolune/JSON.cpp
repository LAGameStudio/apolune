#include "JSON.h"

 void JSONNode::AddChild( JSONNode *n ) {
  children->Add(n);
  n->parent=this;
 }
 void JSONNode::AddElement( JSONNode *n ) {
  elements->Add(n);
  n->parent=this;
 }