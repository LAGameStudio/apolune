/*	Public domain	*/

#ifndef _AGAR_WIDGET_TOOLBAR_H_
#define _AGAR_WIDGET_TOOLBAR_H_

#include <agar/gui/widget.h>
#include <agar/gui/box.h>
#include <agar/gui/button.h>

#include <agar/gui/begin.h>

#define AG_TOOLBAR_MAX_ROWS	8

enum ag_toolbar_type {
	AG_TOOLBAR_HORIZ,
	AG_TOOLBAR_VERT
};

typedef struct ag_toolbar {
	struct ag_box box;
	struct ag_box *rows[AG_TOOLBAR_MAX_ROWS];
	enum ag_toolbar_type type;
	int nRows;			/* Number of rows */
	int nButtons;			/* Total number of buttons */
	int curRow;			/* Current row index */
	Uint flags;
#define AG_TOOLBAR_HOMOGENOUS	0x01	/* Scale buttons homogenously */
#define AG_TOOLBAR_STICKY	0x02	/* Single toggle selection */
#define AG_TOOLBAR_MULTI_STICKY	0x04	/* Multiple toggle selections */
#define AG_TOOLBAR_HFILL	0x08
#define AG_TOOLBAR_VFILL	0x10
#define AG_TOOLBAR_EXPAND	(AG_TOOLBAR_HFILL|AG_TOOLBAR_VFILL)
} AG_Toolbar;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agToolbarClass;
extern DECLSPEC AG_Toolbar *AG_ToolbarNew(void *, enum ag_toolbar_type, int, Uint);
extern DECLSPEC void AG_ToolbarInit(AG_Toolbar *, enum ag_toolbar_type, int, Uint);
extern DECLSPEC void AG_ToolbarScale(void *, int, int);
extern DECLSPEC void AG_ToolbarRow(AG_Toolbar *, int);
extern DECLSPEC AG_Button *AG_ToolbarButton(AG_Toolbar *, const char *, int, void (*)(AG_Event *), const char *, ...);
extern DECLSPEC AG_Button *AG_ToolbarButtonIcon(AG_Toolbar *, AG_Surface *, int, void (*)(AG_Event *), const char *, ...);
extern DECLSPEC void AG_ToolbarSeparator(AG_Toolbar *);
extern DECLSPEC void AG_ToolbarSelect(AG_Toolbar *, AG_Button *);
extern DECLSPEC void AG_ToolbarDeselect(AG_Toolbar *, AG_Button *);
extern DECLSPEC void AG_ToolbarSelectOnly(AG_Toolbar *, AG_Button *);
extern DECLSPEC void AG_ToolbarSelectAll(AG_Toolbar *);
extern DECLSPEC void AG_ToolbarDeselectAll(AG_Toolbar *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_TOOLBAR_H_ */
