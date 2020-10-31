#include "ItemContentTree.h"

#include "Campaign.h"
#include "ThingInstance.h"
#include "GameDereference.h"

void ItemContentNode::Construct() {
 contains = new ItemContentTree;
}

string ItemContentNode::SaveContentsString() {
 return contains ? contains->toString() : string("");
}

void ItemContentNode::LoadContentsString( const char *w ) {
 contains->fromString(w);
}

string ItemContentNode::SaveCampaignString() {
 return campaign ? campaign->filename : string("");
}

void ItemContentNode::LoadCampaignString( const char *w ) {
 campaign=campaigns.findByFilename( w );
}

ItemContentNode::~ItemContentNode() {
 if ( contains ) {
  delete contains;
  contains=null;
 }
}

ItemContentNode *ItemContentTree::FindLike( Commodity *c ) {
 string ref=c->GetReference();
 FOREACH(ItemContentNode,i) if ( i->item == it_commodity ) {
  if ( !str_cmp(ref.c_str(),i->reference.c_str()) ) {
   return i;
  }
 } else if ( i->contains && i->contains->count > 0 ) {
  ItemContentNode *n=i->contains->FindLike(c);
  if ( n ) return n;
 }
 return null;
}

ItemContentNode *ItemContentTree::FindLike( Thing *c ) {
 string ref=c->GetReference();
 FOREACH(ItemContentNode,i) if ( i->item == it_thing ) {
  if ( !str_cmp(ref.c_str(),i->reference.c_str()) ) {
   return i;
  }
 } else if ( i->contains && i->contains->count > 0 ) {
  ItemContentNode *n=i->contains->FindLike(c);
  if ( n ) return n;
 }
 return null;
}

void ItemContentTree::Things( ThingHandles *out, ThingTypes type ) {
 FOREACH(ItemContentNode,i) if ( i->item == it_thing ) {
  GameDereference gd;
  gd.Reference(i->reference.c_str());
  if ( gd.thing ) if ( gd.thing->type == type ) out->Add(gd.thing,true);
 }
}

ItemContentNode *ItemContentTree::FindLike( Vehicle *c ) {
 string ref=c->GetReference();
 FOREACH(ItemContentNode,i) if ( i->item == it_thing ) {
  if ( !str_cmp(ref.c_str(),i->reference.c_str()) ) {
   return i;
  }
 } else if ( i->contains && i->contains->count > 0 ) {
  ItemContentNode *n=i->contains->FindLike(c);
  if ( n ) return n;
 }
 return null;
}

ItemContentNode *ItemContentTree::FindLike( SpaceCar *c ) {
 string ref=c->GetReference();
 FOREACH(ItemContentNode,i) if ( i->item == it_thing ) {
  if ( !str_cmp(ref.c_str(),i->reference.c_str()) ) {
   return i;
  }
 } else if ( i->contains && i->contains->count > 0 ) {
  ItemContentNode *n=FindLike(c);
  if ( n ) return n;
 }
 return null;
}

void ItemContentNode::from( Vehicle *c ) {
 quantity=1;
 reference=c->GetReference();
 unpack=c->toString();
 named=c->ship_name;
 item=it_vehicle;
}

void ItemContentNode::from( Commodity *c ) {
 quantity=1;
 reference=c->GetReference();
 item=it_commodity;
}

void ItemContentNode::from( SpaceCar *c ) {
 quantity=1;
 reference=c->GetReference();
 item=it_spacecar;
}

void ItemContentNode::from( Thing *c ) {
 quantity=1;
 reference=c->GetReference();
 item=it_thing;
}


