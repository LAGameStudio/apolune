#ifndef _H_AGKCOLTREE
#define _H_AGKCOLTREE

//#include "dllmain.h"
#include "3DMath.h"
#include "Face.h"
#include "TreeNode.h"
#include "CollisionObject.h"
#include "CollisionSphere.h"
#include "CollisionResults.h"

namespace AGK
{
	class CollisionTree : private CollisionObject
	{

		public:
	        
			CollisionTree() {}
			~CollisionTree() { delete tree; }
	        
			void setFacesPerNode(int num) { if (num<2) facesPerNode = 2; else facesPerNode = num; }        
			void makeCollisionObject(Face *pFaces);
	        
			bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes);        
			bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes);
	        
			bool collidesPoly(Face* obj2Faces, AGKMatrix4* transform);
			bool collidesSphere(CollisionSphere* colSphere, AGKVector* scale, float scale2, AGKMatrix4* transform);
			bool collidesBox(Box* b, AGKVector* scale, AGKVector* scale2, AGKMatrix4* transform, AGKMatrix4* rTransform);        
			bool collidesTree(TreeNode* colTree, AGKVector* scale, AGKVector* scale2, AGKMatrix4* transform, AGKMatrix4* rTransform);
	        
			TreeNode* getRoot() { return tree; }
	        
			void drawBounds(int freeObj);

			//bool SaveObject( FILE *pFile );
			//bool LoadObject( FILE *pFile );
	              
		protected:
	        
			int facesPerNode;
			TreeNode* tree;
	};
}

#endif
