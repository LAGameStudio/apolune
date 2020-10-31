/*	Public domain	*/

/*
 * Single-window graphics driver framework. In this mode, the driver
 * creates a single "native" display context and Agar emulates a window
 * manager internally.
 */

typedef struct ag_driver_sw_class {
	struct ag_driver_class _inherit;
	Uint flags;
	/* Create or attach to a graphics display */
	int  (*openVideo)(void *drv, Uint w, Uint h, int depth, Uint flags);
	int  (*openVideoContext)(void *drv, void *ctx, Uint flags);
	void (*closeVideo)(void *drv);
	/* Resize the display */
	int  (*videoResize)(void *drv, Uint w, Uint h);
	/* Capture display contents to surface */
	int  (*videoCapture)(void *drv, AG_Surface **);
	/* Clear background */
	void (*videoClear)(void *drv, AG_Color c);
} AG_DriverSwClass;

struct ag_style;

/* General window alignment in view */
enum ag_window_alignment {
	AG_WINDOW_ALIGNMENT_NONE,
	AG_WINDOW_TL,
	AG_WINDOW_TC,
	AG_WINDOW_TR,
	AG_WINDOW_ML,
	AG_WINDOW_MC,
	AG_WINDOW_MR,
	AG_WINDOW_BL,
	AG_WINDOW_BC,
	AG_WINDOW_BR,
	AG_WINDOW_ALIGNMENT_LAST
};

/* Window manager operation */
enum ag_wm_operation {
	AG_WINOP_NONE,		/* No operation */
	AG_WINOP_MOVE,		/* Move window */
	AG_WINOP_LRESIZE,	/* Resize (via left control) */
	AG_WINOP_RRESIZE,	/* Resize (via right control) */
	AG_WINOP_HRESIZE	/* Resize (via horizontal control) */
};

/* Single-window driver instance */
typedef struct ag_driver_sw {
	struct ag_driver _inherit;
	Uint w, h, depth;		/* Video resolution */
	Uint flags;
#define AG_DRIVER_SW_OVERLAY	0x01	/* "Overlay" mode */
#define AG_DRIVER_SW_BGPOPUP	0x02	/* Enable generic background popup */

	struct ag_window *winSelected;	/* Window being moved/resized/etc */
	struct ag_window *winLastKeydown; /* For keyboard processing */
	AG_List *Lmodal;		/* Modal window stack */
	enum ag_wm_operation winop;	/* WM operation in progress */
	struct ag_style *style;		/* Default style for new windows */
	int windowXOutLimit;		/* Limit past left/right boundary */
	int windowBotOutLimit;		/* Limit past bottom boundary */
	int windowIconWidth;		/* Preferred window icon dimensions */
	int windowIconHeight;
	int windowCurX[AG_WINDOW_ALIGNMENT_LAST];	/* For cascading */
	int windowCurY[AG_WINDOW_ALIGNMENT_LAST];
	Uint rNom;			/* Nominal refresh rate (ms) */
	int rCur;			/* Effective refresh rate (ms) */
} AG_DriverSw;

#define AGDRIVER_SW(obj) ((AG_DriverSw *)(obj))
#define AGDRIVER_SW_CLASS(obj) ((struct ag_driver_sw_class *)(AGOBJECT(obj)->cls))

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_ObjectClass agDriverSwClass;
extern DECLSPEC AG_DriverSw *agDriverSw; 
 struct ag_size_alloc;
extern DECLSPEC void AG_WM_BackgroundPopupMenu(AG_DriverSw *);
extern DECLSPEC void AG_WM_CommitWindowFocus(struct ag_window *);
extern DECLSPEC int AG_ResizeDisplay(int, int);
extern DECLSPEC void AG_SetVideoResizeCallback(void (*)(Uint, Uint));
extern DECLSPEC void AG_WM_LimitWindowToView(struct ag_window *);
extern DECLSPEC void AG_WM_LimitWindowToDisplaySize(AG_Driver *, struct ag_size_alloc *);
extern DECLSPEC void AG_WM_GetPrefPosition(struct ag_window *, int *, int *, int, int);
extern DECLSPEC void AG_WM_MouseMotion(AG_DriverSw *, struct ag_window *, int, int);

static __inline__ void
AG_ClearBackground(void)
{
	if (agDriverSw != NULL) {
		AG_Color c = { 0,0,0,0 };
		AGDRIVER_SW_CLASS(agDriverSw)->videoClear(agDriverSw, c);
	}
}

static __inline__ int
AG_SetRefreshRate(int fps)
{
	if (agDriverOps->setRefreshRate == NULL) {
		AG_SetError("Refresh rate not applicable to graphics driver");
		return (-1);
	}
	return agDriverOps->setRefreshRate(agDriverSw, fps);
}

static __inline__ void
AG_EventLoop(void)
{
	if (agDriverSw != NULL) {
		AGDRIVER(agDriverSw)->flags &= ~(AG_DRIVER_FIXED_FPS);
	}
	agDriverOps->genericEventLoop(agDriverSw);
}

static __inline__ void
AG_EventLoop_FixedFPS(void)
{
	if (agDriverSw != NULL) {
		AGDRIVER(agDriverSw)->flags |= AG_DRIVER_FIXED_FPS;
	}
	agDriverOps->genericEventLoop(agDriverSw);
}

static __inline__ int
AG_PendingEvents(AG_Driver *drv)
{
	if (drv != NULL) {
		return AGDRIVER_CLASS(drv)->pendingEvents(drv);
	} else {
		return agDriverOps->pendingEvents(agDriverSw);
	}
}

static __inline__ int
AG_GetNextEvent(AG_Driver *drv, AG_DriverEvent *dev)
{
	if (drv != NULL) {
		return AGDRIVER_CLASS(drv)->getNextEvent(drv, dev);
	} else {
		return agDriverOps->getNextEvent(agDriverSw, dev);
	}
}

static __inline__ int
AG_ProcessEvent(AG_Driver *drv, AG_DriverEvent *dev)
{
	if (drv != NULL) {
		return AGDRIVER_CLASS(drv)->processEvent(drv, dev);
	} else {
		return agDriverOps->processEvent(agDriverSw, dev);
	}
}

static __inline__ void
AG_EventLoop_Drv(AG_Driver *drv)
{
	if (drv != NULL) {
		AGDRIVER_CLASS(drv)->genericEventLoop(drv);
	} else {
		agDriverOps->genericEventLoop(agDriverSw);
	}
}
#ifdef AG_LEGACY

static __inline__ void
AG_ViewUpdateFB(const AG_Rect2 *r2)
{
	AG_Rect r;
	r.x = r2->x1;
	r.y = r2->y1;
	r.w = r2->w;
	r.h = r2->h;
	if (agDriverOps->updateRegion != NULL)
		agDriverOps->updateRegion(agDriverSw, r);
}
#endif 
__END_DECLS
/* Close generated block */
