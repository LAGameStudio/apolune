/*	Public domain	*/

#ifndef _AGAR_WIDGET_MFSPINBUTTON_H_
#define _AGAR_WIDGET_MFSPINBUTTON_H_

#include <agar/gui/widget.h>
#include <agar/gui/textbox.h>
#include <agar/gui/button.h>
#include <agar/gui/ucombo.h>
#include <agar/gui/units.h>

#include <agar/gui/begin.h>

#define AG_MFSPINBUTTON_NOHFILL	0x01
#define AG_MFSPINBUTTON_VFILL	0x02

typedef struct ag_mfspinbutton {
	struct ag_widget wid;
	double xvalue, yvalue;		/* Default x/y value bindings */
	double min, max;		/* Default range bindings */
	double inc;			/* Increment for buttons */
	char format[32];		/* Printing format */
	const char *sep;		/* x/y field separator */
	const AG_Unit *unit;		/* Conversion unit */
	int writeable;			/* 0 = read-only */
	AG_Textbox *input;
	AG_UCombo *units;
	AG_Button *xincbu, *xdecbu;
	AG_Button *yincbu, *ydecbu;
} AG_MFSpinbutton;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agMFSpinbuttonClass;
extern DECLSPEC AG_MFSpinbutton *AG_MFSpinbuttonNew(void *, Uint, const char *, const char *, const char *);
extern DECLSPEC void AG_MFSpinbuttonSetValue(AG_MFSpinbutton *, const char *, double);
extern DECLSPEC void AG_MFSpinbuttonAddValue(AG_MFSpinbutton *, const char *, double);
extern DECLSPEC void AG_MFSpinbuttonSetMin(AG_MFSpinbutton *, double);
extern DECLSPEC void AG_MFSpinbuttonSetMax(AG_MFSpinbutton *, double);
extern DECLSPEC void AG_MFSpinbuttonSetRange(AG_MFSpinbutton *, double, double);
extern DECLSPEC void AG_MFSpinbuttonSetIncrement(AG_MFSpinbutton *, double);
extern DECLSPEC void AG_MFSpinbuttonSelectUnit(AG_MFSpinbutton *, const char *);
extern DECLSPEC void AG_MFSpinbuttonSetPrecision(AG_MFSpinbutton *, const char *, int);
extern DECLSPEC void AG_MFSpinbuttonSetWriteable(AG_MFSpinbutton *, int);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_MFSPINBUTTON_H_ */
