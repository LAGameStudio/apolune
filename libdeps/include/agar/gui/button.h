/*	Public domain	*/

#ifndef _AGAR_WIDGET_BUTTON_H_
#define _AGAR_WIDGET_BUTTON_H_

#include <agar/gui/widget.h>
#include <agar/gui/label.h>

#include <agar/gui/begin.h>

typedef struct ag_button {
	struct ag_widget wid;
	int state;			/* Default state binding */
	AG_Label *lbl;			/* Text label */
	int surface;			/* Icon surface handle */
	enum ag_text_justify justify;	/* Label justification */
	enum ag_text_valign valign;	/* Vertical alignment */
	Uint flags;
#define AG_BUTTON_STICKY	0x002	/* Toggle state */
#define AG_BUTTON_MOUSEOVER	0x004	/* Mouse overlaps */
#define AG_BUTTON_REPEAT	0x008	/* Repeat button-pushed event */
#define AG_BUTTON_HFILL		0x010	/* Fill available width */
#define AG_BUTTON_VFILL		0x020	/* Fill available height */
#define AG_BUTTON_INVSTATE	0x400	/* Invert value of "state" binding */
#define AG_BUTTON_KEYDOWN	0x800	/* Got `key-down' before `key-up' */
#define AG_BUTTON_EXPAND	(AG_BUTTON_HFILL|AG_BUTTON_VFILL)

	int lPad, rPad, tPad, bPad;	/* Padding in pixels */
	AG_Timeout delay_to;		/* Delay for triggering repeat mode */
	AG_Timeout repeat_to;		/* Timeout for repeat mode */
} AG_Button;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agButtonClass;
extern DECLSPEC AG_Button *AG_ButtonNew(void *, Uint, const char *, ...) FORMAT_ATTRIBUTE(printf, 3, 4);
extern DECLSPEC AG_Button *AG_ButtonNewS(void *, Uint, const char *);
extern DECLSPEC AG_Button *AG_ButtonNewFn(void *, Uint, const char *, AG_EventFn, const char *, ...);
extern DECLSPEC AG_Button *AG_ButtonNewInt(void *, Uint, const char *, int *);
extern DECLSPEC AG_Button *AG_ButtonNewUint8(void *, Uint, const char *, Uint8 *);
extern DECLSPEC AG_Button *AG_ButtonNewUint16(void *, Uint, const char *, Uint16 *);
extern DECLSPEC AG_Button *AG_ButtonNewUint32(void *, Uint, const char *, Uint32 *);
extern DECLSPEC AG_Button *AG_ButtonNewFlag(void *, Uint, const char *, Uint *, Uint);
extern DECLSPEC AG_Button *AG_ButtonNewFlag8(void *, Uint, const char *, Uint8 *, Uint8);
extern DECLSPEC AG_Button *AG_ButtonNewFlag16(void *, Uint, const char *, Uint16 *, Uint16);
extern DECLSPEC AG_Button *AG_ButtonNewFlag32(void *, Uint, const char *, Uint32 *, Uint32);
extern DECLSPEC void AG_ButtonSetPadding(AG_Button *, int, int, int, int);
#define AG_ButtonSetPaddingLeft(b,v) AG_ButtonSetPadding((b),(v),-1,-1,-1)
#define AG_ButtonSetPaddingRight(b,v) AG_ButtonSetPadding((b),-1,(v),-1,-1)
#define AG_ButtonSetPaddingTop(b,v) AG_ButtonSetPadding((b),-1,-1,(v),-1)
#define AG_ButtonSetPaddingBottom(b,v) AG_ButtonSetPadding((b),-1,-1,-1,(v))
extern DECLSPEC void AG_ButtonSetFocusable(AG_Button *, int);
extern DECLSPEC void AG_ButtonSetSticky(AG_Button *, int);
extern DECLSPEC void AG_ButtonInvertState(AG_Button *, int);
extern DECLSPEC void AG_ButtonJustify(AG_Button *, enum ag_text_justify);
extern DECLSPEC void AG_ButtonValign(AG_Button *, enum ag_text_valign);
extern DECLSPEC void AG_ButtonSurface(AG_Button *, AG_Surface *);
extern DECLSPEC void AG_ButtonSurfaceNODUP(AG_Button *, AG_Surface *);
extern DECLSPEC void AG_ButtonSetRepeatMode(AG_Button *, int);
extern DECLSPEC void AG_ButtonText(AG_Button *, const char *, ...) FORMAT_ATTRIBUTE(printf, 2, 3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_ButtonTextS(AG_Button *, const char *);
#ifdef AG_LEGACY
# define AG_ButtonAct AG_ButtonNewFn
# define AG_ButtonSetSurface(bu,su) AG_ButtonSurface((bu),(su))
# define AG_ButtonPrintf AG_ButtonText
# define AG_ButtonEnable AG_WidgetEnable
# define AG_ButtonDisable AG_WidgetDisable
# define AG_ButtonSetJustification AG_ButtonJustify
#endif 
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_BUTTON_H_ */
