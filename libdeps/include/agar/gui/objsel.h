/*	Public domain	*/

#ifndef _AGAR_WIDGET_OBJSEL_H_
#define _AGAR_WIDGET_OBJSEL_H_

#include <agar/gui/widget.h>
#include <agar/gui/combo.h>

#include <agar/gui/begin.h>

typedef struct ag_object_selector {
	struct ag_combo com;
	int flags;
#define AG_OBJSEL_PAGE_DATA	0x01
#define AG_OBJSEL_PAGE_GFX	0x02
	char type_mask[AG_OBJECT_HIER_MAX];
	void *pobj;
	void *root;
	void *object;
} AG_ObjectSelector;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agObjectSelectorClass;
extern DECLSPEC AG_ObjectSelector *AG_ObjectSelectorNew(void *, int, void *, void *, const char *, ...) FORMAT_ATTRIBUTE(printf, 5, 6) NONNULL_ATTRIBUTE(5);
extern DECLSPEC AG_TlistItem *AG_ObjectSelectorSelect(AG_ObjectSelector *, void *);
extern DECLSPEC void AG_ObjectSelectorMaskType(AG_ObjectSelector *, const char *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_OBJSEL_H_ */
