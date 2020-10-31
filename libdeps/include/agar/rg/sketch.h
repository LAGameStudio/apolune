/*	Public domain	*/

#ifndef _AGAR_RG_SKETCH_H_
#define _AGAR_RG_SKETCH_H_
#include <agar/rg/begin.h>

#define RG_SKETCH_NAME_MAX	32

struct ag_toolbar;
struct ag_window;

enum rg_sketch_mod_type {
	RG_SKETCH_VERTEX_DISPLACEMENT
};
typedef struct rg_sketch_mod {
	enum rg_sketch_mod_type type;
	VG_Vtx *vtx;			/* Modified vertex */
	VG_Vtx pvtx;			/* Previous value */
} RG_SketchMod;

typedef struct rg_sketch_undoblk {
	struct rg_sketch_mod *mods;	/* Undoable modifications */
	Uint		     nmods;
} RG_SketchUndoBlk;

typedef struct rg_sketch {
	char name[RG_SKETCH_NAME_MAX];
	int flags;
	struct rg_tileset *ts;
	VG *vg;
	Uint nrefs;
	float h, s, v, a;
	struct rg_sketch_undoblk *ublks;
	Uint nublks, curblk;
	AG_TAILQ_ENTRY(rg_sketch) sketches;
} RG_Sketch;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void RG_SketchInit(RG_Sketch *, struct rg_tileset *, int);
extern DECLSPEC void RG_SketchDestroy(RG_Sketch *);
extern DECLSPEC int RG_SketchLoad(RG_Sketch *, AG_DataSource *);
extern DECLSPEC void RG_SketchSave(RG_Sketch *, AG_DataSource *);
extern DECLSPEC void RG_SketchScale(RG_Sketch *, int, int, float, int, int);
extern DECLSPEC void RG_SketchRender(RG_Tile *, RG_TileElement *);
extern DECLSPEC struct ag_toolbar *RG_SketchToolbar(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC struct ag_window *RG_SketchEdit(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC struct ag_window *RG_SketchEditElement(struct rg_tileview *, RG_TileElement *, VG_Element *);
extern DECLSPEC struct ag_window *RG_SketchSelect(struct rg_tileview *, RG_TileElement *, VG_Element *);
extern DECLSPEC void RG_SketchUnselect(struct rg_tileview *, RG_TileElement *, VG_Element *);
extern DECLSPEC void RG_SketchKeyDown(struct rg_tileview *, RG_TileElement *, int, int);
extern DECLSPEC void RG_SketchKeyUp(struct rg_tileview *, RG_TileElement *, int, int);
extern DECLSPEC int RG_SketchWheel(struct rg_tileview *, RG_TileElement *, int);
extern DECLSPEC void RG_SketchButtondown(struct rg_tileview *, RG_TileElement *, float, float, int);
extern DECLSPEC void RG_SketchButtonup(struct rg_tileview *, RG_TileElement *, float, float, int);
extern DECLSPEC void RG_SketchMotion(struct rg_tileview *, RG_TileElement *, float, float, float, float, int);
extern DECLSPEC void RG_SketchBeginUndoBlk(RG_Sketch *);
extern DECLSPEC void RG_SketchUndo(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC void RG_SketchRedo(struct rg_tileview *, RG_TileElement *);
extern DECLSPEC void RG_SketchOpenMenu(struct rg_tileview *, int, int);
extern DECLSPEC void RG_SketchCloseMenu(struct rg_tileview *);
extern DECLSPEC void RG_SketchDrawPolygon(RG_Tile *, VG *, VG_Element *);
__END_DECLS
/* Close generated block */

#include <agar/rg/close.h>
#endif	/* _AGAR_RG_SKETCH_H_ */
