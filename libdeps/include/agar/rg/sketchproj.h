/*	Public domain	*/

#ifndef _AGAR_RG_SKETCHPROJ_H_
#define _AGAR_RG_SKETCHPROJ_H_
#include <agar/rg/begin.h>

struct rg_sketchproj {
	RG_Feature ft;

	char sketch[RG_TILE_ELEMENT_NAME_MAX];
	Uint8 alpha;				/* Overall alpha */
	Uint32 color;				/* Line color */
};

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void RG_SketchProjInit(void *, RG_Tileset *, int);
extern DECLSPEC int RG_SketchProjLoad(void *, AG_DataSource *);
extern DECLSPEC void RG_SketchProjSave(void *, AG_DataSource *);
extern DECLSPEC void RG_SketchProjApply(void *, RG_Tile *, int, int);
extern DECLSPEC AG_Window *RG_SketchProjEdit(void *, RG_Tileview *);
__END_DECLS
/* Close generated block */

#include <agar/rg/close.h>
#endif /* _AGAR_RG_SKETCHPROJ_H_ */
