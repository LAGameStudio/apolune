/*	Public domain	*/

#ifndef _AGAR_CORE_NET_CLIENT_H_
#define _AGAR_CORE_NET_CLIENT_H_
#include <agar/core/begin.h>

#define NC_HOSTNAME_MAX	256
#define NC_PORTNUM_MAX	16
#define NC_USERNAME_MAX	48
#define NC_PASSWORD_MAX	64

typedef struct nc_session {
	const char *name;
	char host[NC_HOSTNAME_MAX];
	char port[NC_PORTNUM_MAX];
	char user[NC_USERNAME_MAX];
	char pass[NC_PASSWORD_MAX];
	int sock;
	struct {
		char *buf;
		size_t len;
		size_t maxlen;
	} read;
	char server_proto[32];
	char client_proto[32];
} NC_Session;

typedef struct nc_result {
	unsigned int argc;
	char	**argv;
	size_t	 *argv_len;
} NC_Result;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void NC_InitSubsystem(Uint);
extern DECLSPEC void NC_Init(NC_Session *, const char *, const char *);
extern DECLSPEC void NC_Destroy(NC_Session *);
extern DECLSPEC int NC_Connect(NC_Session *, const char *, const char *, const char *, const char *);
extern DECLSPEC int NC_Reconnect(NC_Session *);
extern DECLSPEC void NC_Disconnect(NC_Session *);
extern DECLSPEC long NC_Read(NC_Session *, size_t);
extern DECLSPEC long NC_ReadBinary(NC_Session *, size_t);
extern DECLSPEC int NC_Write(NC_Session *, const char *, ...);
extern DECLSPEC NC_Result *NC_Query(NC_Session *, const char *, ...);
extern DECLSPEC NC_Result *NC_QueryBinary(NC_Session *, const char *, ...);
extern DECLSPEC void NC_FreeResult(NC_Result *);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_NET_CLIENT_H_ */
