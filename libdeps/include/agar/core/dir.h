/*	Public domain	*/

#ifndef _AGAR_CORE_DIR_H_
#define _AGAR_CORE_DIR_H_
#include <agar/core/begin.h>

typedef struct ag_dir {
	char **ents;
	int nents;
} AG_Dir;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int AG_MkDir(const char *);
extern DECLSPEC int AG_RmDir(const char *);
extern DECLSPEC int AG_ChDir(const char *);
extern DECLSPEC AG_Dir *AG_OpenDir(const char *);
extern DECLSPEC void AG_CloseDir(AG_Dir *);
extern DECLSPEC int AG_MkPath(const char *);
extern DECLSPEC int AG_GetCWD(char *, size_t);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_DIR_H_ */
