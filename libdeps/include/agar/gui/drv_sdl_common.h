/*	Public domain	*/
/*
 * Code common to all drivers using the SDL library.
 */

#include <agar/config/have_sdl.h>
#ifdef HAVE_SDL

/* XXX */
#undef HAVE_SNPRINTF
#undef HAVE_VSNPRINTF
#undef HAVE_SYS_TYPES_H
#undef HAVE_STDIO_H
#undef HAVE_STDLIB_H
#undef HAVE_STDARG_H
#undef Uint8
#undef Sint8
#undef Uint16
#undef Sint16
#undef Uint32
#undef Sint32
#undef Uint64
#undef Sint64
#include <SDL.h>

#include <agar/gui/begin.h>

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_PixelFormat *AG_SDL_GetPixelFormat(SDL_Surface *);
extern DECLSPEC void AG_SDL_BlitSurface(const AG_Surface *, const AG_Rect *, SDL_Surface *, int, int);
extern DECLSPEC AG_Surface *AG_SDL_ImportSurface(SDL_Surface *);
extern DECLSPEC int AG_SDL_SetRefreshRate(void *, int);
extern DECLSPEC int AG_SDL_InitDefaultCursor(void *);
extern DECLSPEC int AG_SDL_SetCursor(void *, AG_Cursor *);
extern DECLSPEC void AG_SDL_UnsetCursor(void *);
extern DECLSPEC int AG_SDL_CreateCursor(void *, AG_Cursor *);
extern DECLSPEC void AG_SDL_FreeCursor(void *, AG_Cursor *);
extern DECLSPEC int AG_SDL_GetCursorVisibility(void *);
extern DECLSPEC void AG_SDL_SetCursorVisibility(void *, int);
extern DECLSPEC int AG_SDL_PostEventCallback(void *);
extern DECLSPEC int AG_SDL_GetDisplaySize(Uint *, Uint *);
extern DECLSPEC void AG_SDL_BeginEventProcessing(void *);
extern DECLSPEC int AG_SDL_PendingEvents(void *);
extern DECLSPEC void AG_SDL_TranslateEvent(void *, const SDL_Event *, AG_DriverEvent *);
extern DECLSPEC int AG_SDL_GetNextEvent(void *, AG_DriverEvent *);
extern DECLSPEC int AG_SDL_ProcessEvent(void *, AG_DriverEvent *);
extern DECLSPEC void AG_SDL_GenericEventLoop(void *);
extern DECLSPEC void AG_SDL_EndEventProcessing(void *);
extern DECLSPEC void AG_SDL_Terminate(void);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* HAVE_SDL */
