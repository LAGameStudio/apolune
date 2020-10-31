#ifndef _H_AGK_GLOBALS_
#define _H_AGK_GLOBALS_

// can switch UNDEF to "null" for C#
#define UNDEF 0
#ifndef UINT
#define UINT unsigned int
#endif

#ifndef _EXPORT_
#define _EXPORT_ 
#endif

#ifndef PI
#define PI 3.141592654f
#endif

// Box2D forward declarations
class b2Body;
class b2Shape;
class b2World;
class b2Joint;
class b2Fixture;
struct b2DistanceOutput;
class b2Contact;
struct b2ContactEdge;

struct AGKSortValue
{
	UINT iValue;
	void* ptr;
};

#endif