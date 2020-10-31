#ifndef _H_AGKCOLOBJECT
#define _H_AGKCOLOBJECT

#include "3DMath.h"
#include "CollisionResults.h"

namespace AGK
{

	class Face;

	//Template for all collision objects
	//All collision objects are at least a bounding sphere for quick rejection
	class CollisionObject
	{
		public:
	    
			CollisionObject() {}
			virtual ~CollisionObject() {}
	        
			virtual void makeCollisionObject(Face* pFaces) = 0;

			virtual bool intersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, CollisionResults* cRes) = 0;
			virtual bool sphereIntersects(AGKVector* p, AGKVector* v, AGKVector* vn, AGKVector* vi, float rRadius, AGKVector* scale, CollisionResults* cRes) = 0;
			virtual void drawBounds(int freeObj) = 0;

			//virtual bool SaveObject( FILE *pFile ) = 0;
			//virtual bool LoadObject( FILE *pFile ) = 0;
	        
			//do a rough check on the main radii of two collision objects
			bool mainBoundingSpheresHit(CollisionObject* obj2, float sqrObjDist, float scaleValue1, float scaleValue2);
			bool quickRejectRay( const AGKVector* p, const AGKVector* v, const AGKVector* vn, float scale);
			bool quickRejectSphereRay( const AGKVector* p, const AGKVector* v, const AGKVector* vn, float rRadius, float scale);
	        
			//mainRadius is the distance of the furthest vertex from the object's origin (easy to perform quick rejection)
			float mainRadius;
	        
	};

	inline bool CollisionObject::mainBoundingSpheresHit(CollisionObject* colObj2, float sqrObjDist, float scaleValue1, float scaleValue2)
	{
		float largeRadius  = mainRadius*scaleValue1;
		float largeRadius2 = colObj2->mainRadius*scaleValue2;
	    
		if ((largeRadius+largeRadius2) * (largeRadius+largeRadius2) < sqrObjDist) return false;
	    
		return true;
	}

	inline bool CollisionObject::quickRejectRay(const AGKVector* p, const AGKVector* v, const AGKVector* vn, float scale)
	{
		float dist = -vn->x*p->x - vn->y*p->y - vn->z*p->z;
		float lRadius = mainRadius*scale;
	            
		if ( dist <= 0 ) {
			if ( p->SqrLength() > lRadius*lRadius ) return true;
		}
		else {
			if ( dist*dist >= v->SqrLength() ) {
				float newx = p->x + v->x;
				float newy = p->y + v->y;
				float newz = p->z + v->z;
				if ( (newx*newx + newy*newy + newz*newz) > lRadius*lRadius ) return true;
			}
			else {
				float newx = p->x + vn->x*dist;
				float newy = p->y + vn->y*dist;
				float newz = p->z + vn->z*dist;
				if ( (newx*newx + newy*newy + newz*newz) >lRadius*lRadius ) return true;
			}
		}
	    
		return false;
	}

	inline bool CollisionObject::quickRejectSphereRay( const AGKVector* p, const AGKVector* v, const AGKVector* vn, float rRadius, float scale)
	{
		float dist = -vn->x*p->x - vn->y*p->y - vn->z*p->z;
		float lRadius = mainRadius*scale + rRadius;
	            
		if ( dist <= 0 ) {
			if ( p->SqrLength() > lRadius*lRadius ) return true;
		}
		else {
			if ( dist*dist >= v->SqrLength() ) {
				float newx = p->x + v->x;
				float newy = p->y + v->y;
				float newz = p->z + v->z;
				if ( (newx*newx + newy*newy + newz*newz) > lRadius*lRadius ) return true;
			}
			else {
				float newx = p->x + vn->x*dist;
				float newy = p->y + vn->y*dist;
				float newz = p->z + vn->z*dist;
				if ( (newx*newx + newy*newy + newz*newz) > lRadius*lRadius ) return true;
			}
		}
	    
		return false;
	}
}

#endif