Commodity *ItemContentNode::GetCommodity() {
 if ( item == it_commodity ) {
  GameDereference gd;
  gd.Reference(reference.c_str());
  return gd.commodity;
 } else return null;
}
SpaceCar *ItemContentNode::GetSpaceCar() {
 if ( item == it_spacecar ) {
  GameDereference gd;
  gd.Reference(reference.c_str());
  return gd.spacecar;
 } else return null;
}
Vehicle *ItemContentNode::GetVehicle() {
 if ( item == it_vehicle ) {
  GameDereference gd;
  gd.Reference(reference.c_str());
  if ( gd.vehicle ) {
   Vehicle *v=new Vehicle();
   v->fromString(unpack.c_str());
   v->ship_name=named;
   return v;
  }
  return null;
 } else return null;
}
Thing *ItemContentNode::GetThing() {
 if ( item == it_thing ) {
  GameDereference gd;
  gd.Reference(reference.c_str());
  return gd.thing;
 } else return null;
}

GLImage *ItemContentNode::ItemIcon() {
 GameDereference gd;
 gd.Reference(reference.c_str());
 switch ( item ) {
  case it_commodity:
   {
    Commodity *c=gd.commodity;
    return c ? c->image : null;
   }
   break;
  case it_spacecar:
   {
    SpaceCar *sc=gd.spacecar;
    return sc ? sc->body : null;
   }
   break;
  case it_thing:
   {
    Thing *t=gd.thing;
    return t ? t->image : null;
   }
   break;
  case it_vehicle:
   {
    VehicleBlueprint *vb=gd.vehicle;
    return vb && vb->hull && vb->hull->blueprint ? vb->hull->blueprint->image : null;
   }
   break;
 }
 return null;
}

string ItemContentNode::ItemName() {
 GameDereference gd;
 gd.Reference(reference.c_str());
 switch ( item ) {
  case it_commodity:
   {
    Commodity *c=gd.commodity;
    return c ? c->catalog_name : string("Unknown Item");
   }
   break;
  case it_spacecar:
   {
    SpaceCar *sc=gd.spacecar;
    return sc ? sc->makeModel : string("Unknown Item");
   }
   break;
  case it_thing:
   {
    Thing *t=gd.thing;
    return t ? t->named : string("Unknown Item");
   }
   break;
  case it_vehicle:
   {
    VehicleBlueprint *vb=gd.vehicle;
    return vb  ? vb->design_name : string("Unknown Item");
   }
   break;
 }
 return string("Unknown Item");
}

double ItemContentNode::ItemValue() {
 GameDereference gd;
 gd.Reference(reference.c_str());
 switch ( item ) {
  case it_commodity:
   {
    Commodity *c=gd.commodity;
    return c ? c->Value() : 1.0;
   }
   break;
  case it_spacecar:
   {
    SpaceCar *sc=gd.spacecar;
    return sc ? sc->Value() : 1.0;
   }
   break;
  case it_thing:
   {
    Thing *t=gd.thing;
    return t ? t->value : 1.0;
   }
   break;
  case it_vehicle:
   {
    VehicleBlueprint *vb=gd.vehicle;
    return  vb  ? vb->Value() : 1.0;
   }
   break;
 }
 return 1.0;
}

Crayon ItemContentNode::ItemTint() {
 Crayon tint(alabaster);
 GameDereference gd;
 gd.Reference(reference.c_str());
 switch ( item ) {
  case it_commodity:
   {
    Commodity *c=gd.commodity;
    return c ? c->tint : tint;
   }
   break;
  case it_spacecar:
   {
    SpaceCar *sc=gd.spacecar;
    return sc ? tint : tint;
   }
   break;
  case it_thing:
   {
    Thing *t=gd.thing;
    return t ? t->tint : tint;
   }
   break;
  case it_vehicle:
   {
    VehicleBlueprint *vb=gd.vehicle;
    return  vb  ? vb->tint : tint;
   }
   break;
 }
 return tint;
}

void ItemContentTree::Deposit( Commodity *c, int quantity ) {
 ItemContentNode *node=FindLike(c);
 if ( node ) {
  node->quantity+=quantity;
 } else {
  ItemContentNode *n=new ItemContentNode;
  n->from(c);
  n->quantity=quantity;
  Append(n);
 }
 Tally();
}

