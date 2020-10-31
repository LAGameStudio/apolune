#ifndef _H_AGKFACE
#define _H_AGKFACE

#include "3DMath.h"
#include "Box.h"
#include "CollisionResults.h"

namespace AGK
{
	class Face
	{
		public:    
			AGKVector vert1,vert2,vert3;
			AGKVector normal;//,v1_2,v2_3,v3_1;
			float d;
			//bool collisionon;
			unsigned faceId;
			Face* nextFace;
	        
			Face() {}
			~Face() {}

			virtual Face* GetNextFace( ) { return nextFace; }
			bool MakeFace( unsigned id, AGKVector *p1, AGKVector *p2, AGKVector *p3 );
	        
			//test a ray for intersection with this face
			bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, CollisionResults* cRes);
			bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, float rRadius, AGKVector* scale, CollisionResults* cRes);
	        
			bool collidesPoly(AGKVector* p1, AGKVector* p2, AGKVector* p3);
			bool collidesSphere(AGKVector* center, float sRadius, AGKVector* scale);
	    
			bool pointInPoly(AGKVector* p);
			bool lineInPoly(AGKVector* p1, AGKVector* p2);
			bool lineInPoly2D(float v1x,float v1y,float v2x,float v2y,float v3x,float v3y,
							  float p1x,float p1y,float diffPx,float diffPy);
	    
		protected:
	};
}

#endif
