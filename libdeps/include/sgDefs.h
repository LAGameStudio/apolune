#ifndef  __sgDefs__
#define  __sgDefs__


#define   SG_PLATFORM_WINDOWS     0
#define   SG_PLATFORM_MAC         1
#define   SG_PLATFORM_IOS         2
#define   SG_PLATFORM_ANDROID     3


#define   SG_CURRENT_PLATFORM     SG_PLATFORM_WINDOWS


#if (SG_CURRENT_PLATFORM==SG_PLATFORM_WINDOWS)
	#ifndef sgCore_API
	#define  sgCore_API __declspec(dllimport)
	#else
	#undef   sgCore_API
	#define  sgCore_API __declspec(dllexport)
	#endif
#else
   #define  sgCore_API
#endif

#if (SG_CURRENT_PLATFORM==SG_PLATFORM_IOS || SG_CURRENT_PLATFORM==SG_PLATFORM_ANDROID)
    typedef   float   sgFloat;
#else
    typedef   double  sgFloat;
#endif


typedef struct
{
  sgFloat x;
  sgFloat y;
  sgFloat z;
} SG_POINT;

#define  SG_VECTOR   SG_POINT

typedef struct
{
  SG_POINT  p1;
  SG_POINT  p2;
} SG_LINE;

typedef  void*       SG_OBJ_HANDLE;
typedef  void(*SG_DRAW_LINE_FUNC)(SG_POINT*,SG_POINT*);

struct  SG_USER_DYNAMIC_DATA
{
  virtual void Finalize()   =0;
};



void*    sgPrivateAccess(int, void*, void*);
#define  PRIVATE_ACCESS   friend void* sgPrivateAccess(int, void*, void*);



#endif
