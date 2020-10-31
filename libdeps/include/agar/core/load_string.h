/*	Public domain	*/

#ifndef	_AGAR_CORE_LOAD_STRING_H_
#define	_AGAR_CORE_LOAD_STRING_H_

#define AG_LOAD_STRING_MAX 32767

#include <agar/core/begin.h>

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC char *AG_ReadStringLen(AG_DataSource *, size_t);
extern DECLSPEC int AG_ReadStringLenv(AG_DataSource *, size_t, char **);
#define AG_ReadString(nb) AG_ReadStringLen((nb),AG_LOAD_STRING_MAX)
#define AG_ReadStringv(nb,s) AG_ReadStringLenv((nb),AG_LOAD_STRING_MAX,(s))
extern DECLSPEC void AG_WriteString(AG_DataSource *, const char *);
extern DECLSPEC int AG_WriteStringv(AG_DataSource *, const char *);
extern DECLSPEC size_t AG_CopyString(char *, AG_DataSource *, size_t) BOUNDED_ATTRIBUTE(__string__, 1, 3);
extern DECLSPEC void AG_SkipString(AG_DataSource *);
extern DECLSPEC char *AG_ReadNulStringLen(AG_DataSource *, size_t);
extern DECLSPEC size_t AG_CopyNulString(char *, AG_DataSource *, size_t);
#define AG_ReadNulString(nb) AG_ReadNulStringLen((nb),AG_LOAD_STRING_MAX)
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_LOAD_STRING_H_ */
