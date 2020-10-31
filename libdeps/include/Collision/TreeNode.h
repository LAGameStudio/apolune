#ifndef _H_AGKTREEFACE
#define _H_AGKTREEFACE

//#include "dllmain.h"
#include "3DMath.h"
#include "Box.h"
#include "Face.h"
#include "CollisionResults.h"

namespace AGK
{
	class TreeNode
	{
		  public:
	          
			  TreeNode() { left = 0; right = 0; }
			  ~TreeNode();
	          
			  void buildTree(Face* faces,int facesPerNode);
	          
			  bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes);             
			  bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes);
	          
			  bool collidesPoly(AGKVector* p1, AGKVector* p2, AGKVector* p3, AGKVector* max, AGKVector* min);
			  bool collidesSphere(AGKVector* center, float sRadius, AGKVector* scale);
			  bool collidesBox(Box* b, AGKVector* scale, AGKVector* scale2, AGKMatrix4* transform, AGKMatrix4* rTransform);
			  bool collidesTree(TreeNode* node, AGKVector* scale, AGKVector* scale2, AGKMatrix4* transform, AGKMatrix4* rTransform);
	          
			  void drawBounds(int freeObj, int tempObj, int mesh, int* limb);
	          
			  Box* getBounds() { return &bounds; }

			  //bool SaveNode( FILE *pFile );
			  //bool LoadNode( FILE *pFile, int iVersion );
	             
		  protected:     
	          
			  Box bounds;
			  TreeNode* left;
			  TreeNode* right;
	};
}

#endif
