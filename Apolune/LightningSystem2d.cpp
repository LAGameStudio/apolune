#include "LightningSystem2d.h"

LineTreeNode::LineTreeNode(
  LineTree *tree, int depth, LineTreeNode *base,
  double t, double R,
  double rads, double limitAngle,
  int maxBranches, double branchFrequency,
  double diminishing, int maxDepth=6
 )
 : ListItem() {
 this->R=R*diminishing;
 this->diminishing=diminishing;
 this->branchFrequency=branchFrequency*diminishing;
 this->limitAngle=limitAngle*diminishing;
 this->depth=depth+1;
 this->maxDepth=maxDepth;
 this->tree=tree;
 this->parent=base;
 this->maxBranches=maxBranches;
 this->rads=rads;
 tParent=t;
 Vertexd a,b;
 if ( base ) {
  base->line.Point(t,&a);
  double ourangle=rads+double_range(-limitAngle,limitAngle);
  b.Set(
   a.x+R*cos(ourangle),
   a.y+R*sin(ourangle)
  );
  line.Set(a.x,a.y,b.x,b.y);
 }
 branches.Instantiate();
}

LineTreeNode::LineTreeNode( LineTreeNode *source, LineTree *cloned, LineTreeNode *parent ) : ListItem() {
 depth=source->depth;
 maxDepth=source->maxDepth;
 maxBranches=source->maxBranches;
 tParent=source->tParent;
 line.Set(&source->line);
 tree=cloned;
 this->parent=parent;
 this->root=source->root;
 R=source->R;
 branchFrequency=source->branchFrequency;
 diminishing=source->diminishing;
 rads=source->rads;
 limitAngle=source->limitAngle;
}

void LineTreeNode::Grow() {
 if ( depth < maxDepth ) {
  for ( int i=0; i<maxBranches; i++ ) if ( uniform() < branchFrequency ) {
   LineTreeNode *child=
    new LineTreeNode(tree,depth,this,uniform(),R,rads,limitAngle,maxBranches,branchFrequency,diminishing,maxDepth);
   branches->Append(child);
   child->Grow();
  }
 }
}