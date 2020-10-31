#ifndef _H_AGKCOLBOX
#define _H_AGKCOLBOX

#include "3DMath.h"
#include "Box.h"
#include "Face.h"
#include "CollisionObject.h"
#include "CollisionSphere.h"
#include "CollisionResults.h"

namespace AGK
{
	class CollisionBox : private CollisionObject
	{
		public:
	        
			Box bounds;
	        
			CollisionBox() {}
			CollisionBox(Box b) { bounds = b; }
			~CollisionBox() {}
	        
			void makeCollisionObject(Face *pFaces);
	        
			bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes);
			bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes);
	        
			bool collidesPoly(Face* obj2Faces, AGKMatrix4* transform);
			int  collidesBox(Box* b, AGKVector* scale, AGKVector* scale2, AGKMatrix4* transform, AGKMatrix4* rTransform);
			bool collidesSphere(CollisionSphere* colSphere, AGKVector* scale, float scale2, AGKMatrix4* transform);

			//bool SaveObject( FILE *pFile );
			//bool LoadObject( FILE *pFile );
	        
			void drawBounds(int freeObj);
	        
		protected:
	    
	        
	};
}
    
#endif
