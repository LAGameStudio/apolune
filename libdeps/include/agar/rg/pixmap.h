/*	Public domain	*/

#ifndef _AGAR_RG_PIXMAP_H_
#define _AGAR_RG_PIXMAP_H_
#include <agar/rg/begin.h>

#define RG_PIXMAP_NAME_MAX	32

struct ag_window;
struct ag_toolbar;
struct rg_pixmap;
struct rg_tileview;

enum rg_pixmap_mod_type {
	RG_PIXMAP_PIXEL_REPLACE		/* Single pixel replace */
};

typedef struct rg_pixmap_mod {
	enum rg_pixmap_mod_type type;
	Uint16 x, y;			/* Coordinates of pixel in pixmap */
	Uint32 val;			/* Previous value */
} RG_PixmapMod;

typedef struct rg_pixmap_undoblk {
	struct rg_pixmap_mod *mods;	/* Undoable modifications */
	Uint		     nmods;
} RG_PixmapUndoBlk;

enum rg_pixmap_blend_mode {
	RG_PIXMAP_OVERLAY_ALPHA,        /* dA = sA+dA */
	RG_PIXMAP_AVERAGE_ALPHA,        /* dA = (sA+dA)/2 */
	RG_PIXMAP_DEST_ALPHA,           /* dA = dA */
	RG_PIXMAP_NO_BLENDING           /* No blending done */
};

enum rg_brush_type {
	RG_PIXMAP_BRUSH_MONO,  /* Monochromatic (use current color) */
	RG_PIXMAP_BRUSH_RGB    /* Replace by brush color */
};

typedef struct rg_brush {
	char name[RG_PIXMAP_NAME_MAX];
	enum rg_brush_type type;
	int flags;
#define RG_PIXMAP_BRUSH_ONESHOT 0x01	/* Don't mod the same pixel twice
					   in the same pass */
	char px_name[RG_PIXMAP_NAME_MAX];	/* Pixmap reference */
	struct rg_pixmap *px;				/* Resolved pixmap */
	AG_TAILQ_ENTRY(rg_brush) brushes;
} RG_Brush;

typedef struct rg_pixmap {
	char name[RG_PIXMAP_NAME_MAX];
	int flags;
	int xorig, yorig;		/* Pixmap origin point */
	struct rg_tileset *ts;		/* Back pointer to tileset */
	AG_Surface *su;			/* Pixmap surface */
	Uint nrefs;			/* Number of tile references */
	struct rg_pixmap_undoblk *ublks; /* Undo blocks */
	Uint nublks, curblk;

	float h, s, v, a;			/* Current pixel value */
	RG_Brush *curbrush;			/* Current brush */
	enum rg_pixmap_blend_mode blend_mode;	/* Current blending method */
	AG_TAILQ_HEAD_(rg_brush) brushes;	/* Brush references */
	AG_TAILQ_ENTRY(rg_pixmap) pixmaps;
} RG_Pixmap;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC RG_Pixmap *RG_PixmapNew(struct rg_tileset *, const char *, int);
extern DECLSPEC void RG_PixmapInit(RG_Pixmap *, struct rg_tileset *, int);
extern DECLSPEC void RG_PixmapDestroy(RG_Pixmap *);
extern DECLSPEC int RG_PixmapLoad(RG_Pixmap *, AG_DataSource *);
extern DECLSPEC void RG_PixmapSave(RG_Pixmap *, AG_DataSource *);
extern DECLSPEC void RG_PixmapScale(RG_Pixmap *, int, int, int, int);
extern DECLSPEC struct ag_window *RG_PixmapEdit(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC struct ag_toolbar *RG_PixmapToolbar(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC void RG_PixmapButtondown(struct rg_tileview *, RG_TileElement *, int, int, int);
extern DECLSPEC void RG_PixmapButtonup(struct rg_tileview *, RG_TileElement *, int, int, int);
extern DECLSPEC void RG_PixmapMotion(struct rg_tileview *, RG_TileElement *, int, int, int, int, int);
extern DECLSPEC int RG_PixmapWheel(struct rg_tileview *, RG_TileElement *, int);
extern DECLSPEC void RG_PixmapKeydown(struct rg_tileview *, int);
extern DECLSPEC void RG_PixmapKeyup(struct rg_tileview *);
extern DECLSPEC void RG_PixmapBeginUndoBlk(RG_Pixmap *);
extern DECLSPEC void RG_PixmapUndo(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC void RG_PixmapRedo(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC int RG_PixmapPutPixel(struct rg_tileview *, RG_TileElement *, int, int, Uint32, int);
extern DECLSPEC void RG_PixmapApplyBrush(struct rg_tileview *, RG_TileElement *, int, int, Uint32);
extern DECLSPEC Uint32 RG_PixmapSourcePixel(struct rg_tileview *, RG_TileElement *, int, int);
extern DECLSPEC void RG_PixmapSourceRGBA(struct rg_tileview *, RG_TileElement *, int, int, Uint8 *, Uint8 *, Uint8 *, Uint8 *);
extern DECLSPEC RG_Brush *RG_PixmapAddBrush(RG_Pixmap *, enum rg_brush_type, RG_Pixmap *);
extern DECLSPEC void RG_PixmapDelBrush(RG_Pixmap *, RG_Brush *);
extern DECLSPEC void RG_PixmapOpenMenu(struct rg_tileview *, int, int);
extern DECLSPEC void RG_PixmapCloseMenu(struct rg_tileview *);

static __inline__ void
RG_PixmapSetBlendingMode(RG_Pixmap *pixmap, enum rg_pixmap_blend_mode bmode)
{
	pixmap->blend_mode = bmode;
}

static __inline__ void
RG_PixmapSetBrush(RG_Pixmap *pixmap, RG_Brush *brush)
{
	pixmap->curbrush = brush;
}
__END_DECLS
/* Close generated block */

#include <agar/rg/close.h>
#endif	/* _AGAR_RG_PIXMAP_H_ */
