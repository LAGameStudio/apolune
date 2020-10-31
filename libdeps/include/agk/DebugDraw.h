#ifndef _H_DEBUG_DRAW
#define _H_DEBUG_DRAW

#include "Common.h"
#include <Box2D/Box2D.h>

struct b2AABB;

namespace AGK
{
	class cSprite;
	class AGKShader;

	// This class implements debug drawing callbacks that are invoked inside b2World::Step.
	class DebugDraw : public b2Draw
	{
	public:
		AGKShader *m_pShader;

		DebugDraw() {}
		~DebugDraw() {}

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
		void DrawString(int x, int y, const char* string, ...); 
		void DrawAABB(b2AABB* aabb, const b2Color& color);
	};

	// This class gets a callback when things are about to be destroyed in Box2D so AGK can clean up pointers
	class MyDestructionListener : public b2DestructionListener
	{
		public:
			MyDestructionListener() {}
			~MyDestructionListener() {}

			void SayGoodbye(b2Joint* joint);
			void SayGoodbye(b2Fixture* fixture) {}
	};

	class MyRayCastCallback : public b2RayCastCallback
	{
		public:

			MyRayCastCallback();
			void Reset();

			float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

			b2Fixture* m_fixture;
			b2Vec2 m_point;
			b2Vec2 m_normal;
			float32 m_fraction;
			unsigned int m_category;
			int m_group;
			cSprite* m_sprite;
	};
}


#endif
