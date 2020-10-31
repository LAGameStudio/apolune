/*	Public domain	*/

#ifndef _AGAR_WIDGET_NOTEBOOK_H_
#define _AGAR_WIDGET_NOTEBOOK_H_

#include <agar/gui/widget.h>
#include <agar/gui/box.h>
#include <agar/gui/text.h>

#define AG_NOTEBOOK_LABEL_MAX	64

#include <agar/gui/begin.h>

enum ag_notebook_tab_alignment {
	AG_NOTEBOOK_TABS_TOP,
	AG_NOTEBOOK_TABS_BOTTOM,
	AG_NOTEBOOK_TABS_LEFT,
	AG_NOTEBOOK_TABS_RIGHT
};

typedef struct ag_notebook_tab {
	struct ag_box box;
	int label;				/* Label surface mapping */
	char labelText[AG_NOTEBOOK_LABEL_MAX];	/* Label source text */
	AG_TAILQ_ENTRY(ag_notebook_tab) tabs;
} AG_NotebookTab;

typedef struct ag_notebook {
	struct ag_widget wid;
	enum ag_notebook_tab_alignment tab_align;
	int flags;
#define AG_NOTEBOOK_HFILL 	0x01	/* Expand to fill available width */
#define AG_NOTEBOOK_VFILL 	0x02	/* Expand to fill available height */
#define AG_NOTEBOOK_HIDE_TABS	0x04	/* Don't display the tabs. */
#define AG_NOTEBOOK_EXPAND	(AG_NOTEBOOK_HFILL|AG_NOTEBOOK_VFILL)
	int bar_w, bar_h;		/* Dimensions of tab button bar */
	int cont_w, cont_h;		/* Dimensions of largest container */
	int spacing, padding;		/* Spacing for tabs */
	AG_Font *tabFont;		/* Font for tab labels */
	int lblPartial;			/* "..." label */
	int lblPartialWidth;		/* Width of "..." */
	struct ag_notebook_tab *sel_tab;
	AG_TAILQ_HEAD_(ag_notebook_tab) tabs;
	AG_Rect r;			/* View area */
} AG_Notebook;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agNotebookClass;
extern DECLSPEC AG_WidgetClass agNotebookTabClass;
extern DECLSPEC AG_Notebook *AG_NotebookNew(void *, Uint);
extern DECLSPEC void AG_NotebookSetPadding(AG_Notebook *, int);
extern DECLSPEC void AG_NotebookSetSpacing(AG_Notebook *, int);
extern DECLSPEC void AG_NotebookSetTabAlignment(AG_Notebook *, enum ag_notebook_tab_alignment);
extern DECLSPEC void AG_NotebookSetTabFont(AG_Notebook *, AG_Font *);
extern DECLSPEC void AG_NotebookSetTabVisibility(AG_Notebook *, int);
extern DECLSPEC AG_NotebookTab *AG_NotebookAddTab(AG_Notebook *, const char *, enum ag_box_type);
#define AG_NotebookAddTabVert(nb,lbl) AG_NotebookAddTab((nb),(lbl),AG_BOX_VERT)
#define AG_NotebookAddTabHoriz(nb,lbl) AG_NotebookAddTab((nb),(lbl),AG_BOX_HORIZ)
extern DECLSPEC void AG_NotebookDelTab(AG_Notebook *, AG_NotebookTab *);
extern DECLSPEC void AG_NotebookSelectTab(AG_Notebook *, AG_NotebookTab *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_NOTEBOOK_H_ */
