#ifndef _H_AGKCOLRESULTS
#define _H_AGKCOLRESULTS

#include "3DMath.h"

namespace AGK
{
	class CollisionResults
	{
		public:
	        
			CollisionResults();
			CollisionResults(int max);
			~CollisionResults();
	        
			void setMaxCollisions(int cols);        
			int getMaxCollisions() { return maxCols; }  
			void reset(float dist);        
	        
			void sort();        
			void swapElements(int i,int j);
			void copyElements(int i,int j);
	        
			void addPoint( AGKVector* intersect, AGKVector* normal, float distance, int id );        
			void rotatePoints( AGKQuaternion* rot, int obj, int group, float x, float y, float z );
			void completeResults( const AGKVector* p, const AGKVector* v1, bool bShift = true );
	        
			void setRecordAll(bool flag) { recordAll = flag; }    
			void setCurrentDistance(float dist) { finalDistance[0]=dist; }
			void setChanged(bool change) { changed = change; }
			void setScaleOn(float sx, float sy, float sz, AGKVector* p);
			void setScaleOff() { scaleOn = false; }
			void setNumCols(int num) { numOfCol = num; } 
	        
			float getCurrentDistance() { return finalDistance[0]; }
			bool getRecordAll() { return recordAll; }
			bool getChanged() { return changed; }
			bool getScale() { return scaleOn; }
			int getNumCols() { return numOfCol; }
	        
			bool isValidDistance(float dist);
			bool isValidDistanceSqr(float dist);
	        
			float getIntersectX(int num) { if (num>=0 && num<maxCols) return finalIntersectX[num]; return 0; }
			float getIntersectY(int num) { if (num>=0 && num<maxCols) return finalIntersectY[num]; return 0; }
			float getIntersectZ(int num) { if (num>=0 && num<maxCols) return finalIntersectZ[num]; return 0; }
	        
			float getNormalX(int num) { if (num>=0 && num<maxCols) return finalNormalX[num]; return 0; }
			float getNormalY(int num) { if (num>=0 && num<maxCols) return finalNormalY[num]; return 0; }
			float getNormalZ(int num) { if (num>=0 && num<maxCols) return finalNormalZ[num]; return 0; }
	    
			float getBounceX(int num) { if (num>=0 && num<maxCols) return finalBounceX[num]; return 0; }
			float getBounceY(int num) { if (num>=0 && num<maxCols) return finalBounceY[num]; return 0; }
			float getBounceZ(int num) { if (num>=0 && num<maxCols) return finalBounceZ[num]; return 0; }
	        
			float getSlideX(int num) { if (num>=0 && num<maxCols) return finalSlideX[num]; return 0; }
			float getSlideY(int num) { if (num>=0 && num<maxCols) return finalSlideY[num]; return 0; }
			float getSlideZ(int num) { if (num>=0 && num<maxCols) return finalSlideZ[num]; return 0; }
	        
			void storePoints(int num, AGKVector* intersect, AGKVector* normal, AGKVector* slide);
	        
			void setIntersectPoint(int num, AGKVector* p) { finalIntersectX[num] = p->x; finalIntersectY[num] = p->y; finalIntersectZ[num] = p->z; }
			void setNormal(int num, AGKVector* v) { finalNormalX[num] = v->x; finalNormalY[num] = v->y; finalNormalZ[num] = v->z; }
			void setSlidePoint(int num, AGKVector* p) { finalSlideX[num] = p->x; finalSlideY[num] = p->y; finalSlideZ[num] = p->z; }
	        
			float getDistance(int num) { if (num>=0 && num<maxCols) return finalDistance[num]; return 0; }
			int getObj(int num) { if (num>=0 && num<maxCols) return finalObjHit[num]; return 0; }
			int getGroup(int num) { if (num>=0 && num<maxCols) return finalGroupHit[num]; return 0; }
			int getFace(int num) { if (num>=0 && num<maxCols) return finalFaceHit[num]; return 0; }
	        
		protected:
	    
			bool recordAll;
			int maxCols;
			float* finalIntersectX;
			float* finalIntersectY;
			float* finalIntersectZ;
			float* finalNormalX;
			float* finalNormalY;
			float* finalNormalZ;
			float* finalBounceX;
			float* finalBounceY;
			float* finalBounceZ;
			float* finalSlideX;
			float* finalSlideY;
			float* finalSlideZ;
			float* finalDistance;
			int* finalObjHit;
			int* finalGroupHit;
			int* finalFaceHit;
			int numOfCol;
			int firstCol;
	        
	        
			bool changed;
			bool scaleOn;
			float scalex;
			float scaley;
			float scalez;
			float origx;
			float origy;
			float origz;
	};
}

#endif
