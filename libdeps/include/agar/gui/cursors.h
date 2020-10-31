/*	Public domain	*/

#ifndef _AGAR_GUI_CURSORS_H_
#define _AGAR_GUI_CURSORS_H_

#include <agar/gui/drv.h>

#include <agar/gui/begin.h>

#define AG_CURSOR_MAX_W 32
#define AG_CURSOR_MAX_H 32

/* Built-in cursors */
enum {
	AG_DEFAULT_CURSOR,
	AG_FILL_CURSOR,
	AG_ERASE_CURSOR,
	AG_PICK_CURSOR,
	AG_HRESIZE_CURSOR,
	AG_VRESIZE_CURSOR,
	AG_LRDIAG_CURSOR,
	AG_LLDIAG_CURSOR,
	AG_TEXT_CURSOR,
	AG_LAST_CURSOR
};

struct ag_driver;

typedef struct ag_cursor {
	Uint w, h;			/* Cursor dimensions */
	Uint8 *data, *mask;		/* Bitmap data */
	int  xHot, yHot;		/* Hotspot */
	void *p;			/* Driver data */
} AG_Cursor;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int AG_InitStockCursors(struct ag_driver *);
extern DECLSPEC void AG_FreeCursors(struct ag_driver *);
extern DECLSPEC AG_Cursor *AG_CursorNew(void *, Uint, Uint, const Uint8 *, const Uint8 *, int, int);
extern DECLSPEC AG_Cursor *AG_CursorFromXPM(void *, char *[], int, int);
extern DECLSPEC void AG_CursorFree(void *, AG_Cursor *);

static __inline__ void
AG_CursorInit(AG_Cursor *ac)
{
	ac->data = NULL;
	ac->mask = NULL;
	ac->w = 0;
	ac->h = 0;
	ac->xHot = 0;
	ac->yHot = 0;
	ac->p = NULL;
}

static __inline__ AG_Cursor *
AG_GetStockCursor(void *drv, int name)
{
#ifdef AG_DEBUG
	if (name < 0 || name >= AG_LAST_CURSOR)
		AG_FatalError("No such stock cursor");
#endif
	return (&AGDRIVER(drv)->cursors[name]);
}

static __inline__ AG_Cursor *
AG_GetActiveCursor(void *drv)
{
	return (AGDRIVER(drv)->activeCursor);
}

static __inline__ int
AG_CursorIsVisible(void *drv)
{
	return AGDRIVER_CLASS(drv)->getCursorVisibility(drv);
}

static __inline__ void
AG_ShowCursor(void *drv)
{
	AGDRIVER_CLASS(drv)->setCursorVisibility(drv, 1);
}

static __inline__ void
AG_HideCursor(void *drv)
{
	AGDRIVER_CLASS(drv)->setCursorVisibility(drv, 0);
}
#ifdef AG_LEGACY
extern DECLSPEC void AG_SetCursor(int) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_UnsetCursor(void) DEPRECATED_ATTRIBUTE;
#endif
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif	/* _AGAR_GUI_CURSORS_H_ */
