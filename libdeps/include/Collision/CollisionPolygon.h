#ifndef _H_AGKCOLPOLYGON
#define _H_AGKCOLPOLYGON

//#include "dllmain.h"
#include "3DMath.h"
#include "Box.h"
#include "Face.h"
#include "CollisionObject.h"
#include "CollisionSphere.h"

namespace AGK
{
	class CollisionPolygon : private CollisionObject
	{
		public:
	    
			CollisionPolygon() { faces = 0; }
			~CollisionPolygon();
	           
			void makeCollisionObject(Face* pFaces);
	        
			bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes);       
			bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes);
	        
			bool collidesPoly(Face* obj2Faces, AGKMatrix4* transform);
			//bool collidesSphere(CollisionSphere* colSphere, float scale, AGKMatrix4* transform);
			//bool collidesBox(Box* b, AGKVector* scale, AGKMatrix4* transform);
	        
			void drawBounds(int freeObj) {}

			//bool SaveObject( FILE *pFile );
			//bool LoadObject( FILE *pFile );
	        
			Face* faces;
			Box bounds;
	        
		protected:

	        
	};
}

#endif
