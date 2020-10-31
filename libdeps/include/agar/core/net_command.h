/*	Public domain	*/

#include <agar/core/begin.h>

typedef struct ns_command_arg {
	char	 key[32];
	void	*value;
	size_t	 size;
} NS_CommandArg, NC_CommandArg;

typedef struct ns_command {
	char		 name[64];
	NS_CommandArg	*args;
	int		nargs;
} NS_Command, NC_Command;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void NS_InitCommand(NS_Command *);
extern DECLSPEC void NS_DestroyCommand(NS_Command *);
extern DECLSPEC char *NS_CommandString(NS_Command *, const char *);
extern DECLSPEC int NS_CommandInt(NS_Command *, const char *, int *);
extern DECLSPEC int NS_CommandLong(NS_Command *, const char *, long *);
extern DECLSPEC void NS_CommandCopyString(char *, NS_Command *, const char *, size_t);
#define NC_InitCommand NS_InitCommand
#define NC_DestroyCommand NS_DestroyCommand
#define NC_CommandString NS_CommandString
#define NC_CommandInt NS_CommandInt
#define NC_CommandLong NS_CommandLong
#define NC_CommandCopyString NS_CommandCopyString
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
