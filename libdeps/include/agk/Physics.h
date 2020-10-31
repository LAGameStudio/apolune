#ifndef _H_PHYSICS_
#define _H_PHYSICS_

namespace AGK
{
	class Point2D
	{
		public:
			float x, y;
			Point2D *pNext;
			
			Point2D() { x = 0.0f; y = 0.0f; pNext = 0; }
			~Point2D() {};

			static int Count( const Point2D *pPoints );
	};

	class Physics
	{
		public:

			static Point2D* ConvexHull2D( int width, int height, unsigned char* pixels );
			static bool IsPointInConvexHull( Point2D *pPoints, float x, float y );
			static void ConvexHullAddPoint( Point2D * &pPoints, float x, float y );
			static void ConvexHullCleanPoints( Point2D * &pPoints, float min );
			static void ConvexHullReducePoints( Point2D * &pPoints, int max );
	};
}

#endif