/*	Public domain	*/

#ifndef _AGAR_WIDGET_GLVIEW_H_
#define _AGAR_WIDGET_GLVIEW_H_

#include <agar/gui/widget.h>
#include <agar/config/have_opengl.h>

#ifdef HAVE_OPENGL
#include <agar/gui/begin.h>

typedef struct ag_glview {
	struct ag_widget wid;
	Uint flags;
#define AG_GLVIEW_HFILL		0x01
#define AG_GLVIEW_VFILL		0x02
#define AG_GLVIEW_NOMODELVIEW	0x04	/* Don't preserve modelview matrix */
#define AG_GLVIEW_NOTEXTURE	0x08	/* Don't preserve texture matrix */
#define AG_GLVIEW_NOCOLOR	0x10	/* Don't preserve color matrix */
#define AG_GLVIEW_INIT_MATRICES	0x20	/* For initialization */
#define AG_GLVIEW_RESHAPE	0x40	/* Matrices have changed */
#define AG_GLVIEW_EXPAND	(AG_GLVIEW_HFILL|AG_GLVIEW_VFILL)

	int	  wPre, hPre;			/* Initial geometry */

	AG_Event *draw_ev;			/* Draw callback */
	AG_Event *overlay_ev;			/* Graphics overlay callback */
	AG_Event *scale_ev;			/* Scaling/movement event */
	AG_Event *keydown_ev, *keyup_ev;	/* Keyboard events */
	AG_Event *btndown_ev, *btnup_ev;	/* Mouse button events */
	AG_Event *motion_ev;			/* Mouse motion event */

	float mProjection[16];			/* Projection matrix to load */
	float mModelview[16];			/* Modelview matrix to load */
	float mTexture[16];			/* Texture matrix to load */
} AG_GLView;

#define AGGLVIEW(p) ((AG_GLView *)(p))

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agGLViewClass;
extern DECLSPEC AG_GLView *AG_GLViewNew(void *, Uint);
extern DECLSPEC void AG_GLViewDraw(void *);
extern DECLSPEC void AG_GLViewSizeRequest(void *, AG_SizeReq *);
extern DECLSPEC int AG_GLViewSizeAllocate(void *, const AG_SizeAlloc *);
extern DECLSPEC void AG_GLViewSizeHint(AG_GLView *, int, int);
#define AG_GLViewPrescale AG_GLViewSizeHint
extern DECLSPEC void AG_GLViewReshape(AG_GLView *);
extern DECLSPEC void AG_GLViewDrawFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewOverlayFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewScaleFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewKeydownFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewKeyupFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewButtondownFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewButtonupFn(AG_GLView *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_GLViewMotionFn(AG_GLView *, AG_EventFn, const char *, ...);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* HAVE_OPENGL */
#endif /* _AGAR_WIDGET_GLVIEW_H_ */
