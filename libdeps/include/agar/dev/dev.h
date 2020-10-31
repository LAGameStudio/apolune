/*	Public domain	*/

#ifndef _AGAR_DEV_DEV_H_
#define _AGAR_DEV_DEV_H_

#include <agar/config/ag_network.h>
#include <agar/config/ag_threads.h>
#include <agar/config/have_jpeg.h>

#include <agar/dev/begin.h>

struct ag_menu_item;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void DEV_InitSubsystem(Uint);
extern DECLSPEC void DEV_ToolMenu(struct ag_menu_item *);
#if defined(AG_NETWORK) && defined(AG_THREADS) && defined(HAVE_JPEG)
extern DECLSPEC AG_Window *DEV_ScreenshotUploader(void);
#endif
#if defined(AG_NETWORK) && defined(AG_THREADS)
extern DECLSPEC AG_Window *DEV_DebugServer(void);
extern DECLSPEC int DEV_DebugServerStart(void);
#endif
extern DECLSPEC AG_Window *DEV_TimerInspector(void);
extern DECLSPEC AG_Window *DEV_UnicodeBrowser(void);
extern DECLSPEC AG_Window *DEV_DisplaySettings(void);
extern DECLSPEC AG_Window *DEV_CPUInfo(void);
extern DECLSPEC AG_Window *DEV_Browser(void *);
extern DECLSPEC void DEV_BrowserInit(void *);
extern DECLSPEC void DEV_BrowserDestroy(void);
extern DECLSPEC void DEV_BrowserOpenData(void *);
extern DECLSPEC void DEV_BrowserCloseData(void *);
extern DECLSPEC void DEV_BrowserOpenGeneric(AG_Object *);
extern DECLSPEC void DEV_BrowserSaveTo(void *, const char *);
extern DECLSPEC void DEV_BrowserLoadFrom(void *, const char *);
extern DECLSPEC void DEV_BrowserGenericMenu(void *, void *);
extern DECLSPEC void DEV_ConfigShow(void);
extern DECLSPEC void *DEV_ObjectEdit(void *);
extern DECLSPEC AG_Window *DEV_ClassInfo(void);
__END_DECLS
/* Close generated block */

#include <agar/dev/close.h>
#endif	/* _AGAR_DEV_DEV_H_ */