void ItemContentTree::Deposit( Thing *c ) {
 ItemContentNode *node=FindLike(c);
 if ( node ) {
  node->quantity+=1;
 } else {
  ItemContentNode *n=new ItemContentNode;
  n->from(c);
  Append(n);
 }
 Tally();
}

void ItemContentTree::Deposit( SpaceCar *c ) {
 ItemContentNode *node=FindLike(c);
 if ( node ) {
  node->quantity+=1;
 } else {
  ItemContentNode *n=new ItemContentNode;
  n->from(c);
  Append(n);
 }
 Tally();
}

void ItemContentTree::Deposit( Vehicle *c ) {
 ItemContentNode *n=new ItemContentNode;
 n->from(c);
 Append(n);
 Tally();
}

// Returns an int of how short you are if the quantity is not available
int ItemContentTree::Withdraw( Commodity *c, int quantity ) {
 ItemContentNode *node=FindLike(c);
 if ( !node ) return quantity;
 if ( quantity > node->quantity ) {
  int remainder=quantity-node->quantity;
  RemoveInside(node);
  return remainder;
 }
 node->quantity-=quantity;
 Tally();
 return 0;
}

bool ItemContentTree::Withdraw( Thing *t ) {
 ItemContentNode *node=FindLike(t);
 if ( !node ) return false;
 RemoveInside(node);
 delete node;
 Tally();
 return true;
}

bool ItemContentTree::Withdraw( SpaceCar *sc ) {
 ItemContentNode *node=FindLike(sc);
 if ( !node ) return false;
 RemoveInside(node);
 delete node;
 Tally();
 return true;
}

Vehicle *ItemContentTree::Withdraw( ItemContentNode *n ) {
 Vehicle *v=new Vehicle;
 v->fromString(n->unpack.c_str());
 RemoveInside(n);
 Tally();
 return v;
}

bool ItemContentTree::RemoveInside( ItemContentNode *n ) {
 if ( ListIteminList(n) ) {
  Remove(n);
  return true;
 }
 FOREACH(ItemContentNode,m) if ( m->contains && m->contains->count > 0 && m->contains->RemoveInside(n) ) return true;
 return false;
}

int ItemContentTree::Containing( Commodity *c ) {
 ItemContentNode *n=FindLike(c);
 if ( !n ) return 0;
 return n->quantity;
}

void ItemContentTree::Merge( ItemContentNode *n, ItemContentTree *removing_from_tree ) {
 switch ( n->item ) {
  case it_commodity:
   {
    GameDereference *gd=new GameDereference;
    gd->Reference(n->reference.c_str(),n->campaign);
    if ( gd->commodity ) {
     Deposit(gd->commodity,n->quantity);
    }
    removing_from_tree->Remove(n);
    delete n;
   }
   break;
  case it_spacecar:
  case it_thing:
  case it_vehicle:
    removing_from_tree->Remove(n);
    Append(n);
   break;
 }
}

void ItemContentTree::Merge( ItemContentTree *in ) {
 EACHN(in->first,ItemContentNode,node,{ Merge(node,in); });
 Tally();
}

void ItemContentTree::All( ItemContentNodeHandles *nodes, ItemContentTreeHandles *trees ) {
 FOREACH(ItemContentNode,n) {
  nodes->Add(n);
  trees->Add(this);
  if ( n->contains && n->contains->count > 0 ) n->contains->All(nodes,trees);
 }
 Tally();
}

void ItemContentTree::TopLevel( ItemContentNodeHandles *out ) {
 FOREACH(ItemContentNode,n) {
  out->Add(n);
 }
}



// number of items
int ItemContentTree::TallyContent() {
 int total=count;
 FOREACH(ItemContentNode,n) total+=n->contains ? n->contains->TallyContent() : 0;
 talliedContent=total;
 return total;
}

// number of units of weight carried
double ItemContentTree::TallyInUnits() {
 double total=0.0;
 FOREACH(ItemContentNode,n) {
  total+=(double)n->quantity;
  total+=n->contains ? n->contains->TallyInUnits() : 0.0;
 }
 talliedUnits=total;
 return total;
}