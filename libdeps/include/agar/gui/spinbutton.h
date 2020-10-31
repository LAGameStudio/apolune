/*	Public domain	*/

#ifndef _AGAR_WIDGET_SPINBUTTON_H_
#define _AGAR_WIDGET_SPINBUTTON_H_

#include <agar/gui/widget.h>
#include <agar/gui/textbox.h>
#include <agar/gui/button.h>

#include <agar/gui/begin.h>

#define AG_SPINBUTTON_NOHFILL	0x01
#define AG_SPINBUTTON_VFILL	0x02

typedef struct ag_spinbutton {
	struct ag_widget wid;
	int value;			/* Default value binding */
	int min, max;			/* Default range bindings */
	int incr;			/* Increment for buttons */
	int writeable;			/* 0 = read-only */
	AG_Textbox *input;
	AG_Button *incbu;
	AG_Button *decbu;
} AG_Spinbutton;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agSpinbuttonClass;
extern DECLSPEC AG_Spinbutton *AG_SpinbuttonNew(void *, Uint, const char *);
extern DECLSPEC void AG_SpinbuttonAddValue(AG_Spinbutton *, int);
extern DECLSPEC void AG_SpinbuttonSetValue(AG_Spinbutton *, ...);
extern DECLSPEC void AG_SpinbuttonSetMin(AG_Spinbutton *, int);
extern DECLSPEC void AG_SpinbuttonSetMax(AG_Spinbutton *, int);
extern DECLSPEC void AG_SpinbuttonSetRange(AG_Spinbutton *, int, int);
extern DECLSPEC void AG_SpinbuttonSetIncrement(AG_Spinbutton *, int);
extern DECLSPEC void AG_SpinbuttonSetWriteable(AG_Spinbutton *, int);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_SPINBUTTON_H_ */
