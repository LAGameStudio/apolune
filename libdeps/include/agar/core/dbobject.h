/*	Public domain	*/

#ifndef _AGAR_CORE_DBOBJECT_H_
#define _AGAR_CORE_DBOBJECT_H_
#include <agar/core/begin.h>

typedef struct ag_dbobject {
	struct ag_object obj;
} AG_DbObject;

#define AGDBOBJECT(p) ((AG_DbObject *)(p))

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_ObjectClass agDbObjectClass;
extern DECLSPEC AG_DbObject *AG_DbObjectNew(void);
extern DECLSPEC AG_List *AG_DbObjectList(AG_Db *);
extern DECLSPEC int AG_DbObjectLoad(void *, AG_Db *, const char *);
extern DECLSPEC int AG_DbObjectSave(void *, AG_Db *);
extern DECLSPEC int AG_DbObjectInsert(AG_Db *, void *);
extern DECLSPEC int AG_DbObjectDelete(AG_Db *, const char *);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_DBOBJECT_H_ */
