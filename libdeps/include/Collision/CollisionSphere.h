#ifndef _H_AGKCOLSPHERE
#define _H_AGKCOLSPHERE

#include "3DMath.h"
#include "Face.h"
#include "CollisionObject.h"
#include "CollisionResults.h"

namespace AGK
{
	//improvement on bounding sphere, sphere is centered on the collision data
	class CollisionSphere : private CollisionObject
	{
		public:
	    
			CollisionSphere() { radius = 0; }
			~CollisionSphere() {}
	    
			void makeCollisionObject(Face *pFaces);
	        
			bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes);        
			bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes);
	        
			bool collidesPoly(Face* obj2Faces, AGKVector* scale, float lScale, AGKMatrix4* transform);
			bool collidesSphere(CollisionSphere* colSphere, float scale1, float scale2, AGKMatrix4* transform);
	        
			void drawBounds(int freeObj);

			//bool SaveObject( FILE *pFile );
			//bool LoadObject( FILE *pFile );
	        
			//instead of mainRadius
			//radius is the distance of the furthest vertex from the collision object's center (closer fit to vertex data)      
			float radius;
			float centerX, centerY, centerZ;
	};
}

#endif
