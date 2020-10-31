/*	Public domain	*/

#ifndef _AGAR_CORE_NET_SERVER_H_
#define _AGAR_CORE_NET_SERVER_H_
#include <agar/core/begin.h>

#define NS_HOSTNAME_MAX	256

struct ns_server;
struct ns_client;
struct ns_cmd;

enum ns_log_lvl {
	NS_DEBUG,
	NS_INFO,
	NS_NOTICE,
	NS_WARNING,
	NS_ERR,
	NS_CRIT,
	NS_ALERT,
	NS_EMERG
};

typedef int (*NS_LoginFn)(struct ns_server *, void *);
typedef void (*NS_LogoutFn)(struct ns_server *, void *);
typedef void (*NS_SigCheckFn)(struct ns_server *);
typedef int (*NS_ErrorFn)(struct ns_server *);
typedef int (*NS_AuthFn)(struct ns_server *, void *);
typedef int (*NS_CommandFn)(struct ns_server *, NS_Command *, void *);

typedef struct ns_cmd {
	const char *name;
	NS_CommandFn fn;
	void *arg;
} NS_Cmd;

typedef struct ns_auth {
	const char *name;
	NS_AuthFn fn;
	void *arg;
} NS_Auth;

typedef struct ns_server {
	struct ag_object obj;

	Uint flags;

	const char *protoName;		/* Daemon name / protocol signature */
	const char *protoVer;		/* Protocol version string */
	const char *host;		/* Hostname for bind(), or NULL */
	const char *port;		/* Port number or name */

	pid_t listenProc;		/* PID of listening process */

	NS_Cmd	*cmds;			/* Implemented commands */
	Uint	ncmds;
	NS_Auth *authModes;		/* Authentication methods */
	Uint	nAuthModes;

	NS_ErrorFn errorFn;		/* Call when any command fails, instead
					   of returning error to client. */
	NS_SigCheckFn sigCheckFn;	/* Call upon signal interruptions */
	NS_LoginFn loginFn;		/* Call after basic auth */
	NS_LogoutFn logoutFn;		/* Call on disconnection */

	void  **listItems;		/* For list functions */
	size_t *listItemSize;
	Uint	listItemCount;

	AG_TAILQ_HEAD_(ns_client) clients;	/* Connected clients */
} NS_Server;

typedef struct ns_client {
	struct ag_object obj;
	char host[NS_HOSTNAME_MAX];	/* Remote host */
} NS_Client;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_ObjectClass nsServerClass;
extern DECLSPEC AG_ObjectClass nsClientClass;
extern DECLSPEC void NS_InitSubsystem(Uint);
extern DECLSPEC NS_Server *NS_ServerNew(void *, Uint, const char *, const char *, const char *, const char *);
extern DECLSPEC void NS_ServerSetProtocol(NS_Server *, const char *, const char *);
extern DECLSPEC void NS_ServerBind(NS_Server *, const char *, const char *);
extern DECLSPEC void NS_Log(enum ns_log_lvl, const char *, ...);
extern DECLSPEC void NS_RegErrorFn(NS_Server *, NS_ErrorFn);
extern DECLSPEC void NS_RegSigCheckFn(NS_Server *, NS_SigCheckFn);
extern DECLSPEC void NS_RegCmd(NS_Server *, const char *, NS_CommandFn, void *);
extern DECLSPEC void NS_RegAuthMode(NS_Server *, const char *, NS_AuthFn, void *);
extern DECLSPEC void NS_RegLoginFn(NS_Server *, NS_LoginFn);
extern DECLSPEC void NS_RegLogoutFn(NS_Server *, NS_LogoutFn);
extern DECLSPEC int NS_ServerLoop(NS_Server *);
extern DECLSPEC void NS_Logout(NS_Server *, int, const char *, ...);
extern DECLSPEC void NS_Message(NS_Server *, int, const char *, ...);
extern DECLSPEC void NS_BeginData(NS_Server *, size_t);
extern DECLSPEC size_t NS_Data(NS_Server *, char *, size_t);
extern DECLSPEC void NS_EndData(NS_Server *);
extern DECLSPEC void NS_BeginList(NS_Server *);
extern DECLSPEC void NS_EndList(NS_Server *);
extern DECLSPEC void NS_ListItem(NS_Server *, void *, size_t);
extern DECLSPEC void NS_ListString(NS_Server *, const char *, ...);
extern DECLSPEC int NS_Write(NS_Server *, int, const void *, size_t);
extern DECLSPEC int NS_Read(NS_Server *, int, void *, size_t);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
#endif /* _AGAR_CORE_NET_SERVER_H_ */
