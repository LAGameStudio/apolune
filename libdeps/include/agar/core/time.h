/*	Public domain	*/

#include <agar/core/begin.h>

typedef struct ag_time_ops {
	const char *name;
	void   (*Init)(void);
	void   (*Destroy)(void);
	Uint32 (*GetTicks)(void);
	void   (*Delay)(Uint32);
} AG_TimeOps;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC const AG_TimeOps *agTimeOps;
extern DECLSPEC const AG_TimeOps agTimeOps_dummy;
extern DECLSPEC const AG_TimeOps agTimeOps_gettimeofday;
extern DECLSPEC const AG_TimeOps agTimeOps_win32;
extern DECLSPEC const AG_TimeOps agTimeOps_condwait;
extern DECLSPEC void AG_SetTimeOps(const AG_TimeOps *);
#define AG_GetTicks agTimeOps->GetTicks
#define AG_Delay(ms) agTimeOps->Delay(ms)
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
