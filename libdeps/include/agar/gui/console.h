/*	Public domain	*/

#ifndef _AGAR_WIDGET_CONSOLE_H_
#define _AGAR_WIDGET_CONSOLE_H_

#include <agar/gui/widget.h>
#include <agar/gui/scrollbar.h>
#include <agar/gui/text.h>

#include <agar/gui/begin.h>

#define AG_CONSOLE_LINE_MAX	1024

struct ag_console;

typedef struct ag_console_line {
	char *text;			/* Line text */
	size_t len;			/* Length not including NUL */
	int surface;			/* Cached surface handle (or -1) */
	int selected;			/* Row is selected */
	int icon;			/* Icon to display */
	AG_Color cFg;			/* Foreground color (display fmt) */
	AG_Color cBg;			/* Background color (display fmt) */
	void *p;			/* User pointer */
	struct ag_console *cons;	/* Back pointer to Console */
} AG_ConsoleLine;

typedef struct ag_console {
	struct ag_widget wid;
	Uint flags;
#define AG_CONSOLE_HFILL	0x01	/* Fill available width */
#define AG_CONSOLE_VFILL	0x02	/* Fill available height */
#define AG_CONSOLE_AUTOSCROLL	0x04	/* Scroll new lines are added */
#define AG_CONSOLE_EXPAND	(AG_CONSOLE_HFILL|AG_CONSOLE_VFILL)
	int padding;			/* Padding in pixels */
	int lineskip;			/* Space between lines */
	AG_ConsoleLine *lines;		/* Lines in buffer */
	int nLines;			/* Line count */
	int rOffs;			/* Row display offset */
	AG_Color cBg;			/* Background color */
	AG_Scrollbar *vBar;		/* Scrollbar */
	AG_Rect r;			/* View area */
	int rVisible;
	AG_Font *font;			/* Font */
} AG_Console;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agConsoleClass;
extern DECLSPEC AG_Console *AG_ConsoleNew(void *, Uint);
extern DECLSPEC void AG_ConsoleSetPadding(AG_Console *, int);
extern DECLSPEC void AG_ConsoleSetFont(AG_Console *, AG_Font *);
extern DECLSPEC AG_ConsoleLine *AG_ConsoleAppendLine(AG_Console *, const char *);
extern DECLSPEC AG_ConsoleLine *AG_ConsoleMsg(AG_Console *, const char *, ...) FORMAT_ATTRIBUTE(printf, 2, 3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC AG_ConsoleLine *AG_ConsoleMsgS(AG_Console *, const char *);
extern DECLSPEC void AG_ConsoleMsgPtr(AG_ConsoleLine *, void *);
extern DECLSPEC void AG_ConsoleMsgIcon(AG_ConsoleLine *, int);
extern DECLSPEC void AG_ConsoleClear(AG_Console *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_CONSOLE_H_ */
