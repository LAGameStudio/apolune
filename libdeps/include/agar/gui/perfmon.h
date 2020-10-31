/*	Public domain	*/

#ifndef _AGAR_GUI_PERFMON_H_
#define _AGAR_GUI_PERFMON_H_
#include <agar/gui/begin.h>

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int agEventAvg; 
extern DECLSPEC int agIdleAvg; 
extern DECLSPEC AG_Window *agPerfWindow;
extern DECLSPEC AG_Window *AG_PerfMonShow(void);
extern DECLSPEC void AG_PerfMonInit(void);
extern DECLSPEC void AG_PerfMonUpdate(int);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_GUI_PERFMON_H_ */
