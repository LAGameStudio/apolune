/*	Public domain	*/

#ifndef	_AGAR_CORE_LOAD_VERSION_H_
#define	_AGAR_CORE_LOAD_VERSION_H_
#include <agar/core/begin.h>

typedef struct ag_version {
	Uint32	 major;
	Uint32	 minor;
} AG_Version;

#define AG_VERSION_NAME_MAX	48
#define AG_VERSION_MAX		(AG_VERSION_NAME_MAX+8)

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int AG_ReadVersion(AG_DataSource *, const char *, const AG_Version *, AG_Version *);
extern DECLSPEC int AG_WriteVersion(AG_DataSource *, const char *, const AG_Version *);
extern DECLSPEC int AG_ReadObjectVersion(AG_DataSource *, void *, AG_Version *);
extern DECLSPEC void AG_WriteObjectVersion(AG_DataSource *, void *);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif	/* _AGAR_CORE_LOAD_VERSION_H_ */
